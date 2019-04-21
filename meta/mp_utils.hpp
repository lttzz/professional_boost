#ifndef _MP_UTILS_HPP
#define _MP_UTILS_HPP

// 自定义的伪关键字，让模板原程序更清晰直观

#define mp_arglist template         // 元函数参数列表开始
#define mp_arg typename             // 元函数的参数声明
#define mp_function struct          // 元函数的定义
#define mp_date typedef             // 元数据定义

#define mp_return(T) mp_date T type  // 元函数返回
// #define mp_return(T) using type = T; // C++11/14 风格的元函数返回
#define mp_exec(Func)   Func::type   // 获取元函数的返回结果
#define mp_eval(Func) Func::value    // 获取元函数的返回值

#endif  // _MP_UTILS_HPP