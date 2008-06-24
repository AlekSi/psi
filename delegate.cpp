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

			int height = 20;
			if ( ! contact->getAvatar().isNull() ) {
				height = 34;
			}

			/* icon first */
			QRect iconRect(option.rect.topLeft(), QSize(20, height));
			contact->getIcon().paint(painter, iconRect, Qt::AlignVCenter | Qt::AlignHCenter);

			/* then text */
			QRect textRect(iconRect.topRight(), option.rect.size() - QSize(52, 0));
			QFontMetrics fm = painter->fontMetrics();
			QString name = fm.elidedText(contact->getName(), Qt::ElideRight, textRect.width());
			painter->drawText(textRect, Qt::AlignVCenter | Qt::ElideRight, name);

			/* and avatar (if present) */
			if ( ! contact->getAvatar().isNull() and showAvatars_ ) {
				QRect avatarRect(textRect.topRight(), QSize(32, height));
				contact->getAvatar().paint(painter, avatarRect, Qt::AlignVCenter | Qt::AlignHCenter);
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
			if ( contact->getAvatar().isNull() ) {
				return QSize(1, 20);
			} else {
				return QSize(1, 34);
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
