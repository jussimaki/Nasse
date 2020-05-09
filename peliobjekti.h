class Poiminta;
class Pelisektori;
class Pelitaso;


class Peliobjekti : public Objekti
{
protected:

	bool ainaAktiivinen;
	bool aktiivisuus;
	bool tuhottu;

	
	int uinticounter;
	int vesivoimablokki;
	Peliobjekti *taman_paalla;
	bool tallaus;
	int energia;
	int damage;
	bool suunta;

	int taulukkonumero;
	

	int vanha_x, vanha_y;
	int vanha_state;

	int sijainti_x, sijainti_y;
	int kohde_x, kohde_y;
	int fys_state;
	MAKI_ maki_data;
	

	int x_nopeus, y_nopeus;
	int x_liike, y_liike;

	
	bool vasen_torm, oikea_torm, yla_torm, ala_torm;

	bool siivet;
	bool vedessa, pinnalla;
	int pinta_y;
	bool hyppy;
	
	int animaatio;
	int anim_ruutu;
	int ruutu_counter;
	int laatu;

public:
	bool annaSiivet() {return siivet;}
	bool annaVedessa() {return vedessa;}
	
	int annaFysState() {return fys_state;}
	void asetaSiivet(bool s) {siivet = s;}
	bool tuhoaObjekti(GLOBAALI *glob_data);
	void saadaEnergia(int e) {energia = e;}
	bool annaSuunta() {return suunta;}
	
	int AnnaTaulukkoNum() {return taulukkonumero;}
	int AnnaSijaintiX();
	int AnnaSijaintiY();
	int AnnaXnopeus() {return x_nopeus;}
	int AnnaYnopeus() {return y_nopeus;}
	int AnnaVanhaX() {return vanha_x;}
	int AnnaVanhaY() {return vanha_y;}
	int AnnaVanhaState() {return vanha_state;}
	void OtaVanhat();
	int AnnaKohdeX();
	int AnnaKohdeY();
	int AnnaEnergia() {return energia;}
	int AnnaFysState() {return fys_state;}
	virtual int AnnaLeveys(GLOBAALI *glob_data);
	virtual int AnnaKorkeus(GLOBAALI *glob_data);
	bool AnnaAktiivisuus();
	int AnnaLaatu() {return laatu;}
	void AsetaLaatu(int l) {laatu = l;}


	virtual void Gravitaatio();
	void Tallaus(Peliobjekti *kuka, Pelisektori *sektori, GLOBAALI *glob_data);
	Peliobjekti(): animaatio(0), anim_ruutu(0), ruutu_counter(0), ainaAktiivinen(0) {}
	
	virtual void PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso);
	virtual bool Piirra(int x_cam, int y_cam, GLOBAALI *glob_data);
	virtual bool PiirraS(int x_s, int y_s, GLOBAALI *glob_data);
	virtual void animoiObjekti(GLOBAALI *glob_data);
	

	void merkkaaVasenTorm() {vasen_torm = 1;}
	void merkkaaOikeaTorm() {oikea_torm = 1;}
	void merkkaaYlaTorm() {yla_torm = 1;}
	void merkkaaAlaTorm() {ala_torm = 1;}

	

	void EiTuhottu();
	virtual void Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data);
	bool OnkoTuhottu();
	void AsetaTyyppi(int num);
	void AsetaTaulukkoNum(int numero) {taulukkonumero = numero;}
	void AsetaAinaAktiivinen() {ainaAktiivinen = 1;}
	
	
	

	void Damage() {if(damage) damage--;}
	
	bool onkoAinaAktiivinen() {return ainaAktiivinen;}
	void EiAinaAktiivinen() {ainaAktiivinen = 0;}
	void Aktivoi() {aktiivisuus = 1;}
	virtual void Alusta();
	virtual void Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori);
	virtual void Aseta(GLOBAALI *glob_data, int sij_x, int sij_y);
	void AsetaNopeus(int x, int y) {x_nopeus = x; y_nopeus = y;}
	virtual void Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data);
	
	bool TyontoX(Peliobjekti *tyontava, GLOBAALI *glob_data);
	bool TyontoY(Peliobjekti *tyontava, GLOBAALI *glob_data);
	bool PalkkiTyontoY(Peliobjekti *tyontava, GLOBAALI *glob_data);
	void virtual Liikuta(GLOBAALI *glob_data);
	bool SiirraX(Tilemap *mappi, Poiminta *kiinteat, bool mappitestaus, int tileset, GLOBAALI *glob_data);
	bool SiirraY(Tilemap *mappi, Poiminta *kiinteat, Poiminta *palkit, bool mappitestaus, int tileset, GLOBAALI *glob_data);

	void x_collision(GLOBAALI *glob_data, int tileset, Tilemap *mappi, Poiminta *kiinteat);
	void y_collision(GLOBAALI *glob_data, int tileset, Tilemap *mappi, Poiminta *kiinteat, Poiminta *palkit);
	void makisiirto(MAKI_ *maki, Tilemap *mappi, int tileset,GLOBAALI *glob_data, Poiminta *kiinteat);

	virtual void OtaVahinkoa(int maara, Pelisektori *sektori, GLOBAALI *glob_data);
	virtual void PaalleHyppy(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelisektori *sektori);
	void Tallaushyppy() {tallaus = 1;}  

	bool Aktiivisuustesti(int x_cam, int y_cam, GLOBAALI *glob_data, Pelisektori *sektori);
	bool Palautustesti(int x_cam, int y_cam, GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori);

	virtual bool Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso);
	
	void Reunakaannos(Tilemap *mappi, int tileset, GLOBAALI *glob_data);
	void Kaannos();
	void Tarkista_tila(Tilemap *mappi, int tileset, GLOBAALI *glob_data, Poiminta *kiinteat, Poiminta *palkit);
	void maan_paalla(Tilemap *mappi, int tileset, GLOBAALI *glob_data, Poiminta *kiinteat, Poiminta *palkit);
	void vielako_maessa2(MAKI_ *maki, int x, Tilemap *mappi, int tileset, GLOBAALI *glob_data, bool lippu, Poiminta *kiinteat, Poiminta *palkit);
	bool vielako_maessa(MAKI_ *maki, int x, Tilemap *mappi, int tileset, GLOBAALI *glob_data, bool lippu);

	bool pintatesti(Tilemap *mappi, int tileset, GLOBAALI *glob_data, int *pinta_y);
	bool reunalla(Tilemap *mappi, int tileset, bool suunta, GLOBAALI *glob_data);

	bool vinokatto(MAKI_ *maki_1, MAKI_ *maki_2, int *x, GLOBAALI *glob_data);
	bool alablokkitesti(int x_tile, int y_tile, Tilemap *mappi, int tileset, GLOBAALI *glob_data);
	bool ylablokkitesti(int x_tile, int y_tile, Tilemap *mappi, int tileset, GLOBAALI *glob_data);

	bool vesitesti(Tilemap *mappi, int tileset, GLOBAALI *glob_data);
	void KiinteaXtormays(Peliobjekti *tormattava, int delta_x, TORMAYS *tormays, GLOBAALI *glob_data);
	void KiinteaYtormays(Peliobjekti *tormattava, int delta_y, TORMAYS *tormays, GLOBAALI *glob_data);
	void PalkkiYtormays(Peliobjekti *tormattava, int delta_y, TORMAYS *tormays, GLOBAALI *glob_data);

	void KiinteaMakitormays(Peliobjekti *tormattava, MAKI_ *maki, TORMAYS *tormays, GLOBAALI *glob_data);
	void Kyydissa(GLOBAALI *glob_data);
	void KiinteaKuolema(Peliobjekti *kiinteapalkki, GLOBAALI *glob_data, Pelisektori *sektori);
	void SaadaVedessa(bool arvo) {vedessa = arvo;}
	unsigned char otaKitka(Pelisektori *sektori, GLOBAALI *glob_data);
	bool maki_check_x(MAKI_ *maki, int *tormays_x, GLOBAALI *glob_data);
	bool maki_check_y(MAKI_ *maki, int *tormays_y, GLOBAALI *glob_data);
	void tuhoaRiippuvuudet();
	void asetaSuunta(bool s) {suunta = s;}
	bool objektitLeikkaavat(Peliobjekti *obj, GLOBAALI *glob_data);
};
