#include "metacontact.h"
#include "contact.h"

namespace Roster {

	Metacontact::Metacontact(const QString& name) {
		name_ = name;
	}

	const QString& Metacontact::getName() const {
		return name_;
	}

	const QString Metacontact::getStatus() const {
		// FIXME: get REAL status here
		QString status;
		
		foreach(Item* item, items_) {
			Contact* contact = static_cast<Contact*>(item);
			status = contact->getStatus();
		}

		return status;
	}

	const QIcon Metacontact::getIcon() const {
		// FIXME: get REAL icon here
		QIcon icon;
		
		foreach(Item* item, items_) {
			Contact* contact = static_cast<Contact*>(item);
			icon = contact->getIcon();
		}

		return icon;
	}

	const QIcon& Metacontact::getAvatar() const {
		return avatar_;
	}

	void Metacontact::setName(const QString& name) {
		name_ = name;
	}

	void Metacontact::setAvatar(const QIcon& avatar) {
		avatar_ = avatar;
	}
}
