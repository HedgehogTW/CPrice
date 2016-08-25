#ifndef MARGIN_H
#define MARGIN_H

#include <string>
#include <vector>
#include "MainData.h"

using namespace std;

class MarginData
{
public:
	MarginData();
	~MarginData();
	
	bool operator < (const MarginData &m)const {
                return ratio < m.ratio;
	}

	bool readLine(char strline[]);	
	
	int fyear;	
	string tic;
	float ib;
	float sales;
	float ratio;
	
	bool match;	
};

class Margin
{
public:
	Margin(string& varName);
	~Margin();
	void loadDataFile();
	void loadVariableData();
	void saveByYear(int year, wxString& filename, vector<MarginData>& vTA);	
	void sortVarData();
	void sortRatio(int year, vector<MarginData>& vTA);
	void outputCombineData();
	int  getIdx(vector<MarginData> &vTA, string ftic);
	
	wxString	m_strFolder;
	string m_varName;
	vector<vector<MarginData>> m_vTAYear;
	vector<TAMainData> m_vTAMainData;
	string  m_titleData;	
};

#endif // MARGIN_H
