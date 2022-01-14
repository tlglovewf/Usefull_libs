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

    //内角点列行数量
	constexpr int image_count = 10;	//20
	const  cv::Size square_size(30,30); //单位mm 单元格实际尺寸
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
		std::cout << "角点提取完毕." << std::endl;
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

		cv::Mat cameraMatrix;			//相机内参
		cv::Mat descript;				//畸变参数
		std::vector<cv::Mat> rvecsMat;	//所有图像旋转向量
		std::vector<cv::Mat> tvecsMat;	//所有图像平移向量
		double err_first = cv::calibrateCamera(object_points_seq, image_points_seq, image_size, cameraMatrix, descript, rvecsMat, tvecsMat, cv::CALIB_FIX_K3);
		std::cout << "重投影误差 : " << err_first << "像素" << std::endl;
		std::cout << "相机内参:\n" << cameraMatrix << endl;
		std::cout << "畸变参数:\n" << descript << endl;
		std::cout << "标定完成." << std::endl;



		std::cout << "开始评价-->" << std::endl;

		double total_err = 0.0; //所有图像的平均误差的总和
		double err = 0.0;		//每一幅图像平均误差
		double totalErr = 0.0;
		double totalPoints = 0.0;
		PtVec image_points_pro;

		for (int i = 0; i < image_count; ++i)
		{
			cv::projectPoints(object_points_seq[i], rvecsMat[i], tvecsMat[i], cameraMatrix, descript, image_points_pro);

			err = cv::norm(cv::Mat(image_points_seq[i]), cv::Mat(image_points_pro), cv::NORM_L2);//重投误差

			totalErr += err * err;

			totalPoints += object_points_seq[i].size();

			err /= object_points_seq[i].size();

			total_err += err;
		}

		std::cout << "重投影误差2:" << sqrt(totalErr / totalPoints) << "像素" << std::endl;
		std::cout << "重投影误差3:" << total_err / image_count << "像素" << std::endl;
	}
	catch (cv::Exception e)
	{
		std::cout << e.what() << std::endl;
	}


	return 0;
}