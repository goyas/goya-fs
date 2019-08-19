
#include "masterserver_impl.h"

extern std::string FLAGS_namedb_path;

namespace goya {

namespace fs {

MasterServerImpl::MasterServerImpl() {
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, FLAGS_namedb_path, &db_);
  if (!status.ok()) {
    db_ = NULL;
    printf("Open leveldb fail: %s\n", status.ToString().c_str());
  }
}

MasterServerImpl::~MasterServerImpl() {
}

void MasterServerImpl::CreateFile(google::protobuf::RpcController* controller,
                              const ::goya::fs::CreateFileRequest* request,
                              goya::fs::CreateFileResponse* response,
                              google::protobuf::Closure* done) {
  printf("master create file\n");
  response->set_sequence_id(request->sequence_id());
  const std::string& filename = request->file_name();

  std::string file_value;
  leveldb::Status status;
  status = db_->Get(leveldb::ReadOptions(), filename, &file_value);
  if (status.IsNotFound()) {
    status = db_->Put(leveldb::WriteOptions(), filename, file_value);
    if (status.ok()) {
      printf("CreatFile %s\n", filename.c_str());
      response->set_status(0);
    } else {
      printf("CreateFile %s file\n", filename.c_str());
      response->set_status(2);
    }
  } else {
    printf("CreateFile %s fail: already exist~\n", filename.c_str());
    response->set_status(1);
  }

  done->Run();
}

}
}