#ifndef __IP_LOCATOR_H__
#define __IP_LOCATOR_H__

	class IPLocator {
		char *_buff;
		unsigned int m_iFirstIndex;
		unsigned int m_iLastIndex;

		int binary_find(unsigned int ip, int left, int right);
		int readArea(const int offset, char *sArea);
		int readSp(const int offset, char *sSp);

	public:
		IPLocator();

		void open(const char *ipFileName);
		unsigned int ip_str2int(const char *ip);
		int getIpAddress(unsigned int uip, char *area, char *sp);
	};

#endif