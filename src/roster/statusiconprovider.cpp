#include "statusiconprovider.h"
#include "psiiconset.h"

namespace Roster {

	const QIcon StatusIconProvider::getIconForStatus(const StatusType status) const {
		switch (status) {
			case STATUS_ONLINE:
				return IconsetFactory::iconPtr("status/online")->icon();
			case STATUS_DND:
				return IconsetFactory::iconPtr("status/dnd")->icon();
			case STATUS_AWAY:
				return IconsetFactory::iconPtr("status/away")->icon();
			case STATUS_XA:
				return IconsetFactory::iconPtr("status/xa")->icon();
			case STATUS_CHAT:
				return IconsetFactory::iconPtr("status/chat")->icon();
				
		}
		return IconsetFactory::iconPtr("status/offline")->icon();
	}

	const QIcon StatusIconProvider::getIconForGroup(const bool expanded) const {
		if ( expanded ) {
			return IconsetFactory::iconPtr("psi/groupOpen")->icon();
		} else {
			return IconsetFactory::iconPtr("psi/groupClosed")->icon();
		}
	}

}
