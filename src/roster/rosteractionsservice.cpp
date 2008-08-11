#include "rosteractionsservice.h"
#include "psiaccount.h"
#include "avatars.h"

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
		connect(this, SIGNAL(actionOpenWhiteboard(const Jid&)), acc, SLOT(actionOpenWhiteboard(const Jid&)));
		connect(this, SIGNAL(actionResendAuthTo(const Jid&)), acc, SLOT(actionAuth(const Jid&)));
		connect(this, SIGNAL(actionRerequestAuthFrom(const Jid&)), acc, SLOT(actionAuthRequest(const Jid&)));
		connect(this, SIGNAL(actionRemoveAuthFrom(const Jid&)), acc, SLOT(actionAuthRemove(const Jid&)));

		connect(this, SIGNAL(actionAssignAvatar(const Jid&, const QString&)), 
				acc->avatarFactory(), SLOT(importManualAvatar(const Jid&, const QString&)));
		connect(this, SIGNAL(actionClearAvatar(const Jid&)), acc->avatarFactory(), SLOT(removeManualAvatar(const Jid&)));
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

	void RosterActionsService::openWhiteboard(const XMPP::Jid& jid) {
		emit actionOpenWhiteboard(jid);
	}

	void RosterActionsService::resendAuthTo(const XMPP::Jid& jid) {
		emit actionResendAuthTo(jid);
	}

	void RosterActionsService::rerequestAuthFrom(const XMPP::Jid& jid) {
		emit actionRerequestAuthFrom(jid);
	}

	void RosterActionsService::removeAuthFrom(const XMPP::Jid& jid) {
		emit actionRemoveAuthFrom(jid);
	}

	void RosterActionsService::assignAvatar(const XMPP::Jid& jid, const QString& file) {
		emit actionAssignAvatar(jid, file);
	}

	void RosterActionsService::clearAvatar(const XMPP::Jid& jid) {
		emit actionClearAvatar(jid);
	}

}
