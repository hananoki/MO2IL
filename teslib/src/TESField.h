#pragma once

#include "TESSignature.h"

//////////////////////////////////////////////////////////////////////////////////
struct TESField final {
	TESSignature sign;
	quint16 dataSize;

	TESField* next() {
		return (TESField*) ( ( (char*) this ) + sizeof( TESField ) + dataSize );
	}
	template<class T>
	T* data() {
		return (T*) ( ( (char*) this ) + sizeof( TESField ) );
	}

	char* EDID() {
		return (char*) ( ( (char*) this ) + sizeof( TESField ) );
	}
	char* FULL() {
		return (char*) ( ( (char*) this ) + sizeof( TESField ) );
	}
};


