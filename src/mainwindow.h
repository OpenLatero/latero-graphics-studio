#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

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

#endif
