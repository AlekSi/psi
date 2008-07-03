#include <QMenu>
#include <QMenuBar>
#include <QDebug>

#include "mainwindow.h"
#include "model.h"
#include "view.h"
#include "delegate.h"

#include "rootitem.h"
#include "roster.h"
#include "group.h"
#include "contact.h"
#include "resource.h"
#include "manager.h"

namespace Roster {
	MainWindow::MainWindow() {
		setupTestData();

		manager_ = new Manager;
		view_ = new View;
		view_->setItemDelegate(new Delegate);
		model_ = new Model(data_);
		view_->setModel(model_);

		model_->setManager(manager_);
		view_->setManager(manager_);

		setCentralWidget(view_);
		setupMenus();

		view_->expandAll();

		// FIXME: hide resources by default ;-)
		view_->collapse(model_->index(0, 0, model_->index(0, 0)));
		view_->collapse(model_->match(model_->index(0, 0), Qt::DisplayRole, "Romeo", 1, Qt::MatchRecursive).at(0));

		view_->resizeColumnToContents(0);
		resize(200, 350);
	}

	void MainWindow::setupMenus() {
		toggleAvatarsAct_ = new QAction(tr("Show avatars"), this);
		toggleStatusAct_ = new QAction(tr("Show status messages"), this);

		toggleAvatarsAct_->setCheckable(true);
		toggleAvatarsAct_->setChecked(true);
		toggleStatusAct_->setCheckable(true);
		toggleStatusAct_->setChecked(true);

		connect(toggleAvatarsAct_, SIGNAL(toggled(bool)), model_, SLOT(setShowAvatars(bool)));
		connect(toggleStatusAct_, SIGNAL(toggled(bool)), model_, SLOT(setShowStatus(bool)));

		QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
		viewMenu->addAction(toggleAvatarsAct_);
		viewMenu->addAction(toggleStatusAct_);
	}

	void MainWindow::setupTestData() {
		data_ = new RootItem;

		Contact* c1 = new Contact("Gislan", "gislan@utumno.pl");
		Contact* c2 = new Contact("Remko", "remko@el-tramo.be");
		Contact* c3 = new Contact("you@jabber.org", "you@jabber.org");
		Contact* c4 = new Contact("me@jabber.org", "me@jabber.org");
		Contact* c5 = new Contact("Romeo", "romeo@monteki.com");
		
		c1->setIcon(QIcon("icons/online.png"));
		c2->setIcon(QIcon("icons/online.png"));
		c3->setIcon(QIcon("icons/offline.png"));
		c4->setIcon(QIcon("icons/offline.png"));
		c5->setIcon(QIcon("icons/online.png"));

		c1->setAvatar(QIcon("icons/avatar.png"));
		c2->setAvatar(QIcon("icons/remko.jpg"));

//		c1->setStatus("working on GSoC");
//		c4->setStatus("relaxing");
//		c5->setStatus("<3 Juliet");

		Resource* r1 = new Resource("Home", 6);
		Resource* r2 = new Resource("Work", 4);
		Resource* r3 = new Resource("Verona", 6);

		r1->setIcon(QIcon("icons/online.png"));
		r2->setIcon(QIcon("icons/online.png"));
		r3->setIcon(QIcon("icons/online.png"));

		r1->setStatus("Working on GSoC");
		r3->setStatus("<3 Juliet");

		c1->addResource(r1);
		c1->addResource(r2);
		c5->addResource(r3);

		Group* g1 = new Group("Group #1");
		Group* g2 = new Group("Subgroup #2");
	
		g1->addItem(c3);
		g1->addItem(c4);
		g2->addItem(c5);

		g1->addItem(g2);

		Roster* roster = new Roster();
		roster->setName("gislan@utumno.pl");
		roster->setIcon(QIcon("icons/online.png"));
		roster->addItem(c1);
		roster->addItem(c2);
		roster->addItem(g1);

		data_->addItem(roster);
	}
}

