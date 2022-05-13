#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fstream"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mBol(1),mDisplayIndex(-1),mStIndex(-1)
{
    ui->setupUi(this);
    mpMap = new QWebEngineView(ui->bkWidget);

    QString str = QCoreApplication::applicationDirPath();
    mpMap->load(QUrl("file:///"+ str + "/../map.html"));
    QSize sz = ui->bkWidget->size();
    mpMap->setGeometry(0,25,sz.width(),sz.height());

    this->grabKeyboard();
    connect(ui->btnLoad,SIGNAL(clicked()),this,SLOT(OpenDir()));
    connect(ui->btnClear,SIGNAL(clicked()),this,SLOT(ClearOverlays()));
    connect(ui->btnDistance,SIGNAL(clicked()),this,SLOT(DistanceToolSwitch()));
    connect(mpMap,SIGNAL(titleChanged(const QString &)),this,SLOT(GetImage(const QString &)));
}

void MainWindow::ClearOverlays()
{
    mBol = 1;
    mImageList.clear();
    mDirPath.clear();
    ui->btnLoad->setText("LoadReal");
    mpMap->page()->runJavaScript(QString("clearOverlay()"));
    mDisplayIndex = -1;
    mStIndex      = -1;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        --mDisplayIndex;
        displayImg();
        break;
    case Qt::Key_Right:
        ++mDisplayIndex;
        displayImg();
        break;
    }


    QJsonArray index_json;
    QJsonDocument index_doc;
    QByteArray index_bytarray;

    index_json.append(mDisplayIndex + 1);

    index_doc.setArray(index_json);

    index_bytarray = index_doc.toJson(QJsonDocument::Compact);

    QString indexjson(index_bytarray);

    QString cmd = QString("drawcurrentpt(\"%1\")")
            .arg(indexjson);

    mpMap->page()->runJavaScript(cmd);
}
void MainWindow::GetImage(const QString &str)
{
    bool isok;
    mDisplayIndex = str.toInt(&isok);

    if(isok)
    {
        if(mDirPath.isNull())
        {
            if(QMessageBox::Ok == QMessageBox::information(NULL,"info","choose image path",
                                                           QMessageBox::Ok | QMessageBox::Cancel))
            {
                QFileDialog *pathDialog = new QFileDialog(this);

                pathDialog->setWindowTitle("choose image path");

                pathDialog->setFileMode(QFileDialog::DirectoryOnly);

                pathDialog->setDirectory("../../../Datas");

                if((QDialog::Accepted == pathDialog->exec()))
                {
                    mDirPath = pathDialog->selectedFiles()[0];
                }
            }
            else
            {
                ;
            }
        }
        if(mImageList.empty())
        {
            QDir dir;
            dir.setPath(mDirPath);

            if(!dir.exists())
            {
                return;
            }
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList filelist = dir.entryInfoList();

            if(filelist.count() <= 0)
            {
                return;
            }

            for(int i = 0; i < filelist.count(); ++i)
            {
                QFileInfo file_info = filelist.at(i);
                QString suffix = file_info.suffix();

                if(QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0)
                {
                    QString absolte_file_path = file_info.absoluteFilePath();
                    mImageList.append(absolte_file_path);
                }
            }

            mImageList.sort();
        }

        if(!mImageList.empty())
        {
            int edindex = mImageList[0].lastIndexOf('_');
            int stindex = mImageList[0].lastIndexOf('-');

            QString str = mImageList[0].mid(stindex + 1,edindex - stindex - 1);

            mStIndex = str.toInt(&isok);

            if(isok)
            {
                mDisplayIndex = mDisplayIndex - mStIndex;
                mImgDisplay.move(this->x() + (static_cast<int>(this->width()) >> 1),this->y());
                displayImg();
            }
        }
    }
}

void MainWindow::displayImg()
{
    if(mDisplayIndex < 0 ||
       mImageList.empty())
        return;
    assert(mDisplayIndex < mImageList.count());
    QPixmap image(mImageList[mDisplayIndex]);
    mImgDisplay.setAlignment(Qt::AlignCenter);
    image.scaled(400,this->height(),Qt::KeepAspectRatio);
    mImgDisplay.setPixmap(image);
    mImgDisplay.setScaledContents(true);
    this->move(0,0);
    QString strTitle = mImageList[mDisplayIndex].right(12);

    mImgDisplay.setWindowTitle(strTitle);
    mImgDisplay.show();
}

void MainWindow::DistanceToolSwitch()
{
    mpMap->page()->runJavaScript(QString("startDistanceTool()"));
}

void MainWindow::OpenDir()
{
    QFileDialog *filedialog = new QFileDialog(this);
    filedialog->setDirectory("/media/tu/Work/Datas/TracePath");

    if(mBol > 0)
    {
        filedialog->setWindowTitle("choose real trace file.");
        ui->btnLoad->setText("LoadEs");
    }
    else
    {
        filedialog->setWindowTitle("choose estimate trace file.");
        ui->btnLoad->setText("LoadReal");
    }

    if(filedialog->exec() == QDialog::Accepted)
    {
        QString path = filedialog->selectedFiles()[0];
        filedialog->close();
        if(path.isNull())
            return;

        std::ifstream file;

        file.open(path.toUtf8());

        QJsonArray lon_json,lat_json,nm_json,bol_json,resx_json,resy_json;
        QJsonDocument lon_doc,lat_doc,nm_doc,bol_doc,resx_doc,resy_doc;
        QByteArray lon_bytarray,lat_bytarray,nm_bytarray,bol_bytarray,resx_bytarray,resy_bytarray;

        if(file.is_open())
        {
            while(!file.eof())
            {
                std::string str;
                getline(file,str);
                if(str.empty())
                    continue;

                double lat,lon;
                int fname;
                char temp[10] = {0};
                double res_x = 0;
                double res_y = 0;
                double res_z = 0;
                if(!mBol)
                {
                    sscanf(str.c_str(), "%lf,%lf,%lf,%lf,%lf,%d_%s",&lat,&lon,&res_x,&res_y,&res_z,&fname,temp);
                }
                else
                {
                    sscanf(str.c_str(), "%lf,%lf,%d_%s",&lat,&lon,&fname,temp);
                }

                lon_json.append(lon);
                lat_json.append(lat);
                nm_json.append(fname);
                resx_json.append(res_x);
                resy_json.append(res_y);
            }
            file.close();
        }


        lon_doc.setArray(lon_json);
        lat_doc.setArray(lat_json);
        nm_doc.setArray(nm_json);
        resx_doc.setArray(resx_json);
        resy_doc.setArray(resy_json);

        lon_bytarray = lon_doc.toJson(QJsonDocument::Compact);
        lat_bytarray = lat_doc.toJson(QJsonDocument::Compact);
        nm_bytarray  = nm_doc.toJson(QJsonDocument::Compact);
        resx_bytarray = resx_doc.toJson(QJsonDocument::Compact);
        resy_bytarray = resy_doc.toJson(QJsonDocument::Compact);

        QString lonjson(lon_bytarray);
        QString latjson(lat_bytarray);
        QString nmjson(nm_bytarray);
        QString resxjson(resx_bytarray);
        QString resyjson(resy_bytarray);

        bol_json.append(mBol);
        bol_doc.setArray(bol_json);
        bol_bytarray = bol_doc.toJson(QJsonDocument::Compact);
        QString boljson(bol_bytarray);

        QString cmd;

        cmd = QString("showarray(\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\")")
                .arg(lonjson).arg(latjson).arg(nmjson).arg(boljson).arg(resxjson).arg(resyjson);

        mBol = !mBol;

        mpMap->page()->runJavaScript(cmd);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
