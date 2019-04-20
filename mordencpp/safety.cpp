#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////

// C++提出了”异常规范“的概念，可以使用throw{...}的形式来说明函数可能会抛出哪些异常
// C++11/14标准里”异常规范“被彻底废弃，只保留了一个很小的功能：声明函数不会抛出任何异常，并引入了noexcept关键字来表述这一含义
// 使用noexcept来修饰函数可以减少异常处理的额外成本，提高运行效率
void func() noexcept {}

///////////////////////////////////////
#include <cassert>

// 匿名名字空间，作用类似于使用static静态化了名字空间里的成员
namespace {             // 匿名名字空间开始
    int x = 0;
}                       // 匿名名字空间结束

// C++11/14增加了内联名字空间的概念，允许在一个名字空间声明前使用inline关键字修饰，使外部同样可以无须限定直接访问这个名字空间内部的成员
inline namespace temp { // 内联名字空间开始
    int xx = 0;
}                       // 内联名字空间结束

// 内联名字空间对于代码的版本化，可以在多个名字空间里定义不同版本的功能
// 在发布时使用inline对外只暴露一个实现，可以做到很好的隔离版本的差异
namespace release {
    namespace v001 {
        void func() { cout << "v001" << endl; }
    }
    inline namespace v002 {
        void func() { cout << "v002" << endl; }
    }
}

void case1() {
    assert(x == 0);     // 无须名字空间限定，可直接访问
    assert(xx == 0);

    release::func();    // 看不到名字空间，直接调用函数
}

///////////////////////////////////////
#include <cassert>

// 早期C++里enum是弱类型，相当于整数，而且枚举值直接暴露在外部名字空间，缺少限定，命名容易冲突
// 且枚举值可以直接当做整数进行运算，容易产生歧义
enum _color {
    red = 1, 
    green = 2,
    blue = 3
};

// C++11/14为enum增加了安全性，可以使用enum class/struct的形式声明强类型枚举
// 不仅无法隐式转换成整数，而且必须使用类型名限定访问枚举值
// 注意是逗号分隔
enum class color : char {   // 要求枚举使用char类型存储，更好的节省内存空间
    red = 1,
    green = 2,
    blue = 3
};

void case2() {
    
    auto _x = red;          // 弱类型，可以不加限定直接访问，容易引起命名冲突
    // auto _x = _color::red;
    assert(_x == 1);        // 枚举值可以直接当做整数来运算

    auto x = color::red;    // 必须使用类型名限定
    // assert(x == 1);      // 无法隐式转换
}

///////////////////////////////////////

void case3() {
    [[deprecated]] int x = 0;       // 声明变量x已经别废弃
    class [[deprecated]] demo {};   // 声明类demo已经被废弃

    cout << x << endl;              // 没啥感觉啊，就只是弹一个警告
}

///////////////////////////////////////

int main() {
    std::cout << "hello C++11/14 safety features" << std::endl;

    case1();
    case2();
    case3();

    return 0;
}