struct Linkkitieto;
struct Listamaaritys;

class Objekti
{
protected:
	std::vector <void *> *data;
	int tyyppinumero;
	int x_, y_;
public:
	bool LataaData(std::ifstream &tiedosto, GLOBAALI *glob_data, int t, std::vector <Linkkitieto*> *linkkitiedot);
	void TuhoaData(GLOBAALI *glob_data, bool editori);
	std::vector < std::vector <void *> *> * LataaLista(std::ifstream &tiedosto, Listamaaritys *maaritys, std::vector <Linkkitieto*> *linkkitiedot, GLOBAALI *glob_data);
	int annaTyyppi();
	int AnnaX();
	int AnnaY();
};

struct Editorilinkki;

class Editor;
class Editoriobjekti : public Objekti
{
	
public:
	bool TuhoaObjekti(GLOBAALI *glob_data);
	void TuhoaLinkki(Editorilinkki *os);
	void PiirraKuvake(GLOBAALI *glob_data, int x_cam, int y_cam);
	std::vector <void *> *AnnaData();
	bool TallennaData(std::ofstream &tiedosto, GLOBAALI *glob_data, Editor *editori);
	bool TallennaLista(std::ofstream &tiedosto, Listamaaritys *maaritys, std::vector <std::vector<void *>*> *Listadata, Editor *editori, GLOBAALI *glob_data);
	bool LuoObjekti(int t, int x_tile, int y_tile, GLOBAALI *glob_data);
	void LisaaLinkki(Editorilinkki *uusi);
	std::vector <Editorilinkki *> linkit_tahan;
	void x_siirto(int siirto);
	void y_siirto(int siirto);

};
