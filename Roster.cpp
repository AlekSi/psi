#include "Roster.h"

namespace Roster {

	Roster::Roster() {
	}

	Roster::~Roster() {
	}

	const QString& Roster::getName() const {
		return name_;
	}

	void Roster::setName(const QString& name) {
		name_ = name;
	}

	const QIcon& Roster::getIcon() const {
		return icon_;
	}

	void Roster::setIcon(const QIcon& icon) {
		icon_ = icon;
	}
}

