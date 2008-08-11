#ifndef ROSTER_VIEW_H
#define ROSTER_VIEW_H

#include <QTreeView>
#include <QObject>
#include <QModelIndex>
#include <QMap>

namespace XMPP {
	class Jid;
}

namespace Roster {
	class Item;
	class Manager;
	class ViewStateManager;
	class ViewActionsService;

    class View : public QTreeView {
		Q_OBJECT

		public:
			View();

			void setManager(Manager* manager);
			void setViewStateManager(ViewStateManager* vsm);
			void setViewActionsService(ViewActionsService* actionsService);

		public slots:
			void showContextMenu(const QPoint& position);
			void doActivated(const QModelIndex& index);

			void menuHistory();
			void menuSendMessage();
			void menuSendMessageToGroup();
			void menuRename();
			void menuXmlConsole();
			void menuGoOnline();
			void menuGoOffline();
			void menuSendToAll();
			void menuShowResources();
			void menuHideResources();
			void menuRemoveGroup();
			void menuRemoveGroupAndContacts();
			void menuSendFileToResource();
			void menuOpenChatToResource();
			void menuSendMessageToResource();
			void menuOpenChat();
			void menuRemoveContact();
			void menuSendFile();
			void menuExecuteCommand();
			void menuUserInfo();
			void menuAssignAvatar();
			void menuClearAvatar();
			void menuRemoveAuthFrom();
			void menuRerequestAuthFrom();
			void menuResendAuthTo();
			void menuOpenWhiteboard();
			void menuChangeStatus();

		signals:
			void searchInput(const QString& text);

		private slots:
			void itemExpanded(const QModelIndex& index);
			void itemCollapsed(const QModelIndex& index);

		private:
			virtual bool viewportEvent(QEvent* event);
			void keyPressEvent(QKeyEvent* event);

			QModelIndex senderItemIndex() const;
			void initMenu();
			void expandWithManager(const QModelIndex& index, bool expanded);

			QMap<QString, QAction*> menuActions_;
			QAction *sendMessageToGroupAct_, *renameGroupAct_, *removeGroupAct_, *removeGroupAndContactsAct_; // Group menu
			QAction *sendToAllAct_; // Multiple contacts menu
			QAction *sendMessageToResourceAct_, *openChatToResourceAct_, *sendFileToResourceAct_; // Resource menu

			Manager* manager_;
			ViewStateManager* vsm_;
			ViewActionsService* actionsService_;
    };
}

#endif
