#define SUUNTA_YLOS 1
#define SUUNTA_ALAS -1
#define SUUNTA_OIKEA 2
#define SUUNTA_VASEN -2
#define SUUNTA_YLAOIKEA 3
#define SUUNTA_ALAVASEN -3
#define SUUNTA_ALAOIKEA 4
#define SUUNTA_YLAVASEN -4

#define MOODI_VAPAA 0
#define MOODI_ULOSKAVELY 1
#define MOODI_SISAANKAVELY 2
#define MOODI_PIIRTO 3
#define MOODI_TALLENNETAANKO 4
#define MOODI_LOPETETAANKO 5
#define MOODI_GAMEOVER 6


struct lahdekohde
{
	int lahde, kohde;
	int x_0, y_0, x_1, y_1, x_2, y_2;
};

struct askel
{
	unsigned char palatyyppi;
	char suunta;
	int aika;
};

struct Lapaisy
{
	int x,y,numero;
};

struct Piste
{
	int pelinloppu;
	int x,y;
	int symboli;
	char *tiedostonimi;
	int reittimaara;
	int reittikoot[4];
	bool menty[4];
	unsigned char *reitit[4];
};

class Kartta
{
public:
	bool Lataa(char *tiedostonimi, GLOBAALI *glob_data);
	bool Lataa_binaari(char *tiedostonimi, GLOBAALI *glob_data);
	int Logiikka(GLOBAALI *glob_data, char **tiedostonimi);
	bool Piirto(GLOBAALI *glob_data);
	bool AsetaPiirto(int monesko);
	void TallennaPeli();
	bool LataaPeli(char *tiedostonimi);
	void UusiPeli(char *tiedostonimi);
	void Tuhoa();
	void kaynnista(GLOBAALI *glob_data);
private:
	bool yritys();
	bool TeePalanen(unsigned char tyyppi, int *x, int *y);
	bool TeeAnimPalanen(unsigned char tyyppi, int *x, int *y);
	bool uloskavelyfunktio(int laskuri, int *lokaali_x, int *lokaali_y, int tiletyyppi, int suunta);
	bool sisaankavelyfunktio(int laskuri, int *lokaali_x, int *lokaali_y, int tiletyyppi, int suunta);
	bool Teereitti(int x, int y, int reittinumero);
	bool TeereittiVaiheittain(int reittinumero, GLOBAALI *glob_data);
	int leveys, korkeus;
	Tilemap tilemap;
	Tilemap reittimappi;
	unsigned char *reitit;
	int pistemaara;
	int piirtoreitti;
	char *tallennuspaikka;
	std::vector <Lapaisy> lapaisyt;
	
	Piste *pistetaulukko[100]; // taulukko
	Piste **pisteet; // tilemappi

	int viimemoodi,kursori;
	int s_a, s_b,a,b;
	int orig_x_tile, orig_y_tile;
	int x_tile, y_tile;
	int a_x_tile, a_y_tile;
	int o_x_tile, o_y_tile;
	int x_kohta, y_kohta, x_loc, y_loc;
	int moodi;
	int pala_num;
	int suunta, seuraavasuunta;
	int laskuri;
	int kartta_x_cam, kartta_y_cam;
	int tallennettava_x, tallennettava_y;
	

	int animaatio;
};