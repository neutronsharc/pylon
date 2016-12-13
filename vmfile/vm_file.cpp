//
// Created by Shawn on 12/12/16.
//

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#include <unistd.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "vm_file.h"


namespace hcdnbd {


VmFile::~VmFile() {
  Close();
  pthread_rwlock_destroy(&rwlock_);
  LOG(INFO) << "release vmfile " << filename_;
}

ErrCode VmFile::Init(uint64_t size) {
  // open file
  fd_ = open(filename_.c_str(), O_CREAT | O_RDWR | O_CLOEXEC, 0644);
  if (fd_ <= 0) {
    LOG(ERROR) << "failed to open file " << filename_
               << ": " << strerror(errno);
    return kFileError;
  }
  // check file stat
  struct stat st;
  if (fstat(fd_, &st) != 0) {
    LOG(ERROR) << "failed to get fstat " << filename_
               << ": " << strerror(errno);
    return kFileError;
  }
  if (size == 0) {
    size = st.st_size;
  }
  // adjust file size.
  size = VmPage::align_page(size);
  if (size < VmPage::PAGE_SIZE) {
    LOG(ERROR) << "invalid file size " << size;
    Close();
    return kInvalidParameter;
  }
  // truncate file to given size.
  if (ftruncate(fd_, size) != 0) {
    LOG(ERROR) << "failed to truncate file " << filename_
               << " to size " << size << ": " << strerror(errno);
    Close();
    return kFileError;
  }
  // mmap the file.
  buffer_ = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
  if (buffer_ == MAP_FAILED) {
    LOG(ERROR) << "failed to map file " << filename_
               << " size " << size << ": " << strerror(errno);
    Close();
    return kMmapError;
  }

  filesize_ = size;
  LOG(INFO) << "mapped file " << filename_
            << " size " << size << " to address " << buffer_;
  return kOk;
}

ErrCode VmFile::Close() {
  if (buffer_) {
    Flush();
    LOG(INFO) << "unmap buffer " << buffer_ << " for file " << filename_;
    munmap(buffer_, filesize_);
    buffer_ = nullptr;
  }
  if (fd_ > 0) {
    close(fd_);
    fd_ = -1;
    LOG(INFO) << "close file " << filename_;
  }
  return kOk;
}

ErrCode VmFile::Flush() {
  if (buffer_) {
    if (msync(buffer_, filesize_, MS_SYNC) != 0) {
      LOG(ERROR) << "failed to sync vmfile file " << filename_
                 << " size " << filesize_ << ": " << strerror(errno);
    }
  }
  return kOk;
}

ErrCode VmFile::Resize(uint64_t size) {
  uint64_t newsize = VmPage::align_page(size);

  WriteLock();

  if (ftruncate(fd_, newsize) != 0) {
    LOG(ERROR) << "failed to resize truncate file " << filename_
               << " to size " << newsize << ": " << strerror(errno);
    Unlock();
    return kFileError;
  }

  char *newaddr = (char *)mremap(buffer_, filesize_, newsize, MREMAP_MAYMOVE);
  if (newaddr == MAP_FAILED) {
    LOG(ERROR) << "failed to remap file " << filename_
               << " size " << size << ": " << strerror(errno);
    Unlock();
    return kMmapError;
  }

  buffer_ = newaddr;
  filesize_ = newsize;
  Unlock();
  return kOk;
}

ErrCode VmFile::Read(void *buf, uint64_t size, uint64_t offset) {
  ReadLock();
  if (offset + size >= filesize_) {
    Unlock();
    return kOutOfRange;
  }

  memcpy((char*)buf, (char*)buffer_ + offset, size);

  Unlock();
  return kOk;
}

ErrCode VmFile::Write(void *buf, uint64_t size, uint64_t offset) {
  WriteLock();
  if (offset + size >= filesize_) {
    Unlock();
    return kOutOfRange;
  }

  memcpy((char*)buffer_ + offset, buf, size);

  Unlock();
  return kOk;
}

string VmFile::ToString() {
  string s;
  char buf[100];
  sprintf(buf, "%p", buffer());
  s.append("filename: ").append(filename());
  s.append(", size: " ).append(to_string(size()));
  s.append(", buf: " ).append(buf);

  return s;
}

}
