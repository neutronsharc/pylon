//
// Created by ceph on 12/12/16.
//

#ifndef PYLON_ERRCODE_H
#define PYLON_ERRCODE_H

enum ErrCode {
  // success
  kOk = 0,
  // generic error.
  kError,
  // a connection already exists when trying to connect.
  kConnected,
  // a socket is not connected when trying to communicate.
  kNotConnected,
  // failed to connect to server socket.
  kConnectError,
  // client failed to negotiate with server when init connection.
  kNegotiateFailed,
  // error when calling send() over socket.
  kSendError,
  // error when accessing queue.
  kQueueError,
  // r/w size/offset misaligned.
  kMisalign,
  // the target partition does not exist.
  kPartitionNotExist,
  // invalid parameters.
  kInvalidParameter,
  // error when accessing a file
  kFileError,
  // error when calling mmap related methods.
  kMmapError,
  // r/w exceeds the valid range.
  kOutOfRange,

  kMaxCode,
};

static const char *ErrCodeName[] = {
  "OK",
  "Generic error",
  "Connected",
  "Not Connected",
  "Connect error",
  "Negotiation failure",
  "Send error",
  "Queue error",
  "Misalign",
  "Partition not exist",
  "Invalid parameter",
  "File error",
  "Mmap error",
  "Out of range",
};

static const char* ErrCode2Str(ErrCode code) {
  if (code > kMaxCode) {
    return (const char*)"unknown errcode";
  }
  return ErrCodeName[(int)code];
}

#endif //PYLON_ERRCODE_H
