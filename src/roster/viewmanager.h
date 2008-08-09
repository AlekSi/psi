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

			void registerAccount(PsiAccount* acc);
			void sendMessage(Contact* contact);
		
		private:
			QMap<QString, ViewAccountManager*> managers_;
	};

}

#endif
