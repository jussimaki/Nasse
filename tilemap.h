class Tilemap
{
private:
	int x_tilet, y_tilet;
	unsigned char *data;
public:
	Tilemap(): x_tilet(0), y_tilet(0), data(0){}
	void AsetaX(int x);
	void AsetaY(int y);
	void AsetaData(unsigned char *d);
	int AnnaX();
	int AnnaY();
	unsigned char *AnnaData();
	
	bool Piirra(int tileset, int kerros, int x_cam, int y_cam, GLOBAALI *glob_data, bool paallys);
	bool PiirraLeikepoyta(int tileset, int kerros, int x, int y, GLOBAALI *glob_data, bool paallys);
	bool KopioiLeikepoydalle(Tilemap *leikepoyta, int alku_x, int alku_y, int loppu_x, int loppu_y);
	bool OtaLeikepydalta(Tilemap *leikepoyta, int dest_x, int dest_y);
	bool Lataa(std::ifstream &tiedosto, GLOBAALI *glob_data, int tileset, int kerros, int koodi);
	bool Tallenna(std::ofstream &tiedosto, int koodi);
	
	void Alusta(int x, int y);
	void Tuhoa();
	bool MuutaKoko(int x, int y);
	bool x_siirto(int x);
	bool y_siirto(int y);
	void Vapauta();

	void vasenTyonto(int x);
	void ylaTyonto(int y);

	void vasenVeto(int x);
	void ylaVeto(int y);
	void oikeaTyonto(int x);
	void oikeaVeto(int x);

	void alaTyonto(int y);
	void alaVeto(int y);

	int draw_scrolling_tilemap(GLOBAALI *glob_data, int x_cam, int y_cam, int tileset, bool paallys);
};


