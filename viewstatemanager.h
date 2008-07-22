#ifndef ROSTER_VIEWSTATEMANAGER_H
#define ROSTER_VIEWSTATEMANAGER_H

#include <QString>
#include <QMap>

namespace Roster {
	class Contact;
	class Group;
	class Metacontact;
	class Account;

	class ViewStateManager {
		public:
			const bool isGroupExpanded(Group* group) const;
			const bool isContactExpanded(Contact* contact) const;
			const bool isMetacontactExpanded(Metacontact* metacontact) const;
			const bool isAccountExpanded(Account* account) const;

			void setGroupExpanded(Group* group, bool expanded);
			void setContactExpanded(Contact* contact, bool expanded);
			void setMetacontactExpanded(Metacontact* metacontact, bool expanded);
			void setAccountExpanded(Account* account, bool expanded);

		private:
			class GroupSpec {
				public:
					GroupSpec(Group* group);
					const bool operator<(const GroupSpec& g) const;
				private:
					QString groupName_;
					QString accountName_;
			};

			class ContactSpec {
				public:
					ContactSpec(Contact* contact);
					const bool operator<(const ContactSpec& c) const;
				private:
					QString jid_;
					QString groupName_; 
					QString accountName_;
			};

			class MetacontactSpec {
				public:
					MetacontactSpec(Metacontact* metacontact);
					const bool operator<(const MetacontactSpec& m) const;
				private:
					QString name_;
					QString groupName_; 
					QString accountName_;
			};

			class AccountSpec {
				public:
					AccountSpec(Account* account);
					const bool operator<(const AccountSpec& m) const;
				private:
					QString name_;
			};

			QMap<GroupSpec, bool> groups_;
			QMap<ContactSpec, bool> contacts_;
			QMap<MetacontactSpec, bool> metacontacts_;
			QMap<AccountSpec, bool> accounts_;
	};

}

#endif
