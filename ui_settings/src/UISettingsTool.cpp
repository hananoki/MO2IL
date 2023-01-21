#include "UISettingsTool.h"
#include "ui_UISettingsTool.h"

//////////////////////////////////////////////////////////////////////////////////
class UISettingsTool::Impl : public Ui_UISettingsTool {
public:
	UISettingsTool* self;

	QFuture<void> future;
	QFutureWatcher<void> watcher;

	QMenu* contextMenuL;
	QMenu* contextMenuFolder;


	/////////////////////////////////////////
	Impl( UISettingsTool* _self ) :
		self( _self ) {

		setupUi( self );

		listWidget->installEventFilter( self );

		for( auto& s : config.toolPaths ) {
			auto* item = new QListWidgetItem( s );
			item->setFlags( item->flags() | Qt::ItemIsEditable );
			item->setIcon( icon::get( s ) );
			listWidget->addItem( item );
		}

		$Action::triggered( actionAdd, std::bind( &Impl::_addTool, this ) );
		$Action::triggered( actionRemove, std::bind( &Impl::_removeTool, this ) );

		connect( listWidget, &QListWidget::itemChanged, std::bind( &Impl::_itemChanged, this, std::placeholders::_1 ) );
	}


	/////////////////////////////////////////
	void saveList() {
		QStringList lst;
		for( int i = 0; i < listWidget->count(); ++i ) {
			auto* p = listWidget->item( i );
			lst << p->text();
		}
		config.toolPaths = lst;
		Config::save();
	}


	/////////////////////////////////////////
	void _itemChanged( QListWidgetItem* item ) {
		saveList();
	}


	/////////////////////////////////////////
	void _addTool() {
		QString s = QFileDialog::getOpenFileName(
			self,
			tr( u8"ファイルを選択" ),
			environment::currentDirectory(),
			{ u8"実行ファイル(*.exe)" }
		);
		if( s.isEmpty() ) {
			return;
		}
		if( s.isEmpty() )return;

		auto* p = new QListWidgetItem( s );
		p->setIcon( icon::get( s ) );
		listWidget->addItem( p );
		p->setFlags( p->flags() | Qt::ItemIsEditable );
	}


	/////////////////////////////////////////
	void _removeTool() {
		listWidget->takeItem( listWidget->currentRow() );
		saveList();
	}


	/////////////////////////////////////////
	bool dropEvent( QDropEvent* e ) {

		QModelIndex index = listWidget->indexAt( e->pos() );
		if( !index.isValid() ) {  // just in case
			//e->setDropAction( Qt::IgnoreAction );
			if( !e->mimeData()->hasUrls() ) return false;

			for( auto& s : e->mimeData()->urls() ) {
				auto path = QUrl( s ).toLocalFile();

				auto* p = new QListWidgetItem( path );
				listWidget->addItem( p );
				p->setFlags( p->flags() | Qt::ItemIsEditable );
			}

			return true;
		}

		//auto* dragSourceWidget = dynamic_cast<QListWidgetItem*>( e->source() );
		auto* dragSourceWidget = dynamic_cast<QListWidgetItem*>( reinterpret_cast<QListWidgetItem*>( index.internalPointer() ) );

		qDebug() << dragSourceWidget->text();
		//if( dragSourceWidget == UIViewR_SourceFiles::getSourceFilesTreeWidget() ) {
		//}
		return true;
	}
};


//////////////////////////////////////////////////////////////////////////////////
UISettingsTool::UISettingsTool( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

}

/////////////////////////////////////////
UISettingsTool::~UISettingsTool() {

}


/////////////////////////////////////////
/// @brief  イベントフィルター
bool UISettingsTool::eventFilter( QObject* obj, QEvent* event ) {

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



