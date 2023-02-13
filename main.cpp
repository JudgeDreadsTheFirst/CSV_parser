
#include "csv_processing.h"
#include "table.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]){
	std::string str = argv[1];
	char delim = ',';
    
	Table wrk;
	try {
		wrk = readCsvData(str, delim);
	}
	catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
	}
	wrk.calcTable();
	wrk.printTable();
    return 0;
}