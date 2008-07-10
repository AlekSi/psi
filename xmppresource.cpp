#include "xmppresource.h"

namespace Roster {

	XMPPResource::XMPPResource(const QString& name, const int priority, const ShowType show, const QString status) : name_(name), priority_(priority), show_(show), status_(status) {
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

	const QString& XMPPResource::getStatus() const {
		return status_;
	}

	void XMPPResource::setStatus(const QString& status) {
		status_ = status;
	}

	const ShowType XMPPResource::getShow() const {
		return show_;
	}

	void XMPPResource::setShow(const ShowType show) {
		show_ = show;
	}

}

