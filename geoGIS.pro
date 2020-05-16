QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    attributetable.cpp \
    geogridindex.cpp \
    geogridindexarea.cpp \
    geojsonreader.cpp \
    geolayer.cpp \
    geomap.cpp \
    geomultipolygon.cpp \
    geoobject.cpp \
    geopoint.cpp \
    geopolygon.cpp \
    geopolyline.cpp \
    geopostgislinker.cpp \
    geoquadtreeindex.cpp \
    geoquadtreeindexarea.cpp \
    georect.cpp \
    geortreeindex.cpp \
    geortreeindexarea.cpp \
    geoshpreader.cpp \
    geosldreader.cpp \
    geostyle.cpp \
    kdecells.cpp \
    kdewidget.cpp \
    lonlat.cpp \
    main.cpp \
    mainwindow.cpp \
    myfilterproxymodel.cpp \
    openglwidget.cpp \
    spatialindex.cpp \
    spatialindexmanager.cpp


HEADERS += \
    attributetable.h \
    geogridindex.h \
    geogridindexarea.h \
    geojsonreader.h \
    geolayer.h \
    geomap.h \
    geomultipolygon.h \
    geoobject.h \
    geopoint.h \
    geopolygon.h \
    geopolyline.h \
    geopostgislinker.h \
    geoquadtreeindex.h \
    geoquadtreeindexarea.h \
    georect.h \
    geortreeindex.h \
    geortreeindexarea.h \
    geoshpreader.h \
    geosldreader.h \
    geostyle.h \
    kdecells.h \
    kdewidget.h \
    lonlat.h \
    mainwindow.h \
    myfilterproxymodel.h \
    openglwidget.h \
    spatialindex.h \
    spatialindexmanager.h

FORMS += \
    attributetable.ui \
    geopostgislinker.ui \
    kdewidget.ui \
    mainwindow.ui

RC_ICONS = 1.ico
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:LIBS += -lOpengl32 \
                -lglu32 \


#win32: LIBS += -L$$PWD/../../gdal/lib/ -lgdal

#INCLUDEPATH += $$PWD/../../gdal/include
#DEPENDPATH += $$PWD/../../gdal/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../gdal/lib/gdal.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/../../gdal/lib/libgdal.a

#win32: LIBS += -LD:/gdal/lib/ -lgdal

#INCLUDEPATH += D:/gdal/include
#DEPENDPATH += D:/gdal/include

#win32:!win32-g++: PRE_TARGETDEPS += D:/gdal/lib/gdal.lib
#else:win32-g++: PRE_TARGETDEPS += D:/gdal/lib/libgdal.a




win32: LIBS += -L$$PWD/../../gdal1/lib/ -lgdal

INCLUDEPATH += $$PWD/../../gdal1/include
DEPENDPATH += $$PWD/../../gdal1/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../gdal1/lib/gdal.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../gdal1/lib/libgdal.a

win32: LIBS += -LD:/PostgreSQL/11/lib/ -llibpq

INCLUDEPATH += D:/PostgreSQL/11/include
DEPENDPATH += D:/PostgreSQL/11/include

win32:!win32-g++: PRE_TARGETDEPS += D:/PostgreSQL/11/lib/libpq.lib




