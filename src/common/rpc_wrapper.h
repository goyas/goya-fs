// 目前rpc模块使用baidu sofa-pbrpc框架，待以后自己实现

#ifndef _RPC_WRAPPER_H_
#define _RPC_WRAPPER_H_

#include <string>
#include <map>
#include <unistd.h>
#include "masterserver.pb.h"
#include "sofa/pbrpc/pbrpc.h"

namespace goya {

namespace fs {

typedef void(MasterServer_Stub::*Callback)(::google::protobuf::RpcController* controller,
  const CreateFileRequest*, CreateFileResponse*, ::google::protobuf::Closure* done);
  
//typedef void(MasterServer_Stub::*Fptr)(google::protobuf::RpcController*, 
//  const CreateFileRequest*, CreateFileResponse*, Callback*);

typedef std::map<std::string, MasterServer_Stub*> ServerMapTypeDef;

class RpcWrapper {
public:
  //typedef void(MasterServer_Stub::*Callback)(::google::protobuf::RpcController* controller,
  //const CreateFileRequest*, CreateFileResponse*, ::google::protobuf::Closure* done);
  
  bool GetStub(std::string server_addr, MasterServer_Stub*& stub) {
    ServerMapTypeDef::iterator iter = server_map_.find(server_addr);
    if (iter != server_map_.end()) {
      stub = iter->second;
      return true;
    } 
    
    sofa::pbrpc::RpcChannelOptions channel_options;
    sofa::pbrpc::RpcChannel* channel = 
            new sofa::pbrpc::RpcChannel(&rpc_client_, server_addr, channel_options);
    stub = new MasterServer_Stub(channel);
    server_map_[server_addr] = stub;

    return true;
  }
  
  bool SendRequest(MasterServer_Stub* stub, Callback callback, 
                    const CreateFileRequest* request, 
                    CreateFileResponse* response, 
                    int timeout, int retry_times) {
    sofa::pbrpc::RpcController controller;
    controller.SetTimeout(timeout);
    (stub->*callback)(&controller, request, response, nullptr);
    for (int retry = 0; retry < retry_times; ++retry) {
      if (controller.Failed()) {
        printf("Send failed, retry ...\n");
        usleep(100000);
      } else {
        printf("Send success ...\n");
        return true;
      }
      controller.Reset();
    }

    return false;
  }

private:
  sofa::pbrpc::RpcClient rpc_client_;
  sofa::pbrpc::RpcClientOptions client_options_;
  ServerMapTypeDef server_map_;
};

}
}

#endif

