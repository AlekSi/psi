#ifndef ROSTER_MANAGER_H
#define ROSTER_MANAGER_H

#include <QString>
#include <QObject>

namespace Roster {
	class Group;
	class Contact;
	class Item;

	class Manager : public QObject {
		Q_OBJECT

		public:
			void renameContact(Contact* contact, QString newName);
			void copyContact(Contact* contact, Group* group);
			void removeContact(Contact* contact);
			void moveContact(Contact* contact, Group* group);

		signals:
			void itemUpdated(Item* item);
			void itemAdded(Item* item);
			void itemRemoved(Item* item);
			void itemToBeRemoved(Item* item);
	};
}

#endif
