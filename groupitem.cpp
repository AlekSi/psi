#include "groupitem.h"

namespace Roster {

	GroupItem::GroupItem() {
	}

	GroupItem::~GroupItem() {
	}

	int GroupItem::getNbItems() const {
		return items_.count();
	}

	Item* GroupItem::getItem(int i) const {
		Q_ASSERT(i >= 0 && i < items_.count());
		return items_[i];
	}

	void GroupItem::removeItem(Item* item) {
		items_.removeAll(item);
	}

	void GroupItem::addItem(Item* item) {
		item->setParent(this);
		items_.append(item);
	}

	int GroupItem::getIndexOf(Item* item) const {
		 return items_.indexOf(item);
	}

	const QList<Item*>& GroupItem::getItems() const {
		return items_;
	}
}

