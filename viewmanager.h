#ifndef ROSTER_VIEWMANAGER_H
#define ROSTER_VIEWMANAGER_H

#include <QMap>

namespace Roster {
	class Contact;
	class ExpandDataService;
	class Group;
	class Metacontact;
	class Account;

	class ViewManager {
		public:
			ViewManager(ExpandDataService* joinedExpandService);

			void registerAccount(const QString& acname, ExpandDataService* expService);
			void setContactExpanded(Contact* contact, bool expanded);
			void setGroupExpanded(Group* group, bool expanded);
			void setMetacontactExpanded(Metacontact* metacontact, bool expanded);
			void setAccountExpanded(Account* account, bool expanded);

		private:
			QMap<QString, ExpandDataService*> expandServices_;

			ExpandDataService* joinedExpandService_; 
	};
	
}

#endif
