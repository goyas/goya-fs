#include "masterserver_impl.h"
#include "sofa/pbrpc/pbrpc.h"

extern std::string FLAGS_masterserver_port;

int main(int argc, char* argv[]) 
{
  char s[16];
  if (sscanf(argv[1], "--port=%s", s) != 1) {
    printf("Invaild input %s", argv[1]);
    exit(1);
  }
  
  FLAGS_masterserver_port = s;
  
  sofa::pbrpc::RpcServerOptions options;
  sofa::pbrpc::RpcServer rpc_server(options);

  // master service
  goya::fs::MasterServer* masterserver_service = new goya::fs::MasterServerImpl();

  if (!rpc_server.RegisterService(masterserver_service))
    return -1;

  // start service
  std::string server_hostname = std::string("0.0.0.0:") + FLAGS_masterserver_port;
  if (!rpc_server.Start(server_hostname))
    return -1;

  rpc_server.Run();

  return 0;
}

