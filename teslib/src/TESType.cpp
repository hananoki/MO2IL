#include "TESType.h"
#include "TES.h"
#include "TESFile.h"
#include "TESField.h"

//#include "HLogView.h"

//////////////////////////////////////////////////////////////////////////////////

TESTypeOBND::operator QString() const {
	return $$( "(%1, %2, %3), (%4, %5, %6)" )
		.arg( x1 )
		.arg( y1 )
		.arg( z1 )
		.arg( x2 )
		.arg( y2 )
		.arg( z2 );
}

//////////////////////////////////////////////////////////////////////////////////
TESTypeMNAM::TESTypeMNAM( TESField* field, TESFile* file ) {
	for( qint32* st = field->data<qint32>(), *ed = (qint32*) field->next(); st < ed; st++ ) {
		keywords << file->convertLoadOrder( *st );
	}
}


