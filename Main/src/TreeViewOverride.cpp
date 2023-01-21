#include "TreeViewOverride.h"

//////////////////////////////////////////////////////////////////////////////////
class TreeViewOverride::Impl {
public:
	QFuture<void> future;
	QFutureWatcher<void> watcher;

	TreeViewOverride* self;

	QMenu* contextMenu;


	/////////////////////////////////////////
	Impl( TreeViewOverride* _self ) :
		self( _self ) {

	}
};

//////////////////////////////////////////////////////////////////////////////////
TreeViewOverride::TreeViewOverride( QWidget* parent ) :
	HTreeWidget( parent ),
	impl( new Impl( this ) ) {

	
}
