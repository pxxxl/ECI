#pragma once

#include "Libs.hpp"
#include "StaticMemory.hpp"

namespace eci {
	//Singleton
	class Reader
	{
	public:
		static Reader& getInstance() {
			static Reader reader;
			return reader;
		}
		Reader(const Reader&) = delete;
		Reader(const Reader&&) = delete;

		//read a line from stdin, store the string in the reader
		//process it and store the result in cache
		void readLineFromStdin();

		//read a line from file, store the string in the reader
		//process it and store the result in cache
		void readLineFromFile(std::string path, std::string name);

		//get the processed strings
		std::vector<std::string> getResult() const;

		//get the original string
		std::string getString() const;

		//reset the line counter to zero
		void lineCounterRewind() noexcept;


	private:
		Reader() {}
		unsigned lineCounter = 0;
		std::vector<std::string> cache;
		std::string buffer;
		std::ifstream fileReader;
	};

	inline void eci::Reader::readLineFromStdin()
	{
		rewind(stdin);
		buffer.clear();
		cache.clear();
		std::getline(std::cin, buffer);
		if (buffer.length() > readerMaxNumber) {
			lineCounter++;
			char str[200];
			sprintf(str, "Message from eci::Reader::readLineFromStdin\nLine %d : The input string is too long.", lineCounter);
			throw std::runtime_error(std::string(str));
		}
		auto be = buffer.begin();
		auto en = buffer.end();
		std::string store;
		for (; be != en; be++) {
			if (*be != ' ' && *be != '\t') {
				store.push_back(*be);
			}
			else {
				cache.push_back(store);
				store.clear();
			}
		}
		lineCounter++;
		return;
	}

	inline void eci::Reader::readLineFromFile(std::string path, std::string name)
	{
		fileReader.open(path + name, std::istream::in);
		buffer.clear();
		cache.clear();
		std::getline(fileReader, buffer);
		if (buffer.length() > readerMaxNumber) {
			lineCounter++;
			char str[200];
			sprintf(str, "Message from eci::Reader::readLineFromFile\nLine %d : The input string is too long.", lineCounter);
			throw std::runtime_error(std::string(str));
		}
		auto be = buffer.begin();
		auto en = buffer.end();
		std::string store;
		for (; be != en; be++) {
			if (*be != ' ' && *be != '\t') {
				store.push_back(*be);
			}
			else {
				cache.push_back(store);
				store.clear();
			}
		}
		lineCounter++;
		return;
	}
	inline std::vector<std::string> Reader::getResult() const
	{
		return cache;
	}
	inline std::string Reader::getString() const
	{
		return buffer;
	}
	inline void Reader::lineCounterRewind() noexcept
	{
		lineCounter = 0;
		return;
	}
}