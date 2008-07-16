#include <QDebug>

#include "expanddataservice.h"

namespace Roster {

	const bool ExpandDataService::isContactExpanded(const QString& jid, const QString& groupName) const {
		if ( contacts_.contains(qMakePair(jid, groupName)) ) {
			return contacts_[qMakePair(jid, groupName)];
		} else {
			return false;
		}
	}

	void ExpandDataService::setContactExpanded(const QString& jid, const QString& groupName, bool expanded) {
		contacts_.insert(qMakePair(jid, groupName), expanded);
	}

	const bool ExpandDataService::isGroupExpanded(const QString& groupName) const {
		if ( groups_.contains(groupName) ) {
			return groups_[groupName];
		} else {
			return true;
		}
	}

	void ExpandDataService::setGroupExpanded(const QString& groupName, bool expanded) {
		groups_.insert(groupName, expanded);
	}

}
