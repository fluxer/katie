/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the documentation of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:FDL$
** GNU Free Documentation License Usage
** This file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.  Please review the following information to ensure
** the GNU Free Documentation License version 1.3 requirements
** will be met: http://www.gnu.org/copyleft/fdl.html.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "customwidget.h"

QT_BEGIN_NAMESPACE

/*!
    \class QCustomWidget

    \brief The QCustomWidget class enables Katie Designer
    to access and construct custom widgets.

    \inmodule QtUiTools

    QCustomWidget provides a interface for custom widget. The class contains
    a set of functions that must be subclassed to return basic information
    about the widget, such as its class name and the name of its header file.
    Other functions must be implemented to initialize the plugin when it is
    loaded, and to construct instances of the custom widget for \QD to use.

    When implementing a custom widget you must subclass
    QCustomWidget to expose your widget to \QD. For
    example, this is the declaration for the plugin used in the
    \l{Custom Widget Plugin Example}{Custom Widget Plugin example} that
    enables an analog clock custom widget to be used by \QD:

    \snippet examples/designer/customwidgetplugin/customwidgetplugin.h 0

    Note that the only part of the class definition that is specific
    to this particular custom widget is the class name. In addition,
    since we are implementing an interface, we must ensure that it's
    made known to the meta object system using the Q_INTERFACES()
    macro. This enables \QD to use the qobject_cast() function to
    query for supported interfaces using nothing but a QObject
    pointer.

    After \QD loads a custom widget plugin, it calls the interface's
    initialize() function to enable it to set up any resources that it
    may need.

    \QD constructs instances of the custom widget by calling the plugin's
    createWidget() function with a suitable parent widget. Plugins must
    construct and return an instance of a custom widget with the specified
    parent widget. 

    In the implementation of the class you must remember to export
    your custom widget plugin to \QD using the Q_EXPORT_PLUGIN()
    macro. For example, if a library called \c libcustomwidgetplugin.so
    (on Unix) contains a widget class called \c MyCustomWidget, we can
    export it by adding the following line to the file containing the
    plugin implementation:

    \snippet doc/src/snippets/code/doc_src_qtdesigner.cpp 14

    This macro ensures that \QD can access and construct the custom widget.
    Without this macro, there is no way for \QD to use it.

    When implementing a custom widget plugin, you build it as a
    separate library. If you want to include several custom widget
    plugins in the same library, you must in addition subclass
    QCustomWidgetPlugin.

    \warning If your custom widget plugin contains QVariant
    properties, be aware that only the following \l
    {QVariant::Type}{types} are supported:

    \list
    \o QVariant::ByteArray
    \o QVariant::Bool
    \o QVariant::Color
    \o QVariant::Cursor
    \o QVariant::Date
    \o QVariant::DateTime
    \o QVariant::Double
    \o QVariant::Int
    \o QVariant::Point
    \o QVariant::Rect
    \o QVariant::Size
    \o QVariant::SizePolicy
    \o QVariant::String
    \o QVariant::Time
    \o QVariant::UInt
    \endlist

    \sa QCustomWidgetPlugin {Creating Custom
    Widgets for Katie Designer}
*/

/*!
    Constructs an custom widget plugin with the given \a parent. This
    is invoked automatically by the Q_EXPORT_PLUGIN() macro.
*/
QCustomWidget::QCustomWidget(QObject *parent)
    : QObject(parent)
{
}

/*!
    Destroys the custom widget interface.
*/
QCustomWidget::~QCustomWidget()
{
}

/*!
    \fn QString QCustomWidget::name() const

    Returns the class name of the custom widget supplied by the interface.

    The name returned \e must be identical to the class name used for the
    custom widget.
*/

/*!
    \fn QString QCustomWidget::group() const

    Returns the name of the group to which the custom widget belongs.
*/

/*!
    \fn QString QCustomWidget::toolTip() const

    Returns a short description of the widget that can be used by \QD
    in a tool tip.
*/

/*!
    \fn QString QCustomWidget::whatsThis() const

    Returns a description of the widget that can be used by \QD in
    "What's This?" help for the widget.
*/

/*!
    \fn QString QCustomWidget::includeFile() const

    Returns the path to the include file that \l uic uses when
    creating code for the custom widget.
*/

/*!
    \fn QIcon QCustomWidget::icon() const

    Returns the icon used to represent the custom widget in \QD's
    widget box.
*/

/*!
    \fn bool QCustomWidget::isContainer() const

    Returns true if the custom widget is intended to be used as a
    container; otherwise returns false.

    Most custom widgets are not used to hold other widgets, so their
    implementations of this function will return false, but custom
    containers will return true to ensure that they behave correctly
    in \QD.
*/

/*!
    \fn QWidget *QCustomWidget::createWidget(QWidget *parent)

    Returns a new instance of the custom widget, with the given \a
    parent.
*/

/*!
    \fn bool QCustomWidget::isInitialized() const

    Returns true if the widget has been initialized; otherwise returns
    false.

    \sa initialize()
*/

/*!
    \fn void QCustomWidget::initialize()

    Initializes the widget.

    \sa isInitialized()
*/

/*!
    \fn QString QCustomWidget::domXml() const

    Returns the XML that is used to describe the custom widget's
    properties to \QD.
*/

/*!
    \fn QString QCustomWidget::codeTemplate() const

    This function is reserved for future use by \QD.

    \omit
    Returns the code template that \QD includes in forms that contain
    the custom widget when they are saved.
    \endomit
*/

/*!
    \class QCustomWidgetPlugin

    \brief The QCustomWidgetPlugin class allows you to include
    several custom widgets in one single library.

    \inmodule QtUiTools

    When implementing a custom widget plugin, you build it as a
    separate library. If you want to include several custom widget
    plugins in the same library, you must in addition subclass
    QCustomWidgetPlugin.

    QCustomWidgetPlugin contains one single
    function returning a list of the collection's
    QCustomWidget objects. For example, if you have
    several custom widgets \c CustomWidgetOne, \c CustomWidgetTwo and
    \c CustomWidgetThree, the class definition may look like this:

    \snippet doc/src/snippets/code/doc_src_qtdesigner.cpp 12

    In the class constructor you add the interfaces to your custom
    widgets to the list which you return in the customWidgets()
    function:

    \snippet doc/src/snippets/code/doc_src_qtdesigner.cpp 13

    Note that instead of exporting each custom widget plugin using the
    Q_EXPORT_PLUGIN() macro, you export the entire collection. The
    Q_EXPORT_PLUGIN() macro ensures that \QD can access and construct
    the custom widgets. Without this macro, there is no way for \QD to
    use them.

    \sa QCustomWidget, {Creating Custom Widgets for
    Katie Designer}
*/

/*!
    Constructs an custom widget collection plugin with the given
    \a parent. This is invoked automatically by the Q_EXPORT_PLUGIN()
    macro.
*/
QCustomWidgetPlugin::QCustomWidgetPlugin(QObject *parent)
    : QObject(parent)
{
}

/*!
    Destroys the custom widget collection interface.
*/
QCustomWidgetPlugin::~QCustomWidgetPlugin()
{
}

/*!
    \fn QList<QCustomWidget*> QCustomWidgetPlugin::customWidgets() const

    Returns a list of interfaces to the collection's custom widgets.
*/

QT_END_NAMESPACE

#include "moc_customwidget.h"
