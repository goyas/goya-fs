#include "masterserver_impl.h"

extern std::string FLAGS_namedb_path;

namespace goya {
namespace fs {

MasterServerImpl::MasterServerImpl() 
{
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status s = leveldb::DB::Open(options, FLAGS_namedb_path, &db_);
  if (!s.ok()) {
    db_ = NULL;
    printf("Open leveldb fail: %s\n", s.ToString().c_str());
  }
}

MasterServerImpl::~MasterServerImpl() 
{
}

void MasterServerImpl::CreateFile(google::protobuf::RpcController* controller,
  const ::goya::fs::CreateFileRequest* request,
  goya::fs::CreateFileResponse* response,
  google::protobuf::Closure* done) 
{
  printf("masterserver create file\n");
  response->set_sequence_id(request->sequence_id());
  const std::string& filename = request->file_name();
  if (filename.empty() || filename[0] != '/') {
    printf("path format error\n");
    response->set_status(3);
    done->Run();
    return ; 
  }

  std::string file_value;
  leveldb::Status s;
  s = db_->Get(leveldb::ReadOptions(), filename, &file_value);
  if (s.IsNotFound()) {
    FileInfoProto file_info;
    file_info.set_time(time(NULL));
    file_info.set_type(request->type());
    file_info.SerializeToString(&file_value);
    s = db_->Put(leveldb::WriteOptions(), filename, file_value);
    if (s.ok()) {
      printf("CreateFile %s file\n", filename.c_str());
      response->set_status(0);
    } else {
      printf("CreateFile %s file\n", filename.c_str());
      response->set_status(2);
    }
  } else {
    printf("CreateFile %s fail: already exist\n", filename.c_str());
    response->set_status(1);
  }

  done->Run();
}

void MasterServerImpl::ListDirectory(google::protobuf::RpcController* controller,
  const goya::fs::ListDirectoryRequest* request,
  goya::fs::ListDirectoryResponse* response,
  google::protobuf::Closure* done) 
{
  printf("masterserver list directory\n");
  response->set_sequence_id(request->sequence_id());
  std::string path = request->path();
  if (path.empty() || path[0] != '/') {
    printf("path format error\n");
    response->set_status(3);
    done->Run();
    return ; 
  }

  std::string file_key  = path;
  std::string file_value;
  printf("List Directory: %s\n", path.c_str());
  leveldb::Status s;
  s = db_->Get(leveldb::ReadOptions(), file_key, &file_value);
  if (s.IsNotFound()) {
    printf("List Directory: %s isn't exist\n", path.c_str());
    response->set_status(1);    
  } else {
    FileInfoProto* file_info_proto = response->add_file_info_proto();
    file_info_proto->ParseFromString(file_value);
    file_info_proto->set_name(file_key);    
    response->set_status(0);
  }
  
  done->Run();
}
    
}
}

