#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "json.hpp"
#include "Song.cpp"


#include "GlyphManager.cpp"

using namespace std;
using json = nlohmann::json;



class SongLoader {

private:
	GlyphManager glyphs;

public:

	SongLoader() {
		cout << "Song Loader initialized" << endl;

	}


	Song loadSong(string fileName) {
		glyphs.initGlyphs();

		string line;
		string data;
		ifstream songFile;
		songFile.open(fileName);
		string output;
		while (getline(songFile, line)) {
			// Output the text from the file
			data += line;
		}

		json jsonData;
		try {

			jsonData = json::parse(data);

		}
		catch (...) {
			cout << "Error parsing song file" << endl;
		}
		glm::vec3 highlight = glm::vec3(jsonData["highlightColor"][0], jsonData["highlightColor"][1], jsonData["highlightColor"][2]);
		glm::vec3 text = glm::vec3(jsonData["textColor"][0], jsonData["textColor"][1], jsonData["textColor"][2]);

		Song song(jsonData["name"], jsonData["length"], highlight, text);

		//pagesData = json::parse(jsonData["pages"]);
		json pagesData = jsonData["pages"];


		
		for (int i = 0; i < pagesData.size(); i++) {

			Page newPage(pagesData[i]["timestamp"], glm::vec3(pagesData[i]["bgCol"][0], pagesData[i]["bgCol"][1], pagesData[i]["bgCol"][2]));

			json lineData = pagesData[i]["lines"];
			for (int j = 0; j < lineData.size(); j++)
			{
				Line newLine;
				json syllableData = lineData[j]["syllables"];

				for (int k = 0; k < syllableData.size(); k++) {
					cout << "		appending NEW SYLL" << endl;
					newLine.appendSyllable(Syllable(syllableData[k]["timestamp"], syllableData[k]["endTimestamp"], syllableData[k]["text"], syllableData[k]["space"]));
				}

				cout << "	appending NEW LINE" << endl;

                newLine.setPixelWidth(glyphs.getPixelLength(newLine.getLineText()));

				//cout << newLine.getPixelWidth() << endl;
				newPage.appendLine(newLine);
			}

			cout << "appending NEW PAGE" << endl;

			song.appendPage(newPage);
		}


		songFile.close();

		return song;
	}

	




};

