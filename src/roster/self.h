#ifndef ROSTER_SELF_H
#define ROSTER_SELF_H

#include "globals.h"
#include "contact.h"

namespace Roster {

	class Self : public Contact {
		public:
			Self(const QString& name, const XMPP::Jid& jid);
			~Self();

	};

}

#endif
