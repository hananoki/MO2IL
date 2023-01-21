#include "TESSystem.h"
#include "TESFile.h"
#include "TESFormAllocator.h"
//

TESSystem tesSys;

//////////////////////////////////////////////////////////////////////////////////
class TESSystem::Impl {
public:
	//QFuture<void> future;
	//QFutureWatcher<void> watcher;

	TESSystem* self;

	int loadCount = 0;
	int eslCount = 0;

	//QList<QMap<quint32, QString> >map;

	QList<TESFile*> mods;

	QHash<QString, quint32> mainLoaded;
	QHash<QString, quint32> subLoaded;

	// 修飾済みIDの重複フォーム
	QMap<quint32, QList<TESForm*>> overrideFormIDs;

	QString gameTitle;

	/////////////////////////////////////////
	Impl( TESSystem* _self ) :
		self( _self ) {

	}


	/////////////////////////////////////////
	void clear() {
		loadCount = 0;
		eslCount = 0;
		for( int i = 0; i < mods.size(); i++ ) {
			delete mods[ i ];
		}
		mods.clear();
		overrideFormIDs.clear();
		TESFormAllocator::init();
	}


	/////////////////////////////////////////
	TESFile* loadFile( const QString& filePath ) {
		if( filePath.contains( "M2045MagnumRevolverRifle" ) ) {
			qDebug() << "";
		}
		auto* modFile = TESFile::loadFromFile( filePath );

		if( !modFile )return nullptr;

		mods << modFile;

		return modFile;
	}


	/////////////////////////////////////////
	void registerFile( TESFile* modFile ) {
		TESLoadOrder loadOrder;

		if( modFile->isESL() ) {
			loadOrder.main = 0xFE;
			loadOrder.sub = eslCount;
			subLoaded.insert( modFile->fileName().toLower(), loadOrder.sub );
			eslCount++;
		}
		else {
			loadOrder.main = loadCount;
			loadOrder.sub = -1;
			mainLoaded.insert( modFile->fileName().toLower(), loadOrder.main );
			loadCount++;
		}
		modFile->setLoadOrder( loadOrder );
	}


	/////////////////////////////////////////
	void registerForm( TESForm* form ) {
		auto it = overrideFormIDs.find( form->ID );
		if( it == overrideFormIDs.constEnd() ) {
			overrideFormIDs.insert( form->ID, QList<TESForm*>() );
		}
		overrideFormIDs[ form->ID ] << form;
	}


	/////////////////////////////////////////
	quint32 getMasterFormID( const QString& masterFileName ) {
		auto f = masterFileName.toLower();
		auto it = mainLoaded.find( f );
		if( it != mainLoaded.constEnd() ) {
			return ( *it << 24 );
		}
		auto it2 = subLoaded.find( f );
		if( it2 != subLoaded.constEnd() ) {
			return 0xFE000000 | ( *it << 12 );
		}
		return -1;
	}


	/////////////////////////////////////////
	int formStatus( quint32 formID ) {
		int n = overrideFormIDs[ formID ].length();

		return n;
	}


	/////////////////////////////////////////
	TESForm* formData( quint32 formID ) {
		auto it = overrideFormIDs.find( formID );
		if( it == overrideFormIDs.constEnd() ) {
			return nullptr;
		}
		QList<TESForm*>& lst = overrideFormIDs[ formID ];
		return lst.last();
	}
};


//////////////////////////////////////////////////////////////////////////////////
TESSystem::TESSystem() :
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
TESSystem::~TESSystem() {
}

/////////////////////////////////////////
void TESSystem::clear() {
	impl->clear();
}

/////////////////////////////////////////
TESFile* TESSystem::loadFile( const QString& filePath ) {
	return impl->loadFile( filePath );
}


/////////////////////////////////////////
void TESSystem::registerFile( TESFile* modFile ) {
	impl->registerFile( modFile );
}


/////////////////////////////////////////
void TESSystem::registerForm( TESForm* form ) {
	return impl->registerForm( form );
}


/////////////////////////////////////////
quint32 TESSystem::getMasterFormID( const QString& masterFileName ) {
	return impl->getMasterFormID( masterFileName );
}


/////////////////////////////////////////
int TESSystem::formStatus( quint32 formID ) {
	return impl->formStatus( formID );
}

/////////////////////////////////////////
TESForm* TESSystem::formData( quint32 formID ) {
	return impl->formData( formID );
}

/////////////////////////////////////////
const QList<TESForm*>& TESSystem::formDataList( quint32 formID ) {
	return impl->overrideFormIDs[ formID ];

}

/////////////////////////////////////////
void TESSystem::enumerateForm( std::function<void( const QList<TESForm*>& )> cb ) {
	for( auto& k : impl->overrideFormIDs.keys() ) {
		cb( impl->overrideFormIDs[ k ] );
	}
}


/////////////////////////////////////////
void TESSystem::enumerateFormByGroup( quint32 groupID, std::function<void( TESForm* )> cb ) {
	for( auto* file : impl->mods ) {
		for( auto* form : file->formDataList( groupID ) ) {
			cb( form );
		}
	}
}


/////////////////////////////////////////
QString TESSystem::gameTitle() {
	return impl->gameTitle;
}


/////////////////////////////////////////
void TESSystem::setGameTitle( const QString& str ) {
	impl->gameTitle = str;
}

