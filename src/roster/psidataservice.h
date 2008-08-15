#ifndef ROSTER_PSIDATASERVICE_H
#define ROSTER_PSIDATASERVICE_H

#include <QList>
#include <QIcon>

#include "rosterdataservice.h"
#include "psievent.h"

class PsiAccount;
class UserListItem;
class UserResource;
class PsiEvent;

using XMPP::Jid;

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
			const StatusType getStatus() const;
			const XMPP::Jid getJid() const;
			PsiEvent* getIncomingEvent(const XMPP::Jid& jid) const;
			const bool hasManualAvatar(const XMPP::Jid& jid) const;

		private slots:
			void updatedContact(const UserListItem& item);
			void updatedAccount();

		private:
			PsiAccount* acc_;
	};

}

#endif
