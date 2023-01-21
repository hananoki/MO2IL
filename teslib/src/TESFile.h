#pragma once

#include "TES.h"
#include "TESGroupNode.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QSharedDataPointer>

class TESSystem;
class TESForm;
class TESRecordTES4;

//////////////////////////////////////////////////////////////////////////////////
class TESFile :public QObject {
	Q_OBJECT
	friend class TESSystem;
public:
	struct Group;

	TESFile();
	//TESFile( const TESFile& other );

	~TESFile();


	/////////////////////////////////////////
	bool isESL();

	/////////////////////////////////////////
	bool isLocalized();

	/////////////////////////////////////////
	static TESFile* loadFromFile( const QString& fname );
	static TESFile* loadFromFileAsync( const QString& fname );

	TESLoadOrder loadOrder();

	

	/**
	 * @brief  TES4レコードを取得する
	 */
	TESRecordTES4* _TES4();

	QString fileName();
	QString fullPath();

	QString strings( int hash );

	QList<quint32> formTypes();

	/**
	 * @brief  指定したグループIDからフォームリストを取得する
	 */
	QList<TESForm*>& formDataList( quint32 groupID );

	QList<TESGroupNode*>& topGroups();
	TESGroupNode* topGroups( quint32 groupID );

	//QList<CellBlock*> blocks();

	/**
	 * @brief  ロード順に即したフォームIDに修飾する
	 */
	quint32 convertLoadOrder( quint32 id );

	QString convertLoadOrderString( quint32 id );

	void convertLocalized();

	void addTranslateTargetForm( TESForm* fm );

signals:
		void signal_loadComplete(  );

private:
	class Impl;
	std::unique_ptr<Impl> impl;

	void setLoadOrder( TESLoadOrder loadOrder );
};


//Q_DECLARE_SHARED( TESModFile )

