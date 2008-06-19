#include "Item.h"
#include "GroupItem.h"

namespace Roster {

	Item::Item() : parent_(NULL) {
	}

	Item::~Item() {
		if ( parent_ ) {
			parent_->removeItem(this);
		}
	}

	GroupItem* Item::getParent() const {
		return parent_;
	}

	void Item::setParent(GroupItem* parent) {
		parent_ = parent;
	}

}
