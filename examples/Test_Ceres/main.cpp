#include <iostream>
#include <ceres/ceres.h>
using namespace std;

struct CostFunctor
{
	template<typename T>
	bool operator()(const T* const x,const T* const y, T* residual)const {
		//观测值 - 估算值(误差项)
		residual[0] = 10.5 - x[0] - y[0];
		return true;
	}
};



int main(int argc, char** argv)
{
	double initial_x = 0.2;
	double initial_y = 0.5;
	double x = initial_x;
	double y = initial_y;

	ceres::Problem problem;

	ceres::CostFunction* cost_function = new ceres::AutoDiffCostFunction<CostFunctor, 1,1, 1>(new CostFunctor);

	problem.AddResidualBlock(cost_function, NULL, &x, &y);

	ceres::Solver::Options options;

	options.linear_solver_type = ceres::DENSE_QR;

	options.minimizer_progress_to_stdout = true;

	ceres::Solver::Summary summary;

	ceres::Solve(options, &problem, &summary);

	cout << "x : " << initial_x << " -> " << x << endl;
	cout << "y : " << initial_y << " -> " << y << endl;
	getchar();
	return 0;
}