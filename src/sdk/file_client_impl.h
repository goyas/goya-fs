#ifndef _FILE_CLIENT_IMPL_H_
#define _FILE_CLIENT_IMPL_H_

#include <string>
#include <sys/types.h>
#include "file_client.h"
#include "masterserver.pb.h"
#include "common/rpc_wrapper.h"

namespace goya {

namespace fs {

class FileClientImpl : public FileClient {
public:
  virtual void Write(const char * buf, ssize_t size);
  virtual void Read(char * buf, ssize_t size);
};

class FileSystemImpl : public FileSystem {
public:
  FileSystemImpl();
  virtual ~FileSystemImpl();
  bool StartFileSystem(const char* masterserver);
  int CreateDirectory(char* path);
  int ListDirectory(char* path);
  
private:
  std::string         masterserver_addr_;
  MasterServer_Stub*  masterserver_stub_;
  RpcWrapper*         rpc_wrapper_;
};

}

}

#endif
