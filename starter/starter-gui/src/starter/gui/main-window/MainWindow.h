#ifndef STARTER_GUI_MAIN_WINDOW_H
#define STARTER_GUI_MAIN_WINDOW_H

#include <gtkmm.h>

namespace starter { namespace gui { namespace mainwindow {

    class MainWindow: public Gtk::Window
    {
    public:
        MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
        virtual ~MainWindow();
    };

}}}

#endif
