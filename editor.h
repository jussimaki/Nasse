#define mapinmuokkaus 0
#define tilevalinta 1


#define editori_EDITOINTI 0
#define editori_TESTAUS 1

class Editor
{
private:
	Ikkuna *ikkuna;
	Tilemap *valintamappi;
	int leike_alku_x, leike_alku_y;
	int alku_x, alku_y, alku_sektori;
	int valinta_y_cam;
	bool kopiointi;
	int moodi;
	int sektori;
	int hiiri_x, hiiri_y;
	std::vector <Editorisektori *> sektorit;
	bool piirraTilet(GLOBAALI *glob_data);

	Pelitaso testitaso;
	Editoriobjekti *linkki;
	int linkkisektori;
	int editoritila;

	
		
public:
	int editointi(GLOBAALI *glob_data);
	int tasonTestaus(GLOBAALI *glob_data);
	std::vector <Editorisektori *> *annaSektorit();
	//Editor(): ikkuna(0), moodi(mapinmuokkaus), valintamappi(0), kopiointi(0), sektori(0), hiiri_x(320), hiiri_y(240), linkki(0){}
	//Editor(GLOBAALI *glob_data) {Alustus(glob_data);}

	void kaynnista(GLOBAALI *glob_data);
	
	
	Editoriobjekti *annaLinkki() {return linkki;}
	void poistaLinkki() {linkki = 0;}
	int annaLinkkisektori() {return linkkisektori;}
	
	bool tallenna(std::string tiedostonimi, GLOBAALI *glob_data);
	bool tallennaKartta(std::string tiedostonimi, GLOBAALI *glob_data);
	bool lataa(std::ifstream &tiedosto, GLOBAALI *glob_data);
	bool alustus(GLOBAALI *glob_data);
	bool Muokkaa(GLOBAALI *glob_data);
	bool Piirravarjo(int x_tile, int y_tile, int x_cam, int y_cam, GLOBAALI *glob_data);
	bool PiirraObjektit(GLOBAALI *glob_data);
	int Toiminta(GLOBAALI *glob_data);
	bool AlustaValintamappi(GLOBAALI *glob_data);
	bool Valinta(GLOBAALI *glob_data);
	int Kaskyt(GLOBAALI *glob_data);
	bool Tuhoa(GLOBAALI *glob_data);
	int AnnaAlkuX() {return alku_x;}
	int AnnaAlkuY() {return alku_y;}
	void Objektisiirto(int x, int y, GLOBAALI *glob_data);

	std::vector <Editoriobjekti *> *Objektihaku();
};
