#include <QDebug>

#include "groupitem.h"
#include "group.h"
#include "contact.h"

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

	Contact* GroupItem::findContact(const QString& jid) {
		foreach(Item* item, items_) {
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				if ( contact->getJid() == jid ) {
					return contact;
				}
			}
		}

		return 0;
	}
}

