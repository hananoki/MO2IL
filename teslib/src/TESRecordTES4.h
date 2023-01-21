#pragma once

#include "TESFieldReader.h"

//////////////////////////////////////////////////////////////////////////////////
class TESRecordTES4 : public TESFieldReader {
public:
	TESRecordTES4( TESFile* form, TESRecord* r );

	struct Header {
		float Version;
		quint32 NumberOfRecords;
		quint32 NextObjectID;
	} header;

	QList<TESTypeMasterData> master(); // MAST
	//QList<qint32> overriddenForms; // ONAM

		/////////////////////////////////////////
	bool isESL() {
		return 	$::has( record.RecordFlags, Tes4Flag::ESL );
	}

	/////////////////////////////////////////
	bool isLocalized() {
		return 	$::has( record.RecordFlags, Tes4Flag::Localized );
	}

	//struct TransientTypes {
	//	qint32 formType;
	//	QList<qint32> data;
	//};
	//QList<TransientTypes> transientTypes; // TNAM

	virtual QString fieldTypeName( quint32 typeID ) override;

};

//////////////////////////////////////////////////////////////////////////////////
class TESRecordGMST : public TESFieldReader {
public:
	TESRecordGMST( TESForm* form, TESRecord* r );

	virtual QString fieldTypeName( quint32 typeID ) override;
};


//////////////////////////////////////////////////////////////////////////////////
class TESRecordCOBJ : public TESFieldReader {
public:
	TESRecordCOBJ( TESForm* form, TESRecord* r );

	virtual QString fieldTypeName( quint32 typeID ) override;
};



//////////////////////////////////////////////////////////////////////////////////
class TESRecordOMOD : TESFieldReader {
public:
	TESRecordOMOD( TESForm* form, TESRecord* r );

	QString* full;
	QString* desc;
	QString* modl;
	TESTypeMNAM* targetOMODKeywords;
};


//////////////////////////////////////////////////////////////////////////////////
class TESRecordSCPT : public TESFieldReader {
public:
	TESRecordSCPT( TESForm* form, TESRecord* r );

	//char* editorID();

	QString scriptSource();
};


//////////////////////////////////////////////////////////////////////////////////
class TESRecordFallback : public TESFieldReader {
public:
	TESRecordFallback( TESForm* file, TESRecord* r );
	virtual ~TESRecordFallback();

	char* editorID();

	//QString* scriptSource;
	//QString* modl;
	//TESTypeMNAM* targetOMODKeywords;

	//QList<QByteArray> data;
	virtual QString fieldTypeName( quint32 ) override { return ""; }
};


