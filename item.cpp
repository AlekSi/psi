#include "item.h"
#include "groupitem.h"

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

	// FIXME: this won't be unique if one item will appear more than once on roster
	unsigned int Item::getId() const {
		return (unsigned int)(this);
	}

}
