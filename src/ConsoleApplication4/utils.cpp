#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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



#ifdef _WIN32
#include <psapi.h>
#pragma warning(disable : 4996)

int GetMemoryInfo() {
	HANDLE hProcess;
	DWORD processID = GetCurrentProcessId();

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (NULL == hProcess)
		return 1;

	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		printf("Current process memory usage: %d KB\n", pmc.WorkingSetSize / 1024);
	}

	CloseHandle(hProcess);
}
#else
#include <string>
#include <sstream>
#include <unistd.h> // for sysconf
struct MemoryInfo {
	long long totalVirtualMem;
	long long residentSetSize;
};
void GetMemoryInfo() {
	//MemoryInfo memInfo = { 0, 0 };
	//std::string line;
	//std::ifstream statm_file("/proc/self/statm");
	//if (statm_file.is_open()) {
	//	std::getline(statm_file, line);
	//	std::istringstream iss(line);
	//	iss >> memInfo.totalVirtualMem >> memInfo.residentSetSize;
	//	statm_file.close();
	//}
	//else {
	//	std::cerr << "Unable to open /proc/self/statm\n";
	//	return;
	//}

	//// Convert pages to bytes
	//long long pageSize = sysconf(_SC_PAGESIZE);
	//memInfo.totalVirtualMem *= pageSize;
	//memInfo.residentSetSize *= pageSize;

	//std::cout << "Total Virtual Memory: " << memInfo.totalVirtualMem << " bytes\n";
	//std::cout << "Resident Set Size: " << memInfo.residentSetSize << " bytes\n";
}
#endif 


unsigned long Ip2Num(char* ip)
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

void Num2Ip(unsigned long num, char* ip)
{
	sprintf(ip, "%lu.%lu.%lu.%lu", num >> 24 & 0xff, num >> 16 & 0xff, num >> 8 & 0xff, num & 0xff);
}


unsigned char Region2Num(char* region)
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


void Num2Region(unsigned char r, char* region)
{
	strcpy(region, REGION[r]);
}


void UL2Region(unsigned long long num, char* region)
{
#if 0
	sprintf(region, "%s,%s,%s,%s,%s,%s,%s,%s",
		REGION[num >> 56 & 0xff], REGION[num >> 48 & 0xff], REGION[num >> 40 & 0xff], REGION[num >> 32 & 0xff],
		REGION[num >> 24 & 0xff], REGION[num >> 16 & 0xff], REGION[num >> 8 & 0xff], REGION[num & 0xff]);
#else
	sprintf(region, "%s,%s,%s,%s,%s,%s,%s,%s",
		REGION[num & 0xff], REGION[num >> 8 & 0xff], REGION[num >> 16 & 0xff], REGION[num >> 24 & 0xff],
		REGION[num >> 32 & 0xff], REGION[num >> 40 & 0xff], REGION[num >> 48 & 0xff], REGION[num >> 56 & 0xff]);
#endif
}


void ST2Region(union ST num, char* region)
{
	sprintf(region, "%s,%s,%s,%s,%s,%s,%s,%s",
		REGION[num.g[0]], REGION[num.g[1]], REGION[num.g[2]], REGION[num.g[3]], REGION[num.g[4]], REGION[num.g[5]], REGION[num.g[6]], REGION[num.g[7]]);
}



const char CountryCode[255][3] =
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



