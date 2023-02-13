

#include "csv_processing.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <regex>


// TODO: унифицировать итерирование в циклах
// TcODO: заменить где нужно runtime_error на более подходящий тип ошибки
// TODO: добавить в функции логирование ошибок(добавить передачу имени файла лога в формате filename_log.txt)
// (по идее библиотеки для логирования есть в boost но его нельзя взять)

//не очень универсальное решение, что считывание данных возращает объект самописного класса,...
//...подумать как потом map выводить в правильном порядке, что бы возвращать только его

Table readCsvData(std::string inpf, char delimeter){
    std::ifstream file(inpf);

    if(!file.is_open()) throw std::runtime_error("Input file not open!");
    
    std::string s;
    std::getline(file,s);

    std::vector<std::string> column_names;    
	try {
		column_names = parseLine(s, delimeter, true);
	}
	catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
	}
    
    int column_size = column_names.size();
    for(auto i = 1;i!=column_size;i++){
        if(std::count(column_names.begin(),column_names.end(),column_names[i])!=1){
            throw std::runtime_error("[ERROR]:duplicate names in the head line");
        }
    }
	std::vector<std::string> line_nums;
	std::map<std::string, std::string> vals;
	while (!file.eof()) {
		std::getline(file, s);
		std::vector<std::string> line;

		try {
			line = parseLine(s, delimeter, false);
		}
		catch (std::runtime_error e) {
			std::cerr << e.what() << std::endl;
		}

		try {
			checkLine(line, column_size);
		}
		catch (std::invalid_argument e) {
			std::cerr << e.what() << std::endl;
		}
		catch (std::length_error er) {
			std::cerr << er.what() << std::endl;
		}

		int line_size = line.size();
        
		for (auto i = 1; i != std::min(column_size, line_size); i++) {
			vals[column_names[i] + line[0]] = line[i];
		}
		line_nums.push_back(line[0]);
	}
    for(auto i = 1;i!=line_nums.size();i++){
        if(std::count(line_nums.begin(),line_nums.end(),line_nums[i])!=1){
            throw std::runtime_error("[ERROR]:duplicate numbers at the start of the lines");
        }
    }
	file.close();
	Table read_res(vals, column_names, line_nums);
	return read_res;
}

std::vector<std::string> parseLine(std::string Line,char delimeter,bool head){
    std::stringstream ss(Line);
    std::vector<std::string> tokens;

    std::string val;
    int cnt = 0;
    while(std::getline(ss, val,delimeter)){
		if (val.empty()) {
			throw std::runtime_error("[ERROR]: empty cell occured in data");
		}
        if (cnt == 0 && head==true){
            for(std::string::size_type i =0; i < val.size();i++ ){
                if(!isspace(val[i])){
					std::cout << "first column is not space" << std::endl;
                    tokens.push_back(" ");// наверное это неправильная обработка и лучше будет просто выкинуть ошибку
                    //val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());
                    //tokens.push_back(val);
                    break;
                }
				else {
					tokens.push_back(val);
					break;
				}
            }
        }
        else{
            val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());
            if(!val.empty()){
                tokens.push_back(val);
            }
            else if(head==true){
                throw std::runtime_error("[ERROR] Line[1]:The format is broken,space cell in the header");
            }
            else{
                std::cout<< "[WARNING]: got space cell, replacing it with zero, may cause errors"<<std::endl;// тут тоже на пустую строчку лучше выкинуть ошибку
                tokens.push_back("0");
            }
        }
        cnt++;
    }

    return tokens;

}

void checkLine(std::vector<std::string> line, int size){
	// TODO: переименовать регулярные выражения
    std::regex rgx1("^=(([-]{0,1}[0-9]+)|([a-zA-Z0-9]+))[+*/-](([-]{0,1}[0-9]+)|([a-zA-Z0-9]+))$");
    std::regex rgx2("(^[-][0-9]+$)|(^[0-9]+$)");
    std::regex rgx3("^[0-9]+$");
    if(!std::regex_match(line[0],rgx3)){
		std::cout << "[ERROR]: line num is not a positive" << std::endl;
        //throw std::runtime_error("[ERROR]: line num is not a positive");
    }
    if(line.size() != size){
        if(line.size() > size){
            std::cout << "[Warning]: too many cells in line [" << line[0] <<"], some will be unused" << std::endl; 
        }
        if(line.size() < size){
            throw std::length_error("[ERROR]: not enough cells in line ["+line[0]+"], need more data");
        }
    }
    for(auto i = 1; i != line.size(); i++){
        if(!std::regex_match(line[i],rgx2) && !std::regex_match(line[i],rgx1)){
            throw std::invalid_argument("[ERROR]: cell does not match the template");
        }
    }
}