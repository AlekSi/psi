#include "viewactionsservice.h"
#include "psiaccount.h"
#include "rosteractionsservice.h"
#include "contact.h"

namespace Roster {

	ViewActionsService::ViewActionsService() {
	}

	ViewActionsService::~ViewActionsService() {
	}

	void ViewActionsService::registerAccount(const QString& acname, PsiAccount* acc) {
		RosterActionsService* manager = new RosterActionsService(acc);
		managers_.insert(acname, manager);
	}

	void ViewActionsService::unregisterAccount(const QString& acname) {
		managers_.remove(acname);
	}

	void ViewActionsService::sendMessage(Contact* contact) {
		managers_[contact->getAccountName()]->sendMessage(contact->getJid());
	}

	void ViewActionsService::openChat(Contact* contact) {
		managers_[contact->getAccountName()]->openChat(contact->getJid());
	}

	void ViewActionsService::executeCommand(Contact* contact) {
		managers_[contact->getAccountName()]->executeCommand(contact->getJid());
	}

	void ViewActionsService::sendFile(Contact* contact) {
		managers_[contact->getAccountName()]->sendFile(contact->getJid());
	}

	void ViewActionsService::removeContact(Contact* contact) {
		managers_[contact->getAccountName()]->removeContact(contact->getJid());
	}

	void ViewActionsService::showHistory(Contact* contact) {
		managers_[contact->getAccountName()]->showHistory(contact->getJid());
	}

	void ViewActionsService::userInfo(Contact* contact) {
		managers_[contact->getAccountName()]->userInfo(contact->getJid());
	}

}
