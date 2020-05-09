bool KirjoitaString(std::string *str, std::ofstream &tiedosto);
bool LueString(std::string *str, std::ifstream &tiedosto);


class Editorisektori
{
private:
	Editorilayer layerit[3];
	int tileset;
	int musiikki;
	std::vector <Editoriobjekti *> objektit;
	std::vector <Linkkitieto *> linkit;

	int kerros;

public:
	bool kaynnista(GLOBAALI *glob_data);
	bool PiirraTilemap(GLOBAALI *glob_data);

	bool Alustus();
	bool Tuhoa(GLOBAALI *glob_data);
	void PiirraObjektit();
	bool TuhoaTurhat(GLOBAALI *glob_data);

	std::vector <Linkkitieto *> *AnnaLinkkitiedot();

	Tilemap *annamappi();
	Tilemap *annaleikepoyta();
	std::vector <Editoriobjekti *> *AnnaObjektit();
	int AnnaKerros();
	int AnnaTileset();
	int AnnaMusiikki();
	void AsetaTileset(int set);
	void AsetaMusiikki(int m);
	Editorilayer* AnnaLayerit();
	void SaadaKerros(int k);
	int AnnaXcam();
	int AnnaYcam();
	bool Tallenna(std::ofstream &tiedosto, GLOBAALI *glob_data, Editor *editori, int koodi);
	bool Lataa(std::ifstream &tiedosto, GLOBAALI *glob_data, int koodi);
	void AsetaCam(int x, int y) {layerit[0].AsetaCam(x,y);}

	void Objektituhous(GLOBAALI *glob_data, int x_tile, int y_tile);
	void Objektisiirto(int x, int y, GLOBAALI *glob_data, int x_tile, int y_tile);

	void vasenTyonto(int x, GLOBAALI *glob_data);
	void ylaTyonto(int y, GLOBAALI *glob_data);

	void vasenVeto(int x, GLOBAALI *glob_data);
	void ylaVeto(int y, GLOBAALI *glob_data);
	void oikeaTyonto(int x, GLOBAALI *glob_data);
	void oikeaVeto(int x, GLOBAALI *glob_data);

	void alaTyonto(int y, GLOBAALI *glob_data);
	void alaVeto(int y, GLOBAALI *glob_data);


};

