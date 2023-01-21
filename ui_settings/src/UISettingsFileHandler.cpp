#include "UISettingsFileHandler.h"
#include "ui_UISettingsFileHandler.h"

//////////////////////////////////////////////////////////////////////////////////
class UISettingsFileHandler::Impl : public Ui_UISettingsFileHandler {
public:

	UISettingsFileHandler* self;

	QList<ComboBoxFileHandler*> widgets;

	/////////////////////////////////////////
	Impl( UISettingsFileHandler* _self ) :
		self( _self ) {

		setupUi( self );

		comboBox_bsa->bind( &config.bsaToolPath );
		widgets << comboBox_bsa;

		comboBox_esp->bind( &config.espToolPath );
		widgets << comboBox_esp;

		comboBox_nif->bind( &config.nifToolPath );
		widgets << comboBox_nif;

		comboBox_text->bind( &config.textToolPath );
		widgets << comboBox_text;
	}


	/////////////////////////////////////////
	void showEvent( QShowEvent* event ) {
		for( auto& w : widgets ) {
			w->makeItem();
		}
	}

};


//////////////////////////////////////////////////////////////////////////////////
UISettingsFileHandler::UISettingsFileHandler( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
UISettingsFileHandler::~UISettingsFileHandler() {
}


/////////////////////////////////////////
void UISettingsFileHandler::showEvent( QShowEvent* event ) {
	impl->showEvent( event );
}

