#ifndef ROSTER_NOTINLIST_H
#define ROSTER_NOTINLIST_H

#include "contact.h"

namespace Roster {

	class NotInList : public Contact {
		public:
			NotInList(const QString& name, const XMPP::Jid& jid);
			~NotInList();
	};

}

#endif
