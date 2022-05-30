#include "Tool_Curl.h"
#include "curl/curl.h"
#include <QMessageBox>

Tool_Curl* s_single = nullptr;
Tool_Curl::Tool_Curl(QWidget *parent)
    : QMainWindow(parent)
{

	s_single = this;
    ui.setupUi(this);

    connect(ui.btnStart , SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(ui.btnStop  , SIGNAL(clicked()), this, SLOT(stopClicked()));
    connect(this        , SIGNAL(finished()),this, SLOT(onFinished()));
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(onProgressValue(int)));

	ui.pgbProgress->setRange(0, 100);
	ui.pgbProgress->setValue(0);
}

void Tool_Curl::onFinished()
{
    QMessageBox::information(nullptr, "Information", QString("SUCCESSFULLY!!"));
}

void Tool_Curl::onProgressValue(int value)
{
	ui.pgbProgress->setValue(value);
}

#include <thread>
class FileDownload
{
public:
    FileDownload()
    {
        curl_global_init(CURL_GLOBAL_ALL);
        mpHandle = curl_easy_init();
		curl_easy_setopt(mpHandle, CURLOPT_SSL_VERIFYPEER, false); // 不检查证书
		
		curl_easy_setopt(mpHandle, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(mpHandle, CURLOPT_NOSIGNAL, 1L);
		//curl_easy_setopt(mpHandle, CURLOPT_TIMEOUT, 2);  //超时时间
		curl_easy_setopt(mpHandle, CURLOPT_NOPROGRESS, 0);//打开进度相应函数, 为0时似乎要在一个线程中 否则在其他线程会崩溃
		curl_easy_setopt(mpHandle, CURLOPT_PROGRESSFUNCTION, FileDownload::processfunc);
		curl_easy_setopt(mpHandle, CURLOPT_PROGRESSDATA, s_single);
		curl_easy_setopt(mpHandle, CURLOPT_POST, false);
		
		//curl_easy_setopt(mpHandle, CURLOPT_NOPROGRESS, 1);
    }
    ~FileDownload()
    {
        curl_global_cleanup();
    }
#define _MB 1048576  // 1MB
#define MB(X)  ((X)*_MB)
	static size_t write_file_cb(char* ptr, size_t size, size_t nmemb, FILE* userdata)
	{
        static auto totalsz = 0;
		FILE* file = static_cast<FILE*>(userdata);
		size_t len = fwrite(ptr, size, nmemb, file);

        if (totalsz > _MB)
        {
            fflush(file);
            totalsz = 0;
        }
        else
        {
            totalsz += len;
        }

		return len;
	}


    LONGLONG getFileSize(const char* url)
    {
		double lensize = 0.0;
		for (int iTry = 0; iTry < 3; iTry++)//由于curl_easy_perform可能会有偶发性的CURLE_WRITE_ERROR错误，所以添加重试机制
		{
			curl_easy_setopt(mpHandle, CURLOPT_URL, url);
			curl_easy_setopt(mpHandle, CURLOPT_HEADER, 1);
			curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(mpHandle, CURLOPT_NOBODY, 1);
			CURLcode res = curl_easy_perform(mpHandle);
			
			if (res == CURLE_OK) {
				CURLcode resGetInfo = curl_easy_getinfo(mpHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &lensize);
				if (CURLE_OK == resGetInfo) {
					curl_easy_cleanup(mpHandle);
					return (LONGLONG)lensize;
				}
			}
		}
		return 0;
    }
	//curl_easy_setopt(m_pCurl, CURLOPT_POST, true); 
	//下载时不能指定为post方式，否则下载时进度函数dltotal, dlnow都为0，导致下载进度函数执行可能失败！！！
	static int processfunc(void* ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
	{
		Tool_Curl* filedownload = static_cast<Tool_Curl*>(ptr);
		if (filedownload == nullptr)
			return 0;
		if (!filedownload->isStop())
		{
			// set progress value

			emit filedownload->valueChanged(nowDownloaded / totalToDownload);

			return 0;//all is good
		}
		else
		{
			return -1;
		}
	}

    void downloadFile(const char* url, std::function<void()> callback)
    {
        std::string strurl(url);
        mbackend = std::thread([=] {
			
			std::string filename = qApp->applicationDirPath().toStdString() + strurl.substr(strurl.find_last_of('/'));

			FILE* file = fopen(filename.c_str(), "wb");


			auto filesize = getFileSize(strurl.c_str());

			if (filesize < 10)
			{
				return;
			}

			curl_easy_setopt(mpHandle, CURLOPT_WRITEFUNCTION,FileDownload::write_file_cb);// FileDownload::write_file_cb);
			curl_easy_setopt(mpHandle, CURLOPT_WRITEDATA, file);
			curl_easy_setopt(mpHandle, CURLOPT_URL, strurl.c_str());
			
			

			CURLcode result = curl_easy_perform(mpHandle);

			if (result != CURLE_OK)
			{
				//CURLE_OPERATION_TIMEDOUT 超时
				QMessageBox::information(nullptr, "Information", QString("Error Type:%1 ").arg(result));
			}

			fclose(file);
			if (callback)
				callback();
            });
    }



protected:

    CURL* mpHandle = nullptr;
    std::thread    mbackend;
};







void Tool_Curl::startClicked()
{
    if (!ui.leUrl->text().isEmpty())
    {
        static FileDownload filedownload;
        filedownload.downloadFile(ui.leUrl->text().toStdString().c_str(), [&]()
            {
                emit this->finished();
            });
    }
}

void Tool_Curl::stopClicked()
{
	mbStop = true;
}