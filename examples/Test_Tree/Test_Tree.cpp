#include "Test_Tree.h"
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTimer>
#include "NiTreeItemDelegate.h"

#include <thread>

#include <QThread>
QStandardItem* pnode = nullptr;
static QStandardItem* child = nullptr;
Test_Tree::Test_Tree(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.btnGet, SIGNAL(clicked()), this, SLOT(onClicked()));

    ui.treView->setItemDelegate(new NiTreeItemDelegate(ui.treView));
	
	QStandardItemModel* model = new QStandardItemModel();
	ui.treView->setModel(model);

	QStandardItem* item = new QStandardItem("TypeL");

	model->appendRow(item);
	
	QStandardItem* prj = new QStandardItem("111110");
	item->appendRow(prj);

	child = new QStandardItem("xxxxxx");

	child->setData(1, Qt::UserRole + 1);

	prj->appendRow(child);

	QStandardItem* pr = new QStandardItem("222220");
	item->appendRow(pr);

	auto test = new QStandardItem("yyyyyy");

	pr->appendRow(test);


	test->setData(1, Qt::UserRole + 1);
	
	pnode = test;

	ui.treView->expandAll();

	QTimer* timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	
	timer->start(1000);
	qRegisterMetaType<QVector<int> >("");
	static std::thread thd([]() {
		static int index = 0;
		
		while (index < 101)
		{
			QThread::sleep(1);
			++index;
			pnode->setData(index, Qt::DecorationRole);
		}
		});
}

void Test_Tree::onTimer()
{
	//static int index = 0;
	//pnode->setData(index++, Qt::DecorationRole);
}

#include <fstream>
#include <vector>

void Test_Tree::onClicked()
{

	int* x = new int(10);

}