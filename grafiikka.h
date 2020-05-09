
int number_to_string(int number, char **string);


struct Rect
{
	int x, y, width, height;
};

struct Sprite
{
	int width, height;
	SDL_Surface *sdl_data;
};


struct Biisi
{
	FMUSIC_MODULE *handle;
};

struct Aani
{
	FSOUND_SAMPLE *handle;
};


#define VASEN 0
#define KESKI 1
#define OIKEA 2


struct InputData
{
	int hiiri_x_siirto, hiiri_y_siirto;
	unsigned char JOYSTICK_POHJASSA[24], JOYSTICK_PAINETTU[24];
	//unsigned char POHJASSA[256], PAINETTU[256];
	Uint8* nappi;
	unsigned char NAPPI_POHJASSA[1000], NAPPI_PAINETTU[1000];
	unsigned char HIIRINAPPI_POHJASSA[3], HIIRINAPPI_PAINETTU[3];
	unsigned char HIIRINAPPI_ESTO[3];
};

class Grafiikka
{

public:	
	Grafiikka(): seuraava_kolmiot(0), kolmiot(0), rullaus_y(0), x_reso(0), y_reso(0), bittia(0), moodi(0), sdl_screen(0), buffer_16(0), buffer_32(0),
		valipinta(0), padi(0){}
	~Grafiikka();
	SDL_Surface* AnnaOsoitin();
	
		
	void zoom(int x, int y, int lev, int kork)
	{seuraava_zoom_x = x; seuraava_zoom_y = y; seuraava_zoom_lev = lev; seuraava_zoom_kork = kork;}

	void nuku(int aika) {SDL_Delay(aika);}
	bool Alusta(int x, int y, int oikea_x, int oikea_y, int bit, int m, int h_x, int h_y, int s);
	bool Sulje();
	bool LataaSprite(Sprite *sprite, const char *tiedostonimi);
	bool LataaSprite2(Sprite *sprite, const char *tiedostonimi);
	bool VapautaSprite(Sprite *sprite);
	bool TyhjennaRuutu();
	bool Flip();
	bool Skaalaus();
	bool PiirraSprite(Sprite *sprite, int x, int y, Rect *source, bool avain, int alpha);
	bool Input();
	bool nollaaInput();
	
	void lataaBiisi(const char *tiedostonimi, Biisi *biisi);
	void soitaBiisi(Biisi *biisi);
	void vapautaBiisi(Biisi *biisi);
	void pysaytaBiisi(Biisi *biisi);

	void lataaAani(const char *tiedostonimi, Aani *aani);
	void vapautaAani(Aani *aani);
	void soitaAani(Aani *aani);


	InputData* annaInput();
	int AnnaAika();

	void asetaKolmiot(bool k) {seuraava_kolmiot = k;}

	void asetaS(int s) {skaalaus = s;}
	void ValaiseRivi(int y, float aksa, Uint32 *kohdepixelit_32, Uint32 punamaski, Uint32 vihermaski, Uint32 sinimaski);
	void ValaiseRivi2(int y, float aksa, Uint32 *kohdepixelit_32);
	void asetaR(int r) {seuraava_rullaus_y = r; if(seuraava_rullaus_y < 0) seuraava_rullaus_y = 0;}
	int annaR() {return rullaus_y;}

	void asetaKolmiodata(int *x_koord, int *y_koord, int *x_text, int *y_text, int maara)
	{
		kolmio_x = x_koord;
		kolmio_y = y_koord;
		kolmio_text_x = x_text;
		kolmio_text_y = y_text;

		kolmiomaara = maara;
	}
	
	bool rullaus32();
	bool zoom32moodi0();
	bool zoom32moodi1();
	bool zoom32moodi2();
	bool zoom16moodi0();
	SDL_Joystick *padiKiinni() {return padi;}

	void piirraKolmiot();
private:
	InputData inputti;
	
	bool kolmiot, seuraava_kolmiot;
	int rullaus_y;
	int seuraava_rullaus_y;

	int *kolmio_x, *kolmio_y, *kolmio_text_x, *kolmio_text_y; 
	int kolmiomaara;
	int aaniindeksi;

	
	
	int skaalaus;
	SDL_Joystick *padi;
	Uint16 *buffer_16;
	Uint32 *buffer_32;
	int x_oikea, y_oikea;
	int x_reso, y_reso, bittia, moodi;

	int zoom_x, zoom_y, zoom_lev, zoom_kork;
	int seuraava_zoom_x, seuraava_zoom_y, seuraava_zoom_lev, seuraava_zoom_kork;
	int heitto_x, heitto_y;
	SDL_Surface *sdl_screen;
	SDL_Surface *valipinta;

};

