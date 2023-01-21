#pragma once

#include "TESSignature.h"

struct TESRecord;
class TESFile;
class ItemModFormID;
class TESGroupNode;
class TESFormAllocator;

//////////////////////////////////////////////////////////////////////////////////
class TESForm final {
	friend class TESFile;
	friend class TESFieldReader;
	friend class TESFormAllocator;

public:
	~TESForm();
	quint32 ID; // 修飾済みID
	QString editorName;
	QString fullName;
	TESFile* file;
	TESGroupNode* groupNode;
	ItemModFormID* item;

	bool operator <( const TESForm* other ) const;

	quint32 typeID();
	quint32 typeIDFast();

	TESSignature sign();

	TESFieldReader* data();

	QString editorID();
	QString getFullName();

	quint32 getRecordSize();
	quint8* compressedData;

private:
	TESForm( TESFile* file, TESRecord* _fileData );
	TESRecord* fileData;
	std::unique_ptr<TESFieldReader> readdata;

	quint8* decompressedData;
	quint32 decompressedSize;

	quint32 compressedSize;
};


inline
bool TESForm::operator <( const TESForm* other ) const {
	return ( ID < other->ID );
};



