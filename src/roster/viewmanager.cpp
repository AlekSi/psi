#include "viewmanager.h"
#include "psiaccount.h"
#include "viewaccountmanager.h"
#include "contact.h"

namespace Roster {

	ViewManager::ViewManager() {
	}

	ViewManager::~ViewManager() {
	}

	void ViewManager::registerAccount(const QString& acname, PsiAccount* acc) {
		ViewAccountManager* manager = new ViewAccountManager(acc);
		managers_.insert(acname, manager);
	}

	void ViewManager::unregisterAccount(const QString& acname) {
		managers_.remove(acname);
	}

	void ViewManager::sendMessage(Contact* contact) {
		managers_[contact->getAccountName()]->sendMessage(contact->getJid());
	}

	void ViewManager::openChat(Contact* contact) {
		managers_[contact->getAccountName()]->openChat(contact->getJid());
	}

	void ViewManager::executeCommand(Contact* contact) {
		managers_[contact->getAccountName()]->executeCommand(contact->getJid());
	}

	void ViewManager::sendFile(Contact* contact) {
		managers_[contact->getAccountName()]->sendFile(contact->getJid());
	}

	void ViewManager::removeContact(Contact* contact) {
		managers_[contact->getAccountName()]->removeContact(contact->getJid());
	}

	void ViewManager::showHistory(Contact* contact) {
		managers_[contact->getAccountName()]->showHistory(contact->getJid());
	}

	void ViewManager::userInfo(Contact* contact) {
		managers_[contact->getAccountName()]->userInfo(contact->getJid());
	}

}
