#include <QIcon>

#include "resource.h"

namespace Roster {

	Resource::Resource(const QString& name, const int priority, const QString status) : name_(name), priority_(priority), status_(status) {
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

	const QIcon& Resource::getIcon() const {
		return icon_;
	}

	void Resource::setIcon(const QIcon& icon) {
		icon_ = icon;
	}

}

