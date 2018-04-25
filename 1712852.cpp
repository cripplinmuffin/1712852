#include <stdio.h>
#include <fcntl.h> 
#include <io.h>   
#include <stdlib.h>
#include <string.h>
#include <codecvt> 
#include<wchar.h>
using namespace std;

typedef struct SinhVien
{
	wchar_t id[20];
	wchar_t name[30];
	wchar_t major[30];
	int area;
	wchar_t bday[20];
	wchar_t pic[100];
	wchar_t desc[1000];
	wchar_t hobby[10][200];
	int x;
}SV;

int sosv(FILE* fin)
{
	int dem = 0;
	wchar_t c[2000];
	do
	{
		fgetws(c, 2000, fin);
		dem++;
	} while (!feof(fin));
	rewind(fin);
	return dem;
}

void tachkhoa(FILE* fin, int& s)
{
	fseek(fin, 2L, SEEK_CUR);
	fwscanf(fin, L"%d", &s);
	fseek(fin, 1L, SEEK_CUR);
}

void tachnoidung(FILE* fin, wchar_t content[])
{
	int i = 0;
	wchar_t c;
	fseek(fin, 2L, SEEK_CUR);
	while (1)
	{
		c = fgetwc(fin);
		if (c == L'"' || c == L'\n' || c == EOF) break;
		else
		{
			content[i] = c;
			i++;
		}
	}
	content[i] = '\0';
}

int sohob(FILE*fin)
{
	int dem = 0;
	wchar_t c;
	while (!feof(fin))
	{
		c = fgetwc(fin);
		if (c == L'"') dem++;
		if (c == '\n') break;
	}
	return (dem / 2) - 7;
}

void chiahob(FILE* fin, SV sv[], int n)
{
	for (int i = 0; i < n; i++)
	{
		sv[i].x = sohob(fin);
	}
	rewind(fin);
}

void gannoidung(FILE* fin, SV &sv)
{
	int i;
	tachnoidung(fin, sv.id);
	tachnoidung(fin, sv.name);
	tachnoidung(fin, sv.major);
	tachkhoa(fin, sv.area);
	tachnoidung(fin, sv.bday);
	tachnoidung(fin, sv.pic);
	tachnoidung(fin, sv.desc);
	for (i = 0; i < sv.x; i++)
	{
		tachnoidung(fin, sv.hobby[i]);
	}
}

wchar_t* outhtml(SV sv)
{
	wchar_t tenweb[50] = L"Websites\\";
	wchar_t duoifile[50] = L".html";
	wcscat(tenweb, sv.id);
	wcscat(tenweb, duoifile);
	return tenweb;
}

void chenvao(wchar_t cantim[], wchar_t chuoicha[], wchar_t chuoicon[])
{
	wchar_t gep1[200];
	wchar_t* gep2;
	chuoicha = wcsstr(chuoicha, cantim);
	int n = wcslen(chuoicha) - wcslen(chuoicha);
	wcsncpy(gep1, chuoicha, n);
	gep1[n] = L'\0';
	wcscat(chuoicon, chuoicha + wcslen(cantim));
	wcscat(gep1, chuoicon);
	wcscpy(chuoicha, gep1);
}

void email(wchar_t a[], wchar_t mail[])
{
	wchar_t b[30] = L"@student.hcmus.edu.vn";
	wcscpy(mail, a);
	wcscat(mail, b);
}

void ghepsothich(SV sv, wchar_t kq[])
{
	if (sv.x == 0) kq[0] = '\0';
	else
	{
		for (int i = 0; i < sv.x; i++)
		{
			wcscat(kq, L"<li>");
			wcscat(kq, sv.hobby[i]);
			wcscat(kq, L"</li>");
		}
	}
}

void search_add(wchar_t c[], SV sv)
{
	wchar_t gmailsv[60];
	wchar_t chuoisothich[1000]{};
	ghepsothich(sv, chuoisothich);
	wchar_t*e, *m, *f, *g, *h, *k, *i, *l, *mail, *hs;
	wchar_t EMAIL[60] = L"EMAIL";
	wchar_t tenhoa[30] = L"INHOA";
	wchar_t ten[30] = L"TÊN";
	wchar_t mssv[10] = L"ID";
	wchar_t major[30] = L"MAJOR";
	wchar_t birth[20] = L"NGÀY SINH";
	wchar_t JPG[100] = L"HINHANH";
	wchar_t desc[1000] = L"MÔ TẢ";
	wchar_t sothich[20] = L"SOTHICH";
	hs = wcsstr(c, sothich);
	if (hs != 0)
	{
		chenvao(sothich, c, chuoisothich);
	}
	e = wcsstr(c, ten);
	if (e != 0)
	{
		chenvao(ten, c, sv.name);
	}
	m = wcsstr(c, tenhoa);
	if (m != 0){
		chenvao(tenhoa, c, wcsupr(sv.name));
	}
	f = wcsstr(c, mssv);
	if (f != 0)
	{
		chenvao(mssv, c, sv.id);
	}
	g = wcsstr(c, major);
	if (g != 0)
	{
		chenvao(major, c, sv.major);
	}
	i = wcsstr(c, birth);
	if (i != 0)
	{
		chenvao(birth, c, sv.bday);
	}
	k = wcsstr(c, JPG);
	if (k != 0)
	{
		chenvao(JPG, c, sv.pic);
	}
	l = wcsstr(c, desc);
	if (l != 0)
	{
		chenvao(desc, c, sv.desc);
	}
	email(sv.id, gmailsv);
	mail = wcsstr(c, EMAIL);
	if (mail != 0)
	{
		chenvao(EMAIL, c, gmailsv);
	}
}

void xuatnoidungvaohtml(FILE* fDefault, FILE* csv, SV &sv)
{
	rewind(fDefault);
	wchar_t c[500];
	while (!feof(fDefault))
	{
		fgetws(c, 500, fDefault);
		search_add(c, sv);
		fputws(c, csv);
	}
	fclose(csv);
}

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	SV sv[20];
	int soSV, k;
	FILE *fin = _wfopen(L"sv.csv", L"r, ccs=UTF-8");
	if (!fin) wprintf(L"Không thể đọc file \n");
	else
	{
		soSV = sosv(fin);
		chiahob(fin, sv, soSV);
		fseek(fin, 1L, SEEK_CUR);
		for (k = 0; k < soSV; k++)
		{
			fseek(fin, 1L, SEEK_CUR);
			gannoidung(fin, sv[k]);
		}
	}
	int i = 0;
	wchar_t *tenhtml;
	FILE *fDefault = _wfopen(L"Default.html", L"r, ccs=UTF-8");
	if (!fDefault) wprintf(L"Không thể đọc .file \n");
	else
	{
		while (i < soSV)
		{
			tenhtml = outhtml(sv[i]);
			FILE *fweb = _wfopen(tenhtml, L"wt, ccs=UTF-8");
			if (!fweb) wprintf(L"Không thể mở file\n");
			else xuatnoidungvaohtml(fDefault, fweb, sv[i]);
			i++;
		}
	}
	fclose(fin);
}
