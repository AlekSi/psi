#ifndef ROSTER_MAINWINDOW_H
#define ROSTER_MAINWINDOW_H

#include <QMainWindow>

class QAbstractItemModel;
class QTreeView;

namespace Roster {
	class RosterList;

	class MainWindow : public QMainWindow {
		Q_OBJECT

		public:
			MainWindow();

		private:
			void setupTestData();

			QAbstractItemModel* model_;
			QTreeView* view_;
			RosterList* data_;
	};
}

#endif
