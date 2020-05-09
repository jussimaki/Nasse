#define attrib_LUKU 0
#define attrib_LISTA 1
#define attrib_MJONO 2
#define attrib_LINKKI 3


struct MinMaxAlku
{
	int min, max, alku;
};


struct Listamaaritys
{
	std::vector <Attribuutti*> attribuutit;
};

struct Linkkimaaritys
{
	std::vector <int> tyyppinumerot;
	std::vector <std::string> tyyppinimet;
};

struct Linkkitieto
{
	std::vector <void *> *mista;
	int vektori_indeksi;
	int sektori;
	int monesko;
	int tyyppi;
};

struct Editorilinkki
{
	std::vector <void *> *mista;
	int vektori_indeksi;

	int kohdesektori;
	Objekti *mihin;
};