#include <QDebug>

#include "contact.h"
#include "manager.h"
#include "group.h"

namespace Roster {
	void Manager::renameContact(Contact* contact, QString newName) {
		contact->setName(newName);

		emit itemUpdated(contact);
	}

	void Manager::copyContact(Contact* contact, Group* group) {
		Contact* newContact = new Contact(*contact);
		group->addItem(newContact);

		emit itemAdded(newContact);
	}

	void Manager::removeContact(Contact* contact) {
		emit itemToBeRemoved(contact);

		GroupItem* parent = static_cast<GroupItem*>(contact->getParent());
		parent->removeItem(contact);

		emit itemRemoved(contact);

		delete contact;
	}

	void Manager::moveContact(Contact* contact, Group* group) {
		emit itemToBeRemoved(contact);
		GroupItem* parent = static_cast<GroupItem*>(contact->getParent());
		parent->removeItem(contact);
		emit itemRemoved(contact);
		
		group->addItem(contact);
		emit itemAdded(contact);
	}
}

