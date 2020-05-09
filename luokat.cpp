#include "maaritykset.h"
extern Poiminta glob_poiminta;

extern int hyppy_nopeus, bee, kerroin, gravitaatio, imaisu, putoamisnopeus;



#define jarrutus 0.05



int jarrut[] = {125, 125, 125, 125};
int venaus = 50; //x



bool Laatikko::Otalaatikko(Peliobjekti *objekti, GLOBAALI *glob_data)
{
	if(kantotila != 0) return false;

	if(objekti->AnnaSijaintiX() < sijainti_x + AnnaLeveys(glob_data) && objekti->AnnaSijaintiX() + objekti->AnnaLeveys(glob_data) > sijainti_x
		&& objekti->AnnaSijaintiY() < sijainti_y + AnnaKorkeus(glob_data) && objekti->AnnaSijaintiY() + objekti->AnnaKorkeus(glob_data) > sijainti_y)
	{
		x_nopeus = 0;
		y_nopeus = 0;
		fys_state = 0;
		kantaja = objekti;
		kantotila = 1;

		sijainti_x = kantaja->AnnaSijaintiX() + kantaja->AnnaLeveys(glob_data)/2-16;
		sijainti_y = kantaja->AnnaSijaintiY()-32;

		return true;
	}
	else return false;

}

void Laatikko::Paastairti(GLOBAALI *glob_data)
{
	kantotila = 0;
	if(kantaja->annaSuunta()) sijainti_x = kantaja->AnnaSijaintiX() + kantaja->AnnaLeveys(glob_data) -AnnaLeveys(glob_data);
	else sijainti_x = kantaja->AnnaSijaintiX();
	sijainti_y = kantaja->AnnaSijaintiY();
	x_nopeus = (kantaja->AnnaSijaintiX()-kantaja->AnnaVanhaX())*1000;
}

void Laatikko::Heita(GLOBAALI *glob_data)
{
	x_nopeus = 11000;
	if(kantaja->annaSuunta()) sijainti_x = kantaja->AnnaSijaintiX() + kantaja->AnnaLeveys(glob_data) -AnnaLeveys(glob_data);
	else sijainti_x = kantaja->AnnaSijaintiX();
	sijainti_y = kantaja->AnnaSijaintiY();
	if(!kantaja->annaSuunta()) x_nopeus *= -1;
	kantotila = 2;
}

void Laatikko::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	kantaja = 0;
	kantotila = 0;
}

bool Laatikko::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	glob_poiminta.Alusta();

	if(kantotila == 1) fys_state = 0;
	

	sektori->annaQuadtree()->poimiObjektit(sijainti_x -320,sijainti_y -240,sijainti_x + 320, sijainti_y +240,&glob_poiminta,glob_data,0);

	Poiminta *pointteri = &glob_poiminta;

	std::vector <Peliobjekti *> *ainaAktiiviset = sektori->annaAinaAktiiviset();

	for(int index = 0; index < ainaAktiiviset->size(); index++)
	{
		if(!ainaAktiiviset->at(index)->OnkoTuhottu())
		{
			glob_poiminta.Lisaaobjekti(ainaAktiiviset->at(index));
		}
	}
	

	Peliobjekti *obj;

	int indeksi = 0;

	bool osuma = 0;
	if(kantotila == 2)
	while(obj = pointteri->annaAlkio(&indeksi,&pointteri))
	{
		int tnro = obj->annaTyyppi();

		if(glob_data->objektityypit[tnro]->damagenotto)
		if(obj != this && obj != kantaja)
		if((obj->AnnaSijaintiX() < sijainti_x + AnnaLeveys(glob_data))
			&& (obj->AnnaSijaintiX() + obj->AnnaLeveys(glob_data) > sijainti_x)
			&& (obj->AnnaSijaintiY() < sijainti_y + AnnaKorkeus(glob_data))
			&& (obj->AnnaSijaintiY() + obj->AnnaKorkeus(glob_data) > sijainti_y))
		{
			//obj->Tuhottu(sektori);
			obj->OtaVahinkoa(1,sektori,glob_data);
			osuma = 1;
		}
	
	}

	if(osuma || vasen_torm || oikea_torm)
	{
		Tuhottu(sektori, glob_data);
		
		for (int index = 0; index < 250; index++)
		{
			Palanen *os = 0;
			os = (Palanen*)luo_objekti(Palanen_num,glob_data,sektori);
			if(os)
			{
			
				os->Alusta();
				os->Aseta(glob_data,sijainti_x+rand()%31, sijainti_y+rand()%31);
				int kulma = rand()%360;
				int nopeus = 1000 + rand()%15000;
				os->AsetaNopeus(cos(kulma*2*3.14/360)*nopeus,sin(kulma*2*3.14/360)*nopeus);
				os->EiTuhottu();
				sektori->lisaaTuhoutuviin(os);
				os->Kaynnista(sektori,glob_data);
			}
			
		}
			
	}
	

	if(kantotila == 1)
	{
		sijainti_x = kantaja->AnnaSijaintiX() + kantaja->AnnaLeveys(glob_data)/2-16;
		sijainti_y = kantaja->AnnaSijaintiY()-32;

		if(kantaja->OnkoTuhottu()) {kantotila = 0;}
	}
	if(kantotila == 0 || (kantotila != 1 && !x_nopeus)) Gravitaatio();

	if(fys_state)
	{	
		if(x_nopeus > 0) {x_nopeus -= 100; if(x_nopeus < 0) x_nopeus = 0;}
		if(x_nopeus < 0) {x_nopeus += 100; if(x_nopeus > 0) x_nopeus = 0;}

		if(x_nopeus == 0) kantotila = 0;
	}

	return true;
}

void Pelaaja::animoiObjekti(GLOBAALI *glob_data)
{
	Animaatio *os =
		glob_data->objektityypit.at(tyyppinumero)->animaatiot.at(animaatio);

	if(animaatio == 1 || animaatio == 0)
	{
		int laskuri = abs(x_nopeus);

		while(laskuri > 0)
		{
			animoi(os,&ruutu_counter,&anim_ruutu);
			laskuri -= 2000;
		}
	}
	else if(animaatio == 4 || animaatio == 5 || animaatio == 8)
	{
		int laskuri = abs(y_nopeus);

		while(laskuri > 0)
		{
			animoi(os,&ruutu_counter,&anim_ruutu);
			laskuri -= 2000;
		}

	}
	else if(animaatio == 6 || animaatio == 7)
	{

		int laskuri = 0;
		if(y_nopeus < 0)
		{
			laskuri = -y_nopeus;

		while(laskuri > 0)
		{
			animoi(os,&ruutu_counter,&anim_ruutu);
			laskuri -= 2000;
		}
		} else anim_ruutu = 0;

	}else animoi(os,&ruutu_counter,&anim_ruutu);

	
}

void Pelaaja::OtaVahinkoa(int maara, Pelisektori *sektori, GLOBAALI *glob_data)
{
	if(!damage) sektori->pelaajaanOsui(glob_data);;

	Peliobjekti::OtaVahinkoa(maara, sektori, glob_data);
}

bool Pelaaja::Piirra(int x_cam, int y_cam, GLOBAALI *glob_data)
{
	
	if(!damage || damage % 10 < 5)
	{

		Animaatio *os =
		glob_data->objektityypit.at(tyyppinumero)->animaatiot.at(animaatio);

		piirraAnimaatio(os,sijainti_x-x_cam,sijainti_y-y_cam,&ruutu_counter,&anim_ruutu,glob_data);		
	}
	
	

	

	return true;
}

void Pelaaja::piirraInventory(GLOBAALI *glob_data)
{
	for(int index = 0; index < inventory.size(); index++)
	{
		inventory.at(index)->PiirraS(200+50*index,20,glob_data);
	}
}

void Pelaaja::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	koynnos = 0;
	siivet = 0;
	vaihe = 0;
	kannettava = 0;
	suunta = 1;
	inventory.clear();
}

void Pelaaja::AsetaAlkuKoordinaatit(int ax, int yy)
{
	x_ = ax;
	y_ = yy;
}


bool Pelaaja::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kontrolli(glob_data,sektori,taso);

	if(sijainti_y > sektori->annaYcam()+480) Tuhottu(sektori, glob_data);
	if(!koynnos) Gravitaatio();

	Poiminta *logiikka = sektori->annaLogiikka();
	int indeksi = 0;
	Peliobjekti *ptr;

	if(!sektori->onkoLapi())
	while(ptr = logiikka->annaAlkio(&indeksi, &logiikka))
	{
		ptr->PelaajaInteraktio(this, glob_data, taso);
	}

	return true;
}


void Pelaaja::Gravitaatio()
{
	if(!fys_state)
	{
		if(!vedessa)
		{
			if(siivet) y_nopeus += 175;
			else
			{
				y_nopeus += gravitaatio;
				//y_nopeus -= abs(x_nopeus)/150;
				//if(y_nopeus > 0) y_nopeus -= (gravitaatio/7800)*y_nopeus;
			}
		}
		
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

	if(siivet)
	{
		if(y_nopeus > 3500) y_nopeus = 3500;
	}
	else
	{
		
		if(y_nopeus > putoamisnopeus) y_nopeus = putoamisnopeus;
	}
}


bool Pelaaja::Kontrolli(GLOBAALI *glob_data, Pelisektori *sektori, Pelitaso *taso)
{
	bool lapi = sektori->onkoLapi();

	int lev = AnnaLeveys(glob_data);
	int kork = AnnaKorkeus(glob_data);

	InputData *inputti = glob_data->gfx->annaInput();
		
	int *tileset_tiletyypit = glob_data->tilesetit.at(sektori->annaSetti())->kerrokset[0]->tyypit;

	glob_poiminta.Alusta();
	
	
	sektori->annaQuadtree()->poimiObjektit(sijainti_x -320,sijainti_y -240,sijainti_x + 320, sijainti_y +240,&glob_poiminta,glob_data,0);
	
	Poiminta *ptr = &glob_poiminta;
	int indeksi = 0;
	Peliobjekti *obj;


	

	int x1, y1, x2, y2;

	Tilemap *mappi = sektori->annaMappi(0);
	unsigned char *mappidata = mappi->AnnaData();
	int mapinleveys = mappi->AnnaX();

	

	
	bool arvo =
		tilealue(sijainti_x, sijainti_y, sijainti_x, sijainti_y, lev, kork,&x1,&y1,&x2,&y2,mappi);

	bool loytyy_koynnosta = 0;


	
	int koynnos_x, koynnos_y;

	for(int y = y1; y <= y2; y++)
	{
		for(int x = x1; x <= x2; x++)
		{

			unsigned char tyyppi = tiletyyppitaulukko[tileset_tiletyypit[mappidata[y*mapinleveys + x]]];

			if(tyyppi == KOYNNOS && !loytyy_koynnosta)
			{
				if(sijainti_x + 10 < (x+1)*32 -8 && sijainti_y < (y+1)*32 && sijainti_x + lev -10 > x*32 +8 && sijainti_y + kork > y*32)
				loytyy_koynnosta = 1;
				koynnos_x = x;
				koynnos_y = y;
				break;
			}
				
		}
	}

	bool tsekki = 0;
	if(loytyy_koynnosta && sijainti_y < koynnos_y*32-16)
	{
		for(y = y1; y < koynnos_y; y++)
		{
			if(tiletyyppitaulukko[tileset_tiletyypit[mappidata[y*mapinleveys + koynnos_x]]] != KOYNNOS) tsekki = 1;
		}
		
	}

	if(loytyy_koynnosta)
	{
		if(pohjassa(index_YLOS, glob_data) && !tsekki) koynnos = 1;
	} else koynnos = 0;

	

	bool hypynesto = 0;

	// käydään läpi mahdolliset objektit
	while(obj = ptr->annaAlkio(&indeksi,&ptr))
	{
		int num = obj->AnnaTaulukkoNum();

		if(num == Jousi_num)
		{

			int x = obj->AnnaSijaintiX();
			int lev = obj->AnnaLeveys(glob_data);
			int y = obj->AnnaSijaintiY();
			int kork = obj->AnnaKorkeus(glob_data);

			if(x < sijainti_x + AnnaLeveys(glob_data) && x + lev > sijainti_x
				&& sijainti_y + AnnaKorkeus(glob_data) == y)
			{
				hypynesto = 1;
			}
		}
		else if(num == Laatikko_num)
		{
			if((pohjassa(index_JUOKSU,glob_data)) && !kannettava)
			{
				if(((Laatikko*)obj)->Otalaatikko(this,glob_data))
				{
					kannettava = obj;
				}
			}
		}
		else if(num == Avain_num)
		{
			if(((Avain*)obj)->otaAvain(this, sektori, glob_data)) inventory.push_back(obj);
		}
		else if(num == Ovi2_num)
		{
			if(painettu(index_YLOS,glob_data))
			{
				((Ovi2*)obj)->Avaaovi(this,0,glob_data,taso);

				for(int index = 0; index < inventory.size(); index++)
				((Ovi2*)obj)->Avaaovi(this,inventory.at(index),glob_data,taso);
			}
		}
	}


	// jos kantaa jotain
	if(kannettava)
	{
		if(!pohjassa(index_JUOKSU,glob_data))
		{
			if(pohjassa(index_ALAS,glob_data)) ((Laatikko*)kannettava)->Paastairti(glob_data);
			else ((Laatikko*)kannettava)->Heita(glob_data);
			kannettava = 0;
		}
	}

	
	int kiih = 0;
	int x_kiihtyvyys = 0;
	int kiihdytysvoima = 0;
	int jarruvoima = 0;
	float kitkakerroin = (float)otaKitka(sektori, glob_data)/(float)255;

	
	int vauhtisuunta;
	if(x_nopeus == 0) vauhtisuunta = 0;
	else if(x_nopeus > 0) vauhtisuunta = 1;
	else vauhtisuunta = -1;

	int painosuunta = 0;
	if(!lapi && pohjassa(index_VASEN,glob_data)) painosuunta -= 1;
	if(!lapi && pohjassa(index_OIKEA,glob_data)) painosuunta += 1;
	if(painosuunta == 0) juoksulippu = 0;

	bool maassa = 0;
	if(fys_state) {maassa = 1; hyppy = 0;}
	
	bool juoksu = 0;
	if(pohjassa(index_JUOKSU,glob_data)) juoksu = 1;

	int jutska = 0;
	if(painosuunta == -vauhtisuunta) jutska = 1;
	jarruvoima = vauhtisuunta*jarrut[maassa+2*jutska];

	int kohdenopeus;

#define nopeus1 1000
#define nopeus2 3800
#define nopeus3 4100
	if(juoksu)
	{
		switch(vaihe)
		{
		case 0:
			kohdenopeus = painosuunta * nopeus1;
			kiih = 220;
			break;
		case 1:
			kohdenopeus = painosuunta * nopeus2;
			kiih = 270;
			if(y_nopeus < 0) kiih += (y_nopeus)*0.002;
			break;
		case 2:
			kohdenopeus = painosuunta * nopeus2;
			kiih = 230;
			break;
		case 3:
			kohdenopeus = painosuunta * nopeus3;
			kiih = 300;
			break;
		default: break;
		}
		
	}
	else {kohdenopeus = painosuunta * 2500; kiih = 220;}

	


	if((kohdenopeus*painosuunta <= x_nopeus *painosuunta) && !fys_state) jarruvoima = 0;
	else jarruvoima = 125*vauhtisuunta;


	if(x_nopeus*painosuunta < kohdenopeus*painosuunta) kiihdytysvoima = painosuunta*kiih;
	else kiihdytysvoima = 0;

	x_kiihtyvyys = kiihdytysvoima*sqrt(kitkakerroin) - jarruvoima*kitkakerroin;


	// koodi, joka kontrolloi vaiheen vaihtumista
	switch(vaihe)
	{
	case 0:
		if(!maassa) vaihe = 1;

		if(x_nopeus * vauhtisuunta >= nopeus1)
		{
			vaihe = 1;
		}
		break;

	case 1:
		if(x_nopeus * vauhtisuunta >= nopeus2)
		{
			vaihe = 2; juoksucounter = 0;
		}
		
		if(vauhtisuunta*x_nopeus < nopeus1 -100) vaihe = 0;
		break;

	case 2:

		if((painosuunta == vauhtisuunta) && maassa)
		{
			

			juoksucounter++;
		} else juoksucounter = 0;

		if(x_nopeus*vauhtisuunta < nopeus1 - 100) vaihe = 0;

		if(juoksucounter == 60) vaihe = 3;
		break;
	case 3:

		if(vauhtisuunta*x_nopeus < nopeus1 -100) vaihe = 0;

		juoksulippu = 1;

		break;
	default: break;
	}



	int dx;
	
	int alkunopeus = x_nopeus;
	x_nopeus += x_kiihtyvyys;

	int ksuunta = 0;
	if(x_kiihtyvyys > 0) ksuunta = 1; else ksuunta = -1;
	dx = sijainti_x - vanha_x;

	if(x_kiihtyvyys != 0)
	{
		if(alkunopeus*ksuunta <= kohdenopeus*ksuunta && kohdenopeus*ksuunta <= x_nopeus*ksuunta) x_nopeus = kohdenopeus;
	}

	bool check = 1;
	if(taman_paalla)
	{
		int dx2 = (taman_paalla)->AnnaSijaintiX() - (taman_paalla)->AnnaVanhaX();
		if(dx2 == dx) check = 0;
	}
	if(x_nopeus < 0 ) suunta = 0;
	else if(x_nopeus > 0 ) suunta = 1;











	if(!fys_state && siivet && !vedessa)
	{
		if(painettu(index_HYPPY,glob_data))
		{
			if(y_nopeus > -4000) y_nopeus = -5000;
			else
			{
				y_nopeus -= 2000;
				if(y_nopeus < -10000) y_nopeus = -10000;
			}
		}
	}

	if(siivet)
	{
		if(!suunta) animaatio = 4;
		else animaatio = 5;
	}
	else
	{
		if(!suunta)
		{
			if((x_nopeus != 0 || (dx != 0 && !taman_paalla) || (taman_paalla && check)) && fys_state) animaatio = 0; else animaatio = 2;
		}
		else
		{
			if((x_nopeus != 0 || (dx != 0 && !taman_paalla) || (taman_paalla && check)) && fys_state) animaatio = 1; else animaatio = 3;
		}
	}

	if(tallaus)
	{
		glob_data->gfx->soitaAani(glob_data->hyppyaani);
		tallaus = 0;
		
		if(!siivet)
		{
			ilmajuttu = 40;
			y_nopeus = -hyppy_nopeus;
			hyppy = 1;
		} else y_nopeus = -3000;

		if(!lapi && pohjassa(index_HYPPY,glob_data)) y_nopeus -= 1500;

		fys_state = 0;
		
		
	}
	else if(!lapi && painettu(index_HYPPY,glob_data) && (fys_state || koynnos) && !vedessa && !hypynesto)
	{
		koynnos = 0;
		if(!siivet)
		{
			x_nopeus = x_nopeus*0.85;

			y_nopeus = -(int)((double)hyppy_nopeus);
			hyppy = 1;
			ilmajuttu = 30;
			ilmajuttu += abs(x_nopeus)/500;

			glob_data->gfx->soitaAani(glob_data->hyppyaani);
		} else y_nopeus = -6000;

		y_nopeus -= (int)abs((float)x_nopeus/(float)8);
		
		fys_state = FYS_EI;
		taman_paalla = 0;
		//player.palkin_paalla = 0;
		
		
	}

	if(hyppy)
	{
		float jakaja = 1;
		if(x_nopeus != 0) jakaja = abs(x_nopeus)/2500;
		if(jakaja < 1) jakaja = 1;

		if(!lapi && pohjassa(index_HYPPY,glob_data) && !fys_state && ilmajuttu)
		{
			
			double koo = abs(x_nopeus)/500;
			if(koo > 0.2) koo = 0.2;
		

			if(y_nopeus < 0) y_nopeus -= (y_nopeus*0.001*kerroin)/jakaja;
			y_nopeus -= ilmajuttu*0.2 + bee;
			

			ilmajuttu--;
			
		}
		else
		{
			ilmajuttu -=4;
			if(y_nopeus < 0) y_nopeus += imaisu;
		}

		if(ilmajuttu < 0)
		{
			ilmajuttu = 0;
			hyppy = 0;
		}

		
	}

	if(vedessa)
	{
		if(!fys_state)
		{
			if(!suunta) animaatio = 6;
			else animaatio = 7;
		}
		hyppy = 0;

		if(uinticounter > 0) {uinticounter--;y_nopeus -= 600;}

	
		if(!lapi && painettu(index_HYPPY,glob_data))
		{
			fys_state = 0;
			if(vesivoimablokki == 0 && pinnalla)
			{
				vesivoimablokki = 1;
				y_nopeus = -8000;
				uinticounter = 0;
				glob_data->gfx->soitaAani(glob_data->hyppyaani);
			}
			else
			{
				glob_data->gfx->soitaAani(glob_data->uintiaani);

				uinticounter = 10;
	
				if(y_nopeus == 0) y_nopeus = -3000;
				else
				{
					y_nopeus -= 6000;
					
				}
			}

			
		}

	}

	if(koynnos)
	{
		animaatio = 8;
		x_nopeus = 0;
		y_nopeus = 0;

		if(pohjassa(index_YLOS,glob_data)) y_nopeus -= 1700;
		if(pohjassa(index_ALAS,glob_data)) y_nopeus += 1700;

		if(y_nopeus < 0 && tsekki) y_nopeus = 0;

		if(y_nopeus == 0)
		{
			if(pohjassa(index_VASEN,glob_data)) x_nopeus -= 1000;
			if(pohjassa(index_OIKEA,glob_data)) x_nopeus += 1000;
		}

		if(pohjassa(index_JUOKSU,glob_data)) {x_nopeus *= 1.44; y_nopeus *= 1.44;}
	}

	

	return true;
}

float palkki_slaidi(float t, float n)
{
	while(t > 1) t -= 1;

	t *= (4*(n-1) + 8);
	float a = (float)1/2;
	float x;

	if(t < n-1)
	{
		x = t;
	}
	else if(t < (n-1) + 2/a)
	{
		x = n-1 + 1*(t - (n-1)) - (float)0.5*a*(t - (n-1))*(t - (n-1));
	}
	else if(t < 3*(n-1) + 2/a)
	{
		x = n-1 - 1*(t-((n-1) + 2/a));
	}
	else if(t < 3*(n-1) + 4/a)
	{
		x = -(n-1) -1*(t - (3*(n-1) + 2/a)) + (float)0.5*a*(t - (3*(n-1) + 2/a))*(t - (3*(n-1) + 2/a));
	}
	else
	{
		x = -(n-1) + 1*(t - (3*(n-1) + 4/a));
	}

	return x/n;
}

int KiinteaPalkki::AnnaLeveys(GLOBAALI *glob_data)
{
	return (*((int*)data->at(0)))*32;
}

int KiinteaPalkki::AnnaKorkeus(GLOBAALI *glob_data)
{
	return (*((int*)data->at(1)))*32;
}
void KiinteaPalkki::Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori)
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

	sijainti_x = x_*32;
	sijainti_y = y_*32;
}

void Seilaaja::Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori)
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

	sijainti_x = x_*32+16 -AnnaLeveys(glob_data)/2;
	sijainti_y = y_*32+16 -AnnaKorkeus(glob_data)/2;
}

void Kala::Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori)
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

	sijainti_x = x_*32+16 -AnnaLeveys(glob_data)/2;
	sijainti_y = y_*32+16 -AnnaKorkeus(glob_data)/2;
}

void Seilaaja::Liikuta(GLOBAALI *glob_data)
{
}

bool Seilaaja::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	animaatio = 0;

	int r = *((int*)data->at(1));
	int sade = r*32;
	
	int jakso = *((int*)data->at(2))*100;
	float vaihe = ((float)*((int*)data->at(3)))/36;
	
	int vaakapysty = *((int*)data->at(0));
	
	if(vaakapysty == 0)
	{
		
		kohde_x = x_*32+16 -AnnaLeveys(glob_data)/2;
		kohde_y = y_*32+16 -AnnaKorkeus(glob_data)/2;


		kohde_x += (int)((float)sade*palkki_slaidi(
			
			(((float)(aika%jakso)/(float)jakso) + vaihe), (float)r
			
			));		
	}
	else
	{
		kohde_x = x_*32+16 -AnnaLeveys(glob_data)/2;
		kohde_y = y_*32+16 -AnnaKorkeus(glob_data)/2;

		kohde_y += (int)((float)sade*palkki_slaidi(
			
			(((float)(aika%jakso)/(float)jakso) + vaihe), (float)r
			
			));	
	}

	aika++;
	return true;
}


bool KiinteaPalkki::Piirra(int x_cam, int y_cam, GLOBAALI *glob_data)
{
	int leveys = *((int*)data->at(0));
	int korkeus = *((int*)data->at(1));
	int x, y;

	Sprite *kuva = glob_data->objektityypit.at(tyyppinumero)->spritet.at(0);

	Rect laatikko;
	laatikko.width = 32;
	laatikko.height = 32;
	laatikko.x = 1;
	laatikko.y = 300;

	glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam, sijainti_y-y_cam, &laatikko, 0, 0);

	laatikko.x = 34;

	for(x = 0; x < leveys-2; x++)
	{
		glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam+(1+x)*32, sijainti_y-y_cam, &laatikko, 0, 0);
	}

	laatikko.x = 67;

	glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam+(leveys-1)*32, sijainti_y-y_cam, &laatikko, 0, 0);



	laatikko.x = 1;
	laatikko.y = 366;


	glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam, sijainti_y-y_cam+(korkeus-1)*32, &laatikko, 0, 0);

	laatikko.x = 34;

	for(x = 0; x < leveys-2; x++)
	{
		glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam+(1+x)*32, sijainti_y-y_cam+(korkeus-1)*32, &laatikko, 0, 0);
	}

	laatikko.x = 67;

	glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam+(leveys-1)*32, sijainti_y-y_cam+(korkeus-1)*32, &laatikko, 0, 0);

	laatikko.y = 333;

	for(y = 0; y < korkeus-2; y++)
	{
		laatikko.x = 1;
		glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam, sijainti_y-y_cam+(1+y)*32, &laatikko, 0, 0);

		for(x = 0; x < leveys-2; x++)
		{
			laatikko.x = 34;
			glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam+(1+x)*32, sijainti_y-y_cam+(1+y)*32, &laatikko, 0, 0);
		}

		laatikko.x = 67;
		glob_data->gfx->PiirraSprite(kuva, sijainti_x-x_cam+(leveys-1)*32, sijainti_y-y_cam+(1+y)*32, &laatikko, 0, 0);
	}

	return true;
}

void KiinteaPalkki::Liikuta(GLOBAALI *glob_data)
{
}

bool KiinteaPalkki::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
		
	int r = *((int*)data->at(3));
	int sade = r*32;
	
	int jakso = *((int*)data->at(4))*100;
	float vaihe = ((float)*((int*)data->at(5)))/36;
	
	int vaakapysty = *((int*)data->at(2));
	
	kohde_x = x_*32;
	kohde_y = y_*32;

	if(vaakapysty == 0)
	{
		kohde_x += (int)((float)sade*palkki_slaidi(
			
			(((float)(aika%jakso)/(float)jakso) + vaihe), (float)r
			
			));		
	}
	else
	{
		kohde_y += (int)((float)sade*palkki_slaidi(
			
			(((float)(aika%jakso)/(float)jakso) + vaihe), (float)r
			
			));	
	}

	aika++;
	return true;
}


void Amppari::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	if(taulukkonumero != Pelaaja_num) 
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->AnnaSijaintiX() < sijainti_x) suunta = 0; else suunta = 1;
	}

	vaihe = 0;
}

bool Amppari::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	if(suunta)
	{
		x_nopeus = 2000;
		animaatio = 1;
		
	}
	else
	{
		x_nopeus = -2000;
		animaatio = 0;
	}

	if(vaihe < 15) y_nopeus += 150;
	else if(vaihe < 45) y_nopeus -= 150;
	else y_nopeus += 150;
	

	
	vaihe++;
	if(vaihe == 60) vaihe = 0;


	return true;
}

bool Lentoaija::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	int r = *((int*)data->at(1));
	int sade = r*32;
	
	int jakso = *((int*)data->at(2))*100;
	float vaihe = ((float)*((int*)data->at(3)))/36;
	
	int vaakapysty = *((int*)data->at(0));
	
	if(vaakapysty == 0)
	{
		
		kohde_x = x_*32;
		kohde_y = y_*32;


		kohde_x += (int)((float)sade*palkki_slaidi(
			
			(((float)(aika%jakso)/(float)jakso) + vaihe), (float)r
			
			));		
	}
	else
	{
		kohde_x = x_*32;
		kohde_y = y_*32;

		kohde_y += (int)((float)sade*palkki_slaidi(
			
			(((float)(aika%jakso)/(float)jakso) + vaihe), (float)r
			
			));	
	}

	aika++;
	

	return true;
}

void Lentoaija::Liikuta(GLOBAALI *glob_data)
{
}

bool Pomppija::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	if(suunta)
	{
		x_nopeus = 2000;
	}
	else
	{
		x_nopeus = -2000;
	}

	if(fys_state != 0){fys_state = 0;y_nopeus = -7500;}
	Gravitaatio();
	return true;
}

bool Jamppa::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	
	Kaannos();
	

	if(suunta)
	{
		x_nopeus = 700;
		animaatio = 1;
	}
	else
	{
		x_nopeus = -700;
		animaatio = 0;
	}

	

	Gravitaatio();

	return true;
}

bool Aija::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);

	if(suunta)
	{
		x_nopeus = 800;
		animaatio = 1;
	}
	else
	{
		x_nopeus = -800;
		animaatio = 0;
	}

	Gravitaatio();

	return true;
}

bool Piikkityyppi::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);

	if(suunta)
	{
		x_nopeus = 2000;
		animaatio = 1;
	}
	else
	{
		x_nopeus = -2000;
		animaatio = 0;
	}

	Gravitaatio();

	return true;
}


void Siivet::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	Pelisektori *sektori = taso->AnnaSektorit()->at(taso->AnnaSektori());
	if(objektitLeikkaavat(pelaaja,glob_data))
	{
		pelaaja->asetaSiivet(1);
		Tuhottu(sektori, glob_data);
	}
}


void Kolikko::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	Pelisektori *sektori = taso->AnnaSektorit()->at(taso->AnnaSektori());
	if(objektitLeikkaavat(pelaaja,glob_data))
	{
		glob_data->gfx->soitaAani(glob_data->kolikkoaani);
		taso->lisaaKolikko();
		
		Tuhottu(sektori, glob_data);
	}
}


void Ovi::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	bool lapi = taso->AnnaSektorit()->at(taso->AnnaSektori())->onkoLapi();

	if(objektitLeikkaavat(pelaaja,glob_data))
	{
		int pelaaja_fysState = pelaaja->annaFysState();
		if(!lapi && painettu(index_YLOS,glob_data)
			&& pelaaja_fysState
			&& pelaaja->AnnaSijaintiY() + pelaaja->AnnaKorkeus(glob_data)
			== sijainti_y + AnnaKorkeus(glob_data))
		{
			int sektori = *((int*)data->at(0));
			int ax = *((int*)data->at(1));
			int yy = *((int*)data->at(2));

			taso->VaihdaSektori(sektori,ax,yy,glob_data,pelaaja->AnnaEnergia());
		}
	}
}

void Maali::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	if(objektitLeikkaavat(pelaaja,glob_data))
	{
		taso->AnnaSektorit()->at(taso->AnnaSektori())->lapaisy(*((int*)data->at(1)),glob_data);	
	}
}

void KiinteaPalkki::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	aika = 0;
}

void Seilaaja::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	aika = 0;
}

void Lentoaija::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	aika = 0;
}

void Piikkipallo::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	aika = 0;
}

void Sylkija::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	if(taulukkonumero != Pelaaja_num) 
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->AnnaSijaintiX() < sijainti_x) suunta = 0; else suunta = 1;
	}
	counter = 0;

	hyppytila = 0;
}



bool Lumipallo::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	if(g) Gravitaatio();

	int x_cam = sektori->annaXcam();
	int y_cam = sektori->annaYcam();

	if(sijainti_x < x_cam - 200 || sijainti_x > x_cam + 840
		|| sijainti_y < y_cam -200 || sijainti_y > y_cam + 680)
	{
		Pelisektori *os = taso->AnnaSektorit()->at(taso->AnnaSektori());
		Tuhottu(sektori, glob_data);
	}

	return true;
}


bool Sylkija::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);

	int pelaaja_x;
	Tulipallo *os;

	switch(hyppytila)
	{
	case 0:
		if(suunta)
		{
			x_nopeus = 2000;
			animaatio = 1;
		}
		else
		{
			x_nopeus = -2000;
			animaatio = 0;
		}
		

		pelaaja_x = sektori->annaPelaaja()->AnnaSijaintiX();

		if(counter < 50) counter++;

		if(((pelaaja_x > sijainti_x - 500 && pelaaja_x < sijainti_x && !suunta)
			|| (pelaaja_x < sijainti_x + 500 && pelaaja_x > sijainti_x && suunta))
			&& counter == 50)
		{
			hyppytila = 1;
			fys_state = 0;
			y_nopeus = -11000;
			
			int nopeus_x;
			if(suunta) nopeus_x = 5000;
			else nopeus_x = -5000;

			os =  (Tulipallo*)luo_objekti(Tulipallo_num,glob_data,sektori);
			os->Alusta();
			os->asetag(0);
			os->Aseta(glob_data,sijainti_x, sijainti_y);
			os->AsetaNopeus(nopeus_x,0);
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
		}

		break;

	case 1:

		if(suunta) animaatio = 3;
		else animaatio = 2;

		#define M_PI 3.14159265358979323846264338327
		if(fys_state)
		{
			counter = 0;
			hyppytila = 0;
		}
		break;

	default:
		break;
	}
	

	Gravitaatio();

	return true;
}

bool Sydan::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	animaatio = 0;
	return true;
}

void Sydan::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	Pelisektori *sektori = taso->AnnaSektorit()->at(taso->AnnaSektori());

	if(objektitLeikkaavat(pelaaja,glob_data))
	{
		pelaaja->saadaEnergia(pelaaja->AnnaEnergia()+1);
		Pelisektori *os = taso->AnnaSektorit()->at(taso->AnnaSektori());
		Tuhottu(sektori, glob_data);
	}
}

bool Piikkipallo::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	int r = *((int*)data->at(1));
	int sade = r*32;

	int jakso = *((int*)data->at(2))*100;
	float vaihe = ((float)*((int*)data->at(3)))/36;
	
	int myotavasta = *((int*)data->at(0));

	int kerroin = -1;
	if(myotavasta == 0) kerroin *= -1;

	kohde_x = x_*32-(AnnaLeveys(glob_data)-32)/2;
	kohde_y = y_*32-(AnnaKorkeus(glob_data)-32)/2;;

	kohde_x += (int)((float)sade*
	cos((((float)(aika%jakso)/(float)jakso)*kerroin + vaihe)*2*3.141592654)
		);

	

	kohde_y += (int)((float)sade*
	sin((((float)(aika%jakso)/(float)jakso)*kerroin + vaihe)*2*3.141592654)
		);
	

	aika++;
	return true;
}

void Piikkipallo::Liikuta(GLOBAALI *glob_data)
{
}

bool Piikkipallo::Piirra(int x_cam, int y_cam, GLOBAALI *glob_data)
{
	int leveys = *((int*)data->at(0));
	int korkeus = *((int*)data->at(1));

	Sprite *kuva = glob_data->objektityypit.at(tyyppinumero)->spritet.at(0);

	Rect laatikko;

	int r = *((int*)data->at(1));

	int myotavasta = *((int*)data->at(0));
	int kerroin = -1;
	if(myotavasta == 0) kerroin *= -1;

	int jakso = *((int*)data->at(2))*100;
	float vaihe = ((float)*((int*)data->at(3)))/36;

	float ax = cos((((float)(aika%jakso)/(float)jakso)*kerroin + vaihe)*2*3.141592654);
	float yy = sin((((float)(aika%jakso)/(float)jakso)*kerroin + vaihe)*2*3.141592654);

	laatikko.width = 16;
	laatikko.height = 16;
	laatikko.x = 318;
	laatikko.y = 334;

	for(int index = 0; index < (r*2); index++)
	{
		glob_data->gfx->PiirraSprite(kuva, x_*32+8+index*16*ax-x_cam,
			y_*32+8+index*16*yy-y_cam, &laatikko, 1, 0);
	}

	Animaatio *os =
		glob_data->objektityypit.at(tyyppinumero)->animaatiot.at(animaatio);

		piirraAnimaatio(os,sijainti_x-x_cam,sijainti_y-y_cam,&ruutu_counter,&anim_ruutu,glob_data);



	return true;
}

void Matkaaja::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	indeksi = 0;
	x_nopeus = 0;
	y_nopeus = 0;
	k_x = sijainti_x;
	k_y = sijainti_y;
	aika = 0;
	naks = 0;
}


void Matkaaja::Liikuta(GLOBAALI *glob_data)
{
	if(x_nopeus == 0) x_liike = 0;
	if(y_nopeus == 0) y_liike = 0;

	x_liike += x_nopeus;
		
	int jako_x = x_liike/1000;
	if(!naks) kohde_x = sijainti_x;

	kohde_x += jako_x;
	x_liike -= jako_x*1000;

	y_liike += y_nopeus;
	
	int jako_y = y_liike/1000;
	if(!naks) kohde_y = sijainti_y;
	
	kohde_y += jako_y;
	y_liike -= jako_y*1000;

	kohde_x;
	kohde_y;

	if(naks) naks = 0;
}


bool Matkaaja::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	std::vector < std::vector <void *> *> *listaosoitin =
		(std::vector < std::vector <void *> *> *)data->at(1);

	int nopeus = *((int*)data->at(0));

	if(listaosoitin->size() == 0) return true;

	if(aika <= 0)
	{
		naks = 1;
		kohde_x = k_x;
		kohde_y = k_y;

		int delta_x = *((int*)listaosoitin->at(indeksi)->at(0));
		int delta_y = *((int*)listaosoitin->at(indeksi)->at(1));

		k_x += delta_x*32;
		k_y += delta_y*32;
		
		float pituus = sqrt(delta_x*delta_x + delta_y*delta_y);

		if(pituus > 0)
		{
			x_nopeus = (int)((float)delta_x*1000*(float)nopeus/pituus);
			y_nopeus = (int)((float)delta_y*1000*(float)nopeus/pituus);
		}
		else
		{
			x_nopeus = 0;
			y_nopeus = 0;
		}

		aika = (int)(pituus/(float)nopeus*(float)32);

		indeksi++;
		if(indeksi == listaosoitin->size()) indeksi = 0;
	}

	aika--;
	

	return true;
}


bool Pomppivapiikkipallo::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	animaatio = 0;
	Kaannos();
	if(suunta)
	{
		x_nopeus = 2500;
	}
	else
	{
		x_nopeus = -2500;
	}

	if(fys_state != 0){fys_state = 0;y_nopeus = -7000;}
	Gravitaatio();

	return true;
}

bool Bossi1::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();

	int jutska = 1000 + (-energia+5)*400;

	Pelaaja *pelaaja = (Pelaaja*)sektori->annaPelaaja();
	switch(moodi)
	{
	case 0:
		if(suunta)
		{
			x_nopeus = jutska;
			animaatio = 1;
		}
		else
		{
			x_nopeus = -jutska;
			animaatio = 0;
		}

		if(suunta)
		{
			if(fys_state && pelaaja->AnnaSijaintiX() > sijainti_x && pelaaja->AnnaSijaintiX() < sijainti_x +AnnaLeveys(glob_data)/2 + 200)
			{
				moodi = 1;
				counter = 5;
			}

			if(pelaaja->AnnaSijaintiX() < sijainti_x && pelaaja->AnnaSijaintiX() > sijainti_x -100)
			{moodi = 2; counter = 60;}
		}
		else
		{
			if(fys_state && pelaaja->AnnaSijaintiX() < sijainti_x && pelaaja->AnnaSijaintiX() > sijainti_x +AnnaLeveys(glob_data)/2 - 200)
			{					
				moodi = 1;
				counter = 5;
			}

			if(pelaaja->AnnaSijaintiX() > sijainti_x && pelaaja->AnnaSijaintiX() < sijainti_x + AnnaLeveys(glob_data)+100)
			{moodi = 2; counter = 60;}
		}
		break;


	case 1:
		
		if(suunta)
		{
			x_nopeus = 2*jutska;
			animaatio = 1;
		}
		else
		{
			x_nopeus = -2*jutska;
			animaatio = 0;
		}

		if(fys_state)
		{
			if(counter > 0)
			{
				y_nopeus = -8000;
				fys_state = 0;
				counter--;
			}
			else {moodi = 2; counter = 60;}
		}

		
		break;

	case 2:
		if(suunta)
		{
			x_nopeus = 3*jutska;
			animaatio = 1;
		}
		else
		{
			x_nopeus = -3*jutska;
			animaatio = 0;
		}

		counter--;
		if(counter == 0) moodi = 0;
		break;
	}

	
	Gravitaatio();
	return true;
}

void Bossi1::Alusta()
{
	damage = 0;
	energia = 5;
}

void Bossi1::Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data)
{
	energia = 0;
	tuhottu = 1;
	sektori->lapaisy(0,glob_data);
}

void Bossi1::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	if(taulukkonumero != Pelaaja_num) 
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->AnnaSijaintiX() < sijainti_x) suunta = 0; else suunta = 1;
	}

	counter = 0;
	moodi = 0;
}


void Bossi3::Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data)
{
	energia = 0;
	tuhottu = 1;
	sektori->lapaisy(0,glob_data);
}

void Bossi3::Alusta()
{
	damage = 0;
	energia = 5;
};

bool Bossi3::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{

	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);
	Kaannos();

	switch(moodi)
	{
	case 0:
		
		if(!suunta)
		{
			x_nopeus = -4000;
			animaatio = 0;
		}
		else
		{
			x_nopeus = 4000;
			animaatio = 1;
		}

		counter++;

		if(counter == 200) {counter = 0; moodi = 1;}
		break;

	case 1:
		x_nopeus = 0;

		counter++;
		if(counter % 50 == 0)
		{
			Tulipallo *os;
			os = (Tulipallo*)luo_objekti(Tulipallo_num,glob_data,sektori);
			os->Alusta();
			os->asetag(0);
			os->Aseta(glob_data,sijainti_x,sijainti_y+10);
			
			int delta_x = sektori->annaPelaaja()->AnnaSijaintiX() -sijainti_x;
			int delta_y = sektori->annaPelaaja()->AnnaSijaintiY() -sijainti_y;
			
			if(delta_x == 0)
			{
				if(delta_y <= 0) os->AsetaNopeus(0,-5000);
				else os->AsetaNopeus(0,5000);
			}
			else
			{
				double kulma = atan((double)delta_y/(double)delta_x);
				double x_n = cos(kulma)*5000;
				double y_n = sin(kulma)*5000;
			
				if(delta_x < 0)
				{
					x_n *= -1;
					y_n *= -1;
				}
				
				os->AsetaNopeus(x_n,y_n);
			}
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);
		}

		
		if(counter == 400) {counter = 0; moodi = 0;}
		break;

	default: break;
	}

	Gravitaatio();

	return true;
}

void Bossi3::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Peliobjekti::Kaynnista(sektori,glob_data);
	
	counter = 0;
	moodi = 0;
}




void Bossi4::Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data)
{
	energia = 0;
	tuhottu = 1;
	sektori->lapaisy(0,glob_data);
}

void Bossi4::Alusta()
{
	damage = 0;
	energia = 5;
};

bool Bossi4::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{

	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);
	Kaannos();

	switch(moodi)
	{
	case 0:
		
		if(!suunta)
		{
			x_nopeus = -4000;
			animaatio = 0;
		}
		else
		{
			x_nopeus = 4000;
			animaatio = 1;
		}

		counter++;

		if(counter == 200) {counter = 0; moodi = 1;}
		break;

	case 1:
		x_nopeus = 0;

		counter++;
		if(counter % 50 == 0)
		{
			Tulipallo3 *os;
			os = (Tulipallo3*)luo_objekti(Tulipallo3_num,glob_data,sektori);
			os->Alusta();
			
			os->Aseta(glob_data,sijainti_x,sijainti_y+10);
			
			int delta_x = sektori->annaPelaaja()->AnnaSijaintiX() -sijainti_x;
			
			if(delta_x <= 0)
			{
				os->AsetaNopeus(-5000,1000);
				os->asetaSuunta(0);
				
			}
			else{ os->AsetaNopeus(5000,1000); os->asetaSuunta(1);}
			
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);
		}

		
		if(counter == 100) {counter = 0; moodi = 0;}
		break;

	default: break;
	}

	Gravitaatio();

	return true;
}

void Bossi4::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Peliobjekti::Kaynnista(sektori,glob_data);
	
	counter = 0;
	moodi = 0;
}



void Bossi2::Tuhottu(Pelisektori *sektori, GLOBAALI *glob_data)
{
	energia = 0;
	tuhottu = 1;
	sektori->lapaisy(0,glob_data);
}

void Bossi2::Alusta()
{
	damage = 0;
	energia = 5;
};

void Bossi2::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Peliobjekti::Kaynnista(sektori,glob_data);

	counter = 0;
	moodi = 0;
}

bool Bossi2::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
		
	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);
	Kaannos();

	switch(moodi)
	{
	case 0:
		
		if(!suunta)
		{
			x_nopeus = -4000;
			animaatio = 0;
		}
		else
		{
			x_nopeus = 4000;
			animaatio = 1;
		}

		counter++;
		if(counter == 300) {moodi = 1; counter = 0;}
		break;
	case 1:
		x_nopeus = 0;
		counter++;
		if(counter % 100 == 0)
		{
			Tulipallo2 *os;
			os = (Tulipallo2*)luo_objekti(Tulipallo2_num,glob_data,sektori);
			os->Alusta();
			
			os->Aseta(glob_data,sijainti_x,sijainti_y+10);
						
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);
			//os->asetaSuunta(suunta);
		}
		if(counter == 300) {moodi = 0; counter = 0;}
	}
	

	Gravitaatio();

	return true;
}

bool Tulipallo2::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	if(suunta)
	{
		x_nopeus = 2500;
	}
	else
	{
		x_nopeus = -2500;
	}

	if(fys_state != 0){fys_state = 0;y_nopeus = -7500;}
	Gravitaatio();

	aika++;
	if(aika == 500) Tuhottu(sektori, glob_data);

	return true;
}

void Tulipallo2::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Peliobjekti::Kaynnista(sektori, glob_data);
	aika = 0;
}



void Kala::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);

	if(taulukkonumero != Pelaaja_num) 
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->AnnaSijaintiX() < sijainti_x) suunta = 0; else suunta = 1;
	}

	counter = 0;
}

bool Kala::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	if(suunta)
	{
		animaatio = 1;
		x_nopeus = 2000;
	}
	else
	{
		animaatio = 0;
		x_nopeus = -2000;
	}

	//if(y_nopeus > 300) y_nopeus -= 600;

	counter++;

	y_nopeus = sin(2*3.14*(double)counter/36)*1000;

	return true;
}

void Lokki::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	if(taulukkonumero != Pelaaja_num) 
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->AnnaSijaintiX() < sijainti_x) suunta = 0; else suunta = 1;
	}
	moodi = 0;


}

bool Lokki::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Pelaaja *pel = (Pelaaja*)sektori->annaPelaaja();
	
	int delta_x, delta_y, pel_x, pel_y;
	int pituus;

	pel_x = pel->AnnaSijaintiX();
	pel_y = pel->AnnaSijaintiY();
	switch(moodi)
	{
	case 0:
		if(suunta)
		{
			animaatio = 1;
			x_nopeus = 3000;
		}
		else
		{
			animaatio = 0;
			x_nopeus = -3000;
		}

		
		delta_x = pel_x-sijainti_x;
		delta_y = pel_y-sijainti_y;
		
		if(delta_x < 100 && delta_x > -100 && delta_y < 200 && delta_y >= -200)
		{
			if(sijainti_x < pel_x) puoli = 1;
			else puoli = 0;
			moodi = 1;
			counter = 0;
		}
		
		break;
	case 1:

		if(puoli == 0)
		{
			delta_x = pel_x -50 -sijainti_x;
			delta_y = (pel_y-100) -sijainti_y;
			pituus = sqrt(delta_x*delta_x + delta_y*delta_y);
			if(pituus > 1)
			{
				x_nopeus = (delta_x*(3000+pituus*3))/pituus;
				y_nopeus = (delta_y*(3000+pituus*3))/pituus;
			} else {puoli = 1; counter++;}

		}
		else
		{
			delta_x = pel_x +50 -sijainti_x;
			delta_y = (pel_y-100) -sijainti_y;
			pituus = sqrt(delta_x*delta_x + delta_y*delta_y);
			if(pituus > 1)
			{
				x_nopeus = (delta_x*(3000+pituus*3))/pituus;
				y_nopeus = (delta_y*(3000+pituus*3))/pituus;
			} else {puoli = 0; counter++;}

		}

		if(counter == 5)
		{
			moodi = 2;
			counter = 0;
			x_nopeus = 0;
			y_nopeus = 0;
		}

	

		if(x_nopeus > 0) animaatio = 1; else animaatio = 0;
		break;
	case 2:
		if(pel_x > sijainti_x) animaatio = 1; else animaatio = 0;

		counter++;
		if(counter == 60)
		{
			delta_x = pel_x -sijainti_x;
			delta_y = pel_y -sijainti_y;
			pituus = sqrt(delta_x*delta_x + delta_y*delta_y);

			if(pituus > 0)
			{
				x_nopeus = (delta_x*(10000))/pituus;
				y_nopeus = (delta_y*(10000))/pituus;
			}
			else
			{
				x_nopeus = 0;
				y_nopeus = 0;
			}

		}
		break;
	default: break;
	}


	return true;
}

void Paikallaanhyppija::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	aika = 0;
}

bool Paikallaanhyppija::Logiikka(Pelisektori *sektori,  GLOBAALI *glob_data, Pelitaso *taso)
{
	int pel_x = sektori->annaPelaaja()->AnnaSijaintiX();
	if(pel_x < sijainti_x) animaatio = 0;
	else animaatio = 1;
	int jakso = *((int*)data->at(0));
	int vaihe = *((int*)data->at(1));

	if(aika == vaihe)
	{
		

		if(fys_state != 0){fys_state = 0;y_nopeus = -12000;}
	}

	aika++;

	if(aika == jakso*10)
	{
		aika = 0;
	}

	Gravitaatio();
	return true;
}

void Seuraavahyppija::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	counter = 0;
}

bool Seuraaja::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	int x = sektori->annaPelaaja()->AnnaSijaintiX();

	if(x < sijainti_x)
	{
		animaatio = 0;
		x_nopeus -= 100;
	}
	else
	{
		animaatio = 1;
		x_nopeus += 100;
	}

	if(x_nopeus < -3000) x_nopeus = -3000;
	if(x_nopeus > 3000) x_nopeus = 3000;


	if(oikea_torm)
	{
		x_nopeus += 1500;
		oikea_torm = 0;
	}
	if(vasen_torm)
	{
		x_nopeus -= 1500;
		vasen_torm = 0;
	}

	Gravitaatio();

	return true;
}

bool Seuraavahyppija::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	int x = sektori->annaPelaaja()->AnnaSijaintiX();
	
	if(fys_state)
	{
		x_nopeus = 0;
		if(x < sijainti_x) animaatio = 0;
		else animaatio = 1;
	} else
	{
		if(suunta == 0) x_nopeus = -3000; else x_nopeus = 3000;
	}



	if(counter == 0)
	{
		if(fys_state)
		{
			if(x < sijainti_x) suunta = 0;
			else suunta = 1;
			fys_state = 0;
			y_nopeus = -5000;
			counter = 70;
		}
	} else counter--;

	

	Gravitaatio();
	return true;
}

bool Kala2::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	fys_state = 0;
	int ax = sektori->annaPelaaja()->AnnaSijaintiX();
	int yy = sektori->annaPelaaja()->AnnaSijaintiY();

	int delta_x = ax-sijainti_x;
	int delta_y = yy-sijainti_y;

	int etaisyys = sqrt(delta_x*delta_x + delta_y*delta_y);

		
	if(etaisyys != 0)
	{
		x_nopeus += (delta_x*100)/etaisyys;
		y_nopeus += (delta_y*100)/etaisyys;
	}

	int pituus = sqrt(x_nopeus*x_nopeus + y_nopeus*y_nopeus);
	if(pituus > 2500)
	{
		x_nopeus = (x_nopeus*2500)/pituus;
		y_nopeus = (y_nopeus*2500)/pituus;
	}

	int x_cam = sektori->annaXcam();
	int y_cam = sektori->annaYcam();

	if(sijainti_x < x_cam || sijainti_x > x_cam + 640 || sijainti_y < y_cam || sijainti_y > y_cam + 480)
	{
		x_nopeus = 0;
		y_nopeus = 0;
	}

	if(x_nopeus < 0) animaatio = 0;
	else animaatio = 1;

	if(pinnalla && y_nopeus < 0) y_nopeus = 0;
	return true;
}

void Lumipallonheittaja::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	counter = 0;
}

bool Lumipallonheittaja::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	int ax = sektori->annaPelaaja()->AnnaSijaintiX();
	int yy = sektori->annaPelaaja()->AnnaSijaintiY();
	
	int delta_x = ax-sijainti_x;
	int delta_y = yy-sijainti_y;

	if(delta_x < 0) suunta = 0;
	else suunta = 1;
	

	if((counter > 50 && counter < 75) || (counter > 100 && counter < 125)
		|| (counter > 150 && counter < 175))
	{
		if(suunta == 0) animaatio = 2;
		else animaatio = 3;
	}
	else if(suunta == 0) animaatio = 0;
	else animaatio = 1;

	counter++;

	Lumipallo *os;

	int xcam = sektori->annaXcam();

	if(sijainti_x > xcam && sijainti_x < xcam + 640)
	if(counter == 50 || counter == 100 || counter == 150)
	{
		int x_v;
		if(delta_x < 0) x_v = -3500;
		else x_v = 3500;

		int y_v = 0;
		
		

			os =  (Lumipallo*)luo_objekti(Lumipallo_num,glob_data,sektori);
			os->Alusta();
			os->asetag(0);
			os->Aseta(glob_data,sijainti_x, sijainti_y+15);
			if(delta_x < 0) os->AsetaNopeus(x_v,y_v);
			else os->AsetaNopeus(x_v,y_v);
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);
	}

	if(counter == 200) counter = 0;

	Gravitaatio();

	return true;
}




bool Lumipallonheittaja2::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	int ax = sektori->annaPelaaja()->AnnaSijaintiX();
	int yy = sektori->annaPelaaja()->AnnaSijaintiY();
	
	int delta_x = ax-sijainti_x;
	int delta_y = yy-sijainti_y;

	if(delta_x < 0) suunta = 0;
	else suunta = 1;
	
	animaatio = 0;

	counter++;

	Lumipallo *os;

	if(counter == 50 || counter == 100 || counter == 150)
	{
		int x_v;
		

		x_v = delta_x*20;
		int y_v = 20*delta_y;

		int aika = 1000/20;
		int summa = 0;
		for(int index = 0; index < aika; index++)
		{
			summa += 500*index;
		}

		int korjaus;
		if(aika == 0) korjaus = 0;
		else korjaus = summa/aika;

		y_v -= korjaus;
				
		

		os =  (Lumipallo*)luo_objekti(Lumipallo_num,glob_data,sektori);
		os->Alusta();
		os->asetag(1);
		os->Aseta(glob_data,sijainti_x, sijainti_y+15);

		if(delta_x < 0) os->AsetaNopeus(x_v,y_v);
		else os->AsetaNopeus(x_v,y_v);
			
		os->EiTuhottu();
		sektori->lisaaTuhoutuviin(os);
		os->Kaynnista(sektori,glob_data);
	}

	if(counter == 200) counter = 0;

	Gravitaatio();

	return true;
}



void Palanen::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	liike = 1;
}

bool Palanen::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	if(vasen_torm || oikea_torm || yla_torm || ala_torm)
	{
		x_nopeus = 0;
		y_nopeus = 0;
		liike = 0;
	}
	if(liike) Gravitaatio();

	int x_cam = sektori->annaXcam();
	int y_cam = sektori->annaYcam();

	if(sijainti_x < x_cam - 200 || sijainti_x > x_cam + 840
		|| sijainti_y < y_cam -200 || sijainti_y > y_cam + 680)
	{
		Tuhottu(sektori, glob_data);
	}

	return true;
}

void Tulipallotyyppi::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	if(taulukkonumero != Pelaaja_num) 
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->AnnaSijaintiX() < sijainti_x) suunta = 0; else suunta = 1;
	}

	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);
	counter = 0;
}

bool Tulipallotyyppi::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);

	if(suunta)
	{
		x_nopeus = 500;
		animaatio = 1;
	}
	else
	{
		x_nopeus = -500;
		animaatio = 0;
	}

	if(counter == 0)
	{
		if(sijainti_x > sektori->annaXcam() && sijainti_y > sektori->annaYcam()
			&& sijainti_x < sektori->annaXcam() + 640 && sijainti_y < sektori->annaYcam() + 480)
		{
			counter = 150;
			Tulipallo *os;
			os = (Tulipallo*)luo_objekti(Tulipallo_num,glob_data,sektori);
			os->Alusta();
			os->asetag(0);
			os->Aseta(glob_data,sijainti_x,sijainti_y+10);
			int x_n;
			if(suunta) x_n = 2650; else x_n = -2650;
			os->AsetaNopeus(x_n,0);
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);

		}
	} else counter--;

	Gravitaatio();

	return true;
}

bool Avain::otaAvain(Peliobjekti *objekti, Pelisektori *sektori, GLOBAALI *glob_data)
{



	if(objekti->AnnaSijaintiX() < sijainti_x + AnnaLeveys(glob_data) && objekti->AnnaSijaintiX() + objekti->AnnaLeveys(glob_data) > sijainti_x
		&& objekti->AnnaSijaintiY() < sijainti_y + AnnaKorkeus(glob_data) && objekti->AnnaSijaintiY() + objekti->AnnaKorkeus(glob_data) > sijainti_y)
	{
		Tuhottu(sektori, glob_data);

		return true;
	}
	else return false;

}

bool Avain::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Gravitaatio();

	return true;
}

void Ovi2::Avaaovi(Peliobjekti *avaaja, Peliobjekti *avain, GLOBAALI *glob_data, Pelitaso *taso)
{
	int x = avaaja->AnnaSijaintiX();
	int lev = avaaja->AnnaLeveys(glob_data);
	int y = avaaja->AnnaSijaintiY();
	int kork = avaaja->AnnaKorkeus(glob_data);

	if(x < sijainti_x + AnnaLeveys(glob_data) && x + lev > sijainti_x
		&& y + kork == sijainti_y + AnnaKorkeus(glob_data) && avaaja->annaFysState())
	{
		int sektori = *((int*)data->at(0));
		int ax = *((int*)data->at(1));
		int yy = *((int*)data->at(2));
		Peliobjekti *linkki = ((Peliobjekti *)data->at(3));
		if(avain == linkki)
		taso->VaihdaSektori(sektori,ax,yy,glob_data,avaaja->AnnaEnergia());
	}
}

void Jeppe::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	if(taulukkonumero != Pelaaja_num) 
	{
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		if(pelaaja->AnnaSijaintiX() < sijainti_x) suunta = 0; else suunta = 1;
	}

	Tarkista_tila(sektori->annaMappi(0), sektori->annaSetti(), glob_data,0,0);

	counter = 0;
}

bool Jeppe::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Kaannos();
	Reunakaannos(sektori->annaMappi(0),sektori->annaSetti(),glob_data);

	if(suunta)
	{
		x_nopeus = 300;
	}
	else
	{
		x_nopeus = -300;
	}


	if((counter%200 < 100)) animaatio = 0;
	else animaatio = 1;

	if(counter == 50)
	{
		Tulipallo *os;
			os = (Tulipallo*)luo_objekti(Tulipallo_num,glob_data,sektori);
			os->Alusta();
			os->asetag(1);
			os->Aseta(glob_data,sijainti_x,sijainti_y);
			int x_n;
			x_n = -2650;
			os->AsetaNopeus(x_n,-10000);
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);
	}
	if(counter == 150)
	{
		Tulipallo *os;
			os = (Tulipallo*)luo_objekti(Tulipallo_num,glob_data,sektori);
			os->Alusta();
			os->asetag(1);
			os->Aseta(glob_data,sijainti_x,sijainti_y);
			int x_n;
			x_n = 2650;
			os->AsetaNopeus(x_n,-10000);
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);
	}
	

	counter++;
	if(counter == 200) counter = 0;

	Gravitaatio();

	return true;
}

bool Liekinheitin::Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso)
{
	int suunta = *(int*)(data->at(0));
	int jakso = *(int*)(data->at(1));
	int vaihe = *(int*)(data->at(2));
	int pituus = *(int*)(data->at(3));
	int voima = *(int*)(data->at(4));

	animaatio = suunta;

	if(aika >= vaihe && aika <= vaihe + pituus*10 && (aika%5 == 0))
	{

		int x_cam = sektori->annaXcam();
		int y_cam = sektori->annaYcam();

		if(sijainti_x +8+ 640 -20>= x_cam && sijainti_x+8 +20 <= x_cam + 640 + 640 &&
			sijainti_y+8 + 480 -20 >= y_cam && sijainti_y+8 +20 <= y_cam + 480 + 480)
		{

			Liekki *os;
			os = (Liekki*)luo_objekti(Liekki_num, glob_data, sektori);
			os->Alusta();
			
			os->Aseta(glob_data,sijainti_x+8,sijainti_y+8);
			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);


			float nopeus = 1000*voima;
			switch(suunta)
			{
			case 0:
				os->AsetaNopeus(nopeus,0);
				break;
			case 1:
				os->AsetaNopeus(0,-nopeus);
				break;
			case 2:
				os->AsetaNopeus(-nopeus,0);
				break;
			case 3:
				os->AsetaNopeus(0,nopeus);
				break;
			default:
				break;

			}
		}
		
	}

	aika++;

	if(aika == jakso*10)
	{
		aika = 0;
	}

	return true;
}

bool Liekki::Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso)
{
	animaatio = 0;
	x_nopeus *= 0.95;
	y_nopeus *= 0.95;

	counter++;
	if(counter > 40) Tuhottu(sektori, glob_data);
	return true;
}

void Liekki::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	counter = 0;
}

void Tippukivi::Aseta(GLOBAALI *glob_data, Tilemap *mappi, int tileset, Pelisektori *sektori)
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

	sijainti_x = x_*32+8;
	sijainti_y = y_*32;
}


void Tippukivi::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	tila = 0;
}

bool Tippukivi::Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso)
{
	int px;
	int py;
	Peliobjekti *pelaaja = sektori->annaPelaaja();

	switch(tila)
	{
	case 0:
		animaatio = 0;
		px = pelaaja->AnnaSijaintiX();
		py = pelaaja->AnnaSijaintiY();

		if(px < sijainti_x + 150 && px > sijainti_x - 150 && py > sijainti_y)
		{
			tila = 1;
			counter = 25;
		}
		
		break;

	case 1:
		animaatio = 1;

		counter--;
		if(counter == 0) tila = 2;
		break;

	case 2:
		animaatio = 0;
		Gravitaatio();

		if(ala_torm || yla_torm || vasen_torm || oikea_torm)
			Tuhottu(sektori, glob_data);

		break;

	default: break;
	}

	return true;
}

void Tippukivi::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	Pelisektori *os = taso->AnnaSektorit()->at(taso->AnnaSektori());

	if(objektitLeikkaavat(pelaaja,glob_data))
	{
		if(tila == 2) pelaaja->OtaVahinkoa(1,os, glob_data);
	}
}


void Tippupalkki::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	counter = 0;
	tila = 0;
}

bool Tippupalkki::Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso)
{
	Peliobjekti *pelaaja = sektori->annaPelaaja();
	
	int yn = pelaaja->AnnaYnopeus();
	int px1 = pelaaja->AnnaSijaintiX();
	int px2 = px1 + pelaaja->AnnaLeveys(glob_data) -1;

	int py2 = pelaaja->AnnaSijaintiY() + pelaaja->AnnaKorkeus(glob_data) -1;

	bool ehto = (py2 == sijainti_y - 1 && px1 <= sijainti_x + 31 && px2 >= sijainti_x && yn == 0);

	switch(tila)
	{
	case 0:
		animaatio = 0;

		if(ehto)
		{
			tila = 1;
			counter = 60;
		}

		
		break;

	case 1:
		animaatio = 1;
		if((!ehto) && counter > 30)
		{
			tila = 0;
		}
		else
		{
			counter--;
			if(counter == 0) tila = 2;
		}
		
		break;

	case 2:
		animaatio = 0;
		Gravitaatio();
		break;

	default: break;
	}
	return true;
}


bool Jousi::Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso)
{
	kohde_y = y_*32 + jannitys;
	return true;
}


void Jousi::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	tila = 0;
	jannitys = 0;
}

void Jousi::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	int pel_x = pelaaja->AnnaSijaintiX();
	int lev = pelaaja->AnnaLeveys(glob_data);
	int pel_y = pelaaja->AnnaSijaintiY();
	int kork = pelaaja->AnnaKorkeus(glob_data);
	int pel_yv = pelaaja->AnnaYnopeus();

	switch(tila)
	{
	case 0:

		if(jannitys > 0)
		{
			jannitys -= 2;
			if(jannitys < 0) jannitys = 0;
		}
		else
		{
			if(pel_x < sijainti_x + AnnaLeveys(glob_data) && pel_x + lev > sijainti_x
			&& pel_y + kork == sijainti_y && pel_yv >= 0)
			{
				tila = 1;
			}
		}

		break;
	case 1:

		if(pel_x < sijainti_x + AnnaLeveys(glob_data) && pel_x + lev > sijainti_x
			&& pel_y + kork == sijainti_y )
		{
			

			if(jannitys < 16) jannitys += 2;
			else
			{
				glob_data->gfx->soitaAani(glob_data->jousiaani);
				if(pohjassa(index_HYPPY,glob_data))
				{
					pelaaja->AsetaNopeus(pelaaja->AnnaXnopeus(),-12000);
				}
				else pelaaja->AsetaNopeus(pelaaja->AnnaXnopeus(),-8000);

				tila = 0;
			}

		}
		else
		{
			glob_data->gfx->soitaAani(glob_data->jousiaani);
			if(pohjassa(index_HYPPY,glob_data))
			{
				pelaaja->AsetaNopeus(pelaaja->AnnaXnopeus(),-12000);
			}
			else pelaaja->AsetaNopeus(pelaaja->AnnaXnopeus(),-8000);

			tila = 0;
		}
		break;


	}
}

void Jousi::Liikuta(GLOBAALI *glob_data)
{
}

bool Checkpoint::Logiikka(Pelisektori *sektori,GLOBAALI *glob_data, Pelitaso *taso)
{
	if(tila == 0) animaatio = 0;
	else animaatio = 1;

	return true;
}

void Checkpoint::Alusta()
{
	damage = 0;
	energia = 0;
	tila = 0;
}

void Checkpoint::PelaajaInteraktio(Peliobjekti *pelaaja, GLOBAALI *glob_data, Pelitaso *taso)
{
	if(tila == 0)
	{
		if(objektitLeikkaavat(pelaaja, glob_data))
		{
			tila = 1;
			int numero = *((int*)data->at(0));

			taso->checkpoint(taso->AnnaSektori(), x_, y_, numero);
		}
	}
}

void Tykki::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	counter = 0;
}

bool Tykki::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	counter++;

	if(counter == 50)
	{
		counter = 0;

		Tulipallo *os = (Tulipallo*)luo_objekti(Tulipallo_num, glob_data, sektori);
		os->Alusta();
			
		os->Aseta(glob_data,sijainti_x+8,sijainti_y+8);
		os->asetag(0);
		
		Peliobjekti *pelaaja = sektori->annaPelaaja();
		int delta_x = pelaaja->AnnaSijaintiX() - sijainti_x;
		int delta_y = pelaaja->AnnaSijaintiY() - sijainti_y;

		if(delta_x == 0)
		{
			if(delta_y <= 0) os->AsetaNopeus(0,-3000);
			else os->AsetaNopeus(0,3000);
		}
		else
		{
			double kulma = atan((double)delta_y/(double)delta_x);
			double x_nopeus = cos(kulma)*3000;
			double y_nopeus = sin(kulma)*3000;


			if(delta_x < 0)
			{
				x_nopeus *= -1;
				y_nopeus *= -1;
			}
			
			
			os->AsetaNopeus(x_nopeus,y_nopeus);
		}

		os->EiTuhottu();
		sektori->lisaaTuhoutuviin(os);
		os->Kaynnista(sektori,glob_data);

	}
	return true;
}

bool Tulipallo3::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	switch(moodi)
	{
	case 0:

		if(!suunta) x_nopeus = -5000;
		else x_nopeus = 5000;

		

		
			
		if(yla_torm || ala_torm)
		{
			x_nopeus = 0;
			y_nopeus = 0;
			moodi = 1;
			counter = 0;
		}

		Kaannos();
		break;

	case 1:
		
		if(counter % 5 == 0)
		{
			Lieska *os = (Lieska*)luo_objekti(Lieska_num, glob_data, sektori);
			os->Alusta();
			os->Aseta(glob_data, sijainti_x, sijainti_y + AnnaKorkeus(glob_data)-os->AnnaKorkeus(glob_data));

			os->EiTuhottu();
			sektori->lisaaTuhoutuviin(os);
			os->Kaynnista(sektori,glob_data);
			os->AsetaNopeus(-5000,-2000);

			Lieska *os2 = (Lieska*)luo_objekti(Lieska_num, glob_data, sektori);
			os2->Alusta();
			os2->Aseta(glob_data, sijainti_x, sijainti_y + AnnaKorkeus(glob_data)-os->AnnaKorkeus(glob_data));

			os2->EiTuhottu();
			sektori->lisaaTuhoutuviin(os2);
			os2->Kaynnista(sektori,glob_data);
			os2->AsetaNopeus(5000,-2000);
		}

		counter++;
		if(counter == 50) Tuhottu(sektori, glob_data);
		break;
	default: break;
	}

	return true;
}

void Tulipallo3::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	moodi = 0;
}

bool Lieska::Logiikka(Pelisektori *sektori, GLOBAALI *glob_data, Pelitaso *taso)
{
	Gravitaatio();
	counter++;
	if(counter == 25) Tuhottu(sektori, glob_data);

	return true;
}

void Lieska::Kaynnista(Pelisektori *sektori, GLOBAALI *glob_data)
{
	counter = 0;
}