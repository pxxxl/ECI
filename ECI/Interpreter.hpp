#pragma once

#include"Libs.hpp"
#include"Element.hpp"

namespace eci {
	namespace regex_lib {
		const char* define_knot = "^ *knot *[a-z]* *(, *[a-z]* *)* *$";
		const char* define_item = "^[a-z]*(--\(R|U|I|G\d+ *= *-?\d+))+--[a-z] *$";
		const std::vector<std::string> regex_pro({ "",define_knot,define_item });

		constexpr int DEFINE_KNOT = 1;
		constexpr int DEFINE_ITEM = 2;

		constexpr unsigned R = 1;
		constexpr unsigned U = 2;
		constexpr unsigned I = 3;
		constexpr unsigned G = 4;
		constexpr unsigned KNOT = 5;
	}
	using namespace regex_lib;

	using sentence = std::vector<std::string>;


	class interpreter {
	public:
		interpreter(const interpreter&) = delete;
		interpreter(const interpreter&&) = delete;

		void input(std::vector<std::string> lines);

		void preProcess();

		void process();

		std::vector<element> getElements();

		std::vector<branch> getBranches();
	private:
		std::vector<std::string> raw_lines;
		std::vector<sentence> pre_processed;
		std::vector<element> elements;
		std::vector<branch> branches;
	};

	inline void eci::interpreter::input(std::vector<std::string> lines)
	{
		raw_lines = lines;
		return;
	}
	inline void interpreter::preProcess()
	{
		pre_processed.clear();
		bool flag;
		std::regex rege;
		std::smatch result;
		for (auto& line : raw_lines) {
			flag = false;
			for (unsigned i = 1; i < regex_pro.size(); i++) {
				rege = regex_pro[i];
				if (std::regex_match(line, result, rege)) {
					flag = true;
					sentence build(i, std::vector<std::string>());
					//switch here, depending on which sentence the string is
					build.first = i;
					auto type1Process = [&]() {
						auto be_s = (line).cbegin();
						auto en_s = (line).cend();
						std::regex pattern("\w+");
						while (std::regex_search(be_s, en_s, result, pattern)) {
							build.second.push_back(result[0]);
							be_s = result[0].second;
						}
						pre_processed.push_back(build);
					};
					auto type2Process = [&]() {
						auto be_s = (*be_d).cbegin();
						auto en_s = (*be_d).cend();
						std::string nodeB;
						std::regex regA("R|U|I|G\d+");
						std::regex regB("-?\d+");
						std::regex regC("[a-z]*");
						std::regex pattern("R|U|I|G\d+ *= *-?\d+");
						//match knot
						std::regex_search(be_s, en_s, result, regC);
						build.second.push_back(result.str());
						be_s = result[0].second;
						std::regex_search(be_s, en_s, result, regC);
						nodeB = result.str();
						be_s = (*be_d).cbegin();
						//match item
						while (std::regex_search(be_s, en_s, result, regA)) {
							build.second.push_back(result.str());
							be_s = result[0].second;
							std::regex_search(be_s, en_s, result, regB);
							build.second.push_back(result.str());
							be_s = result[0].second;
						}
						build.second.push_back(nodeB);
						pre_processed.push_back(build);
					};
					
					switch (i) {
					case DEFINE_KNOT:type1Process(); break;
					case DEFINE_ITEM:type2Process(); break;
					}
				}
			}
			if (flag == false) {
				auto str = std::string();
				throw std::runtime_error(*be_d);
			}
		}
	}
	inline void interpreter::process()
	{
		items.clear();
		auto be = pre_processed.begin();
		auto en = pre_processed.end();
		for (; be != en; be++) {
			auto be2 = (*be).second.begin();
			auto en2 = (*be).second.end();
			element entity;
			switch ((*be).first) {
			case DEFINE_KNOT:
				for (; be2 != en2; be2++) {
					items.push_back(element({ KNOT ,(*be2),description() }));
				}
				break;
			case DEFINE_ITEM:
				auto stringToDouble = [](std::string str) -> double{
					auto be = str.begin();
					auto en = str.end();
					bool flag = false;
					bool nega = false;
					double demical_bios = 10;
					double num = 0;
					if (*be == '-') {
						nega = true;
						be++;
					}
					for (; be != en; be++) {
						if (*be != '.'||flag == false) {
							num *= 10;
							num += (*be - '0');
						}
						else if (*be != '.' || flag == true) {
							num += (*be - '0') / demical_bios;
							demical_bios *= 10;
						}
						else {
							flag = true;
						}
					}
					return num;
				};
				auto& senten = (*be).second;
				unsigned size = senten.size();
				unsigned TYPE = 0;
				for (unsigned i = 1; i < size - 1; i++) {
					switch (senten[i][0]) {
					case 'R':TYPE = R; break;
					case 'U':TYPE = U; break;
					case 'I':TYPE = I; break;
					case 'G':TYPE = G; break;
					}
					if(i == 1){
						double* ptr = new double(stringToDouble(senten[i + 1]));
						auto des = description(ptr, std::pair(senten[0], senten[3]));
						auto sample = element({ TYPE, senten[i],des });
						items.push_back(sample);
					}
					else {
						double* ptr = new double(stringToDouble(senten[i + 1]));
						auto des = description(ptr, std::pair(senten[i - 1], senten[i + 2]));
						auto sample = element({ TYPE, senten[i],des });
						items.push_back(sample);
					}
				}
				break;
			}
		}
		return;
	}
	inline std::vector<element> interpreter::getElements()
	{
		return items;
	}
	inline std::vector<branch> interpreter::getBranches()
	{
		return branches;
	}

	

}


