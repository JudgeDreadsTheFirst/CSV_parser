

#include "table.h"
#include "csv_processing.h"
#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>

//надо что то придумать с конструктором по умолчанию
Table::Table(){
	
}

Table::Table(std::map<std::string, std::string> m, std::vector<std::string> col, std::vector<std::string> num){
	this->table_vals = m;
	this->columns = col;
	this->line_nums = num;
}

Table::~Table(){
	this->table_vals.erase(this->table_vals.begin(), this->table_vals.end());
	this->columns.erase(this->columns.begin(), this->columns.end());
	this->line_nums.erase(this->line_nums.begin(), this->line_nums.end());
}

int Table::getVal(std::string cell, std::string from_cell) {// много лишнего кода со свитчами,лучше распихать по функциям
	std::regex simple_cell("(^[-][0-9]+$)|(^[0-9]+$)");
	std::regex right_sub("^=[a-zA-Z0-9]+[-][-][0-9]+$");//далее идут регулярные выражения для того что определить с какой стороны находится отрицательное число в выражении с вычитанием
	std::regex left_sub("^=[-][0-9]+[-][a-zA-Z0-9]+$");
	std::regex both_sub("^=[-][0-9]+[-][-][0-9]+$");
	char opers[] = "+*/-";
	int res;
	//сначала проверить, что не пытаемся обратиться к ячейке которую и считаем
	if (from_cell != cell) {
		//проверка что такая ячейка вообще существует в таблице
		if (this->table_vals.count(cell) == 0) {
			throw std::runtime_error("[ERROR]: where is no such cell{" + cell + "}");
		}
		else {
			//если это просто целое число то сразу возвращаем его
			if (std::regex_match(this->table_vals[cell], simple_cell)) {
				res = std::stoi(this->table_vals[cell]);
				return res;
			}// в противоположном случае это выражение(предполагаем что программа завершается еще до этого этапа если ячейка не подходит под формат)
			else {
				for (auto i = 0; i != 4; i++) {
					switch (i) {// в этом операторе свитч по очереди смотрим какая из операций может находится в строке
						case 0:
							if (std::count(this->table_vals[cell].begin(),this->table_vals[cell].end(),opers[i]) == 1) {
								std::string val;
								std::stringstream ss(this->table_vals[cell]);
								std::getline(ss, val, opers[i]);
								val.erase(std::remove(val.begin(), val.end(), '='), val.end());
								//считали строку, по очереди делим на операторы(arg1,arg2), делаем проверки опять же что такие ячейки есть(если это не число)
								// далее примерно по таким же правилам ведем рассчет для других операций
								if (this->table_vals.count(val) == 1) {
									res = getVal(val, cell);
								}
								else if (std::regex_match(val, simple_cell)) {
									res = std::stoi(val);
								}
								else {
									throw std::runtime_error("[ERROR]: where is no such cell{" + val + "}");
								}

								std::getline(ss, val);

								if (this->table_vals.count(val) == 1) {
									res += getVal(val, cell);
								}
								else if (std::regex_match(val, simple_cell)) {
									res += std::stoi(val);
								}
								else{
									throw std::runtime_error("[ERROR]: where is no such cell{" + val + "}");
								}

								return res;
							}
							break;
						case 1:
							if (std::count(this->table_vals[cell].begin(), this->table_vals[cell].end(), opers[i]) == 1) {
								std::stringstream ss(this->table_vals[cell]);
								std::string val1;
								std::string val2;
								int r1, r2;
								std::getline(ss, val1, opers[i]);
								val1.erase(std::remove(val1.begin(), val1.end(), '='), val1.end());


								if (this->table_vals.count(val1) == 1) {
									r1 = getVal(val1, cell);
								}
								else if (std::regex_match(val1, simple_cell)) {
									r1 = std::stoi(val1);
								}
								else {
									throw std::runtime_error("[ERROR]: where is no such cell{" + val1 + "}");
								}

								std::getline(ss, val2);


								if (this->table_vals.count(val2) == 1) {
									r2 = getVal(val2, cell);
								}
								else if (std::regex_match(val2, simple_cell)) {
									r2 = std::stoi(val2);
								}
								else {
									throw std::runtime_error("[ERROR]: where is no such cell{" + val2 + "}");
								}

								return r1*r2;
							}
							break;
						case 2:
							if (std::count(this->table_vals[cell].begin(), this->table_vals[cell].end(), opers[i]) == 1) {
								std::stringstream ss(this->table_vals[cell]);
								std::string val1;
								std::string val2;
								int r1, r2;
								std::getline(ss, val1, opers[i]);
								val1.erase(std::remove(val1.begin(), val1.end(), '='), val1.end());


								if (this->table_vals.count(val1) == 1) {
									r1 = getVal(val1, cell);
								}
								else if (std::regex_match(val1, simple_cell)) {
									r1 = std::stoi(val1);
								}
								else {
									throw std::runtime_error("[ERROR]: where is no such cell{" + val1 + "}");
								}

								std::getline(ss, val2);


								if (this->table_vals.count(val2) == 1) {
									r2 = getVal(val2, cell);
								}
								else if (std::regex_match(val2, simple_cell)) {
									r2 = std::stoi(val2);
								}
								else {
									throw std::runtime_error("[ERROR]: where is no such cell{" + val2 + "}");
								}

								if (r2 == 0) {
									throw std::invalid_argument("[ERROR]:division by zero, new value will be assigned as 1");
								}

								return r1 / r2;
							}
							break;
						case 3: 
							if (std::count(this->table_vals[cell].begin(), this->table_vals[cell].end(), opers[i]) == 1) {
								std::string val;
								std::stringstream ss(this->table_vals[cell]);
								std::getline(ss, val, opers[i]);
								val.erase(std::remove(val.begin(), val.end(), '='), val.end());

								if (this->table_vals.count(val) == 1) {
									res = getVal(val, cell);
								}
								else if (std::regex_match(val, simple_cell)) {
									res = std::stoi(val);
								}
								else {
									std::cout << "where is no such cell{" << val << "}" << std::endl;
									throw std::runtime_error("[ERROR]: where is no such cell{" + val + "}");
								}
								std::getline(ss, val);

								if (this->table_vals.count(val) == 1) {
									res = res - getVal(val, cell);
								}
								else if (std::regex_match(val, simple_cell)) {
									res = res - std::stoi(val);
								}
								else {
									std::cout << "where is no such cell{" << val << "}" << std::endl;
									throw std::runtime_error("[ERROR]: where is no such cell{" + val + "}");
								}
								return res;
							}
							else {
								if (std::regex_match(this->table_vals[cell], left_sub)) {
									std::stringstream ss(this->table_vals[cell]);
									std::string val;
									std::string val1;
									std::getline(ss, val, opers[i]);
									val.erase(std::remove(val.begin(), val.end(), '='), val.end());
									std::getline(ss, val, opers[i]);

									val1 = "-" + val;

									res = std::stoi(val1);
									std::getline(ss, val);
									if (this->table_vals.count(val) == 1) {
										res = res - getVal(val, cell);
									}
									else if (std::regex_match(val, simple_cell)) {
										res = res - std::stoi(val);
									}
									else {
										std::cout << "where is no such cell{" << val << "}" << std::endl;
										throw std::runtime_error("[ERROR]: where is no such cell{" + val + "}");
									}

									return res;
								}
								else if (std::regex_match(this->table_vals[cell], right_sub)){
									std::stringstream ss(this->table_vals[cell]);
									std::string val;
									std::string val1;
									std::getline(ss, val, opers[i]);
									val.erase(std::remove(val.begin(), val.end(), '='), val.end());

									if (this->table_vals.count(val) == 1) {
										res = getVal(val, cell);
									}
									else if (std::regex_match(val, simple_cell)) {
										res = std::stoi(val);
									}
									else {
										std::cout << "where is no such cell{" << val << "}" << std::endl;
										throw std::runtime_error("[ERROR]: where is no such cell{" + val + "}");
									}
									std::getline(ss, val, opers[i]);
									std::getline(ss, val);

									val1 = "-" + val;

									res = res - std::stoi(val1);
									return res;
								}
								else if (std::regex_match(this->table_vals[cell], both_sub)) {
									std::stringstream ss(this->table_vals[cell]);
									std::string val;
									std::string val1;
									std::getline(ss, val, opers[i]);
									val.erase(std::remove(val.begin(), val.end(), '='), val.end());
									std::getline(ss, val, opers[i]);

									val1 = "-" + val;

									res = std::stoi(val1);
									std::getline(ss, val,opers[i]);
									std::getline(ss, val);
									res = res - std::stoi("-" + val);// собственно можно было так и раньше делать...
									return res;
								}
							}
							break;
							
					}
				}
			}
		}
	}
	else {
		throw std::runtime_error("[ERROR]: cell is trying to call herself to calc");
	}
}
/* по идее можно не хранить два вектора просто для вывода, а переписать все, используя только map...
...но так по идее будет другой порядок визуального расположения ячеек, на результат это не влияет никак*/ 
void Table::printTable() {						
	for (auto i = 0; i != this->columns.size();i++) {
		if (i == this->columns.size() - 1) {
			std::cout << this->columns[i];
		}
		else {
			std::cout << this->columns[i] << ',';
		}
	}
	std::cout << std::endl;
	for (auto i = 0; i != this->line_nums.size(); i++) {
		std::cout << line_nums[i] << ',';
		for (auto j = 1; j != this->columns.size(); j++) {
			if (i == this->columns.size() - 1) {
				std::cout << this->table_vals[columns[j] + line_nums[i]];
			}
			else {
				std::cout << this->table_vals[columns[j] + line_nums[i]] << ',';
			}
		}
		std::cout << std::endl;
	}
}

void Table::calcTable() {
	std::regex cell_to_calc("^=(([-]{0,1}[0-9]+)|([a-zA-Z0-9]+))[+*/-](([-]{0,1}[0-9]+)|([a-zA-Z0-9]+))$");
	for (std::map<std::string, std::string>::iterator it = this->table_vals.begin(); it != this->table_vals.end(); ++it) {
		if (std::regex_match(it->second, cell_to_calc)) {
			int temp;
			try {
				temp = getVal(it->first, "0");// какой то тупой костыль выходит, лучше переписать что бы getVal сразу давал string
			}
			catch(std::runtime_error e){
				std::cerr << e.what() << std::endl;
			}
			catch (std::invalid_argument er) {
				std::cerr << er.what() << std::endl;
			}

			std::string tmp = std::to_string(temp);
			it->second = tmp;
		}
	}
}

Table& Table::operator=(const Table& right) {
	if (this == &right) {
		return *this;
	}
	table_vals = right.table_vals;
	columns = right.columns;
	line_nums = right.line_nums;
	return *this;
}