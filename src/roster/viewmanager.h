#ifndef ROSTER_VIEWMANAGER_H
#define ROSTER_VIEWMANAGER_H

#include <QObject>
#include <QMap>

class PsiAccount;

namespace Roster {
	class ViewAccountManager;
	class Contact;

	class ViewManager : public QObject {
		Q_OBJECT

		public:
			ViewManager();
			~ViewManager();

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
		
		private:
			QMap<QString, ViewAccountManager*> managers_;
	};

}

#endif
