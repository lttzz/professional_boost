#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////
#include <map>

// auto只能用于赋值语句的类型推导，且总是推导出值类型（非引用）
// auto&&总是推导出引用类型
void case1() {
    // long x = 0L;
    // const char* s = "zelda";
    auto x = 0L;        // 简单变量
    auto s = "zelda";

    map<string, string> m;  
    // map<string, string>::iterator iter = m.begin();
    auto iter = m.begin();  // 函数迭代器

    auto f = bind1st(std::less<int>(), 2);    // 函数绑定器

    boost::ignore_unused(x, s, iter, f);
}

///////////////////////////////////////

namespace temp {

// C++14里面auto又扩充了能力，可以用在函数的返回值声明，自动推导返回值类型
auto func(int x) -> decltype(x) {
    return x * x;
}
}

void case2() {
    int x = 0;
    const long y = 100;
    volatile string s("one punch");

    auto a1 = ++x;
    auto& a2 = x;
    auto a3 = y * y;
    auto& a4 = y;
    auto a5 = std::move(y);
    auto&& a6 = std::move(y);
    const auto a7 = x + x;
    auto* a8 = &y;
    auto&& a9 = s;

    static_assert(is_same<decltype(a1), int>::value, "");
    static_assert(is_same<decltype(a2), int&>::value, "");
    static_assert(is_same<decltype(a3), long>::value, "");
    static_assert(is_same<decltype(a4), const long&>::value, "");
    static_assert(is_same<decltype(a5), long>::value, "");
    static_assert(is_same<decltype(a6), const long&&>::value, "");
    static_assert(is_same<decltype(a7), const int>::value, "");
    static_assert(is_same<decltype(a8), const long*>::value, "");
    static_assert(is_same<decltype(a9), volatile string&>::value, "");

    using namespace temp;
    static_assert(is_same<decltype(func(3)), int>::value, "");

    auto&& a11  = y;
    static_assert(is_same<decltype(a11), const long&>::value, "");
}

///////////////////////////////////////

// decltype在技术上和用法上和sizeof关键字非常相似，都需要编译器在编译期计算类型
// 但是sizeof返回的是整数，decltype返回的是类型
void case3() {
    int x = 0;
    const long y = 100;

    decltype(x) d1 = x;
    decltype(x)& d2 = x;
    decltype(&x) d3 = &x;
    decltype(x+y) d4 = x + y;
    decltype(y)& d5 = y;

    static_assert(is_same<decltype(d1), int>::value, "");
    static_assert(is_same<decltype(d2), int&>::value, "");
    static_assert(is_same<decltype(d3), int*>::value, "");
    static_assert(is_same<decltype(d4), long>::value, "");
    static_assert(is_same<decltype(d5), const long&>::value, "");
}

///////////////////////////////////////
#include <vector>

// 声明一个函数对象
decltype(std::less<int>()) functor;

// 用于返回值和参数声明
decltype(0.0F) func(decltype(0L) x) {
    return x + x;
}

typedef decltype(func)* func_ptr;

template <typename T>
class demo {};

void case4() {
    vector<int> v;

    decltype(v)::iterator iter;
    demo<decltype(v)> obj;

    boost::ignore_unused(v, iter, obj);
}

///////////////////////////////////////

// decltype(e)的形式获得表达式计算结果的值类型
// decltype((e))的形式获得表达式计算结果的引用类型，类似auto&&的效果
void case5() {
    int x = 0;
    const volatile int y = 0;

    decltype(x) d1 = x;
    decltype((x)) d2 = x;
    decltype(y) d3 = y;
    decltype((y)) d4 = y;

    static_assert(is_same<decltype(d1), int>::value, "");
    static_assert(is_same<decltype(d2), int&>::value, "");
    static_assert(is_same<decltype(d3), const volatile int>::value, "");
    static_assert(is_same<decltype(d4), const volatile int&>::value, "");

    // decltype((e))可以完整地得到真正的引用类型
    struct demo {
        int x = 0;
    };

    volatile auto* p = new demo;

    decltype(p->x) d5 = 42;
    decltype((p->x)) d6 = p->x;

    static_assert(is_same<decltype(d5), int>::value, "");
    static_assert(is_same<decltype(d6), volatile int&>::value, "");
}

///////////////////////////////////////

// C++11/14新增了一种新的函数语法，允许返回值类型后置
// 它使用了auto/decltype的类型推导能力
// 返回值处必须使用auto占位，这时的auto并不具有类型推导能力
// 其次，函数名后使用"-> type"的形式声明真正的返回值类型，这里的type可以是任何类型，也包括decltype
// 虽然看起来古怪，但有时不得不如此做
// 比如泛型编程时，返回值的类型需要由实际的参数决定，所以有必要将返回值类型的声明延后，比如下面这种情况
// 又如lambda表示式中
template <typename T, typename U>
auto calc(T t, U u) -> decltype(t + u) {
    return t + u;
}

void case6() {
    auto v = calc(1L, 0.618);

    static_assert(is_same<decltype(v), double>::value, "");
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 auto" << std::endl;

    case1();
    case2();
    case3();
    case4();
    case5();
    case6();

    return 0;
}