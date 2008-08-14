#include "statusiconprovider.h"
#include "psiiconset.h"

namespace Roster {

	const QIcon StatusIconProvider::getIconForStatus(const StatusType status) const {
		return IconsetFactory::iconPtr(statusToText(status))->icon();
	}

	const QIcon StatusIconProvider::getIconForGroup(const bool expanded) const {
		if ( expanded ) {
			return IconsetFactory::iconPtr("psi/groupOpen")->icon();
		} else {
			return IconsetFactory::iconPtr("psi/groupClosed")->icon();
		}
	}

	const QIcon StatusIconProvider::getIconForEvent(PsiEvent* event) const {
		return PsiIconset::instance()->event2icon(event)->icon();
	}

}
