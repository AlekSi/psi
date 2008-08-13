#ifndef ROSTER_VIEWACTIONSSERVICE_H
#define ROSTER_VIEWACTIONSSERVICE_H

#include <QObject>
#include <QMap>

#include "globals.h"

class PsiAccount;

class RosterActionsService;

namespace Roster {
	class Contact;
	class Account;
	class Resource;

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
			void openWhiteboard(Contact* contact);
			void resendAuthTo(Contact* contact);
			void rerequestAuthFrom(Contact* contact);
			void removeAuthFrom(Contact* contact);
			void assignAvatar(Contact* contact, const QString& file);
			void clearAvatar(Contact* contact);

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

			// resource actions
			void openChat(Resource* resource);
			void openWhiteboard(Resource* resource);
			void sendMessage(Resource* resource);
			void executeCommand(Resource* resource);
	
		private:
			QMap<QString, RosterActionsService*> services_;
	};

}

#endif
