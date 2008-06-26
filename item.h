#ifndef ROSTER_ITEM_H
#define ROSTER_ITEM_H

#include <QMetaType>

namespace Roster {
	class GroupItem;

	class Item {
		public:
			Item();
			virtual ~Item();

			GroupItem* getParent() const;
			unsigned int getId() const;

			void setParent(GroupItem* parent);

		private:
			GroupItem* parent_;
	};
}

Q_DECLARE_METATYPE(Roster::Item*)

#endif
