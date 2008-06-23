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
	$$PWD/rosterlist.h \
	$$PWD/mainwindow.h

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
	$$PWD/rosterlist.cpp \
	$$PWD/mainwindow.cpp

