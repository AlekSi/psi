#ifndef ROSTER_MANAGER_H
#define ROSTER_MANAGER_H

#include <QString>
#include <QObject>

namespace Roster {
	class Group;
	class Contact;
	class Item;
	class GroupItem;
	class Account;
	
	class Manager : public QObject {
		Q_OBJECT

		public:
			void renameContact(Contact* contact, QString newName);
			void copyContact(Contact* contact, Group* group);
			void removeContact(Contact* contact);
			void moveContact(Contact* contact, Group* group);
			void addContact(Contact* contact, Group* group);

			void addGroup(Group* group, GroupItem* parent);
			void addAccount(Account* account, GroupItem* parent);

			void removeItem(Item* item);

		signals:
			void itemUpdated(Item* item);
			void itemAdded(Item* item);
			void itemRemoved(Item* item);
			void itemToBeRemoved(Item* item);
	};
}

#endif
