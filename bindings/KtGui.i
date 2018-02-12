%module KtGui

%include <KtCore.i>

%{
#include "katie/QtGui/QtGui"

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

class QWidget : public QObject, public QPaintDevice{
public:
    QWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    ~QWidget();

    void setWindowTitle(const QString title);
    void show();
};

class QMainWindow : public QWidget {
public:
    QMainWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    ~QMainWindow();
};
