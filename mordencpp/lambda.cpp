#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////
#include <vector>
#include <algorithm>

// 函数式编程是与面向过程编程、面向对象编程、泛型编程并列的一种编程范式
// 它基于λ演算理论，把计算过程视为数学函数的组合计算
// 在C++11/14标准里，lambda表达式实际就是对函数对象的一种强化和扩展，可以直接就地定义”匿名“的函数对象（所谓的语法糖）
// lambda表达式的基本形式：[](params){...}，最简单的lambda表达式是：
// lambda的完整形式为[](){};[captures](params) mutable -> type {...}
// []被称为lambda表达式引出操作符
// lambda表达式的类型称为闭包，无法直接写出，通常用auto自动推导
void case1() {
    auto f1 = [](int x) {   
        return x * x;
    };          // 表达式结束需要加分号

    auto f2 = [](string s) {
        cout << "lambda: " << s << endl;
    };
    auto f3 = [](int x, int y) {
        return x + y;
    };

    cout << f1(3) << endl;  // 直接调用lambda表达式
    f2("heavy rain.");
    cout << f3(1, 5) << endl;

    vector<int> v = {1, 2, 3 ,4};
    std::for_each(v.begin(), v.end(), [](int x) {   // 直接写匿名的lambda表达式
        cout << x << " ";
    });
    cout << endl;

    std::for_each(v.begin(), v.end(), [](int& x) {  // lambda表达式使用引用类型，可以修改值
        if (x > 2) { x *= 2; }                      
    });
    for (auto&& x: v) {
        cout << x << " ";
    }
    cout << endl;

    auto f4 = [](int x) -> long {   // lambda表达式返回值类型会自动推导，同时也支持使用新的返回值后置语法
        return x * x;
    };

    boost::ignore_unused(f1, f2, f3, f4);
}

///////////////////////////////////////
#include <cassert>

// lambda表达式可以捕获其所在表达式外部作用域的变量（外部变量），故其完整形式是：[captures](params) mutable -> type {...}
void case2() {
    int x = 0, y = 0;
    //auto f1 = [](){ return x; }   // 无捕获显然出错
    auto f1 = [=](){ return x; };   // 值捕获所有外部变量（拷贝方式），可访不可改
    auto f2 = [&](){ return ++x; }; // 引用捕获所有外部变量，可访可改，但要当心变量生命周期结束导致的引用无效
    auto f3 = [x](){ return x; };   // 仅值捕获x，可访不可改
    auto f4 = [x, &y]{ y += x; };   // 仅值捕获x，引用捕获y
    auto f5 = [&, y](){ x += y; };  // 值捕获y，引用捕获y之外的所有外部变量
    auto f6 = [&](){ y += ++x; };   // 引用捕获所有外部变量

    assert(f1() == 0 && x == 0);
    assert(f2() == 1 && x == 1);
    assert(f3() == 0);
    f4(); assert(x == 1 && y == 0);
    f5(); assert(x == 1 && y == 0);
    f6(); assert(x == 2 && y == 2);
    // 变量的捕获发生在lambda表达式声明之时
    // 如果使用值捕获的方式，那么即使之后变量的值发生更改，lambda表达式也不会感知到，仍然使用最初的值（捕获声明时变量的值）
    // 如果想使用外部变量最新的值，就要用引用捕获的方式
    assert(f1() == 0 && x == 2);
}

///////////////////////////////////////

// lambda表达式还可以使用关键字mutable修饰，这为值捕获方式提供了”修改“变量的可能
// 但这只是修改内部的一份拷贝，不会影响实际的外部变量
void case3() {
    int x = 0;

    auto f = [=]() mutable { return ++x; };
    assert(f() == 1 && x == 0);
}

///////////////////////////////////////

void case4() {
    vector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;

    std::for_each(v.begin(), v.end(), [&](int x) {
        sum += x;
    });
    assert(sum == 15);

    int k = 3;
    auto c = std::count_if(v.begin(), v.end(), [=](int x) {
        return x > k;
    });
    assert(c == 2);
}

///////////////////////////////////////

// 类型转化，C++11/14标准允许把无捕获的lambda表达式转换为一个签名相同的函数指针
void case5() {
    int x = 0;

    auto f = [](){};                    // 无捕获的lambda表达式
    auto g = [&](){ ++x; };             // 有捕获的lambda表达式

    typedef void (*func)();             // 函数指针类型

    func p1 = f;                        // 隐式转换
    func p2 = [](){ cout << endl; };    // 直接赋值
    // func pp3 = g;                    // 有捕获，无法进行转换

    boost::ignore_unused(x, f, p1, p2);
}

///////////////////////////////////////

// 泛型的lambda表达式
// C++11标准里lambda表达式的函数参数必须是具体的类型，而C++14则为lambda表达式增加了泛型的功能
// 类似模板参数，但未使用template关键字和<>，而是使用auto来声明参数类型，语法不同，推导规则相同
void case6() {
    auto f = [](auto x) { return x; };

    cout << f(5) << endl;
    cout << f("lambda") << endl;
}

///////////////////////////////////////


int main(void) {
    std::cout << "hello C++11/14 lambda" << std::endl;

    case1();
    case2();
    case3();
    case4();
    case5();
    case6();

    return 0;
}