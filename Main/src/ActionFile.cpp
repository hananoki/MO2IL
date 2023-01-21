#include "ActionFile.h"
#include "ItemMO2View.h"
#include "ItemFile.h"
#include "ItemFolder.h"


//////////////////////////////////////////////////////////////////////////////////
ActionOpenFile::ActionOpenFile( HTreeWidget* treeWidget ) :
	QAction( tr( u8"開く" ) ) {

	setIcon( ICON_EXPLORER );

	QObject::connect( this, &QAction::triggered, [treeWidget]() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			item->openFile();
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}


//////////////////////////////////////////////////////////////////////////////////
ActionShowInExplorer::ActionShowInExplorer( HTreeWidget* tw ) :
	QAction( tr( u8"ファイルの場所を開く" ) ),
	treeWidget( tw ) {

	QObject::connect( this, &QAction::triggered, [&]() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			item->showInExplorer();
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}
ActionShowInExplorer::~ActionShowInExplorer() {
	qDebug() << "";
}

//////////////////////////////////////////////////////////////////////////////////
ActionCopyFileName::ActionCopyFileName( HTreeWidget* tw ) :
	QAction( tr( u8"ファイル名をクリップボードにコピー" ) ),
	treeWidget( tw ) {

	QObject::connect( this, &QAction::triggered, [&]() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			QClipboard* clipboard = QApplication::clipboard();
			clipboard->setText( item->fileName() );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}



//////////////////////////////////////////////////////////////////////////////////
ActionOpenBSA::ActionOpenBSA( HTreeWidget* treeWidget ) :
	QAction() {

	setText( path::getBaseName( config.bsaToolPath ) + u8"で開く" );
	setIcon( icon::get( config.bsaToolPath ) );

	QObject::connect( this, &QAction::triggered, [=]() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			QProcess p;
			p.setWorkingDirectory( item->folderPath() );
			p.startDetached( config.bsaToolPath, { path::separatorToOS( item->fullPath ) } );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}

/////////////////////////////////////////
ActionOpenBSA* ActionOpenBSA::create( HTreeWidget* treeWidget ) {
	if( !fs::isExistFile( config.bsaToolPath ) ) return nullptr;
	auto* item = treeWidget->currentItem<ItemFile>();
	if( !item ) return nullptr;
	if( !item->hasSuffix( { "bsa","ba2" } ) ) return nullptr;
	return new ActionOpenBSA( treeWidget );
}


//////////////////////////////////////////////////////////////////////////////////
ActionOpenNIF::ActionOpenNIF( HTreeWidget* treeWidget ) :
	QAction() {

	setText( path::getBaseName( config.nifToolPath ) + u8"で開く" );
	setIcon( icon::get( config.nifToolPath ) );

	QObject::connect( this, &QAction::triggered, [=]() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			QProcess p;
			p.setWorkingDirectory( item->folderPath() );
			p.startDetached( config.nifToolPath, { path::separatorToOS( item->fullPath ) } );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}

/////////////////////////////////////////
ActionOpenNIF* ActionOpenNIF::create( HTreeWidget* treeWidget ) {
	if( !fs::isExistFile( config.nifToolPath ) ) return nullptr;
	auto* item = treeWidget->currentItem<ItemFile>();
	if( !item ) return nullptr;
	if( !item->hasSuffix( "nif" ) ) return nullptr;
	return new ActionOpenNIF( treeWidget );
}


//////////////////////////////////////////////////////////////////////////////////
ActionOpenESP::ActionOpenESP( HTreeWidget* treeWidget ) :
	QAction() {

	setText( path::getBaseName( config.espToolPath ) + u8"で開く" );
	setIcon( icon::get( config.espToolPath ) );

	QObject::connect( this, &QAction::triggered, [=]() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			QProcess p;
			p.setWorkingDirectory( item->folderPath() );
			p.startDetached( config.espToolPath, { path::separatorToOS( item->fullPath ) } );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}

/////////////////////////////////////////
ActionOpenESP* ActionOpenESP::create( HTreeWidget* treeWidget ) {
	if( !fs::isExistFile( config.espToolPath ) ) return nullptr;
	auto* item = treeWidget->currentItem<ItemFile>();
	if( !item ) return nullptr;
	if( !item->hasSuffix( { "esp","esl","esm" } ) ) return nullptr;
	return new ActionOpenESP( treeWidget );
}


//////////////////////////////////////////////////////////////////////////////////
ActionOpenText::ActionOpenText( HTreeWidget* treeWidget ) :
	QAction() {

	setText( path::getBaseName( config.textToolPath ) + u8"で開く" );
	setIcon( icon::get( config.textToolPath ) );

	QObject::connect( this, &QAction::triggered, [=]() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( item ) {
			QProcess p;
			p.setWorkingDirectory( item->folderPath() );
			p.startDetached( config.textToolPath, { path::separatorToOS( item->fullPath ) } );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}

/////////////////////////////////////////
ActionOpenText* ActionOpenText::create( HTreeWidget* treeWidget ) {
	if( !fs::isExistFile( config.textToolPath ) ) return nullptr;
	auto* item = treeWidget->currentItem<ItemFile>();
	if( !item ) return nullptr;
	//if( !item->hasSuffix( { "txt","ini","toml","json","psc" } ) ) return nullptr;
	return new ActionOpenText( treeWidget );
}

