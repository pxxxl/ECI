#pragma once

#include"Interpreter.hpp"

namespace eci {
	using branch_status = unsigned;

	constexpr unsigned BRANCH = 0;
	constexpr unsigned NORTON_BRANCH = 1;
	constexpr unsigned V_BRANCH = 2;

	class branch {
	public:


	private:

		branch_status status = BRANCH;
	};

}