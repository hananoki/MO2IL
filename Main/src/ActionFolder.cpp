#include "ActionFolder.h"
#include "ItemFolder.h"

//////////////////////////////////////////////////////////////////////////////////
ActionOpenFolder::ActionOpenFolder( HTreeWidget* treeWidget ) :
	QAction( tr( u8"フォルダを開く" ) ) {

	setIcon( ICON_EXPLORER );

	connect( this, &QAction::triggered, [treeWidget]() {
		auto* item = treeWidget->currentItem<ItemFolder>();
		if( item ) {
			item->openFolder();
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}


//////////////////////////////////////////////////////////////////////////////////
ActionFolderPathCopy::ActionFolderPathCopy( HTreeWidget* treeWidget ) :
	QAction( tr( u8"フォルダーパスをクリップボードにコピー" ) ) {

	//setIcon( ICON_EXPLORER );

	connect( this, &QAction::triggered, [treeWidget]() {
		auto* item = treeWidget->currentItem<ItemFolder>();
		if( item ) {
			QClipboard* clipboard = QApplication::clipboard();
			clipboard->setText( path::separatorToOS( item->fullPath ) );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );

}


//////////////////////////////////////////////////////////////////////////////////
ActionFolderOpenNexus::ActionFolderOpenNexus( HTreeWidget* treeWidget, int _modID ) :
	QAction( $$( tr( u8"Nexusにアクセス (%1)" ) ).arg( _modID ) ),
	modID( _modID ) {

	setIcon( ICON_INTERNET );

	connect( this, &QAction::triggered, [treeWidget, this]() {
		auto* item = treeWidget->currentItem<ItemFolder>();
		if( item ) {
			$::showInExplorer( utils::nexusURL( modID ) );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );

}


/////////////////////////////////////////
ActionFolderOpenNexus* ActionFolderOpenNexus::create( HTreeWidget* treeWidget ) {
	auto* item = treeWidget->currentItem<ItemFolderMO2View>();
	if( !item ) return nullptr;

	auto metaPath = item->fullPath + "/meta.ini";
	if( !fs::isExistFile( metaPath ) ) return nullptr;

	for( auto& s : fs::readAllLines( metaPath ) ) {
		if( !s.contains( "modid" ) )continue;

		auto ss = s.replace( " ", "" ).split( "=" );

		int modID = $::toInt( ss[ 1 ] );
		if( modID <= 0 )continue;

		return new ActionFolderOpenNexus( treeWidget, modID );
	}

	return nullptr;
}
