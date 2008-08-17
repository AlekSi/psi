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
	class Transport;
	class Self;
	class GroupItem;
	class NotInList;
	
	class Manager : public QObject {
		Q_OBJECT

		public:
			void addContact(Contact* contact, GroupItem* group);
			void addGroup(Group* group, GroupItem* parent);
			void addResource(Resource* resource, GroupItem* groupItem);
			void addAccount(Account* account, GroupItem* parent);
			void addTransport(Transport* transport, GroupItem* groupItem);
			void addMetacontact(Metacontact* metacontact, GroupItem* group);
			void addSelf(Self* self, GroupItem* groupItem);
			void addNil(NotInList* nil, GroupItem* groupItem);

			void removeContact(Contact* contact);	
			void removeResource(Resource* resource);
			void removeAccount(Account* account);
			void removeMetacontact(Metacontact* metacontact);
			void removeTransport(Transport* transport);
			void removeGroup(Group* group);
			void removeSelf(Self* self);
			void removeNil(NotInList* nil);

			void updateState(GroupItem* groupItem, bool expanded);
			void renameContact(Contact* contact, QString newName);
			void setAvatar(Contact* contact, const QIcon& avatar);
			void setStatusMessage(Resource* resource, const QString& statusMessage);
			void setStatus(Resource* resource, const StatusType status);
			void setIncomingEvent(Contact* contact, const EventType event);
			void setStatus(Account* account, const StatusType status);
			void setHasPGPKey(Contact* contact, const bool h);
			void setHasManualAvatar(Contact* contact, const bool h);

			void copyContact(Contact* contact, GroupItem* group);
			void moveContact(Contact* contact, GroupItem* group);

			void removeItem(Item* item, bool delGroup = true);

		signals:
			void itemUpdated(Item* item);
			void itemAdded(Item* item);
			void itemToBeAdded(GroupItem* parent, int row);
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
