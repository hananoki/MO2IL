#include "UIModListView.h"
#include "ui_UIModListView.h"

#include "UIMainWindow.h"
#include "TES.h"
#include "HLogView.h"
#include "VFileSys.h"
#include "ItemModFile.h"
#include "TESFile.h"
#include "TESSystem.h"
#include "App.h"
#include "UIItemFileRoot.h"
#include "TESFormAllocator.h"


//////////////////////////////////////////////////////////////////////////////////
class UIModListView::Impl : public Ui_UIModListView {
public:
	QFuture<void> future;
	QFutureWatcher<void> watcher;

	UIModListView* self;

	QMenu* contextMenuFile;

	/////////////////////////////////////////
	Impl( UIModListView* _self ) :
		self( _self ) {

		setupUi( self );
		treeWidgetMod->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );

		QObject::connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {

			HLogView::info( tr( u8"loadModFile ... complete" ) );

			emit self->signal_completeModList();
		} );

		QObject::connect( &watcher, &QFutureWatcher<void>::progressTextChanged, self, [&]( const QString& progressText ) {
			if( progressText[ 0 ] == '!' ) {
				HLogView::warning( $$( progressText ).replace( 0, 1, "" ) );
			}
			else {
				HLogView::info( progressText );
			}
		} );
		//qDebug() << sizeof( TESForm );
	}


	/////////////////////////////////////////
	void setup() {
#pragma region contextMenuFile
		contextMenuFile = new QMenu();
		contextMenuFile->addAction( $Action::create( tr( u8"エクスプローラで表示" ), ICON_EXPLORER, [&]() {
			//auto* file = treeWidget->currentItem<Item_File>();
			//if( file ) {
			//	$::showInExplorer( file->fullPath );
			//}
			auto* item = treeWidgetMod->currentItem<ItemModFormID>();
			if( item ) {
				$::showInExplorer( path::getDirectoryName( item->m_formData->file->fullPath() ) );
			}
		} ) );
		//contextMenuFile->addAction( $Action::create( tr( u8"FOJPテキストをインポート先に設定" ), [&]() {
		//	App::setImportXML();
		//} ) );

		$TreeWidget::customContextMenuRequested( treeWidgetMod, [&]( const QPoint& pos ) {
			QModelIndex index = treeWidgetMod->indexAt( pos );
			if( !index.isValid() ) return;

			auto* item = dynamic_cast<ItemModFormID*>( reinterpret_cast<QTreeWidgetItem*>( index.internalPointer() ) );
			if( item ) {
				contextMenuFile->exec( treeWidgetMod->viewport()->mapToGlobal( pos ) );
			}
		} );
#pragma endregion

		// アイテム追加
		connect( self, &UIModListView::signal_addItem, self, [&]( QTreeWidgetItem* item ) {
			treeWidgetMod->addTopLevelItem( item );
		} );
		//connect( self, &UIModListView::signal_addItem2, self, [&]( QTreeWidgetItem* item, QList<QTreeWidgetItem*>* child ) {
		//	for( int i = 0; i < child->length(); i++ ) {
		//		item->addChild( (*child)[i] );
		//	}
		//	//delete child;
		//} );

		// Modリスト構築が完了した
		connect( self, &UIModListView::signal_completeModList, [&]() {
			applyDuplicationColor();
		} );

		// Modファイル列挙が完了した時
		connect( &fileDB, &VFileSys::signal_completeStage1, [&]() {
			HLogView::info( tr( u8"loadModFile ... start" ) );

			//treeWidgetMod->clear();
			//future = QtConcurrent::run( this, &Impl::loadAllModFile );
			//watcher.setFuture( future );
		} );



		/// アイテムの選択切り替え
		$TreeWidget::itemSelectionChanged( treeWidgetMod, [&]() {
			auto* item = treeWidgetMod->currentItem<QTreeWidgetItem>();
			if( !item )return;

			auto* itemModFormID = dynamic_cast<ItemModFormID*>( item );
			if( itemModFormID ) {
				//qtApp->setTesFile( itemModFormID );
				emit self->signal_itemSelectionChanged( itemModFormID->m_formData );
				return;
			}
		} );

		// ダブルクリック
		$TreeWidget::itemDoubleClicked( treeWidgetMod, [&]( QTreeWidgetItem* item, int column ) {
			auto* fitem = treeWidgetMod->currentItem<ItemModFormID>();
			if( fitem ) {
				QProcess p;
				auto w = path::getDirectoryName( fitem->m_formData->file->fullPath() );
				p.setWorkingDirectory( w );
				p.startDetached( config.espToolPath, { path::separatorToOS( fitem->m_formData->file->fullPath() ) } );
			}
		} );
	}



	/////////////////////////////////////////
	void loadMod( const QString& filePath ) {

		//if( filePath.contains( "HonestHearts.esm" ) ) {
		//	auto list = tesSys.formDataList( 0x31dac );
		//	qDebug() << "debug";
		//}

		auto* modFile = tesSys.loadFile( filePath );

		TESForm* fd = TESFormAllocator::alloc( nullptr, nullptr ); //@todo 
		fd->file = modFile;

		auto* itemTop = new ItemModFormID( fd );

		auto no = modFile->loadOrder().toString();//modFile->isESL() ? $$( "[FE %1]" ).arg( eslCount, 3, 16, QChar( '0' ) ) : $$( "[%1]" ).arg( loadCount, 2, 16, QChar( '0' ) );
		auto txt = $$( "%1 %2" ).arg( no.toUpper(), -8, QChar( ' ' ) ).arg( modFile->fileName() );
		itemTop->setText( 0, txt );
		//itemTop->setText( 2,  );

		QMap<QString, quint32> rows;
		//for( quint32 id : modFile->formTypes() ) {
		//	auto* topGroupName = tes4lib::id2Name( id );
		//	if( topGroupName ) {
		//		rows.insert( *topGroupName, id );
		//	}
		//	else {
		//		rows.insert( TESSignature( id ).str(), id );
		//	}
		//}
		for( auto& group : modFile->topGroups() ) {
			quint32 id = group->typeID();
			auto* topGroupName = tes4lib::id2Name( id );
			if( topGroupName ) {
				rows.insert( *topGroupName, id );
			}
			else {
				rows.insert( TESSignature( id ).str(), id );
			}
		}

		for( auto& editorName : rows.keys() ) {
			TESSignature sign( rows[ editorName ] );

			auto* typeItem = new QTreeWidgetItem();

			//if( editorName ) {
			typeItem->setText( 0, editorName );
			//}
			//else {
			//	watcher.progressTextChanged( tr( u8"!loadModFile:(%1) typeID not found: %2" )
			//		.arg( modFile->fileName() )
			//		.arg( sign.str() ) );
			//}
			itemTop->addChild( typeItem );

			/*		if( editorName == "Object Reference" ) {
						continue;
					}*/

			if( editorName == "Cell" ) {
				auto* bl = modFile->topGroups( CELL );
				if( bl ) {
					QList<QTreeWidgetItem*> citems;
					for( auto* p : bl->nodes ) {
						auto* ci = new QTreeWidgetItem();
						ci->setText( 0, $$( "Block %1" ).arg( p->groupData->groupName.typeID ) );
						citems << ci;
						for( auto* pp : p->nodes ) {
							auto* c2 = new QTreeWidgetItem();
							c2->setText( 0, $$( "Sub-Block %1" ).arg( pp->groupData->groupName.typeID ) );
							ci->addChild( c2 );
							for( auto* ppp : pp->forms ) {
								c2->addChild( new ItemModFormID( ppp ) );
							}
						}
					}
					//emit self->signal_addItem2( typeItem, citems );
					typeItem->addChildren( citems );
				}

				continue;
			}
			else {
				auto lst = modFile->formDataList( sign.typeID );

				qSort( lst.begin(), lst.end(), []( const TESForm* s1, const TESForm* s2 )->bool {
					return ( s1->ID < s2->ID );
				} );

				QList<QTreeWidgetItem*> citems;

				for( auto* form : lst ) {
					citems << new ItemModFormID( form );
				}
				typeItem->addChildren( citems );
			}
			//emit self->signal_addItem2( typeItem, citems );

		}
		emit self->signal_addItem( itemTop );
	}


	/////////////////////////////////////////
	void loadAllModFile() {
		tesSys.clear();

		auto modFiles = fileDB.getModFiles();

		auto load = [=]( const QString fileName ) {
			auto it = modFiles.find( fileName.toLower() );
			if( it == modFiles.constEnd() ) {
				qDebug() << "not found. " << fileName;
				//HLogView::warning( tr( u8"loadModFile: find not found: %1" ).arg( s.toLower() ) );
				watcher.progressTextChanged( tr( u8"!loadModFile: find not found: %1" ).arg( fileName.toLower() ) );
				return;
			}
			loadMod( *it );
		};

		// plugins.txtに実際に読み込むプラグインが記載されている
		if( config.targetGameTitle == EGameTitle::FalloutNV ) {
			fs::readAllLines( fileDB.mo2ProfilePath() + "/plugins.txt", Encoding::UTF8, [&]( const QString& s ) {
				if( s.isEmpty() )return;
				if( s[ 0 ] == '#' )return;

				load( s );
			} );
		}
		else {
			QStringList disableModName;
			fs::readAllLines( fileDB.mo2ProfilePath() + "/plugins.txt", Encoding::UTF8, [&]( const QString& s ) {
				if( s.isEmpty() )return;
				if( s[ 0 ] == '#' )return;
				if( s[ 0 ] == '*' ) return;
				disableModName << s;
			} );

			auto loadorder = fileDB.mo2ProfilePath() + "/loadorder.txt";

			fs::readAllLines( loadorder, Encoding::UTF8, [&]( const QString& s ) {
				if( s.isEmpty() )return;
				if( s[ 0 ] == '#' )return;

				if( 0 <= disableModName.indexOf( s ) )return;

				load( s );
			} );
		}
	}


	/////////////////////////////////////////
	void applyDuplicationColor() {
		tesSys.enumerateForm( [&]( const QList<TESForm*>& fmList ) {
			//int n = tesSys.formStatus( form->ID );

			if( fmList.length() < 2 )return;

			for( auto* fm : fmList ) {
				if( !fm->item )continue;
				fm->item->setBackgroundColor( 0, "#FFFFD9" );
				fm->item->setBackgroundColor( 1, "#FFFFD9" );
				fm->item->setBackgroundColor( 2, "#FFFFD9" );
			}
		} );
		HLogView::info( "applyDuplicationColor" );
	}


	/////////////////////////////////////////
	void selectModRoot( const QString& modFilePath ) {
		qDebug() << "search: " << modFilePath;

		for( auto& it : treeWidgetMod->topLevelItems<ItemModFormID>() ) {
			if( it->m_formData->file->fullPath() == modFilePath ) {
				auto* tab = (QWidget*)self->parent()->parent();
				auto idx = qtWindow->tabWidget()->indexOf( tab );
				qtWindow->tabWidget()->setCurrentIndex( idx );

				treeWidgetMod->setItemSelected( it ,true);
				treeWidgetMod->scrollToItem(it);
			}
			qDebug() << it->m_formData->file->fullPath();
		}
	}
};



//////////////////////////////////////////////////////////////////////////////////
UIModListView::UIModListView( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		$::restoreState( impl->treeWidgetMod->header(), config.modLeftViewHeader );
		impl->setup();
	} );
}


/////////////////////////////////////////
UIModListView::~UIModListView() {
	$::saveState( config.modLeftViewHeader, impl->treeWidgetMod->header() );
}


/////////////////////////////////////////
ItemModFormID* UIModListView::currentItem() {
	return impl->treeWidgetMod->currentItem<ItemModFormID>();
}

/////////////////////////////////////////
void UIModListView::selectModRoot( const QString& modFilePath ) {
	impl->selectModRoot( modFilePath );
}
