#include "TESFormAllocator.h"
#include "TESForm.h"
#include "TESFieldReader.h"

TESFormAllocator tesFormAllocator;

//////////////////////////////////////////////////////////////////////////////////
class TESFormAllocator::Impl {
public:

	TESFormAllocator* self;

	static const int SIZE = 65535;

	struct MemBlock {
		quint8 mem[ sizeof( TESForm ) * SIZE ];
	};

	QList<MemBlock*> memBufferList;

	int count;

	QMutex mutex;

	/////////////////////////////////////////
	Impl( TESFormAllocator* _self ) :
		self( _self ) {

	}


	/////////////////////////////////////////
	void init() {
		for( auto* p : memBufferList ) {
			int num = SIZE;
			if( memBufferList.last() == p ) {
				num = count;
			}
			TESForm* buf = (TESForm*) p;
			for( int i = 0; i < num; i++ ) {
				buf[ i ].~TESForm();
#if _DEBUG
				int* d = (int*) &buf[ i ];
				for( int j = 0; j < sizeof( TESForm ) / 4; j++ ) {
					*d++ = 0xEFBEADDE;
				}
#endif
			}
			delete p;
		}
		count = SIZE;
		memBufferList.clear();
	}


	/////////////////////////////////////////
	TESForm* alloc() {
		if( SIZE <= count ) {
			memBufferList << new MemBlock();
			count = 0;
		}
		TESForm* buf = (TESForm*) memBufferList.last();
		TESForm* p = (TESForm*) &buf[ count ];
		count++;
		return p;
	}
};


//////////////////////////////////////////////////////////////////////////////////
TESFormAllocator::TESFormAllocator() :
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
void TESFormAllocator::init() {
	tesFormAllocator.impl->init();
}


/////////////////////////////////////////
TESForm* TESFormAllocator::alloc( TESFile* _file, TESRecord* _record ) {
	//QMutexLocker locker( &tesFormAllocator.impl->mutex );
	//return new TESForm( _file, _record );
	return  new ( tesFormAllocator.impl->alloc() ) TESForm( _file, _record );
}
