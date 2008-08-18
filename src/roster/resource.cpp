#include <QIcon>

#include "resource.h"

namespace Roster {

	Resource::Resource(const QString& name, const int priority, const StatusType status, const QString statusMessage)
											: name_(name), priority_(priority), status_(status), statusMessage_(statusMessage) {
	}

	Resource::~Resource() {
	}

	const QString& Resource::getName() const {
		return name_;
	}

	void Resource::setName(const QString& name) {
		name_ = name;
	}

	const int Resource::getPriority() const {
		return priority_;
	}

	void Resource::setPriority(const int priority) {
		priority_ = priority;
	}

	const QString& Resource::getStatusMessage() const {
		return statusMessage_;
	}

	void Resource::setStatusMessage(const QString& statusMessage) {
		statusMessage_ = statusMessage;
	}

	const StatusType Resource::getStatus() const {
		return status_;
	}

	void Resource::setStatus(const StatusType status) {
		status_ = status;
	}

	const QString& Resource::getClientOS() const {
		return clientOS_;
	}

	const QString& Resource::getClientVersion() const {
		return clientVersion_;
	}

	const QString& Resource::getClientName() const {
		return clientName_;
	}

	const QString& Resource::getTune() const {
		return tune_;
	}

	void Resource::setTune(const QString& tune) {
		tune_ = tune;
	}

	void Resource::setClientInfo(const QString& clientOS, const QString& clientVersion, const QString& clientName) {
		clientName_ = clientName;
		clientVersion_ = clientVersion;
		clientOS_ = clientOS;
	}
}

