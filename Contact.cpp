#include "Contact.h"

namespace Roster {

	Contact::Contact(const QString& name, const QString& jid) : name_(name), jid_(jid) {
	}

	Contact::~Contact() {
	}

	const QString& Contact::getName() const {
		return name_;
	}

	const QString& Contact::getJid() const {
		return jid_;
	}

	void Contact::setName(const QString& name) {
		name_ = name;
	}

	void Contact::setJid(const QString& jid) {
		jid_ = jid;
	}

}

