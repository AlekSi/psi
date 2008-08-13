#include <QIcon>

#include "self.h"
#include "resource.h"

namespace Roster {

	Self::Self(const QString& name, const XMPP::Jid& jid) : Contact(name, jid) {
	}

	Self::~Self() {
	}

}
