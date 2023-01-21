#include "UITranslator.h"
#include "ui_UITranslator.h"

#include "ItemModFile.h"

#include "TESField.h"
#include "TESFieldReader.h"
#include "TESFile.h"
#include "TESForm.h"
#include "TESRecordTES4.h"

#pragma warning(disable:4311)
#pragma warning(disable:4302)
#pragma warning(disable:4318)

QDataStream& operator<<( QDataStream& out, const TESGroup& group ) {
	out << group.sign.typeID;
	out << group.DataSize;
	out << group.groupName.typeID;
	out << group.groupType;
	out << group.timeStamp;
	out << group.versionControlInfo;
	out << group.unknown;
	return out;
}
QDataStream& operator<<( QDataStream& out, const TESRecord& r ) {
	out << r.sign.typeID;
	out << r.dataSize;
	out << r.RecordFlags;
	out << r.formID;
	out << r.VersionControlInfo1;
	out << r.FormVersion;
	out << r.VersionControlInfo2;
	return out;
}
QDataStream& operator<<( QDataStream& out, TESField* f ) {
	out << f->sign.typeID;
	out << f->dataSize;

	quint8* p = ( (quint8*) f ) + sizeof( TESField );
	quint8* ed = p + f->dataSize;
	while( p != ed ) {
		out << *p++;
	}
	return out;
}
QDataStream& operator<<( QDataStream& out, TESForm* form ) {
	auto* reader = form->data();
	out << reader->record;
	if( reader->record.sign.typeID == NAVM ) {
	}
	else if( reader->record.isCompressed() ) {
		//qDebug() << "";
		for( quint32 i = 0; i < form->data()->record.dataSize; i++ ) {
			out << form->compressedData[ i ];
		}
	}
	else {
		reader->fields( [&]( TESField* f, void* ) {
			out << f;
		} );
	}
	return out;
}
//QDataStream& operator<<( QDataStream& out, QList<CellBlock*> cell ) {
//	for( auto& m : cell ) {
//		out << m->group;
//		for( auto& s : m->subBlocks ) {
//			out << s->group;
//			for( auto& c : s->cells ) {
//				out << c->form;
//				if( c->cellChildren ) {
//					out << *c->cellChildren;
//					if( c->cellPersistentChilden ) {
//						out << *c->cellPersistentChilden;
//						for( auto& p : c->persistent ) {
//							out << p;
//						}
//					}
//					if( c->cellTemporaryChildren ) {
//						out << *c->cellTemporaryChildren;
//						for( auto& p : c->temporary ) {
//							out << p;
//						}
//					}
//				}
//			}
//		}
//	}
//	return out;
//}
QDataStream& operator<<( QDataStream& out, TESGroupNode* groupNode ) {
	out << *groupNode->groupData;
	for( auto& fm : groupNode->forms ) {
		out << fm;
		if( fm->groupNode ) {
			out << fm->groupNode;
		}
	}
	for( auto& c : groupNode->nodes ) {
		out << c;
	}
	return out;
}


//////////////////////////////////////////////////////////////////////////////////
class ItemTranslator : public HTreeWidgetItem {
public:
	QList<TESForm*> formList;

	ItemTranslator( QList<TESForm*> _formList ) :
		formList( _formList ) {
	}

};


//////////////////////////////////////////////////////////////////////////////////
class UITranslator::Impl : public Ui_UITranslator {
public:
	struct IDString {
		quint32 stringID;
		QByteArray textBytes;
		IDString( quint32 _stringID, QByteArray _text ) :
			stringID( _stringID ),
			textBytes( _text ) {

			if( textBytes.length() == 0 ) {
				textBytes.append( (char) 0 );
			}
			else {
				//int len = text.length();
				quint8 ch = textBytes[ textBytes.length() - 1 ];
				if( ch != 0 ) {
					textBytes.append( (char) 0 );
				}
			}
		}
	};
	using OutputStringList = QList<IDString>;

	UITranslator* self;
	TESForm* m_formData;
	TESFile* m_file;

	QList<quint32> m_targetRecords;
	QHash<quint32, QList<quint32>> m_targetFields;

	QList<TESForm*> m_allTargetForms;



	/////////////////////////////////////////
	Impl( UITranslator* _self, ItemModFormID* _item ) :
		self( _self ),
		m_formData( _item->m_formData ),
		m_file( _item->m_formData ->file ) {

		setupUi( self );

		makeTargetIDs();

		self->setWindowTitle( _item->m_formData->file->fileName() );
		label->setHidden( !_item->m_formData->file->isLocalized() );

		QList<QTreeWidgetItem*> lst;

		// ターゲットとなるフォームから翻訳対象が含まれるフォームを割り出してアイテムにする
		for( auto tid : m_targetRecords ) {
			TESSignature sign( tid );

			auto forms = _item->m_formData->file->formDataList( tid );

			// グループIDのフォームが無い
			if( forms.length() == 0 )continue;

			// 対象フォームと翻訳件数の割り出し
			QList<TESForm*> targetForm;
			int count = 0;
			for( auto* form : forms ) {
				auto tlist = m_targetFields[ form->typeID() ];
				auto* reader = form->data();
				bool find = false;
				reader->fields( [&]( TESField* f, void* p ) {
					// FULLやDESCなど翻訳フィールドが存在するか
					if( tlist.indexOf( f->sign.typeID ) < 0 )return;
					if( !form->file->isLocalized() ) {
						if( *( (char*) p ) == 0 )return;
					}
					count++;
					find = true;
				} );
				if( !find )continue;
				targetForm << form;
				m_allTargetForms << form;
			}
			// このグループ内のフォームに翻訳するテキストが無い場合
			if( targetForm.length() == 0 )continue;

			auto* item = new ItemTranslator( targetForm );

			item->setText( 0, sign.str() + $$( " (%1)" ).arg( count ) );

			lst << item;
			//}
		}
		treeWidget_L->addTopLevelItems( lst );

		$PushButton::clicked( button_makeStrings, [&]( bool ) {_button_makeStrings(); } );

		/// アイテムの選択切り替え
		$TreeWidget::itemSelectionChanged( treeWidget_L, [&]() {_itemSelectionChanged(); } );

		HLogView::info( "copy" );
		QStringList path{
	"_en.STRINGS",
	"_en.DLSTRINGS",
	"_en.ILSTRINGS"
		};
		if( config.targetGameTitle == EGameTitle::SkyrimSE ) {
			path = QStringList{
				"_english.strings",
				"_english.dlstrings",
				"_english.ilstrings"
			};
		}
		QStringList path2{
"_ja.STRINGS",
"_ja.DLSTRINGS",
"_ja.ILSTRINGS"
		};
		if( config.targetGameTitle == EGameTitle::SkyrimSE ) {
			path2 = QStringList{
				"_japanese.strings",
				"_japanese.dlstrings",
				"_japanese.ilstrings"
			};
		}

		for( int i = 0; i < 3; i++ ) {
			auto Ename = path::getBaseName( m_file->fileName() ) + path[ i ];
			auto Jname = path::getBaseName( m_file->fileName() ) + path2[ i ];

			auto targetName = path::getDirectoryName( m_file->fullPath() ) + "/Strings/" + Jname;
			auto mo2overPath = $$( "%1/overwrite/Strings/%2" ).arg( config.getMO2Path() ).arg( Ename );
			if( fs::isExistFile( targetName ) ) {
				fs::cp( targetName, mo2overPath );
			}
		}
	}


	/////////////////////////////////////////
	void makeTargetIDs() {
		fs::readAllLines( ":/res/" + utils::targetGameTitleName() + "/TranslatorType.txt", [&]( const QString& s ) {
			auto ss = s.split( "\t" );
			auto rt = TESSignature( ss[ 0 ] );
			m_targetRecords << rt.typeID;

			QList<quint32> tf;
			for( int i = 1; i < ss.length(); i++ ) {
				tf << TESSignature( ss[ i ] ).typeID;
			}
			m_targetFields.insert( rt.typeID, tf );
		} );
	}


	/////////////////////////////////////////
	void _button_makeStrings() {
		using namespace cpplinq;

		if( m_file->isLocalized() ) {
			HLogView::warning( "Localized" );
			return;
		}
		quint32 stringID = 0;
		OutputStringList strings;
		OutputStringList dlstrings;
		OutputStringList iltrings;
		QList<QByteArray> dup;
		QList<QByteArray> ildup;
		QList<QByteArray> dldup;
		QMap<QByteArray, quint32> map;
		QMap<QByteArray, quint32> ilmap;
		QMap<QByteArray, quint32> dlmap;

		for( auto* form : m_allTargetForms ) {
			form->data()->localizeFields( m_targetFields[ form->typeID() ], [&]( QByteArray& fba,TESField* f, void* p, TESStringsType t ) {
				QByteArray textBytes = QByteArray( reinterpret_cast<char*>( p ), f->dataSize );

				//if( stringID == 131 ) {
				//	qDebug() << "";
				//}

				if( t == TESStringsType::DLStrings ) {
					if( dldup.indexOf( textBytes ) < 0 ) {
						dlstrings << IDString( stringID, textBytes );
						dldup << textBytes;
						dlmap.insert( textBytes, stringID );
						stringID++;
					}
				}
				else if( t == TESStringsType::ILStrings ) {
					if( ildup.indexOf( textBytes ) < 0 ) {
						iltrings << IDString( stringID, textBytes );
						ildup << textBytes;
						ilmap.insert( textBytes, stringID );
						stringID++;
					}
				}
				else {
					if( dup.indexOf( textBytes ) < 0 ) {
						strings << IDString( stringID, textBytes );
						dup << textBytes;
						map.insert( textBytes, stringID );
						stringID++;
					}
				}
			} );
		}

		writeStringsFile( QList<OutputStringList> {strings, dlstrings, iltrings} );

		writePluginFile( map, dlmap, ilmap );
	}


	/////////////////////////////////////////
	void writeStringsFile( QList<OutputStringList> target ) {
		auto  getSize = []( OutputStringList& ss, int i )->int {
			int size = 0;
			for( auto& s : ss ) {
				size += s.textBytes.length();
				if( 1 <= i ) {
					size += 4;
				}
			}
			return size;
		};

		QStringList path{
			"_en.STRINGS",
			"_en.DLSTRINGS",
			"_en.ILSTRINGS"
		};
		if( config.targetGameTitle == EGameTitle::SkyrimSE ) {
			path = QStringList{
				"_english.strings",
				"_english.dlstrings",
				"_english.ilstrings"
			};
		}

		for( int i = 0; i < 3; i++ ) {
			auto fname = path::getBaseName( m_file->fileName() ) + path[ i ];
			auto outputPath = utils::targetGameTitleName() + "/Strings/" + fname;
			binarySerializer::write( outputPath, [&]( auto& out ) {
				out.setByteOrder( QDataStream::LittleEndian );

				auto& t = target[ i ];
				//out.setGenerateByteOrderMark( true );

				if( t.length() == 0 ) {
					out << 0;
					out << 0;
				}
				else {
					out << t.length();
					out << getSize( t, i );
					quint32 offset = 0;
					for( auto& s : t ) {
						out << s.stringID << offset;
						offset += ( s.textBytes.length() );
						if( 1 <= i ) {
							offset += 4;
						}
					}
					for( auto& s : t ) {
						if( 1 <= i ) {
							out << s.textBytes.length();
						}
						for( quint8 c : s.textBytes ) {
							out << c;
						}
					}
				}
			} );
			fs::cp( outputPath, path::getDirectoryName( m_file->fullPath() ) + "/Strings/" + fname );
		}
	}


	/////////////////////////////////////////
	void writePluginFile( QMap<QByteArray, quint32> str2id, QMap<QByteArray, quint32> DLstr2id, QMap<QByteArray, quint32> ILstr2id ) {
		// フィールド内の書換え
		for( auto* form : m_allTargetForms ) {
			form->data()->localizeFields( m_targetFields[ form->typeID() ], [&]( QByteArray& fba, TESField* f, void* p, TESStringsType t ) {
				QByteArray textBytes = QByteArray( reinterpret_cast<char*>( p ), f->dataSize );
				QByteArray old( fba );
				fba.clear();
				fba.append( old.data(), 4 );
				/*		if( t == TESStringsType::Strings ) {
							map
						}*/
				quint16 size = 4;
				fba.append( (char*) &size, 2 );
				//quint32 strID = map[ txt ].stringID;
				switch( t ) {
					case TESStringsType::Strings:
						fba.append( (char*) &str2id[ textBytes ], 4 );
						break;
					case TESStringsType::DLStrings:
						fba.append( (char*) &DLstr2id[ textBytes ], 4 );
						break;
					case TESStringsType::ILStrings:
						fba.append( (char*) &ILstr2id[ textBytes ], 4 );
						break;
				}
			} );

			auto* reader = form->data();
			reader->record.dataSize = reader->fieldDataSize();
		}

		/// group ize
		for( auto& g : m_file->topGroups() ) {
			//if( g->groupName().typeID == CELL ) {
				//auto size = recalcCellData();
			g->calcDataSize();
			//}
			//else {
			//	auto& flist = m_file->formDataList( g->typeID() );
			//	quint32 size = 0;
			//	for( auto* fm : flist )
			//		size += fm->data()->record.dataSize + sizeof( TESRecord );
			//	g->groupData->DataSize = size + sizeof( TESGroup );
			//}
		}

		fs::mv( m_file->fullPath(), m_file->fullPath() + ".bak" );

		auto fname = m_file->fileName();
		auto outputPath = path::getDirectoryName( m_file->fullPath() ) + "/" + fname;

		binarySerializer::write( outputPath, [&]( auto& out ) {
			out.setByteOrder( QDataStream::LittleEndian );

			auto* tes4 = m_file->_TES4();
			tes4->record.RecordFlags |= (quint32) Tes4Flag::Localized;
			out << tes4->record;

			tes4->fields( [&]( TESField* f, void* ) {
				out << f;
			} );

			for( auto& g : m_file->topGroups() ) {

				//if( g->typeID() == CELL ) {
					//out << m_file->blocks();
				out << g;
				//}
				//else {
				//	out << *g->groupData;
				//	for( auto& fm : m_file->formDataList( g->typeID() ) ) {
				//		auto* reader = fm->data();
				//		out << reader->record;
				//		reader->fields( [&]( TESField* f, void* ) {
				//			out << f;
				//		} );
				//	}
				//}
			}
		} );

		//fs::cp( outputPath, path::getDirectoryName( m_file->fullPath() ) + "/Strings/" + fname );
	}


	/////////////////////////////////////////
	void _itemSelectionChanged() {
		treeWidget_R->clear();

		auto* item = treeWidget_L->currentItem<ItemTranslator>();
		if( !item )return;

		QList<QTreeWidgetItem*> lst;


		for( auto* form : item->formList ) {
			auto* reader = form->data();

			auto transTargetFieldIDs = m_targetFields[ form->typeID() ];

			reader->fields( [&]( TESField* f, void* p ) {
				if( transTargetFieldIDs.indexOf( f->sign.typeID ) < 0 )return;

				QString txt;
				if( form->file->isLocalized() ) {
					//auto s = form->file->strings( *f->data<quint32>() );
					//fieldData.insert( DATA, TESFieldValue( *f->data<quint32>(), TESFieldValue::LString ) );
					txt = form->file->strings( *reinterpret_cast<quint32*>( p ) );
				}
				else {
					txt = QString( reinterpret_cast<char*>( p ) );
					if( txt.isEmpty() )return;
				}
				QTreeWidgetItem* item = nullptr;
				item = new QTreeWidgetItem();
				item->setText( 4, txt );
				item->setText( 0, form->sign().str() );
				item->setText( 1, $::toStringHex( form->ID ) );
				item->setText( 2, form->editorName );
				item->setText( 3, f->sign.str() );
				lst << item;
			} );
		}

		treeWidget_R->addTopLevelItems( lst );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UITranslator::UITranslator( QWidget* parent, ItemModFormID* item ) :
	QDialog( parent ),
	impl( new Impl( this, item ) ) {

	//move( config.transPos );
	resize( config.transSize );
	$::restoreState( impl->splitter, config.transSplitter1 );
	$::restoreState( impl->splitter_2, config.transSplitter2 );
}


/////////////////////////////////////////
UITranslator::~UITranslator() {
	config.transPos = pos();
	config.transSize = size();
	$::saveState( config.transSplitter1, impl->splitter );
	$::saveState( config.transSplitter2, impl->splitter_2 );
}


/////////////////////////////////////////
void UITranslator::closeEvent( QCloseEvent* /*event*/ ) {
	delete impl->self;
}
