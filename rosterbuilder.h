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
	class Roster;

	class RosterBuilder : public QObject {
		Q_OBJECT

		public:
			RosterBuilder(Roster* root_, Manager* manager);
			void addService(const QString& acname, RosterDataService* service);	
			void rebuild();

		public slots:
			void setJoinedAccounts(bool joinedAccounts);
		
		private:
			void buildRoster(QString acname);
			void clear(Item* item);
			void buildJoinedAccounts();
			void buildAllAccounts();
			Group* createGroup(const QString& groupName, const QString& acname);

			QMap<QString, RosterDataService*> services_;
			Roster* root_;
			Manager* manager_;
			bool joinedAccounts_;
	};

}

#endif
