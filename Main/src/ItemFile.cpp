#include "ItemFile.h"


static QString size_human( const QFileInfo& finfo ) {
	double num = finfo.size();

	if( num >= 1024 ) {
		num /= 1024.0;
	}
	else {
		num = 1;
	}
	return QLocale( QLocale::English ).toString( (ulong) std::ceil( num ) ) + " KB";
}


/////////////////////////////////////////
ItemFile::ItemFile( QString _fullPath, int column, QString _replace ) :
	fullPath( _fullPath ) {

	if( _replace.isEmpty() ) {
		setText( column, path::getFileName( fullPath ) );
	}
	else {
		setText( column, _fullPath.replace( _replace, "" ) );
	}
	setIcon( column, icon::get( fullPath ) );

	if( column < 0 )return;

	if( column == 0 ) {
		QFileInfo fi( fullPath );
		setLastModified( 1, fi );
		setText( 2, size_human( fi ) );
		setTextAlignment( 2, Qt::AlignRight );
	}
}


/////////////////////////////////////////
bool ItemFile::openFile() {
	if( !fs::isExistFile( fullPath ) ) {
		HLogView::error( u8"ファイルが見つかりません: " + fullPath );
		return false;
	}
	$::showInExplorer( fullPath );
	return true;
}


/////////////////////////////////////////
bool ItemFile::showInExplorer() {
	auto d = folderPath();
	if( !fs::isExistDirectory( d ) )return false;
	$::showInExplorer( d );
	return true;
}


/////////////////////////////////////////
QString ItemFile::fileName() {
	return path::getFileName( fullPath );
}

/////////////////////////////////////////
QString ItemFile::folderPath() {
	return path::getDirectoryName( fullPath );
}

/////////////////////////////////////////
bool ItemFile::hasSuffix( const QString& suffix ) {
	return hasSuffix( QStringList{ suffix } );
}

/////////////////////////////////////////
bool ItemFile::hasSuffix( const QStringList& suffixList ) {
	auto suffix = path::getSuffix( fullPath ).toLower();
	return 0 <= suffixList.indexOf( suffix );
}



//////////////////////////////////////////////////////////////////////////////////
ItemGroupViewFile::ItemGroupViewFile( const QString& _fullPath ) :
	ItemFile( _fullPath ) {

	auto ss = $$( fullPath ).replace( config.getMO2Path() + "/mods/", "" ).split( "/" );
	setText( 1, ss[ 0 ] );

}
