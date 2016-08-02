
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