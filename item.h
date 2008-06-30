#ifndef ROSTER_ITEM_H
#define ROSTER_ITEM_H

#include <QMetaType>

namespace Roster {
	class GroupItem;

	class Item {
		public:
			Item();
			virtual ~Item();

			Item* getParent() const;
			unsigned int getId() const;

			void setParent(Item* parent);

		private:
			Item* parent_;
			unsigned int id_;
	};
}

Q_DECLARE_METATYPE(Roster::Item*)

#endif
