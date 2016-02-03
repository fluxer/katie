#!/usr/bin/python2

import sys, os, re

incmap = {
    'QtCore': {
        'QModelIndexList': 'qabstractitemmodel.h',
        'QGenericArgument': 'qobjectdefs.h',
        'QProcessEnvironment': 'qprocess.h',
        'QMutableHashIterator': 'qhash.h',
        'QLatin1Literal': 'qstringbuilder.h',
        'QObjectList': 'qobject.h',
        'QMapIterator': 'qmap.h',
        'QMutexLocker': 'qmutex.h',
        'QWeakPointer': 'qsharedpointer.h',
        'QArgument': 'qobjectdefs.h',
        'QByteRef': 'qbytearray.h',
        'QChildEvent': 'qcoreevent.h',
        'QMultiMap': 'qmap.h',
        'QMetaEnum': 'qmetaobject.h',
        'QLatin1String': 'qstring.h',
        'QModelIndex': 'qabstractitemmodel.h',
        'QPointF': 'qpoint.h',
        'QTimerEvent': 'qcoreevent.h',
        'QExplicitlySharedDataPointer': 'qsharedpointer.h',
        'QXmlStreamNamespaceDeclarations': 'qxmlstream.h',
        'QAtomicPointer': 'qatomic.h',
        'QReadLocker': 'qreadwritelock.h',
        'QWriteLocker': 'qreadwritelock.h',
        'QAtomicInt': 'qatomic.h',
        'QRectF': 'qrect.h',
        'QSizeF': 'qsize.h',
        'QBitRef': 'qbitarray.h',
        'QXmlStreamReader': 'qxmlstream.h',
        'QXmlStreamWriter': 'qxmlstream.h',
        'QtAlgorithms': 'qalgorithms.h',
        'QAbstractListModel': 'qabstractitemmodel.h',
        'QTime': 'qdatetime.h',
        'QDate': 'qdatetime.h',
        'QMetaProperty': 'qmetaobject.h',
        'QMutableListIterator': 'qlist.h',
        'QSharedDataPointer': 'qshareddata.h',
        'QMetaMethod': 'qmetaobject.h',
        'QLatin1Char': 'qchar.h',
        'QtPlugin': 'qplugin.h',
        'QCharRef': 'qstring.h',
        'QBool': 'qglobal.h',
        'QMutableStringListIterator': 'qstringlist.h',
        'QtDebug': 'qdebug.h',
        'QtEndian': 'qendian.h',
        'QBasicAtomicInt': 'qbasicatomic.h',
        'Q_PID': 'qprocess.h',
        'QMetaClassInfo': 'qmetaobject.h',
        'QFlags': 'qglobal.h',
        'QPersistentModelIndex': 'qabstractitemmodel.h',
        'QSysInfo': 'qglobal.h',
        'QMutableVectorIterator': 'qvector.h',
        'Qt': 'qnamespace.h',
        'QAbstractTableModel': 'qabstractitemmodel.h',
        'QVariantList': 'qvariant.h',
        'QHashIterator': 'qhash.h',
        'QMutableMapIterator': 'qmap.h',
        'QListIterator': 'qlist.h',
        'QVariantMap': 'qvariant.h',
        'QXmlStreamAttributes': 'qxmlstream.h',
        'QXmlStreamAttribute': 'qxmlstream.h',
        'QFileInfoList': 'qfileinfo.h',
        'QGlobalStatic': 'qglobal.h',
        'QMutableSetIterator': 'qset.h',
        'QVariantHash': 'qvariant.h',
        'QDynamicPropertyChangeEvent': 'qcoreevent.h',
        'QXmlStreamEntityResolver': 'qxmlstream.h',
        'QMutableLinkedListIterator': 'qlist.h',
        },
    'QtGui': {
        'QTextTableCell': 'qtexttable.h',
        'QPainterPathStroker': 'qpainterpath.h',
        'QGraphicsPolygonItem': 'qgraphicsitem.h',
        'QSplitterHandle': 'qsplitter.h',
        'QGraphicsDropShadowEffect': 'qgraphicseffect.h',
        'QGraphicsPixmapItem': 'qgraphicsitem.h',
        'QGraphicsRectItem': 'qgraphicsitem.h',
        'QGraphicsScale': 'qgraphicstransform.h',
        'QGraphicsItemGroup': 'qgraphicsitem.h',
        'QGraphicsLineItem': 'qgraphicsitem.h',
        'QGraphicsPathItem': 'qgraphicsitem.h',
        'QConicalGradient': 'qbrush.h',
        'QPlainTextDocumentLayout': 'qplaintextedit.h',
        'QGraphicsOpacityEffect': 'qgraphicseffect.h',
        'QPinchGesture': 'qgesture.h',
        'QGestureEvent': 'qevent.h',
        'QIconDragEvent': 'qevent.h',
        'QIconEngineFactoryInterface': 'qiconengineplugin.h',
        'QIconEngineFactoryInterfaceV2': 'qiconengineplugin.h',
        'QIconEnginePluginV2': 'qiconengineplugin.h',
        'QImageIOHandlerFactoryInterface': 'qimageiohandler.h',
        'QImageTextKeyLang': 'qimage.h',
        'QInputContextFactoryInterface': 'qinputcontextplugin.h',
        'QInputMethodEvent': 'qevent.h',
        'QItemEditorCreator': 'qitemeditorfactory.h',
        'QItemEditorCreatorBase': 'qitemeditorfactory.h',
        'QGraphicsSceneHoverEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneResizeEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneDragDropEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneContextMenuEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneHelpEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneMoveEvent': 'qgraphicssceneevent.h',
        'QGraphicsTextItem': 'qgraphicsitem.h',
        'QGraphicsSimpleTextItem': 'qgraphicsitem.h',
        'QGraphicsRotation': 'qgraphicstransform.h',
        'QGraphicsObject': 'qgraphicsitem.h',
        'QIntValidator': 'qvalidator.h',
        'QX11EmbedContainer': 'qx11embed_x11.h',
        'QPolygonF': 'qpolygon.h',
        'QGraphicsSceneWheelEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneMouseEvent': 'qgraphicssceneevent.h',
        'QResizeEvent': 'qevent.h',
        'QMouseEvent': 'qevent.h',
        'QKeyEvent': 'qevent.h',
        'QWheelEvent': 'qevent.h',
        'QDragEnterEvent': 'qevent.h',
        'QWindowStateChangeEvent': 'qevent.h',
        'QtEvents': 'qevent.h',
        'QTextLine': 'qtextlayout.h',
        'QTextFrame': 'qtextobject.h',
        'QTextCharFormat': 'qtextformat.h',
        'QUndoCommand': 'qundostack.h',
        'QTouchEvent': 'qevent.h',
        'QDropEvent': 'qevent.h',
        'QMoveEvent': 'qevent.h',
        'QMacStyle': 'qmacstyle_mac.h',
        'QStyleOptionGroupBox': 'qstyleoption.h',
        'QStyleOptionComboBox': 'qstyleoption.h',
        'QStyleOptionSpinBox': 'qstyleoption.h',
        'QStyleOptionComplex': 'qstyleoption.h',
        'QStyleOptionButton': 'qstyleoption.h',
        'QStyleOptionFrame': 'qstyleoption.h',
        'QStyleOptionFrameV2': 'qstyleoption.h',
        'QStyleOptionFrameV3': 'qstyleoption.h',
        'QStyleOptionProgressBarV2': 'qstyleoption.h',
        'QStyleOptionToolBar': 'qstyleoption.h',
        'QStyleOptionSlider': 'qstyleoption.h',
        'QStyleOptionToolButton': 'qstyleoption.h',
        'QStyleOptionRubberBand': 'qstyleoption.h',
        'QStyleOptionTabV3': 'qstyleoption.h',
        'QStyleOptionTabBarBase': 'qstyleoption.h',
        'QTextListFormat': 'qtextformat.h',
        'QGradient': 'qbrush.h',
        'QFontMetricsF': 'qfontmetrics.h',
        'QStyleOptionTitleBar': 'qstyleoption.h',
        'QStyleOptionFocusRect': 'qstyleoption.h',
        'QWidgetItem': 'qlayoutitem.h',
        'QShortcutEvent': 'qevent.h',
        'QActionEvent': 'qevent.h',
        'QStandardItem': 'qstandarditemmodel.h',
        'QAbstractUndoItem': 'qtextdocument.h',
        'QWidgetList': 'qwindowdefs.h',
        'QListWidgetItem': 'qlistwidget.h',
        'QCloseEvent': 'qevent.h',
        'QHideEvent': 'qevent.h',
        'QShowEvent': 'qevent.h',
        'QDragMoveEvent': 'qevent.h',
        'QHelpEvent': 'qevent.h',
        'QDoubleValidator': 'qvalidator.h',
        'QTreeWidgetItem': 'qtreewidget.h',
        'QTextTableFormat': 'qtextformat.h',
        'QIconEngineV2': 'qiconengine.h',
        'QItemSelection': 'qitemselectionmodel.h',
        'QContextMenuEvent': 'qevent.h',
        'QPaintEvent': 'qevent.h',
        'QHoverEvent': 'qevent.h',
        'QInputEvent': 'qevent.h',
        'QDragLeaveEvent': 'qevent.h',
        'QFileOpenEvent': 'qevent.h',
        'QAbstractGraphicsShapeItem': 'qgraphicsitem.h',
        'QItemSelectionRange': 'qitemselectionmodel.h',
        'QImageIOPlugin': 'qimageiohandler.h',
        'QFocusEvent': 'qevent.h',
        'QLinearGradient': 'qbrush.h',
        'QStyleOptionViewItem': 'qstyleoption.h',
        'QStyleOptionViewItemV2': 'qstyleoption.h',
        'QStyleOptionViewItemV3': 'qstyleoption.h',
        'QStyleOptionViewItemV4': 'qstyleoption.h',
        'QStyleOptionHeader': 'qstyleoption.h',
        'QTimeEdit': 'qdatetimeedit.h',
        'QStyleOptionMenuItem': 'qstyleoption.h',
        'QStyleOptionGraphicsItem': 'qstyleoption.h',
        'QTextBlock': 'qtextobject.h',
        'QTableWidgetItem': 'qtablewidget.h',
        'QMimeSource': 'qmime.h',
        'QDateEdit': 'qdatetimeedit.h',
        'QWizardPage': 'qwizard.h',
        'QRadialGradient': 'qbrush.h',
        'QRegExpValidator': 'qvalidator.h',
        'QAccessible2Interface': 'qaccessible2.h',
        'QAccessibleActionInterface': 'qaccessible2.h',
        'QAccessibleApplication': 'qaccessibleobject.h',
        'QAccessibleBridgeFactoryInterface': 'qaccessiblebridge.h',
        'QAccessibleBridgePlugin': 'qaccessiblebridge.h',
        'QAccessibleEditableTextInterface': 'qaccessible2.h',
        'QAccessibleFactoryInterface': 'qaccessibleplugin.h',
        'QAccessibleImageInterface': 'qaccessible2.h',
        'QAccessibleEvent': 'qaccessible.h',
        'QAccessibleInterface': 'qaccessible.h',
        'QAccessibleInterfaceEx': 'qaccessible.h',
        'QAccessibleObjectEx': 'qaccessibleobject.h',
        'QAccessibleSimpleEditableTextInterface': 'qaccessible2.h',
        'QAccessibleTable2CellInterface': 'qaccessible2.h',
        'QAccessibleTable2Interface': 'qaccessible2.h',
        'QAccessibleTableInterface': 'qaccessible2.h',
        'QTextLength': 'qtextformat.h',
        'QMacCocoaViewContainer': 'qmaccocoaviewcontainer_mac.h',
        # 'QPlatformIntegrationPlugin': 'qplatformintegrationplugin_qpa.h',
        # 'QPlatformIntegration': 'qplatformintegration_qpa.h',
        },
    'QtDBus': {
        'QDBusPendingCallWatcher': 'qdbuspendingcall.h',
        'QDBusObjectPath': 'qdbusextratypes.h',
        'QDBusAbstractInterfaceBase': 'qdbusabstractinterface.h',
        'QDBusPendingReplyData': 'qdbuspendingreply.h',
        'QDBusSignature': 'qdbusextratypes.h',
        'QDBusVariant': 'qdbusextratypes.h',
        },
    'QtOpenGL': {
        'QGLWidget': 'qgl.h',
        'QGLContext': 'qgl.h',
        'QGLShader': 'qglshaderprogram.h',
        },
    'QtXml': {
        'QXmlSimpleReader': 'qxml.h',
        'QDomAttr': 'qdom.h',
        'QDomDocument': 'qdom.h',
        'QDomElement': 'qdom.h',
        'QDomNode': 'qdom.h',
        'QXmlStreamAttribute': 'qxmlstream.h',
        'QXmlAttributes': 'qxml.h',
        'QXmlDefaultHandler': 'qxml.h',
        'QXmlParseException': 'qxml.h',
        'QXmlLocator': 'qxml.h',
        'QXmlContentHandler': 'qxml.h',
        'QXmlInputSource': 'qxml.h',
        'QDomNodeList': 'qdom.h',
        'QDomText': 'qdom.h',
        'QDomNamedNodeMap': 'qdom.h',
        },
    'QtXmlPatterns': {
        'QXmlItem': 'qabstractxmlnodemodel.h',
        'QXmlNodeModelIndex': 'qabstractxmlnodemodel.h',
        },
    'QtDeclarative': {
        'QDeclarativeListProperty': 'qdeclarativelist.h',
        },
    'QtNetwork': {
        'QNetworkProxyFactory': 'qnetworkproxy.h',
        'QNetworkProxyQuery': 'qnetworkproxy.h',
        'QHttpRequestHeader': 'qhttp.h',
        'QHttpResponseHeader': 'qhttp.h',
        'QHttpHeader': 'qhttp.h',
        'QNetworkConfigurationManager': 'qnetworkconfigmanager.h',
        'QNetworkAddressEntry': 'qnetworkinterface.h',
        'QNetworkCacheMetaData': 'qabstractnetworkcache.h',
        },
    'QtTest': {
        'QEventSizeOfChecker': 'qtestspontaneevent.h',
        'QSpontaneKeyEvent': 'qtestspontaneevent.h',
        'QTestAccessibility': 'qtestaccessible.h',
        'QTestAccessibilityEvent': 'qtestaccessible.h',
        'QTestDelayEvent': 'qtestevent.h',
        'QTestEventList': 'qtestevent.h',
        'QTestKeyClicksEvent': 'qtestevent.h',
        'QTestKeyEvent': 'qtestevent.h',
        'QTestMouseEvent': 'qtestevent.h',
        'QtTestGui': 'qtest_gui.h',
        },
    'QtDesigner': {
        'QDesignerWidgetBoxInterface': 'abstractwidgetbox.h',
        'QDesignerPropertyEditorInterface': 'abstractpropertyeditor.h',
        'QDesignerFormWindowManagerInterface': 'abstractformwindowmanager.h',
        'QDesignerMetaDataBaseInterface': 'abstractmetadatabase.h',
        'QDesignerWidgetDataBaseInterface': 'abstractwidgetdatabase.h',
        'QDesignerWidgetFactoryInterface': 'abstractwidgetfactory.h',
        'QDesignerObjectInspectorInterface': 'abstractobjectinspector.h',
        'QDesignerIntegrationInterface': 'abstractintegration.h',
        'QDesignerFormEditorInterface': 'abstractformeditor.h',
        'QDesignerPromotionInterface': 'abstractpromotioninterface.h',
        'QDesignerFormWindowInterface': 'abstractformwindow.h',
        'QDesignerLayoutDecorationExtension': 'layoutdecoration.h',
        'QDesignerBrushManagerInterface': 'abstractbrushmanager.h',
        'QAbstractFormBuilder': 'abstractformbuilder.h',
        'QDesignerIconCacheInterface': 'abstracticoncache.h',
        'QDesignerPropertySheetExtension': 'propertysheet.h',
        'QDesignerContainerExtension': 'container.h',
        'QDesignerActionEditorInterface': 'abstractactioneditor.h',
        'QDesignerCustomWidgetInterface': 'customwidget.h',
        'QDesignerLanguageExtension': 'abstractlanguage.h',
        'QDesignerResourceBrowserInterface': 'abstractresourcebrowser.h',
        'QDesignerMemberSheetExtension': 'membersheet.h',
        'QDesignerTaskMenuExtension': 'taskmenu.h',
        'QDesignerFormWindowCursorInterface': 'abstractformwindowcursor.h',
        'QDesignerWidgetDataBaseItemInterface': 'abstractwidgetdatabase.h',
        'QDesignerCustomWidgetCollectionInterface': 'customwidget.h',
        'QDesignerDynamicPropertySheetExtension': 'dynamicpropertysheet.h',
        'QDesignerFormEditorPluginInterface': 'abstractformeditorplugin.h',
        'QDesignerFormWindowToolInterface': 'abstractformwindowtool.h',
        'QDesignerExtraInfoExtension': 'extrainfo.h',
        'QExtensionFactory': 'default_extensionfactory.h',
        'QDesignerMetaDataBaseItemInterface': 'abstractmetadatabase.h',
        'QDesignerDnDItemInterface': 'abstractdnditem.h',
        'QDesignerComponents': 'qdesigner_components.h',
        },
    'QtHelp': {
        'QHelpIndexModel': 'qhelpindexwidget.h',
        'QHelpContentModel': 'qhelpcontentwidget.h',
        },
    'QtWebKit': {
        'QWebHitTestResult': 'qwebframe.h',
        },
    }


if '--printmap' in sys.argv:
    for component in incmap:
        for key in incmap[component]:
            print('katie_generate_obsolete(%s.h %s %s)' % (key.lower(), component, incmap[component][key]))
    sys.exit(0)

cppfiles = []
cmakefiles = []
for root, dirs, files in os.walk(os.curdir):
    for f in files:
        if f.endswith(('.cpp', '.cc', '.hpp', '.h')) and not '--no-cpp' in sys.argv:
            cppfiles.append('%s/%s' % (root, f))
        if f.endswith(('CMakeLists.txt', '.cmake')) and not '--no-cmake' in sys.argv:
            cmakefiles.append('%s/%s' % (root, f))

for cpp in cppfiles:
    cpp = os.path.realpath(cpp)
    cppbase = os.path.basename(cpp)
    with open(cpp, 'r') as f:
        cppcontent = f.read()
    replaced = []
    for key in incmap:
        for key2 in incmap[key]:
            inc1 = '%s/%s' % (key, key2)
            inc2 = key2
            replacement = '#include <%s/%s>' % (key, incmap[key][key2])
            # print(inc1, inc2, replacement)
            for match in re.findall('(#include [<|"](?:%s|%s)[>|"])' % (inc1, inc2), cppcontent):
                with open(cpp, 'w') as f:
                    if replacement in replaced:
                        replacement = ""
                        print('removing inclusion of %s in %s' % (match, cpp))
                    else:
                        print('adjusting inclusion of %s with %s in %s' % (match, replacement, cpp))
                    cppcontent = cppcontent.replace(match, replacement)
                    f.write(cppcontent)
                    replaced.append(replacement)

# TODO: second run on same files can cause trouble
for cmake in cmakefiles:
    cmake = os.path.realpath(cmake)
    cmakebase = os.path.basename(cmake)
    with open(cmake, 'r') as f:
        cmakecontent = f.read()
    replacement = '''
find_package(Katie)
if(NOT KATIE_FOUND)
    find_package(Qt4 %s) ## AUTOADJUSTED
endif()
'''
    for match in re.findall('(find_package.*\(.*Qt4(.*)\).*)', cmakecontent):
        if 'AUTOADJUSTED' in match[0]:
            continue
        print(match)
        with open(cmake, 'w') as f:
            print('adjusting package finding in %s' % cmake)
            # and here is the trouble..
            cmakecontent = cmakecontent.replace(match[0], replacement % match[1])
            f.write(cmakecontent)
