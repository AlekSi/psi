#ifndef ROSTER_GLOBALS_H
#define ROSTER_GLOBALS_H

#include <Qt>

namespace Roster {

	enum Roles {
		ItemRole = Qt::UserRole,
		IdRole = Qt::UserRole + 1,
		AvatarRole = Qt::UserRole + 2,
		StatusRole = Qt::UserRole + 3
	};

	enum ShowType {
		STATUS_ONLINE,
		STATUS_AWAY,
		STATUS_CHAT,
		STATUS_DND,
		STATUS_XA,
		STATUS_OFFLINE
	};

}

#endif
