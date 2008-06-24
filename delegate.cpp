#include <QModelIndex>
#include <QPainter>
#include <QDebug>
#include <QFontMetrics>
#include <QStyledItemDelegate>

#include "delegate.h"
#include "item.h"
#include "contact.h"
#include "roster.h"
#include "group.h"

/* FILE TODO:
 * + add more magic numbers
 * + get colors from options file
 */

namespace Roster {
	void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( dynamic_cast<Contact*>(item) ) {
			Contact* contact = dynamic_cast<Contact*>(item);

			painter->save();

			if ( option.state & QStyle::State_Selected ) {
				painter->fillRect(option.rect, option.palette.highlight());
				painter->setPen(option.palette.highlightedText().color());
			}

			QRect rect(option.rect);

			/* icon */
			QRect iconRect(QPoint(rect.left(), rect.top()), QSize(20, rect.height()));
			contact->getIcon().paint(painter, iconRect, Qt::AlignVCenter | Qt::AlignHCenter);
			rect.adjust(20, 0, 0, 0);

			/* avatar (if present) */
			if ( ! contact->getAvatar().isNull() and showAvatars_ ) {
				QRect avatarRect(QPoint(rect.right()-32, rect.top()), QSize(32, 34));
				contact->getAvatar().paint(painter, avatarRect, Qt::AlignVCenter | Qt::AlignHCenter);
				rect.adjust(0, 0, -32, 0);
			}

			/* name */
			QRect textRect(rect);
			if ( showStatus_ and ! contact->getStatus().isEmpty() ) {
				textRect.setHeight(16);
				rect.adjust(0, 16, 0, 0);
			}

			QFontMetrics fm = painter->fontMetrics();
			QString name = fm.elidedText(contact->getName(), Qt::ElideRight, textRect.width());
			painter->drawText(textRect, Qt::AlignVCenter, name);

			/* status message */
			if ( showStatus_ and ! contact->getStatus().isEmpty() ) {
				QFont statusFont;
				statusFont.setItalic(true);
				statusFont.setPointSize(8);

				QFontMetrics statusFm(statusFont);
				QRect statusRect(rect);
				QString status = statusFm.elidedText(contact->getStatus(), Qt::ElideRight, statusRect.width());
				painter->setFont(statusFont);
				painter->drawText(statusRect, Qt::AlignVCenter, status);
			}

			painter->restore();
		} else {
			/* just fall back to Qt's default delegate if there's no need for special painting */
			QStyledItemDelegate::paint(painter, option, index);
		}
	}

	QSize Delegate::sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const {
		Q_UNUSED(option);

		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( dynamic_cast<Contact*>(item) ) {
			Contact* contact = dynamic_cast<Contact*>(item);
			if ( (showAvatars_ and ! contact->getAvatar().isNull()) or (showStatus_ and ! contact->getStatus().isEmpty()) ) {
				return QSize(1, 34);
			} else {
				return QSize(1, 20);
			}
		} else {
			return QSize(1, 20);
		}
	}

	void Delegate::setShowAvatars(bool showAvatars) {
		showAvatars_ = showAvatars;
	}

	void Delegate::setShowStatus(bool showStatus) {
		showStatus_ = showStatus;
	}
}

