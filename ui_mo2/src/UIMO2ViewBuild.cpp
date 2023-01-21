#include "UIMO2ViewBuild.h"
#include "ui_UIMO2ViewBuild.h"

#include "ui/UIMainWindow.h"
#include "ItemFolder.h"

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewBuild::Impl : public Ui_UIMO2ViewBuild {
public:
	using self_t = UIMO2ViewBuild;
	self_t* self;

	QString folderPath;

	int previewCount;
	QHash<int, QLabel*> previewLabel;

	QList<QWidget*> buttons;


	/////////////////////////////////////////
	Impl( self_t* _self ) :
		self( _self ) {

		setupUi( self );
		treeWidget->setup();
	}


	/////////////////////////////////////////
	void setup() {
		$PushButton::click( pushButton, std::bind( &TreeViewBuild::build, treeWidget ) );

		$PushButton::click( pushButton_2, [&]() {
			auto* item = treeWidget->currentItem<ItemFolderMO2View>();

			if( !item )return;

			auto fpath = item->fullPath + "/build.csv";
			$::showInExplorer( fpath );
		} );
	}


	/////////////////////////////////////////
	void initView( const QString& filePath ) {
		treeWidget->open( filePath );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIMO2ViewBuild::UIMO2ViewBuild(QWidget *parent) :
	QWidget(parent),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		//$::restoreState( impl->treeWidget->header(), config.UIMO2File_Header );
		impl->setup();
	} );
}


/////////////////////////////////////////
UIMO2ViewBuild::~UIMO2ViewBuild()
{
}


/////////////////////////////////////////
void UIMO2ViewBuild::initView( const QString& filePath ) {
	impl->initView( filePath );
}
