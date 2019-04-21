#include <iostream>
using namespace std;

///////////////////////////////////////

// template meta-programming
// metaprogram:a program about a program
// 模板元编程是一种函数式编程，已被证实是图灵完备，也就是有足够的计算能力，可以“计算”任何东西
// 模板元编程的运行是在编译期，它把编译器当成了元程序的解释器

///////////////////////////////////////
// for C++98
#include <boost/config.hpp>

// 元函数是一个形式上很像函数的模板类，用于计算（推导）类型
template <int N, int M>
struct meta_func {
    static constexpr int value = N + M;

    // for C++98
    // BOOST_STATIC_CONSTANT(int, value = N + M);
};

template <typename N, typename M>
int func(N n, M m) {
    return n + m;
};

///////////////////////////////////////
#include "mp_utils.hpp"

mp_arglist<int L, int R>        // 这儿参数类型用了int，那么也不难理解下面的typename了
mp_function static_min {
    static constexpr int value = (L < R) ? L : R;
};

///////////////////////////////////////
#include <boost/type_traits.hpp>

// 操作类型的元函数
// 元数据是指针类型返回const T，否则返回const T*
// 因为元编程中不能使用if-else，所以只能进行模板特化
mp_arglist<mp_arg T>            //单参元函数
mp_function  demo_func {
    mp_return(const T*);
};

mp_arglist<mp_arg T>
mp_function demo_func<T*> {     // 对T*进行模板特化
    mp_return(const T);
};

///////////////////////////////////////
#include <cassert>

int main(void) {
    cout << "hello meta-programing" << endl;

    // 元函数meta_func的计算在编译的时候就已经完成了（即模板实例化）
    // meta_func::value实际上是一个编译期的常量，程序运行时不会有任何计算动作而是直接使用结果
    cout << meta_func<10, 10>::value << endl;

    // int i = 10, j = 10;  // 因为元函数的计算发生在编译期，故以此行声明变量元函数无法运行
    constexpr int i = 10, j = 10;               // 定义编译期常量，奇怪的是const前面说了是运行时不可修改的变量，emmm，那倒也说得通。
    // const int i = 10, j = 10;                // ok
    cout << meta_func<i, j>::value << endl;

    assert((static_min<10, 20>::value == 10));  // 编译期比较两个常数

    assert((is_same<mp_exec(demo_func<int>), const int*>::value));
    assert((is_same<mp_exec(demo_func<int*>), const int>::value));

    return 0;
}