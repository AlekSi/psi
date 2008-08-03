#ifndef ROSTER_ROSTERDATASERVICE_H
#define ROSTER_ROSTERDATASERVICE_H

#include <QList>
#include <QIcon>

namespace XMPP {
	class Jid;
}

namespace Roster {
	class XMPPRosterItem;

	class RosterDataService : public QObject {
		Q_OBJECT

		public:
			virtual const QList<XMPPRosterItem*> getRosterItems() const = 0;
			virtual const QIcon getAvatar(const XMPP::Jid& jid) const = 0;

		signals:
			void itemUpdated(const XMPPRosterItem* xitem, const QString& acname);
	};
}

#endif
