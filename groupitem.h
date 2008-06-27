#ifndef ROSTER_GROUPITEM_H
#define ROSTER_GROUPITEM_H

#include <QList>

#include "item.h"

namespace Roster {

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

		protected:
			QList<Item*> items_;
	};

}

#endif
