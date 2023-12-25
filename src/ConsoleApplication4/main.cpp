// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "logger.h"
#include "country.h"

#ifndef GIT_COMMIT_ID
#define GIT_COMMIT_ID "unknown"
#endif

#ifndef BUILD_TIME
#define BUILD_TIME "unknown"
#endif

#define FXULONG_MAX 0xffffffffUL //4G



int main()
{
	Logger::Initialize(0);
	LOG(INFO) << "Git commit ID: " << GIT_COMMIT_ID ;
	LOG(INFO) << "Build time: " << BUILD_TIME ;

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

		ReadStandCountryFile("./origin/asn-country-ipv4.csv", ip, start, end);
		ReadStandCountryFile("./origin/geo-asn-country-ipv4.csv", ip, start, end);
		ReadStandCountryFile("./origin/dbip-country-ipv4.csv", ip, start, end);
		ReadStandCountryFile("./origin/geolite2-country-ipv4.csv", ip, start, end);
		ReadStandCountryFile("./origin/iptoasn-country-ipv4.csv", ip, start, end);

		WriteFile("./origin/out.csv", ip, ONE_SIZE, start, end);

		GetMemoryInfo();
#endif
	}

	unsigned long ae = GetTickCountPortable();
	printf("time:%lf\n", (ae - as) / 1000.0);
	LOG(INFO) << "time:" << (ae - as) / 1000.0;
	return 0;
}

