#ifndef ROSTER_ROSTERBUILDER_H
#define ROSTER_ROSTERBUILDER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "xmpprosteritem.h"

namespace Roster {
	class Manager;
	class Group;
	class GroupItem;
	class Roster;
	class Item;

	class RosterBuilder : public QObject {
		Q_OBJECT

		public:
			RosterBuilder(Manager* manager);

			void buildJoinedAccounts(GroupItem* root);
			void buildAllAccounts(GroupItem* root);

			void addItem(QString account, XMPPRosterItem* item);
			void clear(Item* item);
			
		private:
			void buildRoster(QString account, GroupItem* root);
			Group* getGroupForAdd(QString groupNames, GroupItem* parent);

			QMap< QString, QList<XMPPRosterItem*> > items_;
			Manager* manager_;
	};

}

#endif
