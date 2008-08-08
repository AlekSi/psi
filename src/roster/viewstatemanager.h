#ifndef ROSTER_VIEWSTATEMANAGER_H
#define ROSTER_VIEWSTATEMANAGER_H

#include <QString>
#include <QMap>

#include "globals.h"

namespace Roster {
	class Contact;
	class Group;
	class Metacontact;
	class Account;
	class Transport;
	class Self;

	class ViewStateManager {
		public:
			const bool isGroupExpanded(Group* group) const;
			const bool isContactExpanded(Contact* contact) const;
			const bool isMetacontactExpanded(Metacontact* metacontact) const;
			const bool isAccountExpanded(Account* account) const;
			const bool isTransportExpanded(Transport* transport) const;
			const bool isSelfExpanded(Self* self) const;

			void setGroupExpanded(Group* group, bool expanded);
			void setContactExpanded(Contact* contact, bool expanded);
			void setMetacontactExpanded(Metacontact* metacontact, bool expanded);
			void setAccountExpanded(Account* account, bool expanded);
			void setTransportExpanded(Transport* transport, bool expanded);
			void setSelfExpanded(Self* self, bool expanded);

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
					XMPP::Jid jid_;
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
					const bool operator<(const AccountSpec& a) const;
				private:
					QString name_;
			};

			class TransportSpec {
				public:
					TransportSpec(Transport* transport);
					const bool operator<(const TransportSpec& t) const;
				private:
					XMPP::Jid jid_;
					QString accountName_;
			};

			class SelfSpec {
				public:
					SelfSpec(Self* self);
					const bool operator<(const SelfSpec& t) const;
				private:
					QString accountName_;
			};

			QMap<GroupSpec, bool> groups_;
			QMap<ContactSpec, bool> contacts_;
			QMap<MetacontactSpec, bool> metacontacts_;
			QMap<AccountSpec, bool> accounts_;
			QMap<TransportSpec, bool> transports_;
			QMap<SelfSpec, bool> selfs_;
	};

}

#endif
