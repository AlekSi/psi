#include "transport.h"

namespace Roster {

	Transport::Transport(const QString& name, const XMPP::Jid& jid) : Contact(name, jid) {
	}

	Transport::~Transport() {
	}

}
