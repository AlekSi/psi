#include "statusiconprovider.h"

namespace Roster {

	const QIcon StatusIconProvider::getIconForStatus(const StatusType status) const {
		return QIcon("icons/online.png");
	}

}
