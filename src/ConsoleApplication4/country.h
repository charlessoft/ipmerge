#ifndef COUNTRY_H_
#define COUNTRY_H_
#include "utils.h"
/// <summary>
/// 读github上的国家文件, 包含 sip, eip, region
/// </summary>
/// <param name="filename"></param>
/// <param name="ip"></param>
/// <param name="start"></param>
/// <param name="end"></param>
void ReadStandCountryFile(char* filename, union ST* ip, unsigned long start, unsigned long end);

/// <summary>
/// 读一行,一行由 sip,eip,region组成
/// </summary>
/// <param name="buf"></param>
/// <param name="sip"></param>
/// <param name="eip"></param>
/// <param name="region"></param>
/// <returns></returns>
bool ReadStandCountryLine(char* buf, char* sip, char* eip, char* region);

void BestRegion(unsigned char* num, char* region);

void ST2BestRegion(union ST num, char* region);

void UL2BestRegion(unsigned long long num, char* region);

void WriteFile(char* filename, union ST* ip, unsigned long onesize, unsigned long start, unsigned long end);





#endif