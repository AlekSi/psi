#ifndef ROSTER_ROSTERBUILDER_H
#define ROSTER_ROSTERBUILDER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "xmpprosteritem.h"
#include "globals.h"

namespace Roster {
	class Manager;
	class Group;
	class GroupItem;
	class Item;
	class RosterDataService;
	class Roster;
	class Contact;
	class Metacontact;
	class ViewStateManager;
	class Self;

	class RosterBuilder : public QObject {
		Q_OBJECT

		public:
			RosterBuilder(Roster* root_, Manager* manager, ViewStateManager* vms);
			void registerAccount(const QString& acname, RosterDataService* rosterService);
			void rebuild();
			void setSearch(const QString& searchText);

			unsigned int getFilter();

		public slots:
			void setJoinedAccounts(bool joinedAccounts);
			void setJoinByName(bool joinByName);
			void setFilter(unsigned int itemFilter);
			void setShowOffline(bool show);
			void setShowSelf(bool show);

			void itemAdded(const XMPPRosterItem* xitem, const QString& acname);
			void itemRemoved(const XMPPRosterItem* xitem, const QString& acname);
			void itemChanged(const XMPPRosterItem* xitem, const QString& acname);
			void selfChanged(const XMPPRosterItem* xitem, const QString& acname);
			void accountChanged(const QString& acname);
		
		private:
			void buildRoster(const QString& acname);
			void clear(Item* item);
			void buildJoinedAccounts();
			void buildAllAccounts();

			Group* findGroup(const QString& groupName, const QString& acname, bool create = false);
			QList<Contact*> findContacts(const XMPPRosterItem* xitem, const QString& acname);

			void updateResources(const QList<XMPPResource*> list, GroupItem* groupItem);
			void updateContact(const XMPPRosterItem* xitem, const QString& acname);
			void updateTransport(const XMPPRosterItem* xitem, const QString& acname);
			void updateSelf(const XMPPRosterItem* xitem, const QString& acname);

			void addContact(Contact* contact, Group* group);
			Metacontact* addMetacontact(const QString& name, const QString& acname, GroupItem* parent);
			Group* addGroup(const QString& groupName, const QString& acname, GroupItem* parent);


			const bool isContactVisible(const XMPPRosterItem* xitem) const;
			const bool isTransportVisible(const XMPPRosterItem* xitem) const;

			QMap<QString, RosterDataService*> rosterServices_;

			ViewStateManager* vsm_;
			Roster* root_;
			Manager* manager_;
			bool joinedAccounts_;
			bool joinByName_;
			unsigned int itemFilter_;
			QString searchText_;
	};

}

#endif
