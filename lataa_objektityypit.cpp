#include "maaritykset.h"

bool lataa_lista(std::ifstream &tiedosto, Listamaaritys *maaritys, GLOBAALI *glob_data)
{
	int lista_attribuutit;
	tiedosto >> lista_attribuutit;
	
	for(int index = 0; index < lista_attribuutit; index++)
	{
		Attribuutti *uusi = new Attribuutti;
		tiedosto >> uusi->nimi;
		std::string tyyppi;
		tiedosto >> tyyppi;

		if(tyyppi == "luku")
		{
			uusi->tyyppi = attrib_LUKU;
			uusi->maarite = new MinMaxAlku;
			tiedosto >> ((MinMaxAlku*)uusi->maarite)->min;
			tiedosto >> ((MinMaxAlku*)uusi->maarite)->max;
			tiedosto >> ((MinMaxAlku*)uusi->maarite)->alku;
		}
		else if(tyyppi == "merkkijono")
		{
			uusi->tyyppi = attrib_MJONO;
			uusi->maarite = new Listamaaritys;
		}
		else if(tyyppi == "lista")
		{
			uusi->tyyppi = attrib_LISTA;
			uusi->maarite = new Listamaaritys;
			lataa_lista(tiedosto, (Listamaaritys*)uusi->maarite, glob_data);
		}
		else if(tyyppi == "linkki")
		{
			uusi->tyyppi = attrib_LINKKI;
			uusi->maarite = new Linkkimaaritys;
			int lkm;
			tiedosto >> lkm;

			int indeksi;
			for(indeksi = 0; indeksi < lkm; indeksi++)
			{
				std::string nimi;
				tiedosto >> nimi;
				((Linkkimaaritys*)uusi->maarite)->tyyppinimet.push_back(nimi);
			}
			
			glob_data->linkkimaaritykset.push_back((Linkkimaaritys*)uusi->maarite);
		}

		maaritys->attribuutit.push_back(uusi);
	}

	return true;
}

bool lataa_objektityypit(const char *datatiedosto, const char *datatiedosto_2, GLOBAALI *glob_data)
{
	glob_data->objektityypit.clear();
	int counter = 0;
	int index;

	std::ifstream tiedosto(datatiedosto);
	std::string rivi = "";

	if(tiedosto)
	{
		std::string tyyppinimi;

		int lukumaara;
		tiedosto >> lukumaara;

		for(int tyyppi = 0; tyyppi < lukumaara; tyyppi++)
		{
			tiedosto >> tyyppinimi;
			Objektityyppi *uusi_tyyppi = new Objektityyppi;
			if(tyyppinimi == "x")
			{
				uusi_tyyppi->vanhentunut = 1;
				tiedosto >> tyyppinimi;
			}
			else uusi_tyyppi->vanhentunut = 0;

			uusi_tyyppi->tyyppinumero = yhdista_tyyppi(tyyppinimi, counter, &(glob_data->tyyppinimet));
			if(uusi_tyyppi->tyyppinumero == -1) return false;

			
			uusi_tyyppi->attribuutit.clear();
			uusi_tyyppi->nimi = tyyppinimi;

			int quadtree_num;
			tiedosto >> quadtree_num;
			uusi_tyyppi->quadtree_num = quadtree_num;

			int ainaAktiivinen;
			tiedosto >> ainaAktiivinen;
			uusi_tyyppi->ainaAktiivinen =  ainaAktiivinen;

			int piirtojarjestys;
			tiedosto >> piirtojarjestys;
			uusi_tyyppi->piirtojarjestys = piirtojarjestys;

			int paallehyppy;
			tiedosto >> paallehyppy;
			uusi_tyyppi->paallehyppy = paallehyppy;

			int damagenotto;
			tiedosto >> damagenotto;
			uusi_tyyppi->damagenotto = damagenotto;

			int leveys, korkeus, x_offset, y_offset;
			tiedosto >> leveys;
			tiedosto >> korkeus;
			tiedosto >> x_offset;
			tiedosto >> y_offset;

			uusi_tyyppi->leveys = leveys;
			uusi_tyyppi->korkeus = korkeus;
			uusi_tyyppi->x_offset = x_offset;
			uusi_tyyppi->y_offset = y_offset;

			std::string kuvanimi;
			tiedosto >> kuvanimi;
			
			std::map <std::string, Sprite *>::iterator sprite_iter;

			sprite_iter = glob_data->spritemappi.find(kuvanimi);
			if(sprite_iter == glob_data->spritemappi.end()) return false;
			uusi_tyyppi->kuvake = (*sprite_iter).second;

			

			tiedosto >> uusi_tyyppi->x_koord;
			tiedosto >> uusi_tyyppi->y_koord;
			

			int attribuutit;
			tiedosto >> attribuutit;

			for(index = 0; index < attribuutit; index++)
			{
				Attribuutti *attrib = new Attribuutti;
				tiedosto >> attrib->nimi;

				std::string tyyppi;
				tiedosto >> tyyppi;

				if(tyyppi == "luku")
				{
					attrib->tyyppi = attrib_LUKU;

					attrib->maarite = new MinMaxAlku;
					tiedosto >> ((MinMaxAlku*)attrib->maarite)->min;
					tiedosto >> ((MinMaxAlku*)attrib->maarite)->max;
					tiedosto >> ((MinMaxAlku*)attrib->maarite)->alku;
				}
				else if(tyyppi == "merkkijono")
				{
					attrib->tyyppi = attrib_MJONO;

					
				}
				else if(tyyppi == "lista")
				{
					attrib->tyyppi = attrib_LISTA;
					attrib->maarite = new Listamaaritys;
					lataa_lista(tiedosto, (Listamaaritys*)attrib->maarite, glob_data);
				}
				else if(tyyppi == "linkki")
				{
					attrib->tyyppi = attrib_LINKKI;
					attrib->maarite = new Linkkimaaritys;
					int lkm;
					tiedosto >> lkm;

					int indeksi;
					for(indeksi = 0; indeksi < lkm; indeksi++)
					{
						std::string nimi;
						tiedosto >> nimi;
						((Linkkimaaritys*)attrib->maarite)->tyyppinimet.push_back(nimi);
					}
					glob_data->linkkimaaritykset.push_back((Linkkimaaritys*)attrib->maarite);
				}

				uusi_tyyppi->attribuutit.push_back(attrib);
			}

			int kuvat;
			tiedosto >> kuvat;

			for(index = 0; index < kuvat; index++)
			{
				std::string kuvanimi;
				tiedosto >> kuvanimi;

				std::map <std::string, Sprite *>::iterator kuva_iter;
				kuva_iter = glob_data->spritemappi.find(kuvanimi);
				if(kuva_iter == glob_data->spritemappi.end()) return false;
				
				uusi_tyyppi->spritet.push_back((*kuva_iter).second);
			}


			int animaatiot;
			tiedosto >> animaatiot;

			for(index = 0; index < animaatiot; index++)
			{
				std::string animaationimi;
				tiedosto >> animaationimi;

				std::map <std::string, Animaatio *>::iterator anim_iter;
				anim_iter = glob_data->animaatiomappi.find(animaationimi);
				if(anim_iter == glob_data->animaatiomappi.end()) return false;
				
				uusi_tyyppi->animaatiot.push_back((*anim_iter).second);

			}

			glob_data->objektityypit.push_back(uusi_tyyppi);
			counter++;

		}		
	}

	tiedosto.close();

	glob_data->editoriobjektit = counter;

	std::ifstream tiedosto_2(datatiedosto_2);

	if(tiedosto_2)
	{
		std::string tyyppinimi;

		int lukumaara;
		tiedosto_2 >> lukumaara;

		for(int tyyppi = 0; tyyppi < lukumaara; tyyppi++)
		{
			tiedosto_2 >> tyyppinimi;
			Objektityyppi *uusi_tyyppi = new Objektityyppi;
			if(tyyppinimi == "x")
			{
				uusi_tyyppi->vanhentunut = 1;
				tiedosto_2 >> tyyppinimi;
			}
			else uusi_tyyppi->vanhentunut = 0;

			uusi_tyyppi->tyyppinumero = yhdista_tyyppi(tyyppinimi, counter, &(glob_data->tyyppinimet));
			if(uusi_tyyppi->tyyppinumero == -1) return false;

			
			uusi_tyyppi->attribuutit.clear();

			uusi_tyyppi->nimi = tyyppinimi;

			int quadtree_num;

			tiedosto_2 >> quadtree_num;

			uusi_tyyppi->quadtree_num = quadtree_num;

			uusi_tyyppi->ainaAktiivinen = 0;

			int piirtojarjestys;
			tiedosto_2 >> piirtojarjestys;
			uusi_tyyppi->piirtojarjestys = piirtojarjestys;
			
			int paallehyppy;
			tiedosto_2 >> paallehyppy;
			uusi_tyyppi->paallehyppy = paallehyppy;

			int damagenotto;
			tiedosto_2 >> damagenotto;
			uusi_tyyppi->damagenotto = damagenotto;

			int leveys, korkeus, x_offset, y_offset;
			tiedosto_2 >> leveys;
			tiedosto_2 >> korkeus;
			tiedosto_2 >> x_offset;
			tiedosto_2 >> y_offset;

			uusi_tyyppi->leveys = leveys;
			uusi_tyyppi->korkeus = korkeus;
			uusi_tyyppi->x_offset = x_offset;
			uusi_tyyppi->y_offset = y_offset;

			uusi_tyyppi->kuvake = 0;

			

			uusi_tyyppi->x_koord = 0;
			uusi_tyyppi->y_koord = 0;
			
			int kuvat;
			tiedosto_2 >> kuvat;

			int index;
			for(index = 0; index < kuvat; index++)
			{
				std::string kuvanimi;
				tiedosto_2 >> kuvanimi;

				std::map <std::string, Sprite *>::iterator kuva_iter;
				kuva_iter = glob_data->spritemappi.find(kuvanimi);
				if(kuva_iter == glob_data->spritemappi.end()) return false;
				
				uusi_tyyppi->spritet.push_back((*kuva_iter).second);
			}


			int animaatiot;
			tiedosto_2 >> animaatiot;

			for(index = 0; index < animaatiot; index++)
			{
				std::string animaationimi;
				tiedosto_2 >> animaationimi;

				std::map <std::string, Animaatio *>::iterator anim_iter;
				anim_iter = glob_data->animaatiomappi.find(animaationimi);
				if(anim_iter == glob_data->animaatiomappi.end()) return false;
				
				uusi_tyyppi->animaatiot.push_back((*anim_iter).second);

			}

			glob_data->objektityypit.push_back(uusi_tyyppi);
			counter++;

		}		
	} else return false;

	tiedosto_2.close();

	for(index = 0; index < glob_data->linkkimaaritykset.size(); index++)
	{
		int koko = glob_data->linkkimaaritykset.at(index)->tyyppinimet.size();

		for(int index2 = 0; index2 < koko; index2++)
		{
			std::string nimi = glob_data->linkkimaaritykset.at(index)->tyyppinimet.at(index2);

			int koko2 = glob_data->objektityypit.size();
			bool loytyi = 0;

			for(int index3 = 0; index3 < koko2; index3++)
			{
				if(glob_data->objektityypit.at(index3)->nimi == nimi)
				{
					glob_data->linkkimaaritykset.at(index)->tyyppinumerot.push_back(index3);
					loytyi = 1;
					break;
				}
			}

			if(!loytyi) return false;
		}
	}

	return true;
}



bool lataa_lista_binaari(std::ifstream &tiedosto, Listamaaritys *maaritys, GLOBAALI *glob_data)
{
	int lista_attribuutit;
	tiedosto.read((char*)&lista_attribuutit,sizeof(int));
	
	
	for(int index = 0; index < lista_attribuutit; index++)
	{
		Attribuutti *uusi = new Attribuutti;
		LueString(&(uusi->nimi),tiedosto);
		
		std::string tyyppi;
		LueString(&(tyyppi),tiedosto);
		
		if(tyyppi == "luku")
		{
			uusi->tyyppi = attrib_LUKU;
			uusi->maarite = new MinMaxAlku;

			tiedosto.read((char*)&(((MinMaxAlku*)uusi->maarite)->min),sizeof(int));
			tiedosto.read((char*)&(((MinMaxAlku*)uusi->maarite)->max),sizeof(int));
			tiedosto.read((char*)&(((MinMaxAlku*)uusi->maarite)->alku),sizeof(int));
		}
		else if(tyyppi == "merkkijono")
		{
			uusi->tyyppi = attrib_MJONO;
			uusi->maarite = new Listamaaritys;
		}
		else if(tyyppi == "lista")
		{
			uusi->tyyppi = attrib_LISTA;
			uusi->maarite = new Listamaaritys;
			lataa_lista_binaari(tiedosto, (Listamaaritys*)uusi->maarite, glob_data);
		}
		else if(tyyppi == "linkki")
		{
			uusi->tyyppi = attrib_LINKKI;
			uusi->maarite = new Linkkimaaritys;
			int lkm;
			tiedosto.read((char*)&lkm,sizeof(int));

			int indeksi;
			for(indeksi = 0; indeksi < lkm; indeksi++)
			{
				std::string nimi;
				LueString(&(nimi),tiedosto);
				((Linkkimaaritys*)uusi->maarite)->tyyppinimet.push_back(nimi);
			}
			
			glob_data->linkkimaaritykset.push_back((Linkkimaaritys*)uusi->maarite);
		}

		maaritys->attribuutit.push_back(uusi);
	}

	return true;
}



bool lataa_objektityypit_binaari(const char *datatiedosto, const char *datatiedosto_2, GLOBAALI *glob_data)
{
	glob_data->objektityypit.clear();
	int counter = 0;
	int index;

	std::ifstream tiedosto(datatiedosto, std::ios_base::binary);
	std::string rivi = "";

	if(tiedosto)
	{
		std::string tyyppinimi;

		int lukumaara;
		tiedosto.read((char*)&lukumaara,sizeof(int));
		
		for(int tyyppi = 0; tyyppi < lukumaara; tyyppi++)
		{
			
			LueString(&tyyppinimi,tiedosto);
			Objektityyppi *uusi_tyyppi = new Objektityyppi;
			if(tyyppinimi == "x")
			{
				uusi_tyyppi->vanhentunut = 1;
				LueString(&tyyppinimi,tiedosto);
			}
			else uusi_tyyppi->vanhentunut = 0;

			uusi_tyyppi->tyyppinumero = yhdista_tyyppi(tyyppinimi, counter, &(glob_data->tyyppinimet));
			if(uusi_tyyppi->tyyppinumero == -1) return false;

			
			uusi_tyyppi->attribuutit.clear();
			uusi_tyyppi->nimi = tyyppinimi;

			int quadtree_num;

			
			tiedosto.read((char*)&quadtree_num,sizeof(int));

			uusi_tyyppi->quadtree_num = quadtree_num;

			int ainaAktiivinen;
			tiedosto.read((char*)&ainaAktiivinen,sizeof(int));
			uusi_tyyppi->ainaAktiivinen = ainaAktiivinen;

			int piirtojarjestys;
			tiedosto.read((char*)&piirtojarjestys,sizeof(int));
			uusi_tyyppi->piirtojarjestys = piirtojarjestys;

			int paallehyppy;
			tiedosto.read((char*)&paallehyppy,sizeof(int));
			uusi_tyyppi->paallehyppy = paallehyppy;

			int damagenotto;
			tiedosto.read((char*)&damagenotto,sizeof(int));
			uusi_tyyppi->damagenotto = damagenotto;

			int leveys, korkeus, x_offset, y_offset;
			
			tiedosto.read((char*)&leveys,sizeof(int));
			tiedosto.read((char*)&korkeus,sizeof(int));
			tiedosto.read((char*)&x_offset,sizeof(int));
			tiedosto.read((char*)&y_offset,sizeof(int));

			uusi_tyyppi->leveys = leveys;
			uusi_tyyppi->korkeus = korkeus;
			uusi_tyyppi->x_offset = x_offset;
			uusi_tyyppi->y_offset = y_offset;

			std::string kuvanimi;
			LueString(&kuvanimi,tiedosto);
			
			std::map <std::string, Sprite *>::iterator sprite_iter;

			sprite_iter = glob_data->spritemappi.find(kuvanimi);
			if(sprite_iter == glob_data->spritemappi.end()) return false;
			uusi_tyyppi->kuvake = (*sprite_iter).second;


			tiedosto.read((char*)&(uusi_tyyppi->x_koord),sizeof(int));
			tiedosto.read((char*)&(uusi_tyyppi->y_koord),sizeof(int));
			

			int attribuutit;
			tiedosto.read((char*)&attribuutit,sizeof(int));

			for(index = 0; index < attribuutit; index++)
			{
				Attribuutti *attrib = new Attribuutti;
				LueString(&(attrib->nimi),tiedosto);
				
				std::string tyyppi;
				LueString(&tyyppi,tiedosto);

				if(tyyppi == "luku")
				{
					attrib->tyyppi = attrib_LUKU;

					attrib->maarite = new MinMaxAlku;
					tiedosto.read((char*)&(((MinMaxAlku*)attrib->maarite)->min),sizeof(int));
					tiedosto.read((char*)&(((MinMaxAlku*)attrib->maarite)->max),sizeof(int));
					tiedosto.read((char*)&(((MinMaxAlku*)attrib->maarite)->alku),sizeof(int));
				}
				else if(tyyppi == "merkkijono")
				{
					attrib->tyyppi = attrib_MJONO;

					
				}
				else if(tyyppi == "lista")
				{
					attrib->tyyppi = attrib_LISTA;
					attrib->maarite = new Listamaaritys;
					lataa_lista_binaari(tiedosto, (Listamaaritys*)attrib->maarite, glob_data);
				}
				else if(tyyppi == "linkki")
				{
					attrib->tyyppi = attrib_LINKKI;
					attrib->maarite = new Linkkimaaritys;
					int lkm;
					tiedosto.read((char*)&lkm,sizeof(int));
					
					int indeksi;
					for(indeksi = 0; indeksi < lkm; indeksi++)
					{
						std::string nimi;
						LueString(&nimi,tiedosto);
						
						((Linkkimaaritys*)attrib->maarite)->tyyppinimet.push_back(nimi);
					}
					glob_data->linkkimaaritykset.push_back((Linkkimaaritys*)attrib->maarite);
				}

				uusi_tyyppi->attribuutit.push_back(attrib);
			}

			int kuvat;
			tiedosto.read((char*)&kuvat,sizeof(int));

			for(index = 0; index < kuvat; index++)
			{
				std::string kuvanimi;
				LueString(&kuvanimi,tiedosto);

				std::map <std::string, Sprite *>::iterator kuva_iter;
				kuva_iter = glob_data->spritemappi.find(kuvanimi);
				if(kuva_iter == glob_data->spritemappi.end()) return false;
				
				uusi_tyyppi->spritet.push_back((*kuva_iter).second);
			}


			int animaatiot;
			
			tiedosto.read((char*)&animaatiot,sizeof(int));

			for(index = 0; index < animaatiot; index++)
			{
				std::string animaationimi;
				LueString(&animaationimi,tiedosto);

				std::map <std::string, Animaatio *>::iterator anim_iter;
				anim_iter = glob_data->animaatiomappi.find(animaationimi);
				if(anim_iter == glob_data->animaatiomappi.end()) return false;
				
				uusi_tyyppi->animaatiot.push_back((*anim_iter).second);

			}

			glob_data->objektityypit.push_back(uusi_tyyppi);
			counter++;

		}		
	}

	tiedosto.close();

	glob_data->editoriobjektit = counter;

	std::ifstream tiedosto2(datatiedosto_2, std::ios_base::binary);

	if(tiedosto2)
	{
		std::string tyyppinimi;

		int lukumaara;
		tiedosto2.read((char*)&lukumaara,sizeof(int));

		for(int tyyppi = 0; tyyppi < lukumaara; tyyppi++)
		{
			LueString(&tyyppinimi, tiedosto2);
			
			Objektityyppi *uusi_tyyppi = new Objektityyppi;
			if(tyyppinimi == "x")
			{
				uusi_tyyppi->vanhentunut = 1;
				LueString(&tyyppinimi, tiedosto2);
			}
			else uusi_tyyppi->vanhentunut = 0;

			uusi_tyyppi->tyyppinumero = yhdista_tyyppi(tyyppinimi, counter, &(glob_data->tyyppinimet));
			if(uusi_tyyppi->tyyppinumero == -1) return false;

			
			uusi_tyyppi->attribuutit.clear();

			uusi_tyyppi->nimi = tyyppinimi;

			int quadtree_num;

			tiedosto2.read((char*)&quadtree_num,sizeof(int));

			uusi_tyyppi->quadtree_num = quadtree_num;

			uusi_tyyppi->ainaAktiivinen = 0;

			int piirtojarjestys;
			tiedosto2.read((char*)&piirtojarjestys,sizeof(int));
			uusi_tyyppi->piirtojarjestys = piirtojarjestys;
			
			int paallehyppy;
			tiedosto2.read((char*)&paallehyppy,sizeof(int));
			uusi_tyyppi->paallehyppy = paallehyppy;

			int damagenotto;
			tiedosto2.read((char*)&damagenotto,sizeof(int));
			uusi_tyyppi->damagenotto = damagenotto;

			int leveys, korkeus, x_offset, y_offset;

			tiedosto2.read((char*)&leveys,sizeof(int));
			tiedosto2.read((char*)&korkeus,sizeof(int));
			tiedosto2.read((char*)&x_offset,sizeof(int));
			tiedosto2.read((char*)&y_offset,sizeof(int));

			
			uusi_tyyppi->leveys = leveys;
			uusi_tyyppi->korkeus = korkeus;
			uusi_tyyppi->x_offset = x_offset;
			uusi_tyyppi->y_offset = y_offset;

			uusi_tyyppi->kuvake = 0;

			

			uusi_tyyppi->x_koord = 0;
			uusi_tyyppi->y_koord = 0;
			
			int kuvat;
			tiedosto2.read((char*)&kuvat,sizeof(int));

			int index;
			for(index = 0; index < kuvat; index++)
			{
				std::string kuvanimi;
				LueString(&kuvanimi, tiedosto2);

				std::map <std::string, Sprite *>::iterator kuva_iter;
				kuva_iter = glob_data->spritemappi.find(kuvanimi);
				if(kuva_iter == glob_data->spritemappi.end()) return false;
				
				uusi_tyyppi->spritet.push_back((*kuva_iter).second);
			}


			int animaatiot;
			tiedosto2.read((char*)&animaatiot,sizeof(int));

			for(index = 0; index < animaatiot; index++)
			{
				std::string animaationimi;
				LueString(&animaationimi, tiedosto2);

				std::map <std::string, Animaatio *>::iterator anim_iter;
				anim_iter = glob_data->animaatiomappi.find(animaationimi);
				if(anim_iter == glob_data->animaatiomappi.end()) return false;
				
				uusi_tyyppi->animaatiot.push_back((*anim_iter).second);

			}

			glob_data->objektityypit.push_back(uusi_tyyppi);
			counter++;

		}		
	} else return false;

	tiedosto2.close();

	for(index = 0; index < glob_data->linkkimaaritykset.size(); index++)
	{
		int koko = glob_data->linkkimaaritykset.at(index)->tyyppinimet.size();

		for(int index2 = 0; index2 < koko; index2++)
		{
			std::string nimi = glob_data->linkkimaaritykset.at(index)->tyyppinimet.at(index2);

			int koko2 = glob_data->objektityypit.size();
			bool loytyi = 0;

			for(int index3 = 0; index3 < koko2; index3++)
			{
				if(glob_data->objektityypit.at(index3)->nimi == nimi)
				{
					glob_data->linkkimaaritykset.at(index)->tyyppinumerot.push_back(index3);
					loytyi = 1;
					break;
				}
			}

			if(!loytyi) return false;
		}
	}

	return true;
}


void tuhoa_listamaaritys(Listamaaritys *maaritys)
{
	for(int attribuutti_nro = 0; attribuutti_nro < maaritys->attribuutit.size(); attribuutti_nro++)
	{
		Attribuutti *attribuutti = maaritys->attribuutit.at(attribuutti_nro);

		switch(attribuutti->tyyppi)
		{
		case attrib_LUKU:
			delete (MinMaxAlku*)attribuutti->maarite;
			break;
		case attrib_LISTA:

			tuhoa_listamaaritys((Listamaaritys*)attribuutti->maarite);
			delete (Listamaaritys*)attribuutti->maarite;
			break;
		default: break;
		}

		delete attribuutti;
	}

	maaritys->attribuutit.clear();
}

void vapauta_objektityypit(GLOBAALI *glob_data)
{
	for(int tyyppi_nro = 0; tyyppi_nro < glob_data->objektityypit.size(); tyyppi_nro++)
	{
		Objektityyppi *tyyppi = glob_data->objektityypit.at(tyyppi_nro);

		for(int attribuutti_nro = 0; attribuutti_nro < tyyppi->attribuutit.size(); attribuutti_nro++)
		{

			Attribuutti *attribuutti = tyyppi->attribuutit.at(attribuutti_nro);
			
			switch(attribuutti->tyyppi)
			{
			case attrib_LUKU:
				delete (MinMaxAlku*)attribuutti->maarite;
				break;
			case attrib_LISTA:

				tuhoa_listamaaritys((Listamaaritys*)attribuutti->maarite);
				delete (Listamaaritys*)attribuutti->maarite;
				break;
			default: break;

			}

			delete attribuutti;

			tyyppi->attribuutit.clear();
		}

		delete tyyppi;
	}

	glob_data->objektityypit.clear();
}