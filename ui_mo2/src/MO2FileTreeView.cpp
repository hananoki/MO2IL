#include "MO2FileTreeView.h"
#include "ItemMO2View.h"
#include "ItemFile.h"
#include "ItemFolder.h"
#include "UIMO2ViewFile.h"

#include "ContextMenu.h"


//////////////////////////////////////////////////////////////////////////////////
class MO2FileTreeView::Impl {
public:
	QFuture<void> future;
	QFutureWatcher<void> watcher;

	MO2FileTreeView* self;
	UIMO2ViewFile* parent;

	std::unique_ptr<ContextMenu> contextMenuFile;
	std::unique_ptr<ContextMenu> contextMenuFolder;


	/////////////////////////////////////////
	Impl( MO2FileTreeView* _self, QWidget* _parent ) :
		self( _self ),
		parent( (UIMO2ViewFile*) _parent ->parent() ) {

		self->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );

		connect( self, &MO2FileTreeView::signal_addItem, self, std::bind( &HTreeWidget::addTopLevelItem, self, std::placeholders::_1 ) );
		connect( self, &MO2FileTreeView::signal_addChild, self, [&]( QTreeWidgetItem* item, QTreeWidgetItem* item2 ) {
			item->addChild( item2 );
		} );
		connect( self, &MO2FileTreeView::signal_clearItems, self, std::bind( &HTreeWidget::clear, self ) );


		utils::createFileContextMenu( &contextMenuFile, self );
		utils::createFolderContextMenu( &contextMenuFolder, self );

		$TreeWidget::itemDoubleClicked( self, std::bind( &Impl::_itemDoubleClicked, this ) );
	}


	/////////////////////////////////////////
	/// ダブルクリック
	void _itemDoubleClicked() {
		auto* item = self->currentItem<ItemFile>();
		if( !item ) return;
		$::showInExplorer( item->fullPath );
	}


	/////////////////////////////////////////
	QList<QTreeWidgetItem*> mkdir( QString folderPath ) {
		QList<QTreeWidgetItem*> result;

		fs::enumerateDirectories( folderPath, "*", SearchOption::TopDirectoryOnly, [&]( const QString& s ) {
			auto* item = new ItemFolder( s );
			item->addChildren( mkdir( s ) );
			result << item;
			return true;
		} );

		fs::enumerateFiles( folderPath, "*", SearchOption::TopDirectoryOnly, [&]( const QString& s ) {
			auto* item = new ItemFile( s );
			result << item;
			return true;
		} );

		return result;
	}


	/////////////////////////////////////////
	void open( const QString& folderPath ) {
		if( future.isRunning() ) {
			watcher.waitForFinished();
		}

		if( !fs::isExistDirectory( folderPath ) )return;

		future = QtConcurrent::run( [&]() {
			emit self->signal_clearItems();

			bool findPreview = false;
			QList<ItemFolder*> dlist;
			fs::enumerateDirectories( folderPath, "*", SearchOption::TopDirectoryOnly, [&]( const QString& s ) {
				auto* item = new ItemFolder( s );
				dlist << item;
				emit self->signal_addItem( item );
				return true;
			} );

			fs::enumerateFiles( folderPath, "*", SearchOption::TopDirectoryOnly, [&]( const QString& s ) {
				auto* item = new ItemFile( s );
				emit self->signal_addItem( item );

				QMimeDatabase mimeDatabase;
				QMimeType t = mimeDatabase.mimeTypeForFile( item->fullPath );
				if( t.name().contains( "image" ) ) {
					findPreview = true;
					emit parent->signal_previewItem( item );
				}

				return true;
			} );

			if( !findPreview ) {
				emit parent->signal_hideFlame();
			}

			for( auto& d : dlist ) {
				for( auto& c : mkdir( d->fullPath ) ) {
					emit self->signal_addChild( d, c );
				}
			}

		} );
		watcher.setFuture( future );
	}
};



//////////////////////////////////////////////////////////////////////////////////
MO2FileTreeView::MO2FileTreeView( QWidget* parent ) :
	HTreeWidget( parent ),
	impl( new Impl( this, parent ) ) {

}


/////////////////////////////////////////
void MO2FileTreeView::open( const QString& folderPath ) {
	impl->open( folderPath );
}
