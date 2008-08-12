#ifndef ROSTER_VIEWACTIONSSERVICE_H
#define ROSTER_VIEWACTIONSSERVICE_H

#include <QObject>
#include <QMap>

#include "globals.h"

class PsiAccount;

namespace Roster {
	class RosterActionsService;
	class Contact;
	class Account;

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
			void executeCommand(Contact* contact);
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
	
		private:
			QMap<QString, RosterActionsService*> managers_;
	};

}

#endif
