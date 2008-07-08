#include <QDebug>

#include "contact.h"
#include "manager.h"
#include "group.h"
#include "account.h"

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

		delete contact; // FIXME: shouldn't this be done in RosterBuilder?
	}

	void Manager::moveContact(Contact* contact, Group* group) {
		emit itemToBeRemoved(contact);
		GroupItem* parent = static_cast<GroupItem*>(contact->getParent());
		parent->removeItem(contact);
		emit itemRemoved(contact);
		
		group->addItem(contact);
		emit itemAdded(contact);
	}

	void Manager::addContact(Contact* contact, Group* group) {
		group->addItem(contact);

		emit itemAdded(contact);
	}

	void Manager::addGroup(Group* group, GroupItem* parent) {
		parent->addItem(group);

		emit itemAdded(group);
	}

	void Manager::addAccount(Account* account, GroupItem* parent) {
		parent->addItem(account);

		emit itemAdded(account);
	}

	void Manager::removeItem(Item* item) { 
		// FIXME: isn't that too generic? should we use removeContact / removeGroup / etc ?
		// FIXME: resources not supported
		emit itemToBeRemoved(item);

		GroupItem* parent = static_cast<GroupItem*>(item->getParent());
		parent->removeItem(item);

		emit itemRemoved(item);

		delete item; // FIXME: shouldn't this be done in RosterBuilder?
	}
}

