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

	const QIcon StatusIconProvider::getIconForEvent(const EventType event) const {
		switch (event) {
			case MessageMessageEvent:
				return IconsetFactory::iconPtr("psi/message")->icon();
			case MessageHeadlineEvent:
				return IconsetFactory::iconPtr("psi/headline")->icon();
			case MessageChatEvent:
				return IconsetFactory::iconPtr("psi/chat")->icon();
			case MessageErrorEvent:
				return IconsetFactory::iconPtr("psi/system")->icon();
			case FileEvent:
				return IconsetFactory::iconPtr("psi/file")->icon();
			default:
				return IconsetFactory::iconPtr("psi/system")->icon();
		}
	}

}
