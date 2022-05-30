#include "Tool_Curl.h"
#include "curl/curl.h"
#include <QMessageBox>

#include <filesystem>

Tool_Curl* s_single = nullptr;
Tool_Curl::Tool_Curl(QWidget *parent)
    : QMainWindow(parent)
{

	s_single = this;
    ui.setupUi(this);

    connect(ui.btnStart , SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(ui.btnStop  , SIGNAL(clicked()), this, SLOT(stopClicked()));
    connect(this        , SIGNAL(finished(int)),this, SLOT(onFinished(int)));
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(onProgressValue(int)));

	ui.pgbProgress->setRange(0, 100);
	ui.pgbProgress->setValue(0);
}

void Tool_Curl::onFinished(int result)
{
	if (result != 0)
	{
		QMessageBox::warning(nullptr, "Error", QString("Error Code : %1 !").arg(result));
	}
	else
	{
		QMessageBox::information(nullptr, "Information", QString("SUCCESSFULLY!!"));
	}
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
		//curl_easy_setopt(mpHandle, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(mpHandle, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(mpHandle, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 1L);////设置重定位URL，使用自动跳转，返回的头部中有Location(一般直接请求的url没找到)，则继续请求Location对应的数据
		//curl_easy_setopt(mpHandle, CURLOPT_MAXREDIRS, 5);//查找次数，防止查找太深
		curl_easy_setopt(mpHandle, CURLOPT_CONNECTTIMEOUT, 5);//服务器未连接超时(仅在链接前起作用)

		//指定低于速率 1kb/s 60s 断开链接
		curl_easy_setopt(mpHandle, CURLOPT_LOW_SPEED_LIMIT, 1);
		curl_easy_setopt(mpHandle, CURLOPT_LOW_SPEED_TIME, 60);
		//curl_easy_setopt(mpHandle, CURLOPT_TIMEOUT, 2);  //超时时间

		//curl_easy_setopt(mpHandle, CURLOPT_POST, false);
    }
    ~FileDownload()
    {
		curl_easy_cleanup(mpHandle);
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
			curl_easy_setopt(mpHandle, CURLOPT_HEADER, 1); //获取头信息
			curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(mpHandle, CURLOPT_NOBODY, 1);
			CURLcode res = curl_easy_perform(mpHandle);
			
			if (res == CURLE_OK) {
				CURLcode resGetInfo = curl_easy_getinfo(mpHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &lensize);
				if (CURLE_OK == resGetInfo) {
					curl_easy_setopt(mpHandle, CURLOPT_HEADER, 0); //获取头信息
					curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 0);
					curl_easy_setopt(mpHandle, CURLOPT_NOBODY, 0);
					return (LONGLONG)lensize;
				}
			}
		}
		curl_easy_setopt(mpHandle, CURLOPT_HEADER, 0);
		curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 0);
		curl_easy_setopt(mpHandle, CURLOPT_NOBODY, 0);
		return -1;
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
			if(totalToDownload != 0)
			emit filedownload->valueChanged(((nowDownloaded + filedownload->locFileSize())/ (filedownload->locFileSize() + totalToDownload)) * 100.0);

			return 0;//all is good
		}
		else
		{
			return -1;
		}
	}



    void downloadFile(const char* url, std::function<void(int)> callback)
    {
        std::string strurl(url);
		s_single->setLocFileSize(0);
		mbackend = std::thread([=]() {

			std::string filename = qApp->applicationDirPath().toStdString() + strurl.substr(strurl.find_last_of('/'));

			FILE* file = nullptr;

			std::string range;
			if (!std::filesystem::exists(filename))
			{
				file = fopen(filename.c_str(), "wb");
			}
			else
			{
				auto localsize = std::filesystem::file_size(filename);
				auto netsize   = getFileSize(strurl.c_str());
				if (netsize < 0)
				{
					callback(CURLE_OPERATION_TIMEOUTED);
					return;
				}
				if (localsize == netsize)
				{ //此处仅简要判断, MD5之类的 不考虑
					printf("The file is exist!");
					if (callback)
					{
						callback(0);
					}
					return;
				}
				else if (localsize > netsize)
				{// 若超过 则重写
					file = fopen(filename.c_str(), "wb");
				}
				else
				{//不足 则要以append的形式打开
					file = fopen(filename.c_str(), "ab+");
				
					range = std::to_string(localsize) + "-";

					s_single->setLocFileSize(localsize);
					curl_easy_setopt(mpHandle, CURLOPT_RESUME_FROM_LARGE, localsize);//断点下载设置
				}
			}
			curl_easy_setopt(mpHandle, CURLOPT_URL, strurl.c_str());
			curl_easy_setopt(mpHandle, CURLOPT_WRITEFUNCTION, FileDownload::write_file_cb);// FileDownload::write_file_cb);
			curl_easy_setopt(mpHandle, CURLOPT_WRITEDATA, file);
			
			if (!range.empty())
			{
				curl_easy_setopt(mpHandle, CURLOPT_RANGE,range.c_str());
			}

			curl_easy_setopt(mpHandle, CURLOPT_NOPROGRESS,0);
			curl_easy_setopt(mpHandle, CURLOPT_PROGRESSFUNCTION, FileDownload::processfunc);
			curl_easy_setopt(mpHandle, CURLOPT_PROGRESSDATA, s_single);


			CURLcode result = curl_easy_perform(mpHandle);
			fclose(file);
			if (callback)
			{
				long http_code = 0;
				//result = curl_easy_getinfo(mpHandle, CURLINFO_RESPONSE_CODE, &http_code);
				callback(result);
			}
				
		
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
		FileDownload* filedownload = new FileDownload;
        filedownload->downloadFile(ui.leUrl->text().toStdString().c_str(), [&](int result)
            {
                emit this->finished(result);
            });
    }
	mbStop = false;
}

void Tool_Curl::stopClicked()
{
	mbStop = true;
}