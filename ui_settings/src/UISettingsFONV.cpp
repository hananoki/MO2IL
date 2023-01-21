#include "UISettingsFONV.h"
#include "ui_UISettingsFONV.h"

//////////////////////////////////////////////////////////////////////////////////
class UISettingsFONV::Impl : public Ui_UISettingsFONV {
public:

	UISettingsFONV* self;

	/////////////////////////////////////////
	Impl( UISettingsFONV* _self ) :
		self( _self ) {

		setupUi( self );

		widget_3->bind( &config.fonvMO2Path );
		inputTransPath->bind( &config.fonvXMLPath );
		outputPath->bind( &config.fonvFOJPTextPath );

	}
};


//////////////////////////////////////////////////////////////////////////////////
UISettingsFONV::UISettingsFONV( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
UISettingsFONV::~UISettingsFONV() {
}
