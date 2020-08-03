/*
	made by: Michael Scholten
	created: 10th of july 2020
	updated: 18th of july 2020
*/
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string lowerCase(std::string input) {// converts the input string to lowerCase
	for (int i = 0; i < input.length(); i++) {
		//convert the char at index i of input to lowercase if it is uppercase
		if (input[i] >= 'A' && input[i] <= 'Z') {
			input[i] += 'a' - 'A';
		}
	}
	// return the result
	return input;
}

bool fileRemover(std::string map) {// checks for and removes double files
	//get the start clock so we can calculate how long it took to remove all double files
	time_t start = clock();

	//make a boolean to check whether we have to check again.
	bool deleted;
	do {
		// we don't know whether we're going to check again, so we set deleted to false
		deleted = false;

		// create a vector for the content and a vector for the name and location of files
		std::vector<std::string> content;
		std::vector<fs::path> files;

		//check the given map for files and their content.
		//we do it in a try/catch statement so we can alse check whether the given map exists
		try {
			for (const fs::directory_entry& entry : fs::directory_iterator(map)) {
				//we ignore the directories
				if (!entry.is_directory()) {
					files.push_back(entry.path());
					std::string val = "", line;
					std::ifstream f(entry, std::ios::binary);
					while(getline(f, line)) val += line + "\n";
					f.close();
					content.push_back(val);
				}
			}
		}
		catch (...) {
			//map doesn't exist, so we go back to the main function to ask the user for another map name
			std::cout << "Map bestaat niet.\ncontroleer of je het goed getypt hebt.\nKijk ook eens in de handleiding als het probleem zich voor blijft doen.\n" << std::endl;
			return false;
		}
		//we have not removed anything and want to count deleted files, so we set removed to 0
		int removed = 0;
		//check all files and remove one if two files contain the same data
		for (int i = 0; i < files.size(); i++) {
			int j = 0;
			while (j < content.size()) {
				if (i == j) {
					j++;
					continue;
				}
				if (content[i] == content[j]) {
					fs::remove(files[j]);
					content.erase(content.begin() + j);
					removed++;
					deleted = true;
				}
				else j++;
			}
		}
		//tell the user how many files have been deleted
		std::cout << removed << " bestanden verwijderd." << std::endl;
	} while (deleted);
	//tell the user how much time it took to delete all the double files
	std::cout << (clock() - start) * 1.0 / CLOCKS_PER_SEC << std::endl;
	return true;
}

int main(int argc, char** argv) {// argc is for the amount of arguments and argv is for the value of arguments given in the command prompt
	//if argc == 1, the user hasn't entered any arguments. just the name of the program
	if (argc == 1) {
		// start an infinite while loop to continually ask the user which map needs to be checked and check it
		//the loop only stops when the user enters anything onther than 1, "ja" or "j" when the user is asked whether he/she wants to continue.
		while (1) {
			std::cout << "Uit welke map wil je alle dubbele verwijderen?\n";
			std::string map;
			std::cin >> map;
			if (!fileRemover(map)) continue;
			std::cout << "Wil je het ook bij een andere map doen?\n\t1) ja\n\t2) nee" << std::endl;
			std::string choice;
			std::cin >> choice;
			if (!(choice == "1" || lowerCase(choice) == "ja" || lowerCase(choice) == "j")) {
				break;
			}
		}
		//tell the user that you hop him/her will use the program again in the future and close the program after the user pressed a key
		std::cout << "Ik hoop dat je dit programma nog een keer zal gebruiken." << std::endl;
		std::cin.get();
	}else if(argc == 2 && (argv[1] == "-h" || argv[1] == "--h" || argv[1] == "-help" || argv[1] == "--help")){
		//if the user enters one of the help commands and nothing else, tell that user how to use the program.
		std::cout << "Bedankt for het gebruiken van een van de help instructies.\nDit programma is heel simpel.\n\n"
					 "Als je dubbele bestanden wilt verwijderen, typ je \"bestandFilter naamVanMap\"\n"
					 "Die instructie zal het programma starten en meteen alle bestanden in de naamVanMap map zoeken en lezen om alle dubbele bestanden daarna te verwijderen.\n"
					 "Als die dat eenmaal gedaan heeft herhaalt het die procedure tot het geen enkel bestand meer hoeft te verwijderen.\n\n"
					 "Een andere handige instructie die je kan gebruiken is \"bestandFilter\"\n"
					 "Die instructie doet precies hetzelfde, maar vraagt iedere keer welke map je wilt filteren en start opnieuw als je \"1\", \"ja\", of \"j\" typt bij de vraag \"Wil je het ook bij een andere map doen?\".\n\n"
					 "Tenslotte hebben we alleen nog maar de help instructies.\nDat zijn \"-h\", \"--h\", \"-help\" en \"--help\" om zoveel mogelijk vrijheid en gemak te geven aan de gebruiker.\n\n"
					 "Als je nog met vragen zit, kan je mailen naar \"michael-scholten@hotmail.nl\"\n\n";
		// same sentence as in the standard procedure
		std::cout << "\nIk hoop dat je dit programma nog een keer zal gebruiken." << std::endl;
	}
	else if(argc == 2){
		// if the program has entered one argument and it wasn't one of the help commands, check the map with that name
		fileRemover(argv[1]);
		std::cout << "\nIk hoop dat je dit programma nog een keer zal gebruiken." << std::endl;
	}
	else {
		//if the input didn't comply to one of the other tests, tell the user his/her instruction wasn't valid
		std::cout << "Instructie is ongeldig.\n";
	}
}