#include "TESStringTable.h"
#include "Qtlib.h"
#include <QMap>
#include "TESForm.h"
#include "TESField.h"

//////////////////////////////////////////////////////////////////////////////////
struct StringHeader {
	quint32 count;
	quint32 dataSize;
};


//////////////////////////////////////////////////////////////////////////////////
struct DirectoryEntry {
	quint32 stringID;
	quint32 offset;
};


//////////////////////////////////////////////////////////////////////////////////
class TESStringTable::Impl {
public:
	QMap<quint32, QString> data;

	/////////////////////////////////////////
	Impl( const QString& filePath ) {
		if( !fs::isExistFile( filePath ) ) {
			//HLogView::warning( "file not found: "+ filePath );
			return;
		}

		auto fl = filePath.toLower();
		bool lstring = false;
		if( path::hasSuffix( fl, "dlstrings" ) || path::hasSuffix( fl, "ilstrings" ) ) {
			lstring = true;
		}

		auto bytes = fs::readAll( filePath );
		StringHeader* sh = (StringHeader*) bytes.data();

		QList<DirectoryEntry*> deList;
		DirectoryEntry* de = (DirectoryEntry*) ( bytes.data() + sizeof( StringHeader ) );
		for( quint32 i = 0; i < sh->count; i++ ) {
			deList << &de[ i ];
		}
		char* baseAdress = (char*) &de[ sh->count ];

		int ofs = lstring ? 4 : 0;
		for( auto& pa : deList ) {
			/*auto a = QString::fromUtf8( baseAdress + pa->offset + ofs );
			auto ch = a.toUtf8();
			ch .append( (char) 0 );
			int len = ch.size();*/
			data.insert( pa->stringID, QString::fromUtf8( baseAdress + pa->offset + ofs ) );
		}
	}
};


/////////////////////////////////////////
TESStringTable::TESStringTable( const QString& filePath ) :
	impl( new Impl( filePath ) ) {
}


/////////////////////////////////////////
TESStringTable::~TESStringTable() {
}


/////////////////////////////////////////
QString TESStringTable::operator[]( quint32 hash ) {
	auto it = impl->data.find( hash );
	if( it == impl->data.constEnd() )return "";
	return *it;
}

QList<quint32> TESStringTable::keys() {
	return impl->data.keys();
}


TESStringsType TESStringTable::stringsType( TESForm* form, TESField* field ) {
	TESStringsType type = TESStringsType::Strings;
	if( field->sign.typeID == DESC && form->typeID() == LSCR ) type = TESStringsType::Strings;
	else if( field->sign.typeID == DESC ) type = TESStringsType::DLStrings;
	else if( field->sign.typeID == CNAM && form->typeID() == QUST ) type = TESStringsType::DLStrings;
	else if( field->sign.typeID == CNAM && form->typeID() == BOOK ) type = TESStringsType::DLStrings;
	else if( field->sign.typeID == CNAM && form->typeID() == BOOK ) type = TESStringsType::DLStrings;
	else if( field->sign.typeID == NAM1 && form->typeID() == INFO ) type = TESStringsType::ILStrings;
	return type;
}
