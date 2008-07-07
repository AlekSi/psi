#include "rosterbuilder.h"
#include "group.h"
#include "contact.h"
#include "xmpprosteritem.h"
#include "manager.h"
#include "roster.h"

namespace Roster {

	RosterBuilder::RosterBuilder(Manager* manager) : manager_(manager) {
	}

	void RosterBuilder::buildRoster(QString account, GroupItem* root) {
		foreach(XMPPRosterItem* xitem, items_[account]) { 
			foreach(QString xgroup, xitem->getGroups()) {
				Contact* contact = new Contact(xitem->getName(), xitem->getJid());
				Group* group = getGroupForAdd(xgroup, root);
				manager_->addContact(contact, group);
			}
		}		
	}

	void RosterBuilder::buildAllAccounts(GroupItem* root) {
		clear(root);
		foreach(QString account, items_.keys()) {
			Roster* roster = new Roster(account);
			manager_->addRoster(roster, root);

			buildRoster(account, roster);
		}
	}

	void RosterBuilder::buildJoinedAccounts(GroupItem* root) {
		clear(root);
		foreach(QString account, items_.keys()) {
			buildRoster(account, root);
		}
	}

	void RosterBuilder::clear(Item* item) { // FIXME: no resources support
		if ( GroupItem* g = dynamic_cast<GroupItem*>(item) ) {
			foreach(Item* subitem, g->getItems()) {
				clear(subitem);
				manager_->removeItem(subitem);
			}
		}
	}

	Group* RosterBuilder::getGroupForAdd(QString groupName, GroupItem* parent) {
		QStringList groupNames = groupName.split("::");

		GroupItem* up = parent;

		foreach(QString name, groupNames) {
			Group* next = 0;

			foreach(Item* item, up->getItems()) {
				if ( Group* group = dynamic_cast<Group*>(item) ) {
					if ( group->getName() == name ) {
						next = group;
						break;
					}
				}
			}

			if ( next ) {
				up = next;
			} else {
				Group* newGroup = new Group(name);
				manager_->addGroup(newGroup, up);
				up = newGroup;
			}
		}

		return static_cast<Group*>(up);
	}

	void RosterBuilder::addItem(QString account, XMPPRosterItem* item) {
		items_[account].append(item);
	}

}
