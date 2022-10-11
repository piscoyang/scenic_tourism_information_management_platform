QT       += core gui

QT += sql
QT += network
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    activitydetail.cpp \
    activitymanagement.cpp \
    admin.cpp \
    client.cpp \
    contact.cpp \
    forgetpsw.cpp \
    guide.cpp \
    leavemessage.cpp \
    loadmap.cpp \
    loginform.cpp \
    main.cpp \
    mainwindow.cpp \
    manager.cpp \
    order_information.cpp \
    recommend.cpp \
    reg_account.cpp \
    scenicdetail.cpp \
    scenicinfoedit.cpp \
    scenicmanagement.cpp \
    search.cpp \
    server.cpp \
    ticketbuy.cpp \
    ticketreturn.cpp \
    tourist.cpp \
    touristmanagement.cpp \
    touristmodify.cpp \
    workermanagement.cpp \
    workermodify.cpp

HEADERS += \
    activitydetail.h \
    activitymanagement.h \
    admin.h \
    client.h \
    contact.h \
    forgetpsw.h \
    guide.h \
    leavemessage.h \
    loadmap.h \
    loginform.h \
    mainwindow.h \
    manager.h \
    order_information.h \
    recommend.h \
    reg_account.h \
    scenicdetail.h \
    scenicinfoedit.h \
    scenicmanagement.h \
    search.h \
    server.h \
    ticketbuy.h \
    ticketreturn.h \
    tourist.h \
    touristmanagement.h \
    touristmodify.h \
    workermanagement.h \
    workermodify.h

FORMS += \
    activitydetail.ui \
    activitymanagement.ui \
    admin.ui \
    client.ui \
    contact.ui \
    forgetpsw.ui \
    guide.ui \
    leavemessage.ui \
    loadmap.ui \
    mainwindow.ui \
    manager.ui \
    order_information.ui \
    recommend.ui \
    reg_account.ui \
    scenicdetail.ui \
    scenicinfoedit.ui \
    scenicmanagement.ui \
    search.ui \
    server.ui \
    ticketbuy.ui \
    ticketreturn.ui \
    tourist.ui \
    touristmanagement.ui \
    touristmodify.ui \
    workermanagement.ui \
    workermodify.ui

msvc {

    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS +=/utf-8
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    C:/Users/Junhong Yang/Desktop/景区旅游信息管理平台/debug/ico.ico \


RESOURCES += \
    picture.qrc \
    scenicimg.qrc \
    sounds.qrc

RC_ICONS = ico.ico
