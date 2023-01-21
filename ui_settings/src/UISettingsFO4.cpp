#include "UISettingsFO4.h"
#include "ui_UISettingsFO4.h"

//////////////////////////////////////////////////////////////////////////////////
class UISettingsFO4::Impl : public Ui_UISettingsFO4 {
public:

	UISettingsFO4* self;

	/////////////////////////////////////////
	Impl( UISettingsFO4* _self ) :
		self( _self ) {

		setupUi( self );

		widget->bind( &config.fo4MO2Path );
		widget_2->bind( &config.fo4CKPath );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UISettingsFO4::UISettingsFO4( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

}

/////////////////////////////////////////
UISettingsFO4::~UISettingsFO4() {
}
