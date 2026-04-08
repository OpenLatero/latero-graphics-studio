#include "mainwindow.h"
#include <sstream>
#include <iostream>

MainWindow::MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, latero::graphics::GeneratorPtr gen) :
	managerWidget_(tEngine, aEngine,gen)
{
	set_title("Latero Graphics Studio");
	set_size_request(1000,800);
	set_child(managerWidget_);
	signal_realize().connect(sigc::mem_fun(*this, &MainWindow::CreateMenu));
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

	// Let ApplicationWindow manage the menubar natively
	auto menu_model = std::dynamic_pointer_cast<Gio::Menu>(builder->get_object("MenuBar"));
	get_application()->set_menubar(menu_model);
	set_show_menubar(true);
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


	
