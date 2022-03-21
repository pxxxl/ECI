#pragma once

#include"Libs.hpp"

namespace eci {


	//1.Reader
	constexpr int readerMaxNumber = 100;


	//2.Command match



	//3.Equation solving
	using term = std::pair<double, std::string>;
	using polynomial = std::vector<term>;

	using equation = std::pair<polynomial, polynomial>;
	using solution = std::unordered_map<std::string, polynomial>;

	using solve_status = int;

	constexpr solve_status UNSOLVED = 1;
	constexpr solve_status ERROR_SOLUTION = 1;
	constexpr solve_status SOLVED = 2;


	//4.Console tips






}

