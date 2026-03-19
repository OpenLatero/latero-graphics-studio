#ifndef PIEZO_MANAGER_WIDGET_H
#define PIEZO_MANAGER_WIDGET_H

#include <gtkmm.h>
#include <laterographics/virtualsurfacewidget.h>
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>

class ManagerWidget : public Gtk::Frame
{
public:
    ManagerWidget(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, latero::graphics::GeneratorPtr gen /*= latero::graphics::GeneratorPtr()*/);
	virtual ~ManagerWidget() {};

	void Save();
	void Open();
	void Close();

protected:
	bool OnKeyPress(guint keyval, guint keycode, Gdk::ModifierType state);
	void AddGenerator(latero::graphics::GeneratorPtr gen);
	void UpdateCurrentGenerator();
	void OnPageSwitch(Gtk::Widget* page, guint page_num);

	Gtk::Notebook notebook_;
	latero::graphics::VirtualSurfaceWidget preview_;
	latero::graphics::TactileEngine *tEngine_;
	latero::graphics::AudioEngine *aEngine_;
	std::vector<latero::graphics::GeneratorPtr> list_;
	latero::graphics::GeneratorPtr currentGen_;
};

#endif
