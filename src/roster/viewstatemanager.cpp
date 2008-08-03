#include <QDebug>

#include "viewstatemanager.h"
#include "group.h"
#include "contact.h"
#include "metacontact.h"
#include "account.h"

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

}
