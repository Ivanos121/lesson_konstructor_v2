QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutlesson.cpp \
    add_lesson.cpp \
    main.cpp \
    mainwindow.cpp \
    mergedheader.cpp \
    newlesson.cpp \
    newuser.cpp \
    vertikalheaderview.cpp

HEADERS += \
    aboutlesson.h \
    add_lesson.h \
    mainwindow.h \
    mergedheader.h \
    newlesson.h \
    newuser.h \
    vertikalheaderview.h

FORMS += \
    aboutlesson.ui \
    add_lesson.ui \
    mainwindow.ui \
    newlesson.ui \
    newuser.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
