#include <QIcon>

#include "resource.h"
#include "xmppresource.h"

namespace Roster {

	Resource::Resource(const QString& name, const int priority, const ShowType show, const QString status)
											: name_(name), priority_(priority), show_(show), status_(status) {
	}

	Resource::Resource(const XMPPResource* xresource) {
		name_ = xresource->getName();
		priority_ = xresource->getPriority();
		show_ = xresource->getShow();
		status_ = xresource->getStatus();
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

	const QString& Resource::getStatus() const {
		return status_;
	}

	void Resource::setStatus(const QString& status) {
		status_ = status;
	}

	const QIcon Resource::getIcon() const {
		if ( show_ == STATUS_ONLINE ) {
			return QIcon("icons/online.png");
		} else {
			return QIcon("icons/offline.png");
		}
	}

	const ShowType Resource::getShow() const {
		return show_;
	}

}

