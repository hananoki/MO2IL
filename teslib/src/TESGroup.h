#pragma once

#include "TESSignature.h"

//////////////////////////////////////////////////////////////////////////////////
struct TESGroup final {
	TESSignature sign;
	quint32 DataSize;
	//ラベル。 形式はグループ タイプによって異なります (次のフィールドを参照)。
	TESSignature groupName;
	/*
	0	Top (Type)	char[4]	Record type
	1	World Children	formid	Parent (WRLD)
	2	Interior Cell Block	int32	Block number
	3	Interior Cell Sub-Block	int32	Sub-block number
	4	Exterior Cell Block	int16[2]	Grid Y, X (Note the reverse order)
	5	Exterior Cell Sub-Block	int16[2]	Grid Y, X (Note the reverse order)
	6	Cell Children	formid	Parent (CELL)
	7	Topic Children	formid	Parent (DIAL)
	8	Cell Persistent Childen	formid	Parent (CELL)
	9	Cell Temporary Children	formid	Parent (CELL)
	*/
	int groupType;
	quint16 timeStamp;
	quint16 versionControlInfo;
	quint32 unknown;

	template<class T>
	T* data() {
		return (T*) ( ( (char*) this ) + sizeof( TESGroup ) );
	}
	template<class T>
	T* next() {
		return (T*) ( ( (char*) this ) + DataSize );
	}
};



