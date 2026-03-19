#include "managerwidget.h"
#include <assert.h>
#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <laterographics/generator.h>

ManagerWidget::ManagerWidget(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, latero::graphics::GeneratorPtr gen) :
	preview_(tEngine->Dev(), latero::graphics::GeneratorPtr(), true),
	tEngine_(tEngine),
	aEngine_(aEngine)
{
	preview_.ShowCursor();
	auto box = manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
	Gtk::Expander *exp = manage(new Gtk::Expander("settings"));
	exp->set_expanded(true);
	set_child(*box);
	box->append(preview_);
	preview_.set_expand(true);
	box->append(*exp);
	exp->set_vexpand(false);

	exp->set_child(notebook_);

	notebook_.signal_switch_page().connect(
		sigc::mem_fun(*this,&ManagerWidget::OnPageSwitch));

	AddGenerator(gen);

	auto controller = Gtk::EventControllerKey::create();
	controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &ManagerWidget::OnKeyPress), false);
	add_controller(controller);
};

void ManagerWidget::OnPageSwitch(Gtk::Widget* page, guint page_num)
{
	UpdateCurrentGenerator();
}

void ManagerWidget::Save()
{
	auto dialog = Gtk::FileDialog::create();
	dialog->set_title("Please select a generator file.");
	dialog->set_initial_name("card.gen");

	auto window = dynamic_cast<Gtk::Window*>(get_root());
	dialog->save(*window, [this, dialog](Glib::RefPtr<Gio::AsyncResult>& result) {
		try {
			auto file = dialog->save_finish(result);
			if (currentGen_) currentGen_->SaveToFile(file->get_path());
		} catch (const Gtk::DialogError&) {}
	});
}

void ManagerWidget::Close()
{
	int page = notebook_.get_current_page();
	if (page>=0)
	{
		list_.erase(list_.begin()+page);
		notebook_.remove_page(page);
		UpdateCurrentGenerator();
		preview_.Clear(0xffffffff);
	}
}


void ManagerWidget::Open()
{
	auto dialog = Gtk::FileDialog::create();
	dialog->set_title("Please select a generator file.");

	auto filter = Gtk::FileFilter::create();
	filter->add_pattern("*.gen");
	auto filters = Gio::ListStore<Gtk::FileFilter>::create();
	filters->append(filter);
	dialog->set_filters(filters);

	auto window = dynamic_cast<Gtk::Window*>(get_root());
	dialog->open(*window, [this, dialog](Glib::RefPtr<Gio::AsyncResult>& result) {
		try {
			auto file = dialog->open_finish(result);
			AddGenerator(latero::graphics::Generator::Create(file->get_path(), tEngine_->Dev()));
		} catch (const Gtk::DialogError&) {}
	});
}

void ManagerWidget::AddGenerator(latero::graphics::GeneratorPtr gen)
{
	list_.push_back(gen);
	int i = notebook_.append_page(*manage(gen->CreateWidget(gen)), "generator");
	notebook_.set_current_page(i);
	UpdateCurrentGenerator();
}

void ManagerWidget::UpdateCurrentGenerator()
{
	int page = notebook_.get_current_page();
	if (page<0)
		currentGen_.reset();
	else  
		currentGen_ = list_[page];

	preview_.SetGenerator(currentGen_);
	tEngine_->SetGenerator(currentGen_);
	aEngine_->SetGenerator(currentGen_);
}

bool ManagerWidget::OnKeyPress(guint keyval, guint keycode, Gdk::ModifierType state)
{
	if (currentGen_)
        return currentGen_->OnKeyPress(keyval, keycode, state);
    else
        return false;
}
