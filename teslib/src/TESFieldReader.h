#pragma once
#include <QHash>
#include <QList>
#include <QByteArray>
#include "TESType.h"
#include "TESStringTable.h"

class TESForm;
class TESFile;
class TESRecordTES4;
struct TESRecord;
struct TESField;

class TESFieldValue;

//////////////////////////////////////////////////////////////////////////////////
class TESFieldReader {
	friend class TESRecordTES4;
public:
	TESFile* file;
	TESForm* form;
	TESRecord record;

	QList<QByteArray> data; // 各フィールドのバイトデータ、圧縮展開済み

	//TESFiledReader() {}
	TESFieldReader( TESForm* form, TESRecord* r );
	virtual ~TESFieldReader();

	void fields( std::function<void( TESField*, void* )> cb );
	void localizeFields( QList<quint32> useIDs, std::function<void( QByteArray&,TESField*, void*, TESStringsType )> cb );

	quint32 fieldDataSize();

	template<class T>
	T at( quint32 typeID ) {
		for( auto& d : data ) {
			if( *( (quint32*) d.data() ) == typeID ) {
				return (T) ( d.data() + 6 );
			}
		}
		return nullptr;
	}

	/////////////////////////////////////////
	char* editorID();
	//QString full();
	//QString desc();

	QHash<quint32, TESFieldValue>  fieldData;

	virtual QString fieldTypeName( quint32 typeID ) = 0;

private:
	TESFieldReader( TESFile* f, TESRecord* r ); // TES4用

protected:
	void defaultParseField( TESField*, void* );
};

