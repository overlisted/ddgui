#include <giomm.h>

struct DDSession {
  Gio::File* source;
  Gio::File* destination;

  size_t size;
  size_t progress;

  char* buffer;
  size_t bufferSize;

  DDSession(Gio::File* source, Gio::File* destination, size_t bufferSize);
  ~DDSession();

  void run();
  void cancel();
};