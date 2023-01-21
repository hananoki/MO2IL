#include "UIOverrideCheck.h"
#include "ui_UIOverrideCheck.h"

#include "UIMainWindow.h"
#include "ItemMO2View.h"
#include "ItemFile.h"
#include "ContextMenu.h"
#include <QProgressDialog>


//////////////////////////////////////////////////////////////////////////////////
class UIOverrideCheck::Impl : public Ui_UIOverrideCheck {
public:
	UIOverrideCheck* self;

	QFuture<void> future;
	QFutureWatcher<void> watcher;

	QMap<QString, QList<QString>> fileMapping;
	QList<QString> modFolderPaths;

	QString modFolderPath;
	QString overwriteFolderPath;

	std::unique_ptr<QProgressDialog> pd;

	std::unique_ptr<ContextMenu> contextMenu;


	/////////////////////////////////////////
	Impl( UIOverrideCheck* _self ) :
		self( _self ) {

		setupUi( self );

		treeWidget->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );

		
		/////////////////////////////////////////
		connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			pd->setValue( pd->maximum() );
			//future2 = QtConcurrent::run( this, &Impl::makeTree );
			//watcher2.setFuture( future2 );
		} );

		/////////////////////////////////////////
		connect( &watcher, &QFutureWatcher<void>::progressValueChanged, self, [&]( int progressValue ) {
			pd->setValue( progressValue );
			pd->setLabelText( path::getFileName( modFolderPaths[ progressValue ] ) );
		} );
	}


	/////////////////////////////////////////
	void setup() {
		$PushButton::click( button_override, std::bind( &Impl::on_button, this ) );

		utils::createFileContextMenu( &contextMenu, treeWidget );

		connect(
			self,
			&UIOverrideCheck::signal_addItem,
			self,
			std::bind( &QTreeWidget::addTopLevelItem, treeWidget, std::placeholders::_1 ) );

		$TreeWidget::itemDoubleClicked(
			treeWidget,
			std::bind( &Impl::_itemDoubleClicked, this ) );
	}


	/////////////////////////////////////////
	/// ダブルクリック
	void _itemDoubleClicked() {
		auto* item = treeWidget->currentItem<ItemFile>();
		if( !item ) return;
		$::showInExplorer( item->fullPath );
	}


	/////////////////////////////////////////
	QList<QString> makeUseModFolder() {
		QList<QString> result;

		auto mo2Path = config.getMO2Path();
		auto profileName = config.seleectMO2ProfileName;
		modFolderPath = $$( "%1/mods/" ).arg( mo2Path );
		auto modlist = $$( "%1/profiles/%2/modlist.txt" ).arg( mo2Path ).arg( profileName );

		auto mods = fs::readAllLines( modlist );
		auto rst = mods.rbegin();
		auto red = mods.rend();
		for( ; rst != red; rst++ ) {
			auto s = ( *rst );
			if( s[ 0 ] == '#' )continue;

			bool check = false;
			if( s[ 0 ] == '*' ) continue;
			if( s[ 0 ] == '-' )continue;

			if( s[ 0 ] == '+' ) {
				check = true;
			}

			result << modFolderPath + $$( s ).remove( 0, 1 );
		}

		result << $$( "%1/overwrite" ).arg( mo2Path );

		return result;
	}


	/////////////////////////////////////////
	void on_button() {
		treeWidget->clear();
		fileMapping.clear();
		qputenv( "PATH", $$( environment::currentDirectory() + "/bin" ).toUtf8() );

		HLogView::info( u8"競合ファイルのチェックを開始します" );
		pd = std::make_unique<QProgressDialog>( self );
		pd->setModal( true );

		modFolderPaths = makeUseModFolder();
		pd->setRange( 0, modFolderPaths.length() );
		pd->show();

		QStringList egnore;
		//egnore.append( "Unofficial Skyrim Modders Patch.bsa" );

		future = QtConcurrent::run( [&, egnore]() {
			int i = 0;

			for( auto& folderPath : modFolderPaths ) {
				watcher.progressValueChanged( i );
				fs::enumerateFiles( folderPath, "*.bsa", SearchOption::TopDirectoryOnly, [&]( const QString& filePath ) {
					if( 0 <= egnore.indexOf( path::getFileName( filePath ) ) ) {
						qDebug() << "";
						return true;
					}
					qInfo() << filePath;
					executeBsaFileMapping( filePath );
					return true;
				} );
				fs::enumerateFiles( folderPath, "*", SearchOption::AllDirectories, [&]( const QString& filePath ) {
					if( path::hasSuffix( filePath, "bsa" ) ) return true;
					if( path::hasSuffix( filePath, "txt" ) ) return true;
					if( path::hasSuffix( filePath, "esp" ) ) return true;
					//executeBsaFileMapping( filePath );
					auto fpath = filePath;
					fpath = fpath.replace( "\\", "/" ).replace( folderPath + "/", "" ).toLower();

					auto it = fileMapping.find( fpath );
					if( it == fileMapping.constEnd() ) {
						fileMapping.insert( fpath, QList<QString>() );
						it = fileMapping.find( fpath );
					}
					( *it ) << filePath;
					return true;
				} );
				i++;
			}

			makeTree();
		} );
		watcher.setFuture( future );
	}


	/////////////////////////////////////////
	void executeBsaFileMapping( QString bsaFilePath ) {
		QProcess process;
		QByteArray stdo;

		QObject::connect( &process, &QProcess::readyReadStandardOutput, [&]() {
			stdo.append( process.readAllStandardOutput() );
		} );

		QObject::connect( &process, qOverload<int>( &QProcess::finished ), [&]() {
			auto log = QString::fromLocal8Bit( stdo ).replace( "\r", "" ).split( "\n" );
			//result << log;
			for( auto la : log ) {
				if( la.isEmpty() )continue;

				la = la.replace( "\\", "/" );
				auto it = fileMapping.find( la );
				if( it == fileMapping.constEnd() ) {
					fileMapping.insert( la, QList<QString>() );
					it = fileMapping.find( la );
				}
				( *it ) << bsaFilePath;
			}
		} );

		QStringList args{
			"/s",
			bsaFilePath
		};
		process.start( "SimpleBSAExtractor", args );
		process.waitForFinished( -1 );
	}


	/////////////////////////////////////////
	void setDependencyFiles( const QList<QString>& a, HTreeWidgetItem* item, const QStringList& paths ) {
		if( paths.length() == 1 ) {
			item->setText( 1, $::toString( a.length() ) );
			for( auto& fname : a ) {
				item->addChild( new ItemFile( fname, 1, modFolderPath ) );
			}
		}
	}


	/////////////////////////////////////////
	void makeTree( const QList<QString>& a, HTreeWidgetItem* parent, QStringList paths ) {
		if( paths.length() == 0 ) return;

		HTreeWidgetItem* item = nullptr;
		if( parent ) {
			item = parent->find<HTreeWidgetItem>( paths[ 0 ] );
		}
		if( !item ) {
			item = new HTreeWidgetItem();
			item->setText( 0, paths[ 0 ] );

			setDependencyFiles( a, item, paths );

			if( parent ) {
				parent->addChild( item );
			}
		}
		paths.takeFirst();
		makeTree( a, item, paths );
	}



	/////////////////////////////////////////
	void makeTree() {
		QMap<QString, HTreeWidgetItem*> lst;

		for( auto& fname : fileMapping.keys() ) {
			if( fileMapping[ fname ].length() == 1 )continue;

			HTreeWidgetItem* item = nullptr;
			auto paths = fname.split( "/" );
			auto it = lst.find( paths[ 0 ] );

			if( it == lst.constEnd() ) {
				item = new HTreeWidgetItem();
				item->setText( 0, paths[ 0 ] );
				lst.insert( paths[ 0 ], item );
			}
			else {
				item = ( *it );
			}

			setDependencyFiles( fileMapping[ fname ], item, paths );
			paths.takeFirst();
			makeTree( fileMapping[ fname ], item, paths );
		}

		for( auto& fname : lst.keys() ) {
			emit self->signal_addItem( lst[ fname ] );
		}
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIOverrideCheck::UIOverrideCheck( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		$::restoreState( impl->treeWidget->header(), config.modRightViewHeader );
		impl->setup();
	} );
}


/////////////////////////////////////////
UIOverrideCheck::~UIOverrideCheck() {
}
