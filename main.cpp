#include "gtkmm.h"
#include "dd.h"

struct DDGUI: Gtk::Application {
  struct Window: Gtk::ApplicationWindow {
    Gtk::ComboBoxText* sourceSelect = nullptr;
    Gtk::ComboBoxText* destinationSelect = nullptr;
    Gtk::SpinButton* bsSelect = nullptr;
    Gtk::Button* agreeButton = nullptr;
    Gtk::Button* goButton = nullptr;

    [[maybe_unused]] Window(
      BaseObjectType* super,
      const Glib::RefPtr<Gtk::Builder>& builder
    ): Gtk::ApplicationWindow(super) {
      builder->get_widget("select-source", sourceSelect);
      builder->get_widget("select-destination", destinationSelect);
      builder->get_widget("select-bs", bsSelect);
      builder->get_widget("button-agree", agreeButton);
      builder->get_widget("button-go", goButton);

      #define STRING_SELECT_FILE "Select a file..."
      sourceSelect->append(STRING_SELECT_FILE);
      destinationSelect->append(STRING_SELECT_FILE);

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

    void agreeOverwriting() const {
      goButton->set_sensitive();
    }

    void runDD() const {
      auto* session = new DDSession(
        Gio::File::create_for_path(sourceSelect->get_active_text()),
        Gio::File::create_for_path(destinationSelect->get_active_text()),
        bsSelect->get_value_as_int()
      );

      session->run();

      delete session;
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