#include <iostream>
using namespace std;

#define BOOST_THEAD_VERSION 4
#include <boost/thread.hpp>
#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////

// 编译时需要添加如下链接命令
// g++ -std=c++1z thread.cpp -o thread.o -lpthread -lboost_thread

///////////////////////////////////////

// C++11/14标准引入了新的关键字thread_local，它实现了线程本地存储，即每个线程都拥有一个独立的，线程本地化的拷贝，线程间互不影响
// threa_local变量的生命周期比较特殊，在线程启动时构造，在线程结束时析构

static int y = 0;           // 静态变量，实体存储在本编译单元
thread_local int z = 0;     // 线程局部存储，每个线程拥有独立的实体

void case1() {
    using namespace boost;

    auto f = [&](){
        ++y; ++z;
        cout << y << " " << z << endl;
    };

    boost::thread t1(f);
    boost::thread t2(f);

    t1.join();
    t2.join();

    cout << y << " " << z << endl;  // 此时主线程里y被t1和t2两个线程修改了，但是z因为是线程安全的，线程间互不烦扰，故仍然为0
}

///////////////////////////////////////

// thread_local生命周期
class demo final {
public:
    demo() {
        cout << "demo" << endl;
    }
    ~demo() {
        cout << "~demo" << endl;
    }
};

void test_obj() {
    cout << "begin func" << endl;
    thread_local demo obj;
    cout << "end func" << endl;

    boost::ignore_unused(obj);
}

void case2() {
    using namespace boost;
    auto f = [&](){
        cout << "begin thread" << endl;
        test_obj();
        cout << "end thread" << endl;
    };

    cout << "begin case2" << endl;

    boost::thread t1(f);
    boost::thread t2(f);

    t1.join();
    t2.join();

    cout << "end case2" << endl;
}

///////////////////////////////////////

// 没怎么理解这段代码想干嘛，比较其和普通局部变量的生命周期？
class demo2 final {
public:
    demo2() {
        cout << "demo2" << endl;
    }
    ~demo2() {
        cout << "~demo2" << endl;
    }
};

demo& test_static() {
    thread_local demo obj;
    demo2 obj2;

    return obj;
}

void case3() {
    using namespace boost;

    auto f = [&]() {
        auto& obj = test_static();
        cout << "obj = " << &obj << endl;
    };

    boost::thread t1(f);

    t1.join();
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 thread" << std::endl;

    case1();
    case2();
    case3();

    return 0;
}