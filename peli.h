#define tila_ALKUTEKSTIT 0
#define tila_LOPPUTEKSTIT 1
#define tila_TASO 2
#define tila_KARTTA 3
#define tila_GAMEOVER 4
#define tila_CONTINUE 5

class Peli
{
private:
	int elamat, kolikot;
	int pelitila;
	Kartta kartta;
	Pelitaso taso;
	bool aloitus;

	int valikkonuoli;
	int laskuri;
public:
	int gameoverruutu(GLOBAALI *glob_data);
	void continueruutu(GLOBAALI *glob_data);
	int lopputekstit(GLOBAALI *glob_data);
	bool uusiPeli(GLOBAALI *glob_data, char *save);
	void tulostaTiedot(GLOBAALI *glob_data);
	int logiikka(GLOBAALI *glob_data);
	bool lataaPeli(GLOBAALI *glob_data, char *save);
	bool tallennaPeli();
	void lisaaElama();
	void lisaaKolikko();
	bool alusta(GLOBAALI *glob_data);
	void tuhoa();

	int rullaus;
};