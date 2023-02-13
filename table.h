#pragma once

#include <map>
#include <string>
#include <vector>

class Table
{
private:
    std::vector<std::string> columns;
	std::vector<std::string> line_nums;
    std::map<std::string, std::string> table_vals;

public:
	Table();
	Table(std::map<std::string, std::string> m, std::vector<std::string> col, std::vector<std::string> num);
	~Table();

	void printTable();
	void calcTable();
    int getVal(std::string cell, std::string from_cell); /*
														 так как предположительно здесь сделаю возможно рекурсивного вызова функции
														 по типу С1=А1+B2, и просто вызвать эту же функцию для других ячеек,
														 добавить переменную отвечающую за хранение ячейки из которой вызвали
														 , что бы не пытаться в кольце считать С1=С1+А1-такое должно быть ошибкой
														 */
	
	Table& operator=(const Table& right);
}; 