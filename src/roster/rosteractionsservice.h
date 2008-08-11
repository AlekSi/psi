#ifndef ROSTER_ROSTERACTIONSSERVICE_H
#define ROSTER_ROSTERACTIONSSERVICE_H

#include <QObject>

namespace XMPP {
	class Jid;
}

using XMPP::Jid; // PsiAccount uses Jid instead of XMPP::Jid

class PsiAccount;

namespace Roster {

	class RosterActionsService : public QObject {
		Q_OBJECT

		public:
			RosterActionsService(PsiAccount* acc);
			~RosterActionsService();

			void sendMessage(const XMPP::Jid& jid);
			void openChat(const XMPP::Jid& jid);
			void executeCommand(const XMPP::Jid& jid);
			void sendFile(const XMPP::Jid& jid);
			void removeContact(const XMPP::Jid& jid);
			void showHistory(const XMPP::Jid& jid);
			void userInfo(const XMPP::Jid& jid);
			void openWhiteboard(const XMPP::Jid& jid);
			void resendAuthTo(const XMPP::Jid& jid);
			void rerequestAuthFrom(const XMPP::Jid& jid);
			void removeAuthFrom(const XMPP::Jid& jid);
			void assignAvatar(const XMPP::Jid& jid, const QString& file);
			void clearAvatar(const XMPP::Jid& jid);

		signals:
			void actionSendMessage(const Jid& jid);
			void actionOpenChat(const Jid& jid);
			void actionExecuteCommand(const Jid& jid);
			void actionSendFile(const Jid& jid);
			void actionRemove(const Jid& jid);
			void actionHistory(const Jid& jid);
			void actionUserInfo(const Jid& jid);
			void actionOpenWhiteboard(const Jid& jid);
			void actionResendAuthTo(const Jid& jid);
			void actionRerequestAuthFrom(const Jid& jid);
			void actionRemoveAuthFrom(const Jid& jid);
			void actionAssignAvatar(const Jid& jid, const QString& file);
			void actionClearAvatar(const Jid& jid);
	};

}

#endif
