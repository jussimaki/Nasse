class Quadtree
{
private:
	int alku_x, alku_y, loppu_x, loppu_y;
	std::vector <Peliobjekti *> objektit;
	Quadtree *yv, *yo, *av, *ao;

public:
	std::vector <Peliobjekti *> *AnnaObjektit();
	int Objektimaara();
	Quadtree(): yv(0),yo(0),av(0),ao(0){}
	bool poimiObjektit(int ax, int ay, int lx, int ly, Poiminta *poiminta, GLOBAALI *glob_data, bool piirto, int laatu = -1);
	bool Alusta(int ax, int ay, int lx, int ly);
	bool Tuhoa();
	bool LisaaObjekti(Peliobjekti *objekti);
	bool Sijoita(Peliobjekti *objekti);
	bool Aktiivisuus(int x_cam, int y_cam, int ax, int ay, int lx, int ly, GLOBAALI *glob_data, std::vector <Peliobjekti*> *kadonneet, Poiminta *logiikka, Pelisektori *sektori);
	void poistaYlimenneet(Poiminta *uusiksi);
	void tuhoaVajaat();
	void tuhoustesti();
	int annaQuadtreemaara();
};