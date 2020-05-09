#include "maaritykset.h"

bool tilealue(int x1, int y1, int x2, int y2, int lev, int kork, int *first_x, int *first_y, int *last_x, int *last_y, Tilemap *mappi)
{
	if(x1 < x2)
	{
		*first_x = x1/32;
		*last_x = (x2 + lev-1)/32;
	}
	else
	{
		*first_x = x2/32;
		*last_x = (x1 + lev-1)/32;
	}

	if(y1 < y2)
	{
		*first_y = y1/32;
		*last_y = (y2 + kork-1)/32;
	}
	else
	{
		*first_y = y2/32;
		*last_y = (y1 + kork-1)/32;
	}

	if((*first_x) < 0) *first_x = 0;
	if((*first_y) < 0) *first_y = 0;
	
	int ax = mappi->AnnaX();
	int yy = mappi->AnnaY();
	if((*last_x) > ax-1) *last_x = ax-1;
	if((*last_y) > yy-1) *last_y = yy-1;

	if(*first_x >= ax) return false;
	if(*first_y >= yy) return false;
	if(*last_x < 0) return false;
	if(*last_y < 0) return false;

	return true;
}


float kulmakerroin(MAKI_ *maki)
{
	float kulmakerroin;
	switch(maki->tyyppi & 3)
	{
	case 0:
		kulmakerroin = 0.5;
		break;
	case 1:
		kulmakerroin = 1;
		break;
	case 2:
		kulmakerroin = 2;
		break;
	default: break;
	}
	
	if(maki->tyyppi & MIINUS) kulmakerroin *= -1;
	return kulmakerroin;
}

int maki_alku_x(MAKI_ *maki)
{
	float k = kulmakerroin(maki);
	if((k == 2 && !(maki->tyyppi & 16)) || (k == -2 && (maki->tyyppi & 16))) return maki->x_tile*32 + 16;
	else return maki->x_tile*32;
}

int maki_loppu_x(MAKI_ *maki)
{
	float k = kulmakerroin(maki);
	if((k == 2 && (maki->tyyppi & 16)) || (k == -2 && !(maki->tyyppi & 16))) return maki->x_tile*32 + 15;
	else return (maki->x_tile+1)*32 -1;
}

int vakiotermi(MAKI_ *maki, int *x1, int *y1, int *x2, int *y2)
{
	*x1 = maki->x_tile*32;
	*x2 = (maki->x_tile+1)*32-1;
	*y1 = maki->y_tile*32;
	*y2 = (maki->y_tile+1)*32-1;

	
	int z = 0;

	switch(maki->tyyppi & (255-KATTO))
	{
	case MAKI + KULMAKERROIN_YKSI + YLAVAIHE:
		z = 0;
		break;
	case MAKI + KULMAKERROIN_YKSI + MIINUS + YLAVAIHE:
		z = 31;
		break;
	case MAKI + KULMAKERROIN_PUOLI + YLAVAIHE:
		z = 0;
		*y2 = maki->y_tile*32+15;
		break;
	case MAKI + KULMAKERROIN_PUOLI + ALAVAIHE:
		z = 16;
		*y1 = maki->y_tile*32+16;
		break;
	case MAKI + KULMAKERROIN_PUOLI + MIINUS + YLAVAIHE:
		z = 15;
		*y2 = maki->y_tile*32+15;
		break;
	case MAKI + KULMAKERROIN_PUOLI + MIINUS + ALAVAIHE:
		z = 31;
		*y1 = maki->y_tile*32+16;
		break;
	case MAKI + KULMAKERROIN_KAKSI + YLAVAIHE:
		z = 0;
		*x2 = maki->x_tile*32+15;
		break;
	case MAKI + KULMAKERROIN_KAKSI + ALAVAIHE:
		z = -32;
		*x1 = maki->x_tile*32+16;
		break;
	case MAKI + KULMAKERROIN_KAKSI + MIINUS + YLAVAIHE:
		z = 63;
		*x1 = maki->x_tile*32+16;
		break;
	case MAKI + KULMAKERROIN_KAKSI + MIINUS + ALAVAIHE:
		z = 31;
		*x2 = maki->x_tile*32+15;
		break;
	default:
		break;
	}

	int k = (int)(kulmakerroin(maki)*32);
	int b = -k*maki->x_tile + maki->y_tile*32 +z;
	return b;
}

int maki_y(MAKI_ *maki, int x)
{
	int a;
	int juttu = 0;
	if(kulmakerroin(maki) == -0.5) juttu = 1;
	int plussa = 0;
	if(kulmakerroin(maki) == 2) plussa = 1;
	return plussa + (int)(vakiotermi(maki, &a, &a, &a, &a) + (kulmakerroin(maki)*(x-juttu)));
}

int maki_x(MAKI_ *maki, int y)
{
	int a;
	float k = kulmakerroin(maki);

	return (int)((y - vakiotermi(maki, &a, &a, &a, &a))/kulmakerroin(maki));
}



bool samalinja(MAKI_ *maki_1, MAKI_ *maki_2)
{
	if(maki_1->tyyppi == maki_2->tyyppi && maki_1->x_tile == maki_2->x_tile && maki_1->y_tile == maki_2->y_tile) return 1;

	if((maki_1->tyyppi & KATTO) ^ (maki_2->tyyppi & KATTO)) return 0;

	int oikea_x_tile;
	MAKI_ vasen_maki, oikea_maki;

	if(maki_1->x_tile < maki_2->x_tile)
	{
		vasen_maki = *maki_1;
		oikea_maki = *maki_2;
		oikea_x_tile = maki_2->x_tile;
	}
	else
	{
		vasen_maki = *maki_2;
		oikea_maki = *maki_1;
		oikea_x_tile = maki_2->x_tile+1;
	}

	

	// tätä testiä on taas vähän muutettu, nyt pitäisi toimia kaikissa tilanteissa

	int y1 = maki_y(&vasen_maki, oikea_x_tile*32);
	int y2 = maki_y(&oikea_maki, oikea_x_tile*32);


	int erotus = y2-y1;
	if(erotus <= 2 && erotus >= -2) return 1;

	return 0;
}


void paivita_x(int delta_x, TORMAYS *vanha, TORMAYS *uusi)
{
	if(vanha->tyyppi == 0)
	{
		*vanha  = *uusi;
		return;
	}

	// koodia on muutettu siten, että mikäli tapahtuu useampia törmäyksiä samalle x-arvolle, ja
	// tietyt törmäykset ovat mäen kulmia, niin törmäys mäkeen otetaan huomioon, jottei tiputtaisi mäestä läpi

	// päivitä_y - funktioon ei ole tehty muutoksia, koska tuskin tarvitsee
	if(delta_x < 0)
	{
		if(vanha->x <= uusi->x)
		{
			if((vanha->x < uusi->x) || (uusi->tyyppi == MAKI_TORM && !(uusi->maki.tyyppi & KATTO)))
			*vanha  = *uusi;
		}
	}
	else
	{
		
		if(vanha->x >= uusi->x)
		{
			if((vanha->x > uusi->x) || (uusi->tyyppi == MAKI_TORM && !(uusi->maki.tyyppi & KATTO)))
			*vanha  = *uusi;
		}
	}
}

void paivita_y(int delta_y, TORMAYS *vanha, TORMAYS *uusi)
{
	if(vanha->tyyppi == 0)
	{
		*vanha  = *uusi;
		return;
	}

	if(delta_y < 0)
	{
		if(vanha->y < uusi->y)
		{
			*vanha  = *uusi;
		}
	}
	else
	{

		if(vanha->y > uusi->y)
		{
			*vanha  = *uusi;
		}
	}
}












