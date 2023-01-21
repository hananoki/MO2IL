#include "UISettings.h"
#include "ui_UISettings.h"

#include "UISettingsTool.h"
#include "UISettingsFileHandler.h"
#include "UISettingsFO4.h"
#include "UISettingsFONV.h"
#include "UISettingsSSE.h"

//////////////////////////////////////////////////////////////////////////////////
class UISettings::Impl : public Ui_UISettings {
public:

	UISettings* self;

	QHash<int, QWidget*> widgets;

	/////////////////////////////////////////
	Impl( UISettings* _self ) :
		self( _self ) {

		setupUi( self );

		auto strings = QStringList{
			u8"ツール設定",
			u8"ツリーアイテム設定",
			"Fallout 4",
			"Fallout New Vegas",
			"Skyrim Special Edition"};

		widgets.insert( 0, new UISettingsTool( self ) );
		widgets.insert( 1, new UISettingsFileHandler( self ) );
		widgets.insert( 2, new UISettingsFO4( self ) );
		widgets.insert( 3, new UISettingsFONV( self ) );
		widgets.insert( 4, new UISettingsSSE( self ) );

		int length = widgets.keys().length();
		for( int i = 0; i < length; i++ ) {
			verticalLayout_2->addWidget( widgets[ i ] );
			listWidget->addItem( new QListWidgetItem( strings[i] ) );
		}

		$ListWidget::itemSelectionChanged( listWidget, std::bind( &Impl::_itemSelectionChanged, this ) );

		listWidget->setCurrentRow( config.selectSettingsNo );
	}


	/////////////////////////////////////////
	void _itemSelectionChanged() {
		selectPanel( listWidget->currentRow() );
	}


	/////////////////////////////////////////
	void selectPanel( int no ) {
		for( auto& k : widgets.keys() ) {
			widgets[ k ]->setVisible( k == no );
		}
		config.selectSettingsNo = no;
	}

	
};


//////////////////////////////////////////////////////////////////////////////////
UISettings::UISettings( QWidget* parent ) :
	QDialog( parent ),
	impl( new Impl( this ) ) {

	$::restoreState( impl->splitter, config.settingsSplitter );
}


/////////////////////////////////////////
UISettings::~UISettings() {
	$::saveState( config.settingsSplitter, impl->splitter );
}

