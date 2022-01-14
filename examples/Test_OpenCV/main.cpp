#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>
using namespace std;

int main(int argc, char** argv)
{

    //�ڽǵ���������
	constexpr int image_count = 10;	//20
	const  cv::Size square_size(30,30); //��λmm ��Ԫ��ʵ�ʳߴ�
	const  cv::Size board_size( 6, 8);

	//const cv::Size board_size(13, 12);

	using KeyPts = std::vector<cv::KeyPoint>;
	using PtVec  = std::vector<cv::Point2f>;
	using Pt3Vec = std::vector <cv::Point3f>;

	PtVec image_points;
	std::vector< PtVec > image_points_seq;
	try
	{
		cv::Size image_size;
		for (int i = 0; i <= image_count; ++i)
		{
#if 0
			const std::string prfix = "E:/CamCalibra/Image";
			const std::string sufix = ".tif";
#else
			const std::string prfix = "E:/CamCalibra/data/Source/ResPic";
			const std::string sufix = ".jpg";
#endif 
			
			auto abspath = prfix + std::to_string(i) + sufix;
			cv::Mat imginput = cv::imread(abspath);
			

			image_size.width = imginput.cols;
			image_size.height = imginput.rows;
			bool bok = cv::findChessboardCorners(imginput, board_size, image_points , cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

			if (!bok)
			{
				std::cout << abspath << " Chess Corner Not Found." << std::endl;
			}
			else
			{
				cv::Mat view_gray;
				if (imginput.channels() > 1)
					cv::cvtColor(imginput, view_gray, cv::COLOR_RGB2GRAY);
				else
					view_gray = imginput;

				cv::cornerSubPix(view_gray, image_points, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS, 20, 0.01));

				image_points_seq.push_back(image_points);

				cv::drawChessboardCorners(imginput, board_size, image_points, true);

				cv::imshow("test", imginput);

				cv::waitKey(0);
			}
		}
		std::cout << "�ǵ���ȡ���." << std::endl;
		std::vector<Pt3Vec> object_points_seq;

		for (int t = 0; t < image_count; ++t)
		{
			Pt3Vec object_points;
			for (int i = 0; i < board_size.height; ++i)
			{
				for (int j = 0; j < board_size.width; ++j)
				{
					cv::Point3f realPoint;

					realPoint.x = i * square_size.width;
					realPoint.y = j * square_size.height;
					realPoint.z = 0;
					object_points.push_back(realPoint);
				}
			}
			object_points_seq.push_back(object_points);
		}

		cv::Mat cameraMatrix;			//����ڲ�
		cv::Mat descript;				//�������
		std::vector<cv::Mat> rvecsMat;	//����ͼ����ת����
		std::vector<cv::Mat> tvecsMat;	//����ͼ��ƽ������
		double err_first = cv::calibrateCamera(object_points_seq, image_points_seq, image_size, cameraMatrix, descript, rvecsMat, tvecsMat, cv::CALIB_FIX_K3);
		std::cout << "��ͶӰ��� : " << err_first << "����" << std::endl;
		std::cout << "����ڲ�:\n" << cameraMatrix << endl;
		std::cout << "�������:\n" << descript << endl;
		std::cout << "�궨���." << std::endl;



		std::cout << "��ʼ����-->" << std::endl;

		double total_err = 0.0; //����ͼ���ƽ�������ܺ�
		double err = 0.0;		//ÿһ��ͼ��ƽ�����
		double totalErr = 0.0;
		double totalPoints = 0.0;
		PtVec image_points_pro;

		for (int i = 0; i < image_count; ++i)
		{
			cv::projectPoints(object_points_seq[i], rvecsMat[i], tvecsMat[i], cameraMatrix, descript, image_points_pro);

			err = cv::norm(cv::Mat(image_points_seq[i]), cv::Mat(image_points_pro), cv::NORM_L2);//��Ͷ���

			totalErr += err * err;

			totalPoints += object_points_seq[i].size();

			err /= object_points_seq[i].size();

			total_err += err;
		}

		std::cout << "��ͶӰ���2:" << sqrt(totalErr / totalPoints) << "����" << std::endl;
		std::cout << "��ͶӰ���3:" << total_err / image_count << "����" << std::endl;
	}
	catch (cv::Exception e)
	{
		std::cout << e.what() << std::endl;
	}


	return 0;
}