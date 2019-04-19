#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////

// C++11/14重用了关键字default，允许程序员显示地声明类的缺省构造/析构等特殊的成员函数
// 不仅能够表明代码意图，而且可以让编辑器更好的优化代码
class default_demo {
public:
    // 显示指定构造函数和析构函数使用编辑器的缺省实现
    default_demo() = default;
    ~default_demo() = default;

    // 显示指定拷贝构造函数和拷贝赋值函数使用编辑器的缺省实现
    default_demo(const default_demo&) = default;
    default_demo& operator = (const default_demo&) = default;

    // 显示指定转移构造函数和转移赋值函数使用编辑器的缺省实现
    default_demo(default_demo&&) = default;
    default_demo& operator = (default_demo&&) = default;
public:
   // 使用default声明缺省构造函数后并不影响其他构造函数的重载与实现
    default_demo(int x) {}
};

///////////////////////////////////////

// 与default类似，C++11/14里delete也新增了一种用法，可以显示地禁用某些函数（通常是类的构造函数和拷贝构造函数），以阻止对象的拷贝
class delete_demo {
public:
    delete_demo() = default;
    ~delete_demo() = default;

    // 显示禁用拷贝构造函数和拷贝赋值函数
    delete_demo(const delete_demo&) = delete;
    delete_demo& operator = (const delete_demo&) = delete;
};

void case2() {
    delete_demo obj;
    //delete_demo obj2 = obj;   // 无法拷贝赋值，发生编译错误

    boost::ignore_unused(obj);
}

///////////////////////////////////////
#include <memory>

struct base {
    virtual ~base() = default;

    virtual void f() = 0;
    virtual void g() const {
        cout << "base g()" << endl;
    }
    void h() {
        cout << "base h()" << endl;
    }
};

struct derived1 : public base {
    virtual ~derived1() = default;
    
    // 虚函数重载
    void f() {
        cout << "derived1 f()" << endl;
    }
    // 不是虚函数重载，签名不同，是函数覆盖
    void g() {
        cout << "derived1 g()" << endl;
    }
    // 函数覆盖
    void h() {
        cout << "derived1 h()" << endl;
    }
};

// 熟悉java的应该对override并不是很陌生，此处的功能类似
// override并不是关键字，知识一个标识符，除了在成员函数后有特殊含义外，在其他地方可以当作变量名或函数名使用，但不建议这么做
struct derived2 : public base {
    virtual ~derived2() = default;

    // override修饰，明确是虚函数重载
    void f() override {
        cout << "derived2 f()" << endl;
    }
    // override修饰，明确是虚函数重载
    void g() const override {
        cout << "derived2 g()" << endl;
    }
    void h() {
        cout << "derived2 h()" << endl;
    }
};

void case3() {
    unique_ptr<base> p(new derived2);
}

void case1() {
    default_demo d;

    boost::ignore_unused(d);
}

///////////////////////////////////////

// 类名后使用final，显示地禁止类被继承，即不能再有派生类
// 在虚函数后使用final，显示地禁止该函数在子类中被重载
// final和override混用，能更好的标记类的继承体系和虚函数
// (好像也在java里见过。。。
// 此处final和override一样，也不是关键字，只是一个标识符
struct interface {
    virtual ~interface() = default;

    virtual void f() = 0;
    virtual void g() = 0;
};

struct abstruct : public interface {
    virtual ~abstruct() = default;

    void f() override final {}
    void g() override {};
};

struct last final : public abstruct {   // last类不能再被继承
    virtual ~last() = default;

    // void f() override {}     // 此处f()不能再被重载，发生错误
    void g() override {}
};

///////////////////////////////////////
#include <vector>
#include <cassert>

// C++11/14放宽了对类成员变量初始化的要求，允许类在声明时使用赋值或者花括号的方式直接初始化，无须在构造函数里面特别指定
// 但是这种方法对静态变量不适用(因为需要分配实际且唯一的存储空间)
// 另外这种赋值初始化的形式也不能使用auto来推导变量类型
void case4() {
    class demo {
    public:
        int x = 0;
        string s = "hello";
        vector<int> v{1, 2, 3};
    };

    demo d;

    assert(d.x == 0);
    assert(d.s == "hello");
    assert(d.v.size() == 3);
}

///////////////////////////////////////

// C++11/14引入了委托构造函数的概念，可以直接调用本类的其他构造函数，把对象的构造工作委托给其他构造函数来完成
// 使用委托构造函数
class demo {
private:
    int x, y;
public:
    demo() : demo(0, 0) {}          // 缺省构造函数，委托给双参数的构造函数

    demo(int a) : demo(a, 0) {}     // 单参数构造函数，委托给双参数的构造函数

    demo(int a, int b) { x = a; y = b; }    //  双参数的构造函数，别其他构造函数调用
};

// 未使用委托构造函数
class _demo {
private:
    int x, y;
    void init(int a, int b) { x = a; y = b; }
public:
    _demo() { init(0, 0); }
    _demo(int a) { init(a, 0); }
    _demo(int a, int b) { init(a, b); }
};

void case5() {

}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 class" << std::endl;

    case1();
    case2();
    case3();
    case4();
    case5();

    return 0;
}