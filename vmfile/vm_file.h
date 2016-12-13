//
// Created by Shawn on 12/12/16.
//

#ifndef PYLON_VMFILE_H
#define PYLON_VMFILE_H

#include <stdint.h>
#include <pthread.h>

#include "errcode.h"

using namespace std;

namespace hcdnbd {

struct VmPage {
  enum {
    PAGE_MASK = 4095,
    PAGE_SIZE = 4096,
  };

  // align a value to page size.
  inline static uint64_t align_page(uint64_t v) {
    v += PAGE_MASK;
    return (v & (~PAGE_MASK));
  }
};

class VmFile {
 private:
  // file descriptor.
  int fd_;
  // full path of the backing file
  string filename_;
  // file size
  uint64_t filesize_;

  void* buffer_;

  pthread_rwlock_t rwlock_;

 public:
  VmFile(string filename) :
          filename_(filename), filesize_(0), fd_(-1) {
    pthread_rwlock_init(&rwlock_, NULL);
  }

  ~VmFile();

  ErrCode ReadLock() {
    pthread_rwlock_rdlock(&rwlock_);
  }

  ErrCode Unlock() {
    pthread_rwlock_unlock(&rwlock_);
  }

  ErrCode WriteLock() {
    pthread_rwlock_wrlock(&rwlock_);
  }

  ErrCode Init(uint64_t size);

  ErrCode Close();

  ErrCode Read(void* buf, uint64_t size, uint64_t offset);

  ErrCode Write(void* buf, uint64_t size, uint64_t offset);

  ErrCode Resize(uint64_t size);

  ErrCode Flush();

  uint64_t size() { return filesize_; }

  string& filename() { return filename_; }

  void* buffer() { return buffer_; }

  string ToString();
};

}

#endif //PYLON_VMFILE_H
