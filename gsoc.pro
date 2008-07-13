DEPENDPATH *= $$PWD
MOC_DIR = .build
OBJECTS_DIR = .build
UI_DIR = .build

HEADERS += \
	$$PWD/model.h \
	$$PWD/contact.h \
	$$PWD/groupitem.h \
	$$PWD/item.h \
	$$PWD/account.h \
	$$PWD/group.h \
	$$PWD/view.h \
	$$PWD/delegate.h \
	$$PWD/roster.h \
	$$PWD/mainwindow.h \
	$$PWD/resource.h \
	$$PWD/manager.h \
	$$PWD/xmpprosteritem.h \
	$$PWD/rosterbuilder.h \
	$$PWD/rosterdataservice.h \
	$$PWD/dummydataservice.h \
	$$PWD/xmppresource.h \
	$$PWD/metacontact.h

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/model.cpp \
	$$PWD/contact.cpp \
	$$PWD/item.cpp \
	$$PWD/account.cpp \
	$$PWD/group.cpp \
	$$PWD/groupitem.cpp \
	$$PWD/view.cpp \
	$$PWD/delegate.cpp \
	$$PWD/roster.cpp \
	$$PWD/mainwindow.cpp \
	$$PWD/resource.cpp \
	$$PWD/manager.cpp \
	$$PWD/xmpprosteritem.cpp \
	$$PWD/rosterbuilder.cpp \
	$$PWD/dummydataservice.cpp \
	$$PWD/xmppresource.cpp \
	$$PWD/metacontact.cpp

