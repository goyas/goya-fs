#ifndef _FILE_CLIENT_H_
#define _FILE_CLIENT_H_

namespace goya {

namespace fs {
  
class FileClient {
public:
  FileClient() {}
  virtual ~FileClient() {}
  virtual void Write(const char* buf, ssize_t size) = 0;
  virtual void Read(char* buf, ssize_t size) = 0;

private:
  // 禁止拷贝复制
  FileClient(const FileClient&);
  const FileClient& operator=(const FileClient&);
};

class FileSystem {
public:
  FileSystem() {}
  virtual ~FileSystem() {}
  virtual bool StartFileSystem(const char* masterserver) = 0;
  virtual int CreateDirectory(char* path) = 0;
  virtual int ListDirectory(char* path) = 0;

private:
  // 禁止拷贝复制
  FileSystem(const FileSystem&);
  const FileSystem& operator=(const FileSystem&);
};

}

}

#endif

