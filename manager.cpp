#include <QDebug>

#include "contact.h"
#include "manager.h"
#include "group.h"
#include "account.h"
#include "resource.h"
#include "metacontact.h"

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
		emit itemToBeRemoved(contact);
		contact->getParent()->removeItem(contact);
		emit itemRemoved(contact);

		if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(contact->getParent()) ) {

			foreach(Item* item, metacontact->getItems()) {
				Contact* contact = static_cast<Contact*>(item);
				if ( !contact->getStatusMessage().isEmpty() ) {
					metacontact->setStatusMessage(contact->getStatusMessage());
					break;
				}
			}

			emit itemUpdated(metacontact);
		}
	}

	void Manager::moveContact(Contact* contact, GroupItem* group) {
		removeItem(contact);
		addContact(contact, group);
	}

	void Manager::addContact(Contact* contact, GroupItem* group) {
		addItem(contact, group);
	}

	void Manager::addGroup(Group* group, GroupItem* parent) {
		addItem(group, parent);
	}

	void Manager::addAccount(Account* account, GroupItem* parent) {
		addItem(account, parent);
	}

	void Manager::removeItem(Item* item) { 
		// FIXME: isn't that too generic? should we use removeContact / removeGroup / etc ?
		emit itemToBeRemoved(item);
		item->getParent()->removeItem(item);
		emit itemRemoved(item);
	}

	void Manager::addResource(Resource* resource, Contact* contact) {
		addItem(resource, contact);
		emit itemAdded(resource);

		if ( contact->getIndexOf(resource) == 0 ) {
			setContactStatusMessage(contact, resource->getStatusMessage());
			setContactStatus(contact, resource->getStatus());
		}
	}

	void Manager::addToMetacontact(Contact* contact, Metacontact* metacontact) {
		addItem(contact, metacontact);

		/* if it's the first contact in metacontact, copy stuff to metacontact */
		if ( metacontact->getIndexOf(contact) == 0 ) {
			metacontact->setStatusMessage(contact->getStatusMessage());
			metacontact->setAvatar(contact->getAvatar());
			emit itemUpdated(metacontact);
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

		Contact* contact = static_cast<Contact*>(resource->getParent());
		if ( contact->getIndexOf(resource) == 0 ) {
			setContactStatusMessage(contact, statusMessage);
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

		parent->addItem(item, i);
		emit itemAdded(item);
	}

	void Manager::setStatus(Resource* resource, StatusType status) {
		resource->setStatus(status);
		emit itemUpdated(resource);

		resort(resource);
	
		Contact* contact = static_cast<Contact*>(resource->getParent());
		if ( contact->getIndexOf(resource) == 0 ) {
			setContactStatus(contact, status);
		}
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
		} else if ( const Group* g1 = dynamic_cast<const Group*>(item1) ) {
			if ( const Group* g2 = dynamic_cast<const Group*>(item2) ) {
				return g1->getName().toLower() < g2->getName().toLower();
			} else {
				return true;
			}
		} else if ( dynamic_cast<const Group*>(item2) ) {
			return false;
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
}

