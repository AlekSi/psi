#include <QDebug>

#include "groupitem.h"
#include "group.h"
#include "contact.h"
#include "metacontact.h"

namespace Roster {

	GroupItem::GroupItem() : expanded_(true) {
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

	void GroupItem::addItem(Item* item, int i) {
		item->setParent(this);
		items_.insert(i, item);
	}

	int GroupItem::getIndexOf(Item* item) const {
		 return items_.indexOf(item);
	}

	const QList<Item*>& GroupItem::getItems() const {
		return items_;
	}

	Group* GroupItem::findGroup(const QString& name) {
		foreach(Item* item, items_) {
			if ( Group* group = dynamic_cast<Group*>(item) ) {
				if ( group->getName() == name ) {
					return group;
				}
			}
		}
		
		return 0;
	}

	// FIXME: this is wrong with metacontacts, we need to pass acname as well
	Contact* GroupItem::findContact(const QString& name, const QString& acname) {
		foreach(Item* item, items_) {
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				if ( contact->getName() == name and (contact->getAccountName() == acname or acname.isEmpty()) ) {
					return contact;
				}
			}
		}

		return 0;
	}

	Metacontact* GroupItem::findMetacontact(const QString& name) {
		foreach(Item* item, items_) {
			if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
				if ( metacontact->getName() == name ) {
					return metacontact;
				}
			}
		}

		return 0;
	}

	void GroupItem::setExpanded(const bool expanded) {
		expanded_ = expanded;
	}

	const bool GroupItem::isExpanded() const {
		return expanded_;
	}
}

