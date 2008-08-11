#ifndef ROSTER_VIEWACTIONSSERVICE_H
#define ROSTER_VIEWACTIONSSERVICE_H

#include <QObject>
#include <QMap>

class PsiAccount;

namespace Roster {
	class RosterActionsService;
	class Contact;

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
	
		private:
			QMap<QString, RosterActionsService*> managers_;
	};

}

#endif
