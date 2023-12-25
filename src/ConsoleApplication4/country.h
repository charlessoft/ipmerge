#ifndef COUNTRY_H_
#define COUNTRY_H_

//这里申请4个 占用16G空间 
//不要超过4 因为 lnewRegion 是unsigned long
//可以 改  2  4 8
#define SIZE4GxN 8
union ST
{
	unsigned char g[SIZE4GxN];
	unsigned long long ulg;
};


#endif