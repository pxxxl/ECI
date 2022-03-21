#pragma once

#include"Libs.hpp"

namespace eci {
	namespace text {

		using command = int;
		constexpr command NEW = 1;
		constexpr command DELETEIT = 2;
		constexpr command BUILD = 3;
		constexpr command OPEN = 4;
		constexpr command WATCH = 5;
		constexpr command HELP = 6;

		const char* eci_new = "new";
		const char* eci_delete = "delete";
		const char* eci_build = "build";
		const char* eci_open = "open";
		const char* eci_watch = "watch";
		const char* eci_help = "help";

		const char* console_no_argument =
			"\nusage: \n"
			"   eci new    [circuit_name]\n"
			"   eci delete [circuit_name]\n"
			"   eci build  [circuit_name]\n"
			"   eci open   [circuit_name]\n"
			"   eci watch  [circuit_name]\n"
			"   eci help\n\n"
			"Those above are all eci commands you can use\n"
			"See  github.com/pxxxl/ECI  for a more detailed tutorial"
			;
		const char* console_too_few_argument =
			"Too few arguments for this command"
			"\nusage: \n"
			"   eci new    [circuit_name]\n"
			"   eci delete [circuit_name]\n"
			"   eci build  [circuit_name]\n"
			"   eci open   [circuit_name]\n"
			"   eci watch  [circuit_name]\n"
			"   eci help\n\n"
			"See  github.com/pxxxl/ECI  for a more detailed tutorial";
		const char* console_too_many_argument =
			"Too many arguments for this command"
			"\nusage: \n"
			"   eci new    [circuit_name]\n"
			"   eci delete [circuit_name]\n"
			"   eci build  [circuit_name]\n"
			"   eci open   [circuit_name]\n"
			"   eci watch  [circuit_name]\n"
			"   eci help\n\n"
			"See  github.com/pxxxl/ECI  for a more detailed tutorial\n";
		const char* console_new_completeA = "new blank circuit has been constructed!\n"
			"now type \"eci open "
			;
		const char* console_new_completeB = "\" to edit it!\n\n"
			"See  github.com/pxxxl/ECI  for a more detailed tutorial\n"
			;
		const char* console_delete_complete = "deleted\n";
		const char* console_open_completeA = "When editing completed, type \"eci build ";
		const char* console_open_completeB = "\" to build the circuit\n";
		const char* ei_watch = "watch";
		const char* ei_help = "help";
	}
	using namespace text;

	//Singleton
	//Command from the Console
	class Console {
	public:
		Console(const Console&) = delete;
		Console(const Console&&) = delete;
		static Console& getInstance() {
			static Console instance;
			return instance;
		}

		//Match command
		command matchCommand(std::string command_input);

		//Commands
		void eciNew(const std::string& progName) const;
		void eciDelete(const std::string& progName) const;
		void eciBuild(const std::string& progName) const;
		void eciOpen(const std::string progName) const;
		void eciWatch(const std::string progName) const;
		void eciHelp() const noexcept;

	private:
		Console() = default;
	};

	inline command eci::Console::matchCommand(std::string command_input)
	{
		if (command_input == std::string(eci_new)) {
			return NEW;
		}
		else if (command_input == std::string(eci_delete)) {
			return DELETEIT;
		}
		else if (command_input == std::string(eci_build)) {
			return BUILD;
		}
		else if (command_input == std::string(eci_open)) {
			return OPEN;
		}
		else if (command_input == std::string(eci_watch)) {
			return WATCH;
		}
		else if (command_input == std::string(eci_help)) {
			return HELP;
		}
		return command();
	}

	inline void Console::eciNew(const std::string& progName) const
	{

		auto path = std::filesystem::current_path();
		path = path.parent_path();
		path.append(std::string("storage//" + progName));
		std::filesystem::create_directory(path);
		std::string source = path.string() + std::string("//source.txt");
		std::string result = path.string() + std::string("//result.txt");
		std::string copy = path.string() + std::string("//copy.txt");
		std::ofstream creator;
		creator.open(source, std::ofstream::out);
		creator.close();
		creator.open(result, std::ofstream::out);
		creator.close();
		creator.open(copy, std::ofstream::out);
		creator.close();
		return;
	}

	inline void Console::eciDelete(const std::string& progName) const
	{
		auto path = std::filesystem::current_path();
		path = path.parent_path();
		path.append(std::string("storage//" + progName));
		std::filesystem::remove_all(path);
		return;
	}

	inline void Console::eciBuild(const std::string& progName) const
	{
	}

	inline void Console::eciOpen(const std::string progName) const
	{
		auto path = std::filesystem::current_path();
		path = path.parent_path();
		path.append(std::string("storage//" + progName + std::string("//source.txt")));
		auto command = std::string("notepad ") + path.string();
		system(command.c_str());
		return;
	}



}