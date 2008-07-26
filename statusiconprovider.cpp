#include "statusiconprovider.h"

namespace Roster {

	const QIcon StatusIconProvider::getIconForStatus(const StatusType status) const {
		return QIcon("icons/online.png");
	}

	const QIcon StatusIconProvider::getIconForGroup(const bool expanded) const {
		if ( expanded ) {
			return QIcon("icons/groupopen.png");
		} else {
			return QIcon("icons/groupclose.png");
		}
	}

}
