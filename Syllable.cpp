#include "Syllable.h"
#include <string>
#include <iostream>
// > will end every line


class Syllable
{

private:
	double timestamp;
	double endTimestamp;

	std::string text;
	bool space;


public:
	Syllable(double timestamp, double endTimestamp,  std::string text, bool space) {
		this->timestamp = timestamp;
		this->endTimestamp = endTimestamp;

		this->text = text;
		this->space = space;
	}

	double getTimestamp() {
		return timestamp;
	}
	double getEndTimestamp() {
		return endTimestamp;
	}
	std::string getSyllable() {
		if (space)
			return text + " ";
		else
			return text;
	}




};
