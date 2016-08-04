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

class TA 
{
public:
	TA();
	~TA();
	
	int gvkey;
	int datadate;
	int fyear;	
	string tic;
	float at;
};