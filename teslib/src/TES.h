#pragma once

#include "TESSignature.h"
#include "TESGroup.h"
#include "zlib.h"

class TESFile;
class TESForm;
struct TESRecord;
struct TESField;

enum Tes4Flag : quint32 {
	ESM = 0x00000001,
	Localized = 0x00000080,
	ESL = 0x00000200,
	Compressed = 0x00040000,
};

template<class T>
T* offsetPtr( void* p, int offset ) {
	return (T*) ( ( (char*) p ) + offset );
}

#pragma pack(1)

namespace zlib {
	int uncompress( Bytef* dest, uInt destLen, const Bytef* source, uInt sourceLen );
	int compress( Bytef* dest, uInt destLen, const Bytef* source, uInt sourceLen );
}

//////////////////////////////////////////////////////////////////////////////////
struct TESLoadOrder {
	quint16 main;
	quint16 sub;

	QString toString() {
		if( 0xFE == main ) {
			return $$( "[FE %1]" ).arg( sub, 3, 16, QChar( '0' ) );
		}
		return $$( "[%1]" ).arg( main, 2, 16, QChar( '0' ) );
	}
};





//struct CellForm {
//	TESForm* form;
//	QList<TESForm*> persistent;
//	QList<TESForm*> temporary;
//	std::unique_ptr<TESGroup> cellChildren;
//	std::unique_ptr<TESGroup> cellPersistentChilden;
//	std::unique_ptr<TESGroup> cellTemporaryChildren;
//};
//union CellGrid {
//	quint32 yx;
//	struct {
//		quint16 y;
//		quint16 x;
//	};
//};
//struct CellSubBlock {
//	QList<CellForm*> cells;
//	TESGroup group;
//	//CellGrid grid;
//	CellSubBlock( TESGroup* g ) :group( *g ) {}
//	//CellSubBlock( quint32 yx ) { grid.yx = yx; }
//};
//
//struct CellBlock {
//	QList<CellSubBlock*> subBlocks;
//	TESGroup group;
//	//CellGrid grid;
//	CellBlock( TESGroup* g ) :group( *g ) {}
//	//CellBlock( quint32 yx ) { grid.yx = yx; }
//};
//
//struct World {
//	CellForm cell;
//	QList<CellBlock*> blocks;
//};


//struct TESRecordTES4;

namespace tes4lib {
	QString getStrings( TESField* field, TESFile* file );
	//void read( TESRecordTES4*, TESField* fieldBegin, TESField* fieldEnd );

	QString* id2Name( quint32 id );
	QString toStringFromID( quint32 id );
}



#pragma pack()
