#include "NiTreeItemDelegate.h"
#include <QProgressBar>
NiTreeItemDelegate::NiTreeItemDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

NiTreeItemDelegate::~NiTreeItemDelegate()
{
}

// ����һ���ؼ�
QWidget* NiTreeItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//QProgressBar* bar = new QProgressBar(parent);
	//bar->setRange(0, 100);
	//bar->setValue(30);
	return nullptr;
}