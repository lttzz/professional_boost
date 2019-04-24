#include <iostream>
using namespace std;

#include <boost/functional/factory.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/checked_delete.hpp>

///////////////////////////////////////
#include <memory>

// facotry实现了工厂设计模式，用函数对象封装了对象的创建，消除了new关键字的随意使用，有助于改善程序的设计结构
// factory封装了new操作，基本可以相当于 new T（），可以直接创建T*指针，也就是一个“智能new”
// 使用factory要求被创建的类型T至少有一个public构造函数，否则会因为无权访问构造函数而不能完成创建工作

void case1() {
    // 基本类型指针
    auto pi = boost::factory<int*>()();
    auto ps = boost::factory<string*>()();
    auto pp = boost::factory<pair<int, double>*>()();

    boost::checked_delete(pi);
    boost::checked_delete(ps);
    boost::checked_delete(pp);

    // 智能型指针
    auto up = boost::factory<unique_ptr<int>>()();
    auto sp = boost::factory<shared_ptr<string>>()();
}

///////////////////////////////////////
#include <boost/bind.hpp>

// factory带参创建功能要求参数必须是左值类型
// 可以使用bind()不优雅地解决这一问题
void case2() {
    int a = 10, b = 20;                                     // 声明两个变量
    auto pi = boost::factory<int*>()(a);
    auto ps = boost::factory<string*>()("char* lvalue");    // 字符串是左值?
    auto pp = boost::factory<pair<int, int>*>()(a, b);

    cout << "value_factory: *ps = " << *ps << endl;

    boost::checked_delete(pi);
    boost::checked_delete(ps);
    boost::checked_delete(pp);

    // auto pi2 = boost::factory<int*>()(10);                   // error，参数是右值
    // auto pi3 = boost::factory<pair<int, int>*>()(10, 20);    // error，参数是右值

    // bind对参数类型没有限制，它内部持有的参数拷贝，可以被用作左值
    // 首先创建了一个临时factory<int*>对象，使用bind为它绑定了一个值为10的参数
    auto p = boost::bind(boost::factory<int*>(),10)();
    boost::checked_delete(p);
}

///////////////////////////////////////
#include <boost/functional/value_factory.hpp>

// value_factory是factory库提供的另一个函数对象，同样可以创建函数对象，用法与factory类似
// 但value_factory创建出来的是真正的对象实例，而不是指针
// 因为内部需要返回对象的拷贝，因此类型T必须实现拷贝构造函数
void case3() {
    auto pi = boost::value_factory<int>()();
    auto ps = boost::value_factory<string>()("hello");
    auto pp = boost::value_factory<pair<int, string>>()(pi, ps);

    auto t = boost::bind(boost::value_factory<int>(), 10);
    cout << "value_factory: ps = " << ps << endl;
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello factory" << std::endl;

    case1();
    case2();
    case3();

    return 0;
}