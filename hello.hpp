#ifndef __HELLO_HPP
#define __HELLO_HPP

#include <iostream>

int shaderlog(const char* level, uint32_t shader);
int programlog(const char* level, uint32_t shader);
void checkErr();

#endif