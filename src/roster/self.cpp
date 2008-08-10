#include <QIcon>

#include "self.h"
#include "resource.h"

namespace Roster {

	Self::Self(const QString& name, const XMPP::Jid& jid) : GroupItem(), name_(name), jid_(jid), status_(STATUS_OFFLINE) {
	}

	Self::~Self() {
	}

	const QString& Self::getName() const {
		return name_;
	}

	const XMPP::Jid& Self::getJid() const {
		return jid_;
	}

	void Self::setName(const QString& name) {
		name_ = name;
	}

	void Self::setJid(const XMPP::Jid& jid) {
		jid_ = jid;
	}

	const QIcon& Self::getAvatar() const {
		return avatar_;
	}

	void Self::setAvatar(const QIcon& avatar) {
		avatar_ = avatar;
	}

	const QString Self::getStatusMessage() const {
		return statusMessage_;
	}

	void Self::setStatusMessage(const QString& statusMessage) {
		statusMessage_ = statusMessage;
	}

	void Self::setStatus(const StatusType status) {
		status_ = status;
	}

	const StatusType Self::getStatus() const {
		return status_;
	}

	Resource* Self::findResource(const QString& name) const {
		foreach(Item* item, items_) {
			if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				if ( resource->getName() == name ) {
					return resource;
				}
			}
		}

		return NULL;
	}
}
