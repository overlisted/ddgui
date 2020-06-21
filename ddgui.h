#include <gtkmm/application.h>
#include <gtkmm/button.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/builder.h>

#include <thread>

#include "dd.h"

struct DDGUI: Gtk::Application {
  struct Window: Gtk::ApplicationWindow {
    #define ID_SELECT_FILE "file-select"
    #define STRING_SELECT_FILE "Select a file..."

    Gtk::ComboBoxText* sourceSelect = nullptr;
    Gtk::ComboBoxText* destinationSelect = nullptr;
    Gtk::SpinButton* bsSelect = nullptr;
    Gtk::Button* agreeButton = nullptr;
    Gtk::Button* goButton = nullptr;

    std::thread sessionThread;

    [[maybe_unused]] Window(BaseObjectType* super, const Glib::RefPtr<Gtk::Builder>& builder);

    void agreeOverwriting() const;

    void runDD();

    void onSelectChanged(Gtk::ComboBoxText* widget);
    void onSourceChanged() {
      onSelectChanged(sourceSelect);
    }

    void onDestinationChanged() {
      onSelectChanged(destinationSelect);
    }
  };

  DDGUI(): Gtk::Application("net.overlisted.ddgui", Gio::APPLICATION_FLAGS_NONE) {}

  Window* createWindow();

  void on_activate() override;
};