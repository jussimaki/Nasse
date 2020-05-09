class Valikot
{
public:
	bool alusta(int nro);
	void tuhoa();
	int logiikka(GLOBAALI *glob_data);

private:

	WIN32_FIND_DATA fd;
	HANDLE hFind;

	std::vector <std::string> tiedostolista;
	
	int scroll_y;
	int nappidata[6];
	bool valitus;
	char rivi[20];
	char hakemistonimi[256];
	char polku[256];
	Valikot *uusi;
	int ikkunanro;
	int kursori_x, kursori_y;
	int yes_no;
	int nappi_index;
};

