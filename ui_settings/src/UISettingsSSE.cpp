#include "UISettingsSSE.h"
#include "ui_UISettingsSSE.h"

//////////////////////////////////////////////////////////////////////////////////
class UISettingsSSE::Impl : public Ui_UISettingsSSE {
public:

	UISettingsSSE* self;

	/////////////////////////////////////////
	Impl( UISettingsSSE* _self ) :
		self( _self ) {

		setupUi( self );

		listWidget->installEventFilter( self );

		for( auto& s : config.ssePapyrusImportPaths ) {
			auto* item = new QListWidgetItem( s );
			item->setFlags( item->flags() | Qt::ItemIsEditable );
			item->setIcon( icon::get( s ) );
			listWidget->addItem( item );
		}

		widget_2->bind( &config.sseMO2Path );
		widget_6->bind( &config.sseCKPath );

		connect( label, &HClickableLabel::clicked, std::bind( &Impl::_addTool, this ) );
		connect( label_3, &HClickableLabel::clicked, std::bind( &Impl::_removeTool, this ) );
		connect( listWidget, &QListWidget::itemChanged, std::bind( &Impl::_itemChanged, this, std::placeholders::_1 ) );
	}


	/////////////////////////////////////////
	void saveList() {
		QStringList lst;
		for( int i = 0; i < listWidget->count(); ++i ) {
			auto* p = listWidget->item( i );
			lst << p->text();
		}
		config.ssePapyrusImportPaths = lst;
		Config::save();
	}


	/////////////////////////////////////////
	void _addTool() {
		auto s = environment::currentDirectory();
		auto* p = new QListWidgetItem( s );
		p->setIcon( icon::get( s ) );
		p->setFlags( p->flags() | Qt::ItemIsEditable );
		listWidget->addItem( p );
	}


	/////////////////////////////////////////
	void _removeTool() {
		listWidget->takeItem( listWidget->currentRow() );
		saveList();
	}


	/////////////////////////////////////////
	void _itemChanged( QListWidgetItem* item ) {
		saveList();
	}


	/////////////////////////////////////////
	bool dropEvent( QDropEvent* e ) {
		using namespace cpplinq;

		QModelIndex index = listWidget->indexAt( e->pos() );
		QListWidgetItem* item = nullptr;

		if( index.isValid() ) {
			item = dynamic_cast<QListWidgetItem*>( reinterpret_cast<QListWidgetItem*>( index.internalPointer() ) );
		}

		if( !e->mimeData()->hasUrls() ) return false;

		auto paths = from( e->mimeData()->urls() )
			>> select( []( const auto& s ) { return QUrl( s ).toLocalFile(); } )
			>> select( []( const auto& s ) { return fs::isExistFile( s ) ? path::getDirectoryName( s ) : s; } )
			>> distinct()
			>> to_qlist();

		for( auto& path : paths ) {
			if( item == nullptr ) {
				item = new QListWidgetItem();
				listWidget->addItem( item );
			}
			item->setText( path );
			item->setIcon( icon::get( path ) );
			item->setFlags( item->flags() | Qt::ItemIsEditable );
			item = nullptr;
		}
		saveList();

		return true;
	}
};


//////////////////////////////////////////////////////////////////////////////////
UISettingsSSE::UISettingsSSE( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
UISettingsSSE::~UISettingsSSE() {
}


/////////////////////////////////////////
bool UISettingsSSE::eventFilter( QObject* obj, QEvent* event ) {

	if( event->type() == QEvent::Drop ) {
		QDropEvent* ee = (QDropEvent*) event;
		if( !ee->mimeData()->hasUrls() ) return false;

		return impl->dropEvent( ee );
	}

	if( event->type() == QEvent::DragEnter ) {
		QDragEnterEvent* ee = (QDragEnterEvent*) event;
		ee->acceptProposedAction();
		return true;
	}

	if( event->type() == QEvent::DragMove ) {
		QDragMoveEvent* ee = (QDragMoveEvent*) event;
		ee->acceptProposedAction();
		return true;
	}
	return false;
}
