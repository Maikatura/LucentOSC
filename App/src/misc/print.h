#ifndef PRINT_H
#define PRINT_H

#include <string>
#include "../osclib/osc.h"

void print_hex_dump(char const *ptr, int len);
void print_osc_value(std::string const &addr, osc::Value const &value);
bool IsProgramRunning(const std::wstring& processName);

#endif // PRINT_H
