#include "ItemFolder.h"

//////////////////////////////////////////////////////////////////////////////////
ItemFolder::ItemFolder( QString _fullPath ) :
	fullPath( _fullPath ) {

	setText( 0, path::getFileName( fullPath ) );
	setIcon( 0, icon::get( fullPath ) );
}


/////////////////////////////////////////
void ItemFolder::openFolder() {
	$::showInExplorer( fullPath );
}


//////////////////////////////////////////////////////////////////////////////////
ItemFolderMO2View::ItemFolderMO2View( const QString& _folderPath, bool _check, bool fullpah ) :
	ItemFolder( _folderPath ),
	mode( 0 ) {

	if( fullpah ) {
		setBackgroundColor( 0, "#BFFFBF" );
		setBackgroundColor( 1, "#BFFFBF" );
		setBackgroundColor( 2, "#BFFFBF" );
	}
	else {
		//setIcon( 0, icon::get( folderPath ) );
		if( fs::isExistDirectory( _folderPath + "/strings" ) ) {
			//setIcon( 1, icon::FA::check() );
		}
		if( fs::isExistFile( _folderPath + "/build.csv" ) ) {
			setIcon( 2, ICON_EDITOR );
		}
		if( fs::isExistFile( _folderPath + "/merge.txt" ) ) {
			setIcon( 2, ICON_EDITOR );
		}

	}
	setCheckState( 0, _check );
}


/////////////////////////////////////////
ItemFolderMO2View::ItemFolderMO2View( const QString& _folderPath, int _mode ) :
	ItemFolder( _folderPath ),
	mode( _mode ) {

	setBackgroundColor( 0, "#BFFFBF" );
	setBackgroundColor( 1, "#BFFFBF" );
	setBackgroundColor( 2, "#BFFFBF" );

	setCheckState( 0, true );
}


/////////////////////////////////////////
QStringList ItemFolderMO2View::pluginFiles() {
	return fs::getFiles( fullPath, "*.esp;*.esl;*.esm", SearchOption::TopDirectoryOnly );
}
