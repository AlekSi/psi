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
			const bool isOpen() const;

			void setName(const QString& name);
			void setOpen(const bool open);

		private:
			QString name_;
			bool open_;
	};
}

#endif
