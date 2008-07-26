#include <QIcon>

#include "contact.h"
#include "resource.h"

namespace Roster {

	Contact::Contact(const QString& name, const QString& jid) : GroupItem(), name_(name), jid_(jid) {
	}

	Contact::Contact(const Contact& c) : GroupItem() {
		name_ = c.name_;
		jid_ = c.jid_;
//		icon_ = c.icon_;
		avatar_ = c.avatar_;	
		items_ = c.items_;
	}

	Contact::~Contact() {
	}

	const QString& Contact::getName() const {
		return name_;
	}

	const QString& Contact::getJid() const {
		return jid_;
	}

	void Contact::setName(const QString& name) {
		name_ = name;
	}

	void Contact::setJid(const QString& jid) {
		jid_ = jid;
	}

	const QIcon Contact::getIcon() const {
		// return icon_;
		int maxPriority = -1;
		QIcon icon("icons/offline.png");
		
		foreach(Item* item, items_) {
			Resource* resource = static_cast<Resource*>(item);
			if ( maxPriority < resource->getPriority() ) {
				icon = resource->getIcon();
				maxPriority = resource->getPriority();
			}
		}

		return icon;
	}

	const QIcon& Contact::getAvatar() const {
		return avatar_;
	}

	void Contact::setAvatar(const QIcon& avatar) {
		avatar_ = avatar;
	}

	const QString Contact::getStatusMessage() const {
		return statusMessage_;
	}

	void Contact::addResource(Resource* resource) {
		resource->setParent(this);
		items_.append(resource);
	}

	void Contact::setStatusMessage(const QString& statusMessage) {
		statusMessage_ = statusMessage;
	}

	void Contact::setStatus(const StatusType status) {
		status_ = status;
	}

	const StatusType Contact::getStatus() const {
		return status_;
	}
}

