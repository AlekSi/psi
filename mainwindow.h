#ifndef ROSTER_MAINWINDOW_H
#define ROSTER_MAINWINDOW_H

#include <QMainWindow>

class QAbstractItemModel;
class QTreeView;

namespace Roster {
	class RootItem;
	class Manager;
	class Model;
	class View;
	class RosterBuilder;

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
			RootItem* data_;
			Manager* manager_;
			RosterBuilder* rb_;

			QAction *toggleAvatarsAct_, *toggleStatusAct_;
	};
}

#endif
