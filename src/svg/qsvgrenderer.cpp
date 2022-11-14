/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSvg module of the Katie Toolkit.
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

#include "qsvgrenderer.h"
#include "qsvgtinydocument_p.h"
#include "qbytearray.h"
#include "qdebug.h"
#include "qobject_p.h"


QT_BEGIN_NAMESPACE

/*!
    \class QSvgRenderer
    \ingroup painting

    \brief The QSvgRenderer class is used to draw the contents of SVG files onto paint devices.
    \since 4.1
    \reentrant

    Using QSvgRenderer, Scalable Vector Graphics (SVG) can be rendered onto any QPaintDevice
    subclass, including QWidget and QImage.

    QSvgRenderer provides an API that supports basic features of SVG rendering, such as loading
    and rendering of static drawings. Since the rendering is performed using QPainter, SVG
    drawings can be rendered on any subclass of QPaintDevice.

    SVG drawings are either loaded when an QSvgRenderer is constructed, or loaded later
    using the load() functions. Data is either supplied directly as serialized XML, or
    indirectly using a file name. If a valid file has been loaded, either when the renderer
    is constructed or at some later time, isValid() returns true; otherwise it returns false.
    QSvgRenderer provides the render() slot to render the current document using a given painter.

    The defaultSize() function provides information about the amount of space that is required
    to render the currently loaded SVG file. This is useful for paint devices, such as QWidget,
    that often need to supply a size hint to their parent layout.
    The default size of a drawing may differ from its visible area, found using the \l viewBox
    property.

    Animated SVG drawings are not supported.

    Finally, the QSvgRenderer class provides the repaintNeeded() signal which is emitted
    whenever the rendering of the document needs to be updated.

    \sa {QtSvg Module}, {SVG Viewer Example}
*/

class QSvgRendererPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QSvgRenderer)
public:
    explicit QSvgRendererPrivate()
        : QObjectPrivate(),
          render(nullptr)
    {}
    ~QSvgRendererPrivate()
    {
        delete render;
    }

    QSvgTinyDocument *render;
};

/*!
    Constructs a new renderer with the given \a parent.
*/
QSvgRenderer::QSvgRenderer(QObject *parent)
    : QObject(*(new QSvgRendererPrivate), parent)
{
}

/*!
    Constructs a new renderer with the given \a parent and loads the contents of the
    SVG file with the specified \a filename.
*/
QSvgRenderer::QSvgRenderer(const QString &filename, QObject *parent)
    : QObject(*new QSvgRendererPrivate, parent)
{
    load(filename);
}

/*!
    Constructs a new renderer with the given \a parent and loads the SVG data
    from the byte array specified by \a contents.
*/
QSvgRenderer::QSvgRenderer(const QByteArray &contents, QObject *parent)
    : QObject(*new QSvgRendererPrivate, parent)
{
    load(contents);
}

/*!
    \since 4.5

    Constructs a new renderer with the given \a parent and loads the SVG data
    using the stream reader specified by \a contents.
*/
QSvgRenderer::QSvgRenderer(QXmlStreamReader *contents, QObject *parent)
    : QObject(*new QSvgRendererPrivate, parent)
{
    load(contents);
}

/*!
    Destroys the renderer.
*/
QSvgRenderer::~QSvgRenderer()
{
}

/*!
    Returns true if there is a valid current document; otherwise returns false.
*/
bool QSvgRenderer::isValid() const
{
    Q_D(const QSvgRenderer);
    return d->render;
}

/*!
    Returns the default size of the document contents.
*/
QSize QSvgRenderer::defaultSize() const
{
    Q_D(const QSvgRenderer);
    if (d->render) {
        return d->render->size();
    }
    return QSize();
}

/*!
    Returns viewBoxF().toRect().

    \sa viewBoxF()
*/
QRect QSvgRenderer::viewBox() const
{
    Q_D(const QSvgRenderer);
    if (d->render) {
        return d->render->viewBox().toRect();
    }
    return QRect();
}

/*!
    \property QSvgRenderer::viewBox
    \brief the rectangle specifying the visible area of the document in logical coordinates
    \since 4.2
*/
void QSvgRenderer::setViewBox(const QRect &viewbox)
{
    Q_D(QSvgRenderer);
    if (d->render) {
        d->render->setViewBox(viewbox);
    }
}

/*!
    Loads the SVG file specified by \a filename, returning true if the content
    was successfully parsed; otherwise returns false.
*/
bool QSvgRenderer::load(const QString &filename)
{
    Q_D(QSvgRenderer);
    delete d->render;
    d->render = QSvgTinyDocument::load(filename);
    // force first update
    repaintNeeded();
    return d->render;
}

/*!
    Loads the specified SVG format \a contents, returning true if the content
    was successfully parsed; otherwise returns false.
*/
bool QSvgRenderer::load(const QByteArray &contents)
{
    Q_D(QSvgRenderer);
    delete d->render;
    d->render = QSvgTinyDocument::load(contents);
    // force first update
    repaintNeeded();
    return d->render;
}

/*!
  Loads the specified SVG in \a contents, returning true if the content
  was successfully parsed; otherwise returns false.

  The reader will be used from where it currently is positioned. If \a contents
  is \c null, behavior is undefined.

  \since 4.5
*/
bool QSvgRenderer::load(QXmlStreamReader *contents)
{
    Q_D(QSvgRenderer);
    delete d->render;
    d->render = QSvgTinyDocument::load(contents);
    // force first update
    repaintNeeded();
    return d->render;
}

/*!
    Renders the current document using the given \a painter.
*/
void QSvgRenderer::render(QPainter *painter)
{
    Q_D(QSvgRenderer);
    if (d->render) {
        d->render->draw(painter, QRectF());
    }
}

/*!
    \fn void QSvgRenderer::repaintNeeded()

    This signal is emitted whenever the rendering of the document
    needs to be updated.
*/

/*!
    Renders the given element with \a elementId using the given \a painter
    on the specified \a bounds. If the bounding rectangle is not specified
    the SVG element is mapped to the whole paint device.
*/
void QSvgRenderer::render(QPainter *painter, const QString &elementId,
                          const QRectF &bounds)
{
    Q_D(QSvgRenderer);
    if (d->render) {
        d->render->draw(painter, elementId, bounds);
    }
}

/*!
    Renders the current document sing the given \a painter on the
    specified \a bounds within the painter.  If the bounding rectangle is not
    specified the SVG file is mapped to the whole paint device.
*/
void QSvgRenderer::render(QPainter *painter, const QRectF &bounds)
{
    Q_D(QSvgRenderer);
    if (d->render) {
        d->render->draw(painter, bounds);
    }
}

QRectF QSvgRenderer::viewBoxF() const
{
    Q_D(const QSvgRenderer);
    if (d->render) {
        return d->render->viewBox();
    }
    return QRect();
}

void QSvgRenderer::setViewBox(const QRectF &viewbox)
{
    Q_D(QSvgRenderer);
    if (d->render) {
        d->render->setViewBox(viewbox);
    }
}

/*!
    \since 4.2

    Returns bounding rectangle of the item with the given \a id.
    The transformation matrix of parent elements is not affecting
    the bounds of the element.

    \sa matrixForElement()
*/
QRectF QSvgRenderer::boundsOnElement(const QString &id) const
{
    Q_D(const QSvgRenderer);
    if (d->render) {
        return d->render->boundsOnElement(id);
    }
    return QRectF();
}


/*!
    \since 4.2

    Returns true if the element with the given \a id exists
    in the currently parsed SVG file and is a renderable
    element.

    Note: this method returns true only for elements that
    can be rendered. Which implies that elements that are considered
    part of the fill/stroke style properties, e.g. radialGradients
    even tough marked with "id" attributes will not be found by this
    method.
*/
bool QSvgRenderer::elementExists(const QString &id) const
{
    Q_D(const QSvgRenderer);
    if (d->render) {
        return d->render->elementExists(id);
    }
    return false;
}

/*!
    \since 4.2

    Returns the transformation matrix for the element
    with the given \a id. The matrix is a product of
    the transformation of the element's parents. The transformation of
    the element itself is not included.

    To find the bounding rectangle of the element in logical coordinates,
    you can apply the matrix on the rectangle returned from boundsOnElement().

    \sa boundsOnElement()
*/
QMatrix QSvgRenderer::matrixForElement(const QString &id) const
{
    Q_D(const QSvgRenderer);
    if (d->render) {
        return d->render->matrixForElement(id);
    }
    return QMatrix();
}

QT_END_NAMESPACE

#include "moc_qsvgrenderer.h"
