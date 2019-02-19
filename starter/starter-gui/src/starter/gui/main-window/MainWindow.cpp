#include <starter/gui/main-window/MainWindow.h>

#include <iostream>

namespace starter { namespace gui { namespace mainwindow {

    MainWindow::MainWindow(Gtk::Window::BaseObjectType* gladeBaseWindow, const Glib::RefPtr<Gtk::Builder>& builder)
        : Gtk::Window(gladeBaseWindow)
    {
        std::cout << "[starter::gui::mainwindow::MainWindow::MainWindow] ";
        std::cout << "inicializando widget a partir do builder glade...\n";
    }

    MainWindow::~MainWindow() {
        std::cout << "[starter::gui::mainwindow::MainWindow::~MainWindow] ";
        std::cout << "destruindo MainWindow\n";
    }

}}}
