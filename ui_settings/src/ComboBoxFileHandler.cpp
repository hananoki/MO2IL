#include "ComboBoxFileHandler.h"

/////////////////////////////////////////
ComboBoxFileHandler::ComboBoxFileHandler( QWidget* parent ) :
	HComboBox( parent ) {

	makeItem( true );
}


/////////////////////////////////////////
void ComboBoxFileHandler::makeItem( bool init ) {
	HSignalBlocker _( this );

	clear();
	for( auto& s : config.toolPaths ) {
		addItem( icon::get( s ), path::getBaseName( s ) );
	}

	if( init )return;

	int n = config.toolPaths.indexOf( *bindString );
	setCurrentIndex( n );
}


/////////////////////////////////////////
void ComboBoxFileHandler::bind( QString* t, QObject* _config /*= nullptr*/ ) {
	bindString = t;
	int n = config.toolPaths.indexOf( *bindString );
	setCurrentIndex( n );

	$ComboBox::currentIndexChanged( this, [this, _config]( int index ) {
		*bindString = config.toolPaths[ index ];
		auto* c = dynamic_cast<IConfigChanged*>( _config );
		if( c ) {
			c->changedValue();
		}
		emit currentTextChanged();
	} );
}

