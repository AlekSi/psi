#include <QIcon>

#include "contact.h"
#include "resource.h"
#include "psievent.h"

namespace Roster {

	Contact::Contact(const QString& name, const XMPP::Jid& jid) : GroupItem(), name_(name), jid_(jid), status_(STATUS_OFFLINE), incomingEvent_(NULL) {
	}

	Contact::Contact(const Contact& c) : GroupItem() {
		name_ = c.name_;
		jid_ = c.jid_;
		avatar_ = c.avatar_;	
		items_ = c.items_;
	}

	Contact::~Contact() {
	}

	const QString& Contact::getName() const {
		return name_;
	}

	const XMPP::Jid& Contact::getJid() const {
		return jid_;
	}

	void Contact::setName(const QString& name) {
		name_ = name;
	}

	void Contact::setJid(const XMPP::Jid& jid) {
		jid_ = jid;
	}

	const QIcon& Contact::getAvatar() const {
		return avatar_;
	}

	void Contact::setAvatar(const QIcon& avatar) {
		avatar_ = avatar;
	}

	const QString Contact::getStatusMessage() const {
		return statusMessage_;
	}

	void Contact::setStatusMessage(const QString& statusMessage) {
		statusMessage_ = statusMessage;
	}

	void Contact::setStatus(const StatusType status) {
		status_ = status;
	}

	const StatusType Contact::getStatus() const {
		return status_;
	}

	void Contact::setIncomingEvent(PsiEvent* event) {
		incomingEvent_ = event;
	}

	PsiEvent* Contact::getIncomingEvent() const {
		return incomingEvent_;
	}

}

