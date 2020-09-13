#include <string>
#include <vector>
#include <iostream>
#include "Page.cpp"
class Song {

private:
	std::string title;
	std::vector<Page> pages;
	float length;
	int currPage = 0;
	glm::vec3 highlightColor;
	glm::vec3 textColor;


public:
	Song(std::string title, std::vector<Page> pages, float length, glm::vec3 highlight, glm::vec3 textColor) {
		this->title = title;
		this->pages = pages;
		this->length = length;
		this->highlightColor = highlight;
		this->textColor = textColor;
	}

	Song(std::string title, double length, glm::vec3 highlight, glm::vec3 textColor) {
		this->title = title;
		this->length = length;
		this->highlightColor = highlight;
		this->textColor = textColor;
	}

	double getLength() {
		return length;
	}

	glm::vec3 getHighlightColor() {
		return highlightColor;
	}

	glm::vec3 getTextColor() {
		return textColor;
	}

	//returns size of array
	int appendPage(Page page) {
		pages.push_back(page);
		return pages.size();
	}

	std::string getTitle() {
		return title;
	}

	void updatePage(float time) {



		if (currPage + 1 < pages.size() && pages[currPage + 1].getTimestamp() <= time) {
			nextPage();
		}
		if (!pages[currPage].isEmpty()) {
			getPage().updateLine(time);
		}

		
	}

	Page& getPage() {
		return pages[currPage];
	}
	std::vector<Page>& getPages() {
		return pages;
	}
	int  getPageNumber() {
		return currPage;
	}

	int nextPage() {
		return ++currPage;
	}
};