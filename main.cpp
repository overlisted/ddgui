#include "gtkmm.h"

struct DDGUI: Gtk::Application {
  struct Window: Gtk::ApplicationWindow {
    [[maybe_unused]] Window(
      BaseObjectType* super,
      const Glib::RefPtr<Gtk::Builder>& builder
    ): Gtk::ApplicationWindow(super) {

    }
  };

  DDGUI(): Gtk::Application("net.overlisted.ddgui", Gio::APPLICATION_FLAGS_NONE) {}

  Window* createWindow() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource(get_resource_base_path() + "/window.ui");

    Window* window = nullptr;
    builder->get_widget_derived("app-window", window);

    add_window(*window);

    return window;
  }

  void on_activate() override {
    Window* window = createWindow();

    window->present();
  }
};

int main(int argc, char* argv[]) {
  DDGUI app = DDGUI();

  return app.run(argc, argv);
}