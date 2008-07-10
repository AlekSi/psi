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
	class Item;
	class RosterDataService;

	class RosterBuilder : public QObject {
		Q_OBJECT

		public:
			RosterBuilder(Manager* manager);

			void buildJoinedAccounts(GroupItem* root);
			void buildAllAccounts(GroupItem* root);

			void clear(Item* item);
		
			void addService(const QString& acname, RosterDataService* service);	
		private:
			void buildRoster(QString acname, GroupItem* root);
			Group* getGroupForAdd(QString groupNames, GroupItem* parent);

			QMap<QString, RosterDataService*> services_;
			Manager* manager_;
	};

}

#endif
