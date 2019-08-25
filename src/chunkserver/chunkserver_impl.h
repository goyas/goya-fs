#ifndef _CHUNKSERVER_IMPL_H_
#define _CHUNKSERVER_IMPL_H_

#include "proto/chunkserver.pb.h"

namespace goya {

namespace fs {

class ChunkServerImpl : public ChunkServer {
public:
  ChunkServerImpl() {}
  virtual ~ChunkServerImpl() {}
};

}
}

#endif
