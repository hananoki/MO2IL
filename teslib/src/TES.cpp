
#include "TESFile.h"
#include "TESType.h"
#include "TESSystem.h"
#include "TESField.h"

#pragma warning(disable:4311)
#pragma warning(disable:4302)
//TESRecordTES4::TESRecordTES4( TesFileHeader* filedBegin, TesFileHeader* filedEnd ){
//	//char* author = offsetPtr<char>( cnam, sizeof( TesField ) );
//}


//////////////////////////////////////////////////////////////////////////////////
//QString TESRecord::lstring() {
//	if( isLocalized() ) {
//		QString tt = ( *STRINGS )[ *( (quint32*) c ) ];
//		return tt;
//	}
//	return  QString::fromUtf8( c );
//}




namespace zlib {
	int compress( Bytef* , uInt , const Bytef* source, uInt sourceLen ) {
		
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;

		int ret = deflateInit( &strm, Z_DEFAULT_COMPRESSION );
		assert( ret == Z_OK );

		Bytef* dest = new quint8[ 40960 ];
		memset( dest, 0, 40960 );
		uLongf destLen = 40960;
		int nn =::compress( dest , &destLen, source, sourceLen );

		Bytef* dest2 = new quint8[ 40960 ];
		zlib::uncompress( dest2, 40960, dest ,40960 );

		//int flush = Z_NO_FLUSH;
		// ret = Z_OK;
		////do {
		//	strm.next_in = (Bytef*) source;
		//	strm.avail_in = sourceLen;
		//	//if(/* 入力が尽きていれば */ ) {
		//	//	flush = Z_FINISH;
		//	//}
		//	do {
		//		strm.next_out = ( Bytef* ) dest;
		//		strm.avail_out = 40960;
		//		ret = deflate( &strm, flush );
		//		if( ret == Z_STREAM_ERROR ) {
		//			//assert( ret != Z_STREAM_ERROR );
		//			return Z_STREAM_ERROR;
		//		}
		//		// 出力バッファの中身を処理
		//	} while( strm.avail_out == 0 );
		//	assert( strm.avail_in == 0 );
		////} while( flush != Z_FINISH );
		////assert( ret == Z_STREAM_END );

		return ret;
	}

	int uncompress( Bytef* dest, uInt destLen, const Bytef* source, uInt sourceLen ) {
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;

		strm.next_in = Z_NULL;
		strm.avail_in = 0;
		int ret2 = inflateInit( &strm );
		assert( ret2 == Z_OK );

		int ret = Z_OK;
		//do {
		strm.next_in = (Bytef*) source;
		strm.avail_in = sourceLen;
		assert( strm.avail_in != 0 );
		//do {
		strm.next_out = (Bytef*) dest;
		strm.avail_out = destLen;

		ret = inflate( &strm, Z_NO_FLUSH );

		return ret;
	}
}


//////////////////////////////////////////////////////////////////////////////////
namespace tes4lib {
	QString getStrings( TESField* field, TESFile* file ) {
		if( file->isLocalized() ) {
			QString tt = file->strings( *field->data<quint32>() );
			return tt;
		}
		return  QString::fromUtf8( field->data<char>() );
	}

	//void read( TESRecordTES4* tes4, TESField* field, TESField* fieldEnd ) {

	//	for( ; (int) field < (int) fieldEnd; field = field->next() ) {

	//		switch( field->sign.typeID ) {
	//			

	//			

	//			default:
	//				qDebug() << "TES4: " << field->sign.str();
	//				break;
	//		}
	//	}
	//}

	QHash<quint32, QString> names;

	void init() {
		if( !names.isEmpty() )return;

		//#include "gen_signature_init.txt"
		initIDName( names );

#if 0
		names.insert( ACHR, "Actor Reference" );
		names.insert( ACTI, "Activator" );
		names.insert( ADDN, "Addon Node" );
		names.insert( ALCH, "Ingestible" );//Potion
		names.insert( AMMO, "Ammunition" );//Ammo
		names.insert( ANIO, "Animated Object" );//Animation Object
		names.insert( APPA, "Alchemical Apparatus" );//Apparatus (probably unused)
		names.insert( ARMA, "Armor Addon" );//Armor Addon (Model)
		names.insert( ARMO, "Armor" );
		names.insert( ARTO, "Art Object" );
		names.insert( ASPC, "Acoustic Space" );
		names.insert( ASTP, "Association Type" );
		names.insert( AVIF, "Actor Values Infomation" );//Actor Values/Perk Tree Graphics

		names.insert( BOOK, "Book" );
		names.insert( BPTD, "Body Part Data" );

		names.insert( CAMS, "Camera Shot" );
		names.insert( CELL, "Cell" );
		names.insert( CLAS, "Class" );
		names.insert( CLFM, "Color" );
		names.insert( CLMT, "Climate" );
		names.insert( COBJ, "Constructible Object" );
		names.insert( COLL, "Collision Layer" );
		names.insert( CONT, "Container" );
		names.insert( CPTH, "Camera Path" );
		names.insert( CSTY, "Combat Style" );

		names.insert( DEBR, "Debris" );
		names.insert( DIAL, "Dialog Topic" );
		names.insert( DLBR, "Dialog Branch" );
		names.insert( DLVW, "Dialog View" );
		names.insert( DOBJ, "Default Object Manager" );
		names.insert( DOOR, "Door" );
		names.insert( DUAL, "Dual Cast Data" );//Dual Cast Data( possibly unused

		names.insert( ECZN, "Encounter Zone" );
		names.insert( EFSH, "Effect Shader" );
		names.insert( ENCH, "Object Effect" );//Enchantment
		names.insert( EQUP, "Equip Type" );//Equip Slot( flag - type values )
		names.insert( EXPL, "Explosion" );
		names.insert( EYES, "Eyes" );

		names.insert( FACT, "Faction" );
		names.insert( FLOR, "Flora" );
		names.insert( FLST, "FormID List" );//Form List( non - leveled list )
		names.insert( FSTP, "Footstep" );
		names.insert( FSTS, "Footstep Set" );
		names.insert( FURN, "Furniture" );

		names.insert( GLOB, "Global" );//Global Variable
		names.insert( GMST, "Game Setting" );
		names.insert( GRAS, "Grass" );
		names.insert( GRUP, "Form Group" );

		names.insert( HAZD, "Hazard" );
		names.insert( HDPT, "Head Part" );

		names.insert( IDLE, "Idle Animation" );
		names.insert( IDLM, "Idle Marker" );
		names.insert( IMAD, "Image Space Adapter" );//Image Space Modifier
		names.insert( IMGS, "Image Space" );
		names.insert( INFO, "Dialog Topic Info" );
		names.insert( INGR, "Ingredient" );
		names.insert( IPCT, "Impact" );//Impact Data
		names.insert( IPDS, "Impact Data Set" );

		names.insert( KEYM, "Key" );
		names.insert( KYWD, "Keyword" );

		names.insert( LAND, "Landscape" );
		names.insert( LCRT, "Location Reference Type" );
		names.insert( LCTN, "Location" );
		names.insert( LGTM, "Lighting Template" );
		names.insert( LIGH, "Light" );
		names.insert( LSCR, "Load Screen" );
		names.insert( LTEX, "Landscape Texture" );//Land Texture
		names.insert( LVLI, "Leveled Item" );
		names.insert( LVLN, "Leveled NPC" );//Leveled Actor
		names.insert( LVSP, "Leveled Spell" );

		names.insert( MATO, "Material Object" );
		names.insert( MATT, "Material Type" );
		names.insert( MESG, "Message" );
		names.insert( MGEF, "Magic Effect" );
		names.insert( MISC, "Misc. Item" );//Misc.Object
		names.insert( MOVT, "Movement Type" );
		names.insert( MSTT, "Moveable Static" );//Movable Static
		names.insert( MUSC, "Music Type" );
		names.insert( MUST, "Music Track" );

		names.insert( NAVI, "Navigation Mesh Info Map" );//Navigation( master data )
		names.insert( NAVM, "NavMesh" );
		names.insert( NOTE, "Note" );
		names.insert( NPC_, "Non Player Character (Actor)" );//Actor( NPC, Creature )

		names.insert( OTFT, "Outfit" );

		names.insert( PACK, "Package" );//AI Package
		names.insert( PERK, "Perk" );
		names.insert( PGRE, "Placed grenade" );
		names.insert( PHZD, "Placed hazard" );
		names.insert( PROJ, "Projectile" );

		names.insert( QUST, "Quest" );

		names.insert( RACE, "Race" );//Race / Creature type
		names.insert( REFR, "Object Reference" );
		names.insert( REGN, "Region" );//Region( Audio / Weather )
		names.insert( RELA, "Relationship" );
		names.insert( REVB, "Reverb Parameters" );
		names.insert( RFCT, "Visual Effect" );

		names.insert( SCEN, "Scene" );
		names.insert( SCRL, "Scroll" );
		names.insert( SHOU, "Shout" );
		names.insert( SLGM, "Soul Gem" );
		names.insert( SMBN, "Story Manager Branch Node" );
		names.insert( SMEN, "Story Manager Event Node" );
		names.insert( SMQN, "Story Manager Quest Node" );
		names.insert( SNCT, "Sound Category" );
		names.insert( SNDR, "Sound Descriptor" );//Sound Reference
		names.insert( SOPM, "Sound Output Model" );
		names.insert( SOUN, "Sound Marker" );//Sound
		names.insert( SPEL, "Spell" );
		names.insert( SPGD, "Shader Particle Geometry" );
		names.insert( STAT, "Static" );

		names.insert( TACT, "Talking Activator" );
		names.insert( TES4, "Plugin info / Header" );
		names.insert( TREE, "Tree" );
		names.insert( TXST, "Texture Set" );

		names.insert( VTYP, "Voice Type" );

		names.insert( WATR, "Water" );//Water Type
		names.insert( WEAP, "Weapon" );
		names.insert( WOOP, "Word of Power" );//Word Of Power
		names.insert( WRLD, "Worldspace" );
		names.insert( WTHR, "Weather" );


		names.insert( AECH, "Audio Effect Chain" );
		names.insert( AMDL, "Aim Model" );
		names.insert( AORU, "Attraction Rule" );
		//		names.insert( ARTO, "Art Object" );
		names.insert( BNDS, "Bendable Spline" );
		names.insert( CMPO, "Component" );
		names.insert( DFOB, "Default Object" );
		names.insert( DMGT, "Damage Type" );
		names.insert( GDRY, "God Rays" );
		names.insert( INNR, "Instance Naming Rules" );
		names.insert( KSSM, "Sound Keyword Mapping" );
		names.insert( LAYR, "Layer" );
		names.insert( LENS, "Lens Flare" );
		names.insert( MSWP, "Material Swap" );
		names.insert( NOCM, "Navigation Mesh Obstacle Manager" );
		names.insert( OMOD, "Object Modification" );
		names.insert( OVIS, "Object Visibility Manager" );
		names.insert( PKIN, "Pack-In" );
		names.insert( RFGP, "Reference Group" );
		names.insert( SCCO, "Scene Collection" );
		names.insert( SCOL, "Static Collection" );
		names.insert( SCSN, "Audio Category Snapshot" );
		//		names.insert( SMQN, "Story Manager Quest Node" );
		names.insert( STAG, "Animation Sound Tag Set" );
		names.insert( TERM, "Terminal" );
		names.insert( TRNS, "Transform" );
		names.insert( ZOOM, "Zoom" );

		names.insert( ALOC, "Media Location Controller" );
		names.insert( AMEF, "Ammo Effect" );
		names.insert( CCRD, "Caravan Card" );
		names.insert( CDCK, "Caravan Deck" );
		names.insert( CHAL, "Challenge" );
		names.insert( CHIP, "Casino Chip" );
		names.insert( CMNY, "Caravan Money" );
		names.insert( CREA, "Creature" );
		names.insert( CSNO, "Casino" );
		names.insert( DEHY, "Dehydration Stage" );
		names.insert( HAIR, "Hair" );
		names.insert( HUNG, "Hunger Stage" );
		names.insert( IMOD, "Item Mod" );
		names.insert( LSCT, "Load Screen Type" );
		names.insert( LVLC, "Leveled Creture" );
		names.insert( MICN, "Menu Icon" );
		names.insert( MSET, "Media Set" );
		names.insert( PWAT, "Placeable Water" );
		names.insert( RADS, "Radiatioon Stage" );
		names.insert( RCCT, "Recipe Category" );
		names.insert( RCPE, "Recipe" );
		names.insert( REPU, "Reputation" );
		names.insert( RGDL, "Ragdoll" );
		names.insert( SCPT, "Script" );
		names.insert( SLPD, "Sleep Deprivation Stage" );

		names.insert( EDID, "Editor ID" );

#endif
	}


	QString* id2Name( quint32 id ) {
		init();

		auto it = names.find( id );
		if( it == names.constEnd() )return nullptr;

		return &*it;
	}

	QString toStringFromID( quint32 id ) {
		auto* fm = tesSys.formData( id );
		if( fm ) {
			return $$( "%1:%2" ).arg( id, 8, 16, QChar( '0' ) ).arg( fm->sign().str() ).toUpper();
		}
		return $$( "%1" ).arg( id, 8, 16, QChar( '0' ) ).toUpper();
	}
}


