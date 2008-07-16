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
			const QString& getAccountName() const;

			void setParent(GroupItem* parent);
			void setAccountName(const QString& acname);

			virtual const QString getGroupPath() const;

		private:
			GroupItem* parent_;
			unsigned int id_;
			QString acname_;
	};
}

Q_DECLARE_METATYPE(Roster::Item*)

#endif
