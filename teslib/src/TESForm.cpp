#include "TESForm.h"
#include "TESRecord.h"
#include "TESType.h"
#include "TESFile.h"
#include "TESField.h"
#include "TESRecordTES4.h"
#include "TESSystem.h"

#pragma warning(disable:4311)
#pragma warning(disable:4302)

#define HANDLER(x) \
	case x:\
		readdata = std::make_unique< TESRecord##x >( this, fileData );\
		break;

//////////////////////////////////////////////////////////////////////////////////
TESForm::TESForm( TESFile* _file, TESRecord* _record ) :
	file( _file ),
	item( nullptr ),
	readdata( nullptr ),
	groupNode( nullptr ),
	decompressedData( nullptr ),
	decompressedSize( 0 ),
	compressedData( nullptr ),
	compressedSize( 0 ),
	fileData( _record ) {

	if( _file == nullptr )return;

	ID = file->convertLoadOrder( _record->formID );


	if( _record->isCompressed() ) {
		//if( _record->sign.typeID == NAVM ) {
		//	qDebug() << "";
		//}
		//else {
		//if( _record->formID == 784294 ) {
		//	qDebug() << "";
		//}

			char* zip = offsetPtr<char>( _record, sizeof( TESRecord ) );
			auto dSize = *( (uLongf*) zip );
			zip += 4;
			auto p = new quint8[ dSize ];
			int nn = zlib::uncompress( p, dSize, (const Bytef*) zip, _record->dataSize - 4 );
			if( nn != 1 ) {
				qDebug() << "";
			}
			decompressedData = p;
			decompressedSize = dSize;
		//}
	}

	editorName = editorID();
	fullName = getFullName();

	tesSys.registerForm( this );
}


/////////////////////////////////////////
TESForm::~TESForm() {
	ID = 0xEFBEADDE;
}


/////////////////////////////////////////
quint32 TESForm::typeID() {
	auto* p = data();
	if( !p )return -1;
	return p->record.sign.typeID;
}


/////////////////////////////////////////
quint32 TESForm::typeIDFast() {
	return fileData->sign.typeID;
}


/////////////////////////////////////////
TESSignature TESForm::sign() {
	auto* p = data();
	if( !p )return TESSignature();
	return p->record.sign;
}



TESFieldReader* TESForm::data() {
	if( readdata == nullptr ) {
		if( fileData ) {
			switch( fileData->sign.typeID ) {
				HANDLER( GMST )
					HANDLER( COBJ )
					//HANDLER( OMOD )
					//HANDLER( SCPT )
				case 0:
					break;
				default:
					readdata = std::make_unique<TESRecordFallback>( this, fileData );
					break;
			}
		}
		else if( file ) {
			return file->_TES4();
			//readdata = std::make_unique<TESRecordFallback>( this, file->_TES4() );
			//readdata = file->_TES4();
		}
	}
	return readdata.get();
}


/////////////////////////////////////////
QString TESForm::editorID() {
	//if( !editorName.isEmpty() )return editorName;

	TESField* p, * ed;
	if( decompressedData ) {
		p = (TESField*) decompressedData;
		ed = (TESField*) ( decompressedData + decompressedSize );
	}
	else {
		p = offsetPtr<TESField>( fileData, sizeof( TESRecord ) );
		ed = offsetPtr<TESField>( fileData, sizeof( TESRecord ) + fileData->dataSize );
	}

	for( ; p < ed; p = p->next() ) {
		if( p->sign.typeID == EDID ) {
			return QString( p->data<char>() );
		}
	}
	return $$( "" );
}


/////////////////////////////////////////
QString TESForm::getFullName() {
	//if( !editorName.isEmpty() )return editorName;

	TESField* p, * ed;
	if( decompressedData ) {
		p = (TESField*) decompressedData;
		ed = (TESField*) ( decompressedData + decompressedSize );
	}
	else {
		p = offsetPtr<TESField>( fileData, sizeof( TESRecord ) );
		ed = offsetPtr<TESField>( fileData, sizeof( TESRecord ) + fileData->dataSize );
	}

	for( ; (int) p < (int) ed; p = p->next() ) {
		if( p->sign.typeID == FULL ) {
			auto* c = p->FULL();
			if( file->isLocalized() ) {
				return file->strings( *( (quint32*) c ) );
			}
			return  QString::fromUtf8( c );
		}
	}

	return $$( "" );
}


quint32 TESForm::getRecordSize() {
	//if( fileData->sign.typeID == NAVM ) {
	//	qDebug() << "";
	//}
	//else {
		if( fileData->isCompressed() ) {
			if( !compressedData ) {
				QByteArray by;
				auto* reader = data();
				for( auto& b : reader->data )
					by.append( b );
				Bytef* dest = new quint8[ 80960 ];
				memset( dest, 0, 80960 );
				uLongf destLen = 80960;
				int nn = ::compress( &dest[ 4 ], &destLen, (const Bytef*) by.constData(), by.length() );
				//zlib::compress( 0, 0, ( const Bytef* ) by.constData(), by.length() );
				compressedData = dest;
				//compressedSize = destLen;
				quint32* szp = (quint32*) dest;
				*szp = by.length();
				compressedSize = data()->record.dataSize = ( destLen + 4 );
			}
			return compressedSize;
		}
	//}
	if( readdata == nullptr ) {
		return fileData->dataSize;
	}
	return data()->record.dataSize;
}
