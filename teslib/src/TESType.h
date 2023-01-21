#pragma once

#include "TES.h"
#include "TESRecord.h"
#include "TESFieldReader.h"
#include "Qtlib.h"

class TESFile;
class TESForm;
struct TESField;



//////////////////////////////////////////////////////////////////////////////////
struct TESTypeMasterData {
	QString fileName;
	quint64 data;
};


//////////////////////////////////////////////////////////////////////////////////
struct TESTypeOBND {
	qint16 x1;
	qint16 y1;
	qint16 z1;
	qint16 x2;
	qint16 y2;
	qint16 z2;

	qint16* toPtr() { return (qint16*) this; }
	operator QString() const;
};

//////////////////////////////////////////////////////////////////////////////////
struct TESTypeCObjComponent {
	qint32 componentID;
	qint32 count;
};
//////////////////////////////////////////////////////////////////////////////////
struct TESTypeCObjData {
	qint16 count;
	qint16 pri;
};

//////////////////////////////////////////////////////////////////////////////////
struct TESTypeCondition {
	qint32 type;
	float comparisonValue;
	qint32 function;
	qint32 perk;
	qint32 none;
	qint32 runOn;
	qint32 unused;
	qint32 parameter;
};

//////////////////////////////////////////////////////////////////////////////////
class TESFieldValue {
public:
	//struct _MasterData {
	//	QString fileName;
	//	quint64 data;
	//};

	enum Type {
		String,
		LString,
		Int32,
		UInt32,
		UInt32Array,
		UInt64Array,
		Float,
		Bool,
		FormID,
		CObjData,
		CObjCompenet,
		MasterData,
		FormIDArray,
		Bounds,
	};
	Type type;

	union {
		QString* string;
		qint32* ivalue;
		quint32* uvalue;
		float* fvalue;
		bool* bvalue;
		QList<quint32>* uvalueArray;
		QList<quint64>* uvalue64Array;
		QList<TESTypeMasterData>* masterData;
	};
	TESFieldValue( char* str ) :
		string( new QString( str ) ),
		type( String ) {
	}
	TESFieldValue( QString str ) :
		string( new QString( str ) ),
		type( String ) {
	}
	TESFieldValue( qint32 i ) :
		ivalue( new qint32( i ) ),
		type( Int32 ) {
	}
	TESFieldValue( quint32 i , Type t = Type::UInt32) :
		uvalue( new quint32( i ) ),
		type( t ) {
	}
	TESFieldValue( bool b ) :
		bvalue( new bool( b ) ),
		type( Bool ) {
	}
	TESFieldValue( float f ) :
		fvalue( new float( f ) ),
		type( Float ) {
	}
	TESFieldValue( QList<quint32> list, Type t = Type::UInt32Array ) :
		uvalueArray( new QList<quint32>( list ) ),
		type( t ) {
	}
	TESFieldValue( QList<quint64> list, Type t = Type::UInt64Array ) :
		uvalue64Array( new QList<quint64>( list ) ),
		type( t ) {
	}
	TESFieldValue( QList<TESTypeMasterData> list ) :
		masterData( new QList<TESTypeMasterData>( list ) ),
		type( MasterData ) {
	}

	//~TESFieldValue() {
	//	switch( type ) {
	//		case String:
	//			delete string;
	//			break;
	//		case UInt32:
	//			delete ivalue;
	//			break;
	//	}
	//}

	QString toString() {
		switch( type ) {
			case String:
				return *string;
			case Int32:
			case UInt32:
				return $::toString( *ivalue );
			case Bool:
				return $::toString( *ivalue ? "True" : "False" );
			case Float:
				return $::toString( *fvalue, 'f', 5 );
		}

	}
	//TESFieldValue(const ) {}
	//~TESFieldValue() {}
};


//////////////////////////////////////////////////////////////////////////////////
struct TESTypeMNAM {
	QList<quint32> keywords;

	TESTypeMNAM( TESField* field, TESFile* file );
};





