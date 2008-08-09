#ifndef ROSTER_VIEWACCOUNTMANAGER_H
#define ROSTER_VIEWACCOUNTMANAGER_H

#include <QObject>

namespace XMPP {
	class Jid;
}

using XMPP::Jid;

class PsiAccount;

namespace Roster {

	class ViewAccountManager : public QObject {
		Q_OBJECT

		public:
			ViewAccountManager(PsiAccount* acc);
			~ViewAccountManager();

			void sendMessage(const XMPP::Jid& jid);

		signals:
			void actionSendMessage(const Jid& jid);
	};

}

#endif
