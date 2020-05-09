#include "maaritykset.h"

bool Peliobjekti::objektitLeikkaavat(Peliobjekti *obj, GLOBAALI *glob_data)
{
	int obj_x = obj->sijainti_x;
	int obj_y = obj->sijainti_y;

	int lev = obj->AnnaLeveys(glob_data);
	int kork = obj->AnnaKorkeus(glob_data);

	if(obj_x + lev > sijainti_x && obj_x < sijainti_x + AnnaLeveys(glob_data)
		&& obj_y + kork > sijainti_y && obj_y < sijainti_y + AnnaKorkeus(glob_data))
		return true;

	else return false;

}
void Peliobjekti::EiTuhottu() {tuhottu = 0;}
void Peliobjekti::Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data) {tuhottu = 1; energia = 0;}
bool Peliobjekti::OnkoTuhottu() {return tuhottu;}
void Peliobjekti::AsetaTyyppi(int num) {tyyppinumero = num;}

int Peliobjekti::AnnaSijaintiX() {return sijainti_x;}
int Peliobjekti::AnnaSijaintiY() {return sijainti_y;}
int Peliobjekti::AnnaKohdeX() {return kohde_x;}
int Peliobjekti::AnnaKohdeY() {return kohde_y;}
int Peliobjekti::AnnaLeveys(GLOBAALI *glob_data) {return glob_data->objektityypit.at(tyyppinumero)->leveys;}
int Peliobjekti::AnnaKorkeus(GLOBAALI *glob_data) {return glob_data->objektityypit.at(tyyppinumero)->korkeus;}
bool Peliobjekti::AnnaAktiivisuus() {return aktiivisuus;}

unsigned char Peliobjekti::otaKitka(Pelisektori *sektori, GLOBAALI *glob_data)
{
	if(!fys_state) return 255;
	else
	{
		Tilemap *mappi = sektori->annaMappi(0);
		unsigned char kitka = 0;
		if(fys_state == FYS_KYYDISSA) return 255;

		

		int leveys = mappi->AnnaX();
		unsigned char *osoitin = mappi->AnnaData();
		int *tileset_tiletyypit = glob_data->tilesetit.at(sektori->annaSetti())->kerrokset[0]->tyypit;

		if(fys_state == FYS_MP)
		{
			int eka_x, eka_y, vika_x, vika_y;
			bool arvo = tilealue(sijainti_x, sijainti_y, sijainti_x, sijainti_y,
				AnnaLeveys(glob_data), AnnaKorkeus(glob_data),&eka_x,&eka_y,&vika_x,&vika_y,mappi);

			

			for(int x = eka_x; x <= vika_x; x++)
			{
				unsigned char arvo = kitkat[tileset_tiletyypit[osoitin[(vika_y+1)*leveys + x]]];
				if(arvo > kitka) kitka = arvo;
			}
		}
		if(fys_state == FYS_MAKI)
		{
			int y_tile = (maki_y(&maki_data, sijainti_x + AnnaLeveys(glob_data)/2))/32;
			int x_tile = (sijainti_x + AnnaLeveys(glob_data)/2)/32;

			
			unsigned char arvo = kitkat[tileset_tiletyypit[osoitin[(y_tile)*leveys + x_tile]]];
			
			if(arvo > kitka) kitka = arvo;
		}

		return kitka;
	}
}

void Peliobjekti::KiinteaKuolema(Peliobjekti *kiinteapalkki, GLOBAALI *glob_data, Pelisektori *sektori)
{

	int ax = kiinteapalkki->sijainti_x;
	int yy = kiinteapalkki->sijainti_y;
	int lev = kiinteapalkki->AnnaLeveys(glob_data);
	int kork = kiinteapalkki->AnnaKorkeus(glob_data);

	if(sijainti_x + AnnaLeveys(glob_data) > ax && sijainti_x < ax + lev
		&& sijainti_y + AnnaKorkeus(glob_data) > yy && sijainti_y < yy + kork)
	{
		Tuhottu(sektori, glob_data);
	}
}

void Peliobjekti::Kyydissa(GLOBAALI *glob_data)
{
	if((fys_state == FYS_KYYDISSA) && taman_paalla)
	{
		kohde_x += taman_paalla->kohde_x - taman_paalla->sijainti_x;
		kohde_y += taman_paalla->kohde_y - taman_paalla->sijainti_y;
	}
}

void Peliobjekti::Kaannos()
{
	if(vasen_torm || oikea_torm)
	{
		if(vasen_torm && (suunta == 1))
		{
			suunta = 0;
		}
		else if(oikea_torm && (suunta == 0))
		{
			suunta = 1;
		}
		vasen_torm = 0;
		oikea_torm = 0;
		
	}	
	else if(fys_state == FYS_MAKI && ala_torm)
	{
		suunta = suunta ^ 1;
	}

	vasen_torm = 0;
	oikea_torm = 0;
	yla_torm = 0;
	ala_torm = 0;
}

void Peliobjekti::OtaVahinkoa(int maara, Pelisektori *sektori, GLOBAALI *glob_data)
{
	if(!damage)
	{
		if(energia > 0)
		{
			damage = 120;
			energia -= maara;
		}
		else
		{
			Tuhottu(sektori, glob_data);
		}
	}
}

void Peliobjekti::Tallaus(Peliobjekti *kuka, Pelisektori *sektori, GLOBAALI *glob_data)
{

		OtaVahinkoa(1,sektori,glob_data);
		kuka->Tallaushyppy();
}

void Peliobjekti::OtaVanhat()
{
	vanha_x = sijainti_x;
	vanha_y = sijainti_y;
	vanha_state = fys_state;
}

void Peliobjekti::PaalleHyppy(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelisektori *sektori)
{

	if((pelaaja->AnnaSijaintiY() + pelaaja->AnnaKorkeus(glob_data) <= sijainti_y + 10) || pelaaja->AnnaVanhaY() + pelaaja->AnnaKorkeus(glob_data) <= vanha_y)
		//pelaaja->AnnaVanhaY() + pelaaja->AnnaKorkeus(glob_data) <= vanha_y && damage == 0)
	{
		if(damage == 0) Tallaus(pelaaja,sektori,glob_data);
	}
	else
	{
		if(damage < 110) pelaaja->OtaVahinkoa(1,sektori,glob_data);
	}

}

void Peliobjekti::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	Pelisektori *os = taso->AnnaSektorit()->at(taso->AnnaSektori());
	int x = pelaaja->sijainti_x;
	int lev = pelaaja->AnnaLeveys(glob_data);
	int y = pelaaja->sijainti_y;
	int kork = pelaaja->AnnaKorkeus(glob_data);

	if(x < sijainti_x + AnnaLeveys(glob_data) && x + lev > sijainti_x
		&& y < sijainti_y + AnnaKorkeus(glob_data) && y + kork > sijainti_y)
	{
		switch(glob_data->objektityypit.at(tyyppinumero)->paallehyppy)
		{
			case -1: break;
			case 0:
				PaalleHyppy(pelaaja, glob_data,os);
				break;
			case 1:
				pelaaja->OtaVahinkoa(1,os,glob_data);
				break;
			default: break;
		}
	}
}

void Peliobjekti::Gravitaatio()
{
	if(!fys_state)
	{
		if(!vedessa) y_nopeus += 350;
		
	}

	if(vedessa)
	{
		if(!vesivoimablokki)
		{
			
			if(x_nopeus > 3000)
				x_nopeus -= 500;

			else if(x_nopeus > 2300)
			{
				x_nopeus -= 200;
			}
			
		
			if(x_nopeus < -3000)
				x_nopeus += 500;
			else if(x_nopeus < -2300)
			{
				x_nopeus += 200;
			}

		
			if(fys_state == 0)
			{
				
				//y_nopeus -= 500;
				if(y_nopeus >= 0)
				{
					if(y_nopeus < 500) y_nopeus += 200;
					else if(y_nopeus < 1000) y_nopeus += 100;
					else if(y_nopeus < 1500) y_nopeus += 75;
					else if(y_nopeus < 2500) y_nopeus += 50;
					else if(y_nopeus < 3500) y_nopeus -= 120;
					else if(y_nopeus < 7000) y_nopeus -= 750;
					else y_nopeus -= 1000;
				}
				else
				{
					if(y_nopeus > -500) y_nopeus += 200;
					else if(y_nopeus > -2500) y_nopeus += 300;
					else if(y_nopeus > -5000) y_nopeus += 600;
					else if(y_nopeus > -7500) y_nopeus += 1000;
					else if(y_nopeus > -10000) y_nopeus += 2000;
					else if(y_nopeus > -12000) y_nopeus += 3000;
					else if(y_nopeus > -15000) y_nopeus += 4000;
					else y_nopeus += 5000;
				}
			}


			if(pinnalla && y_nopeus < 0) y_nopeus += 2*(pinta_y - sijainti_y)*(pinta_y - sijainti_y);
		}
		else
		{
			if(y_nopeus == 0) vesivoimablokki = 0;
		}
	}

	if(y_nopeus > 6000) y_nopeus = 6000;
}


void Peliobjekti::KiinteaXtormays(Peliobjekti *tormattava, int delta_x, TORMAYS *tormays, GLOBAALI *glob_data)
{

	// objekti siirtyy x_suunnassa sijainti_x -> kohde_x
	// onko tormattava kiintea objekti tiell‰?


	int t_s_x = tormattava->sijainti_x;
	int t_s_y = tormattava->sijainti_y;
	int t_lev = tormattava->AnnaLeveys(glob_data);
	int t_kork = tormattava->AnnaKorkeus(glob_data);

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);



	// leikkaako objeksi x-siirroksen j‰lkeen kiinte‰n objektin?
	if(t_s_x < kohde_x + lev && t_s_x + t_lev > kohde_x
		&& t_s_y < sijainti_y + kork && t_s_y + t_kork > sijainti_y)
	{
		// objekti oli alunperin palkin vasemmalla puolella?
		if(sijainti_x + lev <= t_s_x)
		{
			vasen_torm = 1;
			TORMAYS uusi_t;
			uusi_t.tyyppi = VASEN_TORM;
			uusi_t.x = t_s_x - lev;
			
			paivita_x(delta_x, tormays, &uusi_t);

		}
		else if(sijainti_x >= t_s_x + t_lev) // vai oikealla?
		{
			oikea_torm = 1;
			TORMAYS uusi_t;
			uusi_t.tyyppi = OIKEA_TORM;
			uusi_t.x = t_s_x + t_lev;
			
			paivita_x(delta_x, tormays, &uusi_t);
		}
	}

}

void Peliobjekti::KiinteaYtormays(Peliobjekti *tormattava, int delta_y, TORMAYS *tormays, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	int t_sij_x = tormattava->sijainti_x;
	int t_sij_y = tormattava->sijainti_y;

	int t_lev = tormattava->AnnaLeveys(glob_data);
	int t_kork = tormattava->AnnaKorkeus(glob_data);

	if(t_sij_x < sijainti_x + lev && t_sij_x + t_lev > sijainti_x
		&& t_sij_y < kohde_y + kork && t_sij_y + t_kork > kohde_y)
	{
		if(sijainti_y + kork <= t_sij_y)
		{
			yla_torm = 1;
			TORMAYS uusi_t;
			uusi_t.tyyppi = YLA_TORM;
			uusi_t.palkki = tormattava;
			uusi_t.y = t_sij_y - kork;
			
			paivita_y(delta_y, tormays, &uusi_t);

		}
		else if(sijainti_y >= t_sij_y + t_kork)
		{
			ala_torm = 1;
			TORMAYS uusi_t;
			uusi_t.tyyppi = ALA_TORM;
			uusi_t.y = t_sij_y + t_kork;
			
			paivita_y(delta_y, tormays, &uusi_t);
		}
	}

}

void Peliobjekti::PalkkiYtormays(Peliobjekti *tormattava, int delta_y, TORMAYS *tormays, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	int t_sij_x = tormattava->sijainti_x;
	int t_sij_y = tormattava->sijainti_y;

	int t_lev = tormattava->AnnaLeveys(glob_data);
	int t_kork = tormattava->AnnaKorkeus(glob_data);



	if(t_sij_x < sijainti_x + lev && t_sij_x + t_lev > sijainti_x
		&& t_sij_y < kohde_y + kork && t_sij_y + t_kork > kohde_y)
	{
		if(sijainti_y + kork <= t_sij_y)
		{
			yla_torm = 1;
			TORMAYS uusi_t;
			uusi_t.tyyppi = YLA_TORM;
			uusi_t.palkki = tormattava;
			uusi_t.y = t_sij_y - kork;
			
			paivita_y(delta_y, tormays, &uusi_t);
		}
	}
}

void Peliobjekti::KiinteaMakitormays(Peliobjekti *tormattava, MAKI_ *maki, TORMAYS *tormays, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	int t_sij_x = tormattava->sijainti_x;
	int t_sij_y = tormattava->sijainti_y;

	int t_lev = tormattava->AnnaLeveys(glob_data);
	int t_kork = tormattava->AnnaKorkeus(glob_data);

	int delta_x = kohde_x - sijainti_x;
	int delta_y = kohde_y - sijainti_y;

	//leikkaako kohdesuorakulmio kiinte‰n objektin nykyisen sijainnin?
	if(t_sij_x < kohde_x + lev && t_sij_x + t_lev > kohde_x
		&& t_sij_y < kohde_y + kork && t_sij_y + t_kork > kohde_y)
	{
		// alkuper‰inen sijainti palkin yl‰puolella
		if(sijainti_y + kork <= t_sij_y && (delta_y > 0))
		{
			// vertaus palkin ylimm‰st‰ pikselist‰
			int l_x = maki_x(maki, t_sij_y);
	
			// Mik‰li voitaisiin osua palkin yl‰reunaan

			// korjattava vika 000
			// korjattu
			
			//if(l_x -lev / 2 <= t_sij_x + t_lev -1 && l_x + lev/2 >= t_sij_x)
			if(l_x -lev / 2 <= t_sij_x + t_lev -1 && l_x - lev/2 + lev -1 >= t_sij_x)
			{
				TORMAYS uusi_t;
				uusi_t.tyyppi = YLA_TORM;

				// muokattu ottamaan x-sijainti palkkia ylemm‰lt‰ pikselilt‰
				//uusi_t.x = maki_x(maki,t_sij_y)-(lev/2);
				uusi_t.x = maki_x(maki,t_sij_y-1)-(lev/2);
				uusi_t.y = t_sij_y - kork;
				uusi_t.palkki = tormattava;
				paivita_x(delta_x, tormays, &uusi_t);
			}			
		}
		else if((sijainti_y >= t_sij_y+t_kork) && (delta_y < 0)) // alkuper‰inen sijainti palkin alapuolella
		{
			// verrataan palkin alimmaista pikseli‰

			//int l_x = maki_x(maki, t_sij_y + t_kork + kork-1);
			int l_x = maki_x(maki, t_sij_y + t_kork -1+ kork-1);


			// korjattava vika 000
			// korjattu

			//if(l_x -lev / 2 <= t_sij_x + t_lev -1 && l_x + lev/2 >= t_sij_x)
			if(l_x -lev / 2 <= t_sij_x + t_lev -1 && l_x - lev/2 + lev -1 >= t_sij_x)
			{
				TORMAYS uusi_t;
				uusi_t.tyyppi = ALA_TORM;

				// objekti asetetaan silti palkin alapuolelle
				uusi_t.x = maki_x(maki,t_sij_y+t_kork+kork-1)-(lev/2);
				uusi_t.y = t_sij_y + t_kork;
				paivita_x(delta_x, tormays, &uusi_t);
			}
		}
			
		if(sijainti_x + lev <= t_sij_x && (delta_x > 0)) // alkuper‰inen sijainti palkin vasemmalla puolella
		{
			// verrataan palkin vasemmaisimpaan pikseliin

			// korjattava vika 000
			// korjattu

			//int l_y = maki_y(maki, t_sij_x - lev/2);
			int l_y = maki_y(maki, t_sij_x - lev + lev/2 + 1);

			if(l_y - kork+1 < t_sij_y + t_kork && l_y >= t_sij_y)
			{
				TORMAYS uusi_t;
				uusi_t.tyyppi = VASEN_TORM;
				uusi_t.x = t_sij_x - lev;

				// korjattava vika 000
				// korjattu
				//uusi_t.y = maki_y(maki,t_sij_x - (lev/2))-kork + 1; //lis‰tty + 1 loppuun

				uusi_t.y = maki_y(maki,t_sij_x - lev + lev/2)-kork + 1; //lis‰tty + 1 loppuun
				paivita_x(delta_x, tormays, &uusi_t);
			}
		}
		else if(sijainti_x >= t_sij_x+t_lev && (delta_x < 0)) // alkuper‰inen sijainti palkin oikealla puolella
		{
			// vertailu tehd‰‰n palkin viimeisen‰ oikeallla olevan pikselin kohdalla

			//int l_y = maki_y(maki, t_sij_x+t_lev + lev/2);

			int l_y = maki_y(maki, t_sij_x+t_lev -1 + lev/2);

			// muutettu: <= -> <, saattaa olla riski, mik‰li ei vertailla oikean pikselin kohdalla
			
			//if(l_y - kork+1 <= t_sij_y+t_kork && l_y >= t_sij_y)
			if(l_y - kork+1 < t_sij_y+t_kork && l_y >= t_sij_y)
			{
				TORMAYS uusi_t;
				uusi_t.tyyppi = OIKEA_TORM;

				// sijoitetaan palkin oikealle puolelle
				uusi_t.x = t_sij_x + t_lev;
				uusi_t.y = maki_y(maki,t_sij_x + t_lev + (lev/2))-kork + 1; //lis‰tty + 1 loppuun
				paivita_x(delta_x, tormays, &uusi_t);
			}
		}
	}
}

bool Peliobjekti::TyontoX(Peliobjekti *tyontava, GLOBAALI *glob_data)
{
	int tyontava_x = tyontava->sijainti_x;
	int tyontava_y = tyontava->sijainti_y;
	

	int tyontava_kohde_x = tyontava->kohde_x;
	int tyontava_leveys = tyontava->AnnaLeveys(glob_data);
	int tyontava_korkeus = tyontava->AnnaKorkeus(glob_data);

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	if(tyontava_kohde_x < sijainti_x + lev && tyontava_kohde_x + tyontava_leveys > sijainti_x
		&& tyontava_y < sijainti_y + kork && tyontava_y + tyontava_korkeus > sijainti_y)
	{
		if(sijainti_x + lev <= tyontava_x)
		{
			
			int kohdearvo = tyontava_kohde_x-lev;
			if(kohde_x > kohdearvo)
			{
				kohde_x = kohdearvo;
				vasen_torm = 1;
			}

		}
		else if(sijainti_x >= tyontava_x + tyontava_leveys)
		{
			
			int kohdearvo = tyontava_kohde_x + tyontava_leveys;
			if(kohde_x < kohdearvo)
			{
				kohde_x = kohdearvo;
				oikea_torm = 1;
			}
		}

		if(fys_state == FYS_MAKI)
			kohde_y = maki_y(&maki_data, kohde_x + lev/2) - kork+1;

	}

	return true;
}

bool Peliobjekti::TyontoY(Peliobjekti *tyontava, GLOBAALI *glob_data)
{
	int tyontava_sijainti_y = tyontava->sijainti_y;

	int tyontava_sijainti_x = tyontava->sijainti_x;
	int tyontava_kohde_y = tyontava->kohde_y;

	int tyontava_leveys = tyontava->AnnaLeveys(glob_data);
	int tyontava_korkeus = tyontava->AnnaKorkeus(glob_data);

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);


	// meneekˆ kiinte‰ objekti t‰m‰n objektin nykyisen sijainnin p‰‰lle y-siirron j‰lkeen?
	if(tyontava_sijainti_x < sijainti_x + lev && tyontava_sijainti_x + tyontava_leveys > sijainti_x
		&& tyontava_kohde_y < sijainti_y + kork && tyontava_kohde_y + tyontava_korkeus > sijainti_y)
	{
		// oliko objekti kiinte‰n objektin alkuper‰isen sijainnin yl‰puolella?
		if(sijainti_y + kork <= tyontava_sijainti_y)
		{
			
			int kohdearvo = tyontava_kohde_y-kork;
			if(kohde_y > kohdearvo)
			{
				yla_torm = 1;
				kohde_y = kohdearvo;
				fys_state = FYS_KYYDISSA;
				taman_paalla = tyontava;
			}
		} // vai alapuolella?
		else if(sijainti_y >= tyontava_sijainti_y + tyontava_korkeus)
		{
			int kohdearvo = tyontava_kohde_y + tyontava_korkeus;
			if(kohde_y < kohdearvo)
			{
				ala_torm = 1;
				kohde_y = kohdearvo;
				hyppy = 0;
				if(y_nopeus < 0) y_nopeus = 0;
			}
		}
	}

	return true;
}

bool Peliobjekti::PalkkiTyontoY(Peliobjekti *tyontava, GLOBAALI *glob_data)
{
	int tyontava_y = tyontava->sijainti_y;

	int tyontava_x = tyontava->sijainti_x;
	int tyontava_kohde_y = tyontava->kohde_y;

	int tyontava_leveys = tyontava->AnnaLeveys(glob_data);
	int tyontava_korkeus = tyontava->AnnaKorkeus(glob_data);

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	if(tyontava_x < sijainti_x + lev && tyontava_x + tyontava_leveys > sijainti_x
		&& tyontava_kohde_y < sijainti_y + kork && tyontava_kohde_y + tyontava_korkeus > sijainti_y)
	{
		if(sijainti_y + kork <= tyontava_y)
		{
			int kohdearvo = tyontava_kohde_y-kork;
			if(kohde_y > kohdearvo) 
			{
				yla_torm = 1;
				kohde_y = kohdearvo;
				fys_state = FYS_KYYDISSA;
				taman_paalla = tyontava;
			}
		}

	}

	return true;
}

bool Peliobjekti::SiirraX(Tilemap *mappi, Poiminta *kiinteat, bool mappitestaus, int tileset, GLOBAALI *glob_data)
{

	if(!mappitestaus)
	{
		sijainti_x = kohde_x;
	}
	else
	{
		
		if(fys_state == FYS_MAKI)
		{	
			makisiirto(&maki_data, mappi, tileset, glob_data, kiinteat);
			
		}
		else
		{
			x_collision(glob_data, tileset, mappi, kiinteat);
		}

	}

	return true;
}


bool Peliobjekti::SiirraY(Tilemap *mappi, Poiminta *kiinteat, Poiminta *palkit, bool mappitestaus, int tileset, GLOBAALI *glob_data)
{
	if(!mappitestaus) sijainti_y = kohde_y;
	else
	{
		if(fys_state != FYS_MAKI)
		{
			y_collision(glob_data, tileset, mappi, kiinteat, palkit);
		}
	}
	return true;
}

bool Peliobjekti::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	animaatio = 0;
	return true;
}

void Peliobjekti::Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori)
{
	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;

	int lev = glob_data->objektityypit.at(tyyppinumero)->leveys;
	int kork = glob_data->objektityypit.at(tyyppinumero)->korkeus;


	hyppy = 0;
	ala_torm = 0;
	yla_torm = 0;
	vasen_torm = 0;
	oikea_torm = 0;
	aktiivisuus = 0;
	x_liike = 0;
	y_liike = 0;
	x_nopeus = 0;
	y_nopeus = 0;
	animaatio = 0;
	anim_ruutu = 0;
	ruutu_counter = 0;
	fys_state = 0;
	taman_paalla = 0;
	tallaus = 0;
	vedessa = 0;
	pinnalla = 0;
	vesivoimablokki = 0;
	uinticounter = 0;

	sijainti_x = x_*32 + 16 - lev/2;
	kohde_x = sijainti_x;

	
	unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_*mapinleveys + x_]]];

	if(tyyppi & MAKI && !(tyyppi & KATTO))
	{
		MAKI_ maki;
		maki.tyyppi = tyyppi & (255-VESI);
		maki.x_tile = x_;
		maki.y_tile = y_;
		sijainti_y = maki_y(&maki,sijainti_x + lev/2) -kork +1;

		maki_data = maki;
		fys_state = FYS_MAKI;


	}
	else
	{	
		sijainti_y = (y_+1)*32 -glob_data->objektityypit.at(tyyppinumero)->korkeus;
	}

	kohde_y = sijainti_y;
}

void Peliobjekti::Aseta(GLOBAALI *glob_data, int sij_x, int sij_y)
{
	hyppy = 0;
	ala_torm = 0;
	yla_torm = 0;
	vasen_torm = 0;
	oikea_torm = 0;
	aktiivisuus = 0;
	x_liike = 0;
	y_liike = 0;
	x_nopeus = 0;
	y_nopeus = 0;
	animaatio = 0;
	anim_ruutu = 0;
	ruutu_counter = 0;
	fys_state = 0;
	taman_paalla = 0;
	tallaus = 0;
	vedessa = 0;
	pinnalla = 0;
	vesivoimablokki = 0;
	uinticounter = 0;
	
	sijainti_x = sij_x;
	sijainti_y = sij_y;

	kohde_x = sijainti_x;
	kohde_y = sijainti_y;
}

void Peliobjekti::Liikuta(GLOBAALI *glob_data)
{

	if(x_nopeus == 0 || (x_nopeus > 0 && x_liike < 0) || (x_nopeus < 0 && x_liike > 0)) x_liike = 0;
	if(y_nopeus == 0 || (y_nopeus > 0 && y_liike < 0) || (y_nopeus < 0 && y_liike > 0)) y_liike = 0;

	float kerroin = 1;
	if(fys_state == FYS_MAKI)
	{
		char tyyppi = maki_data.tyyppi;
		if(tyyppi & KULMAKERROIN_PUOLI) kerroin = (float)0.894;
		else if(tyyppi & KULMAKERROIN_YKSI) kerroin = (float)0.707;
		else if(tyyppi & KULMAKERROIN_KAKSI) kerroin = (float)0.447; 
	}
	x_liike += (int)(x_nopeus*kerroin);
		
	int jako_x = x_liike/1000;
	kohde_x = sijainti_x;

	kohde_x += jako_x;
	x_liike -= jako_x*1000;

	if(fys_state == FYS_MAKI)
	{
		y_liike = 0;
		y_nopeus = 0;

		kohde_y = maki_y(&maki_data, kohde_x + AnnaLeveys(glob_data)/2) - AnnaKorkeus(glob_data)+1;
		
	}
	else
	{
		y_liike += y_nopeus;
	
		int jako_y = y_liike/1000;
		kohde_y = sijainti_y;
	
		kohde_y += jako_y;
		y_liike -= jako_y*1000;
	}

}


void Peliobjekti::Alusta()
{
	damage = 0;
	energia = 0;
}

void Peliobjekti::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	if(taulukkonumero != Pelaaja_num)
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->sijainti_x < sijainti_x) suunta = 0; else suunta = 1;
	} else suunta = 1;
	
}

bool Peliobjekti::Aktiivisuustesti(int x_cam, int y_cam, GLOBAALI *glob_data, Pelisektori *sektori)
{
	int leveys = AnnaLeveys(glob_data);
	int korkeus = AnnaKorkeus(glob_data);

	if(aktiivisuus)
	{
		if(sijainti_x + 640 < x_cam || sijainti_x > x_cam + 640 + 640 ||
			sijainti_y + 480 < y_cam || sijainti_y > y_cam + 480 + 480)
		{
			aktiivisuus = 0;
			return 1;
		}
	}
	else
	{
		if(sijainti_x + 160 > x_cam && sijainti_x < x_cam + 640 + 160 &&
			sijainti_y + korkeus >= y_cam -5 && sijainti_y < y_cam + 480 + 5)
		{
			Aktivoi();
			Kaynnista(sektori, glob_data);
		}
	}

	return 0;
}

bool Peliobjekti::Palautustesti(int x_cam, int y_cam, GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori)
{
	if(x_*32 + 240 < x_cam || x_*32 > x_cam + 640 + 240 ||
			y_*32 + 180 < y_cam || y_*32 > y_cam + 480 + 180)
	{
		Aseta(glob_data, mappi, tileset, sektori);
		return true;
	}
	return false;
}



void Peliobjekti::animoiObjekti(GLOBAALI *glob_data)
{
	int koko = glob_data->objektityypit.at(tyyppinumero)->animaatiot.size();

	if(koko == 0) return;

	if(animaatio >= koko)
		animaatio = koko-1;

	Animaatio *os =
		glob_data->objektityypit.at(tyyppinumero)->animaatiot.at(animaatio);

	animoi(os,&ruutu_counter,&anim_ruutu);
}

bool Peliobjekti::Piirra(int x_cam, int y_cam, GLOBAALI *glob_data)
{
	if(!damage || damage % 10 < 5)
	{

		Animaatio *os =
		glob_data->objektityypit.at(tyyppinumero)->animaatiot.at(animaatio);

		int x_offset = glob_data->objektityypit.at(tyyppinumero)->x_offset;
		int y_offset = glob_data->objektityypit.at(tyyppinumero)->y_offset;
		piirraAnimaatio(os,sijainti_x-x_cam-x_offset,sijainti_y-y_cam-y_offset,&ruutu_counter,&anim_ruutu,glob_data);

		char *jono;
		number_to_string(x_nopeus, &jono);
		//number_to_string(glob_data->objektityypit.at(annaTyyppi())->piirtojarjestys,&jono);
		
		//Tulosta(jono,sijainti_x-x_cam,sijainti_y-y_cam,glob_data);
		delete []jono;
	}
	return true;
}

bool Peliobjekti::PiirraS(int x_s, int y_s, GLOBAALI *glob_data)
{
	if(!damage || damage % 10 < 5)
	{

		Animaatio *os =
		glob_data->objektityypit.at(tyyppinumero)->animaatiot.at(animaatio);

		piirraAnimaatio(os,x_s,y_s,&ruutu_counter,&anim_ruutu,glob_data);
	}
	return true;
}


void Peliobjekti::Reunakaannos(Tilemap *mappi, int tileset, GLOBAALI *glob_data)
{
	if(fys_state == FYS_MP)
	{

		if(reunalla(mappi, tileset, suunta, glob_data))
		{
			suunta = suunta ^ 1;
		}
	}
}

void Peliobjekti::Tarkista_tila(Tilemap *mappi, int tileset, GLOBAALI *glob_data, Poiminta *kiinteat, Poiminta *palkit)
{

	if(fys_state == FYS_MP || (fys_state == 0 && y_nopeus == 0))
	{
		maan_paalla(mappi, tileset, glob_data, kiinteat, palkit);
	}
	else if(fys_state == FYS_MAKI)
	{			
		
		vielako_maessa2(&maki_data, sijainti_x + AnnaLeveys(glob_data)/2, mappi, tileset, glob_data, false, kiinteat, palkit);
	}
	
	bool vesi = vedessa;
	pinnalla = pintatesti(mappi, tileset, glob_data, &pinta_y);

	if(!vesi)
	{
		if(!pinnalla)
		{
			vedessa = vesitesti(mappi, tileset, glob_data);
		} else vedessa = 0;
	} else vedessa = vesitesti(mappi, tileset, glob_data);

	if((!vesi) && vedessa) {vesivoimablokki = 0;}


	if((fys_state == FYS_KYYDISSA) && taman_paalla)
	{
		int ax = taman_paalla->sijainti_x;
		int yy = taman_paalla->sijainti_y;
		int lev = taman_paalla->AnnaLeveys(glob_data);
		int kork = taman_paalla->AnnaKorkeus(glob_data);

		int tamanleveys = AnnaLeveys(glob_data);
		int tamankorkeus = AnnaKorkeus(glob_data);


		// Tarkastetaan ollaanko en‰‰ palkin p‰‰ll‰
		if((sijainti_x + tamanleveys <= ax) || (sijainti_x >= ax+lev) || (yy-tamankorkeus != sijainti_y))
		{
			fys_state = FYS_EI;
			taman_paalla = 0;

			// Ei olla, mutta tarkastetaan ollaanko jonkin muun objektin p‰‰ll‰
			

			int index = 0;
			Peliobjekti *objekti;
			Poiminta *ptr = palkit;

			bool loppu = 0;

			while(objekti = ptr->annaAlkio(&index,&ptr))
			{
				int palkki_y = objekti->sijainti_y;
				int palkki_x1 = objekti->sijainti_x;
				int palkki_x2 = palkki_x1 + objekti->AnnaLeveys(glob_data) -1;

				if((sijainti_x + tamanleveys > palkki_x1) && (sijainti_x <= palkki_x2) && (palkki_y-tamankorkeus == sijainti_y))
				{
					loppu = 1;
					fys_state = FYS_KYYDISSA;
					taman_paalla = objekti;
				}

				if(loppu) break;
			}

			ptr = kiinteat;
			index = 0;

			if(!loppu)
			while(objekti = ptr->annaAlkio(&index,&ptr))
			{
				int palkki_y = objekti->sijainti_y;
				int palkki_x1 = objekti->sijainti_x;
				int palkki_x2 = palkki_x1 + objekti->AnnaLeveys(glob_data) -1;

				if((sijainti_x + tamanleveys > palkki_x1) && (sijainti_x <= palkki_x2) && (palkki_y-tamankorkeus == sijainti_y))
				{
					loppu = 1;
					fys_state = FYS_KYYDISSA;
					taman_paalla = objekti;
				}

				if(loppu) break;
			}
			
			
			// t‰h‰n tarkistus, onko muiden p‰‰ll‰
		}

	}
	

	if(fys_state) y_nopeus = 0;

}

void Peliobjekti::tuhoaRiippuvuudet()
{
	if(fys_state == FYS_KYYDISSA && taman_paalla)
	{
		if(taman_paalla->OnkoTuhottu())
		{
			fys_state = FYS_EI;
			taman_paalla = 0;
		}
	}
}

bool Peliobjekti::vielako_maessa(MAKI_ *maki, int x, Tilemap *mappi, int tileset, GLOBAALI *glob_data, bool lippu)
{
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	int a;
	unsigned char *map = mappi->AnnaData();
	
	int mapinleveys = mappi->AnnaX();

	int x_tile = x/32;
	int x_puoli_tile = x/16;

	int b = vakiotermi(maki, &a, &a, &a, &a);
	float k = kulmakerroin(maki);
	int y = b + (int)(k*x_puoli_tile*16);
	int y_tile = y/32;
	if(k < 0 && y%32 == 0) y_tile--;

	unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x_tile]]];
	if(!(tyyppi & MAKI)) return 0;
	
	MAKI_ maki_2;
	maki_2.x_tile = x_tile;
	maki_2.y_tile = y_tile;
	maki_2.tyyppi = tyyppi & (255-VESI);
	
	if(kulmakerroin(maki) != kulmakerroin(&maki_2)) return 0;

	return samalinja(maki, &maki_2);
}



void Peliobjekti::vielako_maessa2(MAKI_ *maki, int x, Tilemap *mappi, int tileset, GLOBAALI *glob_data, bool lippu, Poiminta *kiinteat, Poiminta *palkit)
{

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);
	fys_state = FYS_EI;
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	int a;
	unsigned char *map = mappi->AnnaData();
	
	int mapinleveys = mappi->AnnaX();

	int x_tile = x/32;
	int x_puoli_tile = x/16;

	int b = vakiotermi(maki, &a, &a, &a, &a);
	float k = kulmakerroin(maki);
	int y = b + (int)(k*x_puoli_tile*16);
	int y_tile = y/32;
	if(k < 0 && y%32 == 0) y_tile--;

	
	unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x_tile]]];
	if(!(tyyppi & MAKI))
	{
			unsigned char t2 = tiletyyppitaulukko[tileset_tiletyypit[map[(y_tile+1)*mapinleveys + x_tile]]];
			if(t2 & YLA_BLOKKI)
			{
				// pelaaja laitetaan ainoastaan ylh‰‰lle ment‰ess‰ maan tasolle
				
				

				if((maki->tyyppi & MIINUS) && (x_tile > maki->x_tile))
				{
					int erotus = maki_y(maki,x_tile*32-1) - (y_tile+1)*32;

					//sijainti_y = (y_tile+1)*32-kork;
					//fys_state = FYS_MP;
					if(erotus < 4 && erotus > -4)
					{
						kohde_y = (y_tile+1)*32-kork +1;
						y_collision(glob_data,tileset,mappi,kiinteat,palkit);
					}
				}

				if(!(maki->tyyppi & MIINUS) && (x_tile < maki->x_tile))
				{
					int erotus = maki_y(maki,(x_tile+1)*32)-(y_tile+1)*32;

					if(erotus < 4 && erotus > -4)
					{
						kohde_y = (y_tile+1)*32-kork +1;
						y_collision(glob_data,tileset,mappi,kiinteat,palkit);
					}

				}
			}
	}
	else
	{
		MAKI_ maki_2;
		maki_2.x_tile = x_tile;
		maki_2.y_tile = y_tile;
		maki_2.tyyppi = tyyppi & (255-VESI);
		
		int delta_x = maki_2.x_tile - maki->x_tile;

		if(samalinja(maki, &maki_2))
		{
			if(kulmakerroin(maki) == kulmakerroin(&maki_2)) // t‰ss‰ tapauksessa se on sama m‰ki
			{
				fys_state = FYS_MAKI;
			}
			else
			{
				
				if(delta_x >= 0)
				{
					if((kulmakerroin(maki) < 0) || kulmakerroin(&maki_2) <= 2*kulmakerroin(maki))
					{
						//fys_state = FYS_MAKI;
						//maki_data = maki_2;
						//sijainti_y = maki_y(&maki_2,x)-kork+1;

						kohde_y = maki_y(&maki_2,x)-kork+2;
						y_collision(glob_data,tileset,mappi,kiinteat,palkit);
					}
				}
				else
				{
					if((kulmakerroin(maki) > 0) || kulmakerroin(&maki_2) >= 2*kulmakerroin(maki))
					{
						//fys_state = FYS_MAKI;
						//maki_data = maki_2;
						//sijainti_y = maki_y(&maki_2,x)-kork+1;

						kohde_y = maki_y(&maki_2,x)-kork+2;
						y_collision(glob_data,tileset,mappi,kiinteat,palkit);
					}
				}
			}
		}
	}

	
}



void Peliobjekti::x_collision(GLOBAALI *glob_data, int tileset, Tilemap *mappi, Poiminta *kiinteat)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();

	TORMAYS tormays;
	tormays.tyyppi = 0;
	
	int alku_x, alku_y, loppu_x, loppu_y;
	bool arvo = tilealue(sijainti_x, sijainti_y, kohde_x, sijainti_y, lev, kork,
		 &alku_x, &alku_y, &loppu_x, &loppu_y, mappi);


	int delta_x = kohde_x - sijainti_x;
	int delta_y = kohde_y - sijainti_y;

	if(delta_x == 0) return;
	
	Poiminta *ptr = kiinteat;
	int indeksi = 0;
	Peliobjekti *obj;

	if(ptr)
	while(obj = ptr->annaAlkio(&indeksi, &ptr))
	{
		KiinteaXtormays(obj,delta_x,&tormays,glob_data);
	}
	

	if(kohde_x < 0 && sijainti_x >= 0)
	{
		TORMAYS uusi_t;
		uusi_t.x = 0;
		uusi_t.tyyppi = OIKEA_TORM;
		paivita_x(delta_x, &tormays, &uusi_t);
	}
	
	if((kohde_x+lev-1 >= mapinleveys*32) && (sijainti_x+lev-1 < mapinleveys*32))
	{
			TORMAYS uusi_t;
			uusi_t.x = mapinleveys*32-lev;
			uusi_t.tyyppi = VASEN_TORM;
			paivita_x(delta_x, &tormays, &uusi_t);
	}

	

	if(delta_x > 0)
	{
		
		if(arvo)
		for(int x_tile = alku_x; x_tile <= loppu_x; x_tile++)
		{
			for(int y_tile = alku_y; y_tile <= loppu_y; y_tile++)
			{
				unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x_tile]]];

				if(tyyppi & MAKI)
				{
					if(((tyyppi & MIINUS) && !(tyyppi & KATTO)) ||
						(!(tyyppi & MIINUS) && (tyyppi & KATTO)))
					{
						
						MAKI_ maki;
						maki.x_tile = x_tile;
						maki.y_tile = y_tile;
						maki.tyyppi = tyyppi & (255-VESI);

						int tormays_x;
						if(maki_check_x(&maki, &tormays_x, glob_data))
						{
							TORMAYS uusi_t;
							uusi_t.x = tormays_x;
							uusi_t.tyyppi = MAKI_TORM;
							uusi_t.maki = maki;

							paivita_x(delta_x, &tormays, &uusi_t);
						}
					}
				}
				else if(tyyppi & VASEN_BLOKKI)
				{
					if(sijainti_x + lev <= x_tile*32)
					{
						TORMAYS uusi_t;
						uusi_t.x = x_tile*32 -lev;
						uusi_t.tyyppi = VASEN_TORM;
						paivita_x(delta_x, &tormays, &uusi_t);
					}
				}
			}
		}
	}
	else
	{

		if(arvo)
		for(int x_tile = alku_x; x_tile <= loppu_x; x_tile++)
		{
			for(int y_tile = alku_y; y_tile <= loppu_y; y_tile++)
			{
				unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x_tile]]];

				if(tyyppi & MAKI)
				{
					if((!(tyyppi & MIINUS) && !(tyyppi & KATTO)) ||
						((tyyppi & MIINUS) && (tyyppi & KATTO)))
					{
						MAKI_ maki;
						maki.x_tile = x_tile;
						maki.y_tile = y_tile;
						maki.tyyppi = tyyppi & (255-VESI); 

						int tormays_x;

						

						if(maki_check_x(&maki, &tormays_x, glob_data))
						{
							TORMAYS uusi_t;
							uusi_t.x = tormays_x;
							uusi_t.tyyppi = MAKI_TORM;
							uusi_t.maki = maki;

							paivita_x(delta_x, &tormays, &uusi_t);
						}
					}
				}
				else if(tyyppi & OIKEA_BLOKKI)
				{
					if(sijainti_x >= (x_tile+1)*32)
					{
						TORMAYS uusi_t;
						uusi_t.x = (x_tile+1)*32;
						uusi_t.tyyppi = OIKEA_TORM;

						paivita_x(delta_x, &tormays, &uusi_t);
					}
				}
			}
		}
	}
	


	if(tormays.tyyppi) sijainti_x = tormays.x;
	else sijainti_x = kohde_x;

	if(tormays.tyyppi == VASEN_TORM || tormays.tyyppi == OIKEA_TORM)
	{
		if(tormays.tyyppi == VASEN_TORM) merkkaaVasenTorm();
		else merkkaaOikeaTorm();

		x_nopeus = 0;
		
		kohde_x = tormays.x;
		return;
	}
	

	// Mik‰li osutaan vinoon kattoon, siirryt‰‰n viel‰ y-suunnassa
	if(tormays.tyyppi == MAKI_TORM)
	{
		if(tormays.maki.tyyppi & KATTO)
		{
			kohde_x = tormays.x;

			x_nopeus = 0;
			if(delta_y < 0) y_nopeus = 0;
		}
		else
		{
			// t‰m‰ kohta saattaa vaatia viel‰ s‰‰tˆ‰
			//if(delta_y >= 0)
			if(1)
			{
				int lopullinen_kohde_y = kohde_y;
				
				fys_state = FYS_MAKI;
				maki_data = tormays.maki;
				
				kohde_y = maki_y(&(tormays.maki), kohde_x + lev/2) - kork+1;
				
				
				makisiirto(&maki_data, mappi, tileset, glob_data,kiinteat);

				if(lopullinen_kohde_y < sijainti_y)
				{
					kohde_y = lopullinen_kohde_y;
					fys_state = FYS_EI;
					
				}
				else y_nopeus = 0;
			}
			else
			{
				kohde_x = tormays.x;
			}
		}
	}

}

void Peliobjekti::maan_paalla(Tilemap *mappi, int tileset, GLOBAALI *glob_data, Poiminta *kiinteat, Poiminta *palkit)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	bool lippu = 0;
	if(fys_state == FYS_MP) lippu = 1;

	fys_state = 0;

	unsigned char *map = mappi->AnnaData();
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	int mapinleveys = mappi->AnnaX();


	if((sijainti_y + kork)%32 != 0) return;

	int x1, x2, y1, y2;
	int x;
	if(!tilealue(sijainti_x, sijainti_y, sijainti_x, sijainti_y, lev, kork, &x1, &y1, &x2, &y2, mappi)) return;

	if(y2 == mappi->AnnaY()-1) return;
	int keski_x = (sijainti_x + lev/2)/32;

	unsigned char tyyppi;

	MAKI_ maki;
	maki.x_tile = keski_x;
	maki.y_tile = y2+1;
	maki.tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + keski_x]]] & (255-VESI);


	// ensin tarkistetaan, seist‰‰nkˆ periaatteessa maan p‰‰ll‰
	for(x = x1; x <= x2; x++)
	{
		tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + x]]];
		if(tyyppi & YLA_BLOKKI) {fys_state = FYS_MP;}
	}


	//jos kuitenkin puolet leveydest‰ on 'm‰ess‰', ei olla maan p‰‰ll‰
	for(x = keski_x; x <= x2; x++)
	{	
		tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + x]]];
		if((tyyppi & MAKI) && (tyyppi & MIINUS) && (tyyppi & 16))
		{
			fys_state = FYS_EI;


			// jos ollaan kuitenkin oltu viimeksi maan p‰‰ll‰, laskeudutaan m‰keen
			float k = kulmakerroin(&maki);
			if(k == -0.5 || k == 0.5)
			{
				if(lippu)
				{
					//sijainti_y = maki_y(&maki, (sijainti_x + lev/2))-kork+1;
					//fys_state = FYS_MAKI;
					//maki_data = maki;

					kohde_y = maki_y(&maki, (sijainti_x + lev/2))-kork+2;
					y_collision(glob_data,tileset,mappi,kiinteat,palkit);

				}
			}
		}
	}

	for(x = x1; x <= keski_x; x++)
	{
		tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + x]]];
		if((tyyppi & MAKI) && !(tyyppi & MIINUS) && (tyyppi & 16))
		{
			fys_state = FYS_EI;
			float k = kulmakerroin(&maki);

			if(k == -0.5 || k == 0.5)
			{
				if(lippu)
				{
					//sijainti_y = maki_y(&maki, (sijainti_x + lev/2))-kork+1;
					//fys_state = FYS_MAKI;
					//maki_data = maki;

					kohde_y = maki_y(&maki, (sijainti_x + lev/2))-kork+2;
					y_collision(glob_data,tileset,mappi,kiinteat,palkit);
				}
			}
		}
	}
}

bool Peliobjekti::maki_check_x(MAKI_ *maki, int *tormays_x, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);
	
	int delta_x = kohde_x - sijainti_x;
	int x1, y1, x2, y2;
	
	int b = vakiotermi(maki, &x1, &y1, &x2, &y2);
	float k = kulmakerroin(maki);
	bool juttu = 0;
	

		
	int pelaaja_y;
	if(maki->tyyppi & KATTO) pelaaja_y = sijainti_y;
	else pelaaja_y = sijainti_y + kork -1;

	if(pelaaja_y < y1 || pelaaja_y > y2) return 0;
	
	int x = maki_x(maki, pelaaja_y);

	if((k == -0.5) || (k == 0.5)) juttu = 1;
	
	
	if(delta_x > 0)
	{
		if(sijainti_x + lev/2 <= x + juttu && kohde_x + lev/2 > x + juttu)
		{
			
			*tormays_x = x + juttu - lev/2;
			return 1;
		}
	}
	else if(delta_x < 0)
	{
		if(sijainti_x + lev/2 >= x && kohde_x + lev/2 < x)
		{
			*tormays_x = x - lev/2;
			return 1;
		}
	}

	return 0;
}

bool Peliobjekti::vesitesti(Tilemap *mappi, int tileset, GLOBAALI *glob_data)
{
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	int eka_x, eka_y, vika_x, vika_y;
	if(!tilealue(sijainti_x, sijainti_y, sijainti_x, sijainti_y, lev, kork, &eka_x, &eka_y, &vika_x, &vika_y, mappi)) return false;


	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();

	for(int x = eka_x; x <= vika_x; x++)
	{
		for(int y = eka_y; y <= vika_y; y++)
		{
			unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y*mapinleveys + x]]];
			if(tyyppi & VESI) return true;
		}
	}

	return false;
}

bool Peliobjekti::pintatesti(Tilemap *mappi, int tileset, GLOBAALI *glob_data, int *pinta_y)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;

	int eka_x, eka_y, vika_x, vika_y;
	if(!tilealue(sijainti_x, sijainti_y, sijainti_x, sijainti_y, lev, kork, &eka_x, &eka_y, &vika_x, &vika_y, mappi)) return false;

	

	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();

	*pinta_y = (eka_y+1)*32;

	/*
	for(int x = eka_x; x <= vika_x; x++)
	{
		unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[eka_y*mapinleveys + x]]];
		if(tyyppi != 0) return false;
	}*/

	int aksa = (sijainti_x + lev/2)/32;

	unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[eka_y*mapinleveys + aksa]]];
	if((tyyppi & VESI) == 0) return true;
	
	return false;
}

bool Peliobjekti::reunalla(Tilemap *mappi, int tileset, bool suunta, GLOBAALI *glob_data)
{

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();
	int mapinkorkeus = mappi->AnnaY();

	if((sijainti_y + kork)%32 != 0) return false;

	int x1, y1, x2, y2;
	int x;
	int keskitile = (sijainti_x + lev/2)/32;
	if(!tilealue(sijainti_x, sijainti_y, sijainti_x, sijainti_y, lev, kork, &x1, &y1, &x2, &y2, mappi)) return false;

	if(y2 == mapinkorkeus-1) return false;
	
	if(suunta)
	{
		if(!(tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + x2]]] & YLA_BLOKKI))
		{
			for(x = keskitile; x <= x2; x++)
			{
				unsigned char tyyppi_1 = tiletyyppitaulukko[tileset_tiletyypit[map[y2*mapinleveys + x]]];
				unsigned char tyyppi_2 = tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + x]]];
				
				if((tyyppi_1 & MAKI) && (tyyppi_1 & MIINUS) && !(tyyppi_1 & KATTO))
				{
					MAKI_ maki;
					maki.tyyppi = tyyppi_1 & (255-VESI);
					
					if(kulmakerroin(&maki) == -1) return 0;
					else if((tyyppi_1 & 16) == ALAVAIHE) return 0;
				}
				else if((tyyppi_2 & MAKI) && !(tyyppi_2  & MIINUS) && !(tyyppi_2 & KATTO))
				{
					
					if((tyyppi_2 & 16) == YLAVAIHE) {return 0;}
				}
			}

			return 1;
		}
		else return 0;
	}
	else
	{
		if(!(tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + x1]]] & YLA_BLOKKI))
		{
			for(x = keskitile; x >= x1; x--)
			{
				unsigned char tyyppi_1 = tiletyyppitaulukko[tileset_tiletyypit[map[y2*mapinleveys + x]]];
				unsigned char tyyppi_2 = tiletyyppitaulukko[tileset_tiletyypit[map[(y2+1)*mapinleveys + x]]];

				if((tyyppi_1 & MAKI) && !(tyyppi_1 & MIINUS))
				{
					MAKI_ maki;
					maki.tyyppi = tyyppi_1 & (255-VESI);
				
					if(kulmakerroin(&maki) == 1) return 0;
					else if((tyyppi_1 & 16) == ALAVAIHE) return 0;
				}
				else if((tyyppi_2 & MAKI) && (tyyppi_2  & MIINUS))
				{
					if((tyyppi_2 & 16) == YLAVAIHE) return 0;
				}
			}
			
			return 1;
		}
		else return 0;
	}
}

void Peliobjekti::makisiirto(MAKI_ *maki, Tilemap *mappi, int tileset,GLOBAALI *glob_data, Poiminta *kiinteat)
{

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();

	TORMAYS tormays;
	tormays.tyyppi = 0;

	int alku_x, alku_y, loppu_x, loppu_y;
	bool arvo = tilealue(sijainti_x, sijainti_y, kohde_x, kohde_y, lev, kork, &alku_x, &alku_y, &loppu_x, &loppu_y, mappi);

	int delta_x = kohde_x - sijainti_x;
	if(delta_x == 0)
	{
		sijainti_x = kohde_x;
		sijainti_y = kohde_y;
		return;
	}

	int indeksi = 0;
	Poiminta *ptr = kiinteat;
	Peliobjekti *obj;

	//kiinteat->KiinteaMakitormays(vanha, uusi, maki, &tormays, glob_data);
	while(obj = ptr->annaAlkio(&indeksi, &ptr))
		KiinteaMakitormays(obj,maki,&tormays,glob_data);

	

	if(arvo)for(int x_tile = alku_x; x_tile <= loppu_x; x_tile++)
	{
		for(int y_tile = alku_y; y_tile <= loppu_y; y_tile++)
		{
			unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x_tile]]];
			
			if((!(maki->tyyppi & MIINUS) && delta_x > 0) ||
				((maki->tyyppi & MIINUS) && delta_x < 0))
			{
				if(!(tyyppi & MAKI) && (tyyppi & YLA_BLOKKI))
				{
					if(sijainti_y + kork-1 < y_tile*32) // oltiinko alunperin yl‰puolella?
					{
						// testaus tapahtuu ylimm‰lt‰ pikselilt‰

						int y = y_tile*32;
						int x = maki_x(maki, y);
	
						// korjattava vika 000
						// korjattu

						//if(x -lev / 2 <= (x_tile+1)*32 -1 && x + lev/2 >= x_tile*32)

						if(x -lev / 2 <= (x_tile+1)*32 -1 && x - lev/2 + lev -1 >= x_tile*32)
						{
							TORMAYS uusi_t;

							// korjaus
							//uusi_t.x = x - lev/2;

							// asetetaan tilen yl‰puolelle
							uusi_t.x = maki_x(maki, y-1) - lev/2;
							uusi_t.y = y_tile*32 - kork;
							uusi_t.tyyppi = YLA_TORM;
							uusi_t.palkki = 0;
							paivita_x(delta_x, &tormays, &uusi_t);
						}
					}
				}
			}
			if((!(maki->tyyppi & MIINUS) && delta_x < 0) ||
				((maki->tyyppi & MIINUS) && delta_x > 0))
			{
				if(!(tyyppi & MAKI) && tyyppi & ALA_BLOKKI)
				{
					if(sijainti_y > (y_tile+1)*32-1) // oltiinko alunperin alapuolella?
					{

						// testaus muokattu tarkistamaan alimmasta pikselist‰						
						//int y = (y_tile+1)*32 + kork-1;

						int y = (y_tile+1)*32-1 + kork-1;
						int x = maki_x(maki, y);
	
						// korjattava vika 000
						// korjattu

						//if(x -lev / 2 <= (x_tile+1)*32 -1 && x + lev/2 >= x_tile*32)

						if(x -lev / 2 <= (x_tile+1)*32 -1 && x - lev/2 + lev -1 >= x_tile*32)
						{
							TORMAYS uusi_t;
							//uusi_t.x = x - lev/2;

							// x-koordinaatti laitetaan vastaamaan tilen alapuolta,
							// ei alinta pikseli‰
							uusi_t.x = maki_x(maki,y+1) - lev/2;
							uusi_t.y = (y_tile+1)*32;
							uusi_t.tyyppi = ALA_TORM;
							paivita_x(delta_x, &tormays, &uusi_t);
						}
					}
				}
			}
			if(delta_x > 0)
			{
				if(tyyppi & MAKI)
				{
					if(!(tyyppi & KATTO))
					{						
						MAKI_ maki_2;
						maki_2.x_tile = x_tile;
						maki_2.y_tile = y_tile;
						maki_2.tyyppi = tyyppi & (255-VESI);
						
						if(kulmakerroin(&maki_2) < kulmakerroin(maki))
						{
							int alku_x = maki_alku_x(&maki_2);
							
							if(alku_x > maki_alku_x(maki))
							{
								
							if(samalinja(maki, &maki_2) && kohde_x + lev/2 >= alku_x)
							{
								TORMAYS uusi_t;
								
								uusi_t.x = alku_x - lev/2;
								uusi_t.y = maki_y(&maki_2, alku_x)-kork+1;
								uusi_t.tyyppi = MAKI_TORM;
								uusi_t.maki = maki_2;
								paivita_x(delta_x, &tormays, &uusi_t);
							}
							
							}
						}
					}
					else
					{
						MAKI_ maki_2;
						maki_2.x_tile = x_tile;
						maki_2.y_tile = y_tile;
						maki_2.tyyppi = tyyppi & (255-VESI);
						

						if(kulmakerroin(&maki_2) > kulmakerroin(maki))
						{
							int leikkaus_x;
							if(vinokatto(maki, &maki_2, &leikkaus_x, glob_data))
							{
								if(sijainti_x + lev / 2 <= leikkaus_x)
								{
																
								TORMAYS uusi_t;
								uusi_t.x = leikkaus_x-lev/2;
								uusi_t.y = maki_y(maki,leikkaus_x)-kork+1;
								uusi_t.tyyppi = MAKI_TORM;
								uusi_t.maki = maki_2;
								paivita_x(delta_x, &tormays, &uusi_t);
								}
							}
						}
					}
				}
				else if(tyyppi & VASEN_BLOKKI)
				{
					if(sijainti_x+lev-1 < x_tile*32) // oltiinko alun perin vasemmalla puolella
					{
						//  tarkastaminen muokattu teht‰v‰ksi vasemmaisimmasta pikselist‰

						// korjattava vika 000
						// korjattu
						// int x = x_tile*32 - lev/2;

						int x = x_tile*32 - lev + lev/2 + 1;
						int y = maki_y(maki, x);
	
						if(y - kork+1 < (y_tile+1)*32 && y >= y_tile*32)
						{
							TORMAYS uusi_t;

							// asetetaan tilen vasemmalle puolelle
							uusi_t.x = x_tile*32 - lev;
							uusi_t.tyyppi = VASEN_TORM;
							paivita_x(delta_x, &tormays, &uusi_t);
						}
					}
				}
			}
			if(delta_x < 0)
			{
				if(tyyppi & MAKI)
				{
					
					if(!(tyyppi & KATTO))
					{
						
						MAKI_ maki_2;
						maki_2.x_tile = x_tile;
						maki_2.y_tile = y_tile;
						maki_2.tyyppi = tyyppi & (255-VESI);

						
						if(kulmakerroin(&maki_2) > kulmakerroin(maki))
						{
							int loppu_x = maki_loppu_x(&maki_2);
							if(loppu_x < maki_loppu_x(maki))
							{
							if(samalinja(maki, &maki_2) && kohde_x + lev/2 <= loppu_x)
							{
								
								TORMAYS uusi_t;
								uusi_t.x = loppu_x - lev/2;
								uusi_t.y = maki_y(&maki_2,loppu_x)-kork+1;
								uusi_t.tyyppi = MAKI_TORM;
								uusi_t.maki = maki_2;
								paivita_x(delta_x, &tormays, &uusi_t);
							}
							}
							
						}
					}
					else
					{
						MAKI_ maki_2;
						maki_2.x_tile = x_tile;
						maki_2.y_tile = y_tile;
						maki_2.tyyppi = tyyppi & (255-VESI);

						
						if(kulmakerroin(maki) > kulmakerroin(&maki_2))
						{
							
							int leikkaus_x;
							
							if(vinokatto(maki, &maki_2, &leikkaus_x, glob_data))
							{
								if(sijainti_x + lev / 2 >= leikkaus_x)
								{
																
								TORMAYS uusi_t;
								uusi_t.x = leikkaus_x-lev/2;
								uusi_t.y = maki_y(maki,leikkaus_x)-kork+1;
								uusi_t.tyyppi = MAKI_TORM;
								uusi_t.maki = maki_2;
								paivita_x(delta_x, &tormays, &uusi_t);
								}
							}
						}
					}
				}
				else if(tyyppi & OIKEA_BLOKKI)
				{
					if(sijainti_x > (x_tile+1)*32-1) // oltiinko alun perin oikealla puolella
					{
						// muokattu testaamaan oikean puoleinen pikseli
						int x = (x_tile+1)*32 -1+ lev/2;

						//int x = (x_tile+1)*32 + lev/2;
						int y = maki_y(maki, x);
	

						// muutetttu <= -> <, saattaa olla riski, ellei verrata oikean pikselin kohdalla

						// if(y - kork+1 <= (y_tile+1)*32 && y >= y_tile*32)

						if(y - kork+1 < (y_tile+1)*32 && y >= y_tile*32)
						{
							TORMAYS uusi_t;

							// asetetaan tilen oikealle puolelle
							uusi_t.x = (x_tile+1)*32;
							uusi_t.tyyppi = OIKEA_TORM;
							paivita_x(delta_x, &tormays, &uusi_t);
						}
					}
				}
			}
		}
	}

	if(tormays.tyyppi != 0)
	{
		if(tormays.tyyppi == YLA_TORM)
		{
			sijainti_x = tormays.x;
			sijainti_y = tormays.y;

			if(tormays.palkki == 0) fys_state = FYS_MP;	
			else
			{
				fys_state = FYS_KYYDISSA;
				taman_paalla = tormays.palkki;
			}

			kohde_y = sijainti_y;


			///// TƒMƒ ON MUUTETTU!!!
			
			x_collision(glob_data,tileset,mappi,kiinteat);
			//collision_detection(vanha, uusi, mappi);
			merkkaaYlaTorm();

			return;
		}
		else if(tormays.tyyppi == ALA_TORM)
		{
			merkkaaAlaTorm();
			sijainti_x = tormays.x;
			sijainti_y = maki_y(maki, tormays.x + lev/2) - kork+1;

			if(vielako_maessa(maki, tormays.x + lev/2, mappi, tileset, glob_data, true))
			{
				fys_state = FYS_MAKI;
			}
			
			kohde_y = sijainti_y;

			if(fys_state == FYS_MAKI)
			{
				kohde_x = sijainti_x;
				x_nopeus = 0;
				return;
			}
			else
			{
				fys_state = FYS_EI;
				
				x_collision(glob_data,tileset,mappi,kiinteat);
				return;
			}
		}
		else if(tormays.tyyppi == VASEN_TORM || tormays.tyyppi == OIKEA_TORM)
		{
			if(tormays.tyyppi == VASEN_TORM) merkkaaVasenTorm();
			else merkkaaOikeaTorm();


			sijainti_x = tormays.x;
			sijainti_y = maki_y(maki, tormays.x+(lev/2)) - kork+1;
			return;
		}
		else if(tormays.tyyppi == MAKI_TORM)
		{

			sijainti_x = tormays.x;
			sijainti_y = tormays.y;

			if(tormays.maki.tyyppi & KATTO)
			{
				x_nopeus = 0;
				kohde_x = sijainti_x;
				kohde_y = sijainti_y;
				merkkaaAlaTorm();
			}
			else
			{
								
				maki_data = tormays.maki;

				kohde_y = maki_y(&maki_data, kohde_x + lev/2) - kork+1;

				makisiirto(&maki_data, mappi, tileset, glob_data, kiinteat);
			}
		}
	}
	else 
	{
		sijainti_x = kohde_x;
		sijainti_y = kohde_y;
	}
	
	return;
}

void Peliobjekti::y_collision(GLOBAALI *glob_data, int tileset, Tilemap *mappi, Poiminta *kiinteat, Poiminta *palkit)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();
	int mapinkorkeus = mappi->AnnaY();
	
	TORMAYS tormays;
	tormays.tyyppi = 0;
	
	int alku_x, alku_y, loppu_x, loppu_y;
	bool arvo = tilealue(sijainti_x, sijainti_y, sijainti_x, kohde_y, lev, kork, &alku_x, &alku_y, &loppu_x, &loppu_y, mappi);

	int delta_y = kohde_y - sijainti_y;

	if(delta_y == 0) return;

	Peliobjekti *obj;
	Poiminta *ptr = kiinteat;
	int indeksi = 0;

	if(ptr)
	{
		while(obj = ptr->annaAlkio(&indeksi, &ptr))
			KiinteaYtormays(obj,delta_y,&tormays,glob_data);
	}

	indeksi = 0;
	ptr = palkit;

	if(ptr)
	{
		while(obj = ptr->annaAlkio(&indeksi, &ptr))
			PalkkiYtormays(obj,delta_y,&tormays,glob_data);
	}

	

	if(kohde_y < 0 && sijainti_y >= 0)
	{
		TORMAYS uusi_t;
		uusi_t.y = 0;
		uusi_t.tyyppi = ALA_TORM;
		paivita_y(delta_y, &tormays, &uusi_t);
	}

	if((kohde_y+kork-1 >= mapinkorkeus*32) && (sijainti_y+kork-1 < mapinkorkeus*32))
	{
		TORMAYS uusi_t;
		uusi_t.y = mapinkorkeus*32-kork;
		uusi_t.tyyppi = YLA_TORM;
		uusi_t.palkki = 0;
		paivita_y(delta_y, &tormays, &uusi_t);
	}
	
	if(delta_y > 0)
	{
		if(arvo)
		for(int x_tile = alku_x; x_tile <= loppu_x; x_tile++)
		{
			for(int y_tile = alku_y; y_tile <= loppu_y; y_tile++)
			{
				unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x_tile]]];

				if(tyyppi & MAKI)
				{
					if(!(tyyppi & KATTO))
					{
						MAKI_ maki;
						maki.x_tile = x_tile;
						maki.y_tile = y_tile;
						maki.tyyppi = tyyppi & (255-VESI);

						int tormays_y;
						
						if(maki_check_y(&maki, &tormays_y, glob_data))
						{
							TORMAYS uusi_t;
							uusi_t.y = tormays_y;
							uusi_t.tyyppi = MAKI_TORM;
							uusi_t.maki = maki;

							paivita_y(delta_y, &tormays, &uusi_t);
						}
					}
				}
				else if(tyyppi & YLA_BLOKKI)
				{
					if(sijainti_y+kork <= y_tile*32)
					{
						if(ylablokkitesti(x_tile,y_tile,mappi,tileset,glob_data))
						{
							TORMAYS uusi_t;
							uusi_t.y = y_tile*32 -kork;
							uusi_t.tyyppi = YLA_TORM;
							uusi_t.palkki = 0;
							paivita_y(delta_y, &tormays, &uusi_t);
						}
					}
				}
			}
		}			
	}
	else
	{
		if(arvo)
		for(int x_tile = alku_x; x_tile <= loppu_x; x_tile++)
		{
			for(int y_tile = alku_y; y_tile <= loppu_y; y_tile++)
			{
				unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x_tile]]];

				if(tyyppi & MAKI)
				{
					if(tyyppi & KATTO)
					{
						MAKI_ maki;
						maki.x_tile = x_tile;
						maki.y_tile = y_tile;
						maki.tyyppi = tyyppi & (255-VESI);

						int tormays_y;

						if(maki_check_y(&maki, &tormays_y, glob_data))
						{
							TORMAYS uusi_t;
							uusi_t.y = tormays_y;
							uusi_t.tyyppi = MAKI_TORM;
							uusi_t.maki = maki;


							paivita_y(delta_y, &tormays, &uusi_t);
						}
					}
				}
				else if(tyyppi & ALA_BLOKKI)
				{
					if(sijainti_y >= (y_tile+1)*32)
					{
						if(alablokkitesti(x_tile,y_tile,mappi,tileset,glob_data))
						{
							TORMAYS uusi_t;
							uusi_t.y = (y_tile+1)*32;
							uusi_t.tyyppi = ALA_TORM;
							paivita_y(delta_y, &tormays, &uusi_t);
						}
					}
				}	
			}
		}
	}
		
	if(tormays.tyyppi != 0)
	{
		sijainti_y = tormays.y;

		if(tormays.tyyppi ==  YLA_TORM)
		{
			if(tormays.palkki == 0)
			{
				if(tormays.y != mapinkorkeus*32-kork) fys_state = FYS_MP;
			}
			else
			{
				fys_state = FYS_KYYDISSA;
				taman_paalla = tormays.palkki;
			}

			merkkaaYlaTorm();

			if(y_nopeus > 0) y_nopeus = 0;
		}
		else if(tormays.tyyppi == ALA_TORM)
		{
			hyppy = 0;
			merkkaaAlaTorm();
			if(y_nopeus < 0) y_nopeus = 0;
			
		}
	}
	else sijainti_y = kohde_y;
	
	if(tormays.tyyppi == MAKI_TORM)
	{
		if(!(tormays.maki.tyyppi & KATTO))
		{
			fys_state = FYS_MAKI;
			maki_data = tormays.maki;
		} else y_nopeus = 0;
	}
}


bool Peliobjekti::alablokkitesti(int x_tile, int y_tile, Tilemap *mappi, int tileset, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();

	int keski_x = (kohde_x + (lev)/2)/32;
	
	if(keski_x < x_tile)
	{

		for(int x = keski_x; x < x_tile; x++)
		{
			unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x]]];
			if((tyyppi & MAKI) && !(tyyppi & 16) && !(tyyppi & MIINUS) && (tyyppi & KATTO)) return 0;
		}
	}
	if(keski_x > x_tile)
	{
		for(int x = keski_x; x > x_tile; x--)
		{
			unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x]]];
			if((tyyppi & MAKI) && !(tyyppi & 16) && (tyyppi & MIINUS) && (tyyppi & KATTO)) return 0;
		}
	}

	return 1;
}

bool Peliobjekti::ylablokkitesti(int x_tile, int y_tile, Tilemap *mappi, int tileset, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
	unsigned char *map = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();

	int keski_x = (kohde_x + (lev)/2)/32;
	
	if(keski_x < x_tile)
	{

		for(int x = keski_x; x < x_tile; x++)
		{
			unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x]]];
			if((tyyppi & MAKI) && (tyyppi & 16) && (tyyppi & MIINUS) && !(tyyppi & KATTO))
			{return 0;}
		}
	}
	if(keski_x > x_tile)
	{
		for(int x = keski_x; x > x_tile; x--)
		{
			unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[map[y_tile*mapinleveys + x]]];
			if((tyyppi & MAKI) && (tyyppi & 16) && !(tyyppi & MIINUS) && !(tyyppi & KATTO))
				{return 0;}
		}
	}

	return 1;
}

bool Peliobjekti::vinokatto(MAKI_ *maki_1, MAKI_ *maki_2, int *x, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	int delta_x = kohde_x - sijainti_x;
	int x1, y1, x2, y2;
	int a;
	int b1 = vakiotermi(maki_1, &a, &a, &a, &a);
	int b2 = vakiotermi(maki_2, &x1, &y1, &x2, &y2);
	int plussa = 0;

	float f_kk1 = kulmakerroin(maki_1);
	float f_kk2 = kulmakerroin(maki_2);

	int kk_1, kk_2;

	if(f_kk1 >= 0) kk_1 = (int)(f_kk1*10 +0.5);
	else kk_1 = (int)(f_kk1*10 -0.5);

	if(f_kk2 >= 0) kk_2 = (int)(f_kk2*10 +0.5);
	else kk_2 = (int)(f_kk2*10 -0.5);

	int jaettava = b1-b2-kork;
	jaettava *= 10;

	

	int jakaja = kk_2 -kk_1;
	if(jaettava < 0) {jaettava *= -1; jakaja *= -1;}

	if(kk_1 < 0) // menn‰‰n oikealle
	{
		switch(kk_2)
		{
		case 10:
			switch(kk_1)
			{
			case -10:
				if((jaettava % jakaja) == 10) plussa = 1;
				break;
			case -5:
				plussa = 1;
				break;
			case -20:
				if((jaettava % jakaja) == 20) plussa = 1;
				break;
			default: break;
			}
			break;

		case 5:
			switch(kk_1)
			{
			case -10:
				plussa = 1;
				break;
			case -20:
				if((jaettava % jakaja) >= 10) plussa = 1;
				break;
			case -5:
				if((jaettava/10)%2) plussa = 2;
				else plussa = 1;
				break;
			default: break;
			}
			break;

		case 20:
			switch(kk_1)
			{
			case -10:
				if((jaettava % jakaja) == 20) plussa = 1;
				break;
			case -5:
				if((jaettava % jakaja) >= 10) plussa = 1;
				break;
			case -20:
				if((jaettava % jakaja) == 30) plussa = 1;
				break;
			}
			break;
		default: break;
		}
	}
	else
	{
		if((kk_1 == 5) && (kk_2 == -5) && ((jaettava/10)%2)) plussa = -1;
	}

	*x = (int)((b1 - b2 - kork)/(kulmakerroin(maki_2)-kulmakerroin(maki_1)));
	*x = (*x) + plussa;

	
	
	if(*x >= x1 && *x <= x2)
	{
		if((delta_x > 0 && kohde_x + lev/2 >= *x) || (delta_x < 0 && kohde_x + lev/2<= *x)) return 1;
		else return 0;
	} else return 0;	
}

bool Peliobjekti::maki_check_y(MAKI_ *maki, int *tormays_y, GLOBAALI *glob_data)
{
	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);
	int delta_y = kohde_y - sijainti_y;
	int x1, y1, x2, y2;
	
	int b = vakiotermi(maki, &x1, &y1, &x2, &y2);
	int juttu = 0;
	float k = kulmakerroin(maki);
	
	if(k == -2 || k == 2) juttu = 1;
	
	if(sijainti_x + lev/2 < x1 || sijainti_x + lev/2 > x2) return 0;
	
	int y = maki_y(maki,sijainti_x + lev/2);
	
	

	if(delta_y > 0)
	{
		if(sijainti_y + kork -1 <= y && kohde_y + kork -1 > y)
		{
			*tormays_y = y - kork+1;
			return 1;
		}
	}
	else if(delta_y < 0)
	{
		if(sijainti_y >= y -juttu && kohde_y < y -juttu)
		{
			*tormays_y = y -juttu;
			return 1;
		}
	}


	return 0;
}

bool Peliobjekti::tuhoaObjekti(GLOBAALI *glob_data)
{
	TuhoaData(glob_data,false);

	return true;
}
