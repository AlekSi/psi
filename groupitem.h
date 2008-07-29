#ifndef ROSTER_GROUPITEM_H
#define ROSTER_GROUPITEM_H

#include <QList>

#include "item.h"

namespace Roster {
	class Contact;
	class Group;
	class Metacontact;

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
			virtual Contact* findContact(const QString& name, const QString& acname = "");
			virtual Metacontact* findMetacontact(const QString& name);

			virtual void setExpanded(const bool expanded);
			virtual const bool isExpanded() const;

		protected:
			QList<Item*> items_;
			bool expanded_;
	};

}

#endif
