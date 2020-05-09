#define LUOUUSIVALIKKO 0
#define LUOVAIMUOKKAA 1
#define LUOOBJEKTI 2
#define MUOKKAALISTAA 3
#define MUOKKAUSVALIKKO 4
#define MUOKKAAOBJEKTIA 5
#define POISTOVALIKKO 6
#define LINKINOTTOVALIKKO 7
#define KOMENTORIVI 8
#define YLEISIKKUNA 9



#define ikkuna_VIRHE -1
#define ikkuna_JATKA 0
#define ikkuna_LOPPU 1
#define ikkuna_LOPPU_SULJEITSESI 2

struct Yleisdata
{
	std::string *nimi;
	std::vector <std::string> *nimet;
	std::vector <int> *minimit;
	std::vector <int> *maksimit;
	std::vector <int> *luvut;
};


struct Listamuokkaus
{
	std::vector < std::vector < void *> * > *lista_os;
	Listamaaritys *maaritys;
};

struct axYy
{
	int x_tile, y_tile;
};

struct Objektikuorma
{
	axYy *koord;
	std::vector <Editoriobjekti *> *objektit;
};

class Editor;
class Ikkuna
{
private:
	Ikkuna *pointer;
	int tyyppi;
	void *data;
	int skrollaus, skrollaus2;
	public:
	Ikkuna(): tyyppi(0), data(0), skrollaus(0), skrollaus2(0), pointer(0){}
	bool Piirra(GLOBAALI *glob_data, Editor *editori);
	bool LuoIkkuna(int t, void *d, GLOBAALI *glob_data);
	bool Tuhoa();
	int Toiminta(GLOBAALI *glob_data, int hiiri_x, int hiiri_y, Editor *editori);
};

