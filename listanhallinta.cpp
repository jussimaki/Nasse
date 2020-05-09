#include "maaritykset.h"

bool UusiAlkio(std::vector < std::vector <void *> * > *lista, Listamaaritys *maaritys, int kohta)
{
	std::vector <void *> *alkio = new std::vector <void *>;
	
	for(unsigned int index = 0; index < maaritys->attribuutit.size(); index++)
	{
		void *attrib_data;
		std::vector < std::vector <void *> *> *vektorios;

		switch(maaritys->attribuutit.at(index)->tyyppi)
		{
		case attrib_LUKU:
			attrib_data = new int;
			*((int*)attrib_data) = ((MinMaxAlku*)(maaritys->attribuutit.at(index)->maarite))->alku;
			break;
		case attrib_LISTA:
			vektorios = new std::vector < std::vector <void *> *>;
			attrib_data = vektorios;
			break;
		case attrib_LINKKI:
			attrib_data = 0;
			break;
		}

		alkio->push_back(attrib_data);
	}
	if(kohta+1 > (int)lista->size()) lista->push_back(alkio);
	else lista->insert(lista->begin()+kohta+1,alkio);
	return true;
}



bool TuhoaLista(std::vector < std::vector <void *> * > *lista, Listamaaritys *maaritys, bool editori)
{
	for(unsigned int index = 0; index < lista->size(); index++)
	{
		for(unsigned int atr = 0; atr < maaritys->attribuutit.size(); atr++)
		{
			Editorilinkki *linkki;
			switch(maaritys->attribuutit.at(atr)->tyyppi)
			{
			case attrib_LUKU:
				delete (int*)(lista->at(index)->at(atr));
				break;
			case attrib_LISTA:
				TuhoaLista((std::vector < std::vector <void *> * > *)lista->at(index)->at(atr),
					(Listamaaritys *)maaritys->attribuutit.at(atr)->maarite, editori);

				delete (std::vector < std::vector <void *> * > *)lista->at(index)->at(atr);

				break;
			case attrib_LINKKI:
				linkki = (Editorilinkki*)(lista->at(index)->at(atr));
				if(linkki)
				{
					if(editori)
					{
						((Editoriobjekti*)linkki->mihin)->TuhoaLinkki(linkki);
						delete linkki;
					}
				}
				break;
			default: break;
			}
		}

		lista->at(index)->clear();
		delete lista->at(index);
	}

	return true;
}

bool TuhoaAlkio(std::vector < std::vector <void *> * > *lista, Listamaaritys *maaritys, int kohta)
{
	std::vector <void *> *alkio = lista->at(kohta);
	
	for(unsigned int index = 0; index < maaritys->attribuutit.size(); index++)
	{
		Editorilinkki *linkki;
		std::vector < std::vector <void *> *> *vektorios;
		switch(maaritys->attribuutit.at(index)->tyyppi)
		{
		case attrib_LUKU:
			delete (int*)(alkio->at(index));
			break;
		case attrib_LISTA:
			vektorios = (std::vector < std::vector <void *> *> *)alkio->at(index);

			TuhoaLista(vektorios, (Listamaaritys*)maaritys->attribuutit.at(index)->maarite,true);
			break;
		case attrib_LINKKI:
			linkki = (Editorilinkki*)(alkio->at(index));
			if(linkki)
			{
				// Tuhotaan osoitettavasta objektista tämä linkki
				((Editoriobjekti*)linkki->mihin)->TuhoaLinkki(linkki);
				delete linkki;
			}
			break;
		default: break;
		}
	}

	alkio->clear();
	delete alkio;
	lista->erase(lista->begin()+kohta);
	return true;
}
