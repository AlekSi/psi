#include "transport.h"
#include "xmpprosteritem.h"

namespace Roster {

	Transport::Transport(const QString& name, const XMPP::Jid& jid) : name_(name), jid_(jid), status_(STATUS_OFFLINE) {
	}

	Transport::~Transport() {
	}

	const QString& Transport::getName() const {
		return name_;
	}

	const XMPP::Jid& Transport::getJid() const {
		return jid_;
	}

	void Transport::setName(const QString& name) {
		name_ = name;
	}

	void Transport::setJid(const XMPP::Jid& jid) {
		jid_ = jid;
	}

	void Transport::setStatus(const StatusType status) {
		status_ = status;
	}

	const StatusType Transport::getStatus() const {
		return status_;
	}

}
