#include "UIModFileView.h"
#include "ui_UIModFileView.h"

#include "UIMainWindow.h"
#include "UIModListView.h"
#include "HLogView.h"

#include "TESSystem.h"
#include "TESField.h"
#include "TESFieldReader.h"
#include "TESRecordTES4.h"

//////////////////////////////////////////////////////////////////////////////////
class UIModFileView::Impl : public Ui_UIModFileView {
public:
	QFuture<void> future;
	QFutureWatcher<void> watcher;

	UIModFileView* self;

	//QList<TESModFile*> mods;
	using aaa = void( UIModFileView::Impl::* )( TESForm* );

	QHash<quint32, aaa> handler;

	/////////////////////////////////////////
	Impl( UIModFileView* _self ) :
		self( _self ) {

		setupUi( self );

		handler.insert( TES4, &Impl::_TypeDraw );
		handler.insert( COBJ, &Impl::_TypeDraw );
		//handler.insert( TES4, &Impl::_TES4 );
		handler.insert( OMOD, &Impl::_OMOD );
		handler.insert( GMST, &Impl::_TypeDraw );
		//handler.insert( SCPT, &Impl::_Fallback );
	}

	TESForm* _formData;

	/////////////////////////////////////////
	void setup() {
		$CheckBox::stateChanged( checkBox, [&]( int /*state*/ ) {
			treeWidget->clear();

			auto it = handler.find( _formData->typeID() );

			if( checkBox->checkState() ) {
				_Fallback( _formData );
			}
			else {
				if( it == handler.constEnd() ) {
					_Fallback( _formData );
				}
				else {
					( this->* * it )( _formData );
				}
			}
		} );

		connect( qtWindow->modListView(), &UIModListView::signal_itemSelectionChanged, [&]( TESForm* form ) {
			treeWidget->clear();
			//if( !record )return;

			_formData = form;

			auto it = handler.find( form->typeID() );

			//if( form->typeID() == SCPT ) {
			//	toolArea->show();
			//}
			//else {
			//	toolArea->hide();
			//}
			if( checkBox->checkState() ) {
				_Fallback( form );
			}
			else {
				if( it == handler.constEnd() ) {
					_Fallback( form );
				}
				else {
					( this->* * it )( form );
				}
			}
		} );
	}

	/////////////////////////////////////////
	void _TypeDraw( TESForm* m_formData ) {
		auto& fi = m_formData->data()->fieldData;

		QHash<quint32, QList<quint32>> aaa;
		aaa.insert( TES4, { CNAM,SNAM,MAST,INTV,INCC } );
		aaa.insert( GMST, { EDID,DATA } );
		aaa.insert( COBJ, { EDID,YNAM,ZNAM  ,DESC,CNAM,BNAM ,FNAM,INTV,FVPA } );

		QList<QTreeWidgetItem*> lst;

		for( auto i : aaa[ m_formData->typeID() ] ) {
			auto* item = new QTreeWidgetItem();
			TESSignature sige( i );
			item->setText( 0, m_formData->data()->fieldTypeName( i ) );

			auto it = fi.find( i );
			if( it != fi.constEnd() ) {
				switch( it->type ) {
					case TESFieldValue::String:
					case TESFieldValue::Int32:
					case TESFieldValue::UInt32:
					case TESFieldValue::Float:
					case TESFieldValue::Bool:
						item->setText( 1, it->toString().replace( "\n", "" ) );
						break;
					case TESFieldValue::LString:
					{
						auto s = m_formData->file->strings( *it->uvalue );
						item->setText( 1, s.replace( "\n", "" ) );
						auto* b = new QTreeWidgetItem();
						b->setText( 0, "Hash" );
						b->setText( 1, $::toString( *it->uvalue ) );
						item->addChild( b );
						break;
					}
					case TESFieldValue::FormID:
					{
						item->setText( 1, m_formData->file->convertLoadOrderString( *it->uvalue ) );
						break;
					}
					case TESFieldValue::FormIDArray:
					{
						for( auto uvalue : *it->uvalueArray ) {
							auto* b = new QTreeWidgetItem();

							b->setText( 0, "Keyword" );
							b->setText( 1, m_formData->file->convertLoadOrderString( uvalue ) );

							item->addChild( b );
						}
						break;
					}
					case TESFieldValue::MasterData:
					{
						for( auto& m : *it->masterData ) {
							auto* b = new QTreeWidgetItem();
							b->setText( 0, "FileName" );
							b->setText( 1, $$( "%1, %2" ).arg( m.fileName ).arg( m.data ) );
							item->addChild( b );
						}
						break;
					}
					case TESFieldValue::CObjData:
					{
						auto* cobjData = (TESTypeCObjData*) it->uvalue;
						auto* b = new QTreeWidgetItem();
						b->setText( 0, "Created Object Count" );
						b->setText( 1, $::toString( cobjData->count ) );
						item->addChild( b );

						auto* c = new QTreeWidgetItem();
						c->setText( 0, "Priority" );
						c->setText( 1, $::toString( cobjData->pri ) );
						item->addChild( c );
						break;
					}
					case TESFieldValue::CObjCompenet:
					{
						for( auto& m : *it->uvalue64Array ) {
							auto* comp = (TESTypeCObjComponent*) &m;
							auto* b = new QTreeWidgetItem();
							b->setText( 0, "Component" );
							b->setText( 1, m_formData->file->convertLoadOrderString( comp->componentID ) );
							item->addChild( b );
							auto* c = new QTreeWidgetItem();
							c->setText( 0, "Count" );
							c->setText( 1, $::toString( comp->count ) );
							item->addChild( c );
						}
						break;
					}
				}
			}
			else {
				item->setTextColor( 0, "#888" );
			}
			lst << item;
		}

		treeWidget->addTopLevelItems( lst );
		treeWidget->expandAll();
	}

	/////////////////////////////////////////
	void _TES4( TESForm* m_formData ) {
#if false
		auto* tes4 = (TESRecordTES4*) m_formData->data();
		QList<QTreeWidgetItem*> lst;

		QList<void*> member{
			tes4->author,
			tes4->desc,
			0 < tes4->master.length() ? &tes4->master : nullptr,
			tes4->intv,
			tes4->incc,
		};
		QStringList editorName{
			"Author",
			"Description",
			"Master Files",
			"INTV",
			"INCC",
		};
		for( int i = 0; i < member.length(); i++ ) {
			auto* item = new QTreeWidgetItem();
			item->setText( 0, editorName[ i ] );
			if( member[ i ] == nullptr ) {
				item->setTextColor( 0, "#888" );
			}
			else {
				switch( i ) {
					case 0:
					case 1:
						item->setText( 1, *( (QString*) member[ i ] ) );
						break;
					case 2:
						for( auto& m : tes4->master ) {
							auto* b = new QTreeWidgetItem();
							b->setText( 0, "FileName" );
							b->setText( 1, $$( "%1, %2" ).arg( m.fileName ).arg( m.data ) );
							item->addChild( b );
						}
						break;

					case 3:
					case 4:
						item->setText( 1, $::toString( *( (qint32*) member[ i ] ) ) );
						break;
				}
			}
			lst << item;
		}

		treeWidget->addTopLevelItems( lst );
		treeWidget->expandAll();
#endif
	}

	/////////////////////////////////////////
	void _GMST( TESForm* m_formData ) {
		auto* r = (TESRecordGMST*) m_formData->data();
		QList<QTreeWidgetItem*> lst;

		r->fields( [&]( TESField* f, void* p ) {
			auto* item = new QTreeWidgetItem();

			auto* nam = tes4lib::id2Name( f->sign.typeID );
			if( !nam ) {
				item->setText( 0, f->sign.str() );
			}
			else {
				item->setText( 0, $$( "%1 - %2" ).arg( f->sign.str() ).arg( *nam ) );
			}

			switch( f->sign.typeID ) {
				case EDID:
					item->setText( 1, f->data<char>() );
					break;

				case DATA:
				{
					auto t = r->editorID()[ 0 ];
					if( t == 'i' ) {
						item->setText( 1, $::toString( *( (qint32*) p ) ) );
					}
					else if( t == 'f' ) {
						item->setText( 1, $::toString( *( (float*) p ), 'f', 5 ) );
					}
					else if( t == 's' ) {
						item->setText( 1, (char*) p );
					}
					break;
				}
			}

			lst << item;
		} );

		treeWidget->addTopLevelItems( lst );
		treeWidget->expandAll();
	}


	/////////////////////////////////////////
	void _OMOD( TESForm* m_formData ) {

		auto* r = (TESRecordOMOD*) m_formData->data();
		QList<QTreeWidgetItem*> lst;

		QList<void*> member{
			r->full,
			r->desc,
			r->targetOMODKeywords,
		};
		QStringList editorName{
			"Name",
			"Description",
			"Target OMOD Keywords",
		};

		QStringList  labels = { "", m_formData->file->fileName() };
		treeWidget->setHeaderLabels( labels );

		for( int i = 0; i < member.length(); i++ ) {
			auto* item = new QTreeWidgetItem();
			item->setText( 0, editorName[ i ] );
			if( member[ i ] == nullptr ) {
				item->setTextColor( 0, "#888" );
			}
			else {
				switch( i ) {
					case 0:
					case 1:
						item->setText( 1, *( (QString*) member[ i ] ) );
						break;
					case 2:
						for( int i = 0; i < r->targetOMODKeywords->keywords.length(); i++ ) {
							auto* b = new QTreeWidgetItem();
							b->setText( 0, $::toString( i ) );
							b->setText( 1, $$( "%1" ).arg( r->targetOMODKeywords->keywords[ i ], 8, 16, QChar( '0' ) ).toUpper() );
							item->addChild( b );
						}
						break;
						//case 2:
						//	for( auto& m : tes4->master ) {
						//		auto* b = new QTreeWidgetItem();
						//		b->setText( 0, "FileName" );
						//		b->setText( 1, $$( "%1, %2" ).arg( m.fileName ).arg( m.data ) );
						//		item->addChild( b );
						//	}
						//	break;

						//case 3:
						//case 4:
						//	item->setText( 1, $::toString( *( (qint32*) member[ i ] ) ) );
						//	break;
				}
			}
			lst << item;
		}

		treeWidget->addTopLevelItems( lst );
		treeWidget->expandAll();
	}


	/////////////////////////////////////////
	void _Fallback( TESForm* m_formData ) {
		auto* r = (TESRecordFallback*) m_formData->data();
		QList<QTreeWidgetItem*> lst;

		for( auto& byteData : r->data ) {
			auto* f = (TESField*) byteData.data();

			auto* item = new QTreeWidgetItem();

			/*		auto* nam = tes4lib::id2Name( f->sign.typeID );
					if( !nam ) {*/
			item->setText( 0, f->sign.str() );
			/*	}
				else {
					item->setText( 0, $$( "%1 - %2" ).arg( f->sign.str() ).arg( *nam ) );
				}*/

			switch( f->sign.typeID ) {
				case EDID:
				case MODL:
				case DMDL:
				case ICON:
				case MICO:
					item->setText( 1, f->data<char>() );
					break;
				case FULL:
				case DESC:
					if( r->form->file->isLocalized() ) {
						item->setText( 1, $$("%1)::%2").arg( *f->data<quint32>() ).arg(r->form->file->strings( *f->data<quint32>() )) );
					}
					else {
						item->setText( 1, f->data<char>() );
					}
					break;
				case OBND:
				{
					auto* obnd = f->data<TESTypeOBND>()->toPtr();
					QStringList lst{ "X1","Y1","Z1","X2" ,"Y2" ,"Z2" };
					for( int i = 0; i < 6; i++ ) {
						auto* x1 = new QTreeWidgetItem();
						x1->setText( 0, lst[ i ] );
						x1->setText( 1, $::toString( obnd[ i ] ) );
						item->addChild( x1 );
					}
					break;
				}
				//case SNAM:
				case VNAM:
				case WNAM:
				case SCRI:
				case SCRO:
				case MDOB:
				{
					quint32 id = m_formData->file->convertLoadOrder( *f->data<quint32>() );
					auto* form = tesSys.formData( id );
					if( form ) {
						item->setText( 1, $$( "[%2] %1" ).arg( form->editorName ).arg( tes4lib::toStringFromID( id ) ) );
					}
					else {
						auto s = tes4lib::toStringFromID( id );
						HLogView::error( "not found form: 0x" + s );
					}
					break;
				}
				case KSIZ:
					item->setText( 1, $::toString( *f->data<quint32>() ) );
					break;
					//case KWDA:
					//	item->setText( 1, $::toString( *( (qint32*) p ) ) );
					//	break;
				default:
				{
					QByteArray b( f->data<char>(), f->dataSize );
					item->setText( 1, $$( "(%2byte) %1" ).arg( QString( b.toHex() ) ).arg( f->dataSize ) );
					break;
				}
			}
			lst << item;
		}
		treeWidget->addTopLevelItems( lst );
		treeWidget->expandAll();
	}
	};


//////////////////////////////////////////////////////////////////////////////////
UIModFileView::UIModFileView( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		$::restoreState( impl->treeWidget->header(), config.modRightViewHeader );
		impl->setup();
	} );
}


/////////////////////////////////////////
UIModFileView::~UIModFileView() {
	$::saveState( config.modRightViewHeader, impl->treeWidget->header() );
}




