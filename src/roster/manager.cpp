#include <QDebug>

#include "contact.h"
#include "manager.h"
#include "group.h"
#include "account.h"
#include "resource.h"
#include "metacontact.h"
#include "transport.h"
#include "self.h"

namespace Roster {
	void Manager::renameContact(Contact* contact, QString newName) {
		contact->setName(newName);
		emit itemUpdated(contact);

		resort(contact);
	}

	void Manager::copyContact(Contact* contact, GroupItem* group) {
		Contact* newContact = new Contact(*contact);
		addContact(newContact, group);
	}

	void Manager::removeContact(Contact* contact) {
		removeItem(contact);

		if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(contact->getParent()) ) {
			if ( metacontact->getNbItems() ) {
				Item* item = metacontact->getItems().at(0);
				Contact* c = static_cast<Contact*>(item);
				metacontact->setStatusMessage(c->getStatusMessage());
				metacontact->setStatus(c->getStatus());
				metacontact->setAvatar(c->getAvatar());

				emit itemUpdated(metacontact);
			} else {
				removeMetacontact(metacontact);
			}
		}
	}

	void Manager::moveContact(Contact* contact, GroupItem* group) {
		removeContact(contact);
		addContact(contact, group);
	}

	void Manager::addContact(Contact* contact, GroupItem* groupItem) {
		addItem(contact, groupItem);

		if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(contact->getParent()) ) {
			/* if it's the first contact in metacontact, copy stuff to metacontact */
			if ( metacontact->getIndexOf(contact) == 0 ) {
				metacontact->setStatus(contact->getStatus());
				metacontact->setStatusMessage(contact->getStatusMessage());
				metacontact->setAvatar(contact->getAvatar());
				emit itemUpdated(metacontact);
			}
		}
	}

	void Manager::addGroup(Group* group, GroupItem* parent) {
		addItem(group, parent);
	}

	void Manager::addAccount(Account* account, GroupItem* parent) {
		addItem(account, parent);
	}

	void Manager::removeItem(Item* item) { 
		emit itemToBeRemoved(item);
		item->getParent()->removeItem(item);
		emit itemRemoved(item);
	}

	void Manager::addResource(Resource* resource, GroupItem* groupItem) {
		addItem(resource, groupItem);

		if ( groupItem->getIndexOf(resource) == 0 ) {
			if ( Contact* contact = dynamic_cast<Contact*>(groupItem) ) {
				setContactStatusMessage(contact, resource->getStatusMessage());
				setContactStatus(contact, resource->getStatus());
			} else if ( Self* self = dynamic_cast<Self*>(groupItem) ) {
				self->setStatus(resource->getStatus());
				self->setStatusMessage(resource->getStatusMessage());
				emit itemUpdated(self);
			} else if ( Transport* transport = dynamic_cast<Transport*>(groupItem) ) {
				transport->setStatus(resource->getStatus());
				emit itemUpdated(transport);
			}
		}
	}

	void Manager::addMetacontact(Metacontact* metacontact, GroupItem* group) {
		addItem(metacontact, group);
	}

	void Manager::updateState(GroupItem* groupItem, bool expanded) {
		groupItem->setExpanded(expanded);
		emit itemUpdated(groupItem);
	}

	void Manager::setAvatar(Contact* contact, const QIcon& avatar) {
		contact->setAvatar(avatar);
		emit itemUpdated(contact);
		
		if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(contact->getParent()) ) {
			if ( metacontact->getIndexOf(contact) == 0 ) {
				metacontact->setAvatar(contact->getAvatar());
				emit itemUpdated(metacontact);
			}
		}
	}

	void Manager::setStatusMessage(Resource* resource, const QString& statusMessage) {
		resource->setStatusMessage(statusMessage);
		emit itemUpdated(resource);

		if ( Contact* contact = dynamic_cast<Contact*>(resource->getParent()) ) {
			if ( contact->getIndexOf(resource) == 0 ) {
				setContactStatusMessage(contact, statusMessage);
			}
		} 
	}

	void Manager::setContactStatusMessage(Contact* contact, const QString& statusMessage) {
		contact->setStatusMessage(statusMessage);
		emit itemUpdated(contact);

		if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(contact->getParent()) ) {
			if ( metacontact->getIndexOf(contact) == 0 ) {
				metacontact->setStatusMessage(statusMessage);
				emit itemUpdated(metacontact);
			}
		}
	}

	void Manager::addItem(Item* item, GroupItem* parent) {
		QList<Item*>::const_iterator it = qLowerBound(parent->getItems().begin(), parent->getItems().end(), item, sortOp);
		int i = 0;
		if ( it == parent->getItems().end() ) {
			i = parent->getNbItems();
		} else {
			i = parent->getIndexOf(*it);
		}

		emit itemToBeAdded(parent, i);
		parent->addItem(item, i);
		emit itemAdded(item);
	}

	void Manager::setStatus(Resource* resource, StatusType status) {
		resource->setStatus(status);
		emit itemUpdated(resource);

		resort(resource);
	
		if ( Contact* contact = static_cast<Contact*>(resource->getParent()) ) {
			if ( contact->getIndexOf(resource) == 0 ) {
				setContactStatus(contact, status);
			}
		} else if ( Transport* transport = static_cast<Transport*>(resource->getParent()) ) {
			if ( transport->getIndexOf(resource) == 0 ) {
				setTransportStatus(transport, status);
			}
		}
	}

	void Manager::setTransportStatus(Transport* transport, StatusType status) {
		transport->setStatus(status);
		emit itemUpdated(transport);
	}

	void Manager::resort(Item* item) {
		GroupItem* parent = item->getParent();
		int i = parent->getIndexOf(item);
		if ( (i > 0 and sortOp(item, parent->getItems()[i-1])) or (i+1 < parent->getNbItems() and sortOp(parent->getItems()[i+1], item)) ) {
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				removeContact(contact);
				addContact(contact, parent);
			} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				removeItem(resource);
				addResource(resource, static_cast<Contact*>(parent));
			} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
				removeItem(metacontact);
				addMetacontact(metacontact, parent);
			}	
		}
	}

	void Manager::setContactStatus(Contact* contact, StatusType status) {
		contact->setStatus(status);
		emit itemUpdated(contact);

		resort(contact);

		if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(contact->getParent()) ) {
			if ( metacontact->getIndexOf(contact) == 0 ) {
				metacontact->setStatus(status);
				emit itemUpdated(metacontact);
				resort(metacontact);
			}
		}
	}

	const bool Manager::sortOp(const Item* item1, const Item* item2) {
		if ( const Account* a1 = dynamic_cast<const Account*>(item1) ) {
			if ( const Account* a2 = dynamic_cast<const Account*>(item2) ) {
				return a1->getName().toLower() < a2->getName().toLower();
			} else {
				return true;
			}
		} else if ( dynamic_cast<const Account*>(item2) ) {
			return false; 
		} else if ( const Self* s1 = dynamic_cast<const Self*>(item1) ) {
			if ( const Self* s2 = dynamic_cast<const Self*>(item2) ) {
				return s1->getName().toLower() < s2->getName().toLower();
			} else {
				return true;
			}
		} else if ( dynamic_cast<const Self*>(item2) ) {
			return false;
		} else if ( const Group* g1 = dynamic_cast<const Group*>(item1) ) {
			if ( const Group* g2 = dynamic_cast<const Group*>(item2) ) {
				return g1->getName().toLower() < g2->getName().toLower();
			} else {
				return true;
			}
		} else if ( dynamic_cast<const Group*>(item2) ) {
			return false;
		} else if ( const Transport* t1 = dynamic_cast<const Transport*>(item1) ) {
			if ( const Transport* t2 = dynamic_cast<const Transport*>(item2) ) {
				return t1->getName().toLower() < t2->getName().toLower();
			} else {
				return false;
			}
		} else if ( dynamic_cast<const Transport*>(item2) ) {
			return true;
		} else if ( const Metacontact* m1 = dynamic_cast<const Metacontact*>(item1) ) {
			if ( const Contact* c2 = dynamic_cast<const Contact*>(item2) ) {
				return (m1->getStatus() < c2->getStatus()) or (m1->getStatus() == c2->getStatus() and m1->getName().toLower() < c2->getName().toLower());
			} else if ( const Metacontact* m2 = dynamic_cast<const Metacontact*>(item2) ) {
				return (m1->getStatus() < m2->getStatus()) or (m1->getStatus() == m2->getStatus() and m1->getName().toLower() < m2->getName().toLower());
			}
		} else if ( const Contact* c1 = dynamic_cast<const Contact*>(item1) ) {
			if ( const Contact* c2 = dynamic_cast<const Contact*>(item2) ) {
				return (c1->getStatus() < c2->getStatus()) or (c1->getStatus() == c2->getStatus() and c1->getName().toLower() < c2->getName().toLower());
			} else if ( const Metacontact* m2 = dynamic_cast<const Metacontact*>(item2) ) {
				return (c1->getStatus() < m2->getStatus()) or (c1->getStatus() == m2->getStatus() and c1->getName().toLower() < m2->getName().toLower());
			}
		} else if ( const Resource* r1 = dynamic_cast<const Resource*>(item1) ) {
			if ( const Resource* r2 = dynamic_cast<const Resource*>(item2) ) {
				return (r1->getPriority() > r2->getPriority()) or (r1->getPriority() == r2->getPriority() and r1->getStatus() < r2->getStatus());
			}
		} 

		qDebug() << "Oops, this should never happen";
		return false;
	}	

	void Manager::addTransport(Transport* transport, GroupItem* groupItem) {
		groupItem->addItem(transport);
	}

	void Manager::removeGroup(Group* group) {
		removeItem(group);
	}

	void Manager::removeMetacontact(Metacontact* metacontact) {
		removeItem(metacontact);
	}

	void Manager::removeAccount(Account* account) {
		removeItem(account);
	}

	void Manager::removeSelf(Self* self) {
		removeItem(self);
	}

	void Manager::addSelf(Self* self, GroupItem* groupItem) {
		addItem(self, groupItem);

		if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(self->getParent()) ) {
			/* if it's the first contact in metacontact, copy stuff to metacontact */
			if ( metacontact->getIndexOf(self) == 0 ) {
				metacontact->setStatus(self->getStatus());
				metacontact->setStatusMessage(self->getStatusMessage());
				metacontact->setAvatar(self->getAvatar());
				emit itemUpdated(metacontact);
			}
		}
	}

	// BEGIN black magic
	void Manager::removeResource(Resource* resource) {
		if ( resource->getParent()->getIndexOf(resource) == 0 ) {
			if ( Contact* contact = dynamic_cast<Contact*>(resource->getParent()) ) {
				if ( contact->getNbItems() > 1 ) {
					Resource* r = static_cast<Resource*>(contact->getItems().at(1));
					contact->setStatusMessage(r->getStatusMessage());
					contact->setStatus(r->getStatus());
				} else {
					contact->setStatusMessage("");
					contact->setStatus(STATUS_OFFLINE);
				}
			} else if ( Transport* transport = dynamic_cast<Transport*>(resource->getParent()) ) {
				if ( transport->getNbItems() > 1 ) {
					Resource* r = static_cast<Resource*>(contact->getItems().at(1));
					transport->setStatus(r->getStatus());
				} else {
					transport->setStatus(STATUS_OFFLINE);
				}
			} else if ( Self* self = dynamic_cast<Self*>(resource->getParent()) ) {
				if ( self->getNbItems() > 1 ) {
					Resource* r = static_cast<Resource*>(contact->getItems().at(1));
					self->setStatus(r->getStatus());
				} else {
					self->setStatus(STATUS_OFFLINE);
				}
			}
		}

		removeItem(resource);
	}
	// END black magic

	void Manager::removeTransport(Transport* transport) {
		removeItem(transport);
	}	
}

