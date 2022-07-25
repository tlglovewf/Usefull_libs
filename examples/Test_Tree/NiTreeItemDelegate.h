#pragma once

#include <QItemDelegate>
#include <QApplication>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QStyleOptionTitleBar>
#include <QStyleOptionProgressBar>
#include <QDebug>
class NiTreeItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	NiTreeItemDelegate(QObject *parent);
	~NiTreeItemDelegate();


	// 创建一个控件
	virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

	void updateEditorGeometry(QWidget* editor,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override
	{
		return;
	}

	// painting
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override
	{
		//QStyleOptionButton* btnstyle = new QStyleOptionButton();
		//btnstyle->text = index.data(Qt::DisplayRole).toString();
		//btnstyle->rect = option.rect;
		//QApplication::style()->drawControl(QStyle::CE_PushButton, btnstyle, painter);
		if ( !index.data(Qt::UserRole + 1).isNull() )
		{
			QStyleOptionProgressBar* bar = new QStyleOptionProgressBar();
			
			bar->rect = option.rect;
			bar->rect.setWidth(bar->rect.width() - 20);
			bar->minimum = 0;
			bar->maximum = 100;
			bar->textAlignment = Qt::AlignCenter;
			bar->textVisible = false;
			bar->direction = QApplication::layoutDirection();
			bar->progress = index.data(Qt::DecorationRole).toInt();
		

			QApplication::style()->drawControl(QStyle::CE_ProgressBar, bar, painter);
			QString  name = index.data(Qt::DisplayRole).toString();

			QApplication::style()->drawItemText(painter, QRect(option.rect.x(), option.rect.y(), 120, 20), 0, bar->palette, true, name);

			QPalette color(Qt::red);

			QApplication::style()->drawItemText(painter, QRect(option.rect.x() + option.rect.width() - 60, option.rect.y(), 30, 20), 0, color, true, "hello",QPalette::ColorRole::Background);
		}
		else
		{
			QItemDelegate::paint(painter, option, index);
		}
		
	}

 
};
