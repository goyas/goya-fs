#ifndef _MASTERSERVER_IMPL_H_
#define _MASTERSERVER_IMPL_H_

#include "proto/masterserver.pb.h"
#include <leveldb/db.h>

namespace goya {
namespace fs {

class MasterServerImpl : public MasterServer {
public:
  MasterServerImpl();
  virtual ~MasterServerImpl();

  void CreateFile(google::protobuf::RpcController* controller,
    const goya::fs::CreateFileRequest* request,
    goya::fs::CreateFileResponse* response,
    google::protobuf::Closure* done);

  void ListDirectory(google::protobuf::RpcController* controller,
    const goya::fs::ListDirectoryRequest* request,
    goya::fs::ListDirectoryResponse* response,
    google::protobuf::Closure* done);

private:
  leveldb::DB* db_;
};

}
}

#endif
