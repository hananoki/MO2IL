#include "UIVirtualFileView.h"
#include "ui_UIVirtualFileView.h"

#include "UIMainWindow.h"
#include "VFileSys.h"


#include "ItemModFile.h"


//////////////////////////////////////////////////////////////////////////////////
class UIVirtualFileView::Impl : public Ui_UIVirtualFileView {
public:
	UIVirtualFileView* self;


	

	/////////////////////////////////////////
	Impl( UIVirtualFileView* _self ) :
		self( _self ) {

		setupUi( self );
	}


	/////////////////////////////////////////
	void setup() {
		// 仮想ファイルの初期化が完了した時
		connect( &fileDB, &VFileSys::signal_initCompleted, [&]() {
			treeWidget->clear();
			QList<QString> dirList;
			QList<QTreeWidgetItem*> fileList;
			fileDB.enumerateFiles( [&]( const QString& folder, const QString& m_file, const QString& fullName ) {
				if( folder.isEmpty() ) {
					auto* a = new ItemFile( fullName );

					fileList << a;
				}
				else {
					auto dirs = folder.split( "/" );
					if( 0 <= dirList.indexOf( dirs.first() ) )return;

					QTreeWidgetItem* a = new QTreeWidgetItem();
					a->setText( 0, dirs.first() );
					a->setIcon( 0, icon::get( environment::currentDirectory() ) );

					dirList << dirs.first();
					treeWidget->addTopLevelItem( a );
				}
			} );
			treeWidget->addTopLevelItems( fileList );
		} );

		/// アイテムの選択切り替え
		$TreeWidget::itemSelectionChanged( treeWidget, [&]() {
			auto* item = treeWidget->currentItem<ItemFile>();
			if( !item )return;

			
			emit self->signal_itemSelectionChanged( item );
		} );

		// ダブルクリック
		$TreeWidget::itemDoubleClicked( treeWidget, [&]( QTreeWidgetItem* item, int column ) {
			auto* fitem = treeWidget->currentItem<ItemFile>();
			if( fitem ) {
				QProcess p;
				p.setWorkingDirectory( environment::currentDirectory() );
				p.startDetached( config.bsaBrowserPath, { path::separatorToOS( fitem->fullPath ) } );
			}
		} );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIVirtualFileView::UIVirtualFileView( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		//$::restoreState( impl->treeWidget->header(), config.fileTreeHeader );
		impl->setup();
	} );
}

UIVirtualFileView::~UIVirtualFileView() {
}
