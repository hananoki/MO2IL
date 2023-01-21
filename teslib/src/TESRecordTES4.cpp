#include "TESRecordTES4.h"
#include "TESType.h"
#include "TESFile.h"
#include "TESForm.h"
#include "TESField.h"

//////////////////////////////////////////////////////////////////////////////////
TESRecordTES4::TESRecordTES4( TESFile* f, TESRecord* r ) :
	TESFieldReader( f, r ) {

	QList<TESTypeMasterData> masterList;

	TESRecord::enumerateFields( r, [&]( TESField* f ) {
		switch( f->sign.typeID ) {
			case HEDR:
				header = *f->data<TESRecordTES4::Header>();
				break;

			case CNAM:
				fieldData.insert( CNAM, TESFieldValue( f->data<char>() ) );
				break;
				//����
			case SNAM:
				fieldData.insert( SNAM, TESFieldValue( f->data<char>() ) );
				break;

			case MAST:
				masterList << TESTypeMasterData();
				masterList.last().fileName = f->data<char>();
				break;

				//��� 0 �ł��B���Ղ̉\��������܂��B
				//Tes3�ł̓}�X�^�[�̃t�@�C���T�C�Y�������ɋL�^����Ă��܂����B Tes4 �ł́A�T�C�Y �R�[�h���z�u����Ă���悤�Ɍ����܂����A�@�\���Ă��炸�A�o�O���������Ă��邩�A�჌�x���Œ�������Ă���\��������܂��B
			case DATA:
				masterList.last().data = *f->data<quint64>();
				break;

				//�^�O�t���ł��镶����̐� (CK �� TagifyMasterfile �R�}���h���C�� �I�v�V�����ł̂ݎg�p)�B
			case INTV:
				fieldData.insert( INTV, TESFieldValue( *f->data<qint32>() ) );
				break;

				//�����̃J�E���^�[�B �}�X�^�[�Ɗ֌W������悤�ł��B
				//Update.esm �� Skyrim 1.6 �A�b�v�f�[�g�œ�������܂����B
			case INCC:
				fieldData.insert( INCC, TESFieldValue( *f->data<qint32>() ) );
				break;

				//�I�[�o�[���C�h���ꂽ�t�H�[��
				//���̃��R�[�h�́A�}�X�^�[�̃Z���̎q���I�[�o�[���C�h���� ESM �t���O�t���t�@�C���ɂ̂ݕ\������܂��B
				//ONAM �T�u���R�[�h�́A�I�[�o�[���C�h���ꂽ�Z���̎q( ACHR�ALAND�ANAVM�APGRE�APHZD�AREFR ) �� FormID �݂̂����X�g���܂��B
				//�p�b�` 1.5.24 �� Update.esm �Ŋm�F����Ă��܂��B
				//���R�[�h���́A�t�B�[���h �T�C�Y�݂̂Ɋ�Â��Ă��܂��B
			case ONAM:
				/*			for( qint32* st = f->data<qint32>(), *ed = (qint32*) f->next(); st < ed; st++ ) {
								overriddenForms << *st;
							}*/
				break;

			case TNAM:
			{
				/*			TESRecordTES4::TransientTypes tnam;
							tnam.formType = *f->data<qint32>();
							for( qint32* st = f->data<qint32>() + 1, *ed = (qint32*) f->next(); st < ed; st++ ) {
								tnam.data << *st;
							}
							transientTypes << tnam;*/
				break;
			}

			default:
				qDebug() << $$( "TES4(%1): %2" ).arg( file->fileName() ).arg( f->sign.str() );
				break;
		}


		return true;
	} );

	if( 0 < masterList.length() ) {
		fieldData.insert( MAST, TESFieldValue( masterList ) );
	}
}


QString TESRecordTES4::fieldTypeName( quint32 typeID ) {
	QHash<quint32, QString > a;
	a.insert( CNAM, "Author" );
	a.insert( SNAM, "Description" );
	a.insert( INTV, "INTV" );
	a.insert( INCC, "INCC" );
	a.insert( MAST, "Master Files" );
	auto it = a.find( typeID );
	if( it == a.constEnd() )return "";
	return *it;
}

QList<TESTypeMasterData> TESRecordTES4::master() {
	auto it = fieldData.find( MAST );
	if( it == fieldData.constEnd() )return QList<TESTypeMasterData>();
	return *it->masterData;
}


//////////////////////////////////////////////////////////////////////////////////
TESRecordGMST::TESRecordGMST( TESForm* form, TESRecord* r ) :
	TESFieldReader( form, r ) {

	fields( [=]( TESField* f, void* p ) {
		switch( f->sign.typeID ) {
			case DATA:
			{
				auto t = form->editorName[ 0 ];
				if( t == 'i' ) {
					fieldData.insert( DATA, TESFieldValue( *reinterpret_cast<int*>( p ) ) );
				}
				else if( t == 'u' ) {
					fieldData.insert( DATA, TESFieldValue( reinterpret_cast<quint32*> ( p ) ) );
				}
				else if( t == 'f' ) {
					fieldData.insert( DATA, TESFieldValue( *reinterpret_cast<float*>( p ) ) );
				}
				else if( t == 'b' ) {
					fieldData.insert( DATA, TESFieldValue( (bool) *reinterpret_cast<quint32*>( p ) ) );
				}
				else if( t == 's' ) {
					if( form->file->isLocalized() ) {
						fieldData.insert( DATA, TESFieldValue( *reinterpret_cast<quint32*>( p ), TESFieldValue::LString ) );
					}
					else {
						fieldData.insert( DATA, TESFieldValue( reinterpret_cast<char*>( p ) ) );
					}
				}
				break;
			}
			default:
				defaultParseField( f, p );
				break;
		}
	} );
}

/////////////////////////////////////////
QString TESRecordGMST::fieldTypeName( quint32 typeID ) {
	QHash<quint32, QString > a;
	a.insert( EDID, "Editor ID" );
	a.insert( DATA, "Value" );
	auto it = a.find( typeID );
	if( it == a.constEnd() )return "";
	return *it;
}


//////////////////////////////////////////////////////////////////////////////////
TESRecordCOBJ::TESRecordCOBJ( TESForm* form, TESRecord* r ) :
	TESFieldReader( form, r ) {

	fields( [&]( TESField* f, void* p ) {
		switch( f->sign.typeID ) {
			case CNAM:
			case BNAM:
				fieldData.insert( f->sign.typeID, TESFieldValue( *f->data<quint32>(), TESFieldValue::FormID ) );
				break;

			case INTV:
				fieldData.insert( f->sign.typeID, TESFieldValue( *f->data<quint32>(), TESFieldValue::CObjData ) );
				break;

			case FNAM:
			{
				QList<quint32> lst;
				auto count = f->dataSize / 4;
				for( auto i = 0; i < count; i++ ) {
					lst << reinterpret_cast<quint32*>( p )[ i ];
				}
				fieldData.insert( FNAM, TESFieldValue( lst, TESFieldValue::FormIDArray ) );
				break;
			}
			case FVPA:
			{
				QList<quint64> lst;
				auto count = f->dataSize / sizeof( quint64 );
				for( auto i = 0; i < count; i++ ) {
					lst << reinterpret_cast<quint64*>( p )[ i ];
				}
				fieldData.insert( FVPA, TESFieldValue( lst, TESFieldValue::CObjCompenet ) );
				break;
			}
			default:
				defaultParseField(f,p);
				break;
		}
	} );
}

QString TESRecordCOBJ::fieldTypeName( quint32 typeID ) {
	QHash<quint32, QString > a;
	a.insert( EDID, "Editor ID" );
	a.insert( DESC, "Description" );
	a.insert( YNAM, "Sound - Pick Up" );
	a.insert( ZNAM, "Sound - Pick Down" );
	a.insert( FVPA, "Components" );
	a.insert( DESC, "Description" );
	a.insert( CNAM, "Created Object" );
	a.insert( BNAM, "Workbench Keyword" );
	a.insert( ANAM, "Menu Art Object" );
	a.insert( FNAM, "Category" );
	a.insert( INTV, "Data" );
	auto it = a.find( typeID );
	if( it == a.constEnd() )return TESSignature( typeID ).str();
	return *it;
}


//////////////////////////////////////////////////////////////////////////////////
TESRecordOMOD::TESRecordOMOD( TESForm* form, TESRecord* r ) :
	TESFieldReader( form, r ),
	full( nullptr ),
	desc( nullptr ),
	modl( nullptr ),
	targetOMODKeywords( nullptr ) {

	TESRecord::enumerateFields( r, [&]( TESField* f ) {
		switch( f->sign.typeID ) {
			/*case FULL:
				full = new QString( tes4lib::getStrings( f, file ) );
				break;
			case DESC:
				desc = new QString( tes4lib::getStrings( f, file ) );
				break;*/
			case MODL:
				modl = new QString( f->data<char>() );
				break;
			case MODT:
				// ??
				break;
				//case MNAM:
				//	targetOMODKeywords = new TESTypeMNAM( f, file );
				//	break;
					//case OBND:
					//	objectBounds = new TESTypeOBND( *p->data<TESTypeOBND>() );
					//	break;
						//OMOD:  "EDID"
						//	OMOD : ""
						//	OMOD : "MODT"
						//	OMOD : "DATA"
			default:
				qDebug() << "OMOD: " << f->sign.str();
				break;
		}
		return true;
	} );
}


//////////////////////////////////////////////////////////////////////////////////
TESRecordSCPT::TESRecordSCPT( TESForm* form, TESRecord* r ) :
	TESFieldReader( form, r ) {
}



QString TESRecordSCPT::scriptSource() {
	auto* ch = at<char*>( SCTX );
	if( !ch )return "";
	return QString( QByteArray( ch ) );
}

//////////////////////////////////////////////////////////////////////////////////
TESRecordFallback::TESRecordFallback( TESForm* form, TESRecord* r ) :
	TESFieldReader( form, r ) {
}

TESRecordFallback::~TESRecordFallback() {
}
