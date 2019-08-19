cmake_minimum_required(VERSION 3.9)
project(goya VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src)
link_directories(${PROJECT_SOURCE_DIR}/thirdparty/install/lib)

add_library(goya "")
target_sources(goya
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/sdk/file_client.h"
    "${PROJECT_SOURCE_DIR}/src/sdk/file_client_impl.h"
    "${PROJECT_SOURCE_DIR}/src/sdk/file_client_impl.cc"
    "${PROJECT_SOURCE_DIR}/src/common/flags.cc"
    "${PROJECT_SOURCE_DIR}/build/masterserver.pb.cc"
    "${PROJECT_SOURCE_DIR}/build/masterserver.pb.h"
    "${PROJECT_SOURCE_DIR}/build/chunkserver.pb.cc"
    "${PROJECT_SOURCE_DIR}/build/chunkserver.pb.h"
)
#target_link_libraries(goya ${PROTOBUF_LIBRARIES})

message(STATUS "*****project binary: " ${PROJECT_BINARY_DIR})

# protobuf path
set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/thirdparty/install)
#set(CMAKE_PREFIX_PATH  ${PROTO_PATH})
#set(CMAKE_INCLUDE_PATH ${PROTO_PATH}/include)
#set(CMAKE_LIBRARY_PATH ${PROTO_PATH}/lib)
message(STATUS "*****cmake prefix: " ${CMAKE_PREFIX_PATH})

find_package(Protobuf REQUIRED)

# ${PROTOBUF_INCLUDE_DIRS}已经把install安装的第三方的头文件给全部包含
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})

message(STATUS "*****protobuf include: " ${PROTOBUF_INCLUDE_DIRS})
message(STATUS "*****cmake source: "  ${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB protofiles "${CMAKE_CURRENT_SOURCE_DIR}/src/proto/*.proto")
PROTOBUF_GENERATE_CPP(PROTOSRCS PROTOHDRS ${protofiles})
message(STATUS ${protofiles})
message(STATUS ${PROTOSRCS}, ${PROTOHDRS})

# proto file
add_library(proto "")
target_sources(proto
  PRIVATE
    "${PROJECT_SOURCE_DIR}/build/masterserver.pb.cc"
    "${PROJECT_SOURCE_DIR}/build/masterserver.pb.h"
    "${PROJECT_SOURCE_DIR}/build/chunkserver.pb.cc"
    "${PROJECT_SOURCE_DIR}/build/chunkserver.pb.h"
)

# master server
add_executable(masterserver "")
target_sources(masterserver
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/master/master_main.cc"
    "${PROJECT_SOURCE_DIR}/src/master/masterserver_impl.cc"
    "${PROJECT_SOURCE_DIR}/src/master/masterserver_impl.h"
    "${PROJECT_SOURCE_DIR}/build/masterserver.pb.cc"
    "${PROJECT_SOURCE_DIR}/build/masterserver.pb.h"
    "${PROJECT_SOURCE_DIR}/src/common/flags.cc"
)
target_link_libraries(masterserver ${PROTOBUF_LIBRARIES} leveldb sofa-pbrpc z snappy) 
message(STATUS "*****protobuf libs: " ${PROTOBUF_LIBRARIES})

# chunk server
add_executable(chunkserver "")
target_sources(chunkserver
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/chunkserver/chunkserver_main.cc"
    "${PROJECT_SOURCE_DIR}/src/chunkserver/chunkserver_impl.cc"
    "${PROJECT_SOURCE_DIR}/src/chunkserver/chunkserver_impl.h"
    "${PROJECT_SOURCE_DIR}/build/chunkserver.pb.cc"
    "${PROJECT_SOURCE_DIR}/build/chunkserver.pb.h"
)
target_link_libraries(chunkserver ${PROTOBUF_LIBRARIES})

add_executable(fs_client "")
target_sources(fs_client
  PRIVATE 
    "${PROJECT_SOURCE_DIR}/src/client/fs_client.cc"
    "${PROJECT_SOURCE_DIR}/src/common/flags.cc"
)
target_link_libraries(fs_client goya sofa-pbrpc z snappy ${PROTOBUF_LIBRARIES})

