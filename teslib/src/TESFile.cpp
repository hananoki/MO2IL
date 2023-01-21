#include "TESFile.h"
#include "TESStringTable.h"
#include "VFileSys.h"
#include "TESSystem.h"
#include "TESType.h"
#include "TESForm.h"
#include "TESField.h"
#include "TESRecordTES4.h"
#include "TESFormAllocator.h"
#include "zlib.h"
//#include "HLogView.h"

//extern QMap<QString, QString> transMap;

//////////////////////////////////////////////////////////////////////////////////
class TESFile::Impl /*: public QSharedData*/ {
public:
	//QString fileName;
	QString fullPath;

	// レコード以下のデータは遅延評価するため、バイナリは保持しておく
	// TESRecord* TESForm::fileDataにレコード初期化用に参照される
	// TESFieldReader評価時にTESRecordは書換え可能なように複製される
	QByteArray bytes;
	//TESRecord* recordHeader; // bytes内の参照

	TESLoadOrder loadOrder;

	std::unique_ptr<TESRecordTES4> tes4;

	// グループID、フォームリスト
	QHash<quint32, QList<TESForm*>> overrideFormIDs;

	//QList<TESGroupData> groups;
	QList<TESGroupNode*> m_topGroups;

	//QList<CellBlock*> cellBlocks;
	QList<TESGroupNode*> m_world;
	//World world;

	std::unique_ptr<TESStringTable> STRINGS;
	std::unique_ptr<TESStringTable> ILSTRINGS;
	std::unique_ptr<TESStringTable> DLSTRINGS;

	QHash<quint32, quint32> cacheMasterID;
	//template<class T>
	//T* getOffsetPtrFromTop( int offset ) {
	//	return offsetPtr<T>( bytes.data(), offset );
	//}



	/////////////////////////////////////////
	QString fileName() {
		return path::getFileName( fullPath );
	}

	QFuture<void> m_future;
	QFutureWatcher<void> m_watcher;

	TESFile* self;

	/////////////////////////////////////////
	Impl( TESFile* _self ) :
		self( _self ),
		STRINGS( nullptr ),
		ILSTRINGS( nullptr ),
		DLSTRINGS( nullptr ) {

		//QObject::connect( &m_watcher, &QFutureWatcher<void>::finished,  [&]() {
		//	
		//} );
		QObject::connect( &m_watcher, &QFutureWatcher<void>::finished, self, [&]() {
			//qDebug() << "bbb";
			for( auto* group : m_topGroups ) {
				QList<TESForm*> result;
				group->getAllForms( result );
				for( auto* fm : result ) {
					addTranslateTargetForm( fm );
				}
			}
			//qDebug() << "aaa";
			emit self->signal_loadComplete();
		} );
		//qDebug() << "aaaaaa";
	}


	/////////////////////////////////////////
	~Impl() {
		for( auto& k : overrideFormIDs.keys() ) {
			//for( auto* fm : formIDs[ k ] ) {
			//	delete fm;
			//}
			auto& lst = overrideFormIDs[ k ];
			for( int i = 0; i < lst.size(); i++ ) {
				//delete lst[ i ];
			}
			lst.clear();
		}
		overrideFormIDs.clear();
	}


	/////////////////////////////////////////
	bool isESL() {
		return tes4->isESL();
	}


	/////////////////////////////////////////
	bool isLocalized() {
		return tes4->isLocalized();
	}


	/////////////////////////////////////////
	void loadStrings() {
		if( !isLocalized() )return;

		auto bname = path::getBaseName( fileName() );
		QString locale = "en";
		if( tesSys.gameTitle() == "Skyrim Special Edition" ) {
			locale = "english";
		}

		auto s = $$( tesSys.gameTitle() + "/strings/%1_%2.STRINGS" ).arg( bname ).arg( locale );
		auto il = $$( tesSys.gameTitle() + "/strings/%1_%2.ILSTRINGS" ).arg( bname ).arg( locale );
		auto dl = $$( tesSys.gameTitle() + "/strings/%1_%2.DLSTRINGS" ).arg( bname ).arg( locale );
		//if( s.isEmpty() ) {
		//	s = fileDB.absolutePath( $$( "strings/%1_%2.STRINGS" ).arg( bname ).arg( locale ) );
		//}
		//auto il = fileDB.absolutePath( $$( "strings/%1_%2.ILSTRINGS" ).arg( bname ).arg( locale ) );
		//auto dl = fileDB.absolutePath( $$( "strings/%1_%2.DLSTRINGS" ).arg( bname ).arg( locale ) );

		STRINGS = std::make_unique<TESStringTable>( s );
		ILSTRINGS = std::make_unique<TESStringTable>( il );
		DLSTRINGS = std::make_unique<TESStringTable>( dl );

		//QStringList dump;
		//for( auto& k : STRINGS->keys() ) {
		//	auto t = ( *STRINGS )[ k ];
		//	dump << $$( "%1\t%2" ).arg( k ).arg( t );
		//}
		//fs::writeAllLines(s+".txt",dump);
	}


	/////////////////////////////////////////
	QString strings( int hash ) {
		auto s = ( *STRINGS )[ hash ];
		if( !s.isEmpty() )return s;

		auto dl = ( *DLSTRINGS )[ hash ];
		if( !dl.isEmpty() )return dl;

		auto il = ( *ILSTRINGS )[ hash ];
		if( !il.isEmpty() )return il;

		return "";
	}



	///////////////////////////////////////////
	//void parseGroup( TESGroupNode* parent ) {
	//	auto& groupData = parent->groupData;
	//	quint32* p = groupData->data<quint32>();
	//	quint32* end = groupData->next<quint32>();
	//	//int groupType = groupData->groupType;

	//	for( ; p < end; ) {
	//		if( *p == GRUP ) {
	//			auto* group = (TESGroup*) p;
	//			quint32* next = group->next<quint32>();

	//			switch( group->groupType ) {
	//				case 2:
	//				case 3:
	//				case 4:
	//				case 5:
	//				case 8:
	//				case 9:
	//					parent->addChild( group );
	//					parseGroup( parent->nodes.last() );
	//					break;
	//				case 1: // WRLD
	//				case 6: // CELL
	//				case 7: // DIAL
	//				case 10://QUST
	//					if( parent->forms.isEmpty() || parent->forms.last() == nullptr ) {
	//						qDebug() << "";
	//					}
	//					else {
	//						parent->forms.last()->groupNode = new TESGroupNode( group );
	//						parseGroup( parent->forms.last()->groupNode );
	//					}
	//					break;
	//			}

	//			p = next;
	//		}
	//		else {
	//			TESRecord* record = (TESRecord*) p;

	//			TESForm* form = TESFormAllocator::alloc( self, record );

	//			parent->addForm( form );

	//			//switch( form->typeIDFast() ) {
	//			//	case CELL:
	//			//	case WRLD:
	//			//	case REFR:
	//			//	case DIAL:
	//			//	case INFO:
	//			//	case QUST:
	//			//		addTranslateTargetForm( form );
	//			//		break;
	//			//}
	//			p = record->next<quint32>();
	//		}
	//	}
	//}


	/////////////////////////////////////////
	void addTranslateTargetForm( TESForm* fm ) {
		auto typeID = fm->typeIDFast();
		auto it = overrideFormIDs.find( typeID );
		if( it == overrideFormIDs.constEnd() ) {
			overrideFormIDs.insert( typeID, QList<TESForm*>() );
			it = overrideFormIDs.find( typeID );
		}
		it->append( fm );
	}


	struct Execute {
		Execute( TESFile::Impl* p ) : self( p ) {}
		TESFile::Impl* self;

		void operator()( TESGroupNode* x ) {

			//x->func();
			TESGroupNode::parseGroup( self->self, x );
		}
	};

	bool loadFromFileAsync( const QString& fname ) {
		if( !fs::isExistFile( fname ) )return false;

		fullPath = fname;

		QFile file( fullPath );
		if( !file.open( QIODevice::ReadOnly ) ) {
			//HLogView::warning( $$( "Open Error: %1" ).arg( fname ) );
			//qDebug() << u8"error. ファイルは存在するが何らかの理由でファイルが開けない";
			return false;
		}
		bytes = file.readAll();

		auto* recordHeader = offsetPtr<TESRecord>( bytes.data(), 0 );// getOffsetPtrFromTop<TESRecord>( 0 );

		tes4 = std::unique_ptr<TESRecordTES4>(
			new TESRecordTES4( self, recordHeader )
			);

		loadStrings();

		tesSys.registerFile( self );

		auto* group = recordHeader->next<TESGroup>();
		for( ; group->sign.typeID == GRUP; group = group->next<TESGroup>() ) {
			m_topGroups << new TESGroupNode( group );

			//parseGroup( m_topGroups.last() );
			TESGroupNode::parseGroup( self, m_topGroups.last() );
		}

		m_future = QtConcurrent::map( m_topGroups, Execute( this ) );
		m_watcher.setFuture( m_future );

		return true;
	}

	/////////////////////////////////////////
	bool loadFromFile( const QString& fname ) {
		if( !fs::isExistFile( fname ) )return false;

		fullPath = fname;

		QFile file( fullPath );
		if( !file.open( QIODevice::ReadOnly ) ) {
			//HLogView::warning( $$( "Open Error: %1" ).arg( fname ) );
			//qDebug() << u8"error. ファイルは存在するが何らかの理由でファイルが開けない";
			return false;
		}
		bytes = file.readAll();

		auto* recordHeader = offsetPtr<TESRecord>( bytes.data(), 0 );// getOffsetPtrFromTop<TESRecord>( 0 );

		tes4 = std::unique_ptr<TESRecordTES4>(
			new TESRecordTES4( self, recordHeader )
			);

		loadStrings();

		tesSys.registerFile( self );

		auto* group = recordHeader->next<TESGroup>();
		for( ; group->sign.typeID == GRUP; group = group->next<TESGroup>() ) {
			m_topGroups << new TESGroupNode( group );

			//parseGroup( m_topGroups.last() );
			TESGroupNode::parseGroup( self, m_topGroups.last() );
		}

		//m_future = QtConcurrent::map( m_topGroups, Execute( this ) );
		//m_watcher.setFuture( m_future );

		//for( auto* group : m_topGroups ) {
		//	QList<TESForm*> result;
		//	group->getAllForms( result );
		//	for( auto* fm : result ) {
		//		addTranslateTargetForm( fm );
		//	}
		//}

		return true;
	}


	/**
	 * @brief  ロード順に即したフォームIDに修飾する
	 */
	quint32 convertLoadOrder( quint32 id ) {
		quint32 loadID = 0;
		int no = ( id & 0xFF000000 ) >> 24;
		auto master = tes4->master();
		if( no < master.length() ) {
			auto  it = cacheMasterID.find( no );
			if( it == cacheMasterID.constEnd() ) {
				loadID = tesSys.getMasterFormID( master[ no ].fileName );
				cacheMasterID.insert( no, loadID );
			}
			else {
				loadID = *it;
			}
		}
		else {
			if( loadOrder.main == 0xFE ) {
				loadID = 0xFE000000 | ( loadOrder.sub << 16 );
			}
			else {
				loadID = loadOrder.main << 24;
			}
		}

		if( 0xFE000000 == ( loadID & 0xFF000000 ) ) {
			return ( loadID & 0xFFFFF000 ) | ( id & 0x00000FFF );
		}
		return ( loadID & 0xFF000000 ) | ( id & 0x00FFFFFF );
	}


	void convertLocalized() {
		//tes4->
	}
};



/////////////////////////////////////////
TESFile::TESFile() :
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
TESFile::~TESFile() {
}


/////////////////////////////////////////
//TESModFile& TESModFile::operator =( const TESModFile& other ) {
//	d = other.d;
//	return *this;
//}

/////////////////////////////////////////
TESFile* TESFile::loadFromFile( const QString& fname ) {
	auto* modFile = new TESFile();
	if( !modFile->impl->loadFromFile( fname ) ) {
		delete modFile;
		modFile = nullptr;
	}
	return 	modFile;
}
/////////////////////////////////////////
TESFile* TESFile::loadFromFileAsync( const QString& fname ) {
	auto* modFile = new TESFile();
	if( !modFile->impl->loadFromFileAsync( fname ) ) {
		delete modFile;
		modFile = nullptr;
	}
	return 	modFile;
}

/////////////////////////////////////////
TESRecordTES4* TESFile::_TES4() {
	return impl->tes4.get();
}

/////////////////////////////////////////
bool TESFile::isESL() {
	return impl->isESL();
}


/////////////////////////////////////////
bool TESFile::isLocalized() {
	return impl->isLocalized();
}


/////////////////////////////////////////
QString TESFile::fileName() {
	return impl->fileName();
}

/////////////////////////////////////////
QString TESFile::fullPath() {
	return impl->fullPath;
}


/////////////////////////////////////////
TESLoadOrder TESFile::loadOrder() {
	return impl->loadOrder;
}

/////////////////////////////////////////
void TESFile::setLoadOrder( TESLoadOrder loadOrder ) {
	impl->loadOrder = loadOrder;
}


QList<quint32> TESFile::formTypes() {
	return impl->overrideFormIDs.keys();
}


/**
 * @brief  指定したグループIDからフォームリストを取得する
 */
QList<TESForm*>& TESFile::formDataList( quint32 groupID ) {
	return impl->overrideFormIDs[ groupID ];
}


/////////////////////////////////////////
QList<TESGroupNode*>& TESFile::topGroups() {
	return impl->m_topGroups;
}


/////////////////////////////////////////
TESGroupNode* TESFile::topGroups( quint32 groupID ) {
	for( auto* p : impl->m_topGroups ) {
		if( p->typeID() == groupID ) {
			return p;
		}
	}
	return nullptr;
}


/////////////////////////////////////////
//QList<CellBlock*> TESFile::blocks() {
//	return impl->cellBlocks;
//}
void TESFile::addTranslateTargetForm( TESForm* fm ) {
	impl->addTranslateTargetForm( fm );
}


/////////////////////////////////////////
QString TESFile::strings( int hash ) {
	return impl->strings( hash );
}


/////////////////////////////////////////
quint32 TESFile::convertLoadOrder( quint32 id ) {
	return impl->convertLoadOrder( id );
}


/////////////////////////////////////////
QString TESFile::convertLoadOrderString( quint32 _id ) {
	quint32 id = convertLoadOrder( _id );
	auto* form = tesSys.formData( id );
	auto txt = form->editorName;
	if( !form->fullName.isEmpty() ) {
		txt = form->fullName;
	}
	if( form ) {
		return $$( "[%2] %1" ).arg( txt ).arg( tes4lib::toStringFromID( id ) );
	}
	return $$( "%1 - not found" ).arg( tes4lib::toStringFromID( id ) );
}

void TESFile::convertLocalized() {
	return impl->convertLocalized();
}

