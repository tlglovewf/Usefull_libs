#include "Tool_BinMerge.h"

#include "FileProcesser.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTimer>

#include <thread>
#include <fstream>
#include <filesystem>



#ifdef Q_OS_WIN
#pragma execution_character_set("utf-8")   //解决 VS编译器下中文乱码
#endif
Tool_BinMerge::Tool_BinMerge(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.btnSelectFiles, SIGNAL(clicked()), this, SLOT(chooseFiles()));
	connect(ui.btnSelectDir, SIGNAL(clicked()), this, SLOT(chooseFiles()));
	connect(ui.btnTrans, SIGNAL(clicked()), this, SLOT(chooseTransFile()));
	connect(ui.btnLvxTrans, SIGNAL(clicked()), this, SLOT(getLvxData()));
	connect(ui.btnPcapTrans, SIGNAL(clicked()), this, SLOT(getpcapData()));
	connect(this, SIGNAL(handleLog(const QString&)), this, SLOT(onHandleLog(const QString&)), Qt::ConnectionType::QueuedConnection);
	connect(this, SIGNAL(endhandle(bool)), this, SLOT(onEndHandle(bool)));

	QTimer::singleShot(100, ui.lstDisplay,SLOT(scrollToBottom()));


	ui.lstDisplay->setModel(new QStandardItemModel);
	ui.lstDisplay->setUniformItemSizes(true); //统一列表大小 优化效率
	ui.lstDisplay->setAutoFillBackground(true);

}

void Tool_BinMerge::closeEvent(QCloseEvent* evt)
{
	if (ui.btnSelectDir->isEnabled())
	{
		evt->accept();
	}
	else
	{
		evt->ignore();
	}
}

void Tool_BinMerge::printLine(const QString& line)
{
	dynamic_cast<QStandardItemModel*>(ui.lstDisplay->model())->appendRow(new QStandardItem(line));
}

void Tool_BinMerge::onHandleLog(const QString& log)
{
	++mPgrIndex;

	printLine(log);
	//ui.lstDisplay->scrollToBottom();
	QTimer::singleShot(0, ui.lstDisplay, SLOT(scrollToBottom()));

	//ui.lstDisplay->scrollTo(ui.lstDisplay->model()->index(mPgrIndex - 1, 0), QAbstractItemView::PositionAtBottom);

	ui.pgbStatus->setValue(mPgrIndex > ui.pgbStatus->maximum() ? ui.pgbStatus->maximum() - 1 : mPgrIndex);
}

void Tool_BinMerge::onEndHandle(bool status)
{
	QString success = "QLabel{background-color:rgb(0,255,0);}";
	QString failed = "QLabel{background-color:rgb(255,0,0);}";
	if (status)
	{
		ui.lblStatus->setText("成功!");
		ui.lblStatus->setStyleSheet(success);
	}
	else
	{
		ui.lblStatus->setText("失败!");
		ui.lblStatus->setStyleSheet(failed);
	}
	ui.pgbStatus->setValue(ui.pgbStatus->maximum());
	switchUiStatus(true);
	mpThread.release();
}


void Tool_BinMerge::switchUiStatus(bool status)
{
	ui.btnSelectDir->setEnabled(status);
	ui.btnSelectFiles->setEnabled(status);
	ui.btnLvxTrans->setEnabled(status);
	ui.btnPcapTrans->setEnabled(status);
	ui.btnTrans->setEnabled(status);
	if (!status)
	{
		mPgrIndex = 0;
		dynamic_cast<QStandardItemModel*>(ui.lstDisplay->model())->clear();
		ui.lblStatus->setText("正在运行...");
		ui.lblStatus->setStyleSheet("QLabel{background-color:rgb(200,200,200);}");
	}
}

void Tool_BinMerge::chooseFiles()
{
	QFileDialog dialog;
	dynamic_cast<QStandardItemModel*>(ui.lstDisplay->model())->clear();
	bool isdir = (sender()->objectName() == "btnSelectDir");
	dialog.setFileMode(isdir ? QFileDialog::Directory : QFileDialog::ExistingFiles);
	dialog.setFilter(QDir::Files);
	QStringList filters = { "*.pcap" };
	dialog.setNameFilters(filters);

	if (QFileDialog::Accepted == dialog.exec())
	{
		std::string suffix;
		std::string path = qApp->applicationDirPath().toStdString() + "/outfile";
		switchUiStatus(false);
		QStringList files;
		if (isdir)
		{
			auto str = dialog.selectedFiles()[0];
			QDir dir(str);
			bool bol = dir.exists();
			files = dir.entryList(filters, QDir::NoDotAndDotDot | QDir::Files);
		}
		else
		{
			files = std::move(dialog.selectedFiles());
		}

		if (!files.isEmpty())
		{
			std::vector<std::string> stdfiles;
			stdfiles.reserve(files.size());

			for (const auto& item : files)
			{
				QString filepath = dialog.selectedFiles()[0] + "/" + item;
				if (suffix.empty())
					suffix = filepath.mid(filepath.lastIndexOf('.') + 1).toStdString();
				stdfiles.emplace_back(isdir ? filepath.toStdString() : item.toStdString());
			}

			ui.pgbStatus->setMaximum(files.size());
			ui.pgbStatus->setValue(mPgrIndex);
			ui.lblStatus->setText("正在合并文件...");
			const QString selectItem = ui.cmbList->currentText();

			if (selectItem == "Pcap")
			{
				mpThread = std::make_unique<std::thread>(([=]() {
					bool rst = FileProcesser::Pcap_Merge(stdfiles, path + "." + suffix, [&](const std::string& path)
						{
							QString name = QString(path.substr(path.find_last_of('/')).c_str());

							emit handleLog(QString("正在合并%1").arg(name));
						});
					emit endhandle(rst);
					}));
			}
			else if (selectItem == "Binary")
			{
				mpThread = std::make_unique<std::thread>(([=]() {
					bool rst = FileProcesser::Binary_Merge(stdfiles, path + "." + suffix, [&](const std::string& path)
						{
							emit handleLog(path.c_str());
						});
					emit endhandle(rst);
					}));
			}
			else
			{

			}
		}
		else
		{
			switchUiStatus(true);
		}
	}

}

void Tool_BinMerge::chooseTransFile()
{
	QString file = QFileDialog::getOpenFileName(nullptr, "", QString(), "Pcap(*.pcap)");

	if (!file.isEmpty())
	{
		printLine(QString("开始转换:") + file);

		auto trans = FileProcesser::CreateTrans("Pcd");


	}
}

#include "LVXDataParse.h"
#include <QTime>
#include <QDebug>
void Tool_BinMerge::getLvxData()
{
	QString file = QFileDialog::getOpenFileName(nullptr, "", QString("E:/Datas"), "Date(*.dat)");
	switchUiStatus(false);
	mpThread = std::make_unique<std::thread>(([=]() {

		std::ofstream  imu_raw(qApp->applicationDirPath().toStdString() + "/imutrace.txt");
		
		ReadLVXRawData(file.toStdString(), [&imu_raw, this](const LvxDataSegment& data) {
			auto imudata = std::move(data.gsofdata);
			//qDebug() << "record imu data : " << endl;
			//qDebug() << fmod(imudata.gpstime / 1000.0, 86400) << " " << imudata.gyro_x << " " << imudata.gyro_y << " " << imudata.acc_x << endl;
			//qDebug() << imudata.lon << " " << imudata.lat << endl;
			std::string  line = std::to_string(imudata.lat) + "," + std::to_string(imudata.lon) + "," + std::to_string(fmod(imudata.gpstime / 1000.0, 86400)) + "\n";
			imu_raw.write(line.c_str(), line.size());
			emit handleLog(line.c_str());
			});
		emit endhandle(true);
		}));
	
}

void Tool_BinMerge::getpcapData()
{
	QString file = QFileDialog::getOpenFileName(nullptr, "", qApp->applicationDirPath(), "Lidar(*.pcap)");
	switchUiStatus(false);

	if (!file.isEmpty())
	{
		mpThread = std::make_unique<std::thread>(([=]() {
			printLine(QString("开始转换:") + file);

			auto trans = FileProcesser::CreateTrans();

			trans->trans(file.toStdString(), qApp->applicationDirPath().toStdString() + "/pcap.txt");
			emit endhandle(true);
			}));
	}
	
}