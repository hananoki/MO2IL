#include "UIToolBar.h"
#include "ui_UIToolBar.h"

#include "UIMainWindow.h"
#include "UISettings.h"
#include "VFileSys.h"
#include "UITranslator.h"
#include "UIPanelInfo.h"

//////////////////////////////////////////////////////////////////////////////////
class UIToolBar::Impl : public Ui_UIToolBar {
public:

	UIToolBar* self;


	/////////////////////////////////////////
	Impl( UIToolBar* _self ) :
		self( _self ) {

		setupUi( self );

		auto titles = utils::targetGameTitleList();
		comboBox->clear();
		$ComboBox::addItems( comboBox, titles );
		for( int i = 0; i < titles.length(); i++ ) {
			auto path = steam::gameExePath( titles[ i ] );
			if( !fs::isExistFile( path ) )continue;
			comboBox->setItemIcon( i, icon::get( path ) );
		}
		comboBox->bind( (int*) &config.targetGameTitle, &config );
		comboBox_2->bind( &config.seleectMO2ProfileName, &config );
	}


	/////////////////////////////////////////
	void initProfileComboBox() {
		HSignalBlocker _( self );
		comboBox_2->clear();
		fs::enumerateDirectories( config.getMO2Path() + "/profiles", "*", SearchOption::TopDirectoryOnly, [&]( const QString& userName ) {
			comboBox_2->addItem( path::getFileName( userName ) );
			return true;
		} );
		comboBox_2->setCurrentText( config.seleectMO2ProfileName );
	}


	/////////////////////////////////////////
	void setup() {
		HSignalBlocker _( self );
		initProfileComboBox();

		pushButton_2->setVisible( false );

		$PushButton::click( button_settings, std::bind( &Impl::_button_settings, this ) );

		$PushButton::clicked( button_refresh, std::bind( &Impl::_button_refresh, this ) );

		$PushButton::clicked( button_info, std::bind( &Impl::_button_info, this ) );

		$PushButton::clicked( pushButton_2, [&]( bool ) {
			auto* item = utils::currentSelectFile();
			if( !item ) {
				HLogView::warning( u8"アイテムを選択してください" );
				return;
			}

			auto* dlg = new UITranslator( self, item );
			dlg->show();
			//} );
		} );

		connect( comboBox, &HComboBox::currentTextChanged, [&]() {
			initProfileComboBox();
			emit self->signal_refresh();
		} );

		connect( comboBox_2, &HComboBox::currentTextChanged, [&]() {
			HLogView::warning( u8"プロファイルを切り替え > " + config.seleectMO2ProfileName );
			initProfileComboBox();
			emit self->signal_refresh();
		} );

		// 仮想ファイルの初期化が完了した時
		connect( &fileDB, &VFileSys::signal_initCompleted, [&]() {
			comboBox->setEnabled( true );
			comboBox_2->setEnabled( true );
		} );


		// ショートカット
		for( auto& s : config.toolPaths ) {
			if( !fs::isExistFile( s ) )continue;
			auto* p = new HFlatPushButton();
			p->setIcon( icon::get( s ) );
			connect( p, &HFlatPushButton::pressed, [s]() {
				auto bak = environment::currentDirectory();
				QDir::setCurrent( path::getDirectoryName( s ) );
				$::showInExplorer( s );
				QDir::setCurrent( bak );
			} );
			horizontalLayout->insertWidget( horizontalLayout->count() - 2, p );
		}
	}


	/////////////////////////////////////////
	void _button_settings() {
		$::showDialog<UISettings>( self );
	}


	/////////////////////////////////////////
	void _button_refresh() {
		initProfileComboBox();
		emit self->signal_refresh();
	}


	/////////////////////////////////////////
	void _button_info() {
		$::showDialog<UIPanelInfo>( self );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIToolBar::UIToolBar( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		//$::restoreState( impl->treeWidgetMod->header(), config.fileTreeHeader );
		impl->setup();
	} );
}

/////////////////////////////////////////
UIToolBar::~UIToolBar() {
}
