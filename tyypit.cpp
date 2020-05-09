#include "maaritykset.h"

Peliobjekti *luo_objekti(int numero, GLOBAALI *glob_data, Pelisektori *sektori)
{
	Peliobjekti *objekti = 0;
	switch(numero)
	{
	case Jamppa_num: objekti = new Jamppa; break;
	case Aija_num: objekti = new Aija; break;
	case Palkki_num: objekti = new Palkki; break;
	case KiinteaPalkki_num: objekti = new KiinteaPalkki; break;
	case Maali_num: objekti = new Maali; break;
	case Kolikko_num: objekti = new Kolikko; break;
	case Ovi_num: objekti = new Ovi; break;
	case Lentoaija_num: objekti = new Lentoaija; break;
	case Pomppija_num: objekti = new Pomppija; break;
	case Pelaaja_num: objekti = new Pelaaja; break;
	case Piikkityyppi_num: objekti = new Piikkityyppi; break;
	case Amppari_num: objekti = new Amppari; break;
	case Sylkija_num: objekti = new Sylkija; break;
	case Tulipallo_num: objekti = new Tulipallo; break;
	case Sydan_num: objekti = new Sydan; break;
	case Piikkipallo_num: objekti = new Piikkipallo; break;
	case Matkaaja_num: objekti = new Matkaaja; break;
	case Pomppivapiikkipallo_num: objekti = new Pomppivapiikkipallo; break;
	case Seilaavapiikkipallo_num: objekti = new Seilaavapiikkipallo; break;
	case Bossi1_num: objekti = new Bossi1; break;
	case Kala_num: objekti = new Kala; break;
	case Lokki_num: objekti = new Lokki; break;
	case Paikallaanhyppija_num: objekti = new Paikallaanhyppija; break;
	case Seuraaja_num: objekti = new Seuraaja; break;
	case Seuraavahyppija_num: objekti = new Seuraavahyppija; break;
	case Kala2_num: objekti = new Kala2; break;
	case Meritahti_num: objekti = new Meritahti; break;
	case Lumipallonheittaja_num: objekti = new Lumipallonheittaja; break;
	case Lumipallo_num: objekti = new Lumipallo; break;
	case Lumipallonheittaja2_num: objekti = new Lumipallonheittaja2; break;
	case Laatikko_num: objekti = new Laatikko; break;
	case Palanen_num: objekti = new Palanen; break;
	case Tulipallotyyppi_num: objekti = new Tulipallotyyppi; break;
	case Avain_num: objekti = new Avain; break;
	case Ovi2_num: objekti = new Ovi2; break;
	case Matkapalkki_num: objekti = new Matkapalkki; break;
	case Pyorivapalkki_num: objekti = new Pyorivapalkki; break;
	case Jeppe_num: objekti = new Jeppe; break;
	case Siivet_num: objekti = new Siivet; break;
	case Liekki_num: objekti = new Liekki; break;
	case Liekinheitin_num: objekti = new Liekinheitin; break;
	case Tippukivi_num: objekti = new Tippukivi; break;
	case Tippupalkki_num: objekti = new Tippupalkki; break;
	case Jaapuikko_num: objekti = new Jaapuikko; break;
	case Jousi_num: objekti = new Jousi; break;
	case Checkpoint_num: objekti = new Checkpoint; break;
	case Tykki_num: objekti = new Tykki; break;
	case Bossi2_num: objekti = new Bossi2; break;
	case Tulipallo2_num: objekti = new Tulipallo2; break;
	case Bossi3_num: objekti = new Bossi3; break;
	case Bossi4_num: objekti = new Bossi4; break;
	case Lieska_num: objekti = new Lieska; break;
	case Tulipallo3_num: objekti = new Tulipallo3; break;
	default: break;
	}


	if(objekti)
	{
		int num = glob_data->tyyppinimet.at(numero).jarjestysnumero;
		objekti->AsetaTaulukkoNum(numero);
		objekti->AsetaTyyppi(num);
		objekti->AsetaLaatu(glob_data->objektityypit.at(num)->quadtree_num);
	
		if(glob_data->objektityypit.at(num)->ainaAktiivinen) objekti->AsetaAinaAktiivinen();
		else objekti->EiAinaAktiivinen();
	}

	return objekti;
}

void lataa_nimet(Alkio *tunnukset, GLOBAALI *glob_data, int lukumaara)
{
	glob_data->tyyppinimet.clear();

	for(int index = 0; index < lukumaara; index++)
	{
		std::string nimi(tunnukset[index].tunnus);

		Numerot num;
		num.nimi = nimi;
		num.objektinumero = tunnukset[index].numero;
		glob_data->tyyppinimet.push_back(num);
	}
}

int yhdista_tyyppi(std::string tunnus, int counter, std::vector <Numerot> *luokkanimet)
{



	for(int index = 0; index < luokkanimet->size(); index++)
	{
		if(luokkanimet->at(index).nimi == tunnus)
		{
			luokkanimet->at(index).jarjestysnumero = counter;
			return luokkanimet->at(index).objektinumero;
		}

	}

	return -1;
}
