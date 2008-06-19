#include <QString>

#include "Group.h"

namespace Roster {

	Group::Group(const QString& name) : name_(name) {
	}

	Group::~Group() {
	}

	const QString& Group::getName() const {
		return name_;
	}

	void Group::setName(const QString& name) {
		name_ = name;
	}

}
