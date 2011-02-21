# see http://merkaartor.be/wiki/merkaartor/Compiling

#Static config
include (Config.pri)

#Custom config
include(Custom.pri)

include(../3rdparty/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.pri)
include(../3rdparty/qttoolbardialog-2.2_1-opensource/src/qttoolbardialog.pri)

#Qt Version
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)

DEFINES += VERSION=$$VERSION
DEFINES += REVISION=$$REVISION

contains(PORTABLE,1): DEFINES += PORTABLE_BUILD

TEMPLATE = app
TARGET = merkaartor

CONFIG += rtti stl
#CONFIG -= exceptions
QT += svg network xml core gui
win32-msvc* {
    LIBS += -lzlib
} else {
    LIBS += -lz
}

!contains(NODEBUG,1) {
    CONFIG += debug
    #OBJECTS_DIR += $$PWD/../tmp/$$(QMAKESPEC)/obj_debug
}
contains(NODEBUG,1) {
    CONFIG += release
    DEFINES += NDEBUG
    #OBJECTS_DIR += $$PWD/../tmp/$$(QMAKESPEC)/obj_release
}
COMMON_DIR=$$PWD/../binaries
OUTPUT_DIR=$$PWD/../binaries/$$(QMAKESPEC)
DESTDIR = $$OUTPUT_DIR/bin

#UI_DIR += $$PWD/../tmp/$$(QMAKESPEC)
#MOC_DIR += $$PWD/../tmp/$$(QMAKESPEC)
#RCC_DIR += $$PWD/../tmp/$$(QMAKESPEC)
INCLUDEPATH += $$MOC_DIR


INCLUDEPATH += $$PWD/../include $$PWD/../interfaces
DEPENDPATH += $$PWD/../interfaces

INCLUDEPATH += $$PWD/../include/builtin-ggl
unix {
    contains(USE_BUILTIN_BOOST,1) {
        INCLUDEPATH += $$PWD/../include/builtin-boost
    }
} else {
    INCLUDEPATH += $$PWD/../include/builtin-boost
}
win32 {
    INCLUDEPATH += $$COMMON_DIR/include
    LIBS += -L$$COMMON_DIR/lib
    RC_FILE = $$PWD/../Icons/merkaartor-win32.rc
}

macx {
    RC_FILE = $$PWD/../Icons/merkaartor.icns
    QMAKE_INFO_PLIST = $$PWD/../macos/Info.plist
    INCLUDEPATH += /opt/local/include
}

contains(NVIDIA_HACK,1) {
    DEFINES += ENABLE_NVIDIA_HACK
}

INCLUDEPATH += $$PWD Render qextserialport GPS NameFinder
DEPENDPATH += $$PWD Render qextserialport GPS NameFinder

TRANSLATIONS += \
    ../translations/merkaartor_ar.ts \
    ../translations/merkaartor_cs.ts \
    ../translations/merkaartor_de.ts \
    ../translations/merkaartor_es.ts \
    ../translations/merkaartor_et.ts \
    ../translations/merkaartor_fr.ts \
    ../translations/merkaartor_hr.ts \
    ../translations/merkaartor_hu.ts \
    ../translations/merkaartor_it.ts \
    ../translations/merkaartor_ja.ts \
    ../translations/merkaartor_nl.ts \
    ../translations/merkaartor_pl.ts \
    ../translations/merkaartor_pt.ts \
    ../translations/merkaartor_ru.ts \
    ../translations/merkaartor_sk.ts \
    ../translations/merkaartor_sv.ts \
    ../translations/merkaartor_uk.ts \
    ../translations/merkaartor_pt_BR.ts \

BINTRANSLATIONS += \
    ../translations/merkaartor_ar.qm \
    ../translations/merkaartor_cs.qm \
    ../translations/merkaartor_de.qm \
    ../translations/merkaartor_es.qm \
    ../translations/merkaartor_et.qm \
    ../translations/merkaartor_fr.qm \
    ../translations/merkaartor_hr.qm \
    ../translations/merkaartor_hu.qm \
    ../translations/merkaartor_it.qm \
    ../translations/merkaartor_ja.qm \
    ../translations/merkaartor_nl.qm \
    ../translations/merkaartor_pl.qm \
    ../translations/merkaartor_pt.qm \
    ../translations/merkaartor_ru.qm \
    ../translations/merkaartor_sk.qm \
    ../translations/merkaartor_sv.qm \
    ../translations/merkaartor_uk.qm \
    ../translations/merkaartor_pt_BR.qm \

#Include file(s)
include(Merkaartor.pri)
include(Backend/Backend.pri)
include(../interfaces/Interfaces.pri)
include(PaintStyle/PaintStyle.pri)
include(PaintStyle/PaintStyleEditor.pri)
include(Features/Features.pri)
include(Layers/Layers.pri)
include(Preferences/Preferences.pri)
include(Sync/Sync.pri)
include(Commands/Commands.pri)
include(Interactions/Interactions.pri)
include (Docks/Docks.pri)
include(QMapControl.pri)
include(ImportExport/ImportExport.pri)
include(Render/Render.pri)
!symbian:include(qextserialport/qextserialport.pri)
include(GPS/GPS.pri)
include(Tools/Tools.pri)
include(TagTemplate/TagTemplate.pri)
include(NameFinder/NameFinder.pri)
include(Utils/Utils.pri)

# Header files
HEADERS += \
    ./MainWindow.h

SOURCES += \
    ./MainWindow.cpp

# Forms
FORMS += \
    ./MainWindow.ui

unix {
    # Prefix: base instalation directory
    isEmpty( PREFIX ) {
        PREFIX = /usr/local
    }
    isEmpty( LIBDIR ) {
        LIBDIR = $${PREFIX}/lib${LIB_SUFFIX}
    }
    DEFINES += PLUGINS_DIR=$${LIBDIR}/merkaartor/plugins
    target.path = $${PREFIX}/bin
    SHARE_DIR = $${PREFIX}/share/merkaartor

    isEmpty(TRANSDIR_MERKAARTOR) {
        TRANSDIR_MERKAARTOR = $${SHARE_DIR}/translations
    }
}
win32 {
    DEFINES += PLUGINS_DIR=plugins
    SHARE_DIR = share
    isEmpty(TRANSDIR_MERKAARTOR) {
        TRANSDIR_MERKAARTOR = translations
    }
    isEmpty(TRANSDIR_SYSTEM) {
        TRANSDIR_SYSTEM = translations
    }
}

DEFINES += SHARE_DIR=$${SHARE_DIR}
INSTALLS += target

win32-msvc* {
    DEFINES += _USE_MATH_DEFINES
}


translations.path =  $${TRANSDIR_MERKAARTOR}
translations.files = $${BINTRANSLATIONS}
DEFINES += TRANSDIR_MERKAARTOR=$$translations.path
INSTALLS += translations

count(TRANSDIR_SYSTEM, 1) {
    DEFINES += TRANSDIR_SYSTEM=$${TRANSDIR_SYSTEM}
}

contains(MOBILE,1) {
    DEFINES += _MOBILE
    win32-wince* {
      DEFINES += _WINCE
    }
}

contains(GEOIMAGE, 1) {
    include(Docks/GeoImage.pri)
}

lists.path = $${SHARE_DIR}
lists.files = \
    $$PWD/../share/BookmarksList.xml \
    $$PWD/../share/Projections.xml \
    $$PWD/../share/WmsServersList.xml \
    $$PWD/../share/TmsServersList.xml
INSTALLS += lists

win32 {
    win32-msvc*:LIBS += -lgdal_i
    win32-g++:LIBS += -lgdal
}
unix {
    LIBS += $$system(gdal-config --libs)
    QMAKE_CXXFLAGS += $$system(gdal-config --cflags)
    QMAKE_CFLAGS += $$system(gdal-config --cflags)
}

LIBS += -lproj

contains (SPATIALITE, 1) {
    DEFINES += USE_SPATIALITE
    LIBS += -lspatialite
}
contains (PROTOBUF, 1) {
    DEFINES += USE_PROTOBUF
}

desktop.path = $${PREFIX}/share/applications
desktop.files = merkaartor.desktop
desktopicons.path = $${PREFIX}/share/icons/hicolor/48x48/apps/
desktopicons.files = $$PWD/../Icons/48x48/merkaartor.png
INSTALLS += desktop desktopicons


