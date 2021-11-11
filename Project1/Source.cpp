#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Windows.h> 
typedef nlohmann::json Json;
std::string UTF8_to_CP1251(std::string const& utf8){
	if (!utf8.empty()){
		int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
		if (wchlen > 0 && wchlen != 0xFFFD){
			std::vector<wchar_t> wbuf(wchlen);
			MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), &wbuf[0], wchlen);
			std::vector<char> buf(wchlen);
			WideCharToMultiByte(1251, 0, &wbuf[0], wchlen, &buf[0], wchlen, 0, 0);

			return std::string(&buf[0], wchlen);
		}
	}
	return std::string();
}
int main() {

	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::cout << "ÏÐÈÂÊÈ" << std::endl;
	std::string s = { u8"ÏÐÈÂÅÒ " };	
	std::cout << UTF8_to_CP1251(s) << std::endl;

	Json json;	
	json["HI"] = s;
	std::string serializedString = json.dump();
	std::ofstream fout;
	fout.open("json.json", std::ofstream::app);
	fout << serializedString << "\n";
	fout.close();

	std::ifstream fin;
	fin.open("json.json");
	if (!fin.is_open())
		std::cout << "error: " << "json.json" << " not open!" << std::endl;
	else {
		std::string line;
		while (std::getline(fin, line)){
			json = Json::parse(line);

			std::cout << UTF8_to_CP1251(json["HI"].get<std::string>()) << std::endl;
		}
	}
	fin.close();
	return 0;
}