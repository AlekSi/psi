#include <Qt>

#include "rosterinstance.h"
#include "view.h"
#include "model.h"
#include "viewstatemanager.h"
#include "manager.h"
#include "rosterbuilder.h"
#include "statusiconprovider.h"
#include "roster.h"
#include "delegate.h"
#include "psidataservice.h"

#include "psiaccount.h"

namespace Roster {

	RosterInstance::RosterInstance() {
		data_ = new Roster;

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
//		connect(view_, SIGNAL(searchInput(const QString&)), SLOT(searchInput(const QString&)));

		model_->setManager(manager_);
		view_->setManager(manager_);
		model_->setRosterBuilder(rb_);
	}

	RosterInstance::~RosterInstance() {
		delete view_;
		delete model_;
		delete rb_;
		delete statusIconProvider_;
		delete vsm_;
		delete manager_;
	}

	Model* RosterInstance::getModel() {
		return model_;
	}

	View* RosterInstance::getView() {
		return view_;
	}

	RosterBuilder* RosterInstance::getRosterBuilder() {
		return rb_;
	}

	void RosterInstance::registerAccount(PsiAccount* acc) {
		RosterDataService* srv = new PsiDataService(acc);
		rb_->registerAccount(acc->jid().full(), srv);
		rb_->rebuild();
	}	

}
