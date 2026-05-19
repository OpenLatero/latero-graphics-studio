#pragma once

#include "managerwidget.h"
#include <gtkmm.h>

class MainWindow : public Gtk::ApplicationWindow
{
public:
	MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, latero::graphics::GeneratorPtr gen);
	virtual ~MainWindow();

protected:
	void OnOpen();
	void OnSave();
	void OnClose();

	void CreateMenu();

	ManagerWidget managerWidget_;
};
