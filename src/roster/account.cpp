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

	void Account::setStatus(const StatusType status) {
		status_ = status;
	}

	const StatusType Account::getStatus() const {
		return status_;
	}

}

