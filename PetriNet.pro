TEMPLATE = app

QT += xmlpatterns

MOC_DIR = moc
OBJECTS_DIR = obj

SOURCES += \
    main.cpp \
    manager.cpp \
    mainwindow.cpp \
    textbrowser.cpp \
    syntaxhighlighter.cpp \
    translate.cpp \
    xmlengine.cpp \
    state.cpp \
    graphicview.cpp \
    diagramitem.cpp

HEADERS += \
    manager.h \
    mainwindow.h \
    textbrowser.h \
    syntaxhighlighter.h \
    translate.h \
    xmlengine.h \
    state.h \
    graphicview.h \
    diagramitem.h

RESOURCES += \
    resource.qrc






