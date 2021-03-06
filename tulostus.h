int Tulosta(char *teksti, int x, int y, GLOBAALI *glob_data);

const int kirjaintaulukko[] =
{
	0, 0, 5, 11,
	5, 0, 3, 11,
	8, 0, 6, 5,
	14, 0, 9, 11,
	23, 0, 7, 12,
	30, 0, 12, 11,
	42, 0, 9, 11,
	51, 0, 5, 4,
	56, 0, 3, 14,
	59, 0, 4, 15,
	63, 0, 7, 6,
	70, 0, 7, 10,
	77, 0, 4, 12,
	81, 0, 5, 7,
	86, 0, 3, 11,
	89, 0, 5, 13,
	94, 0, 8, 11,
	102, 0, 8, 11,
	110, 0, 8, 11,
	118, 0, 8, 11,
	126, 0, 8, 11,
	134, 0, 8, 11,
	142, 0, 8, 11,
	150, 0, 8, 11,
	158, 0, 8, 11,
	166, 0, 8, 11,
	174, 0, 3, 11,
	177, 0, 4, 12,
	0, 16, 9, 11,
	9, 16, 8, 8,
	17, 16, 8, 11,
	25, 16, 8, 11,
	33, 16, 13, 13,
	46, 16, 9, 11,
	55, 16, 10, 11,
	65, 16, 9, 11,
	74, 16, 10, 11,
	84, 16, 9, 11,
	93, 16, 8, 11,
	101, 16, 10, 11,
	111, 16, 10, 11,
	121, 16, 3, 11,
	124, 16, 8, 11,
	132, 16, 9, 11,
	141, 16, 8, 11,
	149, 16, 12, 11,
	161, 16, 10, 11,
	171, 16, 10, 11,
	181, 16, 9, 11,
	190, 16, 10, 11,
	200, 16, 10, 11,
	210, 16, 8, 11,
	218, 16, 9, 11,
	227, 16, 9, 11,
	236, 16, 8, 11,
	244, 16, 14, 11,
	258, 16, 9, 11,
	267, 16, 10, 11,
	277, 16, 10, 11,
	287, 16, 3, 14,
	290, 16, 4, 13,
	294, 16, 4, 14,
	298, 16, 5, 4,
	303, 16, 9, 14,
	312, 16, 4, 3,

	0, 32, 8, 11,
	8, 32, 8, 11,
	16, 32, 8, 11,
	24, 32, 7, 11,
	31, 32, 8, 11,
	39, 32, 5, 11,
	44, 32, 7, 14,
	51, 32, 8, 11,
	59, 32, 4, 11,
	63, 32, 4, 14,
	67, 32, 8, 11,
	75, 32, 3, 11,
	78, 32, 12, 11,
	90, 32, 8, 11,
	98, 32, 8, 11,
	106, 32, 8, 14,
	114, 32, 8, 14,
	122, 32, 6, 11,
	128, 32, 7, 11,
	135, 32, 5, 11,
	140, 32, 7, 11,
	147, 32, 8, 11,
	155, 32, 10, 11,
	165, 32, 8, 11,
	173, 32, 9, 14,
	182, 32, 7, 11,
	189, 32, 6, 14,
	195, 32, 3, 14,
	198, 32, 5, 14,
	203, 32, 6, 3
};