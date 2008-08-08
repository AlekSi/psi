#include <QDebug>

#include "viewstatemanager.h"
#include "group.h"
#include "contact.h"
#include "metacontact.h"
#include "account.h"
#include "transport.h"
#include "self.h"

namespace Roster {

	// Private *Spec classes
	ViewStateManager::GroupSpec::GroupSpec(Group* group) {
		accountName_ = group->getAccountName();
		groupName_ = group->getGroupPath();
	}

	const bool ViewStateManager::GroupSpec::operator<(const GroupSpec& g) const {
		return (accountName_ < g.accountName_ or (accountName_ == g.accountName_ and groupName_ < g.groupName_));
	}

	ViewStateManager::ContactSpec::ContactSpec(Contact* contact) {
		jid_ = contact->getJid();
		groupName_ = contact->getGroupPath();
		accountName_ = contact->getAccountName();
	}

	const bool ViewStateManager::ContactSpec::operator<(const ContactSpec& c) const {
		if ( jid_.full() != c.jid_.full() ) {
			return jid_.full() < c.jid_.full();
		}
		if ( groupName_ != c.groupName_ ) {
			return groupName_ < c.groupName_;
		}
		return accountName_ < c.accountName_;
	}

	ViewStateManager::MetacontactSpec::MetacontactSpec(Metacontact* metacontact) {
		name_ = metacontact->getName();
		groupName_ = metacontact->getGroupPath();
		accountName_ = metacontact->getAccountName();
	}

	const bool ViewStateManager::MetacontactSpec::operator<(const MetacontactSpec& m) const {
		if ( name_ != m.name_ ) {
			return name_ < m.name_;
		}
		if ( groupName_ != m.groupName_ ) {
			return groupName_ < m.groupName_;
		}
		return accountName_ < m.accountName_;
	}

	ViewStateManager::AccountSpec::AccountSpec(Account* account) {
		name_ = account->getName();
	}

	const bool ViewStateManager::AccountSpec::operator<(const AccountSpec& a) const {
		return name_ < a.name_;
	}

	ViewStateManager::TransportSpec::TransportSpec(Transport* transport) {
		jid_ = transport->getJid();
		accountName_ = transport->getAccountName();
	}

	const bool ViewStateManager::TransportSpec::operator<(const TransportSpec& t) const {
		return (accountName_ < t.accountName_ or (accountName_ == t.accountName_ and jid_.full() < t.jid_.full()));
	}

	ViewStateManager::SelfSpec::SelfSpec(Self* self) {
		accountName_ = self->getAccountName();
	}

	const bool ViewStateManager::SelfSpec::operator<(const SelfSpec& s) const {
		return accountName_ < s.accountName_;
	}

	// ViewStateManager
	const bool ViewStateManager::isGroupExpanded(Group* group) const {
		if ( !groups_.contains(GroupSpec(group)) ) {
			return true;
		}
		return groups_.value(GroupSpec(group));
	}

	const bool ViewStateManager::isMetacontactExpanded(Metacontact* metacontact) const {
		return metacontacts_.value(MetacontactSpec(metacontact));
	}

	const bool ViewStateManager::isContactExpanded(Contact* contact) const {
		return contacts_.value(ContactSpec(contact));
	}

	const bool ViewStateManager::isAccountExpanded(Account* account) const {
		if ( !accounts_.contains(AccountSpec(account)) ) {
			return true;
		}
		return accounts_.value(AccountSpec(account));
	}

	const bool ViewStateManager::isTransportExpanded(Transport* transport) const {
		return transports_.value(TransportSpec(transport));
	}

	const bool ViewStateManager::isSelfExpanded(Self* self) const {
		return selfs_.value(SelfSpec(self));
	}

	void ViewStateManager::setGroupExpanded(Group* group, bool expanded) {
		groups_.insert(GroupSpec(group), expanded);
		group->setExpanded(expanded);
	}

	void ViewStateManager::setContactExpanded(Contact* contact, bool expanded) {
		contacts_.insert(ContactSpec(contact), expanded);
		contact->setExpanded(expanded);
	}

	void ViewStateManager::setMetacontactExpanded(Metacontact* metacontact, bool expanded) {
		metacontacts_.insert(MetacontactSpec(metacontact), expanded);
		metacontact->setExpanded(expanded);
	}

	void ViewStateManager::setAccountExpanded(Account* account, bool expanded) {
		accounts_.insert(AccountSpec(account), expanded);
		account->setExpanded(expanded);
	}

	void ViewStateManager::setTransportExpanded(Transport* transport, bool expanded) {
		transports_.insert(TransportSpec(transport), expanded);
		transport->setExpanded(expanded);
	}

	void ViewStateManager::setSelfExpanded(Self* self, bool expanded) {
		selfs_.insert(SelfSpec(self), expanded);
		self->setExpanded(expanded);
	}

}
