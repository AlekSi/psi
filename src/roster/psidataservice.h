#ifndef ROSTER_PSIDATASERVICE_H
#define ROSTER_PSIDATASERVICE_H

#include <QList>
#include <QIcon>

#include "rosterdataservice.h"

class PsiAccount;
class UserListItem;

namespace Roster {
	class XMPPRosterItem;

	class PsiDataService : public RosterDataService {
		Q_OBJECT

		public:
			PsiDataService(PsiAccount* acc);
			~PsiDataService();

			const QList<XMPPRosterItem*> getRosterItems() const;
			const QIcon getAvatar(const XMPP::Jid& jid) const;

		private slots:
			void updatedContact(const UserListItem& item);

		private:
			PsiAccount* acc_;
	};

}

#endif
