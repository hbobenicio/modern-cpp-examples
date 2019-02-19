#include <starter/gui/main-window/MainWindow.h>

#include <iostream>

namespace starter { namespace gui { namespace mainwindow {

    MainWindow::MainWindow(Gtk::Window::BaseObjectType* gladeBaseWindow, const Glib::RefPtr<Gtk::Builder>& builder)
        : Gtk::Window(gladeBaseWindow)
    {
        std::cout << "[starter::gui::mainwindow::MainWindow::MainWindow] ";
        std::cout << "inicializando widget a partir do builder glade...\n";

        this->getWidgets(builder);
        this->addApps();
    }

    MainWindow::~MainWindow() {
        std::cout << "[starter::gui::mainwindow::MainWindow::~MainWindow] ";
        std::cout << "destruindo MainWindow\n";
    }

    void MainWindow::getWidgets(const Glib::RefPtr<Gtk::Builder>& builder) {
        builder->get_widget("titleLabel", this->titleLabel);
        if (!this->titleLabel) {
            std::cerr << "[starter::gui::mainwindow::MainWindow::getWidgets]";
            std::cerr << "não foi possível carregar widget 'titleLabel'";
        }

        builder->get_widget("mainBox", this->mainBox);
        if (!this->mainBox) {
            std::cerr << "[starter::gui::mainwindow::MainWindow::getWidgets]";
            std::cerr << "não foi possível carregar widget 'mainBox'";
        }

        builder->get_widget("appListBox", this->appListBox);
        if (!this->appListBox) {
            std::cerr << "[starter::gui::mainwindow::MainWindow::getWidgets]";
            std::cerr << "não foi possível carregar widget 'appListBox'";
        }
    }

    void MainWindow::addApps() {
        std::vector<std::string> apps;
        apps.emplace_back("Firefox Quantum");
        apps.emplace_back("Firefox Serpro");
        apps.emplace_back("Gnome Terminal");

        for (const auto app: apps) {
            std::cout << "TODO: adicionar app " << app << " na appListBox...\n";
        }
    }

}}}
