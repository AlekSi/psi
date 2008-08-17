#ifndef ROSTER_GLOBALS_H
#define ROSTER_GLOBALS_H

#include <Qt>
#include "common.h"
#include "xmpp_jid.h"
#include "psievent.h"

namespace Roster {

	const QString SEPARATOR = "::";

	enum Roles {
		ItemRole = Qt::UserRole,
		IdRole = Qt::UserRole + 1,
		AvatarRole = Qt::UserRole + 2,
		StatusMessageRole = Qt::UserRole + 3
	};

	enum EventType {
		NoEvent = 0,
		PGPEvent,
		FileEvent,
		RosterExchangeEvent,
		HttpAuthEvent,
		AuthEvent,
		MessageMessageEvent,
		MessageChatEvent,
		MessageHeadlineEvent,
		MessageErrorEvent
	};

	typedef XMPP::Status::Type StatusType;

	const unsigned int FILTER_OFFLINE = 1;
	const unsigned int FILTER_DND = 2;
	const unsigned int FILTER_AWAY = 4;
	const unsigned int FILTER_XA = 8;
	const unsigned int FILTER_TRANSPORTS = 16;
	const unsigned int FILTER_SELF = 32;
	const unsigned int FILTER_HIDDEN = 64;

	const unsigned int FILTER_ALL = FILTER_OFFLINE | FILTER_DND | FILTER_AWAY | 
									FILTER_XA | FILTER_TRANSPORTS | FILTER_SELF | FILTER_HIDDEN;

	const QString statusToText(const StatusType& status);

}

#endif
