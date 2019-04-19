#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////

// 次处usig和typedef等效，但using灵活性更强
void case1() {
    using int64 = long;
    using ll = long long;

    typedef long int64;
    typedef long long ll;

    boost::ignore_unused<int64, ll>();
};

///////////////////////////////////////
#include <map>

template <typename T, typename U>
class demo final {};

template <typename T>
using demo_long = demo<T, long>;        // 第二个模板参数固定为long

template <typename T>
using int_map = std::map<int, T>;       // 为标准容器map起别名，并固定key为int

void case2() {
    int_map<string> m;                  //  使用别名，省略一个模板参数
    demo<char, int> d1;                 //  原模板类，两个模板参数
    demo_long<char> d2;                 //  模板别名，只需要给一个模板参数

    boost::ignore_unused(m, d1, d2);
}

///////////////////////////////////////

// 静态断言，是编译器的断言，与assert的使用类似
void case3() {
    static_assert(sizeof(int) == 4, "int must be 32bit");
    static_assert(sizeof(long) >= 8, "need 64bit");
}

///////////////////////////////////////

// C++里用关键字const定义常量，但是这实际上是运行时不刻修改的“变量”，而在泛型编程里面需要用到在编译器可用的常量
// 在C++11/14之前只能使用宏定义来实现，这脱离了C++编译器的掌控
// C++11/14增加了新关键字constexpr，它相当于编译器的const，但是功能更强，令所修饰的表达式或函数具有编译器的常量性，可以让编译器更好的优化代码
// constexpr让编译期的整数计算成为可能
// 在C++11里constexpr的函数用法还比较严格，仅允许函数里面又一个return语句，在C++14里面作了适当的放宽
constexpr long giga() {
    return 1000*1000*1000;
}

void case4() {
    const int k = 1024;
    constexpr int kk = 1024;

    static_assert(kk == 1024, "");

    boost::ignore_unused(k);
}

///////////////////////////////////////

// C++使用省略号“...”来声明不确定数量的参数
// 声明了可变参数模板后，还需要解开参数包才能使用，这时候不需要使用关键字typedef或class，直接在类型名后面用“..."即可得到具体的数量
template <typename K, typename V>
using x = std::map<K, V>;

template <typename ... Args>            //  声明可变参数模板
void variadic_func(Args...args) {       // 使用...解包，用于模板函数
    cout << sizeof...(Args) << endl;    // sizeof计算数量
}

template <typename ... Args>            // 声明可变参数模板
int print(const char* format, const Args& ... args) {
    return printf(format, args...);     // 解包参数并转发
    // return printf(format, std::forward<Args>(args)...);      // 这边也可以使用完美转发
}

void case5() {
    variadic_func(1, 2, 3);
    print("hello %s %s\n", "variadic", "template");
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 generic" << std::endl;

    case1();
    case2();
    case3();
    case4();
    case5();
    
    return 0;
}