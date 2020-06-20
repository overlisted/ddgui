#include <giomm.h>
#include <filesystem>

std::vector<std::filesystem::path> getAllDevices();

struct DDSession {
  Glib::RefPtr<Gio::FileInputStream> source;
  Glib::RefPtr<Gio::FileOutputStream> destination;

  const gsize size;
  goffset progress = 0;

  char* buffer;
  size_t bufferSize;

  DDSession(const Glib::RefPtr<Gio::File>& source, const Glib::RefPtr<Gio::File>& destination, gsize bufferSize);
  ~DDSession();

  void run();
};