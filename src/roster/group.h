#ifndef ROSTER_GROUP_H
#define ROSTER_GROUP_H

#include <QString>

#include "groupitem.h"

namespace Roster {

	// Group roster item
	class Group : public GroupItem {
		public:
			Group(const QString& name);
			~Group();

			const QString& getName() const;
			void setName(const QString& name);

			const QString getGroupPath() const;

		private:
			QString name_;
	};
}

#endif
