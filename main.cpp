#include <QtGui>

#include "Model.h"
#include "Roster.h"
#include "RosterList.h"
#include "Delegate.h"
#include "Contact.h"
#include "View.h"
#include "Group.h"

using Roster::Contact;
using Roster::Group;
using Roster::Model;
using Roster::View;
using Roster::Delegate;
using Roster::RosterList;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

	Roster::Roster* roster = new Roster::Roster();
	Contact* c1 = new Contact("dupa", "wolowa");
	Contact* c2 = new Contact("kupa", "zupa");
	roster->addItem(c1);
	roster->addItem(c2);

	roster->setName("gislan@utumno.pl");

	Group* g1 = new Group("grupa");
	roster->addItem(g1);

	Contact* c3 = new Contact("aaa", "wolowa");
	Contact* c4 = new Contact("bbb", "zupa");
	g1->addItem(c3);
	g1->addItem(c4);

	RosterList* rl = new RosterList;
	rl->addItem(roster);
	
	View *view = new View();
//	view->setItemDelegate(new Delegate);
	Model* model = new Model(rl);
    view->setModel(model);

	view->expandAll();
	view->resizeColumnToContents(0);
    view->show();

    return app.exec();
}
