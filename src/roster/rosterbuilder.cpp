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
#include "transport.h"
#include "self.h"

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

	void RosterBuilder::updateResources(const QList<XMPPResource*> list, GroupItem* groupItem) {
		QSet<QString> names; // for deleting later
		foreach(XMPPResource* xresource, list) {
			Resource* resource = groupItem->findResource(xresource->getName());

			if ( resource ) {
				if ( resource->getStatus() != xresource->getStatus() ) {
					manager_->setStatus(resource, xresource->getStatus());
				}
				if ( resource->getStatusMessage() != xresource->getStatusMessage() ) {
					manager_->setStatusMessage(resource, xresource->getStatusMessage());
				}
			} else {
				Resource* resource = new Resource(xresource);
				resource->setAccountName(groupItem->getAccountName());
				manager_->addResource(resource, groupItem);
			}

			names.insert(xresource->getName());
		}

		// delete obsolete resources
		foreach(Item* item, groupItem->getItems()) {
			Resource* resource = static_cast<Resource*>(item);
			if ( ! names.contains(resource->getName()) ) {
				manager_->removeResource(resource);
				delete resource;
			}
		}
	}

	void RosterBuilder::updateContact(const XMPPRosterItem* xitem, const QString& acname) {
		RosterDataService* srv = rosterServices_[acname];

		foreach(QString xgroup, xitem->getGroups()) {
			Group* group = findGroup(xgroup, acname);
			Contact* contact = group ? group->findContact(xitem->getName(), acname) : 0;

			if ( isContactVisible(xitem) ) {
				if ( ! contact ) {
					group = findGroup(xgroup, acname, true); // create if needed
					contact = new Contact(xitem->getName(), xitem->getJid());
					contact->setAccountName(acname);
					addContact(contact, group);
				}

				// update contact data
				manager_->setAvatar(contact, srv->getAvatar(contact->getJid()));
				if ( contact->isExpanded() != vsm_->isContactExpanded(contact) ) {
					manager_->updateState(contact, vsm_->isContactExpanded(contact));
				}

				updateResources(xitem->getResources(), contact);
			} else {
				if ( contact ) {
					manager_->removeContact(contact);
				}
			}
		}
	}

	void RosterBuilder::updateTransport(const XMPPRosterItem* xitem, const QString& acname) {
		Group* group = findGroup("Agents/Transports", acname, false);
		Transport* transport = group ? group->findTransport(xitem->getJid(), acname) : 0;
		if ( itemFilter_ & FILTER_TRANSPORTS ) {
			if ( transport ) {
				manager_->removeTransport(transport);
				delete transport;
			}
		} else {
			if ( ! transport ) {
				Group* group = findGroup("Agents/Transports", acname, true);
				transport = new Transport(xitem->getName(), xitem->getJid());
				transport->setAccountName(acname);
				manager_->addTransport(transport, group);
			}

			if ( transport->isExpanded() != vsm_->isTransportExpanded(transport) ) {
				manager_->updateState(transport, vsm_->isTransportExpanded(transport));
			}

			updateResources(xitem->getResources(), transport);
		}
	}

	void RosterBuilder::buildRoster(const QString& acname) {
		RosterDataService* srv = rosterServices_[acname];
		foreach(XMPPRosterItem* xitem, srv->getRosterItems()) { // FIXME: isn't this calling getRosterItems many times?
			if ( srv->isTransport(xitem->getJid()) ) {
				updateTransport(xitem, acname);
			} else {
				updateContact(xitem, acname);
			}
		}

		updateSelf(srv->getSelf(), acname);
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
				manager_->addContact(contact, metacontact);
			} else if ( Contact* similar = group->findContact(contact->getName()) ) {
				Metacontact* metacontact = addMetacontact(contact->getName(), contact->getAccountName(), group);		

				manager_->removeContact(similar);
				manager_->addContact(similar, metacontact);
				manager_->addContact(contact, metacontact);
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

		if ( group->isExpanded() != vsm_->isGroupExpanded(group) ) {
			manager_->updateState(group, vsm_->isGroupExpanded(group));
		}

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

		connect(rosterService, SIGNAL(itemUpdated(const XMPPRosterItem*, const QString&)), 
				SLOT(itemChanged(const XMPPRosterItem*, const QString&)));
		connect(rosterService, SIGNAL(selfUpdated(const XMPPRosterItem*, const QString&)), 
				SLOT(selfChanged(const XMPPRosterItem*, const QString&)));
		connect(rosterService, SIGNAL(accountUpdated(const QString&)), SLOT(accountChanged(const QString&)));
	}

	void RosterBuilder::setJoinByName(bool joinByName) {
		if ( joinByName == joinByName_ ) {
			return;
		}

		joinByName_ = joinByName;
		rebuild();
	}

	void RosterBuilder::itemAdded(const XMPPRosterItem* xitem, const QString& acname) {
		Q_UNUSED(xitem); Q_UNUSED(acname);
		// FIXME: no real stuff here since this is currently not used
	}

	void RosterBuilder::itemRemoved(const XMPPRosterItem* xitem, const QString& acname) {
		Q_UNUSED(xitem); Q_UNUSED(acname);
		// FIXME: no real stuff here since this is currently not used
	}

	const bool RosterBuilder::isContactVisible(const XMPPRosterItem* xitem) const {
		foreach(XMPPResource* xres, xitem->getResources()) {
			if ( xres->getStatus() == STATUS_ONLINE or xres->getStatus() == STATUS_CHAT ) {
				return true;
			} else if ( xres->getStatus() == STATUS_DND and !(itemFilter_ & FILTER_DND) ) {
				return true;
			} else if ( xres->getStatus() == STATUS_AWAY and !(itemFilter_ & FILTER_AWAY) ) {
				return true;
			} else if ( xres->getStatus() == STATUS_XA and !(itemFilter_ & FILTER_XA) ) {
				return true;
			}
		}

		return ! (itemFilter_ & FILTER_OFFLINE);
	}

	void RosterBuilder::itemChanged(const XMPPRosterItem* xitem, const QString& acname) {
		RosterDataService* srv = rosterServices_[acname];

		if ( srv->isTransport(xitem->getJid()) ) {
			updateTransport(xitem, acname);
		} else {
			updateContact(xitem, acname);
		}
	}

	void RosterBuilder::updateSelf(const XMPPRosterItem* xitem, const QString& acname) {
		GroupItem* acc = root_->findAccount(acname);
		if ( ! acc ) {
			acc = root_;
		}

		Self* self = acc->findSelf(acname);

		if ( itemFilter_ & FILTER_SELF ) {
			if ( self ) {
				manager_->removeSelf(self);
				delete self;
			}
		} else {
			if ( ! self ) {
				self = new Self(xitem->getName(), xitem->getJid());
				self->setAccountName(acname);

				manager_->addSelf(self, acc);
			}

			if ( self->isExpanded() != vsm_->isSelfExpanded(self) ) {
				manager_->updateState(self, vsm_->isSelfExpanded(self));
			}

			updateResources(xitem->getResources(), self);	
		}
	}

	void RosterBuilder::accountChanged(const QString& acname) {
		Q_UNUSED(acname);
		// FIXME: do something ;-)
	}

	void RosterBuilder::selfChanged(const XMPPRosterItem* xitem, const QString& acname) {
		updateSelf(xitem, acname);
	}

}

