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
#include "metacontact.h"
#include "viewstatemanager.h"

namespace Roster {

	RosterBuilder::RosterBuilder(Roster* root, Manager* manager, ViewStateManager* vsm) : 
																		vsm_(vsm),
																		root_(root), 
																		manager_(manager), 
																		joinedAccounts_(false), 
																		joinByName_(false),
																		itemFilter_(FILTER_OFFLINE)	{
	}

	void RosterBuilder::rebuild() {
		if ( joinedAccounts_ ) {
			buildJoinedAccounts();
		} else {
			buildAllAccounts();
		}
	}

	void RosterBuilder::setJoinedAccounts(bool joinedAccounts) {
		if ( joinedAccounts == joinedAccounts_ ) {
			return;
		}

		joinedAccounts_ = joinedAccounts;
		rebuild();
	}

	void RosterBuilder::setShowOffline(bool show) {
		if ( show ) {
			setFilter(getFilter() & (~FILTER_OFFLINE));
		} else {
			setFilter(getFilter() | FILTER_OFFLINE);
		}
	}

	unsigned int RosterBuilder::getFilter() {
		return itemFilter_;
	}

	void RosterBuilder::setFilter(unsigned int itemFilter) {
		itemFilter_ = itemFilter;
		rebuild();
	}

	void RosterBuilder::addResource(XMPPResource* xresource, Contact* parent) {
		Resource* resource = new Resource(xresource);
		resource->setAccountName(parent->getAccountName());

		manager_->addResource(resource, parent);
	}

	void RosterBuilder::addItem(const XMPPRosterItem* xitem, const QString& acname) {
		RosterDataService* srv = rosterServices_[acname];

		if ( ! isContactVisible(xitem) ) {
			return;
		}

		foreach(QString xgroup, xitem->getGroups()) {
			Contact* contact = new Contact(xitem->getName(), xitem->getJid());
			contact->setAccountName(acname);

			Group* group = findGroup(xgroup, acname);
			addContact(contact, group);

			manager_->setAvatar(contact, srv->getAvatar(contact->getJid()));
			manager_->updateState(contact, vsm_->isContactExpanded(contact));

			foreach(XMPPResource* xresource, xitem->getResources()) {
				addResource(xresource, contact);
			}
		}
	}

	void RosterBuilder::buildRoster(const QString& acname) {
		RosterDataService* srv = rosterServices_[acname];
		foreach(XMPPRosterItem* xitem, srv->getRosterItems()) { 
			addItem(xitem, acname);
		}
	}

	Metacontact* RosterBuilder::addMetacontact(const QString& name, const QString& acname, GroupItem* parent) {
		Metacontact* metacontact = new Metacontact(name);
		if ( ! joinedAccounts_ ) {
			metacontact->setAccountName(acname);
		}

		manager_->addMetacontact(metacontact, parent);
		manager_->updateState(metacontact, vsm_->isMetacontactExpanded(metacontact));
		return metacontact;
	}

	void RosterBuilder::addContact(Contact* contact, Group* group) {
		if ( ! joinByName_ ) {
			manager_->addContact(contact, group);
		} else {
			if ( Metacontact* metacontact = group->findMetacontact(contact->getName()) ) {
				manager_->addToMetacontact(contact, metacontact);
			} else if ( Contact* similar = group->findContact(contact->getName()) ) {
				Metacontact* metacontact = addMetacontact(contact->getName(), contact->getAccountName(), group);		

				manager_->removeContact(similar);
				manager_->addToMetacontact(similar, metacontact);
				manager_->addToMetacontact(contact, metacontact);
			} else {
				manager_->addContact(contact, group);
			}
		}
	}

	void RosterBuilder::buildAllAccounts() {
		clear(root_);
		foreach(QString name, rosterServices_.keys()) {
			Account* account = new Account(name);
			account->setAccountName(name);
			manager_->addAccount(account, root_);
			manager_->updateState(account, vsm_->isAccountExpanded(account));

			buildRoster(name);
		}
	}

	void RosterBuilder::buildJoinedAccounts() {
		clear(root_);
		foreach(QString acname, rosterServices_.keys()) {
			buildRoster(acname);
		}
	}

	void RosterBuilder::clear(Item* item) {
		if ( GroupItem* g = dynamic_cast<GroupItem*>(item) ) {
			foreach(Item* subitem, g->getItems()) {
				clear(subitem);
				manager_->removeItem(subitem);
				delete subitem;
			}
		}
	}

	Group* RosterBuilder::addGroup(const QString& groupName, const QString& acname, GroupItem* parent) {
		Group* group = new Group(groupName);
		if ( ! joinedAccounts_ ) {
			group->setAccountName(acname);
		}

		manager_->addGroup(group, parent);
		manager_->updateState(group, vsm_->isGroupExpanded(group));
		return group;
	}

	/* create (if it doesn't exist yet) and return group with given name on given account */
	Group* RosterBuilder::findGroup(const QString& groupName, const QString& acname, bool create) {
		QList<QString> groupNames = groupName.split(SEPARATOR);

		GroupItem* up = root_->findAccount(acname);
		if ( ! up ) {
			up = root_;
		}

		foreach(QString name, groupNames) {
			Group* next = up->findGroup(name);

			if ( ! next ) {
				if ( create ) {
					next = addGroup(name, acname, up);
				} else {
					return 0;
				}
			}


			up = next;
		}

		return static_cast<Group*>(up);
	}

	void RosterBuilder::registerAccount(const QString& acname, RosterDataService* rosterService) {
		rosterServices_.insert(acname, rosterService);

		connect(rosterService, SIGNAL(itemUpdated(const XMPPRosterItem*, const QString&)), SLOT(itemChanged(const XMPPRosterItem*, const QString&)));
	}

	void RosterBuilder::setJoinByName(bool joinByName) {
		if ( joinByName == joinByName_ ) {
			return;
		}

		joinByName_ = joinByName;
		rebuild();
	}

	void RosterBuilder::itemAdded(const XMPPRosterItem* xitem, const QString& acname) {
		addItem(xitem, acname);
	}

	void RosterBuilder::itemRemoved(const XMPPRosterItem* xitem, const QString& acname) {
		QList<Contact*> contacts = findContacts(xitem, acname);

		foreach(Contact* contact, contacts) {
			manager_->removeContact(contact);
			delete contact;
		}
	}

	const bool RosterBuilder::isContactVisible(const XMPPRosterItem* xitem) const {
		foreach(XMPPResource* xres, xitem->getResources()) {
			if ( xres->getStatus() == STATUS_ONLINE or xres->getStatus() == STATUS_CHAT ) {
				return true;
			}

			if ( xres->getStatus() == STATUS_DND and !(itemFilter_ & FILTER_DND) ) {
				return true;
			}

			if ( xres->getStatus() == STATUS_AWAY and !(itemFilter_ & FILTER_AWAY) ) {
				return true;
			}

			if ( xres->getStatus() == STATUS_XA and !(itemFilter_ & FILTER_XA) ) {
				return true;
			}
		}

		if ( ! (itemFilter_ & FILTER_OFFLINE) ) {
			return true;
		}

		return false;
	}

	void RosterBuilder::itemChanged(const XMPPRosterItem* xitem, const QString& acname) {
		RosterDataService* srv = rosterServices_[acname];

		if ( isContactVisible(xitem) ) {
			foreach(QString xgroup, xitem->getGroups()) {
				Group* group = findGroup(xgroup, acname);
				Contact* contact = group->findContact(xitem->getName(), acname);

				if ( ! contact ) {
					contact = new Contact(xitem->getName(), xitem->getJid());
					contact->setAccountName(acname);
					addContact(contact, group);
				}

				// update contact data
				manager_->setAvatar(contact, srv->getAvatar(contact->getJid()));
				if ( contact->isExpanded() != vsm_->isContactExpanded(contact) ) {
					manager_->updateState(contact, vsm_->isContactExpanded(contact));
				}

				QSet<QString> names; // for deleting not used later
				// update resources
				foreach(XMPPResource* xresource, xitem->getResources()) {
					Resource* resource = contact->findResource(xresource->getName());

					if ( resource ) {
						if ( resource->getStatus() != xresource->getStatus() ) {
							manager_->setStatus(resource, xresource->getStatus());
						}
						if ( resource->getStatusMessage() != xresource->getStatusMessage() ) {
							manager_->setStatusMessage(resource, xresource->getStatusMessage());
						}
					} else {
						addResource(xresource, contact);
					}

					names.insert(xresource->getName());
				}

				// delete obsolete resources
				foreach(Item* item, contact->getItems()) {
					Resource* resource = dynamic_cast<Resource*>(item);
					if ( ! names.contains(resource->getName()) ) {
						manager_->removeItem(resource);
						delete resource;
					}
				}
			}
		} else { // contact not visible
			foreach(QString xgroup, xitem->getGroups()) {
				Group* group = findGroup(xgroup, acname);
				Contact* contact = group->findContact(xitem->getName(), acname);
				if ( contact ) {
					manager_->removeContact(contact);
				}
			}
		}
	}

	QList<Contact*> RosterBuilder::findContacts(const XMPPRosterItem* xitem, const QString& acname) {
		QList<Contact*> contacts;

		foreach(QString xgroup, xitem->getGroups()) {
			Group* group = findGroup(xgroup, acname, false);

			if ( ! group ) {
				continue;
			}

			if ( joinByName_ ) {
			   	if ( Metacontact* metacontact = group->findMetacontact(xitem->getName()) ) {
					if ( Contact* contact = metacontact->findContact(xitem->getName(), acname) ) {
						contacts.append(contact);
					}
				}
			}

			if ( Contact* contact = group->findContact(xitem->getName(), acname) ) {
				contacts.append(contact);
			}
		}

		return contacts;
	}
}

