#include"Libs.hpp"
#include"Console.hpp"
#include"StaticMemory.hpp"

int main(int argc, char* argv[])
{
	//reference 4. console tips
	if (argc == 1) {
		std::cout << eci::console_no_argument;
		return 0;
	}
	auto& console_handle = eci::Console::getInstance();
	auto argumentCheck = [argc]() {
		if (argc == 2) { std::cout << eci::console_too_few_argument; return 0; }
		if (argc >= 4) { std::cout << eci::console_too_many_argument; return 0; }
	};
	switch (console_handle.matchCommand(std::string(argv[1]))) {
	case eci::NEW:
		argumentCheck();
		console_handle.eciNew(std::string(argv[2]));
		std::cout << eci::console_new_completeA << argv[2] << eci::console_new_completeB;
		break;
	case eci::DELETEIT:
		argumentCheck();
		console_handle.eciDelete(std::string(argv[2]));
		std::cout << argv[2] << eci::console_delete_complete;
		break;
	case eci::BUILD:
		argumentCheck();
		//console_handle.eciBuild(std::string(argv[2]));
		break;
	case eci::OPEN:
		argumentCheck();
		console_handle.eciOpen(std::string(argv[2]));
		std::cout << eci::console_open_completeA << argv[2] << eci::console_open_completeB;
		break;
	case eci::WATCH:
		argumentCheck();
		//console_handle.eciWatch(std::string(argv[2]));
		break;
	case eci::HELP:
		if (argc >= 3) { std::cout << eci::console_too_many_argument; return 0; }
		console_handle.eciNew(std::string(argv[2]));
		break;
	}
}

