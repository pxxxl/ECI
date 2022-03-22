#pragma once

#include"Libs.hpp"

namespace eci{
	using knot = std::string;
	using name = std::string;

	struct branch { knot left; std::vector<element> elements; knot right; };
	
	class element {
	public:
		virtual void* value() = 0;
		name left;
		name right;
		unsigned serial;
	};

	class Vsource{
	public:
		void* value() { return &voltage; }
	private:
		double voltage = 0;
	};

	class Csource{
	public:
		void* value() { return &current; }
	private:
		double current = 0;
	};

	class Resistance{
	public:
		void* value() { return &res; }
	private:
		double res = 0;
	};

}