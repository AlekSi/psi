#ifndef ROSTER_ROSTERDATASERVICE_H
#define ROSTER_ROSTERDATASERVICE_H

#include <QList>
#include <QIcon>

namespace Roster {
	class XMPPRosterItem;

	class RosterDataService {
		public:
			virtual const QList<XMPPRosterItem*> getRosterItems() const = 0;
			virtual const QIcon getAvatar(const QString& jid) const = 0;
	};
}

#endif
