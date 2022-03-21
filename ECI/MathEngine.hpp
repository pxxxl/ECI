#pragma once

#include"Libs.hpp"
#include"StaticMemory.hpp"
#include"e_matrix.hpp"

namespace eci{
	//reference : static memory 3.Equation solving
	class mathEngine
	{
	public:
		mathEngine(const mathEngine&) = delete;
		mathEngine(const mathEngine&&) = delete;
		mathEngine() = default;

		void inputEquation(equation a);

		void setFreeVariable(std::vector<std::string> free_varieble);

		void solve();

		solution getSolution();

		solve_status getSolveStatus() { return status; }

		void clear();

	private:
		unsigned equation_count = 0;
		unsigned variable_count = 0;
		unsigned free_variable_count = 0;
		std::vector<std::string> free_variables;
		std::vector<std::string> constraint_variables;

		std::unordered_map<std::string, unsigned> x_map;
		
		std::vector<equation> equations;

		eds::matrix<double>* solution_matrix;
		solution solution_map;

		solve_status status = UNSOLVED;
	};


	inline void mathEngine::inputEquation(equation a)
	{
		equations.push_back(a);
		equation_count++;
		auto& left = a.first;
		auto& right = a.second;
		auto findVariable = [this](std::string sample)->bool {
			auto be = free_variables.begin();
			auto en = free_variables.end();
			for (; be != en; be++) {
				if (*be == sample) {
					return false;
				}
			}
			be = constraint_variables.begin();
			en = constraint_variables.end();
			for (; be != en; be++) {
				if (*be == sample) {
					return false;
				}
			}
			return true;
		};
		auto insertVariebles = [this, findVariable](polynomial& poly) {
			auto be = poly.begin();
			auto en = poly.end();
			for (; be != en; be++) {
				if (findVariable((*be).second)) {
					constraint_variables.push_back((*be).second);
					variable_count++;
				}
			}
		};
		insertVariebles(left);
		insertVariebles(right);
		return;
	}

	inline void mathEngine::setFreeVariable(std::vector<std::string> free_varieble)
	{
		//find : iterator return
		auto findVariableFromConstrait = [this](std::string sample)->decltype(constraint_variables.begin()) {
			auto size = constraint_variables.size();
			auto be = constraint_variables.begin();
			auto en = constraint_variables.end();
			for (; be != en; be++) {
				if (*be == sample) {
					return be;
				}
			}
			return en;
		};
		//find : false return
		auto findVariableFromFree = [this](std::string sample)->int {
			auto be = free_variables.begin();
			auto en = free_variables.end();
			for (; be != en; be++) {
				if (*be == sample) {
					return false;
				}
			}
			return true;
		};
		auto be = free_varieble.begin();
		auto en = free_varieble.end();
		decltype(be) target;
		for (; be != en; be++) {
			target = findVariableFromConstrait(*be);
			if (target == constraint_variables.end()) {
				//not find 
				throw std::runtime_error("From eci::mathEngine:setFreeVariable : free varieble " + *be + " set failed, no such varieble has stored in constraint variables");
			}
			else {
				if (!findVariableFromFree(*be)) {
					//find in free variable
					throw std::runtime_error("From eci::mathEngine:setFreeVariable : free varieble " + *be + " set failed, varieble has already stored in free variables");
				}
				constraint_variables.erase(target);
				free_variables.push_back(*be);
				variable_count;
				free_variable_count++;
			}
		}
		return;
	}

	inline void mathEngine::solve()
	{
		std::vector<std::string> variable_queue;
		auto be_v = constraint_variables.begin();
		auto en_v = constraint_variables.end();
		unsigned num = 0;
		for (; be_v != en_v; be_v++, num++) {
			variable_queue.push_back(*be_v);
			x_map.insert(std::pair(*be_v, num));
		}
		for (int count = free_variables.size()-1; count >= 0; count--) {
			variable_queue.push_back(free_variables[count]);
			x_map.insert(std::pair(free_variables[count], num));
			num++;
		}
		variable_queue.push_back(std::string(""));
		x_map.insert(std::pair(std::string(""), num));
		//construct matrix;
		solution_matrix = new eds::matrix<double>(equation_count, variable_count);
		auto be_e = equations.begin();
		auto en_e = equations.end();
		decltype(((*be_e).first).begin()) be_poly;
		decltype(((*be_e).first).end()) en_poly;
		unsigned cur_column = 0;
		for (unsigned line = 0; be_e != en_e; be_e++, line++) {
			be_poly = ((*be_e).first).begin();
			en_poly = ((*be_e).first).end();
			for (; be_poly != en_poly; be_poly++) {
				cur_column = x_map[(*be_poly).second];
				(*solution_matrix).reset(line, cur_column, ( * solution_matrix).get(line, cur_column) + (*be_poly).first);
			}
		}
		//solve the matrix
		*solution_matrix = (*solution_matrix).std_line_form();

		//check the result
		unsigned mat_line_max = equation_count;
		unsigned mat_column_max = variable_count;
		auto is_solutionless = [this](eds::matrix<double> mat)->bool {
			bool flag = true;
			unsigned right_column = equation_count - 1;
			for (unsigned line = 0; line < variable_count; line++) {
				if (eds::is_zero(mat.get(line, right_column))) {
					continue;
				}
				for (unsigned left_column = 0; left_column < right_column; left_column++) {
					if (!eds::is_zero(mat.get(line, left_column))) {
						continue;
					}
				}
				flag = false;
			}
			return !flag;
		};
		if (is_solutionless(*solution_matrix)) {
			status = ERROR_SOLUTION;
			return;
		}
		else {
			solution_map.clear();
			unsigned cur_line = 0;
			unsigned cur_column = 0;
			while (cur_line < mat_line_max && cur_column < mat_column_max - 1) {
				if ((*solution_matrix).get(cur_line, cur_column) == 0) {
					cur_column++;
				}
				else {
					auto x_name = variable_queue[cur_column];
					polynomial x_solve;
					for (unsigned i = cur_column + 1; i < mat_column_max; i++) {
						auto item = solution_matrix->get(i, cur_line);
						if (!eds::is_zero(item)) {
							x_solve.push_back(std::pair(item, variable_queue[i]));
						}
					}
					solution_map.insert(std::pair(x_name, x_solve));
					cur_line++;
				}
			}
			status = SOLVED;
		}
		return;
	}

	inline solution mathEngine::getSolution()
	{
		return solution_map;
	}

	inline void mathEngine::clear()
	{
		unsigned equation_count = 0;
		unsigned variable_count = 0;
		unsigned free_variable_count = 0;
		free_variables.clear();
		constraint_variables.clear();
		x_map.clear();
		equations.clear();
		solution_matrix->clear();
		solution_map.clear();
		status = UNSOLVED;
	}

}


