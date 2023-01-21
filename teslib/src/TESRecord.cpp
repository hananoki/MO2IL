#include "TESRecord.h"
#include "TESField.h"
//#include "TESFile.h"

/////////////////////////////////////////
//TESRecord::TESRecord() {
//}

/////////////////////////////////////////
TESRecord::TESRecord( const TESRecord& r ) :
	sign( r.sign ),
	dataSize( r.dataSize ),
	RecordFlags( r.RecordFlags ),
	formID( r.formID ),
	VersionControlInfo1( r.VersionControlInfo1 ),
	FormVersion( r.FormVersion ),
	VersionControlInfo2( r.VersionControlInfo2 ) {
}


/////////////////////////////////////////
void TESRecord::enumerateFields( TESRecord* r, std::function<bool( TESField* )> cb ) {
	auto* p = offsetPtr<TESField>( r, sizeof( TESRecord ) );
	auto* ed = offsetPtr<TESField>( r, sizeof( TESRecord ) + r->dataSize );

	for( ; p < ed; p = p->next() ) {
		if( !cb( p ) )break;
	}
}


