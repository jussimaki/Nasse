#define EI 0
#define VASEN_TORM 1
#define YLA_TORM 2
#define OIKEA_TORM 3
#define ALA_TORM 4
#define MAKI_TORM 5

class Peliobjekti;

struct MAKI_
{
	int x_tile, y_tile;
	int tyyppi;
};

struct TORMAYS
{
	int x, y;
	unsigned char tyyppi;
	MAKI_ maki;
	Peliobjekti *palkki;
};

bool tilealue(int x1, int y1, int x2, int y2, int lev, int kork, int *first_x, int *first_y, int *last_x, int *last_y, Tilemap *mappi);
float kulmakerroin(MAKI_ *maki);
int maki_alku_x(MAKI_ *maki);
int maki_loppu_x(MAKI_ *maki);


int vakiotermi(MAKI_ *maki, int *x1, int *y1, int *x2, int *y2);
int maki_y(MAKI_ *maki, int x);
int maki_x(MAKI_ *maki, int y);

bool samalinja(MAKI_ *maki_1, MAKI_ *maki_2);
void paivita_x(int delta_x, TORMAYS *vanha, TORMAYS *uusi);
void paivita_y(int delta_y, TORMAYS *vanha, TORMAYS *uusi);


