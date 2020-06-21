#include <glibmm/refptr.h>
#include <giomm/file.h>

#include <filesystem>

std::vector<std::filesystem::path> getAllDevices();

struct DDSession {
  const Glib::RefPtr<Gio::FileInputStream> source;
  const Glib::RefPtr<Gio::FileOutputStream> destination;
  const gsize bufferSize;
  const gsize size;

  goffset progress = 0;

  char* buffer;

  DDSession(const Glib::RefPtr<Gio::File>& source, const Glib::RefPtr<Gio::File>& destination, gsize bufferSize);
  ~DDSession();

  void run();
};