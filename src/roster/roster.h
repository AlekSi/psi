#ifndef ROSTER_ROSTER_H
#define ROSTER_ROSTER_H

#include <QString>

#include "groupitem.h"
#include "globals.h"

namespace Roster {
	class Account;

	class Roster : public GroupItem {
		public:
			Roster();
			~Roster();

			Account* findAccount(const QString& name);
			Group* findNestedGroup(const QString& name, const QString& acname);

			const QString getGroupPath() const;
	};

}

#endif
