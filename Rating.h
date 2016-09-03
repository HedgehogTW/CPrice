#ifndef RATING_H
#define RATING_H

#include <string>
#include <vector>
#include "MainData.h"

using namespace std;

class RatingData
{
public:
	RatingData();
	~RatingData();

	bool readLine(char strline[]);	
	
	string datadate;	
	string tic;
	string splticrm;
	int year;
	int month;
	
	bool match;	
};

class Rating
{
public:
	Rating(string& varName);
	~Rating();
	
	void loadDataFile();
	void loadVariableData();	
	void saveByYear(int year, wxString& filename, vector<RatingData>& vTA);	
	void sortVarData();
	void sortRatio(int year, vector<RatingData>& vTA);	
	void outputCombineData();
	int  getIdx(int year, int month, string ftic);
	
	wxString	m_strFolder;
	string m_varName;
	vector<vector<RatingData>> m_vTAYear;
	vector<TAMainData> m_vTAMainData;
	string  m_titleData;

};

#endif // RATING_H
