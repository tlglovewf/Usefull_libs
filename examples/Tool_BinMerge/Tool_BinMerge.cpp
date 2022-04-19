#include "Tool_BinMerge.h"
#include "FileProcesser.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

#include <thread>
#include <fstream>
#include <filesystem>

#ifdef Q_OS_WIN
#pragma execution_character_set("utf-8")   //解决 VS编译器下中文乱码
#endif
Tool_BinMerge::Tool_BinMerge(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.btnSelectFiles, SIGNAL(clicked()), this, SLOT(chooseFiles()));
    connect(ui.btnSelectDir, SIGNAL(clicked()), this, SLOT(chooseFiles()));
    connect(ui.btnTrans, SIGNAL(clicked()), this, SLOT(chooseTransFile()));
    connect(ui.btnLvxTrans, SIGNAL(clicked()), this, SLOT(getLvxData()));

    connect(this, SIGNAL(handleFile(const QString&)), this, SLOT(onHandleFile(const QString&)),Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(endhandle(bool)), this, SLOT(onEndHandle(bool)));
    
    ui.lstDisplay->setModel(new QStandardItemModel);
    ui.lstDisplay->setUniformItemSizes(true); //统一列表大小 优化效率
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

void Tool_BinMerge::onHandleFile(const QString& path)
{
    ++mPgrIndex;
    QString name = path.mid(path.lastIndexOf('/'));

    printLine(QString("正在合并%1").arg(name));


    ui.lstDisplay->scrollToBottom();

    //ui.lstDisplay->scrollTo(ui.lstDisplay->model()->index(mPgrIndex - 1, 0), QAbstractItemView::PositionAtBottom);

    ui.pgbStatus->setValue(mPgrIndex);
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
   switchUiStatus(true);
   mpThread.release();
}

void Tool_BinMerge::switchUiStatus(bool status)
{
    ui.btnSelectDir->setEnabled(status);
    ui.btnSelectFiles->setEnabled(status);
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
            files = dir.entryList(filters, QDir::NoDotAndDotDot | QDir::Files );
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
                if(suffix.empty())
                    suffix =  filepath.mid(filepath.lastIndexOf('.') + 1).toStdString();
               stdfiles.emplace_back(isdir ? filepath.toStdString() : item.toStdString());
            }

            mPgrIndex = 0;
            ui.pgbStatus->setMaximum(files.size());
            ui.pgbStatus->setValue(mPgrIndex);
            ui.lblStatus->setText("正在合并文件...");
            const QString selectItem = ui.cmbList->currentText();
            
            if ( selectItem == "Pcap")
            {
                mpThread = std::make_unique<std::thread>(([=]() {
                    bool rst = FileProcesser::Pcap_Merge(stdfiles, path + "." + suffix, [&](const std::string& path)
                        {
                            emit handleFile(path.c_str());
                        });
                    emit endhandle(rst);
                    }));
            }
            else if (selectItem == "Binary")
            {
                mpThread = std::make_unique<std::thread>(([=]() {
                    bool rst = FileProcesser::Binary_Merge(stdfiles, path + "." + suffix, [&](const std::string& path)
                        {
                            emit handleFile(path.c_str());
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

       auto trans = FileProcesser::CreateTrans();

        
    }
}

#pragma pack(1)
struct GSOF_31H//full nav info

{
    char            recordtype;
    char            recordlength;
    short           gpsweek;
    unsigned long   gpstime;
    char            imualig;
    char            gnssstatus;
    double          lat;
    double          lon;
    double          alt;
    float           northvel;
    float           eastvel;
    float           downvel;
    float           speed;
    double          roll;
    double          pitch;
    double          heading;
    double          trackangle;
    float           gyro_x; //degrees / sec
    float           gyro_y;
    float           gyro_z;
    float           acc_x;  //meters / sec~2
    float           acc_y;
    float           acc_z;
};


struct GSOF_32H//RMS
{
	char            recordtype;
	char            recordlength;
	short           gpsweek;
    unsigned long   gpstime;
    char            imustatus;
    char            gnssstatus;
    float           northRMS;
	float           eastRMS;
    float           downRMS;
    float           northvel;
    float           eastvel;
    float           downvel;
    float           rollRMS;
    float           pitchRMS;
    float           headingRMS;
};


struct RecordData
{
    char stx;
    char status;
    char packettype;
    char length;
    char transnum;
    char pgindex;
    char pgmxindex;
    GSOF_31H gsofdata;
    char     checksum;
    char     etx;
};

#pragma pack(pop)


template<typename T>
T SwitchByteStorage(T t)
{
    constexpr size_t len = sizeof(T);
    char ch[len];
    char dst[len];
	memcpy(ch, &t, len);

    constexpr size_t half = len >> 1;
    for (size_t i = 0; i < half; ++i)
    {
        auto tempidx = len - 1 - i;
        ch[i]       = ch[i] ^ ch[tempidx];
        ch[tempidx] = ch[i] ^ ch[tempidx];
        ch[i]       = ch[i] ^ ch[tempidx];
    }

	T result;
	memcpy(&result, ch, len);
    return result;
}

#include <QDebug>
#include <QTime>
void Tool_BinMerge::getLvxData()
{
    QString file = QFileDialog::getOpenFileName(nullptr, "", QString(), "Date(*.dat)");
    
    if (!file.isEmpty() && std::filesystem::exists(file.toStdString()))
    {
       
        std::ifstream  datfile(file.toStdString(),std::ios::binary);
        constexpr size_t  datasize = sizeof(GSOF_31H);
        constexpr size_t  recordsize = sizeof(RecordData);
        static_assert(datasize == 106/*单条总长度*/, "the full data len is error");
        constexpr char headmark = 0x02;
       
		std::string content;
		content.assign(std::istreambuf_iterator<char>(datfile), std::istreambuf_iterator<char>());

        size_t index = 0;
        do 
        {  //去噪
            while (content[index] != headmark)
                ++index;

            RecordData item;
            memcpy((void*)(&item), (void*)(&content[index]), recordsize);
            qDebug() << (int)item.stx << " " << (int)item.etx << " " << (int)item.packettype << " " << (unsigned char)item.checksum << endl;
            double cur = fmod((SwitchByteStorage(item.gsofdata.gpstime) / 1000.0), 86400);
            qDebug() << (int)item.gsofdata.recordtype << " " << SwitchByteStorage(item.gsofdata.gpstime)<< "->" << cur << " " << SwitchByteStorage(item.gsofdata.acc_x) << endl;
            QTime time = QTime(0,0).addSecs(cur);
            qDebug() << time.toString() << endl;
            index += recordsize;
        } while (index < content.size());
       
    }
}    