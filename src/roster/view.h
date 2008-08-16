#ifndef ROSTER_VIEW_H
#define ROSTER_VIEW_H

#include <QTreeView>
#include <QObject>
#include <QModelIndex>
#include <QMap>

#include "conferencebookmark.h"

namespace XMPP {
	class Jid;
}

class ViewDataService;

namespace Roster {
	class Item;
	class Manager;
	class ViewStateManager;
	class ViewActionsService;
	class Group;

    class View : public QTreeView {
		Q_OBJECT

		public:
			View();

			void setManager(Manager* manager);
			void setViewStateManager(ViewStateManager* vsm);
			void setViewActionsService(ViewActionsService* actionsService);

			void registerAccount(const QString& acname, ViewDataService* ds);
			void unregisterAccount(const QString& acname); 

		public slots:
			void showContextMenu(const QPoint& position);
			void doActivated(const QModelIndex& index);

			// contact
			void menuHistory();
			void menuSendMessage();
			void menuRename();
			void menuXmlConsole();
			void menuShowResources();
			void menuHideResources();
			void menuOpenChat();
			void menuRemoveContact();
			void menuRecvEvent();
			void menuSendFile();
			void menuExecuteCommand();
			void menuUserInfo();
			void menuAssignAvatar();
			void menuClearAvatar();
			void menuRemoveAuthFrom();
			void menuRerequestAuthFrom();
			void menuResendAuthTo();
			void menuOpenWhiteboard();
			void menuInvite();
			void menuAssignKey();
			void menuUnassignKey();

			// contact group handling
			void menuMoveToGroup();
			void menuAddGroup();
			void menuMoveToNone();

			// account
			void menuChangeStatus();
			void menuMood();
			void menuSetAvatarAccount();
			void menuUnsetAvatarAccount();
			void menuAddContact();
			void menuServiceDiscovery();
			void menuModifyAccount();
			void menuNewBlankMessage();
			void menuJoinConference();

			// account->admin
			void menuOnlineUsers();
			void menuSendServerMessage();
			void menuSetMOTD();
			void menuUpdateMOTD();
			void menuDeleteMOTD();
			void menuManageBookmarks();

		signals:
			void searchInput(const QString& text);

		private slots:
			void itemExpanded(const QModelIndex& index);
			void itemCollapsed(const QModelIndex& index);

		private:
			virtual bool viewportEvent(QEvent* event);
			void keyPressEvent(QKeyEvent* event);
	
			template<typename T> T getActionItem();
			QModelIndex senderItemIndex() const;
			ViewDataService* getDataService(Item* item);
			bool isSpecial(Group* group) const;

			void initMenu();
			void expandWithManager(const QModelIndex& index, bool expanded);

			QMap<QString, QAction*> menuActions_;
			QMap<QAction*, ConferenceBookmark> bookmarkActions_;
			QMap<QAction*, QString> inviteActions_;
			QMap<QString, ViewDataService*> dataServices_;

			Manager* manager_;
			ViewStateManager* vsm_;
			ViewActionsService* actionsService_;
    };
}

#endif
