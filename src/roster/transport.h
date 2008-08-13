#ifndef ROSTER_TRANSPORT_H
#define ROSTER_TRANSPORT_H

#include "globals.h"
#include "contact.h"

namespace Roster {
	class Resource;
	
	class Transport : public Contact {
		public:
			Transport(const QString& name, const XMPP::Jid& jid);
			~Transport();
	};
}

#endif
