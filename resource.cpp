#include <QIcon>

#include "resource.h"
#include "xmppresource.h"

namespace Roster {

	Resource::Resource(const QString& name, const int priority, const StatusType status, const QString statusMessage)
											: name_(name), priority_(priority), status_(status), statusMessage_(statusMessage) {
	}

	Resource::Resource(const XMPPResource* xresource) {
		name_ = xresource->getName();
		priority_ = xresource->getPriority();
		status_ = xresource->getStatus();
		statusMessage_ = xresource->getStatusMessage();
	}

	Resource::~Resource() {
	}

	const QString& Resource::getName() const {
		return name_;
	}

	void Resource::setName(const QString& name) {
		name_ = name;
	}

	const int Resource::getPriority() const {
		return priority_;
	}

	void Resource::setPriority(const int priority) {
		priority_ = priority;
	}

	const QString& Resource::getStatusMessage() const {
		return statusMessage_;
	}

	void Resource::setStatusMessage(const QString& statusMessage) {
		statusMessage_ = statusMessage;
	}

	const QIcon Resource::getIcon() const {
		if ( status_ == STATUS_ONLINE ) {
			return QIcon("icons/online.png");
		} else {
			return QIcon("icons/offline.png");
		}
	}

	const StatusType Resource::getStatus() const {
		return status_;
	}

	void Resource::setStatus(const StatusType status) {
		status_ = status;
	}
}

