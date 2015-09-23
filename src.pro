lessThan(QT_MAJOR_VERSION, 5): error(This project requires Qt 5 or later)

CONFIG += c++11 # Needed for C++11 support.

TEMPLATE = app
TARGET = src

QT += core qml quick widgets webkit webkitwidgets network

HEADERS += \
    authform.h \
    vk.h \
    settings.h \
    appviewmodel.h \
    eventemitter.h \
    vkrequest.h \
    downloadmanager.h \
    vksession.h

SOURCES += main.cpp \
    authform.cpp \
    settings.cpp \
    appviewmodel.cpp \
    vkrequest.cpp \
    downloadmanager.cpp \
    vksession.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    main.qml

FORMS += \
    authform.ui
