#include <iostream>

#include <gtkmm.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <starter/core/fs/Paths.h>
#include <starter/gui/main-window/MainWindow.h>

static constexpr const char* LOG_NAME = "starter::gui::main";

// TODO How to configure errors to stderr without creating another logger?
// https://github.com/gabime/spdlog/issues/345
static const auto LOG = spdlog::stdout_color_mt(LOG_NAME);

static void init();

int main(int argc, char* argv[]) {
  init();

  const auto app = Gtk::Application::create(argc, argv, "br.com.hugobenicio.starter");

  // Load the GtkBuilder file and instantiate its widgets:
  // The builder reference is managed by the Glib::RefPtr smartpointer
  const auto builder = Gtk::Builder::create();
  try {
    builder->add_from_file("main-window/MainWindow.glade");

  } catch (const Glib::FileError& err) {
    LOG->error("could not create builder for MainWindow.glade: Glib::FileError: {}", err.what().c_str());
    return EXIT_FAILURE;

  } catch (const Glib::MarkupError& err) {
    LOG->error("could not create builder for MainWindow.glade: Glib::MarkupError: {}", err.what().c_str());
    return EXIT_FAILURE;

  } catch (const Gtk::BuilderError& err) {
    LOG->error("could not create builder for MainWindow.glade: Gtk::BuilderError: {}", err.what().c_str());
    return EXIT_FAILURE;
  }

  starter::gui::mainwindow::MainWindow* mainWindow = nullptr;
  builder->get_widget_derived("mainWindow", mainWindow);
  if (!mainWindow) {
    LOG->error("could not get 'mainWindow' widget");
    return EXIT_FAILURE;
  }

  const int rc = app->run(*mainWindow);

  delete mainWindow;

  return rc;
}

static void init() {
  using starter::core::fs::sep;

  LOG->info("app is initializing...");
  
  std::string starterDir = Glib::get_home_dir() + sep + ".starter";

  LOG->info("creating folder '{}' ...", starterDir);
  if (g_mkdir_with_parents(starterDir.c_str(), 0700)) {
    LOG->error("could not create folder '{}': {}", starterDir, g_strerror(errno));
  }
}
