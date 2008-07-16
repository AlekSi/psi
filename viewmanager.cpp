#include <QDebug>

#include "viewmanager.h"
#include "contact.h"
#include "expanddataservice.h"
#include "group.h"

namespace Roster {

	ViewManager::ViewManager(ExpandDataService* joinedExpandService) : joinedExpandService_(joinedExpandService) {
	}

	void ViewManager::registerAccount(const QString& acname, ExpandDataService* expService) {
		expandServices_.insert(acname, expService);
	}

	void ViewManager::setContactExpanded(Contact* contact, bool expanded) {
		ExpandDataService* srv = expandServices_[contact->getAccountName()];
		srv->setContactExpanded(contact->getJid(), contact->getGroupPath(), expanded);
		contact->setExpanded(expanded);
	}

	void ViewManager::setGroupExpanded(Group* group, bool expanded) {
		ExpandDataService* srv;

		if ( group->getAccountName().isEmpty() ) {
			// using joined accounts
			srv = joinedExpandService_;
		} else {
			srv = expandServices_[group->getAccountName()];
		}

		srv->setGroupExpanded(group->getGroupPath(), expanded);
		group->setExpanded(expanded);
	}

}

