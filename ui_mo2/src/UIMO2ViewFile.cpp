#include "UIMO2ViewFile.h"
#include "ui_UIMO2ViewFile.h"

#include "UIMainWindow.h"
#include "ItemMO2View.h"
#include "ItemFile.h"
#include "ItemFolder.h"
#include "UIMO2View.h"
#include "App.h"
#include "FojpUtility.h"

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewFile::Impl : public Ui_UIMO2ViewFile {
public:
	using self_t = UIMO2ViewFile;
	self_t* self;

	QString folderPath;

	int previewCount;
	QHash<int, QLabel*> previewLabel;

	QList<QWidget*> buttons;


	/////////////////////////////////////////
	Impl( self_t* _self ) :
		self( _self ) {

		setupUi( self );

		toolBar->hide();
		frame->hide();
	}


	/////////////////////////////////////////
	void setup() {
		hideButtons();

		connect( self, &self_t::signal_previewItem, self, std::bind( &Impl::_previewItem, this, std::placeholders::_1 ) );
		connect( self, &self_t::signal_hideFlame, self, std::bind( &Impl::_hideFlame, this ) );

		$TreeWidget::itemSelectionChanged(
			treeWidget,
			std::bind( &Impl::_itemSelectionChanged, this ) );

		connect(
			qtWindow->mo2View(),
			&UIMO2View::signal_itemSelectionChanged,
			std::bind( &Impl::_signal_itemSelectionChanged, this, std::placeholders::_1 ) );

		$Action::triggered(
			actionFOJP, std::bind( &Impl::_on_fojp, this ) );
	}


	/////////////////////////////////////////
	void _previewItem( ItemFile* item ) {
		auto it = previewLabel.find( previewCount );
		if( it == previewLabel.constEnd() ) {
			previewLabel.insert( previewCount, new QLabel( frame ) );
			it = previewLabel.find( previewCount );
			verticalLayout->addWidget( *it );
		}

		QImage img( item->fullPath );

		( *it )->setPixmap( QPixmap::fromImage( img ).scaled(
			qMin( 384, self->width() ),
			qMin( 384, self->height() ),
			Qt::KeepAspectRatio ) );

		item->previewNo = previewCount;

		frame->show();
		( *it )->show();

		previewCount++;
	}


	/////////////////////////////////////////
	void _hideFlame() {
		frame->hide();
	}


	/////////////////////////////////////////
	void _on_fojp() {
		g_fojpUtility.convert();
	}


	/////////////////////////////////////////
	void _signal_itemSelectionChanged( ItemFolderMO2View* item ) {
		toolBar->setVisible( item->mode == 1 );

		initView( item->fullPath );
	}


	/////////////////////////////////////////
	/// アイテムの選択切り替え
	void _itemSelectionChanged() {
		auto* item = treeWidget->currentItem<ItemFile>();

		if( item ) {
			QMimeDatabase mimeDatabase;
			QMimeType t = mimeDatabase.mimeTypeForFile( item->fullPath );

			if( t.name().contains( "image" ) ) {
				bool find = false;
				for( auto& k : previewLabel.keys() ) {
					if( k == item->previewNo ) {
						find = true;
					}
					previewLabel[ k ]->setVisible( k == item->previewNo );
				}
				if( !find ) {
					_previewItem( item );
				}
				frame->show();
			}
			else {
				frame->hide();
			}
		}
	}


	/////////////////////////////////////////
	void hideButtons() {
		for( auto& p : buttons ) p->hide();
	}


	/////////////////////////////////////////
	void initView( const QString& _folderPath ) {
		previewCount = 0;

		for( auto& k : previewLabel.keys() ) {
			previewLabel[ k ]->hide();
		}
		folderPath = _folderPath;
		treeWidget->open( _folderPath );
	}


	/////////////////////////////////////////
	void on_button() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			//$::showInExplorer( path::getDirectoryName( item->full->file->fullPath() ) );
			QProcess p;
			p.setWorkingDirectory( environment::currentDirectory() );
			p.startDetached( config.espToolPath, { path::separatorToOS( item->fullPath ) } );
		}
	}


	/////////////////////////////////////////
	void on_delete() {
		for( auto& p : treeWidget->currentItems<ItemFile>() ) {
			fs::moveToTrash( p->fullPath );
		}
		initView( folderPath );
	}

};


//////////////////////////////////////////////////////////////////////////////////
UIMO2ViewFile::UIMO2ViewFile( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		$::restoreState( impl->treeWidget->header(), config.UIMO2File_Header );
	impl->setup();
	} );
}


/////////////////////////////////////////
UIMO2ViewFile::~UIMO2ViewFile() {
	$::saveState( config.UIMO2File_Header, impl->treeWidget->header() );
}
