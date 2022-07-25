#include "Tool_Curl.h"
#include "curl/curl.h"

#include "T_TimeInteral.h"
#include <QMessageBox>

#include <filesystem>

#include <iostream>

#include <chrono>
Tool_Curl* s_single = nullptr;

CommUtils::TimeInteral timecnt;

Tool_Curl::Tool_Curl(QWidget *parent)
    : QMainWindow(parent)
{

	s_single = this;
    ui.setupUi(this);

    connect(ui.btnStart , SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(ui.btnStop  , SIGNAL(clicked()), this, SLOT(stopClicked()));
    connect(this        , SIGNAL(finished(int)),this, SLOT(onFinished(int)));
	connect(this, SIGNAL(valueChanged(int, const QString&)), this, SLOT(onProgressValue(int, const QString&)));

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

void Tool_Curl::onProgressValue(int value,const QString& speed)
{
	ui.pgbProgress->setValue(value);
	ui.lblspeed->setText(speed);
}

static double lastdownloaded = 0.0;

#include <thread>
class FileDownload
{
public:
    FileDownload()
    {
        curl_global_init(CURL_GLOBAL_ALL);
        mpHandle = curl_easy_init();
		curl_easy_setopt(mpHandle, CURLOPT_SSL_VERIFYPEER, false); // �����֤��
		//curl_easy_setopt(mpHandle, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(mpHandle, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(mpHandle, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 1L);////�����ض�λURL��ʹ���Զ���ת�����ص�ͷ������Location(һ��ֱ�������urlû�ҵ�)�����������Location��Ӧ������
		//curl_easy_setopt(mpHandle, CURLOPT_MAXREDIRS, 5);//���Ҵ�������ֹ����̫��
		curl_easy_setopt(mpHandle, CURLOPT_CONNECTTIMEOUT, 5);//������δ���ӳ�ʱ(��������ǰ������)

		//ָ���������� 1kb/s 60s �Ͽ�����
		curl_easy_setopt(mpHandle, CURLOPT_LOW_SPEED_LIMIT, 1);
		curl_easy_setopt(mpHandle, CURLOPT_LOW_SPEED_TIME, 60);
		//curl_easy_setopt(mpHandle, CURLOPT_CUSTOMREQUEST, "GET");
		//curl_easy_setopt(mpHandle, CURLOPT_TIMEOUT, 2);  //��ʱʱ��

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

	static bool startsWith(const std::string& lhs, const std::string& rhs)
	{
		if (lhs.size() < rhs.size())
			return false;
		else
			return  lhs.substr(0, rhs.size()) == rhs;
	}

	//header һ���ַ���
	static size_t readHeader(char* header, size_t size, size_t nitems, void* userdata) 
	{
		std::string headstr(header);
		std::string valuestr = headstr.substr(headstr.find_last_of(':') + 1);
		if (startsWith(headstr, "Content-Length:") || startsWith(headstr,"content-length"))
		{
			double* data = static_cast<double*>(userdata);

			*data = atoll(valuestr.c_str());
		}
		else if (startsWith(header, "Content-disposition:")) 
		{
			// do something 
			std::cout << valuestr << std::endl;
		}
		else
		{
			;// do something
		}
			
		return size * nitems;
	}

    LONGLONG getFileSize(const char* url)
    {
		double lensize = 0.0;
		for (int iTry = 0; iTry < 3; iTry++)//����curl_easy_perform���ܻ���ż���Ե�CURLE_WRITE_ERROR��������������Ի���
		{
			curl_easy_setopt(mpHandle, CURLOPT_URL, url);
			curl_easy_setopt(mpHandle, CURLOPT_HEADER, 0); //��ȡͷ��Ϣ
			curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(mpHandle, CURLOPT_NOBODY, 1);
			
			curl_easy_setopt(mpHandle, CURLOPT_HEADERDATA, &lensize);
			curl_easy_setopt(mpHandle, CURLOPT_HEADERFUNCTION, readHeader);
			CURLcode res = curl_easy_perform(mpHandle);
			
			curl_easy_setopt(mpHandle, CURLOPT_HEADER, 0); //��ȡͷ��Ϣ
			curl_easy_setopt(mpHandle, CURLOPT_FOLLOWLOCATION, 0);
			curl_easy_setopt(mpHandle, CURLOPT_NOBODY, 0);

			if (res == CURLE_OK) {
				//CURLcode resGetInfo = curl_easy_getinfo(mpHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &lensize);
				return lensize;
			}
		}
		return -1;
    }

	

	//curl_easy_setopt(m_pCurl, CURLOPT_POST, true); 
	//����ʱ����ָ��Ϊpost��ʽ����������ʱ���Ⱥ���dltotal, dlnow��Ϊ0���������ؽ��Ⱥ���ִ�п���ʧ�ܣ�����
	static int processfunc(void* ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
	{
		Tool_Curl* filedownload = static_cast<Tool_Curl*>(ptr);
		static float costtime = 0.0;
		if (nowDownloaded == 0)
		{
			timecnt.start();
		}

		if (filedownload == nullptr)
			return 0;

		static QString speedstr;
		if (nowDownloaded > lastdownloaded)
		{
			costtime += timecnt.count(true);
			if (costtime > 1.0)
			{
				double rate = (nowDownloaded - lastdownloaded) ;
				rate /= costtime;
				std::string unit;
				if (rate < 1024)
				{
					unit = "b/s";
				}
				else if (rate < 1024 * 1024)
				{
					rate /= 1024;
					unit = "kb/s";
				}
				else if (rate < 1024 * 1024 * 1024)
				{
					rate /= (1024 * 1024);
					unit = "mb/s";
				}
				else
				{
					rate /= (1024 * 1024 * 1024);
					unit = "gb/s";
				}

				lastdownloaded = nowDownloaded;

				speedstr = QStringLiteral("%1%2").arg(rate).arg(unit.c_str());
				costtime = 0.0;

			}
		}

		

		if (!filedownload->isStop())
		{
			// set progress value
			if(totalToDownload != 0)
			emit filedownload->valueChanged(((nowDownloaded + filedownload->locFileSize())/ (filedownload->locFileSize() + totalToDownload)) * 100.0, speedstr);

			return 0;//all is good
		}
		else
		{
			return -1;
		}
	}



    void downloadFile(const char* url, std::function<void(int)> callback)
    {
		lastdownloaded = 0;
        std::string strurl(url);
		s_single->setLocFileSize(0);
		mbackend = std::thread([=]() {

			std::string filename = qApp->applicationDirPath().toStdString() + strurl.substr(strurl.find_last_of('/'));

			FILE* file = nullptr;

			std::string range;
			auto netsize = getFileSize(strurl.c_str());

			if (!std::filesystem::exists(filename))
			{
				file = fopen(filename.c_str(), "wb");
			}
			else
			{
				auto localsize = std::filesystem::file_size(filename);
				
				if (netsize < 0)
				{
					callback(CURLE_OPERATION_TIMEOUTED);
					return;
				}
				if (localsize == netsize)
				{ //�˴�����Ҫ�ж�, MD5֮��� ������
					printf("The file is exist!");
					if (callback)
					{
						callback(0);
					}
					return;
				}
				else if (localsize > netsize)
				{// ������ ����д
					file = fopen(filename.c_str(), "wb");
				}
				else
				{//���� ��Ҫ��append����ʽ��
					file = fopen(filename.c_str(), "ab+");
				
					range = std::to_string(localsize) + "-";

					s_single->setLocFileSize(localsize);
					curl_easy_setopt(mpHandle, CURLOPT_RESUME_FROM_LARGE, localsize);//�ϵ���������
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
				result = curl_easy_getinfo(mpHandle, CURLINFO_RESPONSE_CODE, &http_code);
				if (netsize < 1024)
				{
					//std::ifstream infile(filename);
					//add error judge
				}
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