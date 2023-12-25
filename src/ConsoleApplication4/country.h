#ifndef COUNTRY_H_
#define COUNTRY_H_
#include "utils.h"

bool ReadLine(char* buf, char* sip, char* eip, char* region);

void BestRegion(unsigned char* num, char* region)
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

void ST2BestRegion(union ST num, char* region)
{
	BestRegion(num.g, region);
}

void UL2BestRegion(unsigned long long num, char* region)
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

void ReadStandCountryFile(char* filename, union ST* ip, unsigned long start, unsigned long end)
{
	char buf[256] = { 0 };

	char sip[64] = { 0 };
	char eip[64] = { 0 };
	char region[64] = { 0 };

	unsigned long usip = 0;
	unsigned long ueip = 0;
	unsigned char uregion = 0;

	unsigned long m = 0;

	FILE* fp = fopen(filename, "rb");
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

void WriteFile(char* filename, union ST* ip, unsigned long onesize, unsigned long start, unsigned long end)
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

	FILE* fp = fopen(filename, "ab+");
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


bool ReadLine(char* buf, char* sip, char* eip, char* region)
{

	char* tempBuf = nullptr;
	char* sp = buf;

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

}


#endif