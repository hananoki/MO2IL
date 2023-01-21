#include "UIMO2ViewMarge.h"
#include "ui_UIMO2ViewMarge.h"

#include "ui/UIMainWindow.h"
#include "ItemMO2View.h"
#include "ItemFile.h"
#include "ItemFolder.h"
#include "ActionFile.h"
#include "ContextMenu.h"
#include <QProgressDialog>

using FF = std::tuple<QString, QString>;

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewMarge::Impl : public Ui_UIMO2ViewMarge {
public:
	using self_t = UIMO2ViewMarge;
	self_t* self;

	QFuture<void> future1;
	QFutureWatcher<void> watcher1;

	QFuture<void> future;
	QFutureWatcher<void> watcher;
	std::unique_ptr<QProgressDialog> pd;

	std::unique_ptr<ContextMenu> contextMenu;

	QStringList dLog;

	int fileCount;
	int fileProgress;
	QMap<QString, QString> mapping;


	/////////////////////////////////////////
	Impl( self_t* _self ) :
		self( _self ) {

		setupUi( self );
		treeWidget->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );

		pushButton_3->setIcon( ICON_EXEC );

		connect( &watcher, &QFutureWatcher<void>::progressValueChanged, self, [&]( int progressValue ) {
			pd->setValue( progressValue );
		} );
		connect( &watcher, &QFutureWatcher<void>::progressRangeChanged, self, [&]( int mn, int mm ) {
			pd->setRange( mn, mm );
		} );
		connect( &watcher, &QFutureWatcher<void>::progressTextChanged, self, [&]( QString s ) {
			pd->setLabelText( s );
		} );
		connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			HLogView::info( u8"処理が完了しました" );
			pd->setValue( pd->maximum() );
		} );
	}


	/////////////////////////////////////////
	void setup() {
		utils::createFileContextMenu( &contextMenu, treeWidget );

		$TreeWidget::itemDoubleClicked( treeWidget, std::bind( &Impl::_itemDoubleClicked, this ) );

		connect( self, &self_t::signal_addItem, self, std::bind( &HTreeWidget::addTopLevelItem, treeWidget, std::placeholders::_1 ) );

		$PushButton::click( pushButton_3, std::bind( &Impl::on_button, this ) );
	}


	/////////////////////////////////////////
	/// ダブルクリック
	void _itemDoubleClicked() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( !item ) return;
		$::showInExplorer( item->fullPath );
	}

	struct Execute {
		//Execute( UIMain* p ) : self( p ) {}
		//UIMain* self;
		void operator()( const FF& x ) {
			fs::cp( std::get<0>( x ), std::get<1>( x ) );
			//x->func();
		}
	};


	/////////////////////////////////////////
	void initView() {
		//using namespace cpplinq;

		treeWidget->clear();
		mapping.clear();

		future1 = QtConcurrent::run( [&]() {
			auto mo2Path = config.getMO2Path();
			auto modFolderPath = $$( "%1/mods/" ).arg( mo2Path );
			auto* gg = qtWindow->mo2ViewTreeView()->currentItem<ItemFolderMO2View>();

			// 必要なファイルをかき集める
			for( auto& s : fs::readAllLines( gg->fullPath + "/marge.txt" ) ) {
				if( s.isEmpty() ) continue;

				auto folderPath = modFolderPath + s;

				for( auto& fname : fs::getFiles( folderPath, "*", SearchOption::AllDirectories ) ) {
					auto fpath = $$( fname ).replace( folderPath + "/", "" ).toLower();
					auto it = mapping.find( fpath );
					if( it == mapping.constEnd() ) {
						mapping.insert( fpath, fname );
					}
					else {
						( *it ) = fname;
					}
				}
			}

			// フォルダのアイテム構築
			QList<QTreeWidgetItem*> lst;
			for( auto& k : mapping.keys() ) {
				auto paths = k.split( "/" );
				if( paths.length() == 1 )continue;

				auto* item = $TreeWidget::findItem<HTreeWidgetItem>( lst, paths[ 0 ] );
				if( !item ) {
					item = new HTreeWidgetItem();
					item->setText( 0, paths[ 0 ] );
					item->setIcon( 0, icon::get( config.getMO2Path() ) );
					lst << item;
				}
				paths.takeFirst();
				mkTree( item, paths, mapping[ k ] );
			}

			for( auto& p : lst ) {
				emit self->signal_addItem( p );
			}

			// ファイルのアイテム構築
			for( auto& k : mapping.keys() ) {
				auto paths = k.split( "/" );
				if( paths.length() == 1 ) {
					auto* p = new ItemGroupViewFile( mapping[ k ] );
					//treeWidget->addTopLevelItem( p );
					emit self->signal_addItem( p );
				}
			}

			fileCount = mapping.keys().length();
		} );
		watcher1.setFuture( future1 );
	}


	/////////////////////////////////////////
	void mkTree( HTreeWidgetItem* parent, QStringList paths, const QString& filePath ) {
		if( paths.length() == 1 ) {
			auto* p = new ItemGroupViewFile( filePath );
			parent->addChild( p );
			return;
		}
		auto* item = parent->find<HTreeWidgetItem>( paths[ 0 ] );
		if( !item ) {
			item = new HTreeWidgetItem();
			item->setText( 0, paths[ 0 ] );
			item->setIcon( 0, icon::get( config.getMO2Path() ) );
			parent->addChild( item );
		}
		paths.takeFirst();
		mkTree( item, paths, filePath );
	}


	QList<FF> fileCPLiat;

	/////////////////////////////////////////
	void on_button() {
		dLog.clear();

		pd = std::make_unique<QProgressDialog>( self );
		pd->setModal( true );
		pd->setRange( 0, fileCount );
		pd->show();

		fileCPLiat.clear();
		auto* ggg = qtWindow->mo2ViewTreeView()->currentItem<ItemFolderMO2View>();
		for( auto& k : mapping.keys() ) {
			auto fullPath = mapping[ k ];
			auto paths = $$( fullPath ).replace( config.getMO2Path() + "/mods/", "" ).split( "/" );
			paths.takeFirst();
			fileCPLiat << FF{
				mapping[ k ],
				ggg->fullPath + "/" + paths.join( "/" )
			};
		}
		future = QtConcurrent::map( fileCPLiat, Execute() );

		/*	future = QtConcurrent::run( [&]() {
				auto* gg = qtWindow->mo2ViewTreeView()->currentItem<ItemMO2ViewMod>();

				fileProgress = 0;

				for( auto& k : mapping.keys() ) {
					auto fullPath = mapping[ k ];
					auto src = fullPath;
					auto paths = $$( fullPath ).replace( config.getMO2Path() + "/mods/", "" ).split( "/" );
					paths.takeFirst();
					auto dst = gg->folderPath + "/" + paths.join( "/" );
					fs::cp( src, dst );
					dLog << $$( "%1\t%2" ).arg( src ).arg( dst );

					watcher.progressTextChanged( paths.join( "/" ) );
					watcher.progressValueChanged( fileProgress );
					fileProgress++;
				}
			} );*/
		watcher.setFuture( future );
	}


	/////////////////////////////////////////
	void travaseFolder( const QString& tt, HTreeWidgetItem* item ) {
		for( auto* item : item->childItems<HTreeWidgetItem>() ) {
			auto* f = dynamic_cast<ItemGroupViewFile*>( item );
			if( f ) {
				copyFile( tt, f );
			}
			else {
				travaseFolder( tt, item );
			}
		}
	}


	/////////////////////////////////////////
	void copyFile( const QString& tt, ItemGroupViewFile* f ) {
		auto src = f->fullPath;
		auto paths = $$( f->fullPath ).replace( config.getMO2Path() + "/mods/", "" ).split( "/" );
		paths.takeFirst();
		auto dst = tt + "/" + paths.join( "/" );
		fs::cp( src, dst );
		dLog << $$( "%1\t%2" ).arg( src ).arg( dst );

		watcher.progressTextChanged( paths.join( "/" ) );
		watcher.progressValueChanged( fileProgress );
		fileProgress++;
	}

};


//////////////////////////////////////////////////////////////////////////////////
UIMO2ViewMarge::UIMO2ViewMarge( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		$::restoreState( impl->treeWidget->header(), config.UIMO2Marge_Header );
		impl->setup();
	} );
}


/////////////////////////////////////////
UIMO2ViewMarge::~UIMO2ViewMarge() {
	$::saveState( config.UIMO2Marge_Header, impl->treeWidget->header() );
}


/////////////////////////////////////////
void UIMO2ViewMarge::initView() {
	impl->initView();
}
