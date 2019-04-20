#include <iostream>
using namespace std;

#include <boost/core/ignore_unused.hpp>

///////////////////////////////////////

#if __cplusplus < 201103L
    #error "we need the latest C++ standard!"
#endif

///////////////////////////////////////

void case1() {
    cout << "c++: " << __cplusplus << endl;
}

///////////////////////////////////////

// 至少有64位的超长整型long long
void case2() {
    static_assert(sizeof(long long) >= 8, "long long is 64bit");

    long long m = -1000 * 1000;
    unsigned long long n = 1000 * 1000;

    auto a = 524287L;           // 梅森素数，2^19-1
    auto b = 2147483647ULL;     // 梅森素数，2^31-1

    boost::ignore_unused(m, n, a, b);
}

///////////////////////////////////////
#include <cassert>

// C++11/14提供了一种新的字符串的书写方式，即R"()"格式的原始字符串
void case3() {
    string s = R"(this is a "\string\" )";
    cout << s << endl;

    // 显然写正则表达式方便多了
    string reg = R"(^\d+\s\w+)";
    string reg2 = "^\\d+\\s\\w+";
    assert(reg == reg2);

    // 原始字符串还有一种扩展，允许在圆括号两边加上最多16个字符，称为delimiter，更好的标记字符串
    auto b = R"***(BioShock Infinity)***";
    auto d = R"====(Dark Souls)====";

    cout << b << " " << d << endl;
}

///////////////////////////////////////
#include <boost/type_index.hpp>

// 常用字面值
void case4() {
    auto f = 3.14f;
    auto s = L"wide char";
    auto x = 0x100L;

    using namespace boost;

    cout << boost::typeindex::type_id<decltype(f)>().pretty_name() << endl;
    cout << boost::typeindex::type_id<decltype(s)>().pretty_name() << endl;
    cout << boost::typeindex::type_id<decltype(x)>().pretty_name() << endl;
}

///////////////////////////////////////
#include <complex>
#include <boost/xpressive/xpressive.hpp>

// 自定义字面值，需要重载“”运算符
// 用户自定义的后缀必须以“_”开头

long operator "" _kb(unsigned long long v) {    // 自定义后缀_kb
    return v * 1024;                            // 乘1024后返回
}

complex<double> operator "" _c(const char* s, size_t n) {
    using namespace boost::xpressive;

    auto reg = cregex::compile(R"--(([0-9\.]+)\+([0-9\.]+)i)--");
    cmatch what;

    auto ok = regex_match(s, what, reg);
    assert(ok);

    return complex<double>(stod(what[1]), stod(what[2]));
}

void case5() {
    auto x = 2_kb;
    assert(x == 2 * 1024);

    auto c = "1.414+1.414i"_c;
    cout << c << endl;
}
///////////////////////////////////////

// C++14标准新增了贴心的数字分位符
void case6() {
    auto x = 1'327'021'000'001;
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 features" << std::endl;

    case1();
    case2();
    case3();
    case4();
    case5();
    case6();

    return 0;
}