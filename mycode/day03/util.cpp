// 包含必要的头文件
// 这里包含了 C++ 标准库和自定义的头文件
#include <stdio.h>    // 提供 perror() 函数
#include <stdlib.h>   // 提供 exit() 和 EXIT_FAILURE 宏
#include "util.h"     // 包含错误处理函数的声明

/**
 * 错误处理函数的实现
 * @param condition: 错误条件，如果为true则表示发生错误
 * @param errmsg: 错误信息字符串
 * 功能：当错误发生时，打印错误信息并终止程序
 */
void errif(bool condition, const char *errmsg) {
    if (condition) {
        perror(errmsg);       // 打印错误信息，会自动附加系统错误描述
        exit(EXIT_FAILURE);    // 异常终止程序，返回错误状态码
    }
}