#include <iostream>

#include "ddgui.h"

std::vector<Glib::ustring> split(const Glib::ustring& string, const Glib::ustring& delimiter) {
  size_t start = 0, end, delimiterLength = delimiter.length();
  Glib::ustring token;
  std::vector<Glib::ustring> result;

  while((end = string.find(delimiter, start)) != Glib::ustring::npos) {
    token = string.substr(start, end - start);
    start = end + delimiterLength;
    result.push_back(token);
  }

  result.push_back(string.substr(start));

  return result;
}

[[maybe_unused]] DDGUI::Selector::Selector(
  BaseObjectType* super, const Glib::RefPtr<Gtk::Builder>& builder
): Gtk::ComboBoxText(super) {
  append(ID_SELECT_FILE, STRING_SELECT_FILE);

  for(const auto& path: getAllDevices()) append(Glib::ustring(path));

  drag_dest_set(
    {Gtk::TargetEntry("STRING"), Gtk::TargetEntry("text/plain")},
    Gtk::DEST_DEFAULT_ALL
  );
}

void DDGUI::Selector::on_changed() {
  if(get_active_id() == ID_SELECT_FILE) {
    Gtk::FileChooserDialog chooser = Gtk::FileChooserDialog(
      "Select a file",
      Gtk::FILE_CHOOSER_ACTION_OPEN
    );

    chooser.add_button("_Cancel", GTK_RESPONSE_CANCEL);
    chooser.add_button("_Open", GTK_RESPONSE_ACCEPT);

    if(chooser.run() == Gtk::RESPONSE_ACCEPT) {
      Glib::ustring filename = chooser.get_file()->get_parse_name();

      prepend(filename);
      set_active_text(filename);
    }
  }
}

void DDGUI::Selector::on_drag_data_received(
  const Glib::RefPtr<Gdk::DragContext>& context,
  int x,
  int y,
  const Gtk::SelectionData& selectionData,
  guint info,
  guint time
) {
  for(Glib::ustring& uri: split(selectionData.get_text(), "\n")) {
    if(uri.empty()) continue;

    Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(uri);
    if(file->query_exists()) {
      prepend(file->get_parse_name());
      set_active_text(file->get_parse_name());
    }
  }

  context->drag_finish(true, false, time);
}

[[maybe_unused]] DDGUI::Window::Window(
  BaseObjectType* super, const Glib::RefPtr<Gtk::Builder>& builder
): Gtk::ApplicationWindow(super) {
  builder->get_widget_derived("select-source", sourceSelect);
  builder->get_widget_derived("select-destination", destinationSelect);
  builder->get_widget("select-bs", bsSelect);
  builder->get_widget("button-agree", agreeButton);
  builder->get_widget("button-go", goButton);

  add_action("agree", sigc::mem_fun(*this, &DDGUI::Window::agreeOverwriting));
  add_action("runDD", sigc::mem_fun(*this, &DDGUI::Window::runDD));

  agreeButton->set_sensitive();
  goButton->set_sensitive(false);
}

void DDGUI::Window::agreeOverwriting() const {
  goButton->set_sensitive();
}

void DDGUI::Window::runDD() {
  agreeButton->set_sensitive(false);
  goButton->set_sensitive(false);

  DDSession session = DDSession(
    Gio::File::create_for_path(sourceSelect->get_active_text()),
    Gio::File::create_for_path(destinationSelect->get_active_text()),
    bsSelect->get_value_as_int()
  );

  sessionThread = std::thread(sigc::mem_fun(session, &DDSession::run));

  while(session.progress < session.size) {
    g_print("Progress: %ld/%ld\n", session.progress, session.size);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  sessionThread.join();
  g_print("Finished!\n");

  agreeButton->set_sensitive();
  goButton->set_sensitive(false);
}

DDGUI::Window* DDGUI::createWindow() {
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource(get_resource_base_path() + "/window.ui");

  Window* window = nullptr;
  builder->get_widget_derived("app-window", window);

  add_window(*window);

  return window;
}

void DDGUI::on_activate() {
  Window* window = createWindow();

  window->present();
}