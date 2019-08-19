
#include <stdio.h>
#include "file_client_impl.h"

extern std::string FLAGS_masterserver;

namespace goya {

namespace fs {
  
void FileClientImpl::Write(const char* buf, ssize_t size) {
  // TODO
}

void FileClientImpl::Read(char* buf, ssize_t size) {
  // TODO
}

FileSystemImpl::FileSystemImpl() {
  masterserver_stub_  = nullptr;
  rpc_wrapper_        = nullptr;
}

FileSystemImpl::~FileSystemImpl() {
  delete masterserver_stub_;
  delete rpc_wrapper_;
}

bool FileSystemImpl::StartFileSystem(const char* masterserver) {
  masterserver_addr_ = FLAGS_masterserver;
  if (nullptr != masterserver)
    masterserver_addr_ = masterserver;

  rpc_wrapper_ = new RpcWrapper();
  bool ret = rpc_wrapper_->GetStub(masterserver_addr_, masterserver_stub_);

  return ret;
}

int FileSystemImpl::CreateDirectory(char* path) {
  printf("Create directory %s\n", path);
  CreateFileRequest  request;
  CreateFileResponse response;
  request.set_sequence_id(0);
  request.set_file_name(path);
  bool ret = rpc_wrapper_->SendRequest(masterserver_stub_, &MasterServer_Stub::CreateFile,
            &request, &response, 5, 3);
  if (!ret || response.status() != 0) {
    printf("Create directory fail\n");
    return -1;
  }
  
  return 0;
}

int FileSystemImpl::ListDirectory(char* path) {
  // TODO
}

}
}

