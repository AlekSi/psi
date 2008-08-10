#include "rosteractionsservice.h"
#include "psiaccount.h"

using XMPP::Jid;

namespace Roster {
	
	RosterActionsService::RosterActionsService(PsiAccount* acc) {
		connect(this, SIGNAL(actionSendMessage(const Jid&)), acc, SLOT(actionSendMessage(const Jid&)));
		connect(this, SIGNAL(actionOpenChat(const Jid&)), acc, SLOT(actionOpenChat(const Jid&)));
		connect(this, SIGNAL(actionExecuteCommand(const Jid&)), acc, SLOT(actionExecuteCommand(const Jid&)));
		connect(this, SIGNAL(actionSendFile(const Jid&)), acc, SLOT(actionSendFile(const Jid&)));
		connect(this, SIGNAL(actionRemove(const Jid&)), acc, SLOT(actionRemove(const Jid&)));
		connect(this, SIGNAL(actionHistory(const Jid&)), acc, SLOT(actionHistory(const Jid&)));
		connect(this, SIGNAL(actionUserInfo(const Jid&)), acc, SLOT(actionInfo(const Jid&)));
	}

	RosterActionsService::~RosterActionsService() {
	}

	void RosterActionsService::sendMessage(const XMPP::Jid& jid) {
		emit actionSendMessage(jid);
	}

	void RosterActionsService::openChat(const XMPP::Jid& jid) {
		emit actionOpenChat(jid);
	}

	void RosterActionsService::executeCommand(const XMPP::Jid& jid) {
		emit actionExecuteCommand(jid);
	}

	void RosterActionsService::sendFile(const XMPP::Jid& jid) {
		emit actionSendFile(jid);
	}

	void RosterActionsService::removeContact(const XMPP::Jid& jid) {
		emit actionRemove(jid);
	}

	void RosterActionsService::showHistory(const XMPP::Jid& jid) {
		emit actionHistory(jid);
	}

	void RosterActionsService::userInfo(const XMPP::Jid& jid) {
		emit actionUserInfo(jid);
	}

}
