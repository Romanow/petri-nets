TEMPLATE = app

QT += xmlpatterns

MOC_DIR = moc
OBJECTS_DIR = obj

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    textbrowser.cpp \
    syntaxhighlighter.cpp \
    translate.cpp \
    xmlengine.cpp \
    state.cpp \
    graphicview.cpp \
	diagramitem.cpp \
    drawer.cpp \
    petrinet.cpp \
    inputdatadialog.cpp \
    rpn.cpp

HEADERS += \
    mainwindow.h \
    textbrowser.h \
    syntaxhighlighter.h \
    translate.h \
    xmlengine.h \
    state.h \
    graphicview.h \
	diagramitem.h \
    drawer.h \
    petrinet.h \
    inputdatadialog.h \
    rpn.h

RESOURCES += \
    resource.qrc


















