TRANSLATIONS = $$files(*.ts)

qtPrepareTool(LRELEASE, lrelease)
qtPrepareTool(LCONVERT, lconvert)
qtPrepareTool(LUPDATE, lupdate)
LUPDATE += -locations relative -no-ui-lines


addTsTargets(qt, -I../include -I../include/Qt \
    3rdparty/webkit \
    activeqt \
    corelib \
    declarative \
    gui \
    multimedia \
    network \
    opengl \
    plugins \
    script \
    scripttools \
    sql \
    svg \
    xml \
    xmlpatterns \
)
addTsTargets(designer, ../tools/designer/designer.pro)
addTsTargets(linguist, ../tools/linguist/linguist.pro)
addTsTargets(assistant, ../tools/assistant/tools/tools.pro)
addTsTargets(qt_help, ../tools/assistant/lib/lib.pro)
addTsTargets(qtconfig, ../tools/qtconfig/qtconfig.pro)
addTsTargets(qvfb, ../tools/qvfb/qvfb.pro)
addTsTargets(qmlviewer, ../tools/qml/qml.pro)

