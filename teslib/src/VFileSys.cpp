#include "VFileSys.h"
//#include "HLogView.h"
#include "Qtlib.h"
#include <QMap>
#include <QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>
#include "TESSystem.h"

VFileSys fileDB;


//////////////////////////////////////////////////////////////////////////////////
class VFileSys::Impl {
public:
	QFuture<void> future;
	QFutureWatcher<void> watcher;

	VFileSys* self;

	QString gameTitleName;
	QString mo2Path;
	QString profileName;

	QMap<QString, QMap<QString, QString>> map;

	int readStage;


	/////////////////////////////////////////
	Impl( VFileSys* _self ) :
		self( _self ) {
		QObject::connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			//HLogView::info( u8"処理が完了しました" );
			//HLogView::info( tr( u8"initMO2 ... complete" ) );
			emit self->signal_initCompleted();
			writeCache();
		} );

		QObject::connect( &watcher, &QFutureWatcher<void>::progressValueChanged, self, [&]( int progressValue ) {
			if( progressValue == 1 ) {
				emit self->signal_completeStage1();
			}
		} );
	}


	/////////////////////////////////////////
	void initMO2( const QString& installGamePath, const QString& _mo2Path, const QString& _profileName ) {
		if( future.isRunning() )return;

		if( _mo2Path.isEmpty() ) {
			//HLogView::warning( tr( u8"initMO2 ... dirPath is Empty." ) );
			return;
		}
		emit self->signal_initStarted( _mo2Path );

		//HLogView::info( tr( u8"initMO2 ... start" ) );
		gameTitleName = path::getBaseName( installGamePath );
		tesSys.setGameTitle( gameTitleName );
		mo2Path = _mo2Path;
		profileName = _profileName;

		map.clear();
		readStage = 0;
		future = QtConcurrent::run( this, &Impl::init, installGamePath, mo2Path, profileName );
		watcher.setFuture( future );
	}


	/////////////////////////////////////////
	QString cacheFileName() {
		return $$( "%1/%2.cache" ).arg( gameTitleName ).arg( profileName );
	}


	/////////////////////////////////////////
	void writeCache() {
		QStringList s;
		for( auto& folder : map.keys() ) {
			auto& filePair = map[ folder ];
			for( auto& p : filePair.keys() ) {
				s << $$( "%1\t%2\t%3" ).arg( folder ).arg( p ).arg( filePair[ p ] );
			}
		}
		fs::writeAllLines( cacheFileName(), s );
	}


	/////////////////////////////////////////
	void init( const QString& installGamePath, const QString& mo2Path, const QString& profileName ) {
		QStringList modListPath;

		//tesSys.clear();
		if( fs::isExistFile( cacheFileName() ) ) {
			fs::readAllLines( cacheFileName(), Encoding::UTF8, [&]( const QString& s ) {
				auto ss = s.split( "\t" );

				auto it = map.find( ss[0] );
				if( it == map.constEnd() ) {
					map.insert( ss[ 0 ], QMap<QString, QString>() );
					it = map.find( ss[ 0 ] );
				}

				// すでに同じファイルが登録されている
				if( it->contains( ss[ 1 ] ) )return true;

				( *it ).insert( ss[ 1 ], ss[ 2 ] );
			} );
			watcher.progressValueChanged( 1 );
			return;
		}

		auto modFolderPath = $$( "%1/mods/" ).arg( mo2Path );
		auto modlist = $$( "%1/profiles/%2/modlist.txt" ).arg( mo2Path ).arg( profileName );
		fs::readAllLines( modlist, Encoding::UTF8, [&]( const QString& s ) {
			if( s[ 0 ] == '#' )return;
			if( s[ 0 ] == '-' )return;

			if( s[ 0 ] == '*' ) {
			}
			auto modName = QString( s ).replace( 0, 1, "" );
			auto modPath = modFolderPath + modName;

			modListPath << modPath;
		} );

		for( readStage = 0; readStage < 2; readStage++ ) {
			//fs::enumerateDirectories( $$( "%1/overwrite" ).arg( mo2Path ), SearchOption::TopDirectoryOnly, [&](const QString) {
			enumerateFiles( $$( "%1/overwrite" ).arg( mo2Path ) );
			//} );

			for( auto& f : modListPath ) {
				enumerateFiles( f );
			}

			enumerateFiles( installGamePath + "/Data" );

			if( readStage == 0 ) {
				watcher.progressValueChanged( 1 );
			}
		}
	}



	/////////////////////////////////////////
	void enumerateFiles( const QString& modPath ) {

		QString filter = readStage == 0 ? "*.esm;*.esp;*.esl;*.STRINGS;*.DLSTRINGS;*.ILSTRINGS" : "*";

		fs::enumerateFiles( modPath, filter, SearchOption::AllDirectories, [&]( const QString& fname ) {
			auto fname2 = QString( fname ).replace( 0, modPath.length() + 1, "" );

			auto ff = path::getFileName( fname2 ).toLower();
			if( ff == "meta.ini" )return true;

			auto dd = path::getDirectoryName( fname2 ).toLower();

			auto it = map.find( dd );
			if( it == map.constEnd() ) {
				map.insert( dd, QMap<QString, QString>() );
				it = map.find( dd );
			}

			// すでに同じファイルが登録されている
			if( it->contains( ff ) )return true;

			( *it ).insert( ff, fname );
			return true;
		} );
	}
};


/////////////////////////////////////////
VFileSys::VFileSys() :
	impl( new Impl( this ) ) {
}


/////////////////////////////////////////
VFileSys::~VFileSys() {
}

///////////////////////////////////////
QString VFileSys::mo2Path() {
	return impl->mo2Path;
}

///////////////////////////////////////
QString VFileSys::mo2ProfilePath() {
	return $$( "%1/profiles/%2" ).arg( mo2Path() ).arg( impl->profileName );
}

/////////////////////////////////////////

/**
 * @brief  MO2ファイルシステムで初期化する
 */
void VFileSys::initMO2( const QString& installGamePath, const QString& mo2Path, const QString& profileName ) {
	impl->initMO2( installGamePath, mo2Path, profileName );
}



/////////////////////////////////////////
void VFileSys::enumerateFiles( std::function<void( const QString& folder, const QString& file, const QString& fullName )> cb ) {
	for( auto& folder : impl->map.keys() ) {
		auto& filePair = impl->map[ folder ];
		for( auto& p : filePair.keys() ) {
			cb( folder, p, filePair[ p ] );
		}
	}
}


/////////////////////////////////////////
QHash<QString, QString> VFileSys::getModFiles() {
	QHash<QString, QString> result;
	QStringList suffixList{ "esm","esp","esl" };
	auto& dataRoot = impl->map[ "" ];
	for( auto& p : dataRoot.keys() ) {
		//if(p.contains("p.a.c") ) {
		//	qDebug() << "";
		//}
		auto suffix = QFileInfo( dataRoot[ p ] ).suffix();
		if( suffixList.indexOf( suffix ) < 0 )continue;
		//result << VirtualFile{ path::getFileName( dataRoot[ p ] ), dataRoot[ p ] };
		result.insert( p, dataRoot[ p ] );
	}
	return result;
}


///////////////////////////////////////
QString VFileSys::absolutePath( QString virtualPath ) {
	auto ff = path::getFileName( virtualPath ).toLower();
	auto dd = path::getDirectoryName( virtualPath ).toLower();

	auto it = impl->map.find( dd );
	if( it == impl->map.constEnd() )return "";

	auto it2 = it->find( ff );
	if( it2 == it->constEnd() )return "";

	return *it2;
}


///////////////////////////////////////
void VFileSys::dumpText() {
	QStringList s;
	for( auto& folder : impl->map.keys() ) {
		auto& filePair = impl->map[ folder ];
		for( auto& p : filePair.keys() ) {
			s << $$( "%1\t%2\t%3" ).arg( folder ).arg( p ).arg( filePair[ p ] );
		}
	}
	fs::writeAllLines( "FileDB.txt", s );
}
