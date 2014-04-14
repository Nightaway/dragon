#include "Bz2.h"

#include <memory.h>
#include <stdexcept>
#include <bzlib.h>

NS_USING_DRAGON

std::string dragon::Bzip2::Compress(std::string str)
{
	char buf[4096];
	unsigned len = 4096;

	if (::BZ2_bzBuffToBuffCompress(buf, &len, const_cast<char *>(str.c_str()), str.length(), 1, 0, 30) != BZ_OK)  {
		throw std::runtime_error("bz2_compress_error!");
	}
	return std::string(buf, len);
}

std::string dragon::Bzip2::Decompress(std::string str)
{
	char buf[4096];
	unsigned len = 4096;

	int ret;
	if ((ret = ::BZ2_bzBuffToBuffDecompress(buf, &len, const_cast<char *>(str.c_str()), str.length(), 1, 0)) != BZ_OK)  {

		if (ret == BZ_DATA_ERROR) {
			throw std::runtime_error("bz2_decompress_error : DATA_ERROR");
		}
		throw std::runtime_error("bz2_decompress_error!");
	}
	return std::string(buf, len);
}