#ifndef ROSTER_MAINWINDOW_H
#define ROSTER_MAINWINDOW_H

#include <QMainWindow>

class QAbstractItemModel;
class QTreeView;

namespace Roster {
	class Roster;
	class Manager;
	class Model;
	class View;
	class RosterBuilder;
	class ViewManager;
	class ExpandDataService;

	class MainWindow : public QMainWindow {
		Q_OBJECT

		public:
			MainWindow();

		private:
			void setupTestData();
			void buildTestRoster();
			void setupMenus();

			Model* model_;
			View* view_;
			Roster* data_;
			Manager* manager_;
			RosterBuilder* rb_;
			ViewManager* vm_;
			ExpandDataService* joinedExpandService_;

			QAction *toggleAvatarsAct_, *toggleStatusAct_, *toggleJoinedAccountsAct_, *toggleJoinByNameAct_, *toggleShowOfflineAct_;
	};
}

#endif
