#define kasky_luo_ikkuna 0
#define kasky_sulje_ikkuna 2
#define kasky_lisaa_objekti 1
#define kasky_tuhoa_objekti 3
#define kasky_muuta_mappia 4
#define kasky_siirra_mappia 5
#define kasky_uusi_sektori 6
#define kasky_valitse_sektori 7
#define kasky_tuhoa_sektori 8
#define kasky_uusi_taso 9
#define kasky_tallenna_taso 10
#define kasky_lataa_taso 11
#define kasky_sektorisaato 12
#define kasky_komento 13
#define kasky_ota_linkki 14


struct Attribuutti
{
	std::string nimi;
	int tyyppi;
	void *maarite;
};

struct Objektityyppi
{
	int piirtojarjestys;
	int tyyppinumero;
	int ainaAktiivinen;
	int paallehyppy;
	int damagenotto;

	std::string nimi;
	int quadtree_num;
	bool vanhentunut;
	int leveys, korkeus, x_offset, y_offset;
	Sprite *kuvake;
	int x_koord, y_koord;
	std::vector <Sprite *> spritet;

	
	
	std::vector <Attribuutti*> attribuutit;
	std::vector <Animaatio *> animaatiot;
};

struct Anim_tile
{
	int nopeus;
	std::vector <int> ruudut;
	int ruutu;
	int counter;
};

struct Layer
{
	std::string tiedostonimi;
	Sprite *sprite;

	int tilemaara;
	Anim_tile** animoidut;
	int *tyypit;
	unsigned char *avaimet;
	unsigned char *lapinakyvat;

	unsigned char mappaus[300];
};

struct Tileset
{
	bool ladattu;
	std::string nimi;
	Layer* kerrokset[3];
};

struct Kasky
{
	int tyyppi;
	void *data;
};

struct Numerot
{
	std::string nimi;
	int objektinumero, jarjestysnumero;
};


struct Linkkimaaritys;
struct GLOBAALI
{
	Grafiikka *gfx;

	unsigned char saadot;
	int keyboard_indeksit[6];
	int gamepad_indeksit[6];
	std::map <std::string, Sprite *> spritemappi;
	std::map <std::string, Animaatio *> animaatiomappi;
	std::map <std::string, Biisi> biisimappi;
	std::map <std::string, Aani*> aanimappi;

	std::vector <Objektityyppi*> objektityypit;
	std::vector <Numerot> tyyppinimet;
	std::vector <Tileset *> tilesetit;

	std::vector <Linkkimaaritys *> linkkimaaritykset;

	int editoriobjektit;

	Animaatio *karttapelaaja, *karttasymboli, *karttasymboli2;

	Biisi *soivabiisi;
	Biisi valikkobiisi, karttabiisi, perusbiisi, luolabiisi, lumibiisi, rantabiisi, linnabiisi, bossibiisi;
	Biisi kuolemabiisi, tasolapibiisi;

	Aani *jousiaani, *hyppyaani, *kolikkoaani, *uintiaani;
	Sprite *pikkukuvat;
	Sprite *kirjaimet;
	std::vector <Kasky *> kaskyt;
};
