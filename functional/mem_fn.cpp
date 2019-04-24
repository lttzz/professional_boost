#include <iostream>
using namespace  std;

#include <boost/mem_fn.hpp>

///////////////////////////////////////
// mem_fn就像是一个简化版的bind，只能绑定类的成员函数或成员变量
// men_fn不仅支持普通对象，也支持对象指针和智能指针，对于智能指针，会调用boost::get_pointer()获取真正的指针

class demo_class {
public:
    int x;
    demo_class() = default;
    ~demo_class() = default;

    demo_class(int a) : x(a) {}

    void print() {
        cout << x << endl;
    }

    void hello(const char* str) {
        cout << str << endl;
    }
};

///////////////////////////////////////
#include <vector>
#include <algorithm>

void case1() {
    demo_class d(10);
    boost::mem_fn(&demo_class::print)(d);               // 绑定普通对象，调用无参成员函数

    demo_class *p = &d;
    boost::mem_fn(&demo_class::hello)(p, "hello");      // 绑定对象指针，调用单参成员函数

    unique_ptr<demo_class> up(new demo_class(100));     
    boost::mem_fn(&demo_class::print)(up);              // 绑定unique_ptr

    shared_ptr<demo_class> sp(new demo_class(200));     
    boost::mem_fn(&demo_class::hello)(sp, "world");     // 绑定共享指针

    vector<demo_class> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
    for_each(v.begin(), v.end(), boost::mem_fn(&demo_class::print));    // mem_fn调用成员函数

    cout << "mem var: " << boost::mem_fn(&demo_class::x)(d) << endl;    // 访问成员变量
}

///////////////////////////////////////
#include <boost/bind.hpp>

// bind更灵活多变，而mem_fn更简单易懂
// 改写成bind，只需要增加"_1"占位符用于传递对象即可
void case2() {
    demo_class d(10);

    boost::bind(&demo_class::print, _1)(d);
    boost::bind(&demo_class::hello, _1, "hello")(&d);

    vector<demo_class> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
    for_each(v.begin(), v.end(), boost::bind(&demo_class::print, _1));    // mem_fn调用成员函数
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello mem_fn" << std::endl;

    case1();
    case2();

    return 0;
}