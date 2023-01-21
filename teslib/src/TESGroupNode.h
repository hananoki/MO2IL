#pragma once

#include "TESSignature.h"

class TESForm;;
class TESFile;;
struct TESGroup;

//////////////////////////////////////////////////////////////////////////////////
class TESGroupNode {
public:
	TESGroupNode( const TESGroupNode& other );

	TESGroupNode( TESGroup* group );

	TESGroupNode* addChild( TESGroup* group );
	void addForm( TESForm* form );

	TESSignature groupName();
	quint32 typeID();
	quint32 dataSize();

	void getAllForms( QList<TESForm*>& result );

	static void parseGroup( TESFile* file, TESGroupNode* parent );

	/**
	 * @brief  グループのデータサイズを計算して返します
	 */
	quint32 calcDataSize();

	TESGroup* groupData;

	QList<TESGroupNode*> nodes;
	QList<TESForm*> forms;
};





