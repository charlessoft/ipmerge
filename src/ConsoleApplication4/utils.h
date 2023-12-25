#ifndef UTILS_H_
#define  UTILS_H_
#include <string.h>
#include <stdlib.h>
#include <assert.h>


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


#define FXULONG_MAX 0xffffffffUL //4G

char** str_split(char* str, const char* delimiter) {
	char** result;
	size_t count = 0;
	char* tmp = str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = delimiter[0];
	delim[1] = 0;

	/* Count how many elements will be in the array */
	while (*tmp) {
		if (delimiter[0] == *tmp) {
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* Add space for trailing token */
	count += last_comma < (str + strlen(str) - 1);

	/* Add space for terminating null string so caller knows where the list of returned strings ends. */
	count++;

	result = (char** )malloc(sizeof(char*) * count);

	if (result) {
		size_t idx = 0;
		char* token = strtok(str, delim);

		while (token) {
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}


void free_str_split(char** strArray) {
	if (strArray) {
		for (int i = 0; strArray[i] != NULL; i++) {
			free(strArray[i]);
		}
		free(strArray);
	}
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






#endif