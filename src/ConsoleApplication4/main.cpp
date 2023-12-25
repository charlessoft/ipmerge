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


#define FXULONG_MAX 0xffffffffUL //4G

const char REGION[255][3] =
{
	"",
	"AD",
	"AE",
	"AF",
	"AG",
	"AI",
	"AL",
	"AM",
	"AO",
	"AR",
	"AS",
	"AT",
	"AU",
	"AW",
	"AX",
	"AZ",
	"BA",
	"BB",
	"BD",
	"BE",
	"BF",
	"BG",
	"BH",
	"BI",
	"BJ",
	"BL",
	"BM",
	"BN",
	"BO",
	"BQ",
	"BR",
	"BS",
	"BT",
	"BW",
	"BY",
	"BZ",
	"CA",
	"CD",
	"CF",
	"CG",
	"CH",
	"CI",
	"CK",
	"CL",
	"CM",
	"CN",
	"CO",
	"CR",
	"CU",
	"CV",
	"CW",
	"CY",
	"CZ",
	"DE",
	"DJ",
	"DK",
	"DM",
	"DO",
	"DZ",
	"EC",
	"EE",
	"EG",
	"ER",
	"ES",
	"ET",
	"FI",
	"FJ",
	"FK",
	"FM",
	"FO",
	"FR",
	"GA",
	"GB",
	"GD",
	"GE",
	"GF",
	"GG",
	"GH",
	"GI",
	"GL",
	"GM",
	"GN",
	"GP",
	"GQ",
	"GR",
	"GT",
	"GU",
	"GW",
	"GY",
	"HK",
	"HN",
	"HR",
	"HT",
	"HU",
	"ID",
	"IE",
	"IL",
	"IM",
	"IN",
	"IO",
	"IQ",
	"IR",
	"IS",
	"IT",
	"JE",
	"JM",
	"JO",
	"JP",
	"KE",
	"KG",
	"KH",
	"KI",
	"KM",
	"KN",
	"KP",
	"KR",
	"KW",
	"KY",
	"KZ",
	"LA",
	"LB",
	"LC",
	"LI",
	"LK",
	"LR",
	"LS",
	"LT",
	"LU",
	"LV",
	"LY",
	"MA",
	"MC",
	"MD",
	"ME",
	"MF",
	"MG",
	"MH",
	"MK",
	"ML",
	"MM",
	"MN",
	"MO",
	"MP",
	"MQ",
	"MR",
	"MS",
	"MT",
	"MU",
	"MV",
	"MW",
	"MX",
	"MY",
	"MZ",
	"NA",
	"NC",
	"NE",
	"NF",
	"NG",
	"NI",
	"NL",
	"NO",
	"NP",
	"NR",
	"NU",
	"NZ",
	"OM",
	"PA",
	"PE",
	"PF",
	"PG",
	"PH",
	"PK",
	"PL",
	"PM",
	"PR",
	"PS",
	"PT",
	"PW",
	"PY",
	"QA",
	"RE",
	"RO",
	"RS",
	"RU",
	"RW",
	"SA",
	"SB",
	"SC",
	"SD",
	"SE",
	"SG",
	"SI",
	"SK",
	"SL",
	"SM",
	"SN",
	"SO",
	"SR",
	"SS",
	"ST",
	"SV",
	"SX",
	"SY",
	"SZ",
	"TC",
	"TD",
	"TG",
	"TH",
	"TJ",
	"TK",
	"TL",
	"TM",
	"TN",
	"TO",
	"TR",
	"TT",
	"TV",
	"TW",
	"TZ",
	"UA",
	"UG",
	"US",
	"UY",
	"UZ",
	"VA",
	"VC",
	"VE",
	"VG",
	"VI",
	"VN",
	"VU",
	"WF",
	"WS",
	"YE",
	"YT",
	"ZA",
	"ZM",
	"ZW",
};



//这里申请4个 占用16G空间 
//不要超过4 因为 lnewRegion 是unsigned long
//可以 改  2  4 8
#define SIZE4GxN 8
union ST
{
	unsigned char g[SIZE4GxN];
	unsigned long long ulg;
};

bool ReadLine(char *buf, char *sip, char *eip, char *region)
{
#if 0
	char** pdt = str_split(buf, ",");

	strcpy(sip, pdt[0]);
	strcpy(eip, pdt[1]);
	strcpy(region, pdt[2]);
	for (int j = 0; j < 32; j++)
	{
		if (region[j] == '\n' || region[j] == '\r')
		{
			region[j] = 0;
			break;
		}
	}

	free_str_split(pdt);
	return true;
#else
	char *tempBuf = nullptr;
	char *sp = buf;

	int i = 0;
	while (nullptr != (sp = strtok_r(sp, ",", &tempBuf)))
	{
		if (0 == i)
		{
			strcpy(sip, sp);
		}
		else if (1 == i)
		{
			strcpy(eip, sp);
		}
		else if (2 == i)
		{
			strcpy(region, sp);
			for (int i = 0; i < 8; i++)
			{
				if (region[i] == '\n' || region[i] == '\r')
				{
					region[i] = 0;
				}
			}
		}
		//printf(sp);
		//printf("\r\n");
		sp = nullptr;
		i++;
	}

	return i == 3;
#endif 

	
}

unsigned long Ip2Num(char *ip)
{
	unsigned long u1, u2, u3, u4;
	if (sscanf(ip, "%lu.%lu.%lu.%lu", &u1, &u2, &u3, &u4) == 4)
	{
		if (u1 <= 255 && u2 <= 255 && u3 <= 255 && u4 <= 255)
		{
			return u1 * 16777216 + u2 * 65536 + u3 * 256 + u4 ;
		}
	}
	return 0;
}

void Num2Ip(unsigned long num, char *ip)
{
	sprintf(ip, "%lu.%lu.%lu.%lu", num >> 24 & 0xff, num >> 16 & 0xff, num >> 8 & 0xff, num & 0xff);
}

unsigned char Region2Num(char *region)
{
	for (int i = 0; i < 255; i++)
	{
		if (0 == strcmp(region, REGION[i]))
		{
			return i;
		}
	}
	return 0;
}

void Num2Region(unsigned char r, char *region)
{
	strcpy(region, REGION[r]);
}

void UL2Region(unsigned long long num, char *region)
{
#if 0
	sprintf(region, "%s,%s,%s,%s,%s,%s,%s,%s",
		REGION[num >> 56 & 0xff], REGION[num >> 48 & 0xff], REGION[num >> 40 & 0xff], REGION[num >> 32 & 0xff], 
		REGION[num >> 24 & 0xff], REGION[num >> 16 & 0xff], REGION[num >> 8 & 0xff], REGION[num & 0xff]);
#else
	sprintf(region, "%s,%s,%s,%s,%s,%s,%s,%s",
		REGION[num  & 0xff], REGION[num >> 8 & 0xff], REGION[num >> 16 & 0xff], REGION[num >> 24 & 0xff],
		REGION[num >> 32 & 0xff], REGION[num >> 40 & 0xff], REGION[num >> 48 & 0xff], REGION[num >> 56 & 0xff]);
#endif
}

void ST2Region(union ST num, char *region)
{
	sprintf(region, "%s,%s,%s,%s,%s,%s,%s,%s",
		REGION[num.g[0]], REGION[num.g[1]], REGION[num.g[2]], REGION[num.g[3]], REGION[num.g[4]], REGION[num.g[5]], REGION[num.g[6]], REGION[num.g[7]]);
}

void BestRegion(unsigned char *num, char *region)
{
	unsigned char best[SIZE4GxN] = { 0 };
	unsigned char bestNum[SIZE4GxN] = { 0 };
	unsigned char best_index = 0;
	unsigned char best_max = 0;

	bool needadd = false;
	for (int j = 0; j < SIZE4GxN; j++)
	{
		if (0 == num[j])
			continue;

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
			Num2Region(best[k], region);
			break;
		}
	}

}

void ST2BestRegion(union ST num, char *region)
{
	BestRegion(num.g, region);
}

void UL2BestRegion(unsigned long long num, char *region)
{
	unsigned char a[SIZE4GxN] = { 0 };
#if SIZE4GxN == 8
	a[0] = num >> 56 & 0xff;
	a[1] = num >> 48 & 0xff;
	a[2] = num >> 40 & 0xff;
	a[3] = num >> 32 & 0xff;
	a[4] = num >> 24 & 0xff;
	a[5] = num >> 16 & 0xff;
	a[6] = num >> 8 & 0xff;
	a[7] = num & 0xff;
#elif SIZE4GxN == 4
	a[0] = num >> 24 & 0xff;
	a[1] = num >> 16 & 0xff;
	a[2] = num >> 8 & 0xff;
	a[3] = num & 0xff;
#else
	a[0] = num >> 8 & 0xff;
	a[1] = num & 0xff;
#endif

	BestRegion(a, region);
}

void ReadFile(char* filename, union ST *ip, unsigned long start, unsigned long end)
{
	char buf[256] = { 0 };

	char sip[64] = { 0 };
	char eip[64] = { 0 };
	char region[64] = { 0 };

	unsigned long usip = 0;
	unsigned long ueip = 0;
	unsigned char uregion = 0;

	unsigned long m = 0;

	FILE *fp = fopen(filename, "rb");
	if (fp)
	{
		while (fgets(buf, 256, fp) != NULL)
		{
			if (ReadLine(buf, sip, eip, region))
			{
				usip = Ip2Num(sip);
				ueip = Ip2Num(eip);
				uregion = Region2Num(region);
				if (uregion == 0)
					continue;
				if (usip > end)
					break;
				if (ueip < start)
					continue;
				for (unsigned long i = (usip > start ? usip : start); i <= ueip && i < end; i++)
				{
					m = i - start;
					for (int j = 0; j < SIZE4GxN; j++)
					{
						if (0 == ip[m].g[j])
						{
							ip[m].g[j] = uregion;
							break;
						}
					}
				}

			}
		}
		fclose(fp);
		fp = NULL;
	}
}

void WriteFile(char* filename, union ST *ip, unsigned long onesize, unsigned long start, unsigned long end)
{
	char buf[256] = { 0 };

	char sip[64] = { 0 };
	char eip[64] = { 0 };
	char region[64] = { 0 };

	unsigned long usip = 0;
	unsigned long ueip = 0;
	union ST stRegion;
	stRegion.ulg = 0;
	unsigned long long lregion = 0;
	unsigned long long lnewRegion = 0;

	FILE *fp = fopen(filename, "ab+");
	if (fp)
	{
		for (unsigned long i = 0; i < onesize; i++)
		{
#if 0
			lnewRegion = ip[i].g[0];
			for (int j = 1; j < SIZE4GxN; j++)
			{
				if (0 == ip[i].g[j])
					break;
				lnewRegion += (unsigned long long) ip[i].g[j] * (unsigned long long)pow((double)256, j);
			}
#endif
			lnewRegion = ip[i].ulg;
			if (0 == i)
			{
				lregion = lnewRegion;
				stRegion.ulg = lregion;
				usip = i;
			}
			if (lregion != lnewRegion)
			{
				if (i > 0 && 0 != lregion)
				{
					ueip = i - 1;
					Num2Ip(usip + start, sip);
					Num2Ip(ueip + start, eip);

					ST2Region(stRegion, region);
					//UL2Region(lregion, region);  //输出 合并后的 所有区域
					//UL2BestRegion(lregion, region);  //输出 最优区域
					fprintf(fp, "%s,%s,%s\n", sip, eip, region);
				}
				lregion = lnewRegion;
				stRegion.ulg = lregion;
				usip = i;
			}
		}

		if (0 != lregion)
		{
			ueip = onesize - 1;
			Num2Ip(usip + start, sip);
			Num2Ip(ueip + start, eip);

			ST2Region(stRegion, region);
			//UL2Region(lregion, region);  //输出 合并后的 所有区域
			//UL2BestRegion(lregion, region); //输出 最优区域
			fprintf(fp, "%s,%s,%s\n", sip, eip, region);
		}

		fclose(fp);
		fp = NULL;
	}
}

int main()
{
	Logger::Initialize(0);
	LOG(INFO) << "begin";
	FILE * fp = fopen("./testcsv/out.csv", "wb+");
	if (fp)
	{
		fclose(fp);
		fp = NULL;
	}


	unsigned long		as = GetTickCountPortable();

	unsigned int gap = 64;
	unsigned long ONE_SIZE = FXULONG_MAX / gap;
	printf("%lld\r\n", ONE_SIZE);
	union ST *ip = (union ST*)calloc(ONE_SIZE, sizeof(union ST));
	for (unsigned long i = 0; i < gap; i++)
	{
		LOG(INFO) << "处理" << i << "批次";
		memset(ip, 0, ONE_SIZE* sizeof(union ST));

		unsigned long start = ONE_SIZE * i;
		unsigned long end = ONE_SIZE * (i + 1);
#if 0
		ReadFile("d:\\asn-country-ipv4.csv", ip, start, end);
		ReadFile("d:\\geo-asn-country-ipv4.csv", ip, start, end);

		WriteFile("d:\\out.csv", ip, ONE_SIZE, start, end);
#else

		ReadFile("./origin/asn-country-ipv4.csv", ip, start, end);
		ReadFile("./origin/geo-asn-country-ipv4.csv", ip, start, end);
		ReadFile("./origin/dbip-country-ipv4.csv", ip, start, end);
		ReadFile("./origin/geolite2-country-ipv4.csv", ip, start, end);
		ReadFile("./origin/iptoasn-country-ipv4.csv", ip, start, end);

		WriteFile("./origin/out.csv", ip, ONE_SIZE, start, end);

		GetMemoryInfo();
#endif
	}

	unsigned long ae = GetTickCountPortable();
	printf("time:%lf\n", (ae - as) / 1000.0);
	LOG(INFO) << "time:" << (ae - as) / 1000.0;
	return 0;
}

