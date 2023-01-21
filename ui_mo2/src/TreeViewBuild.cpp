#include "TreeViewBuild.h"
#include "App.h"
#include "ItemMO2View.h"
#include <QProgressDialog>

//////////////////////////////////////////////////////////////////////////////////
class TreeViewBuild::Impl {
public:
	QFuture<void> future;
	QFutureWatcher<void> watcher;

	TreeViewBuild* self;

	std::unique_ptr<QProgressDialog> pd;

	QList<ItemBuildTargetFile*> buildTargets;

	QMenu* contextMenuFile;

	/////////////////////////////////////////
	Impl( TreeViewBuild* _self ) :
		self( _self ) {

		self->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );

		QObject::connect( &watcher, &QFutureWatcher<void>::progressValueChanged, self, [&]( int progressValue ) {
			pd->setValue( progressValue );
		pd->setLabelText( buildTargets[ progressValue ]->srcFullPath() );
		HLogView::info( u8"ビルド処理: " + buildTargets[ progressValue ]->srcFullPath() );
		} );

		QObject::connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			HLogView::info( u8"ビルドが完了しました" );
		pd->setValue( pd->maximum() );
		} );
	}


	/////////////////////////////////////////
	~Impl() {
		$::saveState( config.headerTreeViewBuild, self->header() );
	}


	/////////////////////////////////////////
	void setup() {
		$::restoreState( self->header(), config.headerTreeViewBuild );

#pragma region contextMenuFile
		contextMenuFile = new QMenu();

		contextMenuFile->addAction( $Action::create( tr( u8"ソースファイルの場所を開く" ), [&]() {
			auto* item = self->currentItem<ItemBuildTargetFile>();
		if( item ) {
			auto d = path::getDirectoryName( item->srcFullPath() );
			if( fs::isExistDirectory( d ) ) {
				$::showInExplorer( d );
			}
			else {
				HLogView::warning( u8"ビルド元のフォルダが見つかりません: " + d );
			}
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
		} ) );

		$TreeWidget::customContextMenuRequested( self, [&]( const QPoint& pos ) {
			QModelIndex index = self->indexAt( pos );
		if( !index.isValid() ) return;

		auto* item = dynamic_cast<ItemBuildTargetFile*>( reinterpret_cast<QTreeWidgetItem*>( index.internalPointer() ) );
		if( item ) {
			contextMenuFile->exec( self->viewport()->mapToGlobal( pos ) );
		}
		} );
#pragma endregion

		/////////////////////////////////////////
		// ダブルクリック
		$TreeWidget::itemDoubleClicked( self, [&]( QTreeWidgetItem* /*item*/, int /*column*/ ) {
			auto* fitem = self->currentItem<ItemBuildTargetFile>();
		if( fitem ) {
			//qtApp->play( fitem->srcPath );
			$::showInExplorer( fitem->srcPath );
		}
		} );

	}


	/////////////////////////////////////////
	void open( const QString& filePath ) {
		self->clear();

		if( !fs::isExistFile( filePath ) ) return;

		QList<QTreeWidgetItem*> lst;

		fs::readAllLines( filePath, [&]( const QString& s ) {
			if( s.isEmpty() )return;
		if( s[ 0 ] == '#' )return;
		lst << new ItemBuildTargetFile( path::getDirectoryName( filePath ), s );
		} );

		self->addTopLevelItems( lst );
	}


	/////////////////////////////////////////
	void build() {
		buildTargets = self->topLevelItems<ItemBuildTargetFile>();

		HLogView::info( u8"ビルドを開始します" );
		pd = std::make_unique<QProgressDialog>( self );
		pd->setModal( true );

		pd->setRange( 0, buildTargets.length() );
		pd->show();

		future = QtConcurrent::run( [&]() {

			auto toolPath = $$( "xwmaencode.exe" );
			auto PapyrusCompiler = $$( "PapyrusCompiler.exe" );

			QStringList envlst;
			envlst.append( $$( "%1/Tools/Audio" ).arg( utils::installGamePath() ) );
			envlst.append( $$( "%1/Papyrus Compiler" ).arg( utils::installGamePath() ) );
			qputenv( "PATH", envlst.join( ";" ).toUtf8() );

			for( int i = 0; i < buildTargets.length(); i++ ) {
				auto* item = buildTargets[ i ];

				watcher.progressValueChanged( i );

				if( item->papi ) {
					QStringList implst = config.ssePapyrusImportPaths;

					if( !$::isLastModified( item->srcFullPath(), item->destFullPath() ) )continue;

					QProcess process;
					QObject::connect( &process, &QProcess::readyReadStandardOutput, [&]() {
						QByteArray output = process.readAllStandardOutput();
					QString str = QString::fromLocal8Bit( output );

					auto log = str.replace( "\r", "" ).split( "\n" );
					//result << log;
					for( auto la : log ) {
						if( la.isEmpty() )continue;
						qDebug() << la;

					}
					} );
					process.setWorkingDirectory( path::getDirectoryName( item->srcFullPath() ) );

					QStringList args{
						path::getFileName( item->srcPath ),
						"-f=TESV_Papyrus_Flags.flg",
						$$( R"(-i=%1)" ).arg( implst.join( ";" ) ),
						$$( R"(-o=%1)" ).arg( item->folderPath + "/scripts" )
					};
					process.start( PapyrusCompiler, args );
					process.waitForFinished( -1 );
					//qDebug() << "PapyrusCompiler";
					//qDebug() << args[ 0 ] << " " << args[ 1 ] << " " << args[ 2 ] << " " << args[ 3 ];
				}
				else {
					auto destPath = item->destFullPath();

					if( !$::isLastModified( item->srcFullPath(), destPath ) )continue;

					if( path::hasSuffix( destPath, "wav" ) ) {
						fs::cp( item->srcPath, destPath );
					}
					else {
						QProcess p;

						p.setWorkingDirectory( item->folderPath );
						p.start( toolPath, { item->srcPath ,destPath } );
						p.waitForFinished( -1 );
					}
				}
			}

		} );
		watcher.setFuture( future );
	}
};



//////////////////////////////////////////////////////////////////////////////////
TreeViewBuild::TreeViewBuild( QWidget* parent ) :
	HTreeWidget( parent ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
void TreeViewBuild::setup() {
	impl->setup();
}

/////////////////////////////////////////
void TreeViewBuild::build() {
	impl->build();
}

/////////////////////////////////////////
void TreeViewBuild::open( const QString& filePath ) {
	impl->open( filePath );
}
