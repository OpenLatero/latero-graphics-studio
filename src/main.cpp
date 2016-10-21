#include <iostream>
//#include <sys/mman.h>
#include <gtkmm/main.h>
#include "mainwindow.h"
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>
#include <laterographics/graphics/canvas.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
const float DefaultUpdateRateHz = 750.0;

int main(int argc, char *argv[])
{
	// parse command-line parameters

	// description
	po::options_description desc("Options");
	desc.add_options()
    	("haptic-rate", po::value<int>(), "set haptic update rate")
	("input-file", po::value< std::vector<std::string> >(), "load input file")
   	("disable-audio",po::value<bool>(), "disable audio thread")
	;

	// unnamed argument is interpreted as input-file
	po::positional_options_description p;
	p.add("input-file", -1);

	// parse
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	// convert
	if (vm.count("help")) { std::cout << desc << "\n"; return 1; }
	float UpdateRateHz = DefaultUpdateRateHz; // default
	if (vm.count("haptic-rate")) UpdateRateHz = vm["haptic-rate"].as<int>();
	std::string filename;
	if (vm.count("input-file"))
		filename = vm["input-file"].as< std::vector<std::string> >().at(0); // ignore multiples for now
	bool disableAudio = false;
	if (vm.count("disable-audio")) disableAudio = true;

	latero::Tactograph dev;

	std::cout << "Starting engines...\n";
	latero::graphics::TactileEngine tEngine(&dev, boost::posix_time::microseconds(1E6/UpdateRateHz));
	latero::graphics::AudioEngine aEngine(&dev, boost::posix_time::milliseconds(30));
	tEngine.Start();
	if (!disableAudio) aEngine.Start();

	std::cout << "Creating GUI thread...\n";
	Gtk::Main kit(argc, argv);
	latero::graphics::GeneratorPtr gen;
	if (filename=="")
		gen = latero::graphics::Canvas::Create(&dev);
	else
		gen = latero::graphics::Generator::Create(filename,&dev);
	MainWindow wnd(&tEngine, &aEngine, gen);
	Gtk::Main::run(wnd);

	std::cout << "Stopping engines...\n";
	tEngine.Stop();
	aEngine.Stop();

	return 0;
}
