
#include "MainData.h"

#include "MainFrame.h"

MainData::MainData()
{
	m_CL_b10 = 0;
	m_CL_a20 = 0;
	m_CL_b15 = 0;
	m_CL_a105 = 0;

	m_HO_b10 = 0;
	m_HO_a20 = 0;
	m_HO_b15 = 0;
	m_HO_a105 = 0;

	m_NG_b10 = 0;
	m_NG_a20 = 0;
	m_NG_b15 = 0;
	m_NG_a105 = 0;
	
	m_minute = 0;
}


MainData::~MainData()
{
}
void MainData::readLine(wxString& strline)
{
	
	const char* str = strline.c_str();
	sscanf(str, "%d,%d,%d,%lf,%f", &m_date, &m_time, &m_qe, &m_surprise, &m_vix);
	m_minute = 60*(m_time /100) + (m_time %100);
	
//	MainFrame::ShowMessage("%d, %d, %d, %.9f, %.2f\n", m_date, m_time, m_qe, m_surprise, m_vix);
		
}

///////////////////////////////
// PriceData class
///////////////////////////////

PriceData::PriceData()
{
	m_minute = m_price = 0;
	m_hh = m_mm = 0;
}


PriceData::~PriceData()
{
}
void PriceData::readLine(wxString& strline)
{
	
	const char* str = strline.c_str();
	sscanf(str, "%d%d:%d%f,%*d", &m_date, &m_hh, &m_mm, &m_price);
	m_minute = 60*m_hh + m_mm;
//	MainFrame::ShowMessage("%d, %d:%d, %.2f\n", m_date, hh, mm, m_price);
		
}

void PriceData::readLine(char strline[])
{
	
	//const char* str = strline.c_str();
	sscanf(strline, "%d%d:%d%f,%*d", &m_date, &m_hh, &m_mm, &m_price);
	m_minute = 60*m_hh + m_mm;
//	MainFrame::ShowMessage("%d, %d:%d, %.2f\n", m_date, hh, mm, m_price);
		
}

///////////////////////////////
// TA class
///////////////////////////////
TA::TA()
{
	tic.clear();
	match = false;
}


TA::~TA()
{
}

bool TA::readLine(char strline[])
{
	
	char stic[20];
	int n =sscanf(strline, "%d%d%d%s%f", &gvkey, &datadate, &fyear, stic, &at);
	tic = string(stic);
	//MainFrame::ShowMessage("%d %d %d %s %f\n", gvkey, datadate, fyear, stic, at);
	if(n!= 5)
		return false;
	else
		return true;
}

///////////////////////////////
// TAData class
///////////////////////////////
TAData::TAData()
{
	big33 = mid33 = small33 = 0;
	matched = 1;
}


TAData::~TAData()
{
}

bool TAData::readLine(char strline[])
{
	
	char tic[20];
	int n =sscanf(strline, "%d%s%d%d", &firmID, &tic, &year, &ddate);
	firm_tic = string(tic);
	strline[strlen(strline)-1] = 0;
	strLater = string(strline + 43);
	if(n==4)
		return true;
	else
		return false;
}