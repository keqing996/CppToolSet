
#include <iostream>
#include <format>
#include <eigen/Eigen>


int main()
{

    Eigen::MatrixXd test = Eigen::MatrixXd::Zero(10, 10);
    std::cout << test << std::endl;

    return 0;
}