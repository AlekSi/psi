#ifndef ROSTER_GLOBALS_H
#define ROSTER_GLOBALS_H

#include <Qt>
#include "common.h"

namespace Roster {

	enum Roles {
		ItemRole = Qt::UserRole,
		IdRole = Qt::UserRole + 1,
		AvatarRole = Qt::UserRole + 2,
		StatusMessageRole = Qt::UserRole + 3
	};

	typedef XMPP::Status::Type StatusType;

	const unsigned int FILTER_OFFLINE = 1;
	const unsigned int FILTER_DND = 2;
	const unsigned int FILTER_AWAY = 4;
	const unsigned int FILTER_XA = 8;

}

#endif
