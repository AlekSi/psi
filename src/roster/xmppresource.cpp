#include "xmppresource.h"

namespace Roster {

	XMPPResource::XMPPResource(const QString& name, const int priority, const StatusType status, const QString statusMessage) : name_(name), priority_(priority), status_(status), statusMessage_(statusMessage) {
	}

	XMPPResource::~XMPPResource() {
	}

	const QString& XMPPResource::getName() const {
		return name_;
	}

	void XMPPResource::setName(const QString& name) {
		name_ = name;
	}

	const int XMPPResource::getPriority() const {
		return priority_;
	}

	void XMPPResource::setPriority(const int priority) {
		priority_ = priority;
	}

	const QString& XMPPResource::getStatusMessage() const {
		return statusMessage_;
	}

	void XMPPResource::setStatusMessage(const QString& statusMessage) {
		statusMessage_ = statusMessage;
	}

	const StatusType XMPPResource::getStatus() const {
		return status_;
	}

	void XMPPResource::setStatus(const StatusType status) {
		status_ = status;
	}

}

