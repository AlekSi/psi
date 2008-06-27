#include <QIcon>

#include "contact.h"
#include "resource.h"

namespace Roster {

	Contact::Contact(const QString& name, const QString& jid) : name_(name), jid_(jid) {
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

	const QIcon& Contact::getIcon() const {
		return icon_;
	}

	void Contact::setIcon(const QIcon& icon) {
		icon_ = icon;
	}

	const QIcon& Contact::getAvatar() const {
		return avatar_;
	}

	void Contact::setAvatar(const QIcon& avatar) {
		avatar_ = avatar;
	}

	const QString Contact::getStatus() const {
		int maxPriority = -1;
		QString status;
		
		foreach(Item* item, items_) {
			Resource* resource = dynamic_cast<Resource*>(item);

			if ( maxPriority < resource->getPriority() ) {
				status = resource->getStatus();
				maxPriority = resource->getPriority();
			}
		}

		return status;
	}

	void Contact::setStatus(const QString& status) {
		status_ = status;
	}
}

