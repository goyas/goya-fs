#include "masterserver_impl.h"
#include "goya/rpc/rpc_server.h"

extern std::string FLAGS_masterserver_port;

int main(int argc, char* argv[]) 
{
  if (argc < 2) {
	printf("Invaild input, please input --port=...\n");
    return -1;
  }
  char s[16];
  if (sscanf(argv[1], "--port=%s", s) != 1) {
    printf("Invaild input %s", argv[1]);
    return -1;
  }
  
  FLAGS_masterserver_port = s;
  
  //goya::rpc::RpcServerOptions options;
  goya::rpc::RpcServer rpc_server();

  // master service
  goya::fs::MasterServer* masterserver_service = new goya::fs::MasterServerImpl();

  if (!rpc_server.RegisterService(masterserver_service, false))
    return -1;

  // start service
  std::string server_hostname = std::string("0.0.0.0:") + FLAGS_masterserver_port;
  if (!rpc_server.Start(server_hostname))
    return -1;

  //rpc_server.Run();

  return 0;
}

