class Pelaaja : public Peliobjekti
{
private:
	int muuttuja;
	int juoksucounter;
	Peliobjekti *kannettava;
	int vaihe;
	bool juoksulippu;
	bool koynnos;
	int ilmajuttu;
	std::vector <Peliobjekti *> inventory;
public:
	bool annaKoynnos() {return koynnos;}
	void piirraInventory(GLOBAALI *glob_data);
	bool Piirra(int x_cam, int y_cam, GLOBAALI *glob_data);
	void saadailmajuttu(int moi) {ilmajuttu = moi;}
	void AsetaAlkuKoordinaatit(int ax, int yy);
	bool Kontrolli(GLOBAALI *glob_data, Pelisektori *sektori, Pelitaso *taso);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Gravitaatio();
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	void animoiObjekti(GLOBAALI *glob_data);
	void OtaVahinkoa(int maara, Pelisektori *sektori, GLOBAALI *glob_data);
};

class Pomppija : public Peliobjekti
{
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Piikkityyppi : public Peliobjekti
{
public:
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
};

class Jamppa : public Peliobjekti
{
private:
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Aija : public Peliobjekti
{
	public:
		bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Lentoaija : public Peliobjekti
{
	protected:
	int aika;
	public:
		void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
		void Liikuta(GLOBAALI *glob_data);
		bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Kolikko : public Peliobjekti
{
	public:
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
};

class Ovi : public Peliobjekti
{
	public:
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
};

class Ovi2 : public Peliobjekti
{
	public:
	void Avaaovi(Peliobjekti *avaaja, Peliobjekti *avain, GLOBAALI *glob_data, Pelitaso *taso);
};


class Seilaaja : public Peliobjekti
{
	protected:
	int aika;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	void Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori);
	void Liikuta(GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Palkki : public Seilaaja
{
};

class KiinteaPalkki : public Peliobjekti
{
protected:
	int aika;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	void Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori);
	int AnnaLeveys(GLOBAALI *glob_data);
	int AnnaKorkeus(GLOBAALI *glob_data);
	void Liikuta(GLOBAALI *glob_data);
	bool Piirra(int x_cam, int y_cam, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Maali : public Peliobjekti
{
	public:
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
};

class Amppari : public Peliobjekti
{
private:
	int vaihe;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

class Sylkija : public Peliobjekti
{
private:
	int hyppytila;
	int kulma;
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};


class Sydan : public Peliobjekti
{
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
};

class Piikkipallo : public Peliobjekti
{
protected:
	int aika;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	void Liikuta(GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	bool Piirra(int x_cam, int y_cam, GLOBAALI *glob_data);
};

class Matkaaja : public Peliobjekti
{
protected:
	int indeksi;
	int aika;
	int k_x, k_y;
	bool naks;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	void Liikuta(GLOBAALI *glob_data);
};

class Matkapalkki : public Matkaaja
{
};

class Pomppivapiikkipallo : public Peliobjekti
{
protected:
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Seilaavapiikkipallo: public Seilaaja
{
};

class Bossi1 : public Peliobjekti
{
private:
	int moodi;
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Alusta();
	void Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

class Bossi2 : public Peliobjekti
{
private:
	int moodi;
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Alusta();
	void Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);

};

class Bossi3 : public Peliobjekti
{
private:
	int moodi;
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Alusta();
	void Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

class Bossi4 : public Peliobjekti
{
private:
	int moodi;
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Alusta();
	void Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

class Kala : public Peliobjekti
{
private:
	int counter;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori);
};

class Lokki : public Peliobjekti
{
private:
	bool puoli;
	int moodi;
	int counter;
	int k_x, k_y;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Paikallaanhyppija : public Peliobjekti
{
private:
	int aika;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Seuraaja : public Peliobjekti
{
	public:
		bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);

};
class Seuraavahyppija : public Peliobjekti
{
private:
	int counter;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Kala2 : public Peliobjekti
{
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Meritahti : public Palkki
{
};

class Lumipallonheittaja : public Peliobjekti
{
protected:
	int counter;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Lumipallo : public Peliobjekti
{
private:
	bool g;
public:
	void asetag(bool gee) {g = gee;}
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Tulipallo : public Lumipallo
{
};

class Tulipallo2 : public Peliobjekti
{
private:
	int aika;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};


class Lumipallonheittaja2 : public Lumipallonheittaja
{
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
};

class Laatikko : public Peliobjekti
{
private:
	int kantotila;
	Peliobjekti *kantaja;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Otalaatikko(Peliobjekti *objekti, GLOBAALI *glob_data);
	void Paastairti(GLOBAALI *glob_data);
	void Heita(GLOBAALI *glob_data);
};

class Palanen : public Peliobjekti
{
private:
	bool liike;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

class Tulipallotyyppi : public Peliobjekti
{
protected:
	int counter;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
};

class Avain : public Peliobjekti
{
public:
	bool otaAvain(Peliobjekti *objekti, Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
};

class Pyorivapalkki : public Piikkipallo
{
};

class Jeppe : public Peliobjekti
{
protected:
	int counter;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
};

class Siivet : public Peliobjekti
{
public:
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
};

class Liekinheitin : public Peliobjekti
{
private:
	int aika;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data) {aika = 0;}
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
};

class Liekki : public Peliobjekti
{
private:
	int counter;
public:
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);

};


class Tippukivi : public Peliobjekti
{
private:
	int tila;
	int counter;
public:
	void Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
};

class Tippupalkki : public Peliobjekti
{
private:
	int tila;
	int counter;
public:
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
};

class Jaapuikko : public Tippukivi
{
};

class Jousi : public Peliobjekti
{
private:
	int jannitys;
	int tila;
public:
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	void Liikuta(GLOBAALI *glob_data);
};

class Checkpoint : public Peliobjekti
{
private:
	int tila;
public:
	void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
	bool Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso);
	void Alusta();
};

class Tykki : public Peliobjekti
{
private:
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

class Tulipallo3 : public Peliobjekti
{
private:
	int moodi;
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

class Lieska : public Peliobjekti
{
private:
	int counter;
public:
	bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
};

