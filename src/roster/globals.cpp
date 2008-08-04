#include "globals.h"

namespace Roster {
	const QString statusToText(const StatusType& status) {
		QString text = "status/offline";
		switch (status) {
			case STATUS_ONLINE:
				text = "status/online"; break;
			case STATUS_DND:
				text = "status/dnd"; break;
			case STATUS_AWAY:
				text = "status/away"; break;
			case STATUS_XA:
				text = "status/xa"; break;
			case STATUS_CHAT:
				text = "status/chat"; break;
			case STATUS_INVISIBLE:
				text = "status/invisible"; break;
			case STATUS_OFFLINE:
				text = "status/offline"; break;
		}
		return text;
	}
}
