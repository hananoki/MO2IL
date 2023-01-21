#include "UIMainWindow.h"
#include "ui_UIMainWindow.h"


//#include "UIModFileView.h"
#include "ItemModFile.h"
#include "UIMO2ViewFile.h"

#include "TESStringTable.h"
#include "VFileSys.h"
#include "TESSystem.h"
#include "TESRecordTES4.h"

//////////////////////////////////////////////////////////////////////////////////
class UIMainWindow::Impl : public Ui_UIMainWindow {
public:

	UIMainWindow* self;

	QHash<quint32, QString> names;

	std::unique_ptr<TESStringTable> strings;

	/////////////////////////////////////////
	Impl( UIMainWindow* _self ) :
		self( _self ) {

		UIMainWindow::instance = self;
		setupUi( self );

		menubar->setVisible(false);

		//TESStrings s( "P:/Fallout4_Mods/FO4 ENG JP 194/Strings/Fallout4_en.STRINGS" );
		tabWidget->setTabVisible(1,false);

		$Action::triggered( actionDebug, [&]() {
			fileDB.dumpText();
		} );

		$Action::triggered( actionOpen, [&]() {
			//loadModFiles();
			QStringList output2;
			QStringList output;
			output << $$( R"(static void initIDName( QHash<quint32, QString>& names ) {)" );
			fs::readAllLines( "signature.txt", [&]( const QString& s ) {
				if( s.isEmpty() )return;
				if( s[ 0 ] == '#' )return;

				auto ss = s.split( "\t" );

				output2 << $$( R"(static const quint32 %1 = MAKE_SIG( '%2', '%3', '%4', '%5' );)" )
					.arg( ss[ 0 ] )
					.arg( ss[ 0 ][ 0 ] )
					.arg( ss[ 0 ][ 1 ] )
					.arg( ss[ 0 ][ 2 ] )
					.arg( ss[ 0 ][ 3 ] )
					;
				//static const quint32 ANAM = MAKE_SIG( 'A', 'N', 'A', 'M' );
				output << $$( R"(	names.insert( %1, "%2" );)" ).arg( ss[ 0 ] ).arg( ss[ 1 ] );
			} );
			output << $$( R"(})" );

			output2.append( output );

			fs::writeAllLines( "teslib/src/generate_signature.h", output2 );
		} );

		$Action::triggered( actionaa, [&]() {
			int count = 0;
			tesSys.enumerateFormByGroup( SCPT, [&]( TESForm* form ) {

				auto* r = (TESRecordSCPT*) form->data();
				auto src = r->scriptSource();
				if( src.isEmpty() )return;
				auto path = $$( "%1/%2/%3.sctx" )
					.arg( utils::targetGameTitleName() )
					.arg( form->file->fileName() )
					.arg( form->editorName );
				fs::writeAllText( path, src, Encoding::UTF8N );

				count++;
			} );
			qDebug() << count;
		} );
	}


	/////////////////////////////////////////
	void setup() {
		rollbackWindow();


		//tesSys.clear();
		//TESFile::loadFromFileAsync( "C:/Home/Games/Steam/steamapps/common/Skyrim Special Edition/Data/Skyrim.esm" );

		//connect( treeFileViewArea, &UIVirtualFileView::signal_itemSelectionChanged, [&]( ItemFile* item ) {
		//	if( path::hasSuffix( item->fullPath, "strings" ) || path::hasSuffix( item->fullPath, "dlstrings" ) ) {
		//		//qDebug() << "strings";
		//		treeWidget->clear();
		//		strings = std::make_unique<TESStringTable>( item->fullPath );

		//		QList<QTreeWidgetItem*> lst;
		//		for( auto& k : strings->keys() ) {
		//			auto* item = new QTreeWidgetItem();
		//			item->setText( 0, $::toString( k ) );
		//			item->setText( 1, ( *strings )[ k ] );
		//			lst << item;
		//		}
		//		treeWidget->addTopLevelItems( lst );
		//	}
		//} );

		connect( tabWidget, &QTabWidget::currentChanged, [&]( int index ) {
			config.tabIndex = index;
		} );
	}


	/////////////////////////////////////////
	void backupWindow() {
		config.pos = self->pos();
		config.size = self->size();
	}


	/////////////////////////////////////////
	void rollbackWindow() {
		self->move( config.pos );
		self->resize( config.size );
		tabWidget->setCurrentIndex( config.tabIndex );
		//$::restoreState( splitter, config.splitter );
	}
};

UIMainWindow* UIMainWindow::instance;


//////////////////////////////////////////////////////////////////////////////////
UIMainWindow::UIMainWindow( QWidget* parent ) :
	QMainWindow( parent ),
	impl( new Impl( this ) ) {
}


/////////////////////////////////////////
UIMainWindow::~UIMainWindow() {
}


/////////////////////////////////////////
void UIMainWindow::start() {
	impl->setup();

	emit signal_start();

	//fileDB.initMO2( utils::installGamePath(), config.getMO2Path(), config.seleectMO2ProfileName );
	//UIMainWindow::changePanel( EPanelMode::Files );

}


/////////////////////////////////////////
void UIMainWindow::closeEvent( QCloseEvent* /*event*/ ) {
	emit signal_closeWindow();
	impl->backupWindow();
}


/////////////////////////////////////////
UIModListView* UIMainWindow::modListView() {
	return impl->modListView;
}
/////////////////////////////////////////
UIMO2ViewFile* UIMainWindow::mo2View_mo2File() {
	return impl->mo2View->mo2File();
}


/////////////////////////////////////////
//UIVirtualFileView* UIMainWindow::fileView() {
//	return impl->treeFileViewArea;
//}

/////////////////////////////////////////
QTabWidget* UIMainWindow::tabWidget() {
	return impl->tabWidget;
}

/////////////////////////////////////////
UIMO2View* UIMainWindow::mo2View() {
	return impl->mo2View;
}

/////////////////////////////////////////
HTreeWidget* UIMainWindow::mo2ViewTreeView() {
	return impl->mo2View->treeView();
}

/////////////////////////////////////////
UIToolBar* UIMainWindow::toolBar() {
	return impl->toolbar;
}
