#pragma once

#include"Libs.hpp"
#include"StaticMemory.hpp"

namespace eci {
	//Singleton
	class Writer
	{
	public:
		static Writer& getInstance() {
			static Writer Writer;
			return Writer;
		}
		Writer(const Writer&) = delete;
		Writer(const Writer&&) = delete;

		//read a line from file, store the string in the Writer
		//process it and store the result in cache
		void writeLineToFile(std::string path, std::string name, std::string line);

	private:
		Writer() = default;
		std::string buffer;
		std::ofstream fileWriter;
	};
}