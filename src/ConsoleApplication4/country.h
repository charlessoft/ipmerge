#ifndef COUNTRY_H_
#define COUNTRY_H_

//��������4�� ռ��16G�ռ� 
//��Ҫ����4 ��Ϊ lnewRegion ��unsigned long
//���� ��  2  4 8
#define SIZE4GxN 8
union ST
{
	unsigned char g[SIZE4GxN];
	unsigned long long ulg;
};


#endif