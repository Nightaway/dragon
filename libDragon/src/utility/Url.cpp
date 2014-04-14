#include "Url.h"

#include <cstdlib>

NS_USING_DRAGON

inline unsigned char toHex(unsigned char x)
{
	return x > 9 ? x + 55: x + 48;
}

inline unsigned char fromHex(const unsigned char &x)
{
	return isdigit(x) ? x-'0' : x-'A' + 10;
}

static int  php_url_decode(char *str, int len);
static char *php_url_encode(char const *s, int len, int *new_length);

std::string URL::decode(std::string url)
{
	std::string sOut;
	for(size_t ix = 0; ix < url.size(); ix++ )
	{
		unsigned char ch = 0;
				 
		if(url[ix] == '%')
		{
			ch = (fromHex(url[ix+1]) <<4);
			ch |= fromHex(url[ix+2]);
			ix += 2;
		}
		else if (url[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = (unsigned char)url[ix];
		}

		sOut += (unsigned char) ch;
	}
	return sOut;
}

std::string URL::encode(std::string url)
{
	int nl = 0;
	char *ns = php_url_encode(url.c_str(), url.length(), &nl);
	std::string str(ns, nl);
	free(ns);
	return str;
}

static unsigned char hexchars[] = "0123456789ABCDEF";
static int php_htoi(char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}

int php_url_decode(char *str, int len)
{
	char *dest = str;
	char *data = str;

	while (len--) {
		if (*data == '+') {
			*dest = ' ';
		}
		else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) 
				 && isxdigit((int) *(data + 2))) {

			*dest = (char) php_htoi(data + 1);

			data += 2;
			len -= 2;
		} else {
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '\0';
	return dest - str;
}

char *php_url_encode(char const *s, int len, int *new_length)
{
	register unsigned char c;
	unsigned char *to, *start;
	unsigned char const *from, *end;
	
	from = (unsigned char *)s;
	end = (unsigned char *)s + len;
	start = to = (unsigned char *) malloc(3 * len);

	while (from < end) {
		c = *from++;

		if (c == ' ') {
			*to++ = '+';
#ifndef CHARSET_EBCDIC
		} else if ((c < '0' && c != '-' && c != '.') ||
				   (c < 'A' && c > '9') ||
				   (c > 'Z' && c < 'a' && c != '_') ||
				   (c > 'z')) {
			to[0] = '%';
			to[1] = hexchars[c >> 4];
			to[2] = hexchars[c & 15];
			to += 3;
#else /*CHARSET_EBCDIC*/
		} else if (!isalnum(c) && strchr("_-.", c) == NULL) {
			/* Allow only alphanumeric chars and '_', '-', '.'; escape the rest */
			to[0] = '%';
			to[1] = hexchars[os_toascii[c] >> 4];
			to[2] = hexchars[os_toascii[c] & 15];
			to += 3;
#endif /*CHARSET_EBCDIC*/
		} else {
			*to++ = c;
		}
	}
	*to = 0;
	if (new_length) {
		*new_length = to - start;
	}
	return (char *) start;
}