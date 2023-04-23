
#include "print.h"
#include <iostream>
#include "../osclib/osc.h"
#include <string>
#include <windows.h>

void print_hex_dump(const char *ptr, int len)
{
	int offset = 0;
	while (offset < len) {
		char line[100];
		memset(line, ' ', sizeof(line));
		sprintf(line, "%04X ", offset);
		for (int i = 0; i < 16; i++) {
			if (offset + i < len) {
				int c = (unsigned char)ptr[offset + i];
				sprintf(line + 5 + 3 * i, "%02X ", c);
			} else {
				strcpy(line + 5 + 3 * i, "   ");
			}
		}
		for (int i = 0; i < 16; i++) {
			if (offset + i < len) {
				int c = (unsigned char)ptr[offset + i];
				if (c < 0x20 || c > 0x7f) {
					c = '.';
				}
				line[5 + 3 * 16 + i] = c;
			} else {
				line[5 + 3 * 16 + i] = ' ';
			}
		}
		line[5 + 3 * 16 + 16] = 0;
		offset += 16;

		fprintf(stderr, "%s\n", line);
	}
}

void print_osc_value(const std::string &addr, const osc::Value &value)
{
	switch (value.type()) {
	case osc::Value::Type::Void:
		fprintf(stderr, "void\n");
		break;
	case osc::Value::Type::Bool:
		fprintf(stderr, "%s bool %d\n", addr.c_str(), (int)value.GetBoolValue());
		break;
	case osc::Value::Type::Int:
		fprintf(stderr, "%s int %d\n", addr.c_str(), (int)value.GetIntValue());
		break;
	case osc::Value::Type::Float:
		fprintf(stderr, "%s float %f\n", addr.c_str(), value.GetFloatValue());
		break;
	case osc::Value::Type::String:
		fprintf(stderr, "%s string %s\n", addr.c_str(), value.GetStringValue().c_str());
		break;
	}
}

bool IsProgramRunning(const std::wstring& processName)
{
	HWND hwnd = FindWindow(NULL, processName.c_str()); //replace "Program Title" with the title of the program window you want to check

	if(hwnd != NULL)
	{
		return true;
	}

	return false;
}
