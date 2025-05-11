#ifndef UTIL_H    // 防止头文件被重复包含的宏定义开始
#define UTIL_H    // 如果 UTIL_H 未定义，则定义它

// 错误处理函数声明
// 参数1: bool类型，表示是否发生错误的条件
// 参数2: const char*类型，表示错误信息字符串
// 功能: 当第一个参数为true时，打印错误信息并终止程序
void errif(bool condition, const char* errmsg);

#endif           // 防止头文件被重复包含的宏定义结束