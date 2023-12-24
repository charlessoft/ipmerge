// ConsoleApplication4.cpp : Defines the entry point for the console application.
//


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "logger.h"
#include "utils.h"

#ifdef _WIN32
#include <windows.h>

unsigned long GetTickCountPortable() {
	return GetTickCount();
}

char* strtok_r(char* s, const char* delim, char** save_ptr) {
	char* token;
	/*判断参数s是否为NULL，如果是NULL就以传递进来的save_ptr作为起始分解位置；若不是NULL，则以s开始切分*/
	if (s == NULL) s = *save_ptr;

	/* Scan leading delimiters.  */
	s += strspn(s, delim);
	/*判断当前待分解的位置是否为'\0'，若是则返回NULL（联系到（一）中所说对返回值为NULL的解释）；不是则继续。*/
	if (*s == '\0')
		return NULL;

	/* Find the end of the token.  */
	token = s;
	s = strpbrk(token, delim);
	if (s == NULL)
		/* This token finishes the string.  */
		*save_ptr = strchr(token, '\0');
	else {
		/* Terminate the token and make *SAVE_PTR point past it.  */
		*s = '\0';
		*save_ptr = s + 1;
	}

	return token;
}



#else
#include <time.h>

unsigned long  GetTickCountPortable() {
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
		// Handle error
		return 0;
	}
	return (unsigned long long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif





enum FileType
{
	File_Country,
	File_City,
};



//可以 改  2  4 8
#define CountryMax 8
union CountryUnion
{
	unsigned char g[CountryMax]; 
	unsigned long long ulg; // 8个字节
};


unsigned char BestCountry(unsigned char *num)
{
	unsigned char best[CountryMax] = { 0 };
	unsigned char bestNum[CountryMax] = { 0 };
	unsigned char best_index = 0;
	unsigned char best_max = 0;

	bool needadd = false;
	for (int j = 0; j < CountryMax; j++)
	{
		if (0 == num[j])
			break;

		needadd = true;
		for (int k = 0; k <= best_index; k++)
		{
			if (best[k] == num[j])
			{
				bestNum[k]++;

				if (best_max < bestNum[k])
					best_max = bestNum[k];

				needadd = false;
				break;
			}
		}

		if (needadd)
		{
			best[best_index] = num[j];
			bestNum[best_index]++;

			if (best_max < bestNum[best_index])
				best_max = bestNum[best_index];

			best_index++;
		}
	}

	for (int k = 0; k <= best_index; k++)
	{
		if (bestNum[k] == best_max)
		{
			return best[k];
		}
	}
	return 0;
}

#define CITY_STRING_SIZE 32
struct Data
{
	//国家文件
	union CountryUnion uCountry;
	unsigned char ucBestCountry;

	//以下是城市文件 城市只支持一个
	unsigned char ucCityFileCountry;
	char cState[CITY_STRING_SIZE];
	char cCity[CITY_STRING_SIZE];
	double  dLatitude;
	double dLongitude;

	void Copy(struct Data a)
	{
		uCountry.ulg = a.uCountry.ulg;
		ucBestCountry = a.ucBestCountry;
		ucCityFileCountry = a.ucCityFileCountry;
		memcpy(cState, a.cState, CITY_STRING_SIZE);
		memcpy(cCity, a.cCity, CITY_STRING_SIZE);

		dLatitude = a.dLatitude;
		dLongitude = a.dLongitude;
	}

	bool CmpForCity(struct Data a, bool useCountry)
	{
		if (useCountry)
		{
			return ucBestCountry == a.ucBestCountry;
		}
		else
		{
			if (ucBestCountry == a.ucBestCountry)
			{
				return ucCityFileCountry == a.ucCityFileCountry
					&& 0 == strcmp(cState, a.cState)
					&& 0 == strcmp(cCity, a.cCity)
					&& (long long)(dLatitude * 1000) == (long long)(a.dLatitude * 1000)
					&& (long long)(dLongitude * 1000) == (long long)(a.dLongitude * 1000);
			}
			else
			{
				return false;
			}

		}
	}

	bool IsUseCountry()
	{
		if (0 == ucCityFileCountry)
		{
			return true; //useCountry
		}

		if (0 == ucBestCountry)
		{
			ucBestCountry = ucCityFileCountry;
			return false; //useCity
		}

		//不一样 全清
		if (ucBestCountry != ucCityFileCountry)
		{
			ucCityFileCountry = 0;
			memset(cState, 0, CITY_STRING_SIZE);
			memset(cCity, 0, CITY_STRING_SIZE);
			dLatitude = 0;
			dLongitude = 0;
			return true; //useCountry
		}
		else
		{
			return false; //useCity
		}
	}

	void GenBestCountry()
	{
		// 8个字节, 如果==0,说明没有国家字符串
		if (0 == uCountry.ulg)
		{
			ucBestCountry = 0;
		}
		else if (0 == uCountry.g[1])
		{
			// 如果有一个国家
			ucBestCountry = uCountry.g[0];
		}
		else
		{
			bool allsame = true;
			unsigned char a = 0;
			for (int j = 0; j < CountryMax; j++)
			{
				if (0 == uCountry.g[j])
					break;
				if (0 == j)
				{
					a = uCountry.g[j];
				}
				else  if (a != uCountry.g[j])
				{
					allsame = false;
					break;
				}
			}

			if (allsame)
				ucBestCountry = a;
			else
				ucBestCountry = BestCountry(uCountry.g);
		}
	}

};


bool ReadLine(
	char *buf,
	char *sip, int sipSize,
	char *eip, int eipSize,
	char *country, int countrySize,
	char* state, int stateSize,
	char* city, int citySize,
	double *latitude,
	double *longitude,
	FileType type)
{
	char *tempBuf = nullptr;
	char *sp = buf;

	int i = 0;
	while (nullptr != (sp = strtok_r(sp, ",", &tempBuf)))
	{
		if (0 == i)
		{
			strncpy(sip, sp, sipSize - 1);
		}
		else if (1 == i)
		{
			strncpy(eip, sp, eipSize - 1);
		}
		else if (2 == i)
		{
			strncpy(country, sp, countrySize - 1);
			for (int j = 0; j < countrySize; j++)
			{
				if (country[j] == '\n' || country[j] == '\r')
				{
					country[j] = 0;
				}
			}
		}

		if (File_City == type)
		{
			if (3 == i)
			{
				strncpy(state, sp, stateSize - 1);
			}
			else if (4 == i)
			{
				strncpy(city, sp, citySize - 1);
			}
			else if (5 == i)
			{
				(*latitude) = atof(sp);
			}
			else if (6 == i)
			{
				(*longitude) = atof(sp);
			}
		}

		//printf(sp);
		//printf("\r\n");
		sp = nullptr;
		i++;
	}

	if (File_Country == type)
	{
		return i == 9;
	}
	else if (File_City == type)
	{
		return i == 8;
	}
	else
	{
		return false;
	}
}

unsigned long Ip2Num(char *ip)
{
	unsigned long u1, u2, u3, u4;
	if (sscanf(ip, "%lu.%lu.%lu.%lu", &u1, &u2, &u3, &u4) == 4)
	{
		if (u1 <= 255 && u2 <= 255 && u3 <= 255 && u4 <= 255)
		{
			return u1 * 16777216 + u2 * 65536 + u3 * 256 + u4;
		}
	}
	return 0;
}

void Num2Ip(unsigned long num, char *ip)
{
	sprintf(ip, "%lu.%lu.%lu.%lu", num >> 24 & 0xff, num >> 16 & 0xff, num >> 8 & 0xff, num & 0xff);
}

unsigned char Country2Num(char *country)
{
	for (int i = 0; i < 255; i++)
	{
		if (0 == strcmp(country, CountryCode[i]))
		{
			return i;
		}
	}
	return 0;
}

void Num2Country(unsigned char r, char *country)
{
	strcpy(country, CountryCode[r]);
}

const char* Num2CountryEx(unsigned char r)
{
	return CountryCode[r];
}


void UL2Country(unsigned long long num, char *country)
{
#if 0
	sprintf(country, "%s,%s,%s,%s,%s,%s,%s,%s",
		CountryCode[num >> 56 & 0xff], CountryCode[num >> 48 & 0xff], CountryCode[num >> 40 & 0xff], CountryCode[num >> 32 & 0xff],
		CountryCode[num >> 24 & 0xff], CountryCode[num >> 16 & 0xff], CountryCode[num >> 8 & 0xff], CountryCode[num & 0xff]);
#else
	sprintf(country, "%s,%s,%s,%s,%s,%s,%s,%s",
		CountryCode[num & 0xff], CountryCode[num >> 8 & 0xff], CountryCode[num >> 16 & 0xff], CountryCode[num >> 24 & 0xff],
		CountryCode[num >> 32 & 0xff], CountryCode[num >> 40 & 0xff], CountryCode[num >> 48 & 0xff], CountryCode[num >> 56 & 0xff]);
#endif
}

void ST2Country(union CountryUnion num, char *country)
{
	sprintf(country, "%s,%s,%s,%s,%s,%s,%s,%s",
		CountryCode[num.g[0]], CountryCode[num.g[1]], CountryCode[num.g[2]], CountryCode[num.g[3]], CountryCode[num.g[4]], CountryCode[num.g[5]], CountryCode[num.g[6]], CountryCode[num.g[7]]);
}


void ST2BestCountry(union CountryUnion num, char *country)
{
	strcpy(country, Num2CountryEx(BestCountry(num.g)));
}

void UL2BestCountry(unsigned long long num, char *country)
{
	unsigned char a[CountryMax] = { 0 };
#if CountryMax == 8
	a[0] = num >> 56 & 0xff;
	a[1] = num >> 48 & 0xff;
	a[2] = num >> 40 & 0xff;
	a[3] = num >> 32 & 0xff;
	a[4] = num >> 24 & 0xff;
	a[5] = num >> 16 & 0xff;
	a[6] = num >> 8 & 0xff;
	a[7] = num & 0xff;
#elif CountryMax == 4
	a[0] = num >> 24 & 0xff;
	a[1] = num >> 16 & 0xff;
	a[2] = num >> 8 & 0xff;
	a[3] = num & 0xff;
#else
	a[0] = num >> 8 & 0xff;
	a[1] = num & 0xff;
#endif
	strcpy(country, Num2CountryEx(BestCountry(a)));
}


bool ReadCountryLine(
	char* buf,
	char* sip, int sipSize,
	char* eip, int eipSize,
	char* country, int countrySize,
	char* country1, int countrySize1,
	char* country2, int countrySize2,
	char* country3, int countrySize3,
	char* country4, int countrySize4,
	char* country5, int countrySize5
	){

	char* tempBuf = nullptr;
	char* sp = buf; 

	int i = 0;
	while (nullptr != (sp = strtok_r(sp, ",", &tempBuf)))
	{
		if (0 == i)
		{
			strncpy(sip, sp, sipSize - 1);
		}
		else if (1 == i)
		{
			strncpy(eip, sp, eipSize - 1);
		}
		else if (2 == i)
		{
			strncpy(country, sp, countrySize - 1);
		}

		//if (File_City == type)
		{
			if (3 == i)
			{
				strncpy(country1, sp, countrySize1 - 1);

				//strncpy(state, sp, stateSize - 1);
			}
			else if (4 == i)
			{
				strncpy(country2, sp, countrySize2 - 1);

				//strncpy(city, sp, citySize - 1);
			}
			else if (5 == i)
			{
				strncpy(country3, sp, countrySize3 - 1);

				//(*latitude) = atof(sp);
			}
			else if (6 == i)
			{
				strncpy(country4, sp, countrySize4 - 1);
				//(*longitude) = atof(sp);
			}
			else if (7 == i)
			{
				strncpy(country5, sp, countrySize5 - 1);
				//(*longitude) = atof(sp);
			}
			else if (8 == i)
			{

				//strncpy(country5, sp, countrySize5 - 1);
				//(*longitude) = atof(sp);
			}
		}

		//printf(sp);
		//printf("\r\n");
		sp = nullptr;
		i++;
	}
	return true;
	//return i == 9;
	//return false;

// 	if (File_Country == type)
// 	{
// 		return i == 9;
// 	}
// 	else if (File_City == type)
// 	{
// 		return i == 8;
// 	}
// 	else
// 	{
// 		return false;
// 	}

}

// country
void ReadCountryFile(char* filename, struct Data* ip,  unsigned long start, unsigned long end)
{
	char buf[512] = { 0 };

	char sip[32] = { 0 };
	char eip[32] = { 0 };
	char country[32] = { 0 };
	char country1[32] = { 0 };
	char country2[32] = { 0 };
	char country3[32] = { 0 };
	char country4[32] = { 0 };
	char country5[32] = { 0 };



	char state[CITY_STRING_SIZE] = { 0 };
	char city[CITY_STRING_SIZE] = { 0 };
	double latitude = 0;
	double longitude = 0;

	unsigned long usip = 0;
	unsigned long ueip = 0;
	unsigned char ucountry = 0;
	unsigned char ucountry1 = 0;
	unsigned char ucountry2 = 0;
	unsigned char ucountry3 = 0;
	unsigned char ucountry4 = 0;
	unsigned char ucountry5 = 0;


	unsigned long m = 0;

	FILE* fp = fopen(filename, "rb");
	if (fp)
	{
		while (fgets(buf, 256, fp) != NULL)
		{
			memset(sip, 0, 32);
			memset(eip, 0, 32);
			memset(country, 0, 32);
			memset(state, 0, CITY_STRING_SIZE);
			memset(city, 0, CITY_STRING_SIZE);
			latitude = 0;
			longitude = 0;

			if (ReadCountryLine(buf, sip, 32, eip, 32, country, 32,country1,32,country2,32,country3,32,country4,32,country5,32))
			{
				usip = Ip2Num(sip);
				ueip = Ip2Num(eip);
				ucountry = Country2Num(country);
				ucountry1 = Country2Num(country1);
				ucountry2 = Country2Num(country2);
				ucountry3 = Country2Num(country3);
				ucountry4 = Country2Num(country4);
				ucountry5 = Country2Num(country5);

				if (ucountry == 0)
					continue;
				if (usip > end)
					break;
				if (ueip < start)
					continue;

				for (unsigned long i = (usip > start ? usip : start); i <= ueip && i < end; i++)
				{
					m = i - start;

					ip[m].uCountry.g[0] = ucountry;
					ip[m].uCountry.g[1] = ucountry1;
					ip[m].uCountry.g[2] = ucountry2;
					ip[m].uCountry.g[3] = ucountry3;
					ip[m].uCountry.g[4] = ucountry4;
					ip[m].uCountry.g[5] = ucountry5;

				}

			}

		}
	}
}

void ReadFile(char* filename, struct Data *ip, FileType type, unsigned long start, unsigned long end)
{
	char buf[512] = { 0 };

	char sip[32] = { 0 };
	char eip[32] = { 0 };
	char country[32] = { 0 };

	char state[CITY_STRING_SIZE] = { 0 };
	char city[CITY_STRING_SIZE] = { 0 };
	double latitude = 0;
	double longitude = 0;

	unsigned long usip = 0;
	unsigned long ueip = 0;
	unsigned char ucountry = 0;

	unsigned long m = 0;

	FILE *fp = fopen(filename, "rb");
	if (fp)
	{
		while (fgets(buf, 256, fp) != NULL)
		{
			memset(sip, 0, 32);
			memset(eip, 0, 32);
			memset(country, 0, 32);
			memset(state, 0, CITY_STRING_SIZE);
			memset(city, 0, CITY_STRING_SIZE);
			latitude = 0;
			longitude = 0;

			if (ReadLine(buf, sip, 32, eip, 32, country, 32, state, CITY_STRING_SIZE, city, CITY_STRING_SIZE, &latitude, &longitude, type))
			{
				usip = Ip2Num(sip);
				ueip = Ip2Num(eip);
				ucountry = Country2Num(country);
				if (ucountry == 0)
					continue;
				if (usip > end)
					break;
				if (ueip < start)
					continue;

				for (unsigned long i = (usip > start ? usip : start); i <= ueip && i < end; i++)
				{
					m = i - start;

					if (File_Country == type)
					{
						for (int j = 0; j < CountryMax; j++)
						{
							if (0 == ip[m].uCountry.g[j])
							{
								ip[m].uCountry.g[j] = ucountry;
								break;
							}
						}
					}
					else if (File_City == type)
					{
						ip[m].ucCityFileCountry = ucountry;
						memcpy(ip[m].cState, state, CITY_STRING_SIZE);
						memcpy(ip[m].cCity, city, CITY_STRING_SIZE);
						ip[m].dLatitude = latitude;
						ip[m].dLongitude = longitude;
					}
				}

			}
		}
		fclose(fp);
		fp = NULL;
	}
}


void WriteFile_City(char* filename, struct Data *ip, unsigned long onesize, unsigned long start, unsigned long end)
{
	char buf[256] = { 0 };

	char sip[32] = { 0 };
	char eip[32] = { 0 };

	unsigned long usip = 0;
	unsigned long ueip = 0;

	char state[CITY_STRING_SIZE] = { 0 };
	char city[CITY_STRING_SIZE] = { 0 };
	double latitude = 0;
	double longitude = 0;

	struct Data sOld;
	bool useCountry;

	FILE *fp = fopen(filename, "ab+");
	if (fp)
	{
		for (unsigned long i = 0; i < onesize; i++)
		{
			ip[i].GenBestCountry();
			useCountry = ip[i].IsUseCountry();

			if (0 == i)
			{
				sOld.Copy(ip[i]);
				usip = i;
			}
			if (!sOld.CmpForCity(ip[i], useCountry))
			{
				if (i > 0 && 0 != sOld.ucBestCountry)
				{
					ueip = i - 1;
					Num2Ip(usip + start, sip);
					Num2Ip(ueip + start, eip);

					fprintf(fp, "%s,%s,%s,%s,%s,%lf,%lf\n",
						sip, eip, Num2CountryEx(sOld.ucBestCountry),
						sOld.cState, sOld.cCity, sOld.dLatitude, sOld.dLongitude);
				}
				sOld.Copy(ip[i]);
				usip = i;
			}
		}

		if (0 != sOld.ucBestCountry)
		{
			ueip = onesize - 1;
			Num2Ip(usip + start, sip);
			Num2Ip(ueip + start, eip);

			fprintf(fp, "%s,%s,%s,%s,%s,%lf,%lf\n",
				sip, eip, Num2CountryEx(sOld.ucBestCountry),
				sOld.cState, sOld.cCity, sOld.dLatitude, sOld.dLongitude);
		}

		fclose(fp);
		fp = NULL;
	}
}

void WriteFile_Country(char* filename, struct Data *ip, unsigned long onesize, unsigned long start, unsigned long end)
{
	char buf[256] = { 0 };

	char sip[32] = { 0 };
	char eip[32] = { 0 };
	char country[64] = { 0 };

	unsigned long usip = 0;
	unsigned long ueip = 0;
	union CountryUnion stCountry;
	stCountry.ulg = 0;
	unsigned long long lcountry = 0;
	unsigned long long lnewCountry = 0;

	FILE *fp = fopen(filename, "ab+");
	if (fp)
	{
		for (unsigned long i = 0; i < onesize; i++)
		{
#if 0
			lnewCountry = ip[i].uCountry.g[0];
			for (int j = 1; j < CountryMax; j++)
			{
				if (0 == ip[i].uCountry.g[j])
					break;
				lnewCountry += (unsigned long long) ip[i].uCountry.g[j] * (unsigned long long)pow((double)256, j);
			}
#endif
			lnewCountry = ip[i].uCountry.ulg;
			if (0 == i)
			{
				lcountry = lnewCountry;
				stCountry.ulg = lcountry;
				usip = i;
			}
			if (lcountry != lnewCountry)
			{
				if (i > 0 && 0 != lcountry)
				{
					ueip = i - 1;
					Num2Ip(usip + start, sip);
					Num2Ip(ueip + start, eip);

					ST2Country(stCountry, country);
					//ST2BestCountry(stCountry, country);
					//UL2Country(lcountry, country);  //输出 合并后的 所有区域
					//UL2BestCountry(lcountry, country);  //输出 最优区域
					fprintf(fp, "%s,%s,%s\n", sip, eip, country);
				}
				lcountry = lnewCountry;
				stCountry.ulg = lcountry;
				usip = i;
			}
		}

		if (0 != lcountry)
		{
			ueip = onesize - 1;
			Num2Ip(usip + start, sip);
			Num2Ip(ueip + start, eip);

			ST2Country(stCountry, country);
			//ST2BestCountry(stCountry, country);
			//UL2Country(lcountry, country);  //输出 合并后的 所有区域
			//UL2BestCountry(lcountry, country); //输出 最优区域
			fprintf(fp, "%s,%s,%s\n", sip, eip, country);
		}

		fclose(fp);
		fp = NULL;
	}
}

void WriteFile(char* filename, struct Data *ip, FileType type, unsigned long onesize, unsigned long start, unsigned long end)
{

	if (File_Country == type)
	{
		WriteFile_Country(filename, ip, onesize, start, end);
	}
	else if (File_City == type)
	{
		WriteFile_City(filename, ip, onesize, start, end);
	}
}

int main()
{
	Logger::Initialize(0);
	LOG(INFO) << "begin";
#ifdef _WIN32
	FILE * fp = fopen("d:\\testcsv\\out.csv", "wb+");
#else
	FILE * fp = fopen("./testcsv/out.csv", "wb+");
#endif
	if (fp)
	{
		fclose(fp);
		fp = NULL;
}

	unsigned long		as = GetTickCountPortable();

	unsigned int gap = 512;
	unsigned long ONE_SIZE = FXULONG_MAX / gap;
	printf("%lu\r\n", ONE_SIZE);
	struct Data *ip = (struct Data*)calloc(ONE_SIZE, sizeof(struct Data));
	for (unsigned long i = 0; i < gap; i++)
	{

		memset(ip, 0, ONE_SIZE * sizeof(struct Data));

		unsigned long start = ONE_SIZE * i;
		unsigned long end = ONE_SIZE * (i + 1);
		LOG(INFO) << "处理" << i << "批次";
// 
// 
// 		char sip[32] = { 0 };
// 		Num2Ip(start, sip);
// 
// 		printf("%lu %s\r\n", i, sip);
#if 1
// 		ReadFile(".\\origin\\asn-country-ipv4.csv", ip, File_Country, start, end);
// 		ReadFile(".\\origin\\geo-asn-country-ipv4.csv", ip, File_Country, start, end);
// 		ReadFile(".\\origin\\dbip-country-ipv4.csv", ip, File_Country, start, end);
// 		ReadFile(".\\origin\\geolite2-country-ipv4.csv", ip, File_Country, start, end);

		ReadCountryFile(".\\testcsv\\out.csv", ip, start, end);
		ReadFile(".\\testcsv\\dbip-city-ipv4.csv", ip, File_City, start, end);


// 		ReadCountryFile("/ipmerge/src/ConsoleApplication4/testcsv/out_origin.csv", ip, start, end);
// 		ReadFile("./origin/dbip-city-ipv4_bak.csv", ip, File_City,start, end);
		//WriteFile("d:\\testcsv\\out.csv", ip, File_Country, ONE_SIZE, start, end);
		WriteFile("d:\\testcsv\\out.csv", ip, File_City, ONE_SIZE, start, end);
#else

		ReadFile("./testcsv/asn-country-ipv4.csv", ip, start, end);
		ReadFile("./testcsv/geo-asn-country-ipv4.csv", ip, start, end);
		ReadFile("./testcsv/dbip-country-ipv4.csv", ip, start, end);
		ReadFile("./testcsv/geolite2-country-ipv4.csv", ip, start, end);
		ReadFile("./testcsv/iptoasn-country-ipv4.csv", ip, start, end);

		WriteFile("./testcsv/out.csv", ip, ONE_SIZE, start, end);


#endif
	}

	unsigned long ae = GetTickCountPortable();
	printf("time:%lf\n", (ae - as) / 1000.0);
	return 0;
}

