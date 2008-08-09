#include "viewaccountmanager.h"
#include "psiaccount.h"

using XMPP::Jid;

namespace Roster {
	
	ViewAccountManager::ViewAccountManager(PsiAccount* acc) {
		connect(this, SIGNAL(actionSendMessage(const Jid&)), acc, SLOT(actionSendMessage(const Jid&)));
		connect(this, SIGNAL(actionOpenChat(const Jid&)), acc, SLOT(actionOpenChat(const Jid&)));
		connect(this, SIGNAL(actionExecuteCommand(const Jid&)), acc, SLOT(actionExecuteCommand(const Jid&)));
		connect(this, SIGNAL(actionSendFile(const Jid&)), acc, SLOT(actionSendFile(const Jid&)));
		connect(this, SIGNAL(actionRemove(const Jid&)), acc, SLOT(actionRemove(const Jid&)));
		connect(this, SIGNAL(actionHistory(const Jid&)), acc, SLOT(actionHistory(const Jid&)));
		connect(this, SIGNAL(actionUserInfo(const Jid&)), acc, SLOT(actionInfo(const Jid&)));
	}

	ViewAccountManager::~ViewAccountManager() {
	}

	void ViewAccountManager::sendMessage(const XMPP::Jid& jid) {
		emit actionSendMessage(jid);
	}

	void ViewAccountManager::openChat(const XMPP::Jid& jid) {
		emit actionOpenChat(jid);
	}

	void ViewAccountManager::executeCommand(const XMPP::Jid& jid) {
		emit actionExecuteCommand(jid);
	}

	void ViewAccountManager::sendFile(const XMPP::Jid& jid) {
		emit actionSendFile(jid);
	}

	void ViewAccountManager::removeContact(const XMPP::Jid& jid) {
		emit actionRemove(jid);
	}

	void ViewAccountManager::showHistory(const XMPP::Jid& jid) {
		emit actionHistory(jid);
	}

	void ViewAccountManager::userInfo(const XMPP::Jid& jid) {
		emit actionUserInfo(jid);
	}

}
