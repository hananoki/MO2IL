#pragma once
#include <QGlobal.h>

//////////////////////////////////////////////////////////////////////////////////
class TESString {
public:
	const char* c_str();

private:
	quint32 id = -1;
	char* ptr;
};

