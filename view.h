#ifndef ROSTER_VIEW_H
#define ROSTER_VIEW_H

#include <QTreeView>
#include <QObject>
#include <QModelIndex>

#include "item.h"

namespace Roster {

    class View : public QTreeView {
		Q_OBJECT

		public:
			View();

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

		private slots:
			void itemExpanded(const QModelIndex& index);
			void itemCollapsed(const QModelIndex& index);

		private:
			Item* getItem(const QModelIndex& index) const;
			QModelIndex senderItemIndex() const;
			void initMenu();

			QAction *sendMessageAct_, *historyAct_, *showResourcesAct_, *hideResourcesAct_, *renameContactAct_, *removeContactAct_, *sendFileAct_, *openChatAct_; // Contact menu
			QAction *sendMessageToGroupAct_, *renameGroupAct_, *removeGroupAct_, *removeGroupAndContactsAct_; // Group menu
			QAction *goOnlineAct_, *goOfflineAct_, *xmlConsoleAct_; // Roster menu
			QAction *sendToAllAct_; // Multiple contacts menu
			QAction *sendMessageToResourceAct_, *openChatToResourceAct_, *sendFileToResourceAct_; // Resource menu
    };
}

#endif
