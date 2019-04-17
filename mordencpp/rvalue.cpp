#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////
#include <cassert>

void case1() {
    int x = 0;      // 对象实例，有名，x是左值
    int* p = &++x;  // 可以取地址，++x是左值
    assert(p);
    ++x = 10;       // 前置++返回的是左值，可以赋值
    // p = &x++;    // 后置++返回的是一个临时对象，不能取地址或者赋值，是右值，会编译错误
}

///////////////////////////////////////

void case2() {
    int x = 0;
    int& r1 = ++x;          // 左值引用
    int&& r2 = x++;         // 右值引用，引用了前自增前的临时对象xvalue
    const int& r3 = x++;    // 常量左值引用，也可以引用右值，即 const T& 是“万能引用"，可以引用任何对象，但增加了常量性
    const int&& r4 = x++;   // 常量右值引用，但如此会使其无法修改，也就无法转移，没有实际意义

    cout << r1 << " " << r2 << " " << r3 << " " << r4 << endl;

    boost::ignore_unused(r1, r2, r3, r4);
}

///////////////////////////////////////

class moveable {
private:
    int x;
public:
    moveable() {}                   // 缺省构造函数
    moveable(moveable&& other) {    // 转移构造函数
        std::swap(x, other.x);      // “窃取”右值对象的内容
    }

    moveable& operator = (moveable&& other) {   // 转移赋值函数
        std::swap(x, other.x);                  // “窃取”右值对象的内容
        return *this;
    }
public:
    static moveable create() {      // 工厂函数
        moveable obj;               // 函数栈上创建对象
        return obj;                 // 返回临时对象，即右值，会引发转移语义
    }
};

void case3() {
    moveable m1;                        // 缺省构造函数创建对象
    moveable m2(std::move(m1));         // 调用转移构造函数，m1被转移
    moveable m3 = moveable::create();   // 调用转移赋值函数

    boost::ignore_unused(m1, m2, m3);
}

///////////////////////////////////////
#include <vector>
#include <complex>
#include <map>

// C++标准库里的标砖容器（std::array除外），都实现了转移构造函数和转移赋值函数，可以利用转移语义
// 且增加了emplace系列函数，可以使用转移语义直接插入元素，进一步提高了性能
void case4() {
    vector<complex<double>> v;

    v.emplace_back(3, 4);           // 直接使用右值插入元素，无须构造再拷贝
    assert(v.size() == 1);

    for(auto it: v) cout << it << " "; cout << endl;

    map<string, string> m;

    m.emplace("professional", "boost"); // 直接使用右值插入元素，无须构造再拷贝
    assert(m.size() == 1);
    for (auto it: m) cout << it.first << " " << it.second << " "; cout << endl;
}

///////////////////////////////////////

void check(int&) {      // 参数是左值引用
    cout << "lvalue" << endl;
}

void check(int&&) {     // 参数是右值引用
    cout << "rvalue" << endl;
}

// 标准头文件<utility>里定义了一个函数std::forward()，用于在泛型编程时实现“完美转发”，可以把参数原封不动的转发给其他函数
// forward()在使用的时候必须指定模板参数，且参数必须声明为T&&，只有这种类型才能保持原状。
template <typename T>
void print(T&& v) {             // 参数是右值引用，会保持类型不变
    check(std::forward<T>(v));  // 完美转发，依据函数参数类型调用不同的函数
}

void case5() {
    int x = 10;             // x是一个左值对象

    print(x);               // 传递左值引用 
    print(std::move(x));    // 传递右值引用
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 rvalue" << std::endl;

    case1();
    case2();
    case3();
    case4();
    case5();

    return 0;
}