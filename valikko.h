class Valikko
{
public:
	int logiikka(GLOBAALI *glob_data);
	bool alusta(GLOBAALI *glob_data);
	void vapauta(GLOBAALI *glob_data);

private:
	int valikonTila;
	int rullaus;
	int laskuri;
	Pelitaso taso;
	Valikot valikot;
};