#pragma once
#include <QGlobal.h>
#include <QString>
#include "Qtlib.h"

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
union TESSignature {
	char editorName[ 4 ];
	quint32 typeID;
	TESSignature() {}
	TESSignature( quint32 _typeID ) :
		typeID( _typeID ) {
	}
	TESSignature( const QString& s ) {
		//auto * ss = ;
		memcpy( editorName, s.toUtf8().data(), 4 );
		//for( int i = 0; i < 4; i++ )		editorName[ i ] = ss[ i ];
	}
	TESSignature& operator=( const TESSignature& a ) {
		typeID = a.typeID;
		return *this;
	}
	bool operator==( const TESSignature& a ) {
		return this->typeID == a.typeID;
	}
	QString str() const {
		return $$( "%1%2%3%4" ).arg( editorName[ 0 ] ).arg( editorName[ 1 ] ).arg( editorName[ 2 ] ).arg( editorName[ 3 ] );
	}
};

/////////////////////////////////////////
inline
bool operator<( const TESSignature& a, const TESSignature& b ) {
	return a.str() < b.str();
}


//////////////////////////////////////////////////////////////////////////////////
#define MAKE_SIG(n1,n2,n3,n4)  n1 | (n2<<8) | (n3<<16)| (n4<<24)


#if 1
#include "generate_signature.h"

#else
static const quint32 AACT = MAKE_SIG( 'A', 'A', 'C', 'T' );//AACT	Action
static const quint32 ACHR = MAKE_SIG( 'A', 'C', 'H', 'R' );//ACHR	Actor Reference
static const quint32 ACTI = MAKE_SIG( 'A', 'C', 'T', 'I' );//ACTI	Activator
static const quint32 ADDN = MAKE_SIG( 'A', 'D', 'D', 'N' );//ADDN	Addon Node
static const quint32 ALCH = MAKE_SIG( 'A', 'L', 'C', 'H' );//ALCH	Potion
static const quint32 AMMO = MAKE_SIG( 'A', 'M', 'M', 'O' );//AMMO	Ammo
static const quint32 ANIO = MAKE_SIG( 'A', 'N', 'I', 'O' );//ANIO	Animation Object
static const quint32 APPA = MAKE_SIG( 'A', 'P', 'P', 'A' );//APPA	Apparatus (probably unused)
static const quint32 ARMA = MAKE_SIG( 'A', 'R', 'M', 'A' );//ARMA	Armor Addon (Model)
static const quint32 ARMO = MAKE_SIG( 'A', 'R', 'M', 'O' );//ARMO	Armor
static const quint32 ARTO = MAKE_SIG( 'A', 'R', 'T', 'O' );//ARTO	Art Object
static const quint32 ASPC = MAKE_SIG( 'A', 'S', 'P', 'C' );//ASPC	Acoustic Space
static const quint32 ASTP = MAKE_SIG( 'A', 'S', 'T', 'P' );//ASTP	Association Type
static const quint32 AVIF = MAKE_SIG( 'A', 'V', 'I', 'F' );//AVIF	Actor Values/Perk Tree Graphics
static const quint32 BOOK = MAKE_SIG( 'B', 'O', 'O', 'K' );//BOOK	Book
static const quint32 BPTD = MAKE_SIG( 'B', 'P', 'T', 'D' );//BPTD	Body Part Data

static const quint32 CAMS = MAKE_SIG( 'C', 'A', 'M', 'S' );//CAMS	Camera Shot
static const quint32 CELL = MAKE_SIG( 'C', 'E', 'L', 'L' );//CELL	Cell
static const quint32 CLAS = MAKE_SIG( 'C', 'L', 'A', 'S' );//CLAS	Class
static const quint32 CLFM = MAKE_SIG( 'C', 'L', 'F', 'M' );//CLFM	Color
static const quint32 CLMT = MAKE_SIG( 'C', 'L', 'M', 'T' );//CLMT	Climate
static const quint32 COBJ = MAKE_SIG( 'C', 'O', 'B', 'J' );//COBJ	Constructible Object (recipes)
static const quint32 COLL = MAKE_SIG( 'C', 'O', 'L', 'L' );//COLL	Collision Layer
static const quint32 CONT = MAKE_SIG( 'C', 'O', 'N', 'T' );//CONT	Container
static const quint32 CPTH = MAKE_SIG( 'C', 'P', 'T', 'H' );//CPTH	Camera Path
static const quint32 CSTY = MAKE_SIG( 'C', 'S', 'T', 'Y' );//CSTY	Combat Style

static const quint32 DEBR = MAKE_SIG( 'D', 'E', 'B', 'R' );//DEBR	Debris
static const quint32 DIAL = MAKE_SIG( 'D', 'I', 'A', 'L' );//DIAL	Dialog Topic
static const quint32 DLBR = MAKE_SIG( 'D', 'L', 'B', 'R' );//DLBR	Dialog Branch
static const quint32 DLVW = MAKE_SIG( 'D', 'L', 'V', 'W' );//DLVW	Dialog View
static const quint32 DOBJ = MAKE_SIG( 'D', 'O', 'B', 'J' );//DOBJ	Default Object Manager
static const quint32 DOOR = MAKE_SIG( 'D', 'O', 'O', 'R' );//DOOR	Door
static const quint32 DUAL = MAKE_SIG( 'D', 'U', 'A', 'L' );//DUAL	Dual Cast Data (possibly unused)

static const quint32 ECZN = MAKE_SIG( 'E', 'C', 'Z', 'N' );//ECZN	Encounter Zone
static const quint32 EFSH = MAKE_SIG( 'E', 'F', 'S', 'H' );//EFSH	Effect Shader
static const quint32 ENCH = MAKE_SIG( 'E', 'N', 'C', 'H' );//ENCH	Enchantment
static const quint32 EQUP = MAKE_SIG( 'E', 'Q', 'U', 'P' );//EQUP	Equip Slot (flag-type values)
static const quint32 EXPL = MAKE_SIG( 'E', 'X', 'P', 'L' );//EXPL	Explosion
static const quint32 EYES = MAKE_SIG( 'E', 'Y', 'E', 'S' );//EYES	Eyes

static const quint32 FACT = MAKE_SIG( 'F', 'A', 'C', 'T' );//FACT	Faction
static const quint32 FLOR = MAKE_SIG( 'F', 'L', 'O', 'R' );//FLOR	Flora
static const quint32 FLST = MAKE_SIG( 'F', 'L', 'S', 'T' );//FLST	Form List (non-leveled list)
static const quint32 FSTP = MAKE_SIG( 'F', 'S', 'T', 'P' );//FSTP	Footstep
static const quint32 FSTS = MAKE_SIG( 'F', 'S', 'T', 'S' );//FSTS	Footstep Set
static const quint32 FURN = MAKE_SIG( 'F', 'U', 'R', 'N' );//FURN	Furniture

static const quint32 GLOB = MAKE_SIG( 'G', 'L', 'O', 'B' );//GLOB	Global Variable
static const quint32 GMST = MAKE_SIG( 'G', 'M', 'S', 'T' );//GMST	Game Setting
static const quint32 GRAS = MAKE_SIG( 'G', 'R', 'A', 'S' );//GRAS	Grass
static const quint32 GRUP = MAKE_SIG( 'G', 'R', 'U', 'P' );//GRUP	Form Group

static const quint32 HAZD = MAKE_SIG( 'H', 'A', 'Z', 'D' );//HAZD	Hazard
static const quint32 HDPT = MAKE_SIG( 'H', 'D', 'P', 'T' );//HDPT	Head Part

static const quint32 IDLE = MAKE_SIG( 'I', 'D', 'L', 'E' );//IDLE	Idle Animation
static const quint32 IDLM = MAKE_SIG( 'I', 'D', 'L', 'M' );//IDLM	Idle Marker
static const quint32 IMAD = MAKE_SIG( 'I', 'M', 'A', 'D' );//IMAD	Image Space Modifier
static const quint32 IMGS = MAKE_SIG( 'I', 'M', 'G', 'S' );//IMGS	Image Space
static const quint32 INFO = MAKE_SIG( 'I', 'N', 'F', 'O' );//INFO	Dialog Topic Info
static const quint32 INGR = MAKE_SIG( 'I', 'N', 'G', 'R' );//INGR	Ingredient
static const quint32 IPCT = MAKE_SIG( 'I', 'P', 'C', 'T' );//IPCT	Impact Data
static const quint32 IPDS = MAKE_SIG( 'I', 'P', 'D', 'S' );//IPDS	Impact Data Set

static const quint32 KEYM = MAKE_SIG( 'K', 'E', 'Y', 'M' );//KEYM	Key
static const quint32 KYWD = MAKE_SIG( 'K', 'Y', 'W', 'D' );//KYWD	Keyword

static const quint32 LAND = MAKE_SIG( 'L', 'A', 'N', 'D' );//LAND	Landscape
static const quint32 LCRT = MAKE_SIG( 'L', 'C', 'R', 'T' );//LCRT	Location Reference Type
static const quint32 LCTN = MAKE_SIG( 'L', 'C', 'T', 'N' );//LCTN	Location
static const quint32 LGTM = MAKE_SIG( 'L', 'G', 'T', 'M' );//LGTM	Lighting Template
static const quint32 LIGH = MAKE_SIG( 'L', 'I', 'G', 'H' );//LIGH	Light
static const quint32 LSCR = MAKE_SIG( 'L', 'S', 'C', 'R' );//LSCR	Load Screen
static const quint32 LTEX = MAKE_SIG( 'L', 'T', 'E', 'X' );//LTEX	Land Texture
static const quint32 LVLI = MAKE_SIG( 'L', 'V', 'L', 'I' );//LVLI	Leveled Item
static const quint32 LVLN = MAKE_SIG( 'L', 'V', 'L', 'N' );//LVLN	Leveled Actor
static const quint32 LVSP = MAKE_SIG( 'L', 'V', 'S', 'P' );//LVSP	Leveled Spell

static const quint32 MATO = MAKE_SIG( 'M', 'A', 'T', 'O' );//MATO	Material Object
static const quint32 MATT = MAKE_SIG( 'M', 'A', 'T', 'T' );//MATT	Material Type
static const quint32 MESG = MAKE_SIG( 'M', 'E', 'S', 'G' );//MESG	Message
static const quint32 MGEF = MAKE_SIG( 'M', 'G', 'E', 'F' );//MGEF	Magic Effect
static const quint32 MISC = MAKE_SIG( 'M', 'I', 'S', 'C' );//MISC	Misc.Object
static const quint32 MOVT = MAKE_SIG( 'M', 'O', 'V', 'T' );//MOVT	Movement Type
static const quint32 MSTT = MAKE_SIG( 'M', 'S', 'T', 'T' );//MSTT	Movable Static
static const quint32 MUSC = MAKE_SIG( 'M', 'U', 'S', 'C' );//MUSC	Music Type
static const quint32 MUST = MAKE_SIG( 'M', 'U', 'S', 'T' );//MUST	Music Track

static const quint32 NAVI = MAKE_SIG( 'N', 'A', 'V', 'I' );//NAVI	Navigation( master data )
static const quint32 NAVM = MAKE_SIG( 'N', 'A', 'V', 'M' );//NAVM	NavMesh
static const quint32 NOTE = MAKE_SIG( 'N', 'O', 'T', 'E' );//NOTE	Note
static const quint32 NPC_ = MAKE_SIG( 'N', 'P', 'C', '_' );//NPC_	Actor( NPC, Creature )

static const quint32 OTFT = MAKE_SIG( 'O', 'T', 'F', 'T' );//OTFT	Outfit

static const quint32 PACK = MAKE_SIG( 'P', 'A', 'C', 'K' );//PACK	AI Package
static const quint32 PERK = MAKE_SIG( 'P', 'E', 'R', 'K' );//PERK	Perk
static const quint32 PGRE = MAKE_SIG( 'P', 'G', 'R', 'E' );//PGRE	Placed grenade
static const quint32 PHZD = MAKE_SIG( 'P', 'H', 'Z', 'D' );//PHZD	Placed hazard
static const quint32 PROJ = MAKE_SIG( 'P', 'R', 'O', 'J' );//PROJ	Projectile

static const quint32 QUST = MAKE_SIG( 'Q', 'U', 'S', 'T' );//QUST	Quest

static const quint32 RACE = MAKE_SIG( 'R', 'A', 'C', 'E' );//RACE	Race / Creature type
static const quint32 REFR = MAKE_SIG( 'R', 'E', 'F', 'R' );//REFR	Object Reference
static const quint32 REGN = MAKE_SIG( 'R', 'E', 'G', 'N' );//REGN	Region( Audio / Weather )
static const quint32 RELA = MAKE_SIG( 'R', 'E', 'L', 'A' );//RELA	Relationship
static const quint32 REVB = MAKE_SIG( 'R', 'E', 'V', 'B' );//REVB	Reverb Parameters
static const quint32 RFCT = MAKE_SIG( 'R', 'F', 'C', 'T' );//RFCT	Visual Effect

static const quint32 SCEN = MAKE_SIG( 'S', 'C', 'E', 'N' );//SCEN	Scene
static const quint32 SCRL = MAKE_SIG( 'S', 'C', 'R', 'L' );//SCRL	Scroll
static const quint32 SHOU = MAKE_SIG( 'S', 'H', 'O', 'U' );//SHOU	Shout
static const quint32 SLGM = MAKE_SIG( 'S', 'L', 'G', 'M' );//SLGM	Soul Gem
static const quint32 SMBN = MAKE_SIG( 'S', 'M', 'B', 'N' );//SMBN	Story Manager Branch Node
static const quint32 SMEN = MAKE_SIG( 'S', 'M', 'E', 'N' );//SMEN	Story Manager Event Node
static const quint32 SMQN = MAKE_SIG( 'S', 'M', 'Q', 'N' );//SMQN	Story Manager Quest Node
static const quint32 SNCT = MAKE_SIG( 'S', 'N', 'C', 'T' );//SNCT	Sound Category
static const quint32 SNDR = MAKE_SIG( 'S', 'N', 'D', 'R' );//SNDR	Sound Reference
static const quint32 SOPM = MAKE_SIG( 'S', 'O', 'P', 'M' );//SOPM	Sound Output Model
static const quint32 SOUN = MAKE_SIG( 'S', 'O', 'U', 'N' );//SOUN	Sound
static const quint32 SPEL = MAKE_SIG( 'S', 'P', 'E', 'L' );//SPEL	Spell
static const quint32 SPGD = MAKE_SIG( 'S', 'P', 'G', 'D' );//SPGD	Shader Particle Geometry
static const quint32 STAT = MAKE_SIG( 'S', 'T', 'A', 'T' );//STAT	Static

static const quint32 TACT = MAKE_SIG( 'T', 'A', 'C', 'T' );//TACT	Talking Activator
static const quint32 TES4 = MAKE_SIG( 'T', 'E', 'S', '4' );//TES4	Plugin info / Header
static const quint32 TREE = MAKE_SIG( 'T', 'R', 'E', 'E' );//TREE	Tree
static const quint32 TXST = MAKE_SIG( 'T', 'X', 'S', 'T' );//TXST	Texture Set

static const quint32 VTYP = MAKE_SIG( 'V', 'T', 'Y', 'P' );//VTYP	Voice Type

static const quint32 WATR = MAKE_SIG( 'W', 'A', 'T', 'R' );//WATR	Water Type
static const quint32 WEAP = MAKE_SIG( 'W', 'E', 'A', 'P' );//WEAP	Weapon
static const quint32 WOOP = MAKE_SIG( 'W', 'O', 'O', 'P' );//WOOP	Word Of Power
static const quint32 WRLD = MAKE_SIG( 'W', 'R', 'L', 'D' );//WRLD	Worldspace
static const quint32 WTHR = MAKE_SIG( 'W', 'T', 'H', 'R' );//WTHR	Weather

static const quint32 AECH = MAKE_SIG( 'A', 'E', 'C', 'H' );//-AECH	Audio Effect Chain
static const quint32 AMDL = MAKE_SIG( 'A', 'M', 'D', 'L' );//-AMDL	Aim Model
static const quint32 AORU = MAKE_SIG( 'A', 'O', 'R', 'U' );//-AORU	Attraction Rule
//static const quint32 ARTO = MAKE_SIG( 'A', 'R', 'T', 'O' );//-ARTO	Art Object
static const quint32 BNDS = MAKE_SIG( 'B', 'N', 'D', 'S' );//-BNDS	Bendable Spline
static const quint32 CMPO = MAKE_SIG( 'C', 'M', 'P', 'O' );//-CMPO	Component
static const quint32 DFOB = MAKE_SIG( 'D', 'F', 'O', 'B' );//-DFOB	Default Object
static const quint32 DMGT = MAKE_SIG( 'D', 'M', 'G', 'T' );//-DMGT	Damage Type
static const quint32 GDRY = MAKE_SIG( 'G', 'D', 'R', 'Y' );//-GDRY	God Rays
static const quint32 INNR = MAKE_SIG( 'I', 'N', 'N', 'R' );//-INNR	Instance Naming Rules
static const quint32 KSSM = MAKE_SIG( 'K', 'S', 'S', 'M' );//-KSSM	Sound Keyword Mapping
static const quint32 LAYR = MAKE_SIG( 'L', 'A', 'Y', 'R' );//-LAYR	Layer
static const quint32 LENS = MAKE_SIG( 'L', 'E', 'N', 'S' );//-LENS	Lens Flare
static const quint32 MSWP = MAKE_SIG( 'M', 'S', 'W', 'P' );//-MSWP	Material Swap
static const quint32 NOCM = MAKE_SIG( 'N', 'O', 'C', 'M' );//-NOCM Navigation Mesh Obstacle Manager
static const quint32 OMOD = MAKE_SIG( 'O', 'M', 'O', 'D' );//-OMOD Object Modification
static const quint32 OVIS = MAKE_SIG( 'O', 'V', 'I', 'S' );//-OVIS Object Visibility Manager
static const quint32 PKIN = MAKE_SIG( 'P', 'K', 'I', 'N' );//-PKIN Pack-In
static const quint32 RFGP = MAKE_SIG( 'R', 'F', 'G', 'P' );//-RFGP Reference Group
static const quint32 SCCO = MAKE_SIG( 'S', 'C', 'C', 'O' );//-SCCO Scene Collection
static const quint32 SCOL = MAKE_SIG( 'S', 'C', 'O', 'L' );//-SCOL Static Collection
static const quint32 SCSN = MAKE_SIG( 'S', 'C', 'S', 'N' );//-SCSN Audio Category Snapshot
//static const quint32 SMQN = MAKE_SIG( 'S', 'M', 'Q', 'N' );//-SMQN Story Manager Quest Node
static const quint32 STAG = MAKE_SIG( 'S', 'T', 'A', 'G' );//-STAG Animation Sound Tag Set
static const quint32 TERM = MAKE_SIG( 'T', 'E', 'R', 'M' );//-TERM Terminal
static const quint32 TRNS = MAKE_SIG( 'T', 'R', 'N', 'S' );//-TRNS Transform
static const quint32 ZOOM = MAKE_SIG( 'Z', 'O', 'O', 'M' );//-ZOOM Zoom



static const quint32 ANAM = MAKE_SIG( 'A', 'N', 'A', 'M' );
static const quint32 ATTX = MAKE_SIG( 'A', 'T', 'T', 'X' );
static const quint32 BNAM = MAKE_SIG( 'B', 'N', 'A', 'M' );
static const quint32 CITC = MAKE_SIG( 'C', 'I', 'T', 'C' );
static const quint32 CNAM = MAKE_SIG( 'C', 'N', 'A', 'M' );
static const quint32 DATA = MAKE_SIG( 'D', 'A', 'T', 'A' );
static const quint32 DESC = MAKE_SIG( 'D', 'E', 'S', 'C' );
static const quint32 EDID = MAKE_SIG( 'E', 'D', 'I', 'D' );
static const quint32 FNAM = MAKE_SIG( 'F', 'N', 'A', 'M' );
static const quint32 FULL = MAKE_SIG( 'F', 'U', 'L', 'L' );
static const quint32 HEDR = MAKE_SIG( 'H', 'E', 'D', 'R' );
static const quint32 INTV = MAKE_SIG( 'I', 'N', 'T', 'V' );
static const quint32 INCC = MAKE_SIG( 'I', 'N', 'C', 'C' );
static const quint32 KSIZ = MAKE_SIG( 'K', 'S', 'I', 'Z' );
static const quint32 KWDA = MAKE_SIG( 'K', 'W', 'D', 'A' );
static const quint32 LNAM = MAKE_SIG( 'L', 'N', 'A', 'M' );
static const quint32 MAST = MAKE_SIG( 'M', 'A', 'S', 'T' );
static const quint32 MNAM = MAKE_SIG( 'M', 'N', 'A', 'M' );
static const quint32 MODL = MAKE_SIG( 'M', 'O', 'D', 'L' );
static const quint32 MODT = MAKE_SIG( 'M', 'O', 'D', 'T' );
static const quint32 OBND = MAKE_SIG( 'O', 'B', 'N', 'D' );
static const quint32 ONAM = MAKE_SIG( 'O', 'N', 'A', 'M' );
static const quint32 PNAM = MAKE_SIG( 'P', 'N', 'A', 'M' );
static const quint32 SCHR = MAKE_SIG( 'S', 'C', 'H', 'R' );
static const quint32 SCDA = MAKE_SIG( 'S', 'C', 'D', 'A' );
static const quint32 SCTX = MAKE_SIG( 'S', 'C', 'T', 'X' );
static const quint32 SNAM = MAKE_SIG( 'S', 'N', 'A', 'M' );
static const quint32 TNAM = MAKE_SIG( 'T', 'N', 'A', 'M' );
static const quint32 VMAD = MAKE_SIG( 'V', 'M', 'A', 'D' );
static const quint32 VNAM = MAKE_SIG( 'V', 'N', 'A', 'M' );
static const quint32 WNAM = MAKE_SIG( 'W', 'N', 'A', 'M' );
static const quint32 XNAM = MAKE_SIG( 'X', 'N', 'A', 'M' );

// NV
static const quint32 ALOC = MAKE_SIG( 'A', 'L', 'O', 'C' );//ALOC	Media Location Controller
static const quint32 AMEF = MAKE_SIG( 'A', 'M', 'E', 'F' );//AMEF	Ammo Effect
static const quint32 CCRD = MAKE_SIG( 'C', 'C', 'R', 'D' );//CCRD	Caravan Card
static const quint32 CDCK = MAKE_SIG( 'C', 'D', 'C', 'K' );//CDCK	Caravan Deck
static const quint32 CHAL = MAKE_SIG( 'C', 'H', 'A', 'L' );//CHAL	Challenge
static const quint32 CHIP = MAKE_SIG( 'C', 'H', 'I', 'P' );//CHIP	Casino Chip
static const quint32 CMNY = MAKE_SIG( 'C', 'M', 'N', 'Y' );//CMNY	Caravan Money
static const quint32 CREA = MAKE_SIG( 'C', 'R', 'E', 'A' );//CREA	Creature
static const quint32 CSNO = MAKE_SIG( 'C', 'S', 'N', 'O' );//CSNO	Casino
static const quint32 DEHY = MAKE_SIG( 'D', 'E', 'H', 'Y' );//DEHY	Dehydration Stage
static const quint32 HAIR = MAKE_SIG( 'H', 'A', 'I', 'R' );//HAIR	Hair
static const quint32 HUNG = MAKE_SIG( 'H', 'U', 'N', 'G' );//HUNG	Hunger Stage
static const quint32 IMOD = MAKE_SIG( 'I', 'M', 'O', 'D' );//IMOD	Item Mod
static const quint32 LSCT = MAKE_SIG( 'L', 'S', 'C', 'T' );//LSCT	Load Screen Type
static const quint32 LVLC = MAKE_SIG( 'L', 'V', 'L', 'C' );//LVLC	Leveled Creture
static const quint32 MICN = MAKE_SIG( 'M', 'I', 'C', 'N' );//MICN	Menu Icon
static const quint32 MSET = MAKE_SIG( 'M', 'S', 'E', 'T' );//MSET	Media Set
static const quint32 PWAT = MAKE_SIG( 'P', 'W', 'A', 'T' );//PWAT	Placeable Water
static const quint32 RADS = MAKE_SIG( 'R', 'A', 'D', 'S' );//RADS	Radiatioon Stage
static const quint32 RCCT = MAKE_SIG( 'R', 'C', 'C', 'T' );//RCCT	Recipe Category
static const quint32 RCPE = MAKE_SIG( 'R', 'C', 'P', 'E' );//RCPE	Recipe
static const quint32 REPU = MAKE_SIG( 'R', 'E', 'P', 'U' );//REPU	Reputation
static const quint32 RGDL = MAKE_SIG( 'R', 'G', 'D', 'L' );//RGDL	Ragdoll
static const quint32 SCPT = MAKE_SIG( 'S', 'C', 'P', 'T' );//SCPT	Script
static const quint32 SLPD = MAKE_SIG( 'S', 'L', 'P', 'D' );//SLPD	Sleep Deprivation Staged

static const quint32 SCRI = MAKE_SIG( 'S', 'C', 'P', 'T' );
#endif

#pragma pack()
