#include <QIcon>

#include "contact.h"
#include "resource.h"
#include "psievent.h"

namespace Roster {

	Contact::Contact(const QString& name, const XMPP::Jid& jid) : GroupItem(), name_(name), jid_(jid), status_(STATUS_OFFLINE), incomingEvent_(NoEvent), hasPGPKey_(false), hasManualAvatar_(false) {
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

	void Contact::setIncomingEvent(const EventType event) {
		incomingEvent_ = event;
	}

	const EventType Contact::getIncomingEvent() const {
		return incomingEvent_;
	}

	void Contact::setHasPGPKey(const bool h) {
		hasPGPKey_ = h;
	}

	const bool Contact::hasPGPKey() const {
		return hasPGPKey_;
	}

	const bool Contact::hasManualAvatar() const {
		return hasManualAvatar_;
	}

	void Contact::setHasManualAvatar(const bool h) {
		hasManualAvatar_ = h;
	}

	const XMPP::Subscription Contact::getSubscription() const {
		return subscription_;
	}

	void Contact::setSubscription(const XMPP::Subscription& subscription) {
		subscription_ = subscription;
	}


}

