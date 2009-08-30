#ifndef ROSTER_VIEWACTIONSSERVICE_H
#define ROSTER_VIEWACTIONSSERVICE_H

#include <QObject>
#include <QMap>

#include "globals.h"

class PsiAccount;

class RosterActionsService;
class ConferenceBookmark;

namespace Roster {
	class Contact;
	class Account;
	class Resource;
	class Group;
	class GroupItem;
	class NotInList;
	class Metacontact;

	class ViewActionsService : public QObject {
		Q_OBJECT

		public:
			ViewActionsService();
			~ViewActionsService();

			void registerAccount(const QString& acname, PsiAccount* acc);
			void unregisterAccount(const QString& acname);
			
			// contact actions
			void sendMessage(Contact* contact);
			void openChat(Contact* contact);
			void sendFile(Contact* contact);
			void removeContact(Contact* contact);
			void showHistory(Contact* contact); 
			void userInfo(Contact* contact);
#ifdef WHITEBOARDING
			void openWhiteboard(Contact* contact);
#endif
			void resendAuthTo(Contact* contact);
			void rerequestAuthFrom(Contact* contact);
			void removeAuthFrom(Contact* contact);
			void assignAvatar(Contact* contact, const QString& file);
			void clearAvatar(Contact* contact);
			void recvEvent(Contact* contact);
			void invite(Contact* contact, QString groupchat);
			void assignKey(Contact* contact);
			void unassignKey(Contact* contact);
			void moveToGroup(Contact* contact, const QString& target);
			void copyToGroup(Contact* contact, const QString& target);
			void moveToNone(Contact* contact);
			void rename(Contact* contact, const QString& name);

			// not in list actions
			void addAuthorize(NotInList* nil);

			// group actions
			void rename(Group* group, const QString& name);
			void sendMessage(Group* group);
			void remove(Group* group);
			void removeAll(Group* group);
			void moveGroup(Group* group, GroupItem* parent);

			// account actions
			void changeStatus(Account* account, const StatusType status);
			void newBlankMessage(Account* account);
			void serviceDiscovery(Account* account);
			void mood(Account* account);
			void setAvatar(Account* account);
			void unsetAvatar(Account* account);
			void modifyAccount(Account* account);
			void addContact(Account* account);
			void xmlConsole(Account* account);
			void setMOTD(Account* account);
			void updateMOTD(Account* account);
			void deleteMOTD(Account* account);
			void onlineUsers(Account* account);
			void sendServerMessage(Account* account);
			void manageBookmarks(Account* account);
			void joinConference(Account* account, ConferenceBookmark c);

			// resource actions
			void openChat(Resource* resource);
#ifdef WHITEBOARDING
			void openWhiteboard(Resource* resource);
#endif
			void sendMessage(Resource* resource);
			void executeCommand(Resource* resource);

			// metacontact actions
			void sendMessage(Metacontact* metacontact);
			void openChat(Metacontact* metacontact);
			void sendFile(Metacontact* metacontact);
#ifdef WHITEBOARDING
			void openWhiteboard(Metacontact* metacontact);
#endif
			void recvEvent(Metacontact* metacontact);

	
		private:
			QMap<QString, RosterActionsService*> services_;
	};

}

#endif
