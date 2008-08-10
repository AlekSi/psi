#ifndef ROSTER_VIEWACCOUNTMANAGER_H
#define ROSTER_VIEWACCOUNTMANAGER_H

#include <QObject>

namespace XMPP {
	class Jid;
}

using XMPP::Jid; // PsiAccount uses Jid instead of XMPP::Jid

class PsiAccount;

namespace Roster {

	class ViewAccountManager : public QObject {
		Q_OBJECT

		public:
			ViewAccountManager(PsiAccount* acc);
			~ViewAccountManager();

			void sendMessage(const XMPP::Jid& jid);
			void openChat(const XMPP::Jid& jid);
			void executeCommand(const XMPP::Jid& jid);
			void sendFile(const XMPP::Jid& jid);
			void removeContact(const XMPP::Jid& jid);
			void showHistory(const XMPP::Jid& jid);
			void userInfo(const XMPP::Jid& jid);

		signals:
			void actionSendMessage(const Jid& jid);
			void actionOpenChat(const Jid& jid);
			void actionExecuteCommand(const Jid& jid);
			void actionSendFile(const Jid& jid);
			void actionRemove(const Jid& jid);
			void actionHistory(const Jid& jid);
			void actionUserInfo(const Jid& jid);
	};

}

#endif