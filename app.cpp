/*
	made by: Michael Scholten
	created: 10th of july 2020
	updated: 18th of july 2020
*/

#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <fstream>
#include <sstream>

void getDirectoryEntries(const std::string&, std::vector<std::string>&);
std::string readFile(const std::string&);
void filterDirectory(const std::string&);

int main() {
	while(true){
		std::string path;
		std::cout << "Enter the directory to remove duplicate files from: ";
		std::cin >> path;
		if(path[path.size() - 1] != '/'){
			path += "/";
		}
		filterDirectory(path);
	}
}

void getDirectoryEntries(const std::string& directoryPath, std::vector<std::string>& entries){
	struct dirent *entry;
	DIR *directory = opendir(directoryPath.c_str());
	if(directory != NULL){
		while((entry = readdir(directory)) != NULL){
			if(entry->d_name != std::string(".") && entry->d_name != std::string("..")){
				entries.push_back(directoryPath + entry->d_name);
			}
		}
		closedir(directory);
	}
}

std::string readFile(const std::string& fileName){
	std::ifstream file(fileName, std::ios::binary);
	if(file.is_open()){
		std::stringstream content;
		content << file.rdbuf();
		return content.str();
	}else{
		std::cerr << "ERROR: Failed to open file [" << fileName <<  "]!";
		return "";
	}
}

void filterDirectory(const std::string& path){
	std::vector<std::string> entries;
	getDirectoryEntries(path, entries);
	size_t originalSize = entries.size();
	for(size_t i = 0;i < entries.size();i++){
		std::string mainEntry = entries[i];
		size_t j = i + 1;
		while(j < entries.size()){
			std::string checkEntry = entries[j];
			if(readFile(mainEntry) == readFile(checkEntry)){
				remove(checkEntry.c_str());
				entries.erase(entries.begin() + j);
				std::cout << "Removed: " << checkEntry << "\n";
			}else{
				j++;
			}
		}
	}
	std::cout << "Removed " << originalSize - entries.size() << " files.\n";
}
