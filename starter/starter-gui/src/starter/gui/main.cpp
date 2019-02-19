#include <iostream>
#include <gtkmm.h>
#include <starter/core/Starter.h>
#include <starter/gui/main-window/MainWindow.h>

int main(int argc, char* argv[]) {
  starter::core::sayHello();

  auto app = Gtk::Application::create(argc, argv, "br.com.hugobenicio.starter");

  //Load the GtkBuilder file and instantiate its widgets:
  const auto builder = Gtk::Builder::create();
  try {
    builder->add_from_file("main-window/MainWindow.glade");

  } catch (const Glib::FileError& err) {
    std::cerr << "não foi possível criar builder para MainWindow.glade: ";
    std::cerr << "Glib::FileError: " << err.what() << '\n';
    return EXIT_FAILURE;

  } catch (const Glib::MarkupError& err) {
    std::cerr << "não foi possível criar builder para MainWindow.glade: ";
    std::cerr << "Glib::MarkupError: " << err.what() << '\n';
    return EXIT_FAILURE;

  } catch (const Gtk::BuilderError& err) {
    std::cerr << "não foi possível criar builder para MainWindow.glade: ";
    std::cerr << "Gtk::BuilderError: " << err.what() << '\n';
    return EXIT_FAILURE;
  }

  starter::gui::mainwindow::MainWindow* mainWindow = nullptr;
  builder->get_widget_derived("mainWindow", mainWindow);
  if (!mainWindow) {
    std::cerr << "não foi possível obter widget 'mainWindow'\n";
    return EXIT_FAILURE;
  }

  const int rc = app->run(*mainWindow);

  delete mainWindow;

  return rc;
}
