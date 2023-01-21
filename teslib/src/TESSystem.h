#pragma once

#include "TES.h"
#include "TESForm.h"

class TESFile;
class TESForm;

//////////////////////////////////////////////////////////////////////////////////
class TESSystem {
public:
	TESSystem();
	~TESSystem();

	void clear();

	TESFile* loadFile( const QString& filePath );

	void registerFile( TESFile* modFile );

	quint32 getMasterFormID( const QString& masterFileName );

	void registerForm( TESForm* form );

	int formStatus( quint32 formID );
	TESForm* formData( quint32 formID );
	const QList<TESForm*>& formDataList( quint32 formID );

	void enumerateForm( std::function<void( const QList<TESForm*>& )> cb );

	void enumerateFormByGroup( quint32 groupID, std::function<void( TESForm* )> cb );

	QString gameTitle();
	void setGameTitle( const QString& str );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

extern TESSystem tesSys;
