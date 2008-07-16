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
	class ExpandDataService;
	class Roster;
	class Contact;
	class Metacontact;

	class RosterBuilder : public QObject {
		Q_OBJECT

		public:
			RosterBuilder(Roster* root_, Manager* manager, ExpandDataService* joinedExpandService);
			void registerAccount(const QString& acname, RosterDataService* rosterService, ExpandDataService* expService);
			void rebuild();

		public slots:
			void setJoinedAccounts(bool joinedAccounts);
			void setJoinByName(bool joinByName);

			void itemAdded(const XMPPRosterItem* xitem, const QString& acname);
			void itemRemoved(const XMPPRosterItem* xitem, const QString& acname);
			void itemChanged(const XMPPRosterItem* xitem, const QString& acname);
		
		private:
			void buildRoster(const QString& acname);
			void clear(Item* item);
			void buildJoinedAccounts();
			void buildAllAccounts();
			Group* findGroup(const QString& groupName, const QString& acname, bool create = true);
			QList<Contact*> findContacts(const XMPPRosterItem* xitem, const QString& acname);
			void addContact(Contact* contact, Group* group);
			void addItem(const XMPPRosterItem* xitem, const QString& acname);
			Metacontact* addMetacontact(const QString& name, const QString& acname, GroupItem* parent);
			Group* addGroup(const QString& groupName, const QString& acname, GroupItem* parent);

			// FIXME: that's just not nice
			QMap<QString, RosterDataService*> rosterServices_;
			QMap<QString, ExpandDataService*> expandServices_;
			ExpandDataService* joinedExpandService_;

			Roster* root_;
			Manager* manager_;
			bool joinedAccounts_;
			bool joinByName_;
	};

}

#endif
