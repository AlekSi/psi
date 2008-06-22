#include <QtGui>

#include "model.h"
#include "roster.h"
#include "rosterlist.h"
#include "delegate.h"
#include "contact.h"
#include "view.h"
#include "group.h"

using Roster::Contact;
using Roster::Group;
using Roster::Model;
using Roster::View;
using Roster::Delegate;
using Roster::RosterList;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

	Roster::Roster* roster = new Roster::Roster();
	Contact* c1 = new Contact("gislan@utumno.pl", "w/e");
	c1->setIcon(QIcon("icons/online.png"));
	c1->setAvatar(QIcon("icons/avatar.png"));
	Contact* c2 = new Contact("remko@el-tramo.be", "zupa");
	c2->setAvatar(QIcon("icons/remko.jpg"));
	c2->setIcon(QIcon("icons/offline.png"));
	roster->addItem(c1);
	roster->addItem(c2);
	roster->setIcon(QIcon("icons/online.png"));

	roster->setName("gislan@utumno.pl");

	Group* g1 = new Group("grupa");
	roster->addItem(g1);

	Contact* c3 = new Contact("aaa", "wolowa");
	c3->setIcon(QIcon("icons/online.png"));
	Contact* c4 = new Contact("bbb", "zupa");
	c4->setIcon(QIcon("icons/online.png"));
	g1->addItem(c3);
	g1->addItem(c4);

	RosterList* rl = new RosterList;
	rl->addItem(roster);
	
	View *view = new View();
	view->setItemDelegate(new Delegate);
	Model* model = new Model(rl);
    view->setModel(model);

	view->expandAll();
	view->resize(200, 350);
	view->resizeColumnToContents(0);
    view->show();

    return app.exec();
}
