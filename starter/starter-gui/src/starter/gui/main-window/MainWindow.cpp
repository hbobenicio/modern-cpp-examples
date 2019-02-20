#include <starter/gui/main-window/MainWindow.h>

#include <iostream>

#include <spdlog/sinks/stdout_color_sinks.h>

#include <starter/core/AppEntry.h>

namespace starter { namespace gui { namespace mainwindow {

    static constexpr const char* LOG_NAME = "starter::gui::mainwindow::MainWindow";

    // TODO How to configure errors to stderr without creating another logger?
    // https://github.com/gabime/spdlog/issues/345
    static const auto LOG = spdlog::stdout_color_mt(LOG_NAME);

    MainWindow::MainWindow(Gtk::Window::BaseObjectType* gladeBaseWindow, const Glib::RefPtr<Gtk::Builder>& builder)
        : Gtk::Window(gladeBaseWindow)
    {
        LOG->info("constructing/initializing MainWindow from glade file...");

        this->getWidgets(builder);
        this->addApps();
    }

    MainWindow::~MainWindow() {
        LOG->debug("~MainWindow");
    }

    void MainWindow::getWidgets(const Glib::RefPtr<Gtk::Builder>& builder) {
        builder->get_widget("titleLabel", this->titleLabel);
        if (!this->titleLabel) {
            LOG->error("could not get widget: 'titleLabel'");
        }

        builder->get_widget("mainBox", this->mainBox);
        if (!this->mainBox) {
            LOG->error("could not get widget: 'mainBox'");
        }

        builder->get_widget("appListBox", this->appListBox);
        if (!this->appListBox) {
            LOG->error("could not get widget: 'appListBox'");
        }
    }

    void MainWindow::addApps() {
        using starter::core::AppEntry;

        std::vector<AppEntry> apps = AppEntry::mockEntries();

        for (const auto app: apps) {
            LOG->warn("TODO: add {} in the appListBox", app.name);

            auto row = Gtk::manage(new Gtk::ListBoxRow());
            this->appListBox->append(*row);
        }
    }

}}}
