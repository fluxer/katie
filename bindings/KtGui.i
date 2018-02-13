%module KtGui

%include "tricks.i"
// for reference to Core base classes
%include "KtCore.i"

%include "gui/qfontinfo.i"
%include "gui/qfontmetrics.i"
%include "gui/qwidget.i"

%{
#include "QtGui/QtGui"

QT_USE_NAMESPACE
%}

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
