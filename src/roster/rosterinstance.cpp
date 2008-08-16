#include <Qt>
#include <QDebug>

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
#include "viewactionsservice.h"

#include "psiaccount.h"

namespace Roster {

	RosterInstance::RosterInstance() {
		data_ = new Roster;

		manager_ = new Manager;
		vsm_ = new ViewStateManager;
		rb_ = new RosterBuilder(data_, manager_, vsm_);

		actionsService_ = new ViewActionsService;
		statusIconProvider_ = new StatusIconProvider;
		
		view_ = new View;
		view_->setItemDelegate(new Delegate);
		model_ = new Model(data_);
		view_->setModel(model_);
		view_->setViewStateManager(vsm_);
		model_->setStatusIconProvider(statusIconProvider_);

		connect(model_, SIGNAL(expand(const QModelIndex&)), view_, SLOT(expand(const QModelIndex&)));
		connect(model_, SIGNAL(collapse(const QModelIndex&)), view_, SLOT(collapse(const QModelIndex&)));

		model_->setManager(manager_);
		view_->setManager(manager_);
		view_->setViewActionsService(actionsService_);
		model_->setViewActionsService(actionsService_);
	}

	RosterInstance::~RosterInstance() {
		delete view_;
		delete model_;
		delete rb_;
		delete statusIconProvider_;
		delete vsm_;
		delete actionsService_;
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

	void RosterInstance::accountAdded(PsiAccount* acc) {
		RosterDataService* srv = new PsiDataService(acc);
		QString acname = acc->jid().full();

		rb_->registerAccount(acname, srv);
		actionsService_->registerAccount(acname, acc);
		view_->registerAccount(acname, acc);

		rb_->rebuild();
	}

	void RosterInstance::accountRemoved(PsiAccount* acc) {
		QString acname = acc->jid().full();

		rb_->unregisterAccount(acname);
		actionsService_->unregisterAccount(acname);
		view_->unregisterAccount(acname);

		rb_->rebuild();
	}

	void RosterInstance::accountUpdated(PsiAccount* acc) {
		// FIXME: this is performance killer
		QString acname = acc->jid().full();
		rb_->unregisterAccount(acname);
		actionsService_->unregisterAccount(acname);
		view_->unregisterAccount(acname);

		accountAdded(acc);
	}

}
