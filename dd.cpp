#include "dd.h"

DDSession::DDSession(
  Gio::File* source, Gio::File* destination, size_t bufferSize
): source(source), destination(destination), bufferSize(bufferSize), buffer(new char[bufferSize]) {

}

DDSession::~DDSession() {
  delete source;
  delete destination;
  delete[] buffer;
}

void DDSession::run() {

}

void DDSession::cancel() {

}