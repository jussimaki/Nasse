#define oMaara 53

#define Pelaaja_num 0
#define Jamppa_num 1
#define Palkki_num 2
#define KiinteaPalkki_num 3
#define Maali_num 4
#define Kolikko_num 5
#define Ovi_num 6
#define Lentoaija_num 7
#define Pomppija_num 8
#define Piikkityyppi_num 9
#define Aija_num 10
#define Amppari_num 11
#define Sylkija_num 12
#define Tulipallo_num 13
#define Sydan_num 14
#define Piikkipallo_num 15
#define Matkaaja_num 16
#define Pomppivapiikkipallo_num 17
#define Seilaavapiikkipallo_num 18
#define Bossi1_num 19
#define Kala_num 20
#define Lokki_num 21
#define Paikallaanhyppija_num 22
#define Seuraaja_num 23
#define Seuraavahyppija_num 24
#define Kala2_num 25
#define Meritahti_num 26
#define Lumipallonheittaja_num 27
#define Lumipallo_num 28
#define Lumipallonheittaja2_num 29
#define Laatikko_num 30
#define Palanen_num 31
#define Tulipallotyyppi_num 32
#define Avain_num 33
#define Ovi2_num 34
#define Matkapalkki_num 35
#define Pyorivapalkki_num 36
#define Jeppe_num 37
#define Siivet_num 38
#define Liekki_num 39
#define Liekinheitin_num 40
#define Tippukivi_num 41
#define Tippupalkki_num 42
#define Jaapuikko_num 43
#define Jousi_num 44
#define Checkpoint_num 45
#define Tykki_num 46
#define Bossi2_num 47
#define Tulipallo2_num 48
#define Bossi3_num 49
#define Bossi4_num 50
#define Lieska_num 51
#define Tulipallo3_num 52

struct Alkio
{
	char *tunnus;
	int numero;
};



const Alkio tunnukset[oMaara] =
{
	
	"Pelaaja", Pelaaja_num,
	"Jamppa", Jamppa_num,
	"Palkki", Palkki_num,
	"KiinteaPalkki", KiinteaPalkki_num,
	"Maali", Maali_num,
	"Kolikko", Kolikko_num,
	"Ovi", Ovi_num,
	"Lentoaija", Lentoaija_num,
	"Pomppija", Pomppija_num,
	"Piikkityyppi", Piikkityyppi_num,
	"Aija", Aija_num,
	"Amppari", Amppari_num,
	"Sylkija", Sylkija_num,
	"Tulipallo", Tulipallo_num,
	"Sydan", Sydan_num,
	"Piikkipallo", Piikkipallo_num,
	"Matkaaja", Matkaaja_num,
	"Pomppivapiikkipallo", Pomppivapiikkipallo_num,
	"Seilaavapiikkipallo", Seilaavapiikkipallo_num,
	"Bossi1", Bossi1_num,
	"Kala", Kala_num,
	"Lokki", Lokki_num,
	"Paikallaanhyppija", Paikallaanhyppija_num,
	"Seuraaja", Seuraaja_num,
	"Seuraavahyppija", Seuraavahyppija_num,
	"Kala2", Kala2_num,
	"Meritahti", Meritahti_num,
	"Lumipallonheittaja", Lumipallonheittaja_num,
	"Lumipallo", Lumipallo_num,
	"Lumipallonheittaja2", Lumipallonheittaja2_num,
	"Laatikko", Laatikko_num,
	"Palanen", Palanen_num,
	"Tulipallotyyppi", Tulipallotyyppi_num,
	"Avain", Avain_num,
	"Ovi2", Ovi2_num,
	"Matkapalkki", Matkapalkki_num,
	"Pyorivapalkki", Pyorivapalkki_num,
	"Jeppe", Jeppe_num,
	"Siivet", Siivet_num,
	"Liekki", Liekki_num,
	"Liekinheitin", Liekinheitin_num,
	"Tippukivi", Tippukivi_num,
	"Tippupalkki", Tippupalkki_num,
	"Jaapuikko", Jaapuikko_num,
	"Jousi", Jousi_num,
	"Checkpoint", Checkpoint_num,
	"Tykki", Tykki_num,
	"Bossi2", Bossi2_num,
	"Tulipallo2", Tulipallo2_num,
	"Bossi3", Bossi3_num,
	"Bossi4", Bossi4_num,
	"Lieska", Lieska_num,
	"Tulipallo3", Tulipallo3_num
};



Peliobjekti *luo_objekti(int numero, GLOBAALI *glob_data, Pelisektori *sektori);
void lataa_nimet(Alkio *tunnukset, GLOBAALI *glob_data, int lukumaara);
int yhdista_tyyppi(std::string tunnus, int counter, std::vector <Numerot> *luokkanimet);
