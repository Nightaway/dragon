#include "IPLocator.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>

#define MAX_LENGTH 60

static const unsigned int MAXCOOKIESIZE = 2048;
static const int       FULL_RECORD_LENGTH = 50;

static const int CITYCONFIDENCE_FIXED_RECORD     = 4;
static const int CITYCONFIDENCEDIST_FIXED_RECORD = 6;

const int IP_SIZE = 4;
const int OFFSET_SIZE = 3;
const int INDEX_RECORD_SIZE = 7;		

enum
{
	REDIRECT_MODE_1 = 0x01,
	REDIRECT_MODE_2 = 0x02
};

IPLocator::IPLocator() : _buff(NULL)
{
	
}

void IPLocator::open(const char *ipFileName)
{
	int iShmSize;
	FILE *fd = fopen(ipFileName, "rb");
	if (fd == NULL)
		throw std::runtime_error("Open IP Library Failed.");

	fseek(fd,0,SEEK_END);
    iShmSize = ftell(fd); 
	fseek(fd, 0, SEEK_SET);
	
	_buff= (char*) malloc(iShmSize);
	int nbyte = fread((void*)_buff, 1, iShmSize, fd);

	memcpy((void*)&m_iFirstIndex, _buff, 4);
	memcpy((void*)&m_iLastIndex, _buff + 4, 4);

	fclose(fd);
}

unsigned int IPLocator::ip_str2int(const char *ip)
{
	char lip[4] = {0};
	unsigned u_ip = 0;
	int ipno = 0;
	char *pstr1 = (char*)ip;
	char *pstr2 = (char*)ip;
	while(*pstr2 != '\0')
	{
		if (*pstr2 == '.')
		{
			ipno++;
			pstr2++;
			strncpy(lip, pstr1, pstr2 - pstr1 - 1);
			u_ip += atoi(lip);
			u_ip <<= 8;
			pstr1 = pstr2;
			memset(lip, 0x00, strlen(lip)); 
		}
		pstr2++;
	}
	if (ipno == 3 && *pstr2 == 0)
	{
		strncpy(lip, pstr1, pstr2 - pstr1);
		u_ip += atoi(lip);
		memset(lip, 0x00,sizeof(lip)); 
	}
	return u_ip;
}

int IPLocator::binary_find(unsigned int ip, int left, int right)
{
	if ( right-left == 1) 
	{
		return left;
	}
	else
	{
		int middle = (left + right) / 2;

		int offset = m_iFirstIndex + middle * INDEX_RECORD_SIZE;
		unsigned int new_ip = 0;
		memcpy((void*)&new_ip, _buff + offset, 4);

		if ( ip >= new_ip ) {
			return binary_find(ip, middle, right);
		}else{
			return binary_find(ip, left, middle);
		}
	}
}

int IPLocator::getIpAddress(unsigned int uip, char *area, char *sp)
{
	int record_num = (m_iLastIndex - m_iFirstIndex) / INDEX_RECORD_SIZE + 1;
	int index  = binary_find(uip, 0, record_num - 1);
	int index_offset = m_iFirstIndex + index * INDEX_RECORD_SIZE + 4;
	int addr_offset = 0;

	memcpy((void*)&addr_offset, _buff + index_offset, 3);
	
	char cRedirectMode;
	cRedirectMode = *(_buff + addr_offset + 4);

	int iCountryOffset = 0;
	if (cRedirectMode == REDIRECT_MODE_1)
	{
		memcpy((void*)&iCountryOffset, _buff+addr_offset + 5, 3);

		char byte = *(_buff + iCountryOffset);
		if (byte == REDIRECT_MODE_2)
		{
			int  iCtOf = 0;
			memcpy((void*)&iCtOf, _buff + iCountryOffset + 1, 3);
			readArea(iCtOf, area);
			readSp(iCountryOffset + 4, sp);
		}
		else
		{
			readArea(iCountryOffset, area);
			int newoffset = iCountryOffset + strlen(area) + 1;
			readSp(newoffset, sp);
		}
	}
	else if (cRedirectMode == REDIRECT_MODE_2)
	{
		memcpy((void*)&iCountryOffset, _buff + addr_offset + 5, 3);
		readArea(iCountryOffset, area);
		readSp(addr_offset + 8, sp);
	} 
	else
	{
		readArea(addr_offset + 4, area);
		int newoffset = addr_offset + 4 + strlen(area) + 1;
		readSp(newoffset, sp);
	}

	return 0;
}

int IPLocator::readArea(const int offset, char *sArea)
{
	int len = 0;
	char sCountry[MAX_LENGTH] = {0};
	
	char ch = *(_buff + offset);
	while ( ch != 0 && ch != EOF ) 
	{
		if(len>=MAX_LENGTH) return 0;	
		ch = *(_buff + offset + len);
		sCountry[len] = ch;
		len++;
		
	}
	strncpy(sArea, sCountry, len);

	return 0; 
}

int IPLocator::readSp(const int offset, char *sSp)
{
	char b = *(_buff + offset);

	if ( b == REDIRECT_MODE_1 || b == REDIRECT_MODE_2) 
	{
		int iSpOffset=0;
		memcpy((void*)&iSpOffset, _buff + offset + 1, 3);

		if ( iSpOffset) 
		{ 
			readArea(iSpOffset, sSp);
		}
		else
		{
			strncpy(sSp, "Unkown", 6);
		}   
	}
	else
	{
		readArea(offset, sSp);
	}   

	return 0;
}
