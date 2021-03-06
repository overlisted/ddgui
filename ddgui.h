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
  struct Selector: Gtk::ComboBoxText {
    #define ID_SELECT_FILE "file-select"
    #define STRING_SELECT_FILE "Select a file..."

    [[maybe_unused]] Selector(BaseObjectType* super, const Glib::RefPtr<Gtk::Builder>& builder);

    void on_changed() override;
    void on_drag_data_received(
      const Glib::RefPtr<Gdk::DragContext>& context,
      int x,
      int y,
      const Gtk::SelectionData& selectionData,
      guint info,
      guint time
    ) override;
  };

  struct Window: Gtk::ApplicationWindow {
    Selector* sourceSelect = nullptr;
    Selector* destinationSelect = nullptr;
    Gtk::SpinButton* bsSelect = nullptr;
    Gtk::Button* agreeButton = nullptr;
    Gtk::Button* goButton = nullptr;

    DDSession* ddSession = nullptr;
    std::thread* ddSessionThread = nullptr;
    std::thread* progressThread = nullptr;

    [[maybe_unused]] Window(BaseObjectType* super, const Glib::RefPtr<Gtk::Builder>& builder);

    void progressThreadRun() const;
    void agreeOverwriting() const;
    void runDD();
  };

  DDGUI(): Gtk::Application("net.overlisted.ddgui", Gio::APPLICATION_FLAGS_NONE) {}

  Window* createWindow();

  void on_activate() override;
};