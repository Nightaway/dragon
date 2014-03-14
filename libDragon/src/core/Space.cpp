#include "Space.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/mman.h>
#include <errno.h>

#include "Object.h"

NS_USING_DRAGON

Space::Space() : 
	addr_(NULL), 
	size_(0),
	pos_(0)
{

}

Space::~Space()
{

}

void *Space::Allocate(unsigned size)
{
	void *addr = reinterpret_cast<char *>(addr_) + size;
	pos_ += size;
	return addr;
}

SemiSpace::SemiSpace() 
		: is_head_used(true), 
		  head_addr_(addr_), 
                  half_addr_(NULL)
{

}

SemiSpace::~SemiSpace()
{

}

NamedSemiSpace::NamedSemiSpace(const char *name, unsigned size) :
						 name_(name), fd_(-1)
{
	size_ = size;
}

NamedSemiSpace::~NamedSemiSpace()
{

}

void NamedSemiSpace::Create()
{
    fd_ = shm_open(name_, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    char buff[255];
    if (fd_ == -1) {
	sprintf(buff, "shm_open error : %s \n", strerror(errno));
	throw std::runtime_error(std::string(buff, strlen(buff)));
    }

    if (ftruncate(fd_, size_) == -1) {
        sprintf(buff, "ftruncate : %s \n", strerror(errno));
	throw std::runtime_error(std::string(buff, strlen(buff)));
    }

    addr_ = mmap(NULL, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);

    if (close(fd_) == -1) {
	sprintf(buff, "close : %s \n", strerror(errno));
	throw std::runtime_error(std::string(buff, strlen(buff)));
    }

    if (addr_ == MAP_FAILED) {
        sprintf(buff, "mmap : %s \n", strerror(errno));
	throw std::runtime_error(std::string(buff, strlen(buff)));
    }

    half_addr_ = reinterpret_cast<char *>(addr_) + (size_ / 2);
    head_addr_ = addr_;
}

void NamedSemiSpace::Destroy()
{
	munmap(addr_, size_);
	shm_unlink(name_);
}

