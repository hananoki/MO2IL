#include "TESGroupNode.h"
#include "TESForm.h"
#include "TESRecord.h"
#include "TESGroup.h"
#include "TESFormAllocator.h"
#include "TESFile.h"


/////////////////////////////////////////
TESGroupNode::TESGroupNode( const TESGroupNode& other ) :
	groupData( other.groupData ),
	nodes( other.nodes ) {
}


/////////////////////////////////////////
TESGroupNode::TESGroupNode( TESGroup* group ) :
	groupData( group ) {
}

/////////////////////////////////////////
TESSignature TESGroupNode::groupName() {
	return groupData->groupName;
}


/////////////////////////////////////////
quint32 TESGroupNode::typeID() {
	return groupData->groupName.typeID;
}


/////////////////////////////////////////
quint32 TESGroupNode::dataSize() {
	return groupData->DataSize;
}

/////////////////////////////////////////
TESGroupNode* TESGroupNode::addChild( TESGroup* group ) {
	nodes << new TESGroupNode( group );
	return nodes.last();
}


/////////////////////////////////////////
void TESGroupNode::addForm( TESForm* form ) {
	forms << form;
}


/////////////////////////////////////////
void TESGroupNode::getAllForms( QList<TESForm*>& result ) {
	result.append( forms );
	for( auto& c : nodes ) {
		c->getAllForms( result );
	}
	for( auto& fm : forms ) {
		if( fm->groupNode ) {
			fm->groupNode->getAllForms( result );
		}
	}
}


/////////////////////////////////////////
/**
 * @brief  グループのデータサイズを計算して返します
 */
quint32 TESGroupNode::calcDataSize() {
	quint32 size = sizeof( TESGroup );
	for( auto& c : nodes ) {
		size += c->calcDataSize();
	}
	for( auto& fm : forms ) {
		size += fm->getRecordSize() + sizeof( TESRecord );
		if( fm->groupNode ) {
			size += fm->groupNode->calcDataSize();
		}
	}
	groupData->DataSize = size;
	return size;
}


/////////////////////////////////////////
void TESGroupNode::parseGroup( TESFile* file, TESGroupNode* parent ) {
	auto& groupData = parent->groupData;
	quint32* p = groupData->data<quint32>();
	quint32* end = groupData->next<quint32>();
	//int groupType = groupData->groupType;

	for( ; p < end; ) {
		if( *p == GRUP ) {
			auto* group = (TESGroup*) p;
			quint32* next = group->next<quint32>();

			switch( group->groupType ) {
				case 2:
				case 3:
				case 4:
				case 5:
				case 8:
				case 9:
					parent->addChild( group );
					parseGroup( file, parent->nodes.last() );
					//QtConcurrent::run( &TESGroupNode::parseGroup, file, parent->nodes.last() );
					break;
				case 1: // WRLD
				case 6: // CELL
				case 7: // DIAL
				case 10://QUST
					if( parent->forms.isEmpty() || parent->forms.last() == nullptr ) {
						qDebug() << "";
					}
					else {
						parent->forms.last()->groupNode = new TESGroupNode( group );
						//QtConcurrent::run(  &TESGroupNode::parseGroup, file, parent->forms.last()->groupNode);
						parseGroup( file, parent->forms.last()->groupNode );
					}
					break;
			}

			p = next;
		}
		else {
			TESRecord* record = (TESRecord*) p;

			TESForm* form = TESFormAllocator::alloc( file, record );

			parent->addForm( form );

			file->addTranslateTargetForm( form );
			//switch( form->typeIDFast() ) {
			//	case CELL:
			//	case WRLD:
			//	case REFR:
			//	case DIAL:
			//	case INFO:
			//	case QUST:
			//		addTranslateTargetForm( form );
			//		break;
			//}
			p = record->next<quint32>();
		}
	}
}
