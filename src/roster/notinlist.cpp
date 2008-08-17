#include "notinlist.h"

namespace Roster {

	NotInList::NotInList(const QString& name, const XMPP::Jid& jid) : Contact(name, jid) {
	}

	NotInList::~NotInList() {
	}

}
