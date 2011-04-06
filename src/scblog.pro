# $Id: scblog.pro 52 2011-04-05 14:04:15Z kua $

TEMPLATE      = app
QT            = core network xml
CONFIG       += qt
CONFIG       += console link_pkgconfig
CONFIG       -= app_bundle
PKGCONFIG     = libwhiteboard dbus-glib-1
MOC_DIR       = bin
OBJECTS_DIR   = bin
INCLUDEPATH  += /usr/local/include/whiteboard/qt-core \
                blogservice/inc smartspace/inc core/inc
LIBS         += -L/usr/local/lib \
                -lq_whiteboard_node
HEADERS       = core/inc/BlogObject.h \
                core/inc/Post.h \
                core/inc/Comment.h \
                blogservice/inc/LjHandler.h \
                blogservice/inc/RequestCreator.h \
                blogservice/inc/ResponseParser.h \
                smartspace/inc/SSHandler.h \
                smartspace/inc/ScriboHandler.h \
                smartspace/inc/ConferenceHandler.h \
                smartspace/inc/Ontology.h \
                core/inc/Synchronizer.h
SOURCES       = core/src/BlogObject.cpp \
                core/src/Post.cpp \
                core/src/Comment.cpp \
                blogservice/src/LjHandler.cpp \
                blogservice/src/RequestCreator.cpp \
                blogservice/src/ResponseParser.cpp \
                smartspace/src/SSHandler.cpp \
                smartspace/src/ScriboHandler.cpp \
                smartspace/src/ConferenceHandler.cpp \
                core/src/Synchronizer.cpp \
                main.cpp