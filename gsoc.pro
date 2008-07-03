DEPENDPATH *= $$PWD
MOC_DIR = .build
OBJECTS_DIR = .build
UI_DIR = .build

HEADERS += \
	$$PWD/model.h \
	$$PWD/contact.h \
	$$PWD/groupitem.h \
	$$PWD/item.h \
	$$PWD/roster.h \
	$$PWD/group.h \
	$$PWD/view.h \
	$$PWD/delegate.h \
	$$PWD/rootitem.h \
	$$PWD/mainwindow.h \
	$$PWD/resource.h \
	$$PWD/manager.h

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/model.cpp \
	$$PWD/contact.cpp \
	$$PWD/item.cpp \
	$$PWD/roster.cpp \
	$$PWD/group.cpp \
	$$PWD/groupitem.cpp \
	$$PWD/view.cpp \
	$$PWD/delegate.cpp \
	$$PWD/rootitem.cpp \
	$$PWD/mainwindow.cpp \
	$$PWD/resource.cpp \
	$$PWD/manager.cpp

