#pragma once

#include "TES.h"
#include "TESSignature.h"

struct TESField;

//////////////////////////////////////////////////////////////////////////////////
struct TESRecord final {
public:
	TESSignature sign;
	quint32 dataSize;
	quint32 RecordFlags;
	quint32 formID;//uint32	Record( form ) identifier

	quint32 VersionControlInfo1;
	quint16 FormVersion;
	quint16 VersionControlInfo2;

	//TESRecord();
	TESRecord( const TESRecord& r );

	/////////////////////////////////////////
	bool isCompressed();

	/////////////////////////////////////////
	bool isLocalized();

	

	template<class T>
	T* data() {
		return (T*) ( ( (char*) this ) + sizeof( TESRecord ) );
	}
	template<class T>
	T* next() {
		return (T*) ( ( (char*) this ) + sizeof( TESRecord ) + dataSize );
	}

	static void enumerateFields( TESRecord* r, std::function<bool( TESField* )> cb );

	
};


/////////////////////////////////////////
inline
bool TESRecord::isCompressed() {
	return $::has( RecordFlags, Tes4Flag::Compressed );
}

/////////////////////////////////////////
inline
bool TESRecord::isLocalized() {
	return 	$::has( RecordFlags, Tes4Flag::Localized );
}




