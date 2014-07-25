#-------------------------------------------------
#
# Project created by QtCreator 2014-06-06T11:42:47
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Warp2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    messageeditor.cpp \
    sendprogressdialog.cpp \
    messagesender.cpp \
    keyListEditor.cpp \
    idcreator.cpp \
    keygenerator.cpp \
    messagedelegate.cpp \
    message.cpp

HEADERS  += mainwindow.h \
    messageeditor.h \
    sendprogressdialog.h \
    messagesender.h \
    keyListEditor.h \
    idcreator.h \
    keygenerator.h \
    warp2ServerURL.h \
    messagedelegate.h \
    message.h

FORMS    += mainwindow.ui \
    messageeditor.ui \
    sendprogressdialog.ui \
    keylisteditor.ui \
    idcreator.ui
