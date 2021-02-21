DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += ADS_STATIC

INCLUDEPATH += ads/src/

windows {
	# MinGW
	*-g++* {
		QMAKE_CXXFLAGS += -Wall -Wextra -pedantic
	}
	# MSVC
	*-msvc* {
                QMAKE_CXXFLAGS += /utf-8
        }
}

RESOURCES += ads/src/ads.qrc

HEADERS += \
    ads/src/ads_globals.h \
    ads/src/DockAreaWidget.h \
    ads/src/DockAreaTabBar.h \
    ads/src/DockContainerWidget.h \
    ads/src/DockManager.h \
    ads/src/DockWidget.h \
    ads/src/DockWidgetTab.h \ 
    ads/src/DockingStateReader.h \
    ads/src/FloatingDockContainer.h \
    ads/src/FloatingDragPreview.h \
    ads/src/DockOverlay.h \
    ads/src/DockSplitter.h \
    ads/src/DockAreaTitleBar_p.h \
    ads/src/DockAreaTitleBar.h \
    ads/src/ElidingLabel.h \
    ads/src/IconProvider.h \
    ads/src/DockComponentsFactory.h  \
    ads/src/DockFocusController.h


SOURCES += \
    ads/src/ads_globals.cpp \
    ads/src/DockAreaWidget.cpp \
    ads/src/DockAreaTabBar.cpp \
    ads/src/DockContainerWidget.cpp \
    ads/src/DockManager.cpp \
    ads/src/DockWidget.cpp \
    ads/src/DockingStateReader.cpp \
    ads/src/DockWidgetTab.cpp \
    ads/src/FloatingDockContainer.cpp \
    ads/src/FloatingDragPreview.cpp \
    ads/src/DockOverlay.cpp \
    ads/src/DockSplitter.cpp \
    ads/src/DockAreaTitleBar.cpp \
    ads/src/ElidingLabel.cpp \
    ads/src/IconProvider.cpp \
    ads/src/DockComponentsFactory.cpp \
    ads/src/DockFocusController.cpp


unix:!macx {
HEADERS += ads/src/linux/FloatingWidgetTitleBar.h
SOURCES += ads/src/linux/FloatingWidgetTitleBar.cpp
QT += x11extras
LIBS += -lxcb
}



