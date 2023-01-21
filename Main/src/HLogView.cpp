#include "HLogView.h"

//////////////////////////////////////////////////////////////////////////////////
class HLogView::Impl {
public:
	static HLogView* instance;

	/////////////////////////////////////////
	Impl( HLogView* _self ) {

		instance = _self;

		connect( instance, &HLogView::signal_addLog, instance, [&]( QString text, ELogType logType ) {
			QIcon icon;
			if( logType == ELogType::Info )icon = icon::SP_MessageBoxInformation();
			else if( logType == ELogType::Warning )icon = icon::SP_MessageBoxWarning();
			else if( logType == ELogType::Error )icon = icon::SP_MessageBoxCritical();

			auto* item = new QTreeWidgetItem();
			item->setText( 0, $::toString( QDateTime::currentDateTime(), true ) );
			item->setIcon( 1, icon );
			item->setText( 2, text );
			instance->addTopLevelItem( item );
			instance->scrollToBottom();

			qDebug() << text;
		} );

		connect( instance, &HLogView::signal_clearLog, instance, [&]() {
			instance->clear();
		} );
	}
};

HLogView* HLogView::Impl::instance;


//////////////////////////////////////////////////////////////////////////////////
HLogView::HLogView( QWidget* parent ) :
	QTreeWidget( parent ),
	impl( new Impl( this ) ) {

	setHeaderLabels( { "","","" } );
	setHeaderHidden( true );
	setRootIsDecorated(false);
	header()->setMinimumSectionSize(16);
	setAlternatingRowColors( true );
}

/////////////////////////////////////////
void HLogView::resizeEvent( QResizeEvent* ) {
	//auto w = width();

	setColumnWidth( 0, 110 );
	setColumnWidth( 1, 20 );
}


/////////////////////////////////////////
void HLogView::info( QString text ) {
	//UIPanelLog::Impl::instance->impl->info( text );
	emit HLogView::Impl::instance->signal_addLog( text, ELogType::Info );
}

/////////////////////////////////////////
void HLogView::warning( QString text ) {
	emit HLogView::Impl::instance->signal_addLog( text, ELogType::Warning );
}

/////////////////////////////////////////
void HLogView::error( QString text ) {
	emit HLogView::Impl::instance->signal_addLog( text, ELogType::Error );
}

/////////////////////////////////////////
void HLogView::clear() {
	emit HLogView::Impl::instance->signal_clearLog();


}
