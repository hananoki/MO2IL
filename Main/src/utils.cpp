#include "utils.h"
#include "steam.h"
#include "TESFile.h"
#include "UIMainWindow.h"
#include "UIModListView.h"

#include "zlib.h"
#include "ItemFile.h"
#include "ItemFolder.h"
#include "ActionFile.h"
#include "ActionFolder.h"
#include "ContextMenu.h"

namespace utils {

	QStringList targetGameTitleList() {
		return QStringList{
			"Fallout 4",
			"Fallout New Vegas",
			"Skyrim Special Edition",
		};
	}

	QString targetGameTitleName() {
		return targetGameTitleList()[ config.targetGameTitle ];
	}

	QString installGamePath() {

		return steam::installGamePath( targetGameTitleName() );
	}


	ItemModFormID* currentSelectFile() {
		auto* item = qtWindow->modListView()->currentItem();
		return item;
	}


	/////////////////////////////////////////
	QStringList bsaExtractedFileList( const QString& filePath ) {
		QProcess process;
		QByteArray stdo;
		QStringList result;
		qputenv( "PATH", $$( environment::currentDirectory() + "/bin" ).toUtf8() );

		QObject::connect( &process, &QProcess::readyReadStandardOutput, [&]() {
			stdo.append( process.readAllStandardOutput() );
		} );

		QObject::connect( &process, qOverload<int>( &QProcess::finished ), [&]() {
			auto lst = QString::fromLocal8Bit( stdo ).replace( "\r", "" ).split( "\n" );
			using namespace cpplinq;

			result = from( lst )
				>> where( []( const QString& s ) { return !s.isEmpty(); } )
				>> to_qlist();
		} );

		QStringList args{
			"/s",
			filePath
		};

		process.start( "SimpleBSAExtractor", args );
		process.waitForFinished( -1 );

		return result;
	}


	/////////////////////////////////////////
	void createFileContextMenu( std::unique_ptr<ContextMenu>* contextMenu, HTreeWidget* treeWidget ) {
		$TreeWidget::customContextMenuRequested( treeWidget, [treeWidget, contextMenu]( const QPoint& pos ) {
			QModelIndex index = treeWidget->indexAt( pos );
			if( !index.isValid() ) return;

			auto* item = dynamic_cast<ItemFile*>( reinterpret_cast<QTreeWidgetItem*>( index.internalPointer() ) );
			if( item ) {
				( *contextMenu ) = std::make_unique<ContextMenu>();
				( *contextMenu )->addAction<ActionOpenFile>( treeWidget );
				( *contextMenu )->addAction<ActionShowInExplorer>( treeWidget );
				( *contextMenu )->addAction<ActionCopyFileName>( treeWidget );
				( *contextMenu )->addSeparator();
				( *contextMenu )->addAction<ActionOpenText>( treeWidget );
				( *contextMenu )->addAction<ActionOpenBSA>( treeWidget );
				( *contextMenu )->addAction<ActionOpenNIF>( treeWidget );
				( *contextMenu )->addAction<ActionOpenESP>( treeWidget );
				( *contextMenu )->exec( treeWidget->viewport()->mapToGlobal( pos ) );
			}
		} );
	}


	/////////////////////////////////////////
	void createFolderContextMenu( std::unique_ptr<ContextMenu>* contextMenu, HTreeWidget* treeWidget ) {
		$TreeWidget::customContextMenuRequested( treeWidget, [treeWidget, contextMenu]( const QPoint& pos ) {
			QModelIndex index = treeWidget->indexAt( pos );
			if( !index.isValid() ) return;

			auto* item = dynamic_cast<ItemFolder*>( reinterpret_cast<QTreeWidgetItem*>( index.internalPointer() ) );
			if( item ) {
				( *contextMenu ) = std::make_unique<ContextMenu>();
				( *contextMenu )->addAction<ActionOpenFolder>( treeWidget );
				( *contextMenu )->addAction<ActionFolderPathCopy>( treeWidget );

				auto* item2 = dynamic_cast<ItemFolderMO2View*>( item );
				if( item2 ) {
					( *contextMenu )->addSeparator();
					( *contextMenu )->addAction<ActionFolderOpenNexus>( treeWidget );
				}

				( *contextMenu )->exec( treeWidget->viewport()->mapToGlobal( pos ) );
			}

		} );
	}


	/////////////////////////////////////////
	QString nexusURL( int modID ) {
		QStringList title{
			"fallout4",
			"newvegas",
			"skyrimspecialedition",
			//"fallout3"
		};

		return $$( "https://www.nexusmods.com/%1/mods/%2" )
			.arg( title[ config.targetGameTitle ] )
			.arg( modID );
	}

	QStringList& expandTree() {
		if( config.targetGameTitle == EGameTitle::Fallout4 ) return config.fo4ExpandTree;
		if( config.targetGameTitle == EGameTitle::FalloutNV ) return config.fonvExpandTree;
		return  config.sseExpandTree;
	}

};


