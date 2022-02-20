/*
	made by: Michael Scholten
	created: 10th of July 2020
	updated: 20th of February 2022
*/

#include <iostream>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <sstream>

void getDirectoryEntries(const std::string&, std::vector<std::string>&);
std::string readFile(const std::string&);
void filterDirectory(const std::string&);

int main() {
	while(true){
		// request a directory from the user
		std::string path;
		std::cout << "Enter the directory to remove duplicate files from: ";
		std::cin >> path;

		// make sure the path ends with a '/'
		if(path[path.size() - 1] != '/'){
			path += "/";
		}

		// filter the directory
		filterDirectory(path);
	}
}

void getDirectoryEntries(const std::string& directoryPath, std::vector<std::string>& entries){
	// declare a directory entry pointer
	struct dirent *entry;

	// open the directory, so we can read it's entries
	DIR *directory = opendir(directoryPath.c_str());

	// only if the directory exists, we can search for entries
	if(directory != NULL){
		// iterate through the directory entries
		while((entry = readdir(directory)) != NULL){
			// only add an entry if it isn't a directory
			if(entry->d_type != DT_DIR){
				// add the entry to the end of the vector
				entries.push_back(directoryPath + entry->d_name);
			}
		}
		// close the directory as we are done using it
		closedir(directory);
	}
}

std::string readFile(const std::string& fileName){
	// read the file in binary, so the program works for all files
	std::ifstream file(fileName, std::ios::binary);

	// only if the file did open, we read the content
	if(file.is_open()){
		// read the content of the file and return it
		std::stringstream content;
		content << file.rdbuf();
		return content.str();
	}else{
		// if we were unable to open the file, we display and error message and return and empty string
		std::cerr << "ERROR: Failed to open file [" << fileName <<  "]!\n";
		return "";
	}
}

void filterDirectory(const std::string& path){
	// create a new vector for the entries and add all the entries in the given directory to the vector
	std::vector<std::string> entries;
	getDirectoryEntries(path, entries);

	// store the resulting size, so we can calculate the number of removed entries
	size_t originalSize = entries.size();

	// iterate through the entries
	for(size_t i = 0;i < entries.size();i++){
		// start the inner iterator 1 position farther than the current position of the outer iterator
		size_t j = i + 1;
		while(j < entries.size()){
			// check whether the contents of the entries are the same
			std::string checkDirectory = entries[j];
			if(readFile(entries[i]) == readFile(checkDirectory)){
				// if so, we delete the file and remove it from the vector
				remove(checkDirectory.c_str());
				entries.erase(entries.begin() + j);
				std::cout << "Removed: " << checkDirectory << "\n";
			}else{
				// if the contents were not equal, we increment the outer iterator
				j++;
			}
		}
	}
	// display the amount of files that were removed
	std::cout << "Removed " << originalSize - entries.size() << " files.\n";
}
