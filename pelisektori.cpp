#include "maaritykset.h"



extern int varasto;


Peliobjekti *Pelisektori::annaPelaaja() {return &pelaaja;}
Tilemap *Pelisektori::annaMappi(int num) {return &(tilemapit[num]);}
std::vector <Peliobjekti *> *Pelisektori::annaObjektit() {return &objektit;}
std::vector <Linkkitieto *> *Pelisektori::annaLinkkitiedot() {return &linkit;}
std::vector <Peliobjekti *> *Pelisektori::annaAinaAktiiviset() {return &ainaAktiiviset;}
Poiminta *Pelisektori::annaLogiikka() {return &logiikka;}

int Pelisektori::annaXcam() {return x_cam;}
int Pelisektori::annaYcam() {return y_cam;}


Quadtree* Pelisektori::annaQuadtree(){return &quadtree;}

const int sirpalesarakkeet = 31;
const int sirpalerivit = 23;
const int sivu = 20;
const int sirpalemaara = 2*sirpalesarakkeet*sirpalerivit;

const kolmiorivit = 15, kolmiosarakkeet = 20;
const int kertoma = 32;
const int kolmiomaara = kolmiorivit*kolmiosarakkeet*2;

extern int x_koord[sirpalemaara*3], y_koord[sirpalemaara*3], x_text[sirpalemaara*3], y_text[sirpalemaara*3];

void Pelisektori::itsari(GLOBAALI *glob_data)
{
	if(!lapi && !kuolema) pelaaja.Tuhottu(this, glob_data);
}

bool Pelisektori::vapauta(GLOBAALI *glob_data)
{
	for(int index = 0; index < objektit.size(); index++)
	{
		objektit.at(index)->tuhoaObjekti(glob_data);
		delete objektit.at(index);
	}
	objektit.clear();

	tilemapit[0].Tuhoa();
	tilemapit[1].Tuhoa();
	tilemapit[2].Tuhoa();

	ainaAktiiviset.clear();

	return true;
}

void Pelisektori::lapaisy(int nro, GLOBAALI *glob_data)
{
	 soitaGlobaaliBiisi(glob_data, &glob_data->tasolapibiisi); lapi = 1; maalinro = nro; loppucounter = 0;
}

void Pelisektori::kasitteleTuhoutuvat()
{
	int koko = tuhoutuvat.size();
	int index = 0;
	
	while(index < koko)
	{
		if(tuhoutuvat.at(index)->OnkoTuhottu())
		{
			delete tuhoutuvat.at(index);
			tuhoutuvat.erase(tuhoutuvat.begin()+index);
			koko--;
		} else index++;
	}
}

void Pelisektori::pelaajaanOsui(GLOBAALI *glob_data)
{
	damageVarina = 10;
	
	glob_data->gfx->asetaKolmiot(1);
	alustaSirpaleet();
	glob_data->gfx->asetaKolmiodata(&(x_koord[0]),&(y_koord[0]),&(x_text[0]), &(y_text[0]), kolmiomaara);
}

void Pelisektori::camera(GLOBAALI *glob_data)
{
	#define cmitta 32

	int fys_state = pelaaja.AnnaFysState();
	int vanha_state = pelaaja.AnnaVanhaState();
	int lev = pelaaja.AnnaLeveys(glob_data);
	int kork = pelaaja.AnnaKorkeus(glob_data);

	int sij_x = pelaaja.AnnaSijaintiX();
	int sij_y = pelaaja.AnnaSijaintiY();

	int vanha_x = pelaaja.AnnaVanhaX();
	int vanha_y = pelaaja.AnnaVanhaY();

	int delta_x = sij_x - vanha_x;
	int delta_y = sij_y - vanha_y;

	int x_nopeus = pelaaja.AnnaXnopeus();
	int y_nopeus = pelaaja.AnnaYnopeus();

	int pel_y = sij_y + kork/2;
	
	
	switch(x_camera_mode)
	{
	case 0:
		if(delta_x > 0 && sij_x+lev/2-x_cam > 320+80) {x_camera_mode = 1; x_cam_speed = 0;}
		if(delta_x < 0 && sij_x+lev/2-x_cam < 320-80) {x_camera_mode = 2; x_cam_speed = 0;}
		break;
	case 1:
		x_cam += 4 + x_nopeus/1000;
		if(sij_x + lev/2 -x_cam <= (320-cmitta)) {x_cam = sij_x + lev/2 - (320-cmitta); x_camera_mode = 3;}
		break;
	case 2:
		x_cam += -4 + x_nopeus/1000;
		if(sij_x + lev /2-x_cam >= 320+cmitta) {x_cam = sij_x+lev/2-(320+cmitta); x_camera_mode = 4;}
		break;
	case 3:
		if(sij_x+lev/2 -x_cam > 320-cmitta) x_cam = sij_x+lev/2-(320-cmitta);
		if(sij_x+lev/2 -x_cam < 320-64) x_camera_mode = 0;
		break;
	case 4:
		if(sij_x+lev/2 -x_cam < (640-(320-cmitta))) x_cam = sij_x+lev/2-(640-(320-cmitta));
		if(sij_x+lev/2 -x_cam > (640-(320-64))) x_camera_mode = 0;
		break;
	}

	

	if(fys_state == FYS_MP && !vanha_state)
	{
		if(pel_y < y_cam + 160 && pel_y -300 < y_cam)
		{
			y_cam_dest = pel_y - 300;
			if(y_cam_dest < 10*32) y_cam_dest = 10*32;
			if(y_cam_dest + 480 > (tilemapit[0].AnnaY()-10)*32)
				y_cam_dest = (float)((tilemapit[0].AnnaY()-10)*32 -480);
			y_camera_mode = 1;
		}
		if(pel_y > y_cam + 320 && pel_y-300 > y_cam)
		{
			y_cam_dest = pel_y - 300;
			if(y_cam_dest < 10*32) y_cam_dest = 10*32;
			if(y_cam_dest + 480 > (tilemapit[0].AnnaY()-10)*32)
				y_cam_dest = (float)((tilemapit[0].AnnaY()-10)*32 -480);
			y_camera_mode = 2;
		}
	}

	if(fys_state == FYS_MAKI || fys_state == FYS_KYYDISSA)
	{
		if((pel_y < y_cam + 120) && (delta_y < 0)) y_camera_mode = 3;
		if((pel_y > y_cam + 360) && (delta_y > 0)) y_camera_mode = 4;
	}

	if(y_camera_mode == 0 && fys_state == FYS_EI)
	{
		if((pel_y < y_cam + 80) && (delta_y < 0)) y_camera_mode = 5;
		if((pel_y > y_cam + 400) && (delta_y > 0)) y_camera_mode = 6;
	}
	
	

	if(y_camera_mode == 1)
	{
		y_cam -= 8;

		if(fys_state != FYS_MP) y_camera_mode = 0;
		if(y_cam <= y_cam_dest) {y_cam = y_cam_dest; y_camera_mode = 0;}
	}
	if(y_camera_mode == 2)
	{
		y_cam += 8;

		if(fys_state != FYS_MP) y_camera_mode = 0;
		if(y_cam >= y_cam_dest) {y_cam = y_cam_dest; y_camera_mode = 0;}
	}

	if(y_camera_mode == 3)
	{
		if(!(fys_state == FYS_MAKI || fys_state == FYS_KYYDISSA)) y_camera_mode = 0;
		else if(pel_y > y_cam + 260) y_camera_mode = 0;
		else if(delta_y < 0)
		{
			y_cam += delta_y;
			if(pel_y < y_cam + 240) y_cam -= 2;
			if(pel_y > y_cam + 240) y_cam = pel_y -240;
			
		}
	}

	if(y_camera_mode == 4)
	{
		if(!(fys_state == FYS_MAKI || fys_state == FYS_KYYDISSA)) y_camera_mode = 0;
		else if(pel_y < y_cam + 220) y_camera_mode = 0;
		else if(delta_y > 0)
		{
			y_cam += delta_y;
			if(pel_y > y_cam + 240) y_cam += 2;
			if(pel_y < y_cam + 240) y_cam = pel_y -240;
		}
	}

	if(y_camera_mode == 5)
	{
		if(delta_y > 0) y_camera_mode = 0;
		else if(fys_state != FYS_EI) y_camera_mode = 0;
		else
		{
			y_cam += delta_y;
			if(pel_y < y_cam + 240) y_cam -= 3;
			if(pel_y > y_cam + 240) y_cam = pel_y-240;
		}
	}

	if(y_camera_mode == 6)
	{
		if(delta_y < 0) y_camera_mode = 0;
		else if(fys_state != FYS_EI) y_camera_mode = 0;
		else
		{
			y_cam += delta_y;
			if(pel_y > y_cam + 240) y_cam += 3;
			if(pel_y < y_cam + 240) y_cam = pel_y-240;
		}
	}

	
	if(x_cam < 0) x_cam = 0;
	if(x_cam + 640 > tilemapit[0].AnnaX()*32)
		x_cam = tilemapit[0].AnnaX()*32 -640;


	if(y_cam < 10*32) y_cam = 10*32;
	if(y_cam + 480 > (tilemapit[0].AnnaY()-10)*32)
		y_cam = ((tilemapit[0].AnnaY()-10)*32 -480);

	if(y_cam < 0) y_cam = 0;
	if(y_cam > tilemapit[0].AnnaY()*32 - 480) y_cam = tilemapit[0].AnnaY()*32 - 480;

}

int Pelisektori::toiminta(GLOBAALI *glob_data, Pelitaso *taso)
{
	int skipit = 0;
	
	while(varasto >= timestep)
	{
		skipit ++;
		varasto -= timestep;
	}

	InputData *input = glob_data->gfx->annaInput();


	if(ei_frameskippeja)skipit = 0;

	int paluuarvo;


	if(!pause)
	for(int ind = 0; ind < 1+skipit; ind++) 
	{				
		paluuarvo = logic(glob_data, taso);
		if(paluuarvo != -2) break;
	} else paluuarvo = -2;

	
	
	piirto(glob_data, taso);

	if(pause)
	{
		Rect laatikko;
		laatikko.x = 64;
		laatikko.y = 42;
		laatikko.width = 241;
		laatikko.height = 70;

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,200,205, &laatikko,1,0);
	}

	

	return paluuarvo;
}


bool Pelisektori::piirraObjektit(GLOBAALI *glob_data)
{
	piirrettavat.Alusta();
	
	quadtree.poimiObjektit(x_cam-320, y_cam-240, x_cam+640+320, y_cam+480+240, &piirrettavat,glob_data,1);

	for(int index = 0; index < tuhoutuvat.size(); index++)
	{
		piirrettavat.Lisaaobjekti(tuhoutuvat.at(index));
	}

	int indeksi = 0;
	Peliobjekti *objekti;
	Poiminta *ptr;

	for(int layeri = 0; layeri < 3; layeri++)
	{
		indeksi = 0;
		ptr = &piirrettavat;

		while(objekti = ptr->annaAlkio(&indeksi, &ptr))
		{
			if(glob_data->objektityypit.at(objekti->annaTyyppi())->piirtojarjestys == layeri)
			objekti->Piirra(x_cam,y_cam,glob_data);
		}
	}

	return 0;
}


bool Pelisektori::sammuta(GLOBAALI *glob_data)
{
	// versiossa 1.02 siirretty pelitason sammutukseen
	//pysaytaGlobaaliBiisi(glob_data);


	quadtree.Tuhoa();

	int koko = tuhoutuvat.size();
	int index = 0;
	
	while(index < koko)
	{
		delete tuhoutuvat.at(index);
		index++;
	}

	tuhoutuvat.clear();

	kadonneet.clear();


	
	return true;
}



bool Pelisektori::kaynnista(GLOBAALI *glob_data, int alku_x, int alku_y, int energia)
{
	varasto  = 0;
	Biisi *biisi;
	switch(musiikki)
	{
	case 0:
		biisi = &glob_data->perusbiisi; break;
	case 1:
		biisi = &glob_data->luolabiisi; break;
	case 2:
		biisi = &glob_data->linnabiisi; break;
	case 3:
		biisi = &glob_data->rantabiisi; break;
	case 4:
		biisi = &glob_data->lumibiisi; break;
	case 5:
		biisi = &glob_data->bossibiisi; break;
	};

	soitaGlobaaliBiisi(glob_data, biisi);
	for(int setti = 0; setti < glob_data->tilesetit.size(); setti++)
	{
		if(setti == tileset) continue;
		vapauta_setti(glob_data,setti);
	}

	lataa_setti(glob_data,tileset);


	kadonneet.clear();

	// lis‰t‰‰n pelaaja sektoriin ja quadtreehen
	damageVarina = 0;
	glob_data->gfx->asetaKolmiot(0);
	lapi = 0;
	kuolema = 0;
	pause = 0;

	//pelaaja = (Pelaaja*)luo_objekti(Pelaaja_num,glob_data,this);

	int num = glob_data->tyyppinimet.at(Pelaaja_num).jarjestysnumero;
	pelaaja.AsetaTaulukkoNum(Pelaaja_num);
	pelaaja.AsetaTyyppi(num);
	pelaaja.AsetaLaatu(glob_data->objektityypit.at(num)->quadtree_num);


	pelaaja.AsetaAlkuKoordinaatit(alku_x, alku_y);
	pelaaja.Alusta();
	pelaaja.Aseta(glob_data, &tilemapit[0], tileset, this);
	pelaaja.EiTuhottu();
	pelaaja.EiAinaAktiivinen();
	
	
	int x = tilemapit[0].AnnaX();
	int y = tilemapit[0].AnnaY();
	
	

	quadtree.Alusta(-640-160,-480-120,x*32+640+160,y*32+480+120);

	int index;
	for(index = 0; index < objektit.size(); index++)
	{
		int luokka = 0;
		if(luokka == 0) objektit.at(index)->EiTuhottu();


		int tyyppi = objektit.at(index)->annaTyyppi();

		if(!objektit.at(index)->OnkoTuhottu())
		{
			objektit.at(index)->Aseta(glob_data,&tilemapit[0],tileset,this);
			quadtree.LisaaObjekti(objektit.at(index));
		}
	}

	for(index = 0; index < ainaAktiiviset.size(); index++)
	{
		ainaAktiiviset.at(index)->Aktivoi();
		ainaAktiiviset.at(index)->Kaynnista(this, glob_data);
	}

	quadtree.LisaaObjekti(&pelaaja);
	pelaaja.saadaEnergia(energia);

	x_cam = (float)((pelaaja.AnnaSijaintiX() + pelaaja.AnnaLeveys(glob_data)/2 -320));

	if(x_cam < 0) x_cam = 0;
	if(x_cam + 640 > x*32) x_cam = x*32 -640;

	y_cam = (float)((pelaaja.AnnaSijaintiY() + pelaaja.AnnaKorkeus(glob_data) -300));

	if(y_cam < 10*32) y_cam = 10*32;
	if(y_cam > (y-10)*32 -480) y_cam = (y-10)*32 -480;

	if(y_cam < 0) y_cam = 0;
	if(y_cam > y*32 - 480) y_cam = y*32 - 480;


	frame = 0;
	x_camera_mode = 0;
	y_camera_mode = 0;

	return true;
}

bool Pelisektori::alustaObjektit(bool ekakerta)
{
	
	for(int index = 0; index < objektit.size(); index++)
	{
		int luokka = 0;

		if(luokka != 2 || ekakerta)
		{
			objektit.at(index)->Alusta();
			objektit.at(index)->EiTuhottu();
		}
	}

	return true;
}

bool Pelisektori::lataa(std::ifstream &tiedosto, GLOBAALI *glob_data, int koodi)
{
	// t‰ss‰ funktiossa ladataan pelisektoriin tiedot,
	// esim. objektit objektivektoriin.
	// t‰ytet‰‰n ainaAktiiviset-vektori

	tileset = 0;
	musiikki = 0;
	
	std::string setinnimi = "";
	
	LueString(&setinnimi, tiedosto);

	int numero = 0;
	bool loytyi = 0;

	unsigned int index;
	for(index = 0; index < glob_data->tilesetit.size(); index++)
	{
		if(glob_data->tilesetit.at(index)->nimi == setinnimi)
		{
			numero = index;
			loytyi = 1;
			break;
		}
	}

	tileset = numero;
	tiedosto.read((char*)&musiikki, sizeof(int));
	
	for(index = 0; index < 3; index++)
	{
		tiedosto.read((char*)(&(nopeudet[index])), sizeof(int));
		tilemapit[index].AsetaData(0);
		tilemapit[index].Lataa(tiedosto,glob_data,numero, index, koodi);
	}



	unsigned int tyyppimaara;

	tiedosto.read((char*)&tyyppimaara, sizeof(int));

	for(index = 0; index < tyyppimaara; index++)
	{
		std::string tyyppinimi;
		LueString(&tyyppinimi, tiedosto);

		loytyi = 0;

		unsigned int indeksi;
		int tyyppinumero;
		for(indeksi = 0; indeksi < glob_data->objektityypit.size(); indeksi++)
		{
			if(glob_data->objektityypit.at(indeksi)->nimi == tyyppinimi)
			{
				numero = indeksi;
				loytyi = 1;
				tyyppinumero = glob_data->objektityypit.at(indeksi)->tyyppinumero;
			}
		}
		if(!loytyi) return false;

		unsigned int maara;
		tiedosto.read((char*)&maara, sizeof(int));

		
		for(indeksi = 0; indeksi < maara; indeksi++)
		{
			int koko;
			tiedosto.read((char*)&koko, sizeof(int));

			if(loytyi)
			{
				Peliobjekti *uusi = luo_objekti(tyyppinumero, glob_data, this);
				int jarjestysnumero = glob_data->tyyppinimet.at(tyyppinumero).jarjestysnumero;

				uusi->LataaData(tiedosto, glob_data, jarjestysnumero, &linkit);
				objektit.push_back(uusi);

				if(uusi->onkoAinaAktiivinen()) lisaaKasittely(uusi);
			}
			else
			{
				for(int tyhja = 0; tyhja < koko; tyhja++)
				{
					int jep;
					tiedosto.read((char*)&jep, sizeof(int));
				}
			}
		}

	}

	return true;
}



bool Pelisektori::piirto(GLOBAALI *glob_data, Pelitaso *taso)
{
	
	taustapiirto(glob_data);
	tilemapit[0].draw_scrolling_tilemap(glob_data, x_cam,y_cam,tileset,false);
	
	piirraObjektit(glob_data);

//	if(ohjelmatila == PELITILA)
//	{
		for(int index = 0; index < pelaaja.AnnaEnergia(); index++)
		{
			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 120 + index*40,10, &laatikot[sydan_kuva],1,0);

		}

		
	//}

	tilemapit[0].draw_scrolling_tilemap(glob_data, x_cam,y_cam,tileset,true);

	pelaaja.piirraInventory(glob_data);

	if(lapi)
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 208, 40, &laatikot[levelclear_kuva],1,0);
	if(kuolema)
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 208, 40, &laatikot[youredead_kuva],1,0);

	if(damageVarina)
	{
		damageVarina--;

		varina(glob_data);

		if(!damageVarina) glob_data->gfx->asetaKolmiot(0);
	}

	return true;
}


void Pelisektori::pauseta()
{
	if(!lapi && !kuolema) pause = !pause;
}

int Pelisektori::logic(GLOBAALI *glob_data, Pelitaso *taso)
{
	// Tyhjennet‰‰n logiikka-poiminta
	logiikka.Alusta();
	
	int koko;
	int index;


	// lis‰t‰‰n aina aktiiviset objektit logiikka-poimintaan
	for(index = 0; index < ainaAktiiviset.size(); index++)
	{
		if(!ainaAktiiviset.at(index)->OnkoTuhottu())
		{
			logiikka.Lisaaobjekti(ainaAktiiviset.at(index));
		}
	}

	// lis‰t‰‰n tuhoutuvat objektit logiikka-pomintaan

	for(index = 0; index < tuhoutuvat.size(); index++)
		logiikka.Lisaaobjekti(tuhoutuvat.at(index));
	


	// poimii logiikka-poimintaan kaikki l‰hell‰ olevat objektit
	// sek‰ lis‰‰ kadonneet-poimintaan kaikki deaktivoituneet objektit,
	// ei poimita aina aktiivisia objekteja, ne on jo poimittu
	quadtree.Aktiivisuus(x_cam, y_cam, x_cam-640-160,y_cam-480-120,
		x_cam+640+640+160,y_cam+480+480+120,glob_data,&kadonneet,&logiikka,this);

	tarkistaTilat(glob_data);
	
	
	Peliobjekti *objekti;


	// suoritetaan objektien logiikka-funktio
	int indeksi = 0;
	Poiminta *ptr = &logiikka;
	while(objekti = ptr->annaAlkio(&indeksi,&ptr))
	{
		objekti->Logiikka(this,glob_data,taso);
		objekti->animoiObjekti(glob_data);
		objekti->Damage();
	}



	// sijoitetaan peliobjektien nykyiset sijainnit ja tilat vanhoiksi
	indeksi = 0;
	ptr = &logiikka;
	while(objekti = ptr->annaAlkio(&indeksi,&ptr))
	{
		objekti->OtaVanhat();
	}

	
	// nopeuden ja nykyisen paikan perusteella m‰‰ritell‰‰n kohdesijainti
	indeksi = 0;
	ptr = &logiikka;
	while(objekti = ptr->annaAlkio(&indeksi,&ptr))
	{
		objekti->Liikuta(glob_data);
	}


	// Kyydiss‰:
	// lis‰t‰‰n objektien kohdesijaintiin p‰‰ll‰seist‰v‰n objektin
	// kohde- ja sijaintikoordinaattien erotus
	indeksi = 0;
	ptr = &logiikka;
	while(objekti = ptr->annaAlkio(&indeksi,&ptr))
	{
		objekti->Kyydissa(glob_data);
	}

	
	// m‰‰ritell‰‰n objektien lopullinen sijainti ja tila
	tormaykset(glob_data);


	// katsoo, ovatko kadonneet objektit tarpeeksi kaukana kamerasta, jotta ne voi lis‰t‰
	// quadtreehen alkuper‰iselle paikalleen odottamaan aktivoituista ja poistaa kadonneista
	koko = kadonneet.size();
	index = 0;
	while(index < koko)
	{
		if(kadonneet.at(index)->Palautustesti(x_cam, y_cam, glob_data, &tilemapit[0], tileset, this))
		{
			quadtree.LisaaObjekti(kadonneet.at(index));
			kadonneet.erase(kadonneet.begin()+index);
			koko--;
		} else index++;
	}


	// tarkistaa ovatko objektit joihin objekti osoittaa, tuhottuja
	indeksi = 0;
	ptr = &logiikka;
	while(objekti = ptr->annaAlkio(&indeksi,&ptr))
	{
		objekti->tuhoaRiippuvuudet();;
	}



	quadtree.tuhoustesti(); // poistaa tuhoutuneet quadtreesta
	kasitteleTuhoutuvat(); // tuhoaa v‰liaikaiset tuhoutuneet objektit muistista kokonaan


	uusiksi.Alusta();
	quadtree.poistaYlimenneet(&uusiksi);
	quadtree.tuhoaVajaat();

	// sijoittaa ylimenneet objektit uusiin quadtreihin
	ptr = &uusiksi;
	indeksi = 0;
	while(objekti = ptr->annaAlkio(&indeksi,&ptr))
		quadtree.LisaaObjekti(objekti);




	AnimoiTilet(glob_data,tileset);
	camera(glob_data);

	if(pelaaja.OnkoTuhottu() && !kuolema)
	{
		kuolema = 1; loppucounter = 0;
		soitaGlobaaliBiisi(glob_data, &glob_data->kuolemabiisi);
	}

	
	if(lapi)
	{
		if(loppucounter >= 100)
		{
			glob_data->gfx->asetaR((loppucounter-100)*8);
		}
		if (loppucounter < 200) loppucounter++;
		else {glob_data->gfx->asetaR(0);return maalinro;}
	}

	if(kuolema)
	{
		if(loppucounter == 100)
		{
			alustaSirpaleet(); // hajoaminen
			glob_data->gfx->asetaKolmiodata(&(x_koord[0]),&(y_koord[0]),&(x_text[0]), &(y_text[0]), sirpalemaara); //hajoaminen
			glob_data->gfx->asetaKolmiot(1); //molemmat
		}
		if(loppucounter > 100)
		{
			
				kasitteleSirpaleet();//hajoaminen

			//glob_data->gfx->asetaR((loppucounter-100)*8);
		}

		if (loppucounter < 250) loppucounter++;
		else
		{
			glob_data->gfx->asetaKolmiot(0);
			//glob_data->gfx->asetaR(0);
			return -1;
		}
	}


	return -2;
}


void Pelisektori::tarkistaTilat(GLOBAALI *glob_data)
{
	tavalliset.Alusta();
	kiinteat.Alusta();
	palkit.Alusta();

	quadtree.poimiObjektit(x_cam-640,y_cam-480,x_cam+640+640,y_cam+480+480,&tavalliset,glob_data,0,0);
	quadtree.poimiObjektit(x_cam-640,y_cam-480,x_cam+640+640,y_cam+480+480,&kiinteat,glob_data,0,1);
	quadtree.poimiObjektit(x_cam-640,y_cam-480,x_cam+640+640,y_cam+480+480,&palkit,glob_data,0,3);

	for(int index = 0; index < ainaAktiiviset.size(); index++)
	{
		Peliobjekti *obj = ainaAktiiviset.at(index);
		int laatu = obj->AnnaLaatu();

		switch(laatu)
		{
		case 0:
			tavalliset.Lisaaobjekti(obj);
			break;
		case 1:
			kiinteat.Lisaaobjekti(obj);
			break;
		case 2:
			break;
		case 3:
			palkit.Lisaaobjekti(obj);
			break;
		}

	}

	for(index = 0; index < tuhoutuvat.size(); index++)
	{
		Peliobjekti *obj = tuhoutuvat.at(index);
		int laatu = obj->AnnaLaatu();

		switch(laatu)
		{
		case 0:
			tavalliset.Lisaaobjekti(obj);
			break;
		case 1:
			kiinteat.Lisaaobjekti(obj);
			break;
		case 2:
			break;
		case 3:
			palkit.Lisaaobjekti(obj);
			break;
		}

	}

	Poiminta *ptr = &tavalliset;
	Peliobjekti *objekti;
	int indeksi = 0;

	//tavalliset.Tilantarkastus(&tilemapit[0], tileset, glob_data);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		objekti->Tarkista_tila(&tilemapit[0], tileset, glob_data,&kiinteat,&palkit);
	}
}

bool Pelisektori::tormaykset(GLOBAALI *glob_data)
{
	int x_tilet = tilemapit[0].AnnaX();
	int y_tilet = tilemapit[0].AnnaY();
	kiinteat.Alusta();
	tavalliset.Alusta();
	kolikot.Alusta();
	palkit.Alusta();

	quadtree.poimiObjektit(x_cam-640,y_cam-480,x_cam+640+640,y_cam+480+480,&tavalliset,glob_data,0,0);
	quadtree.poimiObjektit(x_cam-640,y_cam-480,x_cam+640+640,y_cam+480+480,&kiinteat,glob_data,0,1);
	quadtree.poimiObjektit(x_cam-640,y_cam-480,x_cam+640+640,y_cam+480+480,&kolikot,glob_data,0,2);
	quadtree.poimiObjektit(x_cam-640,y_cam-480,x_cam+640+640,y_cam+480+480,&palkit,glob_data,0,3);

	
	for(int index = 0; index < ainaAktiiviset.size(); index++)
	{
		Peliobjekti *obj = ainaAktiiviset.at(index);
		int laatu = obj->AnnaLaatu();

		switch(laatu)
		{
		case 0:
			tavalliset.Lisaaobjekti(obj);
			break;
		case 1:
			kiinteat.Lisaaobjekti(obj);
			break;
		case 2:
			kolikot.Lisaaobjekti(obj);
			break;
		case 3:
			palkit.Lisaaobjekti(obj);
			break;
		}

	}

	for(index = 0; index < tuhoutuvat.size(); index++)
	{
		Peliobjekti *obj = tuhoutuvat.at(index);
		int laatu = obj->AnnaLaatu();

		switch(laatu)
		{
		case 0:
			tavalliset.Lisaaobjekti(obj);
			break;
		case 1:
			kiinteat.Lisaaobjekti(obj);
			break;
		case 2:
			kolikot.Lisaaobjekti(obj);
			break;
		case 3:
			palkit.Lisaaobjekti(obj);
			break;
		}

	}

	

	int indeksi = 0;
	Poiminta *ptr = &kiinteat;
	Peliobjekti *objekti;

	//kiinteat.KiinteaSiirtoX(glob_data,&tavalliset,tileset);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		Poiminta *ptr2 = &tavalliset;
		int indeksi2 = 0;

		Peliobjekti *objekti2;

		while(objekti2 = ptr2->annaAlkio(&indeksi2, &ptr2))
		{
			objekti2->TyontoX(objekti,glob_data);
		}

		objekti->SiirraX(0,0,0,tileset,glob_data);
	}

	

	ptr = &palkit;
	indeksi = 0;

	//palkit.PalkkiSiirtoX(glob_data);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		objekti->SiirraX(0,0,0,0,glob_data);
	}

	
	ptr = &tavalliset;
	indeksi = 0;

	//tavalliset.ObjektiSiirtoX(glob_data, &tilemapit[0], &kiinteat, tileset);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		objekti->SiirraX(&tilemapit[0],&kiinteat,1,tileset,glob_data);
	}


	ptr = &kiinteat;
	indeksi = 0;

	//kiinteat.KiinteaSiirtoY(glob_data,&tavalliset,tileset);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		Poiminta *ptr2 = &tavalliset;
		int indeksi2 = 0;

		Peliobjekti *objekti2;

		while(objekti2 = ptr2->annaAlkio(&indeksi2, &ptr2))
		{
			objekti2->TyontoY(objekti, glob_data);
		}

		objekti->SiirraY(0,0,0,0,tileset,glob_data);
	}
	
	

	ptr = &palkit;
	indeksi = 0;

	//palkit.PalkkiSiirtoY(glob_data,&tavalliset);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		Poiminta *ptr2 = &tavalliset;
		int indeksi2 = 0;
		Peliobjekti *objekti2;

		while(objekti2 = ptr2->annaAlkio(&indeksi2, &ptr2))
		{
			objekti2->PalkkiTyontoY(objekti, glob_data);
		}

		objekti->SiirraY(0,0,0,0,0,glob_data);
	}

	

	ptr = &tavalliset;
	indeksi = 0;

	//tavalliset.ObjektiSiirtoY(glob_data, &tilemapit[0], &kiinteat, &palkit, tileset);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		objekti->SiirraY(&tilemapit[0],&kiinteat,&palkit,1,tileset, glob_data);
	}
	

	ptr = &kolikot;
	indeksi = 0;

	//kolikot.Siirto(glob_data, tileset);
	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		objekti->SiirraX(0,0,0,tileset,glob_data);
		objekti->SiirraY(0,0,0,0,tileset,glob_data);
	}
	

	



	indeksi = 0;
	ptr = &tavalliset;
	//tavalliset.KiinteaKuolema(&kiinteat,glob_data,this);

	while(objekti = ptr->annaAlkio(&indeksi, &ptr))
	{
		int indeksi2 = 0;
		Poiminta *ptr2 = &kiinteat;
		Peliobjekti *objekti2;
		while(objekti2 = ptr2->annaAlkio(&indeksi2, &ptr2))
		{
			objekti->KiinteaKuolema(objekti2, glob_data,this);
		}
	}

	return true;
}

int Pelisektori::taustapiirto(GLOBAALI *glob_data)
{
	 
	char tausta_piirretaan[21*16];
	char taustamaski[21*16];
	char taustakerros_2[21*16];
	char paallysmaski[21*16];
	char tausta_2_piirretaan[21*16];
	char taustakerros[21*16];

	memset(&tausta_piirretaan,0,21*16*sizeof(char));
	memset(&taustamaski,0,21*16*sizeof(char));
	memset(&taustakerros_2,0,21*16*sizeof(char));
	memset(&paallysmaski,0,21*16*sizeof(char));
	memset(&tausta_2_piirretaan,0,21*16*sizeof(char));
	memset(&taustakerros,0,21*16*sizeof(char));
	
	
	if(x_cam < 0) x_cam = 0;
	if(y_cam < 0) y_cam = 0;

	if(x_cam > tilemapit[0].AnnaX()*32 -640) x_cam = tilemapit[0].AnnaX()*32 -640;
	if(y_cam > tilemapit[0].AnnaY()*32 -480) y_cam = tilemapit[0].AnnaY()*32 -480;


	int x_juttu;
	if(x_cam%32 != 0) x_juttu = 1;
	else x_juttu = 0;

	int y_juttu;
	if(y_cam%32 != 0) y_juttu = 1;
	else y_juttu = 0;

	int y = y_cam/32;
	int x = x_cam/32;

	// Asetetaan kameroiden sijainnit

	
	int x_cam_2 = x_cam/2;
	int y_cam_2 = (tilemapit[1].AnnaY()-15)*32 -((tilemapit[0].AnnaY()-15)*32 -y_cam)/2;

	if(x_cam_2 < 0) x_cam_2 = 0;
	if(y_cam_2 < 0) y_cam_2 = 0;

	//if(x_cam_2 > tilemapit[1]->AnnaX()*32 -640) x_cam_2 = tilemapit[1]->AnnaX()*32 -640;
	if(y_cam_2 > tilemapit[1].AnnaY()*32 -480) y_cam_2 = tilemapit[1].AnnaY()*32 -480;





	int x_cam_3 = x_cam/4;
	int y_cam_3 = (tilemapit[2].AnnaY()-15)*32 -((tilemapit[0].AnnaY()-15)*32 -y_cam)/4;

	if(x_cam_3 < 0) x_cam_3 = 0;
	if(y_cam_3 < 0) y_cam_3 = 0;

	//if(x_cam_3 > tilemapit[2]->AnnaX()*32 -640) x_cam_3 = tilemapit[2]->AnnaX()*32 -640;
	if(y_cam_3 > tilemapit[2].AnnaY()*32 -480) y_cam_3 = tilemapit[2].AnnaY()*32 -480;

	

	int rivi;
	
	int index = y*tilemapit[0].AnnaX() +x;
	
	// Kopioidaan tilemapeista t‰ll‰ hetkell‰ n‰kyv‰ data taulukoihin
	
	unsigned char *mappi = tilemapit[0].AnnaData();
	// Kerros 1


	for(rivi = 0; rivi < 15+y_juttu; rivi++)
	{
		memcpy(&(paallysmaski[rivi*21]), &(mappi[index]), (x_juttu+20)*sizeof(char));
		index += tilemapit[0].AnnaX();
	}


	unsigned char *tilemaski = glob_data->tilesetit.at(tileset)->kerrokset[0]->avaimet;
	for(index = 0; index < 21*16; index++)
	{
		paallysmaski[index] = tilemaski[paallysmaski[index]];
	}

	// Taustakerros 1
	int jakaus = 0;
	int leveys_1, leveys_2;

	y = y_cam_2/32;
	x = x_cam_2/32;

	if(x_cam_2%32 != 0) x_juttu = 1;
	else x_juttu = 0;

	if(y_cam_2%32 != 0) y_juttu = 1;
	else y_juttu = 0;


	if(x_cam_2 / (tilemapit[1].AnnaX()*32) != (x_cam_2+20*32) / (tilemapit[1].AnnaX()*32))
	{
		jakaus = 1;
		leveys_2 = tilemapit[1].AnnaX() -(x_cam_2/32)%tilemapit[1].AnnaX();
		leveys_1 = 20 -leveys_2 +x_juttu;
		
	}

	index = y*tilemapit[1].AnnaX();
	
	mappi = tilemapit[1].AnnaData();
	
	for(rivi = 0; rivi < 15+y_juttu; rivi++)
	{
		if(jakaus)
		{
			memcpy(&(taustakerros[rivi*21]), &(mappi[index+(x%tilemapit[1].AnnaX())]), leveys_2*sizeof(char));
			memcpy(&(taustakerros[rivi*21+leveys_2]), &(mappi[index]), leveys_1*sizeof(char));
		}
		else
			memcpy(&(taustakerros[rivi*21]), &(mappi[index+(x%tilemapit[1].AnnaX())]), (20+x_juttu)*sizeof(char));

		index += tilemapit[1].AnnaX();
	}

	memcpy(taustamaski, taustakerros, 21*16);
	unsigned char * tilemaski_2 = glob_data->tilesetit.at(tileset)->kerrokset[1]->avaimet;


	for(index = 0; index < 21*16; index++)
	{
		taustamaski[index] = tilemaski_2[taustamaski[index]];
	}

	// Taustakerros 2

	leveys_1 = 0;
	leveys_2 = 0;
	jakaus = 0;
	
	y = y_cam_3/32;
	x = x_cam_3/32;

	if(x_cam_3%32 != 0) x_juttu = 1;
	else x_juttu = 0;

	if(y_cam_3%32 != 0) y_juttu = 1;
	else y_juttu = 0;

	leveys_1 = 0;
	leveys_2 = 0;
	jakaus = 0;

	

	
	if(x_cam_3 / (tilemapit[2].AnnaX()*32) != (x_cam_3+20*32) / (tilemapit[2].AnnaX()*32))
	{
		jakaus = 1;
		leveys_2 = tilemapit[2].AnnaX() -((int)(x_cam_3/32))%tilemapit[2].AnnaX();
		leveys_1 = 20 -leveys_2 +x_juttu;
		
	}

	index = y*tilemapit[2].AnnaX();

	mappi = tilemapit[2].AnnaData();

	for(rivi = 0; rivi < 15+y_juttu; rivi++)
	{
		if(jakaus)
		{
			memcpy(&(taustakerros_2[rivi*21]), &(mappi[index+(x%tilemapit[2].AnnaX())]), leveys_2*sizeof(char));
			memcpy(&(taustakerros_2[rivi*21+leveys_2]), &(mappi[index]), leveys_1*sizeof(char));
		}
		else
			memcpy(&(taustakerros_2[rivi*21]), &(mappi[index+(x%tilemapit[2].AnnaX())]), (20+x_juttu)*sizeof(char));

		index += tilemapit[2].AnnaX();
	}

	///////

	// Testataan piirret‰‰nkˆ Tausta 1


	for(index = 0; index < 21*16; index++) tausta_piirretaan[index] = 1;
	poista_tausta(paallysmaski, tausta_piirretaan, (int)x_cam, (int)y_cam, x_cam_2, y_cam_2);


	// Testataan piirret‰‰nkˆ Tausta 2

	for(index = 0; index < 21*16; index++) tausta_2_piirretaan[index] = 1;

	// Paallyskerroksen perusteella
	poista_tausta(paallysmaski, tausta_2_piirretaan, (int)x_cam, (int)y_cam, x_cam_3, y_cam_3);

	// 1.Taustakerroksen perusteella
	poista_tausta(taustamaski, tausta_2_piirretaan, x_cam_2, y_cam_2, x_cam_3, y_cam_3);
	
	
	// Piirret‰‰n tausta 2

	Rect lahde;
	Sprite *kuva = glob_data->tilesetit.at(tileset)->kerrokset[2]->sprite;

	int kohde_x, kohde_y;

	
	if(x_cam_3%32 != 0) x_juttu = 1;
	else x_juttu = 0;

	if(y_cam_3%32 != 0) y_juttu = 1;
	else y_juttu = 0;

	Anim_tile **animaatiotaulukko = glob_data->tilesetit.at(tileset)->kerrokset[2]->animoidut;
	int y_piirto = 0;

	int t_maara = glob_data->tilesetit.at(tileset)->kerrokset[2]->tilemaara;

	for(y_piirto = 0; y_piirto < 15+y_juttu; y_piirto++)
	{

		for(int x_piirto = 0; x_piirto < 20+x_juttu; x_piirto++)
		{
			kohde_x = -(x_cam_3%32) + x_piirto*32;
			kohde_y = -(y_cam_3%32) + y_piirto*32;

			if(tausta_2_piirretaan[y_piirto*21+x_piirto])
			{	
				unsigned char tile = taustakerros_2[y_piirto*21+x_piirto];

				if(tile < t_maara)
				{
					Anim_tile *anim = animaatiotaulukko[tile];

					if(anim)
					{
						int ruutu;
						if(anim->ruutu == 0) ruutu = tile;
						else ruutu = anim->ruudut.at(anim->ruutu-1);
						lahde.x = 1+(ruutu%20)*33;
						lahde.y = 1+(int)(ruutu/20)*33;
					}
					else
					{
						lahde.x = 1+(tile%20)*33;
						lahde.y = 1+(int)(tile/20)*33;
						
					}	
					
					lahde.width = 32;
					lahde.height = 32;

					
					glob_data->gfx->PiirraSprite(kuva, kohde_x, kohde_y, &lahde, 0, 0);
				}
			}
		}

	}

	// Piirret‰‰n tausta 1

		
	kuva = glob_data->tilesetit.at(tileset)->kerrokset[1]->sprite;

	if(x_cam_2%32 != 0) x_juttu = 1;
	else x_juttu = 0;

	if(y_cam_2%32 != 0) y_juttu = 1;
	else y_juttu = 0;
	
	animaatiotaulukko = glob_data->tilesetit.at(tileset)->kerrokset[1]->animoidut;
	
	t_maara = glob_data->tilesetit.at(tileset)->kerrokset[1]->tilemaara;
	for(y_piirto = 0; y_piirto < 15+y_juttu; y_piirto++)
	{

		for(int x_piirto = 0; x_piirto < 20+x_juttu; x_piirto++)
		{
			kohde_x = -(x_cam_2%32) + x_piirto*32;
			kohde_y = -(y_cam_2%32) + y_piirto*32;

			if(tausta_piirretaan[y_piirto*21+x_piirto])
			{	
				unsigned char tile = taustakerros[y_piirto*21+x_piirto];

				if(tile && (tile < t_maara))
				{

					bool avain;
					if(tilemaski_2[tile] & 2) avain = 1; else avain = 0;

					Anim_tile *anim = animaatiotaulukko[tile];

					if(anim)
					{
						int ruutu;
						if(anim->ruutu == 0) ruutu = tile;
						else ruutu = anim->ruudut.at(anim->ruutu-1);
						lahde.x = 1+(ruutu%20)*33;
						lahde.y = 1+(ruutu/20)*33;
					}
					else
					{
						lahde.x = 1+(tile%20)*33;
						lahde.y = 1+(tile/20)*33;
					
					}

					lahde.width = 32;
					lahde.height = 32;
					
					if(tilemaski_2[tile] & 4)
						glob_data->gfx->PiirraSprite(kuva, kohde_x,kohde_y, &lahde, avain, 128);
					else
					glob_data->gfx->PiirraSprite(kuva, kohde_x,kohde_y, &lahde, avain, 0);
				}
			}
		}

	}





	return 0;
}

