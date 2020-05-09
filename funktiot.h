bool lataa_kuvat_binaari(GLOBAALI *glob_data, const char *datatiedosto);
bool lataa_kuvat(GLOBAALI *glob_data, const char *datatiedosto);
bool lataa_animaatiot(GLOBAALI *glob_data, const char *datatiedosto);
bool lataa_animaatiot_binaari(GLOBAALI *glob_data, const char *datatiedosto);
void vapauta_animaatiot(GLOBAALI *glob_data);
void vapauta_kuvat(GLOBAALI *glob_data);


bool lataa_biisit(GLOBAALI *glob_data, const char *datatiedosto);
bool lataa_aanet(GLOBAALI *glob_data, const char *datatiedosto);

bool lataa_biisit_binaari(GLOBAALI *glob_data, const char *datatiedosto);
bool lataa_aanet_binaari(GLOBAALI *glob_data, const char *datatiedosto);

void lataa_animoidut(std::ifstream &tiedosto, Layer *kerros,int oikeamaara);
void lataa_animoidut_binaari(std::ifstream &tiedosto, Layer *kerros,int oikeamaara);
void lataa_avaimet(std::ifstream &tiedosto, Layer *kerros, int oikeamaara);
void lataa_avaimet_binaari(std::ifstream &tiedosto, Layer *kerros, int oikeamaara);
void lataa_setti(GLOBAALI *glob_data,int numero);
void vapauta_setti(GLOBAALI *glob_data,int numero);
bool lataa_tilesetit(GLOBAALI *glob_data, const char *datatiedosto);
bool lataa_tilesetit_binaari(GLOBAALI *glob_data, const char *datatiedosto);
void vapauta_tilesetit(GLOBAALI *glob_data);

bool lataa_lista(std::ifstream &tiedosto, Listamaaritys *maaritys, GLOBAALI *glob_data);
bool lataa_objektityypit(const char *datatiedosto, const char *datatiedosto_2, GLOBAALI *glob_data);
bool lataa_lista_binaari(std::ifstream &tiedosto, Listamaaritys *maaritys, GLOBAALI *glob_data);
bool lataa_objektityypit_binaari(const char *datatiedosto, const char *datatiedosto_2, GLOBAALI *glob_data);
void tuhoa_listamaaritys(Listamaaritys *maaritys);
void vapauta_objektityypit(GLOBAALI *glob_data);

bool tallenna_configit(GLOBAALI *glob_data);
void lataa_configit(GLOBAALI *glob_data);
bool alustus(GLOBAALI *glob_data);
bool lopetus(GLOBAALI *glob_data);
