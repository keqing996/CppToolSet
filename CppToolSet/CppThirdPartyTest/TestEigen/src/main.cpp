
#include <iostream>
#include <format>
#include <eigen/Eigen>


int main()
{
    Eigen::MatrixXf m1 = Eigen::MatrixXf::Identity(3, 3);

    m1(0, 2) = 5;
    std::cout << m1 << std::endl;
    std::cout << std::endl;

    Eigen::MatrixXf m2(3, 3);
    m2.row(0) = Eigen::Vector3f(1, 2, 3);
    m2.row(1) = Eigen::Vector3f(3, 7, 3);
    m2.row(2) = Eigen::Vector3f(4, 2, 9);

    std::cout << m2 << std::endl;
    std::cout << std::endl;

    std::cout << m1 * m2 << std::endl;
    std::cout << std::endl;

    std::cout << m2.transpose() << std::endl;
    std::cout << std::endl;

    std::cout << m2.determinant() << std::endl;
    std::cout << std::endl;

    std::cout << m2.inverse() << std::endl;
    std::cout << std::endl;

    Eigen::MatrixXd test = Eigen::MatrixXd::Zero(10, 10);
    std::cout << test << std::endl;

    return 0;
}