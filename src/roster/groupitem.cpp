#include <QDebug>

#include "globals.h"
#include "groupitem.h"
#include "group.h"
#include "contact.h"
#include "metacontact.h"
#include "self.h"
#include "resource.h"
#include "transport.h"
#include "notinlist.h"

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

	Contact* GroupItem::findContact(const XMPP::Jid& jid, const QString& acname) {
		foreach(Item* item, items_) {
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				if ( contact->getJid().full() == jid.full() and contact->getAccountName() == acname ) {
					return contact;
				}
			}
		}

		return 0;
	}

	Contact* GroupItem::findContact(const QString& name) {
		foreach(Item* item, items_) {
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				if ( contact->getName() == name ) {
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

	Self* GroupItem::findSelf(const QString& acname) const {
		foreach(Item* item, items_) {
			if ( Self* self = dynamic_cast<Self*>(item) ) {
				if ( self->getAccountName() == acname ) {
					return self;
				}
			}
		}

		return 0;
	}

	Resource* GroupItem::findResource(const QString& name) const {
		foreach(Item* item, items_) {
			if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				if ( resource->getName() == name ) {
					return resource;
				}
			}
		}

		return NULL;
	}

	Transport* GroupItem::findTransport(const XMPP::Jid& jid, const QString& acname) const {
		foreach(Item* item, items_) {
			if ( Transport* transport = dynamic_cast<Transport*>(item) ) {
				if ( transport->getJid().full() == jid.full() and transport->getAccountName() == acname ) {
					return transport;
				}
			}
		}

		return NULL;
	}

	NotInList* GroupItem::findNil(const XMPP::Jid& jid, const QString& acname) const {
		foreach(Item* item, items_) {
			if ( NotInList* nil = dynamic_cast<NotInList*>(item) ) {
				if ( nil->getJid().full() == jid.full() and nil->getAccountName() == acname ) {
					return nil;
				}
			}
		}

		return NULL;
	}

	void GroupItem::setExpanded(const bool expanded) {
		expanded_ = expanded;
	}

	const bool GroupItem::isExpanded() const {
		return expanded_;
	}
}

