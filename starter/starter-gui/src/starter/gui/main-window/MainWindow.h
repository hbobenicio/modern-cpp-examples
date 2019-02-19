#ifndef STARTER_GUI_MAIN_WINDOW_H
#define STARTER_GUI_MAIN_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/listbox.h>

namespace starter { namespace gui { namespace mainwindow {

    class MainWindow: public Gtk::Window
    {
    private:
        Gtk::Label* titleLabel = nullptr;
        Gtk::Box* mainBox = nullptr;
        Gtk::ListBox* appListBox = nullptr;

    public:
        MainWindow(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
        virtual ~MainWindow();

    private:
        void getWidgets(const Glib::RefPtr<Gtk::Builder>& builder);
        void addApps();
    };

}}}

#endif
