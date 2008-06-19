#ifndef ROSTER_VIEW_H
#define ROSTER_VIEW_H

#include <QTreeView>
#include <QObject>
#include <QModelIndex>

#include "Item.h"

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
			void menuRenameGroup();

		private:
			Item* getItem(const QModelIndex& index) const;

			QAction *sendMessageAct_, *historyAct_; // Contact menu
			QAction *sendMessageToGroupAct_, *renameGroupAct_; // Group menu
    };
}

#endif
