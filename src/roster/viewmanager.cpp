#include "viewmanager.h"
#include "psiaccount.h"
#include "viewaccountmanager.h"
#include "contact.h"

namespace Roster {

	ViewManager::ViewManager() {
	}

	ViewManager::~ViewManager() {
	}

	void ViewManager::registerAccount(PsiAccount* acc) {
		ViewAccountManager* manager = new ViewAccountManager(acc);
		managers_.insert(acc->jid().full(), manager);
	}

	void ViewManager::sendMessage(Contact* contact) {
		if ( managers_.contains(contact->getAccountName()) ) {
			managers_[contact->getAccountName()]->sendMessage(contact->getJid());
		}
	}

}
