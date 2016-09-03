#pragma once

#include <string>
#include <wx/string.h>

using namespace std;

class MainData
{
public:
	MainData();
	~MainData();

	void readLine(wxString& strline);
	
	
	int m_date;
	int m_time;
	int m_minute;
	int m_qe;
	double m_surprise;
	float m_vix;
	
	float m_CL_b10;
	float m_CL_a20;
	float m_CL_b15;
	float m_CL_a105;

	float m_HO_b10;
	float m_HO_a20;
	float m_HO_b15;
	float m_HO_a105;

	float m_NG_b10;
	float m_NG_a20;
	float m_NG_b15;
	float m_NG_a105;
};

class PriceData
{
public:
	PriceData();
	~PriceData();

	void readLine(wxString& strline);	
	void readLine(char strline[]);	
	
	int m_date;
	int m_hh;
	int m_mm;
	int m_minute;
	float m_price;
};
////////////////////////// new task
class TAMainData
{
public:
	TAMainData();
	~TAMainData();
	
	bool readLine(char strline[]);
	
	int firmID;
	string firm_tic;
	int year;
	int month;
	string strLater;
	int big33, mid33, small33;
	int matched;
	int DIVD;
	int dd;
};

class TA 
{
public:
	TA();
	~TA();
    TA& operator=(TA other)
    {
        
        std::swap(gvkey, other.gvkey);
        std::swap(datadate, other.datadate);
        std::swap(fyear, other.fyear);
        std::swap(tic, other.tic);	
        std::swap(at, other.at);		
        return *this;
    }
	bool operator < (const TA &m)const {
                return at < m.at;
	}

	bool readLine(char strline[]);	
	
	int gvkey;
	int datadate;
	int fyear;	
	string tic;
	float at;
	bool match;

};

