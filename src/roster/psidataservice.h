#ifndef ROSTER_PSIDATASERVICE_H
#define ROSTER_PSIDATASERVICE_H

#include <QList>
#include <QIcon>

#include "rosterdataservice.h"

class PsiAccount;
class UserListItem;
class UserResource;

namespace Roster {

	class PsiDataService : public RosterDataService {
		Q_OBJECT

		public:
			PsiDataService(PsiAccount* acc);
			~PsiDataService();

			const QList<UserListItem*> getRosterItems() const;
			const QIcon getAvatar(const XMPP::Jid& jid) const;
			const bool isTransport(const XMPP::Jid& jid) const;
			const UserListItem* getSelf() const;
			const bool isEnabled() const;

		private slots:
			void updatedContact(const UserListItem& item);
			void updatedAccount();

		private:
			PsiAccount* acc_;
	};

}

#endif
