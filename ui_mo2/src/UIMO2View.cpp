#include "UIMO2View.h"
#include "ui_UIMO2View.h"

#include "UIMainWindow.h"

#include "ItemMO2View.h"
#include "ItemFile.h"
#include "ItemFolder.h"
#include "UIToolBar.h"
#include "ContextMenu.h"

//////////////////////////////////////////////////////////////////////////////////
class UIMO2View::Impl : public Ui_UIMO2View {
public:
	UIMO2View* self;

	QFuture<void> future;
	QFutureWatcher<void> watcher;

	std::unique_ptr<ContextMenu> contextMenuFolder;

	QList<ItemFolderMO2View*> modItems;
	QList<ItemMO2Group*> modGroups;


	/////////////////////////////////////////
	Impl( UIMO2View* _self ) :
		self( _self ) {

		setupUi( self );
		treeWidget->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );

		tabWidget->setTabVisible( 1, false );
		tabWidget->setTabVisible( 2, false );
		tabWidget->setTabVisible( 3, false );

		treeWidget->onMousePressEvent = [&]( QMouseEvent* e ) {
			auto* item = treeWidget->currentItem<ItemFolderMO2View>();
			if( item ) {
				if( e->button() == Qt::LeftButton ) {
					//$::showInExplorer( item->folderPath );
					auto* mimeData = new QMimeData;
					QList<QUrl> urls;
					urls << QUrl::fromLocalFile( item->fullPath );
					mimeData->setUrls( urls );
					//mimeData->setText( item->folderPath );
					auto* drag = new QDrag( treeWidget );
					drag->setMimeData( mimeData );
					drag->exec( Qt::CopyAction );
				}
				//return true;
			}
			return false;
		};
	}


	/////////////////////////////////////////
	void setup() {
		utils::createFolderContextMenu( &contextMenuFolder, treeWidget );

		$CheckBox::stateChanged( checkBox, std::bind( &Impl::_stateChanged, this, std::placeholders::_1 ) );

		connect( self, &UIMO2View::signal_addItem, self, std::bind( &HTreeWidget::addTopLevelItem, treeWidget, std::placeholders::_1 ) );

		connect( self, &UIMO2View::signal_makeTreeComplete, self, std::bind( &Impl::_makeTreeComplete, this ) );

		/////////////////////////////////////////
		/// アイテムの選択切り替え
		$TreeWidget::itemSelectionChanged( treeWidget, [&]() {
			auto* item = treeWidget->currentItem<QTreeWidgetItem>();

			if( dynamic_cast<ItemFolderMO2View*>( item ) ) {
				emit self->signal_itemSelectionChanged( (ItemFolderMO2View*) item );
			}
		} );

		connect( self,
			&UIMO2View::signal_itemSelectionChanged,
			std::bind( &Impl::_itemSelectionChanged, this, std::placeholders::_1 ) );

		connect(
			qtWindow->toolBar(),
			&UIToolBar::signal_refresh,
			std::bind( &Impl::_refresh, this ) );

		$LineEdit::textChanged(
			lineEdit,
			std::bind( &Impl::incrementalSearch, this, std::placeholders::_1 ) );

		lineEdit->setClearButtonEnabled( true );

		makeTree();
	}


	/////////////////////////////////////////
	void _makeTreeComplete() {
		treeWidget->setExpandItemPaths( utils::expandTree() );
		applyFilter();
	}


	/////////////////////////////////////////
	void _stateChanged( int state ) {
		applyFilter();
	}


	/////////////////////////////////////////
	void _refresh() {
		treeWidget->clear();
		makeTree();
	}


	/////////////////////////////////////////
	void _itemSelectionChanged( ItemFolderMO2View* item ) {
		tabWidget->setTabVisible( 0, true );

		auto fsond = item->fullPath + "/build.csv";
		tabWidget->setTabVisible( 1, fs::isExistFile( fsond ) );

		mo2viewBuild->initView( fsond );

		auto merge = item->fullPath + "/merge.txt";
		tabWidget->setTabVisible( 2, fs::isExistFile( merge ) );
		mo2Merge->initView();

		tabWidget->setTabVisible( 3, item->mode == 1 );

	}


	/////////////////////////////////////////
	void incrementalSearch( QString text ) {
		applyFilter();
	}


	/////////////////////////////////////////
	void applyFilter() {
		QString text = lineEdit->text();
		bool enableOnly = checkBox->checkState() == Qt::Unchecked ? false : true;

		auto ss2 = text.toLower();
		for( auto& p : modItems ) {
			if( text.isEmpty() ) {
				p->setHidden( false );
			}
			else {
				//if( p->text( 0 ).contains( "Heartland" ) ) {
				//}
				auto ss1 = p->text( 0 ).toLower();
				p->setHidden( !ss1.contains( ss2 ) );
			}

			if( !enableOnly ) continue;

			if( p->checkState( 0 ) == Qt::Unchecked ) {
				p->setHidden( true );
			}
		}

		for( auto& p : modGroups ) {
			bool hide = true;
			for( auto& c : p->childItems<ItemFolderMO2View>() ) {
				if( !c->isHidden() ) {
					hide = false;
					break;
				}
			}
			p->setHidden( hide );
		}
	}


	/////////////////////////////////////////
	void makeTree() {
		treeWidget->clear();
		modItems.clear();
		modGroups.clear();

		future = QtConcurrent::run( [&]() {
			{
				auto installPath = utils::installGamePath();
				if( !installPath.isEmpty() ) {
					auto dataPath = utils::installGamePath() + "/Data";
					if( fs::isExistDirectory( dataPath ) ) {
						auto* data = new ItemFolderMO2View( dataPath, true, true );
						modItems << data;
						emit self->signal_addItem( data );
					}
					else {
						HLogView::error( dataPath + "が見つかりませんでした" );
					}
				}
				else {
					HLogView::error( "ゲームインストールパスが見つかりませんでした" );
				}

				auto ovPath = config.getMO2Path() + "/overwrite";
				if( fs::isExistDirectory( ovPath ) ) {
					auto* data2 = new ItemFolderMO2View( config.getMO2Path() + "/overwrite", true, true );
					modItems << data2;
					emit self->signal_addItem( data2 );
				}
				else {
					HLogView::error( ovPath + "が見つかりませんでした" );
				}

				if( config.targetGameTitle == EGameTitle::FalloutNV ) {
					if( fs::isExistDirectory( config.fonvXMLPath ) ) {
						auto* data2 = new ItemFolderMO2View( config.fonvXMLPath, 1 );
						modItems << data2;
						emit self->signal_addItem( data2 );
					}
				}
			}
			auto mo2Path = config.getMO2Path();
			auto profileName = config.seleectMO2ProfileName;
			auto modFolderPath = $$( "%1/mods/" ).arg( mo2Path );
			auto modListTxtFilePath = $$( "%1/profiles/%2/modlist.txt" ).arg( mo2Path ).arg( profileName );

			ItemMO2Group* groupItem = nullptr;
			QList<QTreeWidgetItem*> lst;
			auto mods = fs::readAllLines( modListTxtFilePath );
			auto rst = mods.rbegin();
			auto red = mods.rend();
			for( ; rst != red; rst++ ) {
				auto& s = ( *rst );
				if( s[ 0 ] == '#' )continue;

				if( s.contains( "_separator" ) ) {
					if( groupItem ) {
						emit self->signal_addItem( groupItem );
					}
					groupItem = new ItemMO2Group( $$( s ).remove( 0, 1 ) );
					modGroups << groupItem;
					continue;
				}

				if( s[ 0 ] == '*' ) continue;

				bool check = false;
				//if( s[ 0 ] == '-' ) continue;
				if( s[ 0 ] == '+' ) {
					check = true;
				}
				auto folderPath = config.getMO2Path() + "/mods/" + $$( s ).remove( 0, 1 );
				auto* p = new ItemFolderMO2View( folderPath, check );
				modItems << p;
				if( groupItem ) {
					groupItem->addChild( p );
				}
				else {
					emit self->signal_addItem( p );
				}
			}

			if( groupItem ) {
				emit self->signal_addItem( groupItem );
			}

			emit self->signal_makeTreeComplete();
		} );

		watcher.setFuture( future );
	}


	/////////////////////////////////////////
	QList<QTreeWidgetItem*> mkdir( QString folderPath ) {
		QList<QTreeWidgetItem*> lst;

		fs::enumerateDirectories( folderPath, "*", SearchOption::TopDirectoryOnly, [&]( const QString& s ) {
			auto* a = new QTreeWidgetItem();
			a->setText( 0, path::getBaseName( s ) );
			a->setIcon( 0, icon::get( s ) );
			auto child = mkdir( s );
			a->addChildren( child );
			lst << a;
			return true;
		} );

		fs::enumerateFiles( folderPath, "*", SearchOption::TopDirectoryOnly, [&]( const QString& s ) {
			lst << new ItemFile( s );
			return true;
		} );

		return lst;
	}


	/////////////////////////////////////////
	QList<ItemFolderMO2View*> modFolderItemByChecked() {
		using namespace cpplinq;

		return from( modItems )
			>> where( [&]( auto* p )->bool { return p->checkState( 0 ) != Qt::Unchecked; } )
			>> to_qlist();
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIMO2View::UIMO2View( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		$::restoreState( impl->splitter, config.mo2ViewSplitter );
		$::restoreState( impl->treeWidget->header(), config.mo2ViewHeaderL );
		impl->setup();
	} );
}


/////////////////////////////////////////
UIMO2View::~UIMO2View() {
	$::saveState( config.mo2ViewHeaderL, impl->treeWidget->header() );
	$::saveState( config.mo2ViewSplitter, impl->splitter );
	utils::expandTree() = impl->treeWidget->getExpandItemPaths();
}


/////////////////////////////////////////
HTreeWidget* UIMO2View::treeView() {
	return impl->treeWidget;
}


/////////////////////////////////////////
UIMO2ViewFile* UIMO2View::mo2File() {
	return impl->mo2File;
}


/////////////////////////////////////////
QList<ItemFolderMO2View*> UIMO2View::modFolderItemByChecked() {
	return impl->modFolderItemByChecked();
}


/////////////////////////////////////////
/// @brief  イベントフィルター
//bool UIMO2View::eventFilter( QObject* obj, QEvent* event ) {
//
//	//if( event->type() == QEvent::Drop ) {
//	//	QDropEvent* ee = (QDropEvent*) event;
//	//	if( !ee->mimeData()->hasUrls() ) return false;
//
//	//	//return impl->dropEvent( ee );
//	//}
//
//	//if( event->type() == QEvent::DragEnter ) {
//	//	QDragEnterEvent* ee = (QDragEnterEvent*) event;
//	//	ee->acceptProposedAction();
//	//	return true;
//	//}
//	if( event->type() != QEvent::HoverMove )
//		qDebug() << event->type();
//	if( event->type() == QEvent::MouseButtonPress ) {
//		QMouseEvent* ee = (QMouseEvent*) event;
//		//ee->acceptProposedAction();
//		return true;
//	}
//	return false;
//}
