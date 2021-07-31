#!/usr/bin/python

import sys, os, re

incmap = {
    'QtCore': {
        'QAbstractFileEngineIterator': 'qabstractfileengine.h',
        'QAbstractListModel': 'qabstractitemmodel.h',
        'QAbstractTableModel': 'qabstractitemmodel.h',
        'QArgument': 'qobjectdefs.h',
        'QAtomicInt': 'qatomic.h',
        'QAtomicPointer': 'qatomic.h',
        'QBasicAtomicInt': 'qbasicatomic.h',
        'QBitRef': 'qbitarray.h',
        'QByteRef': 'qbytearray.h',
        'QCharRef': 'qstring.h',
        'QChildEvent': 'qcoreevent.h',
        'QContiguousCacheData': 'qcontiguouscache.h',
        'QContiguousCacheTypedData': 'qcontiguouscache.h',
        'QDate': 'qdatetime.h',
        'QDynamicPropertyChangeEvent': 'qcoreevent.h',
        'QEvent': 'qcoreevent.h',
        'QExplicitlySharedDataPointer': 'qsharedpointer.h',
        'QFileInfoList': 'qfileinfo.h',
        'QFlag': 'qglobal.h',
        'QFlags': 'qglobal.h',
        'QFutureInterfaceBase': 'qfutureinterface.h',
        'QFutureIterator': 'qfuture.h',
        'QFutureWatcherBase': 'qfuturewatcher.h',
        'QGenericArgument': 'qobjectdefs.h',
        'QGenericReturnArgument': 'qobjectdefs.h',
        'QGlobalStatic': 'qglobal.h',
        'QGlobalStaticDeleter': 'qglobal.h',
        'QHashData': 'qhash.h',
        'QHashIterator': 'qhash.h',
        'QHashNode': 'qhash.h',
        'QIncompatibleFlag': 'qglobal.h',
        'QInternal': 'qnamespace.h',
        'QLatin1Char': 'qchar.h',
        'QLatin1String': 'qstring.h',
        'QLineF': 'qline.h',
        'QLinkedListData': 'qlinkedlist.h',
        'QLinkedListIterator': 'qlinkedlist.h',
        'QLinkedListNode': 'qlinkedlist.h',
        'QListData': 'qlist.h',
        'QListIterator': 'qlist.h',
        'QMapData': 'qmap.h',
        'QMapIterator': 'qmap.h',
        'QMapNode': 'qmap.h',
        'QMapPayloadNode': 'qmap.h',
        'QMetaClassInfo': 'qmetaobject.h',
        'QMetaEnum': 'qmetaobject.h',
        'QMetaMethod': 'qmetaobject.h',
        'QMetaObjectAccessor': 'qobjectdefs.h',
        'QMetaProperty': 'qmetaobject.h',
        'QMetaTypeId': 'qmetatype.h',
        'QMetaTypeId2': 'qmetatype.h',
        'QModelIndex': 'qabstractitemmodel.h',
        'QModelIndexList': 'qabstractitemmodel.h',
        'QMultiHash': 'qhash.h',
        'QMultiMap': 'qmap.h',
        'QMutableFutureIterator': 'qfuture.h',
        'QMutableHashIterator': 'qhash.h',
        'QMutableLinkedListIterator': 'qlist.h',
        'QMutableListIterator': 'qlist.h',
        'QMutableMapIterator': 'qmap.h',
        'QMutableSetIterator': 'qset.h',
        'QMutableStringListIterator': 'qstringlist.h',
        'QMutableVectorIterator': 'qvector.h',
        'QMutexLocker': 'qmutex.h',
        'QNoDebug': 'qdebug.h',
        'QObjectData': 'qobject.h',
        'QObjectList': 'qobject.h',
        'QPersistentModelIndex': 'qabstractitemmodel.h',
        'QPointF': 'qpoint.h',
        'QProcessEnvironment': 'qprocess.h',
        'QReadLocker': 'qreadwritelock.h',
        'QRectF': 'qrect.h',
        'QReturnArgument': 'qobjectdefs.h',
        'QScopedArrayPointer': 'qscopedpointer.h',
        'QScopedPointerArrayDeleter': 'qscopedpointer.h',
        'QScopedPointerPodDeleter': 'qscopedpointer.h',
        'QSetIterator': 'qset.h',
        'QSharedDataPointer': 'qshareddata.h',
        'QSizeF': 'qsize.h',
        'QStringListIterator': 'qstringlist.h',
        'QStringRef': 'qstring.h',
        'QSysInfo': 'qglobal.h',
        'QTextDecoder': 'qtextcodec.h',
        'QTextEncoder': 'qtextcodec.h',
        'QTextStreamFunction': 'qtextstream.h',
        'QTextStreamManipulator': 'qtextstream.h',
        'QTime': 'qdatetime.h',
        'QTimerEvent': 'qcoreevent.h',
        'QTypeInfo': 'qglobal.h',
        'QVariantHash': 'qvariant.h',
        'QVariantList': 'qvariant.h',
        'QVariantMap': 'qvariant.h',
        'QVectorData': 'qvector.h',
        'QVectorIterator': 'qvector.h',
        'QVectorTypedData': 'qvector.h',
        'QWeakPointer': 'qsharedpointer.h',
        'QWriteLocker': 'qreadwritelock.h',
        'Q_PID': 'qprocess.h',
        'Qt': 'qnamespace.h',
        'QtAlgorithms': 'qalgorithms.h',
        'QtCleanUpFunction': 'qcoreapplication.h',
        'QtConfig': 'qconfig.h',
        'QtContainerFwd': 'qcontainerfwd.h',
        'QtDebug': 'qdebug.h',
        'QtEndian': 'qendian.h',
        'QtGlobal': 'qglobal.h',
        'QtMsgHandler': 'qglobal.h',
        'QtPlugin': 'qplugin.h',
        'QtPluginInstanceFunction': 'qplugin.h',
    },
    'QtDBus': {
        'QDBusAbstractInterfaceBase': 'qdbusabstractinterface.h',
        'QDBusObjectPath': 'qdbusextratypes.h',
        'QDBusPendingCallWatcher': 'qdbuspendingcall.h',
        'QDBusPendingReplyData': 'qdbuspendingreply.h',
        'QDBusSignature': 'qdbusextratypes.h',
        'QDBusVariant': 'qdbusextratypes.h',
    },
    'QtDeclarative': {
        'QDeclarativeAttachedPropertiesFunc': 'qdeclarativeprivate.h',
        'QDeclarativeListProperty': 'qdeclarativelist.h',
        'QDeclarativeListReference': 'qdeclarativelist.h',
        'QDeclarativeProperties': 'qdeclarativeproperty.h',
        'QDeclarativeTypeInfo': 'qdeclarativeprivate.h',
    },
    'QtDesigner': {
        'QAbstractExtensionFactory': 'extension.h',
        'QAbstractExtensionManager': 'extension.h',
        'QAbstractFormBuilder': 'abstractformbuilder.h',
        'QDesignerActionEditorInterface': 'abstractactioneditor.h',
        'QDesignerBrushManagerInterface': 'abstractbrushmanager.h',
        'QDesignerComponents': 'qdesigner_components.h',
        'QDesignerContainerExtension': 'container.h',
        'QDesignerCustomWidgetCollectionInterface': 'customwidget.h',
        'QDesignerCustomWidgetInterface': 'customwidget.h',
        'QDesignerDnDItemInterface': 'abstractdnditem.h',
        'QDesignerDynamicPropertySheetExtension': 'dynamicpropertysheet.h',
        'QDesignerExtraInfoExtension': 'extrainfo.h',
        'QDesignerFormEditorInterface': 'abstractformeditor.h',
        'QDesignerFormEditorPluginInterface': 'abstractformeditorplugin.h',
        'QDesignerFormWindowCursorInterface': 'abstractformwindowcursor.h',
        'QDesignerFormWindowInterface': 'abstractformwindow.h',
        'QDesignerFormWindowManagerInterface': 'abstractformwindowmanager.h',
        'QDesignerFormWindowToolInterface': 'abstractformwindowtool.h',
        'QDesignerIconCacheInterface': 'abstracticoncache.h',
        'QDesignerIntegrationInterface': 'abstractintegration.h',
        'QDesignerLanguageExtension': 'abstractlanguage.h',
        'QDesignerLayoutDecorationExtension': 'layoutdecoration.h',
        'QDesignerMemberSheetExtension': 'membersheet.h',
        'QDesignerMetaDataBaseInterface': 'abstractmetadatabase.h',
        'QDesignerMetaDataBaseItemInterface': 'abstractmetadatabase.h',
        'QDesignerObjectInspectorInterface': 'abstractobjectinspector.h',
        'QDesignerPromotionInterface': 'abstractpromotioninterface.h',
        'QDesignerPropertyEditorInterface': 'abstractpropertyeditor.h',
        'QDesignerPropertySheetExtension': 'propertysheet.h',
        'QDesignerResourceBrowserInterface': 'abstractresourcebrowser.h',
        'QDesignerTaskMenuExtension': 'taskmenu.h',
        'QDesignerWidgetBoxInterface': 'abstractwidgetbox.h',
        'QDesignerWidgetDataBaseInterface': 'abstractwidgetdatabase.h',
        'QDesignerWidgetDataBaseItemInterface': 'abstractwidgetdatabase.h',
        'QDesignerWidgetFactoryInterface': 'abstractwidgetfactory.h',
        'QExtensionFactory': 'default_extensionfactory.h',
        'QFormBuilder': 'formbuilder.h',
    },
    'QtGui': {
        'QAbstractGraphicsShapeItem': 'qgraphicsitem.h',
        'QAbstractUndoItem': 'qtextdocument.h',
        'QAccessible2Interface': 'qaccessible2.h',
        'QAccessibleActionInterface': 'qaccessible2.h',
        'QAccessibleApplication': 'qaccessibleobject.h',
        'QAccessibleBridgeFactoryInterface': 'qaccessiblebridge.h',
        'QAccessibleBridgePlugin': 'qaccessiblebridge.h',
        'QAccessibleEditableTextInterface': 'qaccessible2.h',
        'QAccessibleEvent': 'qaccessible.h',
        'QAccessibleFactoryInterface': 'qaccessibleplugin.h',
        'QAccessibleImageInterface': 'qaccessible2.h',
        'QAccessibleInterface': 'qaccessible.h',
        'QAccessibleInterfaceEx': 'qaccessible.h',
        'QAccessibleObjectEx': 'qaccessibleobject.h',
        'QAccessibleSimpleEditableTextInterface': 'qaccessible2.h',
        'QAccessibleTable2CellInterface': 'qaccessible2.h',
        'QAccessibleTable2Interface': 'qaccessible2.h',
        'QAccessibleTableInterface': 'qaccessible2.h',
        'QAccessibleTextInterface': 'qaccessible2.h',
        'QAccessibleValueInterface': 'qaccessible2.h',
        'QAccessibleWidgetEx': 'qaccessiblewidget.h',
        'QActionEvent': 'qevent.h',
        'QAnimationDriver': 'qabstractanimation.h',
        'QBrushData': 'qbrush.h',
        'QClipboardEvent': 'qevent.h',
        'QCloseEvent': 'qevent.h',
        'QConicalGradient': 'qbrush.h',
        'QContextMenuEvent': 'qevent.h',
        'QDateEdit': 'qdatetimeedit.h',
        'QDoubleSpinBox': 'qspinbox.h',
        'QDoubleValidator': 'qvalidator.h',
        'QDragEnterEvent': 'qevent.h',
        'QDragLeaveEvent': 'qevent.h',
        'QDragMoveEvent': 'qevent.h',
        'QDropEvent': 'qevent.h',
        'QFocusEvent': 'qevent.h',
        'QFontMetricsF': 'qfontmetrics.h',
        'QGestureEvent': 'qevent.h',
        'QGradient': 'qbrush.h',
        'QGradientStop': 'qbrush.h',
        'QGradientStops': 'qbrush.h',
        'QGraphicsAnchor': 'qgraphicsanchorlayout.h',
        'QGraphicsBlurEffect': 'qgraphicseffect.h',
        'QGraphicsColorizeEffect': 'qgraphicseffect.h',
        'QGraphicsDropShadowEffect': 'qgraphicseffect.h',
        'QGraphicsEllipseItem': 'qgraphicsitem.h',
        'QGraphicsItemGroup': 'qgraphicsitem.h',
        'QGraphicsLineItem': 'qgraphicsitem.h',
        'QGraphicsObject': 'qgraphicsitem.h',
        'QGraphicsOpacityEffect': 'qgraphicseffect.h',
        'QGraphicsPathItem': 'qgraphicsitem.h',
        'QGraphicsPixmapItem': 'qgraphicsitem.h',
        'QGraphicsPolygonItem': 'qgraphicsitem.h',
        'QGraphicsRectItem': 'qgraphicsitem.h',
        'QGraphicsRotation': 'qgraphicstransform.h',
        'QGraphicsScale': 'qgraphicstransform.h',
        'QGraphicsSceneContextMenuEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneDragDropEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneHelpEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneHoverEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneMouseEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneMoveEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneResizeEvent': 'qgraphicssceneevent.h',
        'QGraphicsSceneWheelEvent': 'qgraphicssceneevent.h',
        'QGraphicsSimpleTextItem': 'qgraphicsitem.h',
        'QGraphicsTextItem': 'qgraphicsitem.h',
        'QHBoxLayout': 'qboxlayout.h',
        'QHelpEvent': 'qevent.h',
        'QHideEvent': 'qevent.h',
        'QHoverEvent': 'qevent.h',
        'QIconEngineFactoryInterface': 'qiconengineplugin.h',
        'QIconEngineFactoryInterfaceV2': 'qiconengineplugin.h',
        'QIconEnginePluginV2': 'qiconengineplugin.h',
        'QIconEngineV2': 'qiconengine.h',
        'QImageIOHandlerFactoryInterface': 'qimageiohandler.h',
        'QImageIOPlugin': 'qimageiohandler.h',
        'QInputContextFactoryInterface': 'qinputcontextplugin.h',
        'QInputEvent': 'qevent.h',
        'QInputMethodEvent': 'qevent.h',
        'QIntValidator': 'qvalidator.h',
        'QItemEditorCreator': 'qitemeditorfactory.h',
        'QItemEditorCreatorBase': 'qitemeditorfactory.h',
        'QItemSelection': 'qitemselectionmodel.h',
        'QItemSelectionRange': 'qitemselectionmodel.h',
        'QKeyEvent': 'qevent.h',
        'QLinearGradient': 'qbrush.h',
        'QListWidgetItem': 'qlistwidget.h',
        'QMatrix2x2': 'qgenericmatrix.h',
        'QMatrix2x3': 'qgenericmatrix.h',
        'QMatrix2x4': 'qgenericmatrix.h',
        'QMatrix3x2': 'qgenericmatrix.h',
        'QMatrix3x3': 'qgenericmatrix.h',
        'QMatrix3x4': 'qgenericmatrix.h',
        'QMatrix4x2': 'qgenericmatrix.h',
        'QMatrix4x3': 'qgenericmatrix.h',
        'QMouseEvent': 'qevent.h',
        'QMoveEvent': 'qevent.h',
        'QPaintEngineState': 'qpaintengine.h',
        'QPaintEvent': 'qevent.h',
        'QPainterPathPrivate': 'qpainterpath.h',
        'QPainterPathStroker': 'qpainterpath.h',
        'QPanGesture': 'qgesture.h',
        'QPinchGesture': 'qgesture.h',
        'QPlainTextDocumentLayout': 'qplaintextedit.h',
        'QPolygonF': 'qpolygon.h',
        'QRadialGradient': 'qbrush.h',
        'QRegExpValidator': 'qvalidator.h',
        'QResizeEvent': 'qevent.h',
        'QShortcutEvent': 'qevent.h',
        'QShowEvent': 'qevent.h',
        'QSpacerItem': 'qlayoutitem.h',
        'QSplitterHandle': 'qsplitter.h',
        'QStandardItem': 'qstandarditemmodel.h',
        'QStandardItemEditorCreator': 'qitemeditorfactory.h',
        'QStatusTipEvent': 'qevent.h',
        'QStyleFactoryInterface': 'qstyleplugin.h',
        'QStyleHintReturn': 'qstyleoption.h',
        'QStyleHintReturnMask': 'qstyleoption.h',
        'QStyleHintReturnVariant': 'qstyleoption.h',
        'QStyleOptionButton': 'qstyleoption.h',
        'QStyleOptionComboBox': 'qstyleoption.h',
        'QStyleOptionComplex': 'qstyleoption.h',
        'QStyleOptionDockWidget': 'qstyleoption.h',
        'QStyleOptionDockWidgetV2': 'qstyleoption.h',
        'QStyleOptionFocusRect': 'qstyleoption.h',
        'QStyleOptionFrame': 'qstyleoption.h',
        'QStyleOptionFrameV2': 'qstyleoption.h',
        'QStyleOptionFrameV3': 'qstyleoption.h',
        'QStyleOptionGraphicsItem': 'qstyleoption.h',
        'QStyleOptionGroupBox': 'qstyleoption.h',
        'QStyleOptionHeader': 'qstyleoption.h',
        'QStyleOptionMenuItem': 'qstyleoption.h',
        'QStyleOptionProgressBar': 'qstyleoption.h',
        'QStyleOptionProgressBarV2': 'qstyleoption.h',
        'QStyleOptionRubberBand': 'qstyleoption.h',
        'QStyleOptionSizeGrip': 'qstyleoption.h',
        'QStyleOptionSlider': 'qstyleoption.h',
        'QStyleOptionSpinBox': 'qstyleoption.h',
        'QStyleOptionTab': 'qstyleoption.h',
        'QStyleOptionTabBarBase': 'qstyleoption.h',
        'QStyleOptionTabBarBaseV2': 'qstyleoption.h',
        'QStyleOptionTabV2': 'qstyleoption.h',
        'QStyleOptionTabV3': 'qstyleoption.h',
        'QStyleOptionTabWidgetFrame': 'qstyleoption.h',
        'QStyleOptionTabWidgetFrameV2': 'qstyleoption.h',
        'QStyleOptionTitleBar': 'qstyleoption.h',
        'QStyleOptionToolBar': 'qstyleoption.h',
        'QStyleOptionToolBox': 'qstyleoption.h',
        'QStyleOptionToolBoxV2': 'qstyleoption.h',
        'QStyleOptionToolButton': 'qstyleoption.h',
        'QStyleOptionViewItem': 'qstyleoption.h',
        'QStyleOptionViewItemV2': 'qstyleoption.h',
        'QStyleOptionViewItemV3': 'qstyleoption.h',
        'QStyleOptionViewItemV4': 'qstyleoption.h',
        'QSwipeGesture': 'qgesture.h',
        'QTableWidgetItem': 'qtablewidget.h',
        'QTableWidgetSelectionRange': 'qtablewidget.h',
        'QTapAndHoldGesture': 'qgesture.h',
        'QTapGesture': 'qgesture.h',
        'QTextBlock': 'qtextobject.h',
        'QTextBlockFormat': 'qtextformat.h',
        'QTextBlockGroup': 'qtextobject.h',
        'QTextBlockUserData': 'qtextobject.h',
        'QTextCharFormat': 'qtextformat.h',
        'QTextFragment': 'qtextobject.h',
        'QTextFrame': 'qtextobject.h',
        'QTextFrameFormat': 'qtextformat.h',
        'QTextFrameLayoutData': 'qtextobject.h',
        'QTextImageFormat': 'qtextformat.h',
        'QTextInlineObject': 'qtextlayout.h',
        'QTextItem': 'qpaintengine.h',
        'QTextLength': 'qtextformat.h',
        'QTextLine': 'qtextlayout.h',
        'QTextListFormat': 'qtextformat.h',
        'QTextObjectInterface': 'qabstracttextdocumentlayout.h',
        'QTextTableCell': 'qtexttable.h',
        'QTextTableCellFormat': 'qtextformat.h',
        'QTextTableFormat': 'qtextformat.h',
        'QTileRules': 'qdrawutil.h',
        'QTimeEdit': 'qdatetimeedit.h',
        'QTouchEvent': 'qevent.h',
        'QTreeWidgetItem': 'qtreewidget.h',
        'QUndoCommand': 'qundostack.h',
        'QUnixPrintWidget': 'qprintdialog.h',
        'QUpdateLaterEvent': 'qevent.h',
        'QVBoxLayout': 'qboxlayout.h',
        'QWhatsThisClickedEvent': 'qevent.h',
        'QWheelEvent': 'qevent.h',
        'QWidgetData': 'qwidget.h',
        'QWidgetItem': 'qlayoutitem.h',
        'QWidgetItemV2': 'qlayoutitem.h',
        'QWidgetList': 'qwindowdefs.h',
        'QWidgetMapper': 'qwindowdefs.h',
        'QWidgetSet': 'qwindowdefs.h',
        'QWindowStateChangeEvent': 'qevent.h',
        'QWizardPage': 'qwizard.h',
        'QX11EmbedContainer': 'qx11embed_x11.h',
        'QX11EmbedWidget': 'qx11embed_x11.h',
        'QX11Info': 'qx11info_x11.h',
        'QtEvents': 'qevent.h',
    },
    'QtNetwork': {
        'QHttpHeader': 'qhttp.h',
        'QHttpPart': 'qhttpmultipart.h',
        'QHttpRequestHeader': 'qhttp.h',
        'QHttpResponseHeader': 'qhttp.h',
        'QIPv6Address': 'qhostaddress.h',
        'QNetworkAddressEntry': 'qnetworkinterface.h',
        'QNetworkCacheMetaData': 'qabstractnetworkcache.h',
        'QNetworkConfigurationManager': 'qnetworkconfigmanager.h',
        'QNetworkProxyFactory': 'qnetworkproxy.h',
        'QNetworkProxyQuery': 'qnetworkproxy.h',
        'Q_IPV6ADDR': 'qhostaddress.h',
    },
    'QtSql': {
        'QSqlDriverFactoryInterface': 'qsqldriverplugin.h',
        'QSqlRelation': 'qsqlrelationaltablemodel.h',
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
    'QtXml': {
        'QDomAttr': 'qdom.h',
        'QDomCDATASection': 'qdom.h',
        'QDomCharacterData': 'qdom.h',
        'QDomComment': 'qdom.h',
        'QDomDocument': 'qdom.h',
        'QDomDocumentFragment': 'qdom.h',
        'QDomDocumentType': 'qdom.h',
        'QDomElement': 'qdom.h',
        'QDomEntity': 'qdom.h',
        'QDomEntityReference': 'qdom.h',
        'QDomImplementation': 'qdom.h',
        'QDomNamedNodeMap': 'qdom.h',
        'QDomNode': 'qdom.h',
        'QDomNodeList': 'qdom.h',
        'QDomNotation': 'qdom.h',
        'QDomProcessingInstruction': 'qdom.h',
        'QDomText': 'qdom.h',
        'QXmlAttributes': 'qxml.h',
        'QXmlContentHandler': 'qxml.h',
        'QXmlDTDHandler': 'qxml.h',
        'QXmlDeclHandler': 'qxml.h',
        'QXmlDefaultHandler': 'qxml.h',
        'QXmlEntityResolver': 'qxml.h',
        'QXmlErrorHandler': 'qxml.h',
        'QXmlInputSource': 'qxml.h',
        'QXmlLexicalHandler': 'qxml.h',
        'QXmlLocator': 'qxml.h',
        'QXmlNamespaceSupport': 'qxml.h',
        'QXmlParseException': 'qxml.h',
        'QXmlReader': 'qxml.h',
        'QXmlSimpleReader': 'qxml.h',
        'QXmlStreamAttribute': 'qxmlstream.h',
        'QXmlStreamAttributes': 'qxmlstream.h',
        'QXmlStreamEntityDeclaration': 'qxmlstream.h',
        'QXmlStreamEntityDeclarations': 'qxmlstream.h',
        'QXmlStreamEntityResolver': 'qxmlstream.h',
        'QXmlStreamNamespaceDeclaration': 'qxmlstream.h',
        'QXmlStreamNamespaceDeclarations': 'qxmlstream.h',
        'QXmlStreamNotationDeclaration': 'qxmlstream.h',
        'QXmlStreamNotationDeclarations': 'qxmlstream.h',
        'QXmlStreamReader': 'qxmlstream.h',
        'QXmlStreamWriter': 'qxmlstream.h',
    },
}

if '--printmap' in sys.argv:
    for component in incmap:
        for key in incmap[component]:
            print('katie_generate_obsolete(%s %s %s)' % (key, component, incmap[component][key]))
    sys.exit(0)
elif '--sortmap' in sys.argv:
    for component in sorted(incmap):
        print("    '%s': {" % component)
        for key in sorted(incmap[component]):
            print("        '%s': '%s'," % (key, incmap[component][key]))
        print("    },")
    sys.exit(0)

cppfiles = []
for root, dirs, files in os.walk(os.curdir):
    for f in files:
        if f.endswith(('.cpp', '.cc', '.hpp', '.h')):
            cppfiles.append('%s/%s' % (root, f))

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
