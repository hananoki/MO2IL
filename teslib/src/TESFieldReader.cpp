#include "TESFieldReader.h"
#include "TESForm.h"
#include "TESRecord.h"
#include "TESFile.h"
#include "TESField.h"


//////////////////////////////////////////////////////////////////////////////////
TESFieldReader::TESFieldReader( TESFile* f, TESRecord* r ) :
	file( f ),
	record( *r ) {

	TESRecord::enumerateFields( r, [&]( TESField* f ) {
		data << QByteArray( (char*) f, sizeof( TESField ) + f->dataSize );
		return true;
	} );
}

/////////////////////////////////////////
TESFieldReader::TESFieldReader( TESForm* _form, TESRecord* r ) :
	form( _form ),
	file( _form->file ),
	record( *r ) {

	//if(r->formID == 784294 ) {
	//	qDebug() << "";
	//}
	if( form && form->decompressedData ) {
		auto* p = (TESField*) _form->decompressedData;
		auto* ed = (TESField*) ( _form->decompressedData + form->decompressedSize );
		for( ; p < ed; p = p->next() ) {
			data << QByteArray( (char*) p, sizeof( TESField ) + p->dataSize );
		}
	}
	else {
		TESRecord::enumerateFields( r, [&]( TESField* f ) {
			data << QByteArray( (char*) f, sizeof( TESField ) + f->dataSize );
			return true;
		} );
	}
}


/////////////////////////////////////////
TESFieldReader::~TESFieldReader() {
}


/////////////////////////////////////////
void TESFieldReader::fields( std::function<void( TESField*, void* )> cb ) {
	for( auto& p : data ) {
		TESField* f = (TESField*) p.data();
		cb( f, p.data() + sizeof( TESField ) );
	}
}

/////////////////////////////////////////
void TESFieldReader::localizeFields( QList<quint32> useIDs, std::function<void( QByteArray&, TESField*, void*, TESStringsType type )> cb ) {
	for( auto& p : data ) {
		TESField* f = (TESField*) p.data();

		if( useIDs.indexOf( f->sign.typeID ) < 0 )continue;

		cb( p, f, p.data() + sizeof( TESField ), TESStringTable::stringsType( form, f ) );
	}
}

/////////////////////////////////////////
/**
 * 全てのフィールドデータサイズを取得します
 * TESRecordのデータサイズ用
 */
quint32 TESFieldReader::fieldDataSize() {
	quint32 size = 0;
	for( auto& p : data ) {
		size += sizeof( TESField );
		size += ( (TESField*) p.data() )->dataSize;
	}
	return size;
}


/////////////////////////////////////////
char* TESFieldReader::editorID() {
	return at<char*>( EDID );
}


/////////////////////////////////////////
void TESFieldReader::defaultParseField( TESField* f, void* p ) {
	switch( f->sign.typeID ) {
		case EDID:
			fieldData.insert( EDID, reinterpret_cast<char*>( p ) );
			break;
		case DESC:
			if( form->file->isLocalized() ) {
				fieldData.insert( DESC, TESFieldValue( *reinterpret_cast<quint32*>( p ), TESFieldValue::LString ) );
			}
			else {
				fieldData.insert( DESC, TESFieldValue( reinterpret_cast<char*>( p ) ) );
			}
			break;
	}
}


