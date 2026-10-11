#include "mainwindow.h"
#include <sstream>
#include <iostream>

MainWindow::MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, latero::graphics::GeneratorPtr gen) :
	mainbox_(Gtk::Orientation::VERTICAL),
	managerWidget_(tEngine, aEngine,gen)
{
	set_title("Latero Graphics Studio");
	set_size_request(1000,800);
	set_child(mainbox_);
	CreateMenu();
	managerWidget_.set_vexpand(true);
	mainbox_.append(managerWidget_);
	maximize();
}

void MainWindow::CreateMenu()
{
	// Register actions directly on the window so they're accessible as win.*
	add_action("open",  sigc::mem_fun(*this, &MainWindow::OnOpen));
	add_action("save",  sigc::mem_fun(*this, &MainWindow::OnSave));
	add_action("close", sigc::mem_fun(*this, &MainWindow::OnClose));

	// Define the menubar using Builder XML
	auto builder = Gtk::Builder::create_from_string(R"(
	<?xml version="1.0" encoding="UTF-8"?>
	<interface>
  	<menu id="MenuBar">
    <submenu>
      <attribute name="label">File</attribute>
      <item>
        <attribute name="label">Open</attribute>
        <attribute name="action">win.open</attribute>
      </item>
      <item>
        <attribute name="label">Save</attribute>
        <attribute name="action">win.save</attribute>
      </item>
      <item>
        <attribute name="label">Close</attribute>
        <attribute name="action">win.close</attribute>
      </item>
    </submenu>
  	</menu>
	</interface>
	)");

	auto menu_model = std::dynamic_pointer_cast<Gio::Menu>(builder->get_object("MenuBar"));

#ifdef __APPLE__
	// macOS uses the global menubar, which requires the window to be attached to the application
	signal_realize().connect([this, menu_model]{
		get_application()->set_menubar(menu_model);
		set_show_menubar(true);
	});
#else
	// elsewhere, embed the menubar in the window
	mainbox_.append(*Gtk::make_managed<Gtk::PopoverMenuBar>(menu_model));
#endif
}

MainWindow::~MainWindow() 
{
}

void MainWindow::OnOpen()
{
	managerWidget_.Open();
}

void MainWindow::OnSave()
{
	managerWidget_.Save();
}

void MainWindow::OnClose()
{
	managerWidget_.Close();
}


	
