#include "ddgui.h"

DDGUI::Window::Window(
  BaseObjectType* super,const Glib::RefPtr<Gtk::Builder>& builder
): Gtk::ApplicationWindow(super) {
  builder->get_widget("select-source", sourceSelect);
  builder->get_widget("select-destination", destinationSelect);
  builder->get_widget("select-bs", bsSelect);
  builder->get_widget("button-agree", agreeButton);
  builder->get_widget("button-go", goButton);

  sourceSelect->append(ID_SELECT_FILE, STRING_SELECT_FILE);
  destinationSelect->append(ID_SELECT_FILE, STRING_SELECT_FILE);

  sourceSelect->signal_changed().connect(sigc::mem_fun(this, &DDGUI::Window::onSourceChanged));
  destinationSelect->signal_changed().connect(sigc::mem_fun(this, &DDGUI::Window::onDestinationChanged));

  for(const auto& path: getAllDevices()) {
    Glib::ustring unicoded(path);

    sourceSelect->append(unicoded);
    destinationSelect->append(unicoded);
  }

  add_action("agree", sigc::mem_fun(this, &DDGUI::Window::agreeOverwriting));
  add_action("runDD", sigc::mem_fun(this, &DDGUI::Window::runDD));

  agreeButton->set_sensitive();
  goButton->set_sensitive(false);
}

void DDGUI::Window::agreeOverwriting() const {
  goButton->set_sensitive();
}

void DDGUI::Window::runDD() const {
  DDSession session = DDSession(
    Gio::File::create_for_path(sourceSelect->get_active_text()),
    Gio::File::create_for_path(destinationSelect->get_active_text()),
    bsSelect->get_value_as_int()
  );

  session.run();
}

void DDGUI::Window::onSelectChanged(Gtk::ComboBoxText *widget) {
  if(widget->get_active_id() == ID_SELECT_FILE) {
    Gtk::FileChooserDialog chooser = Gtk::FileChooserDialog(
      *this,
      "Select a file",
      Gtk::FILE_CHOOSER_ACTION_OPEN
    );

    chooser.add_button("_Cancel", GTK_RESPONSE_CANCEL);
    chooser.add_button("_Open", GTK_RESPONSE_ACCEPT);

    if(chooser.run() == Gtk::RESPONSE_ACCEPT) {
      Glib::ustring filename = chooser.get_file()->get_parse_name();

      widget->append(filename);
      widget->set_active_text(filename);
    }
  }
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