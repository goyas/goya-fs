#include <string>
#include <string.h>
#include "sdk/file_client_impl.h"

using namespace goya::fs;

extern std::string FLAGS_masterserver;

void Usage() {
  printf("Use:\nfs_client <commond> path\n");
  printf("\t commond:\n");
  printf("\t mkdir <path> : make director\n");
  printf("\t ls <path> : list directory\n");
}

void Dump(int argc, char** argv) {
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
      Usage();
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
    if (argc_ < 3) {
      Usage();
      return -1;
    }
    
    fsFileInfo* files = nullptr;
    char* path = argv_[2];
    int ret = fs_.ListDirectory(path, files);
    if (ret) {
      fprintf(stderr, "List diretory %s fail\n", path);
      return ret;
    }
    
    int32_t type = files->mode;
    time_t  time = files->time;
    char permission[16] = "drwxrwxrwx";
    for (int i = 0; i < 10; ++i) {
      if ((type & (1 << (9 - i))) == 0)
        permission[i] = '-';
    }
    struct tm* tm_s = localtime(&time);
    char time_buf[64];
    snprintf(time_buf, sizeof(time_buf), "%4d-%02d-%02d %2d:%02d",
      tm_s->tm_year + 1900, tm_s->tm_mon + 1, tm_s->tm_mday, tm_s->tm_hour, tm_s->tm_min);
    printf("%s\t%s %s\n", permission, time_buf, files->filename.c_str());

    return ret;
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

int main(int argc, char* argv[]) 
{
  if (argc < 2) {
    Usage();
    return -1;
  }
  
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

