#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////
#include <vector>

void case1() {
    // 常规索引遍历数组
    int a[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;

    // 常规迭代器遍历容器，访问元素需要对迭代器进行解引用
    vector<int> v = {123, 456};
    for (auto iter = v.begin(); iter != v.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;
}

///////////////////////////////////////

void case2() {
    // C++11/14引入了一种便捷的for循环形式，即“基于范围的for(range-based for)”无须显示使用迭代器首尾位置，也无须解引用迭代器
    // 新式for循环支持C++內建数组和所有的标准容器，对于其他类型，只要具有begin()和end()成员函数，或者可以使用std::begin()和std::end()确定迭代器的范围就可以使用新式for循环
    int a[] = {1, 2, 3, 4};
    for (auto x: a) {
        cout << x << " ";
    }
    cout << endl;

    // 为auto添加修饰，使用auto&/auto&&避免拷贝
    for (auto&& x : a) {
        cout << x << " ";
    }
    cout << endl;

    vector<int> v = {123, 456};
    for (const auto&x: v) {
        cout << x << " ";
    }
    cout << endl;

    // 使用auto&以修改元素的值
    for (auto& x: v) {
        cout << ++x << " ";
    }
    cout << endl;

    // 新式for循环只是一种"语法糖"，本质上还是用迭代器实现的，想当于下面这样
    // 可见，迭代器的范围在循环开始就已经确定了，所以在for循环体里不能变动容器，也不能增减容器里面的元素，否则对发生未定义的错误
    auto&& _range = v;
    for(auto _begin = std::begin(_range), _end = std::end(_range); _begin != _end; ++_begin) {
        auto x = *_begin;
    }

    boost::ignore_unused(a, v);
}

///////////////////////////////////////

namespace std {

// 自定义类，添加begin()和end()成员函数
template <typename T>
auto begin(const std::pair<T, T>& p) -> decltype(p.first) {
    return p.first;
}

template <typename T>
auto end(const std::pair<T, T>& p) -> decltype(p.second) {
    return p.second;
}

}

void case3() {
    vector<int> v = {1, 2, 3, 4, 5};

    auto r = std::make_pair(v.begin(), v.begin() + 3);

    for (auto x: r) {
        cout << x << " ";
    }
    cout << endl;
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 for" << std::endl;

    case1();
    case2();
    case3();

    return 0;
}