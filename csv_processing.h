#pragma once

#include "table.h"
#include <string>
#include <vector>

Table readCsvData(std::string inpf, char delimeter);
Table writeCsvData(std::string outf, char delimeter);

std::vector<std::string> parseLine(std::string line, char delimeter,bool head);
void checkLine(std::vector<std::string> line, int size); //whtCheck - equals 0 when checking column line, equals 1 when checking other lines 