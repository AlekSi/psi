#include "dummydataservice.h"
#include "xmpprosteritem.h"
#include "xmppresource.h"

namespace Roster {

	DummyDataService::DummyDataService() {
		// some stuff to test
		QList<QString> generic;
		generic << "Generic";
		QList<QString> nested;
		nested << "Group #1::Group #2";
		XMPPRosterItem* gislan = new XMPPRosterItem("Gislan", "gislan@utumno.pl", generic);
		XMPPRosterItem* gislan2 = new XMPPRosterItem("Gislan", "gislan@jabster.pl", generic);
		XMPPRosterItem* remko = new XMPPRosterItem("Remko", "remko@el-tramo.be", generic);
		XMPPRosterItem* kev = new XMPPRosterItem("Kev", "kevdadrum@jabber.ex.ac.uk", nested);

		XMPPResource* home = new XMPPResource("Home", 5, STATUS_ONLINE, "Home, sweet home");
		XMPPResource* work = new XMPPResource("Work", 7, STATUS_DND, "Working on GSoC");
		XMPPResource* psi = new XMPPResource("Psi", 5, STATUS_ONLINE);

		gislan->setResource(home);
		gislan->setResource(work);
		remko->setResource(psi);

		items_ << gislan << remko << kev << gislan2;
	}

	DummyDataService::~DummyDataService() {
	}

	const QList<XMPPRosterItem*> DummyDataService::getRosterItems() const {
		return items_;
	}

	const QIcon DummyDataService::getAvatar(const QString& jid) const {
		if ( jid == "gislan@utumno.pl" ) {
			return QIcon("icons/avatar.png");
		} else if ( jid == "remko@el-tramo.be") {
			return QIcon("icons/remko.jpg");
		} else {
			return QIcon();
		}
	}

}
