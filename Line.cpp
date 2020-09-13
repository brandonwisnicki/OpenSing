#include <vector>
#include "Syllable.cpp"
#include <string>

class Line {

private:
	std::vector<Syllable> syllables;
	int currSyllable = 0;
	double endTimestamp = 0;
	float pixelWidth =0;

public:
	
	void setPixelWidth(float w) {

		pixelWidth = w;
	}

	float getPixelWidth() {
		return pixelWidth;
	}

	float getTimestamp() {

		if (syllables.size() > 0)
			return syllables[0].getTimestamp();
		else
			return NULL;
	}

	std::vector<Syllable>& getSyllables() {
		return syllables;
	}


	void updateSyllable(float time) {

		//std::cout << "time: " << time << " timestamp: " << lines[currLine + 1].getTimestamp() << std::endl;

		if (currSyllable + 1 < syllables.size() && syllables[currSyllable + 1].getTimestamp() <= time) {

			nextSyllable();
		}
		//std::cout << currLine;

	}
	int nextSyllable() {
		return ++currSyllable;
	}


	float getLengthToHighlight(float time) {

		float length = 0;
		for (int i = 0; i < currSyllable; i++) {
			length += syllables[i].getSyllable().size();
		}

			int currSyllLength = syllables[currSyllable].getSyllable().size();


			double totalLength = syllables[currSyllable].getEndTimestamp() - syllables[currSyllable].getTimestamp();
			double timePassed = time - syllables[currSyllable].getTimestamp();

			double percent = timePassed / totalLength;

			if (percent > 1.0) {
				percent = 1.0;
			}

			float currSyllHighlight = percent * syllables[currSyllable].getSyllable().size();
			length += currSyllHighlight;

		

		return length;

	}

	//returns size of array
	int appendSyllable(Syllable syllable) {
		syllables.push_back(syllable);
		return syllables.size();
	}

	std::string getLineText() {
		std::string text = "";
		for (int i = 0; i < syllables.size(); i++) {
			text += syllables[i].getSyllable();
		}
		return text;
	}



/*
	float getLineWidth() {

		std::string lineText = "";

		

		std::string::const_iterator c;
		float totalWidth = 0;
		for (c = lineText.begin(); c != lineText.end(); c++)
		{

			Character ch = FontManager::getCharacters()[*c];
			float w = ch.Size.x;

			totalWidth += w;


		}

		std::cout << "total width is: " << std::endl;

		return totalWidth;
	}
	*/
};
