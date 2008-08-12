#include "account.h"

namespace Roster {

	Account::Account() {
	}

	Account::Account(const QString& name, const XMPP::Jid& jid) : name_(name), jid_(jid) {
	}

	Account::~Account() {
	}

	const QString& Account::getName() const {
		return name_;
	}

	void Account::setName(const QString& name) {
		name_ = name;
	}

	const XMPP::Jid& Account::getJid() const {
		return jid_;
	}

	void Account::setJid(const XMPP::Jid& jid) {
		jid_ = jid;
	}

	void Account::setStatus(const StatusType status) {
		status_ = status;
	}

	const StatusType Account::getStatus() const {
		return status_;
	}

}

