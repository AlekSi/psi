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

#include "dummydataservice.h"

namespace Roster {
	MainWindow::MainWindow() {
		data_ = new Roster;
//		setupTestData();

		manager_ = new Manager;
		rb_ = new RosterBuilder(data_, manager_);

		view_ = new View;
		view_->setItemDelegate(new Delegate);
		model_ = new Model(data_);
		view_->setModel(model_);

		model_->setManager(manager_);
		view_->setManager(manager_);
		model_->setRosterBuilder(rb_);

		setCentralWidget(view_);
		setupMenus();

		buildTestRoster();

		view_->expandAll();

		// FIXME: hide resources by default ;-)
		//view_->collapse(model_->index(0, 0, model_->index(0, 0)));
		//view_->collapse(model_->match(model_->index(0, 0), Qt::DisplayRole, "Romeo", 1, Qt::MatchRecursive).at(0));

		view_->resizeColumnToContents(0);
		resize(200, 350);
	}

	void MainWindow::setupMenus() {
		toggleAvatarsAct_ = new QAction(tr("Show avatars"), this);
		toggleStatusAct_ = new QAction(tr("Show status messages"), this);
		toggleJoinedAccountsAct_ = new QAction(tr("Join all accounts"), this);
		toggleJoinByNameAct_ = new QAction(tr("Join contacts by name"), this);

		toggleAvatarsAct_->setCheckable(true);
		toggleAvatarsAct_->setChecked(true);
		toggleStatusAct_->setCheckable(true);
		toggleStatusAct_->setChecked(true);
		toggleJoinedAccountsAct_->setCheckable(true);
		toggleJoinedAccountsAct_->setChecked(true);
		toggleJoinByNameAct_->setCheckable(true);
		toggleJoinByNameAct_->setChecked(true);
		
		//connect(toggleAvatarsAct_, SIGNAL(toggled(bool)), model_, SLOT(setShowAvatars(bool)));
		connect(toggleStatusAct_, SIGNAL(toggled(bool)), model_, SLOT(setShowStatus(bool)));
		connect(toggleStatusAct_, SIGNAL(toggled(bool)), SLOT(testSlot(bool)));
		connect(toggleJoinedAccountsAct_, SIGNAL(toggled(bool)), rb_, SLOT(setJoinedAccounts(bool)));
		connect(toggleJoinByNameAct_, SIGNAL(toggled(bool)), rb_, SLOT(setJoinByName(bool)));

		QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
		viewMenu->addAction(toggleAvatarsAct_);
		viewMenu->addAction(toggleStatusAct_);
		viewMenu->addAction(toggleJoinedAccountsAct_);
		viewMenu->addAction(toggleJoinByNameAct_);
	}

	void MainWindow::testSlot(bool opt) {
		qDebug() << "test slot";
		RosterDataService* service = new DummyDataService();

		rb_->itemAdded(service->getRosterItems().at(0), "gislan@utumno.pl");
		//rb_->itemRemoved(service->getRosterItems().at(0), "gislan@utumno.pl");
	}
		
	void MainWindow::buildTestRoster() {
		RosterDataService* service = new DummyDataService();
		RosterDataService* service2 = new DummyDataService();
		rb_->addService("gislan@utumno.pl", service);
		rb_->addService("gislan@jabster.pl", service2);

/*
		XMPPRosterItem* d = new XMPPRosterItem("Romeo", "romeo@jabber.org", generic);
		XMPPRosterItem* e = new XMPPRosterItem("Juliet", "juliet@jabber.org", generic);
		XMPPRosterItem* f = new XMPPRosterItem("Hamlet", "hamlet@jabber.org", nested);

		rb_->addItem("gislan@jabster.pl", d);
		rb_->addItem("gislan@jabster.pl", e);
		rb_->addItem("gislan@jabster.pl", f);
*/
		rb_->rebuild();
		//rb_->buildJoinedAccounts(data_);
	//	rb_->buildAllAccounts(data_);
	}

	void MainWindow::setupTestData() {
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

		Account* account = new Account("gislan@utumno.pl");
		account->setIcon(QIcon("icons/online.png"));

		Metacontact* m1 = new Metacontact("GSoCers");
		m1->addItem(c1);
		m1->addItem(c2);
		account->addItem(m1);

//		account->addItem(c1);
//		account->addItem(c2);
		account->addItem(g1);

		data_->addItem(account);
	}
}

