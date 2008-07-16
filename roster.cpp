#include "roster.h"
#include "account.h"
#include "group.h"

namespace Roster {

	Roster::Roster() {
	}

	Roster::~Roster() {
	}

	Account* Roster::findAccount(const QString& name) {
		foreach(Item* item, items_) {
			if ( Account* account = dynamic_cast<Account*>(item) ) {
				if ( account->getName() == name ) {
					return account;
				}
			}
		}

		return 0;
	}

	Group* Roster::findNestedGroup(const QString& name, const QString& acname) {
		GroupItem* item = findAccount(acname);
		
		if ( ! item ) {
			item = this;
		}

		QList<QString> names = name.split(SEPARATOR);
		foreach(QString lname, names) {
			item = item->findGroup(lname);

			if ( ! item ) {
				return 0;
			}
		}

		return static_cast<Group*>(item);
	}

	const QString Roster::getGroupPath() const {
		return QString();
	}
}

