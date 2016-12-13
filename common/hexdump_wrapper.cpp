//
// Created by Shawn on 12/13/16.
//

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "hexdump_wrapper.h"


void HexDumpBuffer(void* buffer, int size, string* outstr) {
  int error;
  char *fmt = (char*)HEXDUMP_C;
  int flags = 0;

  struct hexdump *hd = hxd_open(&error);
  if (!hd) {
    LOG(ERROR) << "failed to init hexdump";
    return;
  }

  error = hxd_compile(hd, fmt, flags);
  if (error) {
    LOG(ERROR) << "failed to compile hexdump " << hxd_strerror(error);
    hxd_close(hd);
    return;
  }

  error = hxd_write(hd, buffer, size);
  if (error) {
    LOG(ERROR) << "failed to compile hexdump " << hxd_strerror(error);
    hxd_close(hd);
    return;
  }

  // tell hxd to flush the tail of internal buf.
  hxd_flush(hd);

  int len;
  int buflen = 512;
  char buf[buflen];
  while ((len = hxd_read(hd, buf, buflen))) {
    outstr->append(buf, len);
  }
  sprintf(buf, "    has converted %d bytes\n", size);
  outstr->append(buf);

  hxd_close(hd);
}