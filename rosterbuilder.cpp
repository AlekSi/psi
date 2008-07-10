#include <QIcon>

#include "rosterbuilder.h"
#include "group.h"
#include "contact.h"
#include "xmpprosteritem.h"
#include "xmppresource.h"
#include "manager.h"
#include "account.h"
#include "rosterdataservice.h"
#include "resource.h"

namespace Roster {

	RosterBuilder::RosterBuilder(Manager* manager) : manager_(manager) {
	}

	void RosterBuilder::buildRoster(QString acname, GroupItem* root) {
		RosterDataService* srv = services_[acname];
		foreach(XMPPRosterItem* xitem, srv->getRosterItems()) { 
			foreach(QString xgroup, xitem->getGroups()) {
				Contact* contact = new Contact(xitem->getName(), xitem->getJid());
				contact->setAvatar(srv->getAvatar(contact->getJid()));

				Group* group = getGroupForAdd(xgroup, root);
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

	void RosterBuilder::buildAllAccounts(GroupItem* root) {
		clear(root);
		foreach(QString name, services_.keys()) {
			Account* account = new Account(name);
			manager_->addAccount(account, root);

			buildRoster(name, account);
		}
	}

	void RosterBuilder::buildJoinedAccounts(GroupItem* root) {
		clear(root);
		foreach(QString acname, services_.keys()) {
			buildRoster(acname, root);
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

	Group* RosterBuilder::getGroupForAdd(QString groupName, GroupItem* parent) {
		QStringList groupNames = groupName.split("::");

		GroupItem* up = parent;

		foreach(QString name, groupNames) {
			Group* next = 0;

			foreach(Item* item, up->getItems()) {
				if ( Group* group = dynamic_cast<Group*>(item) ) {
					if ( group->getName() == name ) {
						next = group;
						break;
					}
				}
			}

			if ( next ) {
				up = next;
			} else {
				Group* newGroup = new Group(name);
				manager_->addGroup(newGroup, up);
				up = newGroup;
			}
		}

		return static_cast<Group*>(up);
	}

	void RosterBuilder::addService(const QString& acname, RosterDataService* service) {
		services_.insert(acname, service);
	}

}

