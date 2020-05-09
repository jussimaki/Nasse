class Peli;

class Pelitaso
{
private:
	int alku_sektori, alku_x, alku_y;
	int sektori;
	std::vector <Pelisektori *> sektorit;
	int alkurullaus;


	bool vaihto;
	int vanhaenergia;
	int seuraavasektori;
	int seuraava_x, seuraava_y;
	int checkpoint_nro;

	int maalinro;
	Peli *peli;
public:
	
	void lisaaKolikko();
	void lisaaElama();

	int Toiminta(GLOBAALI *glob_data, Peli *p  = 0);
	bool Kaynnista(GLOBAALI *glob_data);
	bool AlustaObjektit(bool ekakerta);
	void pauseta();
	void itsari(GLOBAALI *glob_data);

	void checkpoint(int sekt, int x, int y, int checkpoint);
	void Sammuta(GLOBAALI *glob_data);
	bool Lataa(std::string tiedostonimi, GLOBAALI *glob_data);
	bool Vapauta(GLOBAALI *glob_data);

	std::vector <Pelisektori *> *AnnaSektorit();
	int AnnaSektori() {return sektori;}
	int AnnaAlkusektori() {return alku_sektori;}
	bool VaihdaSektori(int sekt, int x, int y, GLOBAALI *glob_data, int v_energ);
	int annarullaus() {return alkurullaus;}


};



