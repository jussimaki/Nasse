class Pelitaso;

class Pelisektori
{
private:
	int frame;
	int musiikki, tileset;
	int x_cam, y_cam;
	int x_cam_dest, y_cam_dest;
	Tilemap tilemapit[3];
	int nopeudet[3];
	
	std::vector <Peliobjekti *> objektit;
	std::vector <Peliobjekti *> tuhoutuvat;
	
	Quadtree quadtree;
	Pelaaja pelaaja;

	std::vector <Peliobjekti*> kadonneet;
	std::vector <Peliobjekti *> ainaAktiiviset;
	Poiminta piirrettavat, logiikka, tavalliset, kiinteat, kolikot, palkit, uusiksi;

	std::vector <Linkkitieto *> linkit;

	int x_camera_mode, y_camera_mode;
	int x_cam_speed, y_cam_speed;
	bool lapi;
	bool kuolema;
	int maalinro;
	int loppucounter;
	bool pause;
	int damageVarina;


public:
	Peliobjekti *annaPelaaja();
	Quadtree* annaQuadtree();
	
	Tilemap *annaMappi(int num);
	void pelaajaanOsui(GLOBAALI *glob_data);
	void pauseta();
	int annaSetti() {return tileset;}
	void tarkistaTilat(GLOBAALI *glob_data);
	std::vector <Peliobjekti *> *annaObjektit();
	std::vector <Linkkitieto *> *annaLinkkitiedot();
	bool alustaObjektit(bool ekakerta);
	bool kaynnista(GLOBAALI *glob_data, int alku_x, int alku_y, int energia);
	int toiminta(GLOBAALI *glob_data, Pelitaso *taso);
	bool lataa(std::ifstream &tiedosto, GLOBAALI *glob_data, int koodi);
	int taustapiirto(GLOBAALI *glob_data);
	bool vapauta(GLOBAALI *glob_data);
	bool piirto(GLOBAALI *glob_data, Pelitaso *taso);
	bool piirraObjektit(GLOBAALI *glob_data);
	int logic(GLOBAALI *glob_data, Pelitaso *taso);
	bool tormaykset(GLOBAALI *glob_data);
	void lisaaKasittely(Peliobjekti *objekti) {ainaAktiiviset.push_back(objekti);}
	void lisaaTuhoutuviin(Peliobjekti *objekti) {tuhoutuvat.push_back(objekti);}
	void kasitteleTuhoutuvat();
	void camera(GLOBAALI *glob_data);
	int annaXcam();
	int annaYcam();
	std::vector <Peliobjekti *> *annaAinaAktiiviset();

	void itsari(GLOBAALI *glob_data);
	
	void lapaisy(int nro, GLOBAALI *glob_data);
	bool sammuta(GLOBAALI *glob_data);
	bool onkoLapi() {return lapi;}
	Poiminta *annaLogiikka();
};

