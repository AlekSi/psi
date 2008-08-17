#ifndef ROSTER_GROUPITEM_H
#define ROSTER_GROUPITEM_H

#include <QList>

#include "item.h"

namespace XMPP {
	class Jid;
}

namespace Roster {
	class Contact;
	class Group;
	class Metacontact;
	class Self;
	class Resource;
	class Transport;
	class NotInList;

	// generic class for roster items that can contain other items
	class GroupItem : public Item {
		public:
			GroupItem();
			virtual ~GroupItem();

			virtual int getNbItems() const; // number of items
			virtual Item* getItem(int i) const; // 0-based index
			virtual int getIndexOf(Item* item) const;
			virtual void removeItem(Item* item);
			virtual void addItem(Item* item);
			virtual void addItem(Item* item, int i);
			virtual const QList<Item*>& getItems() const;
			virtual Group* findGroup(const QString& name);
			virtual Contact* findContact(const QString& name);
			virtual Contact* findContact(const XMPP::Jid& jid, const QString& acname);
			virtual Metacontact* findMetacontact(const QString& name);
			virtual Self* findSelf(const QString& acname) const;
			virtual Resource* findResource(const QString& name) const;
			virtual Transport* findTransport(const XMPP::Jid& jid, const QString& acname) const;
			virtual NotInList* findNil(const XMPP::Jid& jid, const QString& acname) const;

			virtual void setExpanded(const bool expanded);
			virtual const bool isExpanded() const;

		protected:
			QList<Item*> items_;
			bool expanded_;
	};

}

#endif
