#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <Cereal/archives/binary.hpp>
#include <Cereal/archives/xml.hpp>

using namespace std;

class MyData
{
public:
	MyData() = default;
	MyData(int _a, int _b) :a(_a), b(_b) {}
	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(a, b);
	}

	friend ostream& operator << (ostream & os, const MyData & data)
	{
		os << data.a << " " << data.b << endl;
		return os;
	}

protected:
	int a = 20;
	int b = 40;
};


struct extriparams
{
	Eigen::Matrix4d  _ext;	//外参
};

struct CamParams : public extriparams
{
	double _p1 = 0.0;		//相机畸变参数
	double _p2 = 0.0;		//相机畸变参数
	double _k1 = 0.0;		//相机畸变参数
	double _k2 = 0.0;		//相机畸变参数
	double _k3 = 0.0;		//相机畸变参数
	Eigen::Matrix3d  _ins;	//内参
	Eigen::Matrix4d  _ext;	//外参
};

struct ImuParams : public extriparams
{
	
};

struct  LidarParams : public extriparams
{
};


class SensorConfig
{
public:
	using  KeyV = std::pair<std::string, std::string>;
	SensorConfig() = default;
	virtual	 ~SensorConfig() = default;
	virtual std::string name()const = 0;
	virtual void parse(const std::string& value)
	{
		auto kv = parseImpl(value);

		double x, y, z;

		sscanf(kv.second.c_str(), "%lf,%lf,%lf", &x, &y, &z);

		if (kv.first == "PXYZ")
		{
			_pos = Eigen::Vector3d(x, y, z);
		}
		else if (kv.first == "RXYZ")
		{
			std::cout << "Tran r p y to matrix" << std::endl;
		}
		else
		{
		}
		
	}

	
	virtual Eigen::Matrix3d getRotate()const { return _rot; }
	virtual Eigen::Vector3d getPos()const { return _pos; }
protected:
	virtual KeyV parseImpl(const std::string& value)
	{
		size_t ipos = value.find_first_of(':');
		std::string key = value.substr(0, ipos);
		std::string v = value.substr(ipos + 1);
		return { key,v };
	}

protected:
	Eigen::Matrix3d	_rot;
	Eigen::Vector3d	_pos;
};

class CameraConfig :public SensorConfig
{
public:
	virtual std::string name()const { return "Camera"; }

	virtual void parse(const std::string& value)
	{

	}
};
class LidarConfig : public SensorConfig
{
public:
	virtual std::string name()const { return "Lidar"; }
};
class ImuConfig : public SensorConfig
{
public:
	virtual std::string name()const { return "Imu"; }
};

#include <map>

class ConfigFile final
{
public:
	using SensorMap = std::map<std::string, SensorConfig*>;
	ConfigFile(const std::string& file)
	{

		auto cam = new CameraConfig;
		auto imu = new ImuConfig;
		auto lidar = new LidarConfig;
		_sensor[cam->name()]	= cam;
		_sensor[imu->name()]	= imu;
		_sensor[lidar->name()]	= lidar;

		std::ifstream ifile(file);
		if (ifile.is_open())
		{
			SensorMap::iterator iter = _sensor.end();
			while (!ifile.eof())
			{
				std::string line;
				std::getline(ifile, line);
				if (line[0] == '[')
				{
					std::string type = line.substr(1, line.size() - 2);
					iter = _sensor.find(type);
				}
				else
				{
					if (!line.empty())
					{
						iter->second->parse(line);
					}
				}
			}
		}
	}

	~ConfigFile()
	{
		for (auto& item : _sensor)
		{

		}
	}

protected:
	CamParams				_camparam;	//相机参数
	ImuParams				_imuparam;	//imu参数
	LidarParams				_lidarparam;//lidar参数
	SensorMap				_sensor;	//相机
};
#include <fstream>
#include <Eigen/Geometry>
#define D2R 0.0174532925388889
Eigen::Quaterniond euler2Rotation(Eigen::Vector3d  eulerAngles)
{
	double roll = D2R * eulerAngles(0);
	double pitch = D2R * eulerAngles(1);
	double yaw = D2R * eulerAngles(2);
	// Abbreviations for the various angular functions
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);

	Eigen::Quaterniond q;
	q.w() = cr * cp * cy + sr * sp * sy;
	q.x() = sr * cp * cy - cr * sp * sy;
	q.y() = cr * sp * cy + sr * cp * sy;
	q.z() = cr * cp * sy - sr * sp * cy;

	return q;
}


Eigen::Matrix4d GetTransMatrix(const Eigen::Matrix3d& rot, const Eigen::Vector3d& pos)
{
	auto t = -rot * pos;
	Eigen::Matrix4d temp;
	temp.setIdentity();
	temp.block<3, 3>(0, 0) = rot;
	temp.block<3, 1>(0, 3) = t;
	return temp;
}

class A
{
public:
	void test(int a, int b, int c)
	{
		std::cout << a << " " << b << " " << c << std::endl;
	}
};

int main(int argc, char** argv)
{

	A a;
	auto func = std::bind(&A::test, &a,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	func(10, 9, 8);

	return 0;
	//std::ifstream infile("");

	//ConfigFile cfgfile("C:\\Users\\69335\\Downloads\\L099相机内外参及说明20220405\\params.txt");
	 
	auto qua = euler2Rotation(Eigen::Vector3d(-90.0239748166667, 179.355695516667, 0.104133333333));
	
	Eigen::Vector3d  position(0.13483, -0.03166, -0.27308);
	
	auto t = -qua.toRotationMatrix() * position;
	std::cout << t << std::endl;
	std::cout << "------" << std::endl;
	Eigen::Matrix4d mat4x4 = GetTransMatrix(qua.toRotationMatrix(), position);
	

	Eigen::Vector4d te(position.x(),position.y(),position.z(), 1.0);

	std::cout << mat4x4 * te << std::endl;
	

	getchar();
	return 0;

	Eigen::Matrix4d rmat4x4;
	rmat4x4 << 1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16;

	std::cout << rmat4x4 << std::endl;
	std::cout << "-" << std::endl;
	//block 表示块大小  () 表示矩阵行列号取值
	std::cout << rmat4x4.block<3, 1>(0, 2) << std::endl;

	Eigen::Matrix2f matrix;

	matrix << 0, 1, 2, 3;
	cout << matrix << endl;
	cout << "-----" << endl;
	cout << matrix.transpose() << endl;
	cout << "-----" << endl;
	cout << matrix.inverse() << endl;

	{
		/*******/
		ofstream file("test.bin");
		cereal::BinaryOutputArchive ar(file);
		//cereal::XMLOutputArchive ar(file);
		int a = 10;
		std::string b = "test";
		int c = 15;
		MyData data(5,10);
		ar(a, cereal::binary_data(b.c_str(),10),  c, data);
	}

	{
		ifstream file("test.bin");
		cereal::BinaryInputArchive ar(file);
		//cereal::XMLInputArchive ar(file);

		int a;
		char br[10] = { 0 };

		auto b = cereal::binary_data(br, 10);
		int c;
		MyData data;
		ar(a,b, c, data);
		
		std::cout << a << " " << br << " " << c << " " << data << std::endl;
	}

	return 0;
}
