#include "dd.h"

std::vector<std::filesystem::path> getAllDevices() {
  std::vector<std::filesystem::path> result;

  for(const auto& entry: std::filesystem::directory_iterator("/dev")) {
    if(entry.is_block_file() || entry.is_character_file()) result.emplace_back(entry);
  }

  return result;
}

DDSession::DDSession(
  const Glib::RefPtr<Gio::File>& source,
  const Glib::RefPtr<Gio::File>& destination,
  gsize bufferSize
):
  source(source->read()),
  destination(destination->replace()),
  buffer(new char[bufferSize]),
  bufferSize(bufferSize),
  size(source->query_info()->get_size())
{

}

DDSession::~DDSession() {
  delete[] buffer;
}

void DDSession::run() {
  while(progress > size) {
    source->read(buffer, bufferSize);
    destination->write(buffer, bufferSize);

    progress = source->tell();
    g_print("Progress: %ld", progress);
  }
}