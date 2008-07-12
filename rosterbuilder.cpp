#include <QIcon>
#include <QDebug>

#include "rosterbuilder.h"
#include "group.h"
#include "contact.h"
#include "xmpprosteritem.h"
#include "xmppresource.h"
#include "manager.h"
#include "account.h"
#include "rosterdataservice.h"
#include "resource.h"
#include "roster.h"

namespace Roster {

	RosterBuilder::RosterBuilder(Roster* root, Manager* manager) : root_(root), manager_(manager), joinedAccounts_(true) {
	}

	void RosterBuilder::rebuild() {
		if ( joinedAccounts_ ) {
			buildAllAccounts();
		} else {
			buildJoinedAccounts();
		}
	}

	void RosterBuilder::setJoinedAccounts(bool joinedAccounts) {
		if ( joinedAccounts == joinedAccounts_ ) {
			return;
		}

		joinedAccounts_ = joinedAccounts;
		rebuild();
	}

	void RosterBuilder::buildRoster(QString acname) {
		RosterDataService* srv = services_[acname];
		foreach(XMPPRosterItem* xitem, srv->getRosterItems()) { 
			foreach(QString xgroup, xitem->getGroups()) {
				Contact* contact = new Contact(xitem->getName(), xitem->getJid());
				contact->setAvatar(srv->getAvatar(contact->getJid()));

				Group* group = createGroup(xgroup, acname);

				manager_->addContact(contact, group);

				foreach(XMPPResource* xresource, xitem->getResources()) {
					Resource* resource = new Resource(xresource->getName(), xresource->getPriority(), xresource->getStatus());
					// FIXME: move it to some better place
					if ( xresource->getShow() == STATUS_ONLINE ) {
						resource->setIcon(QIcon("icons/online.png"));
					} else {
						resource->setIcon(QIcon("icons/offline.png"));
					}

					manager_->addResource(resource, contact);
				}

			}
		}
	}

	void RosterBuilder::buildAllAccounts() {
		clear(root_);
		foreach(QString name, services_.keys()) {
			Account* account = new Account(name);
			manager_->addAccount(account, root_);

			buildRoster(name);
		}
	}

	void RosterBuilder::buildJoinedAccounts() {
		clear(root_);
		foreach(QString acname, services_.keys()) {
			buildRoster(acname);
		}
	}

	void RosterBuilder::clear(Item* item) {
		if ( GroupItem* g = dynamic_cast<GroupItem*>(item) ) {
			foreach(Item* subitem, g->getItems()) {
				clear(subitem);
				manager_->removeItem(subitem);
			}
		}
	}

	/* create (if it doesn't exist yet) and return group with given name on given account */
	Group* RosterBuilder::createGroup(const QString& groupName, const QString& acname) {
		QList<QString> groupNames = groupName.split(SEPARATOR);

		GroupItem* up = root_->findAccount(acname);
		if ( ! up ) {
			up = root_;
		}

		foreach(QString name, groupNames) {
			Group* next = up->findGroup(name);

			if ( ! next ) {
				// add new group
				next = new Group(name);
				manager_->addGroup(next, up);
			}

			up = next;
		}

		return static_cast<Group*>(up);
	}


	void RosterBuilder::addService(const QString& acname, RosterDataService* service) {
		services_.insert(acname, service);
	}

}

