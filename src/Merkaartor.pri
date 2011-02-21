# Header files
HEADERS += Global.h \
    ./IProgressWindow.h \
    ./Maps/Coord.h \
    ./Document.h \
    ./Maps/MapTypedef.h \
    ./Maps/Painting.h \
    ./Maps/Projection.h \
    ./Maps/FeatureManipulations.h \
    ./MapView.h \
    ./TagModel.h \
    ./GotoDialog.h \
    ./TerraceDialog.h \
    ../include/builtin-ggl/ggl/extensions/gis/projections/impl/geocent.h \
    ../interfaces/IMapAdapter.h

# Source files
SOURCES += Global.cpp \
    ./Maps/Coord.cpp \
    ./Document.cpp \
    ./Maps/Painting.cpp \
    ./Maps/Projection.cpp \
    ./Maps/FeatureManipulations.cpp \
    ./MapView.cpp \
    ./Main.cpp \
    ./TagModel.cpp \
    ./GotoDialog.cpp \
    ./TerraceDialog.cpp \
    ../include/builtin-ggl/ggl/extensions/gis/projections/impl/geocent.c

# Forms
FORMS += ./AboutDialog.ui \
    ./DownloadMapDialog.ui \
    ./UploadMapDialog.ui \
    ./GotoDialog.ui \
    ./TerraceDialog.ui \
    ./MultiProperties.ui \
    PropertiesDialog.ui

# Resource file(s)
RESOURCES += ../Icons/AllIcons.qrc \
    ../share/share.qrc \
    ../Styles/Styles.qrc
OTHER_FILES += ../CHANGELOG
