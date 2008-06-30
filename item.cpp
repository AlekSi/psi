#include "item.h"

namespace Roster {

	Item::Item() : parent_(NULL) {
		static unsigned int nextId = 0;
		id_ = nextId++;
	}

	Item::~Item() {
	}

	Item* Item::getParent() const {
		return parent_;
	}

	void Item::setParent(Item* parent) {
		parent_ = parent;
	}

	unsigned int Item::getId() const {
		return id_;
	}

}
