#include "maaritykset.h"

char *poimi_sana(char *osoitin, char **uusi_kohta)
{

	char *sana = 0;
	int index = 0;

	while(1)
	{
		if(osoitin[index] == '\0') {*uusi_kohta = &(osoitin[index]); return 0;}
		else if(osoitin[index] != ' ') {break;}
		index++;
	}

	int kohta = index;

	int pituus = 0;

	while(osoitin[index] != ' ' && osoitin[index] != '\0')
	{
		index++;
		pituus++;
	}

	*uusi_kohta = &(osoitin[index]);

	sana = new char[pituus+1];
	memcpy(sana,&(osoitin[kohta]),pituus*sizeof(char));
	sana[pituus] = '\0';

	
	return sana;
}

int vertaa_sanoja(char *eka, char *toka)
{
	int pituus_1 = 0, pituus_2 = 0;
	
	while(eka[pituus_1] != '\0') pituus_1++;
	while(toka[pituus_2] != '\0') pituus_2++;

	if(pituus_1 != pituus_2) return 0;
	
	for(int index = 0; index < pituus_1; index++)
		if(eka[index] != toka[index]) return 0;
	
	return 1;
}