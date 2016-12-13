#include <iostream>
#include <memory>

#include <gflags/gflags.h>
#include <glog/logging.h>


#include "hexdump_wrapper.h"
#include "vm_file.h"
#include "utils.h"

using namespace hcdnbd;

// we don't support 'localhost' name lookup now.
DEFINE_string(file, "", "file full path");
DEFINE_string(size, "", "file size, xKMGT bytes");

bool CheckCli() {
  if (FLAGS_file.size() <= 0) {
    LOG(ERROR) << "must pass file name";
    return false;
  }

  if (FLAGS_size.size() <= 0) {
    LOG(ERROR) << "must pass file size";
    return false;
  }
  return true;
}

void FileTest(VmFile* file) {
  int blksize = 1024 * 1024;

  int bufsize = 4096;
  char buf[bufsize];

  uint64_t filesize = file->size();
  uint64_t blks = filesize / blksize;

  for (uint64_t i = 0; i < blks; i++) {
    memset(buf, i + 2, bufsize);
    if (file->Write(buf, bufsize, i * blksize) != kOk) {
      LOG(ERROR) << "failed to write file at offset " << i * blksize;
    }
  }

}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (!CheckCli()) return -1;

  std::unique_ptr<VmFile> vmfile(new VmFile(FLAGS_file));
  uint64_t filesize = KMGToValue((char*)FLAGS_size.c_str());

  if (vmfile->Init(filesize) != kOk) {
    LOG(ERROR) << "failed to init file";
    return -1;
  }

  FileTest(vmfile.get());

  /*
  string os;
  char buf[500];
  strcpy(buf, "this is xza test str");
  HexDumpBuffer(buf, strlen(buf), &os);
  cout << os;
   */
  return 0;
}