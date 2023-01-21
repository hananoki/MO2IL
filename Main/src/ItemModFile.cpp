#include "ItemModFile.h"
#include "TESForm.h"


ItemModFormID::ItemModFormID( TESForm* _formData ) :
	m_formData( _formData ) {

	_formData->item = this;

	setText( 0, QString( "%1" ).arg( _formData->ID, 8, 16, QChar( '0' ) ).toUpper() );
	setText( 1, _formData->editorName );
	setText( 2, _formData->fullName );
}
