#ifndef ROSTER_MANAGER_H
#define ROSTER_MANAGER_H

#include <QString>
#include <QObject>
#include <QIcon>

#include "globals.h"

namespace Roster {
	class Group;
	class Contact;
	class Item;
	class GroupItem;
	class Account;
	class Resource;
	class Metacontact;
	
	class Manager : public QObject {
		Q_OBJECT

		public:
			void renameContact(Contact* contact, QString newName);
			void copyContact(Contact* contact, GroupItem* group);
			void removeContact(Contact* contact);
			void moveContact(Contact* contact, GroupItem* group);
			void addContact(Contact* contact, GroupItem* group);

			void addGroup(Group* group, GroupItem* parent);
			void addResource(Resource* resource, Contact* contact);
			void addAccount(Account* account, GroupItem* parent);
			void addToMetacontact(Contact* contact, Metacontact* metacontact);
			void addMetacontact(Metacontact* metacontact, GroupItem* group);

			void removeItem(Item* item);
			void updateState(GroupItem* groupItem, bool expanded);

			void setAvatar(Contact* contact, const QIcon& avatar);
			void setStatusMessage(Resource* resource, const QString& statusMessage);
			void setStatus(Resource* resource, StatusType status);

		signals:
			void itemUpdated(Item* item);
			void itemAdded(Item* item);
			void itemRemoved(Item* item);
			void itemToBeRemoved(Item* item);

		private:
			void setContactStatusMessage(Contact* contact, const QString& statusMessage);
			void setContactStatus(Contact* contact, StatusType status);
			void addItem(Item* item, GroupItem* parent);
			void resort(Item* item);

			static const bool sortOp(const Item* item1, const Item* item2);
	};
}

#endif
