%module KtGui

%include "tricks.i"
// for reference to Core base classes
%include "KtCore.i"

%include "gui/qfontinfo.i"
%include "gui/qfontmetrics.i"
%include "gui/qwidget.i"

%{
#include "QtGui/QApplication"
#include "QtGui/QPaintDevice"
#include "QtGui/QMainWindow"
#include "QtGui/QMatrix"
#include "QtGui/QTransform"
#include "QtGui/QTextFormat"
#include "QtGui/QTextLength"

QT_USE_NAMESPACE
%}

// temporary implementations for testing purposes
class QApplication : public QCoreApplication {
public:
    QApplication(int argc, char *argv[]);
    ~QApplication();

    static int exec();
};

class QPaintDevice {
public:
    virtual ~QPaintDevice();
protected:
    QPaintDevice();
};

class QMainWindow : public QWidget {
public:
    QMainWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    ~QMainWindow();
};

// stub implementations for QVariant testing
class QMatrix {
public:
    QMatrix();
    ~QMatrix();
};

class QTransform {
public:
    QTransform();
    ~QTransform();
};

class QTextFormat {
public:
    QTextFormat();
    ~QTextFormat();
};

class QTextLength {
public:
    QTextLength();
    ~QTextLength();
};
