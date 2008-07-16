#include <QString>

#include "group.h"
#include "roster.h"

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

	const QString Group::getGroupPath() const {
		QString path = getParent()->getGroupPath();
		if ( path.isEmpty() ) {
			return name_;
		} else {
			return path + SEPARATOR + name_;
		}
	}

}
