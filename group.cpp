#include <QString>

#include "group.h"

namespace Roster {

	Group::Group(const QString& name) : name_(name), open_(true) {
	}

	Group::~Group() {
	}

	const QString& Group::getName() const {
		return name_;
	}

	void Group::setName(const QString& name) {
		name_ = name;
	}

	const bool Group::isOpen() const {
		return open_;
	}

	void Group::setOpen(const bool open) {
		open_ = open;
	}
}
