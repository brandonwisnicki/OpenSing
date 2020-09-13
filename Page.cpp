#include <vector>
#include "Line.cpp"
#include <glm\ext\vector_float3.hpp>
#include <iostream>


class Page
{

private:
	std::vector<Line> lines;
	double timestamp;
	glm::vec3 bgColor;
	int currLine = 0;

public:
	Page(std::vector<Line> lines, double timestamp, glm::vec3 bgColor) {
		this->lines = lines;
		this->timestamp = timestamp;
		this->bgColor = bgColor;

	}

	Page( double timestamp, glm::vec3 bgColor) {

		this->timestamp = timestamp;
		this->bgColor = bgColor;
	}


	std::vector<Line>& getLines() {
		return lines;
	}

	bool isEmpty() {
		if (lines.size() == 0) {
			return true;
		}
		return false;
	}


	Line& getCurrLine() {
		return lines[currLine];
	}

	void updateLine(float time) {

		//std::cout << "time: " << time << " timestamp: " << lines[currLine + 1].getTimestamp() << std::endl;
		getCurrLine().updateSyllable(time);

		if (currLine + 1 < lines.size() && lines[currLine + 1].getTimestamp() <= time) {

			nextLine();
		}


		//std::cout << currLine;

	}
	int nextLine() {
		return ++currLine;
	}
	int  getLineNumber() {
		return currLine;
	}

	//returns size of array
	int appendLine(Line line) {
		lines.push_back(line);
		return lines.size();
	}

	double  getTimestamp() {
		return timestamp;
	}

	glm::vec3 getBgColor() {
		return bgColor;
	}

};

