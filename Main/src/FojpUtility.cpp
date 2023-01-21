#include "FojpUtility.h"

#include "UIMainWindow.h"
#include "UIMO2View.h"
#include "ItemFolder.h"

#include <QDomDocument>
#include <QDomNode>
using namespace cpplinq;

//////////////////////////////////////////////////////////////////////////////////
class TranslatorData {
public:
	QString EDID;
	QString REC;
	QString Source;
	QString Dest;

	bool DOOR_FULL = false;
	bool AMMO_FULL = false;
	bool CHAL_FULL = false;
	bool CHAL_DESC = false;

	bool operator==( const TranslatorData& a ) {
		if( EDID != a.EDID )return false;
		if( Source != a.Source )return false;
		if( Dest != a.Dest )return false;
		return true;
	}
};


//////////////////////////////////////////////////////////////////////////////////
class FojpUtility::Impl {
public:

	FojpUtility* self;

	QFuture<void> future;
	QFutureWatcher<void> watcher;

	/////////////////////////////////////////
	Impl( FojpUtility* _self ) :
		self( _self ) {

		connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			HLogView::info( u8"処理が完了しました" );
		} );
		connect( &watcher, &QFutureWatcher<void>::progressTextChanged, self, [&]( const QString& progressText ) {
			HLogView::info( progressText );
		} );
	}


	/////////////////////////////////////////
	void convert() {
		HLogView::info( u8"変換を開始します" );

		future = QtConcurrent::run( [&]() {
			QList<TranslatorData> data;
		//auto files = fs::getFiles( config.inputTransPath, "*.xml" );
		//auto txtfiles = fs::getFiles( "txt", "*.txt" );
		QStringList lst;

		auto items = qtWindow->mo2View()->modFolderItemByChecked();

#if true

		auto readData = [&]( QStringList fileList ) {
			for( auto& p : fileList ) {
				//if( p.contains( "Universal" ) ) {
				//	qDebug() << "";
				//}
				auto files = fs::getFiles( config.fonvXMLPath, path::getBaseName( p ) + "*.xml" );

				for( auto& f : files ) {

					if( !$::isLastModified( f, "txt/" + path::getBaseName( f ) + ".txt" ) ) {
						//watcher.progressTextChanged( u8"テキストキャッシュから読み込み >> " + f );

						auto ll = fs::readAllLines( "txt/" + path::getBaseName( f ) + ".txt" );
						lst.append( ll );
					}
					else {
						watcher.progressTextChanged( u8"XMLからテキストに変換 >> " + f );
						auto transXML = parseXML( f );
						auto txt = from( transXML )
							>> select( []( const auto& x ) { return  x.Source + "\t" + x.Dest; } )
							>> to_qlist();

						txt.insert( 0, $$( u8"### %1\t###%1" ).arg( path::getFileName( f ) ) );

						auto door = from( transXML )
							>> where( []( const auto& x ) { return  x.DOOR_FULL; } )
							>> select( []( const auto& x ) { return  x.Source + u8" to[%]\t[%]へ続く" + x.Dest; } )
							>> to_qlist();

						auto ammo = from( transXML )
							>> where( []( const auto& x ) { return  x.AMMO_FULL; } )
							>> select( []( const auto& x ) { return   u8"[%] " + x.Source + u8"(s) added\t[%]個の" + x.Dest + u8"を入手"; } )
							>> to_qlist();

						QStringList chal_gen;
						auto CHAL_FULL = from( transXML )
							>> where( []( const auto& x ) { return  x.CHAL_FULL; } )
							>> to_qlist();
						auto CHAL_DESC = from( transXML )
							>> where( []( const auto& x ) { return  x.CHAL_DESC; } )
							>> to_qlist();

						//if( CHAL_FULL.length() == CHAL_DESC.length() ) {
						for( int i = 0; i < CHAL_FULL.length(); i++ ) {
							bool find = false;
							for( auto& cd : CHAL_DESC ) {
								if( CHAL_FULL[ i ].EDID == cd.EDID ) {
									chal_gen << CHAL_FULL[ i ].Source + " [%] " + cd.Source + "\t[%][CRLF][CRLF]" + cd.Dest;
									find = true;
									break;
								}
							}
							if( find )continue;
							watcher.progressTextChanged( u8"CHAL NotFound >> " + CHAL_FULL[ i ].EDID );
						}

						//txt.append( door );
						//txt.append( ammo );
						//txt.append( chal_gen );

						QStringList output;
						output << door;
						output << ammo;
						output << chal_gen;
						output << txt;

						output = from( output )
							>> distinct()
							>> to_qlist();

						fs::writeAllLines( "txt/" + path::getBaseName( f ) + ".txt", output );
						//data.append( transXML );
						lst.append( output );
					}
				}
			}
		};
		readData( QStringList{
			"FalloutNV.esm",
			"DeadMoney.esm",
			"HonestHearts.esm",
			"OldWorldBlues.esm",
			"LonesomeRoad.esm",
			"GunRunnersArsenal.esm",
			"ClassicPack.esm",
			"CaravanPack.esm",
			"MercenaryPack.esm",
			"TribalPack.esm"
			} );

#endif

		for( auto& item : items ) {
			readData( item->pluginFiles() );
		//	readData( item->sctxFiles );
		}

		QStringList en;
		QStringList jp;
		for( auto s : lst ) {
			if( s.isEmpty() )continue;
			auto ss = s.split( "\t" );
			en << ss[ 0 ];
			jp << ss[ 1 ];
		}

		auto outputTransPath = config.fonvFOJPTextPath;

		fs::writeAllLines( outputTransPath + "/xtFOJP_en.txt", en, Encoding::Shift_JIS );

		fs::writeAllLines( outputTransPath + "/xtFOJP_jp.txt", jp, Encoding::Shift_JIS );


		//auto fojp = fs::readAllLines( config.fojpPath );
		//QStringList output;
		//bool beginScope = false;
		//for( auto& l : fojp ) {
		//	if( !beginScope ) output.append( l );

		//	$::match( l, R"(<!-- x2FOJP Begin -->)", [&]( auto m ) {
		//		auto space = l.replace( "<!-- x2FOJP Begin -->", "" ).replace( "\r\n", "" );
		//		//for( var p in datas ) {
		//		output.append( space + $$( R"(<file jp="%1x2FOJP_jp.txt" en="%1x2FOJP_en.txt" />)" ).arg( config.addDataPath ) );
		//		//}
		//		//b.Append( $"{space}<file jp=\"{rela}NV Muffler_jp.txt\" en=\"{rela}NV Muffler_en.txt\" />" );

		//		beginScope = true;
		//	} );

		//	$::match( l, "<!-- x2FOJP End -->", [&]( auto m ) {
		//		output.append( l );
		//		beginScope = false;
		//	} );
		//}
		//fs::writeAllLines( config.fojpPath, output, Encoding::UTF8N );


		} );

		watcher.setFuture( future );
	}


	/////////////////////////////////////////
	QList<TranslatorData> parseXML( QString filePath ) {
		QList<TranslatorData> result;

		QDomDocument domDocument;
		domDocument.setContent( fs::readAll( filePath ) );

		for( auto n1 = domDocument.documentElement().firstChild(); !n1.isNull(); n1 = n1.nextSibling() ) {
			if( n1.toElement().tagName() != "Content" ) continue;
			for( auto n2 = n1.toElement().firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
				auto elem2 = n2.toElement();
				if( elem2.tagName() != "String" )continue;

				if( elem2.hasAttribute( "Partial" ) ) {
					auto partialType = $::toInt( elem2.attribute( "Partial" ) );
					// 仮和訳
					//if( partialType == 1 ) {
					//}

					// 翻訳不要
					if( partialType == 2 ) {
						continue;
					}
				}

				TranslatorData td;

				for( QDomNode n3 = n2.toElement().firstChild(); !n3.isNull(); n3 = n3.nextSibling() ) {
					auto tagName = n3.toElement().tagName();
					if( tagName == "EDID" ) {
						td.EDID = n3.firstChild().toText().data();
						//if( td.EDID .contains( "NVChallengeLocks" )) {
						//	qDebug() << "";
						//}
					}
					else if( tagName == "REC" ) {
						auto txt = n3.firstChild().toText().data();
						if( txt == "TES4:CNAM" )continue;

						if( txt == "AMMO:FULL" ) {
							td.AMMO_FULL = true;
						}
						if( txt == "DOOR:FULL" ) {
							td.DOOR_FULL = true;
						}
						if( txt == "CHAL:FULL" ) {
							td.CHAL_FULL = true;
						}
						if( txt == "CHAL:DESC" ) {
							td.CHAL_DESC = true;
						}
					}
					else if( tagName == "Source" ) {
						auto text = n3.firstChild().toText().data();
						if( n3.firstChild().toText().data().contains( "A Note on Temporary Companions" ) ) {
							qDebug() << "";
						}
						if( text.contains( "Case, " ) ) {
							td.AMMO_FULL = true;
						}
						//auto ss = text.data();
						auto ss2 = modifyKeyString( text );
						td.Source = ss2;
					}
					else if( tagName == "Dest" ) {
						td.Dest = modifyKeyString( n3.firstChild().toText().data() );
					}
				}

				//if( td.Source == td.Dest )continue;
				if( 0 <= result.indexOf( td ) )continue;
				result.append( td );
			}
		}

		return result;
	}


	/////////////////////////////////////////
	/// @brief  FOJPが認識する辞書キーに変換する
	QString modifyKeyString( QString  s ) {
		s.replace( QRegularExpression( "{.*?}" ), "" );
		s.replace( u8" – ", " " );
		s.replace( "\r", "" );
		s.replace( QRegularExpression( R"(
+$)" ), "" );
		s.replace( "\n", "[CRLF]" );
		s.replace( QRegularExpression( R"(%[0-9]?\.[0-9][fd])" ), "[%]" );
		s.replace( QRegularExpression( R"(%g)" ), "[%]" );

		s.replace( QRegularExpression( "&[pP][cC][nN][aA][mM][eE];" ), "[%]" );
		s.replace( QRegularExpression( R"(&s[a-zA-Z0-9]*;)" ), "[%]" );
		s.replace( QRegularExpression( R"(&-s[a-zA-Z0-9]*;)" ), "[%]" );

		s.replace( R"(%%)", "%" );
		s.replace( u8"¡", "!" );
		s.replace( u8"’", "'" );
		s.replace( u8"~", "" );
		if( s.contains( " & " ) ) {
			if( $::match( s, " & [0-9]" ) ) {
				s.replace( u8"&", "" );
			}
		}
		//s.replace( u8"&", "" );

		QString s12 = u8R"(ÀÁÂÄÆÇÈÉÊËÌÍÎÏÑÒÓÔÖÙÚÛÜßàáâäæçèéêëìíîïñòóôöùúûü‘’“”¡¿)";
		QString s13 = u8R"(AAAAACEEEEIIIINOOOOUUUUsaaaaaceeeeiiiinoooouuuu''""  )";
		for( int i = 0; i < s12.length(); i++ ) {
			s.replace( s12[ i ], s13[ i ] );
		}
		return s;
	}

};



//////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////
FojpUtility::FojpUtility() :
	impl( new Impl( this ) ) {
}


/////////////////////////////////////////
FojpUtility::~FojpUtility() {
}


/////////////////////////////////////////
void FojpUtility::convert() {
	impl->convert();
}

