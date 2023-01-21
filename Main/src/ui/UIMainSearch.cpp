#include "UIMainSearch.h"
#include "ui_UIMainSearch.h"

#include "UIMainWindow.h"
#include <QProgressDialog>
#include "ItemMO2View.h"
#include "ItemFile.h"
#include "ContextMenu.h"

//////////////////////////////////////////////////////////////////////////////////
class UIMainSearch::Impl : public Ui_UIMainSearch {
public:
	UIMainSearch* self;

	QFuture<void> future;
	QFutureWatcher<void> watcher;
	std::unique_ptr<QProgressDialog> pd;

	std::unique_ptr<ContextMenu> contextMenu;

	QString folderPath;

	/////////////////////////////////////////
	Impl( UIMainSearch* _self ) :
		self( _self ) {

		setupUi( self );

		treeWidget->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );

		

		/////////////////////////////////////////
		connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			HLogView::info( u8"検索が完了しました" );
			pd->setValue( pd->maximum() );
		} );

		/////////////////////////////////////////
		connect( &watcher, &QFutureWatcher<void>::progressTextChanged, self, [&]( const QString& text ) {
			pd->setLabelText( text );
		} );

		/////////////////////////////////////////
		connect( &watcher, &QFutureWatcher<void>::progressValueChanged, self, [&]( int progressValue ) {
			pd->setValue( progressValue );
		} );
	}


	/////////////////////////////////////////
	void setup() {
		utils::createFileContextMenu( &contextMenu, treeWidget );

		/////////////////////////////////////////
		// アイテム追加
		connect(
			self,
			&UIMainSearch::signal_addItem,
			self,
			std::bind( &HTreeWidget::addTopLevelItem, treeWidget, std::placeholders::_1 ) );

		$TreeWidget::itemDoubleClicked(
			treeWidget,
			std::bind( &Impl::_itemDoubleClicked, this ) );

		$PushButton::click(
			pushButton,
			std::bind( &Impl::on_button, this ) );

		connect(
			lineEdit,
			&QLineEdit::returnPressed,
			std::bind( &Impl::on_button, this ) );
	}


	/////////////////////////////////////////
	/// ダブルクリック
	void _itemDoubleClicked() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( !item ) return;
		$::showInExplorer( item->fullPath );
	}


	/////////////////////////////////////////
	void searchBSA( const QString& bsaPath, const QString& rmn ) {
		auto t = lineEdit->text().toLower();

		auto files = utils::bsaExtractedFileList( bsaPath );

		QTreeWidgetItem* rt = new QTreeWidgetItem();
		rt->setText( 0, $$( bsaPath ).replace( rmn, "" ) );
		rt->setIcon( 0, icon::get( bsaPath ) );

		for( auto& fpath : files ) {
			auto ff = /*path::getFileName(*/ fpath.replace( "\\", "/" ) /*)*/;
			if( ff.contains( t ) ) {
				QTreeWidgetItem* item = new QTreeWidgetItem();
				item->setText( 0, fpath.replace( "\\", "/" ) );
				rt->addChild( item );
			}
		}
		if( rt->childCount() == 0 )return;
		emit self->signal_addItem( rt );
	}


	/////////////////////////////////////////
	void searchFolder( const QString& folderPath ) {
		auto t = lineEdit->text().toLower();

		auto mo2Path = config.getMO2Path();
		auto modsPath = $$( "%1/mods/" ).arg( mo2Path );

		QTreeWidgetItem* rt = new QTreeWidgetItem();
		auto a = $$( folderPath ).replace( modsPath, "" );
		rt->setText( 0, a );
		rt->setIcon( 0, icon::get( folderPath ) );

		auto modFolderPath = $$( "%1/" ).arg( folderPath );

		for( auto& f : fs::getFiles( folderPath, "*", SearchOption::AllDirectories ) ) {
			auto fpath = $$( f ).replace( modFolderPath, "" );
			auto fpathl =/* path::getFileName( */fpath/* )*/.toLower();
			if( fpathl.contains( t ) ) {
				rt->addChild( new ItemFile( f, 0, modFolderPath ) );
			}
		}

		if( rt->childCount() == 0 )return;
		emit self->signal_addItem( rt );
	}


	/////////////////////////////////////////
	void on_button() {
		if( lineEdit->text().isEmpty() ) {
			HLogView::warning( u8"検索する文字列が空です" );
			return;
		}
		HLogView::info( u8"検索を開始します" );

		pd = std::make_unique<QProgressDialog>( self );
		pd->setModal( true );

		auto sysBSA = fs::getFiles( utils::installGamePath() + "/Data", "*.bsa", SearchOption::TopDirectoryOnly );
		auto modsFolders = fs::getDirectories( $$( "%1/mods" ).arg( config.getMO2Path() ), "*", SearchOption::TopDirectoryOnly );

		pd->setRange( 0, modsFolders.length() + sysBSA.length() );
		pd->show();

		treeWidget->clear();

		future = QtConcurrent::run( [&, sysBSA, modsFolders]() {
			auto t = lineEdit->text();
			auto gData = utils::installGamePath() + "/Data";
			auto sysBSA = fs::getFiles( gData, "*.bsa", SearchOption::TopDirectoryOnly );
			int i = 0;
			gData = utils::installGamePath() + "/";
			for( auto& f : sysBSA ) {
				searchBSA( f, gData );
				watcher.progressValueChanged( i );
				watcher.progressTextChanged( path::getFileName( f ) );
				i++;
			}

			auto rnm = $$( "%1/mods/" ).arg( config.getMO2Path() );
			for( auto& d : modsFolders ) {
				for( auto& f : fs::getFiles( d, "*.bsa", SearchOption::TopDirectoryOnly ) ) {
					searchBSA( f, rnm );
				}
				searchFolder( d );
				watcher.progressValueChanged( i );
				watcher.progressTextChanged( path::getFileName( d ) );
				i++;
			}
		} );
		watcher.setFuture( future );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIMainSearch::UIMainSearch( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		impl->setup();
	} );
}


/////////////////////////////////////////
UIMainSearch::~UIMainSearch() {
}
