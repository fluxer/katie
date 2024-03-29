/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qplatformdefs.h"
#include "qdebug.h"
#include "qpdf_p.h"
#include "qfile.h"
#include "qtemporaryfile.h"
#include "qcups_p.h"
#include "qprinterinfo.h"
#include "qnumeric.h"
#include "qx11info_x11.h"
#include "qcore_unix_p.h"
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PRINTER

extern QSizeF qt_paperSizeToQSizeF(QPrinter::PaperSize size);

/* also adds a space at the end of the number */
static const char *qt_real_to_string(qreal val, char *buf)
{
    const char *ret = buf;

    if (qIsNaN(val)) {
        *(buf++) = '0';
        *(buf++) = ' ';
        *buf = 0;
        return ret;
    }

    if (val < 0) {
        *(buf++) = '-';
        val = -val;
    }
    unsigned int ival = (unsigned int) val;
    qreal frac = val - (qreal)ival;

    int ifrac = (int)(frac * 1000000000);
    if (ifrac == 1000000000) {
        ++ival;
        ifrac = 0;
    }
    QSTACKARRAY(char, output, 256);
    int i = 0;
    while (ival) {
        output[i] = '0' + (ival % 10);
        ++i;
        ival /= 10;
    }
    int fact = 100000000;
    if (i == 0) {
        *(buf++) = '0';
    } else {
        while (i) {
            *(buf++) = output[--i];
            fact /= 10;
            ifrac /= 10;
        }
    }

    if (ifrac) {
        *(buf++) =  '.';
        while (fact) {
            *(buf++) = '0' + ((ifrac/fact) % 10);
            fact /= 10;
        }
    }
    *(buf++) = ' ';
    *buf = 0;
    return ret;
}

static const char *qt_int_to_string(int val, char *buf)
{
    const char *ret = buf;
    if (val < 0) {
        *(buf++) = '-';
        val = -val;
    }
    QSTACKARRAY(char, output, 256);
    int i = 0;
    while (val) {
        output[i] = '0' + (val % 10);
        ++i;
        val /= 10;
    }
    if (i == 0) {
        *(buf++) = '0';
    } else {
        while (i)
            *(buf++) = output[--i];
    }
    *(buf++) = ' ';
    *buf = 0;
    return ret;
}


namespace QPdf {
    ByteStream::ByteStream(QByteArray *byteArray, bool fileBacking)
            : dev(new QBuffer(byteArray)),
            fileBackingEnabled(fileBacking),
            fileBackingActive(false),
            handleDirty(false)
    {
        dev->open(QIODevice::ReadWrite | QIODevice::Append);
    }

    ByteStream::ByteStream(bool fileBacking)
            : dev(new QBuffer(&ba)),
            fileBackingEnabled(fileBacking),
            fileBackingActive(false),
            handleDirty(false)
    {
        dev->open(QIODevice::ReadWrite);
    }

    ByteStream::~ByteStream()
    {
        delete dev;
    }

    ByteStream &ByteStream::operator <<(char chr)
    {
        if (handleDirty) prepareBuffer();
        dev->write(&chr, 1);
        return *this;
    }

    ByteStream &ByteStream::operator <<(const char *str)
    {
        if (handleDirty) prepareBuffer();
        dev->write(str, strlen(str));
        return *this;
    }

    ByteStream &ByteStream::operator <<(const QByteArray &str)
    {
        if (handleDirty) prepareBuffer();
        dev->write(str);
        return *this;
    }

    ByteStream &ByteStream::operator <<(const ByteStream &src)
    {
        Q_ASSERT(!src.dev->isSequential());
        if (handleDirty) prepareBuffer();
        // We do play nice here, even though it looks ugly.
        // We save the position and restore it afterwards.
        ByteStream &s = const_cast<ByteStream&>(src);
        qint64 pos = s.dev->pos();
        s.dev->reset();
        while (!s.dev->atEnd()) {
            QByteArray buf = s.dev->read(chunkSize());
            dev->write(buf);
        }
        s.dev->seek(pos);
        return *this;
    }

    ByteStream &ByteStream::operator <<(qreal val) {
        QSTACKARRAY(char, buf, 256);
        qt_real_to_string(val, buf);
        *this << buf;
        return *this;
    }

    ByteStream &ByteStream::operator <<(int val) {
        QSTACKARRAY(char, buf, 256);
        qt_int_to_string(val, buf);
        *this << buf;
        return *this;
    }

    ByteStream &ByteStream::operator <<(const QPointF &p) {
        QSTACKARRAY(char, buf, 256);
        qt_real_to_string(p.x(), buf);
        *this << buf;
        qt_real_to_string(p.y(), buf);
        *this << buf;
        return *this;
    }

    QIODevice *ByteStream::stream()
    {
        dev->reset();
        handleDirty = true;
        return dev;
    }

    void ByteStream::clear()
    {
        dev->open(QIODevice::ReadWrite | QIODevice::Truncate);
    }

    void ByteStream::prepareBuffer()
    {
        Q_ASSERT(!dev->isSequential());
        qint64 size = dev->size();
        if (fileBackingEnabled && !fileBackingActive
                && size > maxMemorySize()) {
            // Switch to file backing.
            QTemporaryFile *newFile = new QTemporaryFile;
            newFile->open();
            dev->reset();
            while (!dev->atEnd()) {
                QByteArray buf = dev->read(chunkSize());
                newFile->write(buf);
            }
            delete dev;
            dev = newFile;
            ba.clear();
            fileBackingActive = true;
        }
        if (dev->pos() != size) {
            dev->seek(size);
            handleDirty = false;
        }
    }
}

QPdfPage::QPdfPage()
    : QPdf::ByteStream(true) // Enable file backing
{
}

void QPdfPage::streamImage(int w, int h, int object)
{
    *this << w << "0 0 " << -h << "0 " << h << "cm /Im" << object << " Do\n";
    if (!images.contains(object))
        images.append(object);
}

QByteArray QPdf::generatePath(const QPainterPath &path, const QTransform &matrix, PathFlags flags)
{
    QByteArray result;
    if (!path.elementCount())
        return result;

    ByteStream s(&result);

    int start = -1;
    for (int i = 0; i < path.elementCount(); ++i) {
        const QPainterPath::Element &elm = path.elementAt(i);
        switch (elm.type) {
        case QPainterPath::MoveToElement:
            if (start >= 0
                && path.elementAt(start).x == path.elementAt(i-1).x
                && path.elementAt(start).y == path.elementAt(i-1).y)
                s << "h\n";
            s << matrix.map(QPointF(elm.x, elm.y)) << "m\n";
            start = i;
            break;
        case QPainterPath::LineToElement:
            s << matrix.map(QPointF(elm.x, elm.y)) << "l\n";
            break;
        case QPainterPath::CurveToElement:
            Q_ASSERT(path.elementAt(i+1).type == QPainterPath::CurveToDataElement);
            Q_ASSERT(path.elementAt(i+2).type == QPainterPath::CurveToDataElement);
            s << matrix.map(QPointF(elm.x, elm.y))
              << matrix.map(QPointF(path.elementAt(i+1).x, path.elementAt(i+1).y))
              << matrix.map(QPointF(path.elementAt(i+2).x, path.elementAt(i+2).y))
              << "c\n";
            i += 2;
            break;
        default:
            qFatal("QPdf::generatePath(), unhandled type: %d", elm.type);
        }
    }
    if (start >= 0
        && path.elementAt(start).x == path.elementAt(path.elementCount()-1).x
        && path.elementAt(start).y == path.elementAt(path.elementCount()-1).y)
        s << "h\n";

    Qt::FillRule fillRule = path.fillRule();

    const char *op = "";
    switch (flags) {
    case ClipPath:
        op = (fillRule == Qt::WindingFill) ? "W n\n" : "W* n\n";
        break;
    case FillPath:
        op = (fillRule == Qt::WindingFill) ? "f\n" : "f*\n";
        break;
    case StrokePath:
        op = "S\n";
        break;
    case FillAndStrokePath:
        op = (fillRule == Qt::WindingFill) ? "B\n" : "B*\n";
        break;
    }
    s << op;
    return result;
}

QByteArray QPdf::generateMatrix(const QTransform &matrix)
{
    QByteArray result;
    ByteStream s(&result);
    s << matrix.m11()
      << matrix.m12()
      << matrix.m21()
      << matrix.m22()
      << matrix.dx()
      << matrix.dy()
      << "cm\n";
    return result;
}

QByteArray QPdf::generateDashes(const QPen &pen)
{
    QByteArray result;
    ByteStream s(&result);
    s << '[';

    QVector<qreal> dasharray = pen.dashPattern();
    qreal w = pen.widthF();
    if (w < 0.001)
        w = 1;
    for (int i = 0; i < dasharray.size(); ++i) {
        qreal dw = dasharray.at(i)*w;
        if (dw < 0.0001) dw = 0.0001;
        s << dw;
    }
    s << ']';
    //qDebug() << "dasharray: pen has" << dasharray;
    //qDebug() << "  => " << result;
    return result;
}



static const char* pattern_for_brush[] = {
    0, // NoBrush
    0, // SolidPattern
    "0 J\n"
    "6 w\n"
    "[] 0 d\n"
    "4 0 m\n"
    "4 8 l\n"
    "0 4 m\n"
    "8 4 l\n"
    "S\n", // Dense1Pattern

    "0 J\n"
    "2 w\n"
    "[6 2] 1 d\n"
    "0 0 m\n"
    "0 8 l\n"
    "8 0 m\n"
    "8 8 l\n"
    "S\n"
    "[] 0 d\n"
    "2 0 m\n"
    "2 8 l\n"
    "6 0 m\n"
    "6 8 l\n"
    "S\n"
    "[6 2] -3 d\n"
    "4 0 m\n"
    "4 8 l\n"
    "S\n", // Dense2Pattern

    "0 J\n"
    "2 w\n"
    "[6 2] 1 d\n"
    "0 0 m\n"
    "0 8 l\n"
    "8 0 m\n"
    "8 8 l\n"
    "S\n"
    "[2 2] -1 d\n"
    "2 0 m\n"
    "2 8 l\n"
    "6 0 m\n"
    "6 8 l\n"
    "S\n"
    "[6 2] -3 d\n"
    "4 0 m\n"
    "4 8 l\n"
    "S\n", // Dense3Pattern

    "0 J\n"
    "2 w\n"
    "[2 2] 1 d\n"
    "0 0 m\n"
    "0 8 l\n"
    "8 0 m\n"
    "8 8 l\n"
    "S\n"
    "[2 2] -1 d\n"
    "2 0 m\n"
    "2 8 l\n"
    "6 0 m\n"
    "6 8 l\n"
    "S\n"
    "[2 2] 1 d\n"
    "4 0 m\n"
    "4 8 l\n"
    "S\n", // Dense4Pattern

    "0 J\n"
    "2 w\n"
    "[2 6] -1 d\n"
    "0 0 m\n"
    "0 8 l\n"
    "8 0 m\n"
    "8 8 l\n"
    "S\n"
    "[2 2] 1 d\n"
    "2 0 m\n"
    "2 8 l\n"
    "6 0 m\n"
    "6 8 l\n"
    "S\n"
    "[2 6] 3 d\n"
    "4 0 m\n"
    "4 8 l\n"
    "S\n", // Dense5Pattern

    "0 J\n"
    "2 w\n"
    "[2 6] -1 d\n"
    "0 0 m\n"
    "0 8 l\n"
    "8 0 m\n"
    "8 8 l\n"
    "S\n"
    "[2 6] 3 d\n"
    "4 0 m\n"
    "4 8 l\n"
    "S\n", // Dense6Pattern

    "0 J\n"
    "2 w\n"
    "[2 6] -1 d\n"
    "0 0 m\n"
    "0 8 l\n"
    "8 0 m\n"
    "8 8 l\n"
    "S\n", // Dense7Pattern

    "1 w\n"
    "0 4 m\n"
    "8 4 l\n"
    "S\n", // HorPattern

    "1 w\n"
    "4 0 m\n"
    "4 8 l\n"
    "S\n", // VerPattern

    "1 w\n"
    "4 0 m\n"
    "4 8 l\n"
    "0 4 m\n"
    "8 4 l\n"
    "S\n", // CrossPattern

    "1 w\n"
    "-1 5 m\n"
    "5 -1 l\n"
    "3 9 m\n"
    "9 3 l\n"
    "S\n", // BDiagPattern

    "1 w\n"
    "-1 3 m\n"
    "5 9 l\n"
    "3 -1 m\n"
    "9 5 l\n"
    "S\n", // FDiagPattern

    "1 w\n"
    "-1 3 m\n"
    "5 9 l\n"
    "3 -1 m\n"
    "9 5 l\n"
    "-1 5 m\n"
    "5 -1 l\n"
    "3 9 m\n"
    "9 3 l\n"
    "S\n", // DiagCrossPattern
};

QByteArray QPdf::patternForBrush(const QBrush &b)
{
    int style = b.style();
    if (style > Qt::DiagCrossPattern)
        return QByteArray();
    return pattern_for_brush[style];
}

static void moveToHook(qreal x, qreal y, void *data)
{
    QPdf::Stroker *t = (QPdf::Stroker *)data;
    if (!t->first)
        *t->stream << "h\n";
    if (!t->cosmeticPen)
        t->matrix.map(x, y, &x, &y);
    *t->stream << x << y << "m\n";
    t->first = false;
}

static void lineToHook(qreal x, qreal y, void *data)
{
    QPdf::Stroker *t = (QPdf::Stroker *)data;
    if (!t->cosmeticPen)
        t->matrix.map(x, y, &x, &y);
    *t->stream << x << y << "l\n";
}

static void cubicToHook(qreal c1x, qreal c1y,
                        qreal c2x, qreal c2y,
                        qreal ex, qreal ey,
                        void *data)
{
    QPdf::Stroker *t = (QPdf::Stroker *)data;
    if (!t->cosmeticPen) {
        t->matrix.map(c1x, c1y, &c1x, &c1y);
        t->matrix.map(c2x, c2y, &c2x, &c2y);
        t->matrix.map(ex, ey, &ex, &ey);
    }
    *t->stream << c1x << c1y
               << c2x << c2y
               << ex << ey
               << "c\n";
}

QPdf::Stroker::Stroker()
    : stream(0),
    first(true),
    dashStroker(&basicStroker)
{
    stroker = &basicStroker;
    basicStroker.setMoveToHook(moveToHook);
    basicStroker.setLineToHook(lineToHook);
    basicStroker.setCubicToHook(cubicToHook);
    cosmeticPen = true;
    basicStroker.setStrokeWidth(.1);
}

void QPdf::Stroker::setPen(const QPen &pen)
{
    if (pen.style() == Qt::NoPen) {
        stroker = 0;
        return;
    }
    qreal w = pen.widthF();
    bool zeroWidth = w < 0.0001;
    cosmeticPen = pen.isCosmetic();
    if (zeroWidth)
        w = .1;

    basicStroker.setStrokeWidth(w);
    basicStroker.setCapStyle(pen.capStyle());
    basicStroker.setJoinStyle(pen.joinStyle());
    basicStroker.setMiterLimit(pen.miterLimit());

    QVector<qreal> dashpattern = pen.dashPattern();
    if (zeroWidth) {
        for (int i = 0; i < dashpattern.size(); ++i)
            dashpattern[i] *= 10.;
    }
    if (!dashpattern.isEmpty()) {
        dashStroker.setDashPattern(dashpattern);
        dashStroker.setDashOffset(pen.dashOffset());
        stroker = &dashStroker;
    } else {
        stroker = &basicStroker;
    }
}

void QPdf::Stroker::strokePath(const QPainterPath &path)
{
    if (!stroker)
        return;
    first = true;

    stroker->strokePath(path, this, cosmeticPen ? matrix : QTransform());
    *stream << "h f\n";
}

QByteArray QPdf::ascii85Encode(const QByteArray &input)
{
    int isize = input.size()/4*4;
    QByteArray output(input.size()*5/4+7, Qt::Uninitialized);
    char *out = output.data();
    const uchar *in = (const uchar *)input.constData();
    for (int i = 0; i < isize; i += 4) {
        uint val = (((uint)in[i])<<24) + (((uint)in[i+1])<<16) + (((uint)in[i+2])<<8) + (uint)in[i+3];
        if (val == 0) {
            *out = 'z';
            ++out;
        } else {
            char base[5];
            base[4] = val % 85;
            val /= 85;
            base[3] = val % 85;
            val /= 85;
            base[2] = val % 85;
            val /= 85;
            base[1] = val % 85;
            val /= 85;
            base[0] = val % 85;
            *(out++) = base[0] + '!';
            *(out++) = base[1] + '!';
            *(out++) = base[2] + '!';
            *(out++) = base[3] + '!';
            *(out++) = base[4] + '!';
        }
    }
    //write the last few bytes
    int remaining = input.size() - isize;
    if (remaining) {
        uint val = 0;
        for (int i = isize; i < input.size(); ++i)
            val = (val << 8) + in[i];
        val <<= 8*(4-remaining);
        char base[5];
        base[4] = val % 85;
        val /= 85;
        base[3] = val % 85;
        val /= 85;
        base[2] = val % 85;
        val /= 85;
        base[1] = val % 85;
        val /= 85;
        base[0] = val % 85;
        for (int i = 0; i < remaining+1; ++i)
            *(out++) = base[i] + '!';
    }
    *(out++) = '~';
    *(out++) = '>';
    output.resize(out-output.data());
    return output;
}

const char *QPdf::toHex(ushort u, char *buffer)
{
    int i = 3;
    while (i >= 0) {
        ushort hex = (u & 0x000f);
        if (hex < 0x0a)
            buffer[i] = '0'+hex;
        else
            buffer[i] = 'A'+(hex-0x0a);
        u = u >> 4;
        i--;
    }
    buffer[4] = '\0';
    return buffer;
}

const char *QPdf::toHex(uchar u, char *buffer)
{
    int i = 1;
    while (i >= 0) {
        ushort hex = (u & 0x000f);
        if (hex < 0x0a)
            buffer[i] = '0'+hex;
        else
            buffer[i] = 'A'+(hex-0x0a);
        u = u >> 4;
        i--;
    }
    buffer[2] = '\0';
    return buffer;
}

#define Q_MM(n) int((n * 720 + 127) / 254)
#define Q_IN(n) int(n * 72)

static const char * const psToStr[QPrinter::NPaperSize+1] =
{
    "A4", "B5", "Letter", "Legal", "Executive",
    "A0", "A1", "A2", "A3", "A5", "A6", "A7", "A8", "A9", "B0", "B1",
    "B10", "B2", "B3", "B4", "B6", "B7", "B8", "B9", "C5E", "Comm10E",
    "DLE", "Folio", "Ledger", "Tabloid", 0
};

QPdf::PaperSize QPdf::paperSize(QPrinter::PaperSize paperSize)
{
    QSizeF s = qt_paperSizeToQSizeF(paperSize);
    PaperSize p = { Q_MM(s.width()), Q_MM(s.height()) };
    return p;
}

const char *QPdf::paperSizeToString(QPrinter::PaperSize paperSize)
{
    return psToStr[paperSize];
}

// -------------------------- base engine, shared code between PS and PDF -----------------------

QPdfBaseEngine::QPdfBaseEngine(QPdfBaseEnginePrivate &dd, PaintEngineFeatures f)
    : QAlphaPaintEngine(dd, f)
{
    Q_D(QPdfBaseEngine);
#if !defined(QT_NO_CUPS)
    if (QCUPSSupport::isAvailable()) {
        QCUPSSupport cups;
        const cups_dest_t* printers = cups.availablePrinters();
        int prnCount = cups.availablePrintersCount();

        for (int i = 0; i <  prnCount; ++i) {
            if (printers[i].is_default) {
                d->printerName = QString::fromLocal8Bit(printers[i].name);
                break;
            }
        }

    } else
#endif
    {
        d->printerName = qGetEnv("PRINTER");
        if (d->printerName.isEmpty())
            d->printerName = qGetEnv("LPDEST");
        if (d->printerName.isEmpty())
            d->printerName = qGetEnv("NPRINTER");
        if (d->printerName.isEmpty())
            d->printerName = qGetEnv("NGPRINTER");
    }
}

void QPdfBaseEngine::drawPoints (const QPointF *points, int pointCount)
{
    if (!points)
        return;

    Q_D(QPdfBaseEngine);
    QPainterPath p;
    for (int i=0; i!=pointCount;++i) {
        p.moveTo(points[i]);
        p.lineTo(points[i] + QPointF(0, 0.001));
    }

    bool hadBrush = d->hasBrush;
    d->hasBrush = false;
    drawPath(p);
    d->hasBrush = hadBrush;
}

void QPdfBaseEngine::drawLines (const QLineF *lines, int lineCount)
{
    if (!lines)
        return;

    Q_D(QPdfBaseEngine);
    QPainterPath p;
    for (int i=0; i!=lineCount;++i) {
        p.moveTo(lines[i].p1());
        p.lineTo(lines[i].p2());
    }
    bool hadBrush = d->hasBrush;
    d->hasBrush = false;
    drawPath(p);
    d->hasBrush = hadBrush;
}

void QPdfBaseEngine::drawRects (const QRectF *rects, int rectCount)
{
    if (!rects)
        return;

    Q_D(QPdfBaseEngine);
    if (d->useAlphaEngine) {
        QAlphaPaintEngine::drawRects(rects, rectCount);
        if (!continueCall())
            return;
    }

    if (d->clipEnabled && d->allClipped)
        return;
    if (!d->hasPen && !d->hasBrush)
        return;

    QBrush penBrush = d->pen.brush();
    if (d->simplePen || !d->hasPen) {
        // draw strokes natively in this case for better output
        if(!d->simplePen && !d->stroker.matrix.isIdentity())
            *d->currentPage << "q\n" << QPdf::generateMatrix(d->stroker.matrix);
        for (int i = 0; i < rectCount; ++i)
            *d->currentPage << rects[i].x() << rects[i].y() << rects[i].width() << rects[i].height() << "re\n";
        *d->currentPage << (d->hasPen ? (d->hasBrush ? "B\n" : "S\n") : "f\n");
        if(!d->simplePen && !d->stroker.matrix.isIdentity())
            *d->currentPage << "Q\n";
    } else {
        QPainterPath p;
        for (int i=0; i!=rectCount; ++i)
            p.addRect(rects[i]);
        drawPath(p);
    }
}

void QPdfBaseEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
    Q_D(QPdfBaseEngine);

    if (d->useAlphaEngine) {
        QAlphaPaintEngine::drawPolygon(points, pointCount, mode);
        if (!continueCall())
            return;
    }

    if (!points || !pointCount)
        return;

    bool hb = d->hasBrush;
    QPainterPath p;

    switch(mode) {
        case OddEvenMode:
            p.setFillRule(Qt::OddEvenFill);
            break;
        case ConvexMode:
        case WindingMode:
            p.setFillRule(Qt::WindingFill);
            break;
        case PolylineMode:
            d->hasBrush = false;
            break;
        default:
            break;
    }

    p.moveTo(points[0]);
    for (int i = 1; i < pointCount; ++i)
        p.lineTo(points[i]);

    if (mode != PolylineMode)
        p.closeSubpath();
    drawPath(p);

    d->hasBrush = hb;
}

void QPdfBaseEngine::drawPath (const QPainterPath &p)
{
    Q_D(QPdfBaseEngine);

    if (d->useAlphaEngine) {
        QAlphaPaintEngine::drawPath(p);
        if (!continueCall())
            return;
    }

    if (d->clipEnabled && d->allClipped)
        return;
    if (!d->hasPen && !d->hasBrush)
        return;

    if (d->simplePen) {
        // draw strokes natively in this case for better output
        *d->currentPage << QPdf::generatePath(p, QTransform(), d->hasBrush ? QPdf::FillAndStrokePath : QPdf::StrokePath);
    } else {
        if (d->hasBrush)
            *d->currentPage << QPdf::generatePath(p, d->stroker.matrix, QPdf::FillPath);
        if (d->hasPen) {
            *d->currentPage << "q\n";
            QBrush b = d->brush;
            d->brush = d->pen.brush();
            setBrush();
            d->stroker.strokePath(p);
            *d->currentPage << "Q\n";
            d->brush = b;
        }
    }
}

void QPdfBaseEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    Q_D(QPdfBaseEngine);

    if (d->useAlphaEngine) {
        QAlphaPaintEngine::drawTextItem(p, textItem);
        if (!continueCall())
            return;
    }

    if (!d->hasPen || (d->clipEnabled && d->allClipped))
        return;

    if (d->stroker.matrix.type() >= QTransform::TxProject) {
        QPaintEngine::drawTextItem(p, textItem);
        return;
    }

    *d->currentPage << "q\n";
    if(!d->simplePen)
        *d->currentPage << QPdf::generateMatrix(d->stroker.matrix);

    bool hp = d->hasPen;
    d->hasPen = false;
    QBrush b = d->brush;
    d->brush = d->pen.brush();
    setBrush();

    const QTextItemInt &ti = static_cast<const QTextItemInt &>(textItem);
    d->drawTextItem(p, ti);
    d->hasPen = hp;
    d->brush = b;
    *d->currentPage << "Q\n";
}


void QPdfBaseEngine::updateState(const QPaintEngineState &state)
{
    Q_D(QPdfBaseEngine);

    if (d->useAlphaEngine) {
        QAlphaPaintEngine::updateState(state);
        if (!continueCall())
            return;
    }

    QPaintEngine::DirtyFlags flags = state.state();

    if (flags & DirtyTransform)
        d->stroker.matrix = state.transform();

    if (flags & DirtyPen) {
        d->pen = state.pen();
        d->hasPen = d->pen.style() != Qt::NoPen;
        d->stroker.setPen(d->pen);
        QBrush penBrush = d->pen.brush();
        bool oldSimple = d->simplePen;
        d->simplePen = (d->hasPen && (penBrush.style() == Qt::SolidPattern) && penBrush.isOpaque());
        if (oldSimple != d->simplePen)
            flags |= DirtyTransform;
    }
    if (flags & DirtyBrush) {
        d->brush = state.brush();
        if (d->brush.color().alpha() == 0 && d->brush.style() == Qt::SolidPattern)
            d->brush.setStyle(Qt::NoBrush);
        d->hasBrush = d->brush.style() != Qt::NoBrush;
    }
    if (flags & DirtyBrushOrigin) {
        d->brushOrigin = state.brushOrigin();
        flags |= DirtyBrush;
    }
    if (flags & DirtyOpacity)
        d->opacity = state.opacity();

    bool ce = d->clipEnabled;
    if (flags & DirtyClipPath) {
        d->clipEnabled = true;
        updateClipPath(state.clipPath(), state.clipOperation());
    } else if (flags & DirtyClipRegion) {
        d->clipEnabled = true;
        QPainterPath path;
        QVector<QRect> rects = state.clipRegion().rects();
        for (int i = 0; i < rects.size(); ++i)
            path.addRect(rects.at(i));
        updateClipPath(path, state.clipOperation());
        flags |= DirtyClipPath;
    } else if (flags & DirtyClipEnabled) {
        d->clipEnabled = state.isClipEnabled();
    }

    if (ce != d->clipEnabled)
        flags |= DirtyClipPath;
    else if (!d->clipEnabled)
        flags &= ~DirtyClipPath;

    setupGraphicsState(flags);
}

void QPdfBaseEngine::setupGraphicsState(QPaintEngine::DirtyFlags flags)
{
    Q_D(QPdfBaseEngine);
    if (flags & DirtyClipPath)
        flags |= DirtyTransform|DirtyPen|DirtyBrush;

    if (flags & DirtyTransform) {
        *d->currentPage << "Q\n";
        flags |= DirtyPen|DirtyBrush;
    }

    if (flags & DirtyClipPath) {
        *d->currentPage << "Q q\n";

        d->allClipped = false;
        if (d->clipEnabled && !d->clips.isEmpty()) {
            for (int i = 0; i < d->clips.size(); ++i) {
                if (d->clips.at(i).isEmpty()) {
                    d->allClipped = true;
                    break;
                }
            }
            if (!d->allClipped) {
                for (int i = 0; i < d->clips.size(); ++i) {
                    *d->currentPage << QPdf::generatePath(d->clips.at(i), QTransform(), QPdf::ClipPath);
                }
            }
        }
    }

    if (flags & DirtyTransform) {
        *d->currentPage << "q\n";
        if (d->simplePen && !d->stroker.matrix.isIdentity())
            *d->currentPage << QPdf::generateMatrix(d->stroker.matrix);
    }
    if (flags & DirtyBrush)
        setBrush();
    if (d->simplePen && (flags & DirtyPen))
        setPen();
}

extern Q_AUTOTEST_EXPORT QPainterPath qt_regionToPath(const QRegion &region);

void QPdfBaseEngine::updateClipPath(const QPainterPath &p, Qt::ClipOperation op)
{
    Q_D(QPdfBaseEngine);
    QPainterPath path = d->stroker.matrix.map(p);
    //qDebug() << "updateClipPath: " << d->stroker.matrix << p.boundingRect() << path.boundingRect() << op;

    if (op == Qt::NoClip) {
        d->clipEnabled = false;
        d->clips.clear();
    } else if (op == Qt::ReplaceClip) {
        d->clips.clear();
        d->clips.append(path);
    } else if (op == Qt::IntersectClip) {
        d->clips.append(path);
    } else { // UniteClip
        // ask the painter for the current clipping path. that's the easiest solution
        path = painter()->clipPath();
        path = d->stroker.matrix.map(path);
        d->clips.clear();
        d->clips.append(path);
    }

    if (d->useAlphaEngine) {
        // if we have an alpha region, we have to subtract that from the
        // any existing clip region since that region will be filled in
        // later with images
        QPainterPath alphaClip = qt_regionToPath(alphaClipping());
        if (!alphaClip.isEmpty()) {
            if (!d->clipEnabled) {
                QRect r = d->fullPage ? d->paperRect() : d->pageRect();
                QPainterPath dev;
                dev.addRect(QRect(0, 0, r.width(), r.height()));
                if (path.isEmpty())
                    path = dev;
                else
                    path = path.intersected(dev);
                d->clipEnabled = true;
            } else {
                path = painter()->clipPath();
                path = d->stroker.matrix.map(path);
            }
            path = path.subtracted(alphaClip);
            d->clips.clear();
            d->clips.append(path);
        }
    }
}

void QPdfBaseEngine::setPen()
{
    Q_D(QPdfBaseEngine);
    if (d->pen.style() == Qt::NoPen)
        return;
    QBrush b = d->pen.brush();
    Q_ASSERT(b.style() == Qt::SolidPattern && b.isOpaque());

    QColor rgba = b.color();
    if (d->colorMode == QPrinter::GrayScale) {
        qreal gray = qGray(rgba.rgba())/255.;
        *d->currentPage << gray << gray << gray;
    } else {
        *d->currentPage << rgba.redF()
                        << rgba.greenF()
                        << rgba.blueF();
    }
    *d->currentPage << "SCN\n";

    *d->currentPage << d->pen.widthF() << "w ";

    int pdfCapStyle = 0;
    switch(d->pen.capStyle()) {
    case Qt::FlatCap:
        pdfCapStyle = 0;
        break;
    case Qt::SquareCap:
        pdfCapStyle = 2;
        break;
    case Qt::RoundCap:
        pdfCapStyle = 1;
        break;
    default:
        break;
    }
    *d->currentPage << pdfCapStyle << "J ";

    int pdfJoinStyle = 0;
    switch(d->pen.joinStyle()) {
    case Qt::MiterJoin:
        pdfJoinStyle = 0;
        break;
    case Qt::BevelJoin:
        pdfJoinStyle = 2;
        break;
    case Qt::RoundJoin:
        pdfJoinStyle = 1;
        break;
    default:
        break;
    }
    *d->currentPage << pdfJoinStyle << "j ";

    *d->currentPage << QPdf::generateDashes(d->pen) << " 0 d\n";
}

bool QPdfBaseEngine::newPage()
{
    Q_D(QPdfBaseEngine);
    setupGraphicsState(DirtyBrush|DirtyPen|DirtyClipPath);
    QFile *outfile = qobject_cast<QFile*> (d->outDevice);
    if (outfile && outfile->error() != QFile::NoError)
        return false;
    return true;
}


int QPdfBaseEngine::metric(QPaintDevice::PaintDeviceMetric metricType) const
{
    Q_D(const QPdfBaseEngine);
    int val;
    QRect r = d->fullPage ? d->paperRect() : d->pageRect();
    switch (metricType) {
    case QPaintDevice::PdmWidth:
        val = r.width();
        break;
    case QPaintDevice::PdmHeight:
        val = r.height();
        break;
    case QPaintDevice::PdmDpiX:
    case QPaintDevice::PdmDpiY:
        val = d->resolution;
        break;
    case QPaintDevice::PdmWidthMM:
        val = qRound(r.width()*25.4/d->resolution);
        break;
    case QPaintDevice::PdmHeightMM:
        val = qRound(r.height()*25.4/d->resolution);
        break;
    case QPaintDevice::PdmNumColors:
        val = INT_MAX;
        break;
    case QPaintDevice::PdmDepth:
        val = 32;
        break;
    default:
        qWarning("QPrinter::metric: Invalid metric command");
        return 0;
    }
    return val;
}

void QPdfBaseEngine::setProperty(PrintEnginePropertyKey key, const QVariant &value)
{
    Q_D(QPdfBaseEngine);
    switch (int(key)) {
    case PPK_CollateCopies:
        d->collate = value.toBool();
        break;
    case PPK_ColorMode:
        d->colorMode = QPrinter::ColorMode(value.toInt());
        break;
    case PPK_Creator:
        d->creator = value.toString();
        break;
    case PPK_DocumentName:
        d->title = value.toString();
        break;
    case PPK_FullPage:
        d->fullPage = value.toBool();
        break;
    case PPK_CopyCount: // fallthrough
    case PPK_NumberOfCopies:
        d->copies = value.toInt();
        break;
    case PPK_Orientation:
        d->orientation = QPrinter::Orientation(value.toInt());
        break;
    case PPK_PageOrder:
        d->pageOrder = QPrinter::PageOrder(value.toInt());
        break;
    case PPK_PaperSize:
        d->paperSize = QPrinter::PaperSize(value.toInt());
        break;
    case PPK_PaperSource:
        d->paperSource = QPrinter::PaperSource(value.toInt());
        break;
    case PPK_PrinterName:
        d->printerName = value.toString();
        break;
    case PPK_Resolution:
        d->resolution = value.toInt();
        break;
    case PPK_SelectionOption:
        d->selectionOption = value.toString();
        break;
    case PPK_FontEmbedding:
        d->embedFonts = value.toBool();
        break;
    case PPK_Duplex:
        d->duplex = static_cast<QPrinter::DuplexMode> (value.toInt());
        break;
    case PPK_CupsPageRect:
        d->cupsPageRect = value.toRect();
        break;
    case PPK_CupsPaperRect:
        d->cupsPaperRect = value.toRect();
        break;
    case PPK_CupsOptions:
        d->cupsOptions = value.toStringList();
        break;
    case PPK_CupsStringPageSize:
        d->cupsStringPageSize = value.toString();
        break;
    case PPK_CustomPaperSize:
        d->paperSize = QPrinter::Custom;
        d->customPaperSize = value.toSizeF();
        break;
    case PPK_PageMargins:
    {
        QList<QVariant> margins(value.toList());
        Q_ASSERT(margins.size() == 4);
        d->leftMargin = margins.at(0).toReal();
        d->topMargin = margins.at(1).toReal();
        d->rightMargin = margins.at(2).toReal();
        d->bottomMargin = margins.at(3).toReal();
        d->hasCustomPageMargins = true;
        break;
    }
    default:
        break;
    }
}

QVariant QPdfBaseEngine::property(PrintEnginePropertyKey key) const
{
    Q_D(const QPdfBaseEngine);

    QVariant ret;
    switch (int(key)) {
    case PPK_CollateCopies:
        ret = d->collate;
        break;
    case PPK_ColorMode:
        ret = d->colorMode;
        break;
    case PPK_Creator:
        ret = d->creator;
        break;
    case PPK_DocumentName:
        ret = d->title;
        break;
    case PPK_FullPage:
        ret = d->fullPage;
        break;
    case PPK_CopyCount:
        ret = d->copies;
        break;
    case PPK_SupportsMultipleCopies:
#if !defined(QT_NO_CUPS)
        if (QCUPSSupport::isAvailable())
            ret = true;
        else
#endif
            ret = false;
        break;
    case PPK_NumberOfCopies:
#if !defined(QT_NO_CUPS)
        if (QCUPSSupport::isAvailable())
            ret = 1;
        else
#endif
            ret = d->copies;
        break;
    case PPK_Orientation:
        ret = d->orientation;
        break;
    case PPK_PageOrder:
        ret = d->pageOrder;
        break;
    case PPK_PaperSize:
        ret = d->paperSize;
        break;
    case PPK_PaperSource:
        ret = d->paperSource;
        break;
    case PPK_PrinterName:
        ret = d->printerName;
        break;
    case PPK_Resolution:
        ret = d->resolution;
        break;
    case PPK_SupportedResolutions:
        ret = QList<QVariant>() << 72;
        break;
    case PPK_PaperRect:
        ret = d->paperRect();
        break;
    case PPK_PageRect:
        ret = d->pageRect();
        break;
    case PPK_SelectionOption:
        ret = d->selectionOption;
        break;
    case PPK_FontEmbedding:
        ret = d->embedFonts;
        break;
    case PPK_Duplex:
        ret = d->duplex;
        break;
    case PPK_CupsPageRect:
        ret = d->cupsPageRect;
        break;
    case PPK_CupsPaperRect:
        ret = d->cupsPaperRect;
        break;
    case PPK_CupsOptions:
        ret = d->cupsOptions;
        break;
    case PPK_CupsStringPageSize:
        ret = d->cupsStringPageSize;
        break;
    case PPK_CustomPaperSize:
        ret = d->customPaperSize;
        break;
    case PPK_PageMargins:
    {
        QList<QVariant> margins;
        if (d->hasCustomPageMargins) {
            margins << d->leftMargin << d->topMargin
                    << d->rightMargin << d->bottomMargin;
        } else {
            const qreal defaultMargin = 10; // ~3.5 mm
            margins << defaultMargin << defaultMargin
                    << defaultMargin << defaultMargin;
        }
        ret = margins;
        break;
    }
    default:
        break;
    }
    return ret;
}

QPdfBaseEnginePrivate::QPdfBaseEnginePrivate()
    : clipEnabled(false), allClipped(false), hasPen(true), hasBrush(false), simplePen(false),
      useAlphaEngine(false),
      outDevice(0), fd(-1),
      duplex(QPrinter::DuplexNone), collate(false), fullPage(false), embedFonts(true), copies(1),
      pageOrder(QPrinter::FirstPageFirst), orientation(QPrinter::Portrait),
      paperSize(QPrinter::A4), colorMode(QPrinter::Color), paperSource(QPrinter::Auto),
      hasCustomPageMargins(false),
      leftMargin(0), topMargin(0), rightMargin(0), bottomMargin(0)
{
    resolution = QX11Info::appDpiY();
    postscript = false;
    currentObject = 1;
    currentPage = 0;
    stroker.stream = 0;
}

bool QPdfBaseEngine::begin(QPaintDevice *pdev)
{
    Q_D(QPdfBaseEngine);
    d->pdev = pdev;

    d->postscript = false;
    d->currentObject = 1;

    d->currentPage = new QPdfPage;
    d->stroker.stream = d->currentPage;
    d->opacity = 1.0;

    return d->openPrintDevice();
}

bool QPdfBaseEngine::end()
{
    Q_D(QPdfBaseEngine);
    qDeleteAll(d->fonts);
    d->fonts.clear();
    delete d->currentPage;
    d->currentPage = 0;

    d->closePrintDevice();
    return true;
}

bool QPdfBaseEnginePrivate::openPrintDevice()
{
    if (outDevice)
        return false;

#if !defined(QT_NO_CUPS)
    if (QCUPSSupport::isAvailable()) {
        QCUPSSupport cups;
        QPair<int, QString> ret = cups.tempFd();
        if (ret.first < 0) {
            qWarning("QPdfPrinter: Could not open temporary file to print");
            return false;
        }
        cupsTempFile = ret.second;
        outDevice = new QFile();
        qobject_cast<QFile*>(outDevice)->open(ret.first, QIODevice::WriteOnly);
        fd = ret.first;
    }
#endif

    // dummy device as required by PS header generator for example
    if (!outDevice) {
        outDevice = new QTemporaryFile();
        qobject_cast<QTemporaryFile*>(outDevice)->open();
    }

    return true;
}

void QPdfBaseEnginePrivate::closePrintDevice()
{
    if (!outDevice)
        return;
    outDevice->close();
    if (fd >= 0)
        qt_safe_close(fd);
    fd = -1;
    delete outDevice;
    outDevice = 0;

#if !defined(QT_NO_CUPS)
    if (!cupsTempFile.isEmpty()) {
        QString tempFile = cupsTempFile;
        cupsTempFile.clear();

        // Set up print options.
        QByteArray prnName;
        QList<QPair<QByteArray, QByteArray> > options;
        QVector<cups_option_t> cupsOptStruct;

        if (!printerName.isEmpty()) {
            prnName = printerName.toLocal8Bit();
        } else {
            QPrinterInfo def = QPrinterInfo::defaultPrinter();
            if (def.isNull()) {
                qWarning("Could not determine printer to print to");
                QFile::remove(tempFile);
                return;
            }
            prnName = def.printerName().toLocal8Bit();
        }

        if (!cupsStringPageSize.isEmpty()) {
            options.append(QPair<QByteArray, QByteArray>("media", cupsStringPageSize.toLocal8Bit()));
        }

        if (copies > 1) {
            options.append(QPair<QByteArray, QByteArray>("copies", QString::number(copies).toLocal8Bit()));
        }

        if (collate) {
            options.append(QPair<QByteArray, QByteArray>("Collate", "True"));
        }

        switch (duplex) {
        case QPrinter::DuplexNone:
            options.append(QPair<QByteArray, QByteArray>("sides", "one-sided"));
            break;
        case QPrinter::DuplexAuto:
            if (orientation == QPrinter::Portrait)
                options.append(QPair<QByteArray, QByteArray>("sides", "two-sided-long-edge"));
            else
                options.append(QPair<QByteArray, QByteArray>("sides", "two-sided-short-edge"));
            break;
        case QPrinter::DuplexLongSide:
            options.append(QPair<QByteArray, QByteArray>("sides", "two-sided-long-edge"));
            break;
        case QPrinter::DuplexShortSide:
            options.append(QPair<QByteArray, QByteArray>("sides", "two-sided-short-edge"));
            break;
        }

        if (QCUPSSupport::cupsVersion() >= 10300 && orientation == QPrinter::Landscape) {
            options.append(QPair<QByteArray, QByteArray>("landscape", ""));
        }

        QStringList::const_iterator it = cupsOptions.constBegin();
        while (it != cupsOptions.constEnd()) {
            options.append(QPair<QByteArray, QByteArray>((*it).toLocal8Bit(), (*(it+1)).toLocal8Bit()));
            it += 2;
        }

        for (int c = 0; c < options.size(); ++c) {
            cups_option_t opt;
            opt.name = options[c].first.data();
            opt.value = options[c].second.data();
            cupsOptStruct.append(opt);
        }

        // Print the file.
        QCUPSSupport cups;
        cups_option_t* optPtr = cupsOptStruct.size() ? &cupsOptStruct.first() : 0;
        cups.printFile(prnName.constData(), tempFile.toLocal8Bit().constData(),
                title.toLocal8Bit().constData(), cupsOptStruct.size(), optPtr);

        QFile::remove(tempFile);
    }
#endif
}

QPdfBaseEnginePrivate::~QPdfBaseEnginePrivate()
{
    qDeleteAll(fonts);
    delete currentPage;
}

void QPdfBaseEnginePrivate::drawTextItem(const QPointF &p, const QTextItemInt &ti)
{
    Q_Q(QPdfBaseEngine);

    QFontEngine *fe = ti.fontEngine;

    QFontEngine::FaceId face_id = fe->faceId();
    bool noEmbed = false;
    if (face_id.filename.isEmpty()
        || (!postscript && ((fe->fsType & 0x200) /* bitmap embedding only */
                            || (fe->fsType == 2) /* no embedding allowed */))) {
        *currentPage << "Q\n";
        q->QPaintEngine::drawTextItem(p, ti);
        *currentPage << "q\n";
        if (face_id.filename.isEmpty())
            return;
        noEmbed = true;
    }

    QFontSubset *font = fonts.value(face_id, 0);
    if (!font) {
        font = new QFontSubset(fe, requestObject());
        font->noEmbed = noEmbed;
    }
    fonts.insert(face_id, font);

    if (!currentPage->fonts.contains(font->object_id))
        currentPage->fonts.append(font->object_id);

    const qreal size = ti.fontEngine->fontDef.pixelSize;
    QStdVector<glyph_t> glyphs(ti.glyphs.numGlyphs);
    QStdVector<QFixedPoint> positions(ti.glyphs.numGlyphs);
    ti.fontEngine->getGlyphPositions(ti.glyphs, p, glyphs, positions);
    if (glyphs.size() == 0)
        return;
    const int synthesized = ti.fontEngine->synthesized();
    const qreal stretch = synthesized & QFontEngine::SynthesizedStretch ? ti.fontEngine->fontDef.stretch/100. : 1.;

    *currentPage << "BT\n"
                 << "/F" << font->object_id << size << "Tf "
                 << stretch << "0 0 -1 0 0 Tm\n";


#if 0
    // #### implement actual text for complex languages
    const unsigned short *logClusters = ti.logClusters;
    int pos = 0;
    do {
        int end = pos + 1;
        while (end < ti.num_chars && logClusters[end] == logClusters[pos])
            ++end;
        *currentPage << "/Span << /ActualText <FEFF";
        for (int i = pos; i < end; ++i) {
            s << toHex((ushort)ti.chars[i].unicode(), buf);
        }
        *currentPage << "> >>\n"
            "BDC\n"
            "<";
        int ge = end == ti.num_chars ? ti.num_glyphs : logClusters[end];
        for (int gs = logClusters[pos]; gs < ge; ++gs)
            *currentPage << toHex((ushort)ti.glyphs[gs].glyph, buf);
        *currentPage << "> Tj\n"
            "EMC\n";
        pos = end;
    } while (pos < ti.num_chars);
#else
    qreal last_x = 0.;
    qreal last_y = 0.;
    for (int i = 0; i < glyphs.size(); ++i) {
        qreal x = positions[i].x.toReal();
        qreal y = positions[i].y.toReal();
        x /= stretch;
        QSTACKARRAY(char, buf, 5);
        int g = font->addGlyph(glyphs[i]);
        *currentPage << x - last_x << last_y - y << "Td <"
                     << QPdf::toHex((ushort)g, buf) << "> Tj\n";
        last_x = x;
        last_y = y;
    }
#endif

    *currentPage << "ET\n";
}

QRect QPdfBaseEnginePrivate::paperRect() const
{
    int w;
    int h;
    if (paperSize == QPrinter::Custom) {
        w = qRound(customPaperSize.width()*resolution/72.);
        h = qRound(customPaperSize.height()*resolution/72.);
    } else {
#if !defined(QT_NO_CUPS)
        if (QCUPSSupport::isAvailable() && !cupsPaperRect.isNull()) {
            QRect r = cupsPaperRect;
            w = r.width();
            h = r.height();
        } else
#endif
        {
            QPdf::PaperSize s = QPdf::paperSize(paperSize);
            w = s.width;
            h = s.height;
        }
        w = qRound(w*resolution/72.);
        h = qRound(h*resolution/72.);
    }
    if (orientation == QPrinter::Portrait)
        return QRect(0, 0, w, h);
    else
        return QRect(0, 0, h, w);
}

QRect QPdfBaseEnginePrivate::pageRect() const
{
    if(fullPage)
        return paperRect();

    QRect r;

#if !defined(QT_NO_CUPS)
    if (!hasCustomPageMargins && QCUPSSupport::isAvailable() && !cupsPageRect.isNull()) {
        r = cupsPageRect;
        if (r == cupsPaperRect) {
            // if cups doesn't define any margins, give it at least approx 3.5 mm
            r = QRect(10, 10, r.width() - 20, r.height() - 20);
        }
    } else
#endif
    {
        QPdf::PaperSize s;
        if (paperSize == QPrinter::Custom) {
            s.width = qRound(customPaperSize.width());
            s.height = qRound(customPaperSize.height());
        } else {
            s = QPdf::paperSize(paperSize);
        }
        if (hasCustomPageMargins)
            r = QRect(0, 0, s.width, s.height);
        else
            r = QRect(72/3, 72/3, s.width - 2*72/3, s.height - 2*72/3);
    }

    int x = qRound(r.left()*resolution/72.);
    int y = qRound(r.top()*resolution/72.);
    int w = qRound(r.width()*resolution/72.);
    int h = qRound(r.height()*resolution/72.);
    if (orientation == QPrinter::Portrait)
        r = QRect(x, y, w, h);
    else
        r = QRect(y, x, h, w);

    if (hasCustomPageMargins) {
        r.adjust(qRound(leftMargin*(resolution/72.)),
                 qRound(topMargin*(resolution/72.)),
                 -qRound(rightMargin*(resolution/72.)),
                 -qRound(bottomMargin*(resolution/72.)));
    }
    return r;
}

#endif

QT_END_NAMESPACE
