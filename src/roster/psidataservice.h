#ifndef ROSTER_PSIDATASERVICE_H
#define ROSTER_PSIDATASERVICE_H

#include <QList>
#include <QIcon>

#include "rosterdataservice.h"

class PsiAccount;
class UserListItem;
class UserResource;

namespace Roster {
	class XMPPRosterItem;
	class XMPPResource;

	class PsiDataService : public RosterDataService {
		Q_OBJECT

		public:
			PsiDataService(PsiAccount* acc);
			~PsiDataService();

			const QList<XMPPRosterItem*> getRosterItems() const;
			const QIcon getAvatar(const XMPP::Jid& jid) const;
			const bool isTransport(const XMPP::Jid& jid) const;
			const XMPPRosterItem* getSelf() const;

		private slots:
			void updatedContact(const UserListItem& item);
			void updatedAccount();

		private:
			XMPPRosterItem* buildRosterItem(const UserListItem* item) const;
			XMPPResource* buildResource(const UserResource& res) const;

			PsiAccount* acc_;
	};

}

#endif
