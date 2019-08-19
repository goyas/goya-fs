#include <string>
#include <string.h>
#include "sdk/file_client_impl.h"

using namespace goya::fs;

extern std::string FLAGS_masterserver;

void print_usage() {
  printf("Use:\nfs_client <commond> path\n");
  printf("\t commond:\n");
  printf("\t    -mkdir <path> : make director\n");
  printf("\t    -ls <path> : list the directory\n");
}

void dump(int argc, char** argv) {
  printf("dump\n");
  for (int i = 0; i < argc; ++i) {
    printf("arg[%d]=%s\n", i, argv[i]);
  }
}

class FSOperator {
public:
  FSOperator(int argc, char* argv[]) : argc_(argc), argv_(argv) {}
  virtual ~FSOperator() {}
  
  virtual bool Start(const char* masterserver) {
    return fs_.StartFileSystem(masterserver);
  }
  virtual int Execute() = 0;
  
protected:
  int             argc_;
  char**          argv_;  
  FileSystemImpl  fs_;
};

class FSMkdirImpl : public FSOperator {
public:
  FSMkdirImpl(int argc, char* argv[]) : FSOperator(argc, argv) {}
  virtual ~FSMkdirImpl() {}
  
  int Execute() {
    if (argc_ < 3) {
      print_usage();
      return -1;
    }
    int ret = 0;
    for (int i = 2; i < argc_; ++i) {
      ret |= fs_.CreateDirectory(argv_[i]);
      if (0 != ret) {
        fprintf(stderr, "Create diretory %s fail\n", argv_[i]);
      }
    }

    return ret;
  }
};

class FSLsdirImpl : public FSOperator {
public:
  FSLsdirImpl(int argc, char* argv[]) : FSOperator(argc, argv) {}
  virtual ~FSLsdirImpl() {}
  
  int Execute() {
    
  }
};

class FSCmdFactory {
public:
  FSCmdFactory() {}
  virtual ~FSCmdFactory() {}
  
  FSOperator* CreateOpObject(int argc, char* argv[]) {
    if (0 == strcmp(argv[1], "mkdir")) {
      operator_ = new FSMkdirImpl(argc, argv);
    } else if (0 == strcmp(argv[1], "ls")) {
      operator_ = new FSLsdirImpl(argc, argv);
    } else {
      printf("Unknow common: %s\n", argv[1]);
      return nullptr;
    }
    return operator_;
  }

private:
  FSOperator* operator_;
};

int main(int argc, char* argv[]) {
  FSCmdFactory* factory = new FSCmdFactory();
  FSOperator* opertator = factory->CreateOpObject(argc, argv);
  if (!opertator) {
    fprintf(stderr, "create opertator object fail\n");
    return -1;
  }

  // 1. start prepare resource
  opertator->Start(FLAGS_masterserver.c_str());
  
  // 2. execte
  opertator->Execute();
}

