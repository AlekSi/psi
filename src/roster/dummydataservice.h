#ifndef ROSTER_DUMMYDATASERVICE_H
#define ROSTER_DUMMYDATASERVICE_H

#include <QList>
#include <QIcon>

#include "rosterdataservice.h"

namespace Roster {
	class XMPPRosterItem;

	class DummyDataService : public RosterDataService {
		public:
			DummyDataService();
			~DummyDataService();

			const QList<XMPPRosterItem*> getRosterItems() const;
			const QIcon getAvatar(const QString& jid) const;

		private:
			QList<XMPPRosterItem*> items_;
	};

}

#endif
