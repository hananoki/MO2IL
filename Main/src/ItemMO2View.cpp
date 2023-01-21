#include "ItemMO2View.h"

//////////////////////////////////////////////////////////////////////////////////
ItemMO2Group::ItemMO2Group( const QString& _folderName ) {

	folderPath = config.getMO2Path() + "/mods/" + _folderName;
	setText( 0, $$( _folderName ).replace( "_separator", "" ) );
}

