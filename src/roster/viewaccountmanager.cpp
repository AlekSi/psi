#include "viewaccountmanager.h"
#include "psiaccount.h"

using XMPP::Jid;

namespace Roster {
	
	ViewAccountManager::ViewAccountManager(PsiAccount* acc) {
		connect(this, SIGNAL(actionSendMessage(const Jid&)), acc, SLOT(actionSendMessage(const Jid&)));
	}

	ViewAccountManager::~ViewAccountManager() {
	}

	void ViewAccountManager::sendMessage(const XMPP::Jid& jid) {
		emit actionSendMessage(jid);
	}

}
