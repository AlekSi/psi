#ifndef ROSTER_GROUP_H
#define ROSTER_GROUP_H

#include <QString>

#include "GroupItem.h"

namespace Roster {

	// Group roster item
	class Group : public GroupItem {
		public:
			Group(const QString& name);
			~Group();

			const QString& getName() const;
			void setName(const QString& name);

		private:
			QString name_;
	};
}

#endif
