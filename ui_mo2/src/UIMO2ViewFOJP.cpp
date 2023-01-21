#include "UIMO2ViewFOJP.h"
#include "ui_UIMO2ViewFOJP.h"

using namespace cpplinq;

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewFOJP::Impl : public Ui_UIMO2ViewFOJP {
public:
	using self_t = UIMO2ViewFOJP;
	self_t* self;

	/////////////////////////////////////////
	Impl( self_t* _self ) :
		self( _self ) {

		setupUi( self );

		listWidget->setAcceptDrops( true );
		listWidget->installEventFilter( self );
	}


	/////////////////////////////////////////
	bool dropEvent( QObject* obj, QDropEvent* e ) {
		auto files = from( e->mimeData()->urls() )
			>> select( []( auto x ) {return x.toLocalFile(); } )
			>> select( []( auto x ) {return x.replace( "_en.txt", "" ); } )
			>> select( []( auto x ) {return x.replace( "_jp.txt", "" ); } )
			>> distinct()
			>> where( []( auto x ) {return fs::isExistFile( x + "_en.txt" ); } )
			>> where( []( auto x ) {return fs::isExistFile( x + "_jp.txt" ); } )
			>> to_qlist();

		listWidget->clear();

		for( auto& s : files ) {
			auto* item = new QListWidgetItem( s + "_(en|jp).txt" );
			listWidget->addItem( item );
		}

		return true;
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIMO2ViewFOJP::UIMO2ViewFOJP( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
UIMO2ViewFOJP::~UIMO2ViewFOJP() {
}


/////////////////////////////////////////
bool UIMO2ViewFOJP::eventFilter( QObject* obj, QEvent* event ) {

	if( event->type() == QEvent::Drop ) {
		QDropEvent* ee = (QDropEvent*) event;
		if( !ee->mimeData()->hasUrls() ) return false;

		return impl->dropEvent( obj, ee );
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

