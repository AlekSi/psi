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
			void menuRename();
			void menuXmlConsole();
			void menuShowResources();
			void menuHideResources();
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
			void menuMood();
			void menuSetAvatarAccount();
			void menuUnsetAvatarAccount();
			void menuAddContact();
			void menuServiceDiscovery();
			void menuModifyAccount();
			void menuNewBlankMessage();

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
			void initMenu();
			void expandWithManager(const QModelIndex& index, bool expanded);

			QMap<QString, QAction*> menuActions_;

			Manager* manager_;
			ViewStateManager* vsm_;
			ViewActionsService* actionsService_;
    };
}

#endif
