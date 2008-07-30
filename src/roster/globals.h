#ifndef ROSTER_GLOBALS_H
#define ROSTER_GLOBALS_H

#include <Qt>

namespace Roster {

	enum Roles {
		ItemRole = Qt::UserRole,
		IdRole = Qt::UserRole + 1,
		AvatarRole = Qt::UserRole + 2,
		StatusMessageRole = Qt::UserRole + 3
	};

	enum StatusType {
		STATUS_ONLINE,
		STATUS_AWAY,
		STATUS_CHAT,
		STATUS_DND,
		STATUS_XA,
		STATUS_OFFLINE
	};

	const unsigned int FILTER_OFFLINE = 1;
	const unsigned int FILTER_DND = 2;
	const unsigned int FILTER_AWAY = 4;
	const unsigned int FILTER_XA = 8;

}

#endif
