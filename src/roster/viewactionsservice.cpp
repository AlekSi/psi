#include "viewactionsservice.h"
#include "psiaccount.h"
#include "rosteractionsservice.h"
#include "contact.h"
#include "account.h"

namespace Roster {

	ViewActionsService::ViewActionsService() {
	}

	ViewActionsService::~ViewActionsService() {
	}

	void ViewActionsService::registerAccount(const QString& acname, PsiAccount* acc) {
		services_.insert(acname, acc);
	}

	void ViewActionsService::unregisterAccount(const QString& acname) {
		services_.remove(acname);
	}

	void ViewActionsService::sendMessage(Contact* contact) {
		services_[contact->getAccountName()]->actionSendMessage(contact->getJid());
	}

	void ViewActionsService::openChat(Contact* contact) {
		services_[contact->getAccountName()]->actionOpenChat(contact->getJid());
	}

	void ViewActionsService::executeCommand(Contact* contact) {
		services_[contact->getAccountName()]->actionExecuteCommand(contact->getJid());
	}

	void ViewActionsService::sendFile(Contact* contact) {
		services_[contact->getAccountName()]->actionSendFile(contact->getJid());
	}

	void ViewActionsService::removeContact(Contact* contact) {
		services_[contact->getAccountName()]->actionRemove(contact->getJid());
	}

	void ViewActionsService::showHistory(Contact* contact) {
		services_[contact->getAccountName()]->actionHistory(contact->getJid());
	}

	void ViewActionsService::userInfo(Contact* contact) {
		services_[contact->getAccountName()]->actionInfo(contact->getJid());
	}

	void ViewActionsService::openWhiteboard(Contact* contact) {
		services_[contact->getAccountName()]->actionOpenWhiteboard(contact->getJid());
	}

	void ViewActionsService::resendAuthTo(Contact* contact) {
		services_[contact->getAccountName()]->actionAuth(contact->getJid());
	}

	void ViewActionsService::rerequestAuthFrom(Contact* contact) {
		services_[contact->getAccountName()]->actionAuthRequest(contact->getJid());
	}

	void ViewActionsService::removeAuthFrom(Contact* contact) {
		services_[contact->getAccountName()]->actionAuthRemove(contact->getJid());
	}

	void ViewActionsService::assignAvatar(Contact* contact, const QString& file) {
	//	services_[contact->getAccountName()]->assignAvatar(contact->getJid(), file);
	}

	void ViewActionsService::clearAvatar(Contact* contact) {
	//	services_[contact->getAccountName()]->clearAvatar(contact->getJid());
	}

	void ViewActionsService::changeStatus(Account* account, const StatusType status) {
		services_[account->getAccountName()]->changeStatus(status);
	}

	void ViewActionsService::newBlankMessage(Account* account) {
		XMPP::Jid jid;
		services_[account->getAccountName()]->actionSendMessage(jid);
	}

	void ViewActionsService::serviceDiscovery(Account* account) {
		services_[account->getAccountName()]->actionDisco(account->getJid(), "");
	}

	void ViewActionsService::mood(Account* account) {
		services_[account->getAccountName()]->actionSetMood();
	}

	void ViewActionsService::setAvatar(Account* account) {
		services_[account->getAccountName()]->actionSetAvatar();
	}

	void ViewActionsService::unsetAvatar(Account* account) {
		services_[account->getAccountName()]->actionUnsetAvatar();
	}

	void ViewActionsService::modifyAccount(Account* account) {
		services_[account->getAccountName()]->modify();
	}

	void ViewActionsService::addContact(Account* account) {
		services_[account->getAccountName()]->openAddUserDlg();
	}

	void ViewActionsService::xmlConsole(Account* account) {
		services_[account->getAccountName()]->showXmlConsole();
	}

}
