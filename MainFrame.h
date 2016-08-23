#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"
#include <string>
#include <vector>
#include <wx/arrstr.h>


#include "MainData.h"
using namespace std;

class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();
	
	int readMainData(string pathName);
	void setDataFileName();
	void processCL(wxArrayString& names);
	void processHO(wxArrayString& names);
	void processNG(wxArrayString& names);
	void saveData(const char* fname);
	
	///////////////////// TA
	void saveTAyear(int year, wxString& filename, vector<TA>& vTA);
	void sortTAByat(string& varName, int year, vector<TA>& vTA);
	int  getIdx(vector<TA> &vTA, string ftic);
	void loadDataFile();
	void loadVariableData(string& varName);
	void sortVarData(string& varName);
	void outputCombineData(string& varName);
	void saveSeparatedYearData(string& varName);
	
	wxString	m_strFolder;
	wxString m_strMainTitle;
	vector<MainData> m_vMain;
	wxArrayString  	m_CLnames;
	wxArrayString  	m_HOnames;
	wxArrayString 	m_NGnames;
	
	vector<vector<TA>> m_vTAYear;
	vector<TAData> m_vTAData;
	string  m_titleData;
	
/////////////////////////	
	
	static void ShowMessage(wxString szFormat,...) {
		wxString strMsg;
		va_list argList;
		va_start(argList, szFormat);
		strMsg.PrintfV(szFormat, argList);
		va_end(argList); 
		// 以上這幾行是用來處理類似printf 的參數

		m_pThis->m_richTextMsg->AppendText(strMsg);
		int lastPos = m_pThis->m_richTextMsg->GetLastPosition();
		m_pThis->m_richTextMsg->ShowPosition(lastPos);
	}


    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	
	static MainFrame *m_pThis;

protected:
    virtual void OnProcessMKVALT(wxCommandEvent& event);
    virtual void OnProcessTA(wxCommandEvent& event);
    virtual void OnProcessEMP(wxCommandEvent& event);
    virtual void OnProcessLCT(wxCommandEvent& event);
    virtual void OnProcessLT(wxCommandEvent& event);
    virtual void OnProcessSALE(wxCommandEvent& event);
    virtual void OnTACombineData(wxCommandEvent& event);
    virtual void OnTASortAt(wxCommandEvent& event);
    virtual void OnTACheckTA(wxCommandEvent& event);
    virtual void OnSelectFolder(wxCommandEvent& event);
    virtual void OnPriceProcess(wxCommandEvent& event);
};
#endif // MAINFRAME_H
