#include "item.h"
#include "groupitem.h"

namespace Roster {

	Item::Item() : parent_(NULL) {
		static unsigned int nextId = 0;
		id_ = nextId++;
	}

	Item::~Item() {
	}

	GroupItem* Item::getParent() const {
		return parent_;
	}

	void Item::setParent(GroupItem* parent) {
		parent_ = parent;
	}

	unsigned int Item::getId() const {
		return id_;
	}

	void Item::setAccountName(const QString& acname) {
		acname_ = acname;
	}

	const QString& Item::getAccountName() const {
		return acname_;
	}

	const QString Item::getGroupPath() const {
		return parent_->getGroupPath();
	}
}
