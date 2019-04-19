#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////
#include <set>
#include <complex>
#include <cassert>
#include <vector>

// 在函数里面也可以直接使用{...}作为返回值，其类型会自动推导
set<int> get_set() {
    return {2, 4, 6};
}

// C++11/14标准统一使用花括号{}初始化变量，称为列表初始化
// 实际上，花括号形式的语法会生成一个类型为std::initializer_list的对象，它定义在头文件<initializer_list>里
// 其具有类似标准容器的接口，只要类型实现对它的构造函数就可以支持列表初始化
void case1() {
    int x{};
    double y{2.718};
    complex<double> c{1, 1};
    string s{"venom snake"};

    assert(x == 0);
    
    int a[] = {1, 2, 3};
    vector<int> v = {4, 5, 6};

    auto ss = get_set();
    assert(ss.size() == 3);

    boost::ignore_unused(x, y, c, s, a, v, ss);
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 init" << std::endl;

    case1();

    return 0;
}