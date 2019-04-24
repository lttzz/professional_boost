#include <iostream>
using namespace std; 

#include <boost/functional/hash.hpp>
///////////////////////////////////////
#include <complex>
#include <vector>
#include <map>
#include <array>
#include <stack>

// 各种基本数据类型和标准库容器的散列值
void case1() {
    cout << std::hex;

    cout << hash<int>()(0x2000) << endl;
    cout << hash<double>()(1.732) << endl;
    cout << hash<const char*>()("string") << endl;
    cout << hash<string>()("string") << endl;

    complex<double> c(1.0, 2.0);
    cout << boost::hash<decltype(c)>()(c) << std::endl;
    // cout << boost::hash<complex<double>>()(c) << endl;

    vector<int> v(12);
    cout << boost::hash<decltype(v)>()(v) << endl;
    // cout << boost::hash<vector<int>>()(v) << endl;

    map<int, string> m;
    cout << boost::hash<decltype(m)>()(m) << endl;

    array<int, 5> ar;
    cout << boost::hash<decltype(ar)>()(ar) << endl;

    // stack<int> s;
    // cout << boost::hash<decltype(s)>()(s) << endl;       // error，hash计算不支持的类型
}

///////////////////////////////////////
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

// hash也常用作无序容器的模板参数，作为容器计算散列值的一个策略对象
void case2() {
    boost::unordered_map<int, string, boost::hash<int>> um;
    boost::unordered_set<int, boost::hash<int>> us;
}

///////////////////////////////////////

// 计算自定义数据类型的散列值
class person final {
private:
    int id;
    string name;
    unsigned int age;
public:
    person() = default;
    ~person() = default;
    person(int a, const char* b, unsigned int c) : id(a), name(b), age(c) {}

    size_t hash_value() const {             // 自定义的散列计算函数
        size_t seed = 1984;                 // seed可以使任意整数
        boost::hash_combine(seed, id);
        boost::hash_combine(seed, name);
        boost::hash_combine(seed, age);

        return seed;
    }

    friend bool operator == (person const& l, person const& r) {    // 兼容无序容器所需
        return l.id == r.id;
    }
};

size_t hash_value(person const& p) {        // 同名字空间重载hash_value
    return p.hash_value();
}

void case3() {
    person p(1, "adam", 3);
    cout << boost::hash<person>()(p) << endl;

    boost::unordered_set<person> us;

    us.insert(person(1, "adam", 20));
    us.insert(person(1, "eva", 20));
    assert(us.size() == 2);
}

///////////////////////////////////////

class demo_class {
private:
    vector<string> v;
    int x;
public:
    size_t hash_value() const {
        size_t seed = 0;
        boost::hash_combine(seed, x);
        boost::hash_range(seed, v.rbegin(), v.rend());

        return seed;
    }
};

void case4() {
    vector<int> v{1, 2, 3, 4, 5};
    auto hv = boost::hash_range(v.begin(), v.end());

    boost::unordered_set<int> us{1, 2, 3, 4, 5};
    hv = boost::hash_range(v.begin(), v.end());

    (void)hv;
}

///////////////////////////////////////


int main(void) {
    std::cout << "hello hash" << std::endl;
    
    case1();
    case2();
    case3();
    case4();

    return 0;
}