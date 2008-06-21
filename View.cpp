#include <QMenu>
#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QHeaderView>

#include "View.h"
#include "Model.h"
#include "Group.h"
#include "Contact.h"
#include "Roster.h"

namespace Roster {

	/* Look & feel, initializations, connects */
	View::View() {
		setContextMenuPolicy(Qt::CustomContextMenu);
		setDragEnabled(true);
		setAcceptDrops(true);
		setSelectionMode(ExtendedSelection);
		header()->hide();
//		setHeaderHidden(true); // QT 4.4 required

		setStyleSheet("QTreeView::branch { image: none; width: 0px }"); // FIXME: externalize this
		setIndentation(2);

		/* context menu */
		sendMessageAct_ = new QAction(QIcon("icons/send.png"), tr("Send &message"), this);
		connect(sendMessageAct_, SIGNAL(triggered()), this, SLOT(menuSendMessage()));
		historyAct_ = new QAction(QIcon("icons/history.png"), tr("&History"), this);
		connect(historyAct_, SIGNAL(triggered()), this, SLOT(menuHistory()));
		sendMessageToGroupAct_ = new QAction(QIcon("icons/send.png"), tr("Send message to group"), this);
		connect(sendMessageToGroupAct_, SIGNAL(triggered()), this, SLOT(menuSendMessageToGroup()));
		renameGroupAct_ = new QAction(tr("Re&name"), this);
		connect(renameGroupAct_, SIGNAL(triggered()), this, SLOT(menuRenameGroup()));
		goOnlineAct_ = new QAction(tr("Online"), this);
		connect(goOnlineAct_, SIGNAL(triggered()), this, SLOT(menuGoOnline()));
		goOfflineAct_ = new QAction(tr("Offline"), this);
		connect(goOfflineAct_, SIGNAL(triggered()), this, SLOT(menuGoOffline()));
		xmlConsoleAct_ = new QAction(tr("&XML Console"), this);
		connect(xmlConsoleAct_, SIGNAL(triggered()), this, SLOT(menuXmlConsole()));

		/* view signals */
		connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(showContextMenu(const QPoint&)));
		connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(doActivated(const QModelIndex&)));
	}

	/* build and display context menu */
	void View::showContextMenu(const QPoint& position) {
		QModelIndex index = indexAt(position);
		if ( ! index.isValid() ) {
			return;
		}

		QMenu* menu = new QMenu();

		Q_ASSERT(index.data(Qt::UserRole).canConvert<Item*>());
		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( dynamic_cast<Group*>(item) ) { 
			Group* group = dynamic_cast<Group*>(item);
			qDebug() << "Context menu opened for group" << group->getName();

			sendMessageToGroupAct_->setData(QVariant::fromValue<Item*>(item));
			menu->addAction(sendMessageToGroupAct_);
			renameGroupAct_->setData(QVariant::fromValue<Item*>(item));
			menu->addAction(renameGroupAct_);
		} else if ( dynamic_cast<Contact*>(item) ) { 
			Contact* contact = dynamic_cast<Contact*>(item);
			qDebug() << "Context menu opened for contact" << contact->getName();

			sendMessageAct_->setData(QVariant::fromValue<Item*>(item));
			menu->addAction(sendMessageAct_);
			historyAct_->setData(QVariant::fromValue<Item*>(item));
			menu->addAction(historyAct_);
		} else if ( dynamic_cast<Roster*>(item) ) {
			Roster* roster = dynamic_cast<Roster*>(item);
			qDebug() << "Context menu opened for roster" << roster->getName();

			QMenu* statusMenu = new QMenu(tr("&Status"));
			goOnlineAct_->setData(QVariant::fromValue<Item*>(item));
			statusMenu->addAction(goOnlineAct_);
			goOfflineAct_->setData(QVariant::fromValue<Item*>(item));
			statusMenu->addAction(goOfflineAct_);
			menu->addMenu(statusMenu);

			xmlConsoleAct_->setData(QVariant::fromValue<Item*>(item));
			menu->addAction(xmlConsoleAct_);
			
		}

		menu->popup( this->mapToGlobal(position) );
	}

	/* trigger default action when user clicks on roster item */
	void View::doActivated(const QModelIndex& index) {
		Q_ASSERT(index.data(Qt::UserRole).canConvert<Item*>());
		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( dynamic_cast<Group*>(item) ) {
			Group* group = dynamic_cast<Group*>(item);
			qDebug() << "Default action triggered on group" << group->getName();
		} else if ( dynamic_cast<Contact*>(item) ) {
			Contact* contact = dynamic_cast<Contact*>(item);
			qDebug() << "Default action triggered on contact" << contact->getName();
		} else if ( dynamic_cast<Roster*>(item) ) {
			Roster* roster = dynamic_cast<Roster*>(item);
			qDebug() << "Default action triggered on roster" << roster->getName();
		}
	}

	/* menu action for (contact)->send message */
	void View::menuSendMessage() {
		QAction* action = static_cast<QAction*>(sender());
		Contact* contact = static_cast<Contact*>(action->data().value<Item*>());
		qDebug() << "send message to" << contact->getName();
	}

	/* menu action for (contact)->history */
	void View::menuHistory() {
		QAction* action = static_cast<QAction*>(sender());
		Contact* contact = static_cast<Contact*>(action->data().value<Item*>());
		qDebug() << "history for" << contact->getName();
	}

	/* menu action for (group)->send message to group */
	void View::menuSendMessageToGroup() {
		QAction* action = static_cast<QAction*>(sender());
		Group* group = static_cast<Group*>(action->data().value<Item*>());
		qDebug() << "send message to group" << group->getName();
	}

	/* menu action for (group)->rename */
	void View::menuRenameGroup() {
		QAction* action = static_cast<QAction*>(sender());
		Group* group = static_cast<Group*>(action->data().value<Item*>());
		qDebug() << "rename group" << group->getName();
	}

	/* menu action for (roster)->status->online */
	void View::menuGoOnline() {
		QAction* action = static_cast<QAction*>(sender());
		Roster* roster = static_cast<Roster*>(action->data().value<Item*>());
		qDebug() << "go online on roster" << roster->getName();
	}

	/* menu action for (roster)->status->offline */
	void View::menuGoOffline() {
		QAction* action = static_cast<QAction*>(sender());
		Roster* roster = static_cast<Roster*>(action->data().value<Item*>());
		qDebug() << "go offline on roster" << roster->getName();
	}

	/* menu action for (roster)->xml console */
	void View::menuXmlConsole() {
		QAction* action = static_cast<QAction*>(sender());
		Roster* roster = static_cast<Roster*>(action->data().value<Item*>());
		qDebug() << "xml console on roster" << roster->getName();
	}
}

