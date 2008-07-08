#include "account.h"

namespace Roster {

	Account::Account() {
	}

	Account::Account(const QString& name) : name_(name) {
	}

	Account::~Account() {
	}

	const QString& Account::getName() const {
		return name_;
	}

	void Account::setName(const QString& name) {
		name_ = name;
	}

	const QIcon& Account::getIcon() const {
		return icon_;
	}

	void Account::setIcon(const QIcon& icon) {
		icon_ = icon;
	}
}

