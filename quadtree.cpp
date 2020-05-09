#include "maaritykset.h"

std::vector <Peliobjekti *> * Quadtree::AnnaObjektit() {return &objektit;}

int Quadtree::Objektimaara()
{
	if(yv) return yv->Objektimaara() + yo->Objektimaara() + av->Objektimaara() + ao->Objektimaara();

	else return objektit.size();
}

int Quadtree::annaQuadtreemaara()
{
	if(yv)
	{
		return 1 + yv->annaQuadtreemaara() + yo->annaQuadtreemaara()
			+ av->annaQuadtreemaara() + ao->annaQuadtreemaara();
	} else return 1;
}

void Quadtree::poistaYlimenneet(Poiminta *uusiksi)
{
	if(yv)
	{
		yv->poistaYlimenneet(uusiksi);
		yo->poistaYlimenneet(uusiksi);
		av->poistaYlimenneet(uusiksi);
		ao->poistaYlimenneet(uusiksi);
	}
	else
	{
		int koko = objektit.size();
		int index = 0;

		while(index < koko)
		{
			Peliobjekti *obj = objektit.at(index);
			int x = obj->AnnaSijaintiX();
			int y = obj->AnnaSijaintiY();

			if((x < alku_x ) || (x > loppu_x) || (y < alku_y) || (y > loppu_y))
			{
				uusiksi->Lisaaobjekti(obj);
				objektit.erase(objektit.begin() + index);
				koko--;
			} else index++;
		}
	}
}

void Quadtree::tuhoaVajaat()
{
	if(yv)
	{
		yv->tuhoaVajaat();
		yo->tuhoaVajaat();
		av->tuhoaVajaat();
		ao->tuhoaVajaat();
 
		if(yv->Objektimaara() + yo->Objektimaara() + av->Objektimaara() + ao->Objektimaara() < 9)
		{
			int index;
			std::vector <Peliobjekti *> *osoitin;

			osoitin = yv->AnnaObjektit();
			for(index = 0; index < osoitin->size(); index++)
			{
				objektit.push_back(osoitin->at(index));
			}

			osoitin = yo->AnnaObjektit();
			for(index = 0; index < osoitin->size(); index++)
			{
				objektit.push_back(osoitin->at(index));
			}

			osoitin = av->AnnaObjektit();
			for(index = 0; index < osoitin->size(); index++)
			{
				objektit.push_back(osoitin->at(index));
			}

			osoitin = ao->AnnaObjektit();
			for(index = 0; index < osoitin->size(); index++)
			{
				objektit.push_back(osoitin->at(index));
			}

			yv->Tuhoa();
			delete yv;
			yv = 0;

			yo->Tuhoa();
			delete yo;
			yo = 0;

			av->Tuhoa();
			delete av;
			av = 0;

			ao->Tuhoa();
			delete ao;
			ao = 0;
		}
	}
}

bool Quadtree::Aktiivisuus(int x_cam, int y_cam, int ax, int ay, int lx, int ly, GLOBAALI *glob_data, std::vector <Peliobjekti*> *kadonneet, Poiminta *logiikka, Pelisektori *sektori)
{
	int vasenloppu = (alku_x + loppu_x)/2;
	int oikeaalku = vasenloppu+1;

	int ylaloppu = (alku_y + loppu_y)/2;
	int alaalku = ylaloppu+1;

	if(yv)
	{
		if(ax <= vasenloppu && lx >= alku_x
			&& ay <= ylaloppu && ly >= alku_y) yv->Aktiivisuus(x_cam,y_cam,ax,ay,lx,ly,glob_data,kadonneet,logiikka,sektori);

		if(ax <= loppu_x && lx >= oikeaalku
			&& ay <= ylaloppu && ly >= alku_y) yo->Aktiivisuus(x_cam,y_cam,ax,ay,lx,ly,glob_data,kadonneet,logiikka,sektori);
		
		if(ax <= vasenloppu && lx >= alku_x
			&& ay < loppu_y && ly >= alaalku) av->Aktiivisuus(x_cam,y_cam,ax,ay,lx,ly,glob_data,kadonneet,logiikka,sektori);

		if(ax <= loppu_x && lx >= oikeaalku
			&& ay < loppu_y && ly >= alaalku) ao->Aktiivisuus(x_cam,y_cam,ax,ay,lx,ly,glob_data,kadonneet,logiikka,sektori);
			
	}
	else
	{
		int koko = objektit.size();
		int index = 0;

		while(index < koko)
		{
			
			if(objektit.at(index)->onkoAinaAktiivinen()) // nämä lisätään erikseen aina
			{					
				index++;
			}
			else if(objektit.at(index)->Aktiivisuustesti(x_cam, y_cam, glob_data, sektori))
			{
				kadonneet->push_back(objektit.at(index));
				objektit.erase(objektit.begin()+index);
				koko--;
			}
			else
			{
				if(objektit.at(index)->AnnaAktiivisuus())
				{
					logiikka->Lisaaobjekti(objektit.at(index));
				}
				index++;
			}
		}
	}

	return true;
}


bool Quadtree::poimiObjektit(int ax, int ay, int lx, int ly, Poiminta *poiminta, GLOBAALI *glob_data, bool piirto, int laatu)
{
	int vasenloppu = (alku_x + loppu_x)/2;
	int oikeaalku = vasenloppu+1;

	int ylaloppu = (alku_y + loppu_y)/2;
	int alaalku = ylaloppu+1;

	if(yv)
	{
		if(ax <= vasenloppu && lx >= alku_x
			&& ay <= ylaloppu && ly >= alku_y) yv->poimiObjektit(ax,ay,lx,ly,poiminta,glob_data,piirto,laatu);

		if(ax <= loppu_x && lx >= oikeaalku
			&& ay <= ylaloppu && ly >= alku_y) yo->poimiObjektit(ax,ay,lx,ly,poiminta,glob_data,piirto,laatu);
		
		if(ax <= vasenloppu && lx >= alku_x
			&& ay <= loppu_y && ly >= alaalku) av->poimiObjektit(ax,ay,lx,ly,poiminta,glob_data,piirto,laatu);

		if(ax <= loppu_x && lx >= oikeaalku
			&& ay <= loppu_y && ly >= alaalku) ao->poimiObjektit(ax,ay,lx,ly,poiminta,glob_data,piirto,laatu);
			
	}
	else
	{
		for(int index = 0; index < objektit.size(); index++)
		{
			Peliobjekti *obj = objektit.at(index);

			// aina aktiivisia objekteja ei poimita, paitsi piirtäessä
			if((!obj->onkoAinaAktiivinen()) || piirto)
			{
				if(obj->AnnaAktiivisuus())
				{
					int obj_x = obj->AnnaSijaintiX();
					int obj_y = obj->AnnaSijaintiY();
		
					int leveys = obj->AnnaLeveys(glob_data);
					int korkeus = obj->AnnaKorkeus(glob_data);
		
					if(obj_x + leveys-1 >= ax && obj_x < lx && obj_y + korkeus-1 >= ay && obj_y < ly)
						if(obj->AnnaLaatu() == laatu || laatu == -1) poiminta->Lisaaobjekti(obj);
				}
			}
		}
	}

	return true;
}

bool Quadtree::Tuhoa()
{
	objektit.clear();
	
	if(yv)
	{
		yv->Tuhoa();
		delete yv;
		yv = 0;

		yo->Tuhoa();
		delete yo;
		yo = 0;

		av->Tuhoa();
		delete av;
		av = 0;

		ao->Tuhoa();
		delete ao;
		ao = 0;
	}

	return true;
}

bool Quadtree::Alusta(int ax, int ay, int lx, int ly)
{
	alku_x = ax;
	alku_y = ay;
	loppu_x = lx;
	loppu_y = ly;

	yv = 0;
	yo = 0;
	av = 0;
	ao = 0;

	objektit.clear();

	return true;
};

bool Quadtree::Sijoita(Peliobjekti *objekti)
{
	int vasenloppu = (alku_x + loppu_x)/2;
	int oikeaalku = vasenloppu+1;

	int ylaloppu = (alku_y + loppu_y)/2;
	int alaalku = ylaloppu+1;

	int obj_x = objekti->AnnaSijaintiX();
	int obj_y = objekti->AnnaSijaintiY();

	if(obj_x <= vasenloppu)
	{
		if(obj_y <= ylaloppu) yv->LisaaObjekti(objekti);
		else av->LisaaObjekti(objekti);
	}
	else
	{
		if(obj_y <= ylaloppu) yo->LisaaObjekti(objekti);
		else ao->LisaaObjekti(objekti);
	}

	return true;
}


bool Quadtree::LisaaObjekti(Peliobjekti *objekti)
{
	int vasenloppu = (alku_x + loppu_x)/2;
	int oikeaalku = vasenloppu+1;

	int ylaloppu = (alku_y + loppu_y)/2;
	int alaalku = ylaloppu+1;

	int x = objekti->AnnaSijaintiX();
	int y = objekti->AnnaSijaintiY();
	int index;

	
	if(!yv && objektit.size() >= 8)
	{
		
		if((loppu_x - alku_x > 20) && (loppu_y - alku_y > 20))
		{
			yv = new Quadtree;
			yv->Alusta(alku_x, alku_y, vasenloppu, ylaloppu);

			yo = new Quadtree;
			yo->Alusta(oikeaalku, alku_y, loppu_x, ylaloppu);
	
			av = new Quadtree;
			av->Alusta(alku_x, alaalku, vasenloppu, loppu_y);
	
			ao = new Quadtree;
			ao->Alusta(oikeaalku, alaalku, loppu_x, loppu_y);

			for(index = 0; index < objektit.size(); index++) Sijoita(objektit.at(index));
			
			objektit.clear();
		}
	}

	if(yv) // Quadtree jaettu neljään osaan
	{
		
		Sijoita(objekti);
	}
	else
	{
		
		objektit.push_back(objekti);
	}

	return true;
}


void Quadtree::tuhoustesti()
{
	if(yv)
	{
		yv->tuhoustesti();
		yo->tuhoustesti();
		av->tuhoustesti();
		ao->tuhoustesti();
	}
	else
	{
		int koko = objektit.size();
		int index = 0;

		while(index < koko)
		{
			if(objektit.at(index)->OnkoTuhottu())
			{
				objektit.erase(objektit.begin()+index);
				koko--;
			} else index++;
		}
	}
}