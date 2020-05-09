class Poiminta
{
private:
	Poiminta *next;
	int maara;
	bool seuraava;
	Peliobjekti *poimitut[1000];
public:
	Poiminta(){seuraava = 0; maara = 0;next = 0;}
	~Poiminta();
	bool Alusta();
	bool Lisaaobjekti(Peliobjekti *objekti);
	void Vapauta();
	Peliobjekti *annaAlkio(int *indeksi, Poiminta **pointteri);
};