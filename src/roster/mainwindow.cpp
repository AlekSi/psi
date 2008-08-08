#include <QMenu>
#include <QMenuBar>
#include <QDebug>

#include "mainwindow.h"
#include "model.h"
#include "view.h"
#include "delegate.h"

#include "roster.h"
#include "account.h"
#include "group.h"
#include "contact.h"
#include "resource.h"
#include "manager.h"
#include "metacontact.h"
#include "rosterbuilder.h"
#include "rosterdataservice.h"
#include "viewstatemanager.h"
#include "statusiconprovider.h"

#include "psidataservice.h"

namespace Roster {
	MainWindow::MainWindow() {
		data_ = new Roster;
//		setupTestData();

		manager_ = new Manager;
		vsm_ = new ViewStateManager;
		rb_ = new RosterBuilder(data_, manager_, vsm_);

		statusIconProvider_ = new StatusIconProvider;
		
		view_ = new View;
		view_->setItemDelegate(new Delegate);
		model_ = new Model(data_);
		view_->setModel(model_);
		view_->setViewStateManager(vsm_);
		model_->setStatusIconProvider(statusIconProvider_);

		connect(model_, SIGNAL(expand(const QModelIndex&)), view_, SLOT(expand(const QModelIndex&)));
		connect(model_, SIGNAL(collapse(const QModelIndex&)), view_, SLOT(collapse(const QModelIndex&)));
		connect(view_, SIGNAL(searchInput(const QString&)), SLOT(searchInput(const QString&)));

		model_->setManager(manager_);
		view_->setManager(manager_);
		model_->setRosterBuilder(rb_);

		setCentralWidget(view_);
		setupMenus();

		buildTestRoster();

//		view_->expandAll();

		// FIXME: hide resources by default ;-)
		//view_->collapse(model_->index(0, 0, model_->index(0, 0)));
		//view_->collapse(model_->match(model_->index(0, 0), Qt::DisplayRole, "Romeo", 1, Qt::MatchRecursive).at(0));

		view_->resizeColumnToContents(0);
		resize(200, 350);
	}

	void MainWindow::searchInput(const QString& text) {
		searchText_ += text;
		rb_->setSearch(searchText_);
	}

	void MainWindow::setupMenus() {
		toggleAvatarsAct_ = new QAction(tr("Show avatars"), this);
		toggleStatusAct_ = new QAction(tr("Show status messages"), this);
		toggleJoinedAccountsAct_ = new QAction(tr("Join all accounts"), this);
		toggleJoinByNameAct_ = new QAction(tr("Join contacts by name"), this);
		toggleShowOfflineAct_ = new QAction(tr("Show offline contacts"), this);

		toggleAvatarsAct_->setCheckable(true);
		toggleAvatarsAct_->setChecked(true);
		toggleStatusAct_->setCheckable(true);
		toggleStatusAct_->setChecked(true);
		toggleJoinedAccountsAct_->setCheckable(true);
		toggleJoinedAccountsAct_->setChecked(false);
		toggleJoinByNameAct_->setCheckable(true);
		toggleJoinByNameAct_->setChecked(false);
		toggleShowOfflineAct_->setCheckable(true);
		toggleShowOfflineAct_->setChecked(false);
		
		connect(toggleAvatarsAct_, SIGNAL(toggled(bool)), model_, SLOT(setShowAvatars(bool)));
		connect(toggleStatusAct_, SIGNAL(toggled(bool)), model_, SLOT(setShowStatusMessages(bool)));
		connect(toggleJoinedAccountsAct_, SIGNAL(toggled(bool)), rb_, SLOT(setJoinedAccounts(bool)));
		connect(toggleJoinByNameAct_, SIGNAL(toggled(bool)), rb_, SLOT(setJoinByName(bool)));
		connect(toggleShowOfflineAct_, SIGNAL(toggled(bool)), rb_, SLOT(setShowOffline(bool)));

		QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
		viewMenu->addAction(toggleAvatarsAct_);
		viewMenu->addAction(toggleStatusAct_);
		viewMenu->addAction(toggleJoinedAccountsAct_);
		viewMenu->addAction(toggleJoinByNameAct_);
		viewMenu->addAction(toggleShowOfflineAct_);
	}

		
	void MainWindow::buildTestRoster() {/*
		RosterDataService* rosterService = new PsiDataService();
		rb_->registerAccount("gislan@utumno.pl", rosterService);

		RosterDataService* rosterService2 = new PsiDataService();
		rb_->registerAccount("gislan@jabster.pl", rosterService2);
		rb_->rebuild();*/
	}

	RosterBuilder* MainWindow::getRosterBuilder() {
		return rb_;
	}

}

