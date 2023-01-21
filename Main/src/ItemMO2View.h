#pragma once

//////////////////////////////////////////////////////////////////////////////////
class ItemMO2Group : public HTreeWidgetItem {
public:
	QString folderPath;

	ItemMO2Group( const QString& name );
};


//////////////////////////////////////////////////////////////////////////////////
class ItemBuildTargetFile : public HTreeWidgetItem {
public:
	QString folderPath;
	QString srcPath;
	QString destPath;

	QString destFullPath() {
		return $$( "%1/%2" ).arg( folderPath ).arg( destPath );
	}
	QString srcFullPath() {
		QFileInfo fi( srcPath );
		if( fi.isAbsolute() )return srcPath;

		return $$( "%1/%2" ).arg( folderPath ).arg( srcPath );
	}

	bool papi;

	/////////////////////////////////////////
	ItemBuildTargetFile( const QString& _folderPath, const QString& line ) :
		folderPath( _folderPath ),
		papi( false ) {

		auto ss = line.split( "\t" );
		setText( 0, ss[ 0 ] );
		setText( 1, ss[ 1 ] );
		setText( 2, ss[ 2 ] );
		destPath = ss[ 0 ];
		srcPath = ss[ 2 ];
		if( path::hasSuffix( destPath, "pex" ) ) {
			papi = true;
		}
	}
};

