#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <wx/dirdlg.h> 
#include <wx/string.h>
#include <wx/wfstream.h>
#include <algorithm>

#include "Margin.h"

MainFrame *MainFrame::m_pThis=NULL;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
	m_pThis = this;

	m_strFolder = "d:\\tmp\\data\\";
	m_textCtrlFolder->SetValue(m_strFolder);
	setDataFileName();
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("My MainFrame"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
}
void MainFrame::OnPriceProcess(wxCommandEvent& event)
{
	string filename = m_strFolder.ToStdString() + "\\DATE0730.csv";
	int c = readMainData(filename);
	
	wxString msg = "read " + filename + "\n";
	ShowMessage(msg);
	ShowMessage("read %d records\n", c);
	
	processCL(m_CLnames);
	processHO(m_HOnames);
	processNG(m_NGnames);
	saveData(filename.c_str());
}

void MainFrame::saveData(const char* fname)
{
	FILE* fp = fopen(fname, "w");
//	ShowMessage(m_strMainTitle);
	char *str = m_strMainTitle.char_str();
	fprintf(fp, "%s\n", str);
	int count = m_vMain.size();
	for(int i=0; i<count; i++) {
		fprintf(fp, "%d, %d, %d, %.9f, %.2f, ",
			m_vMain[i].m_date, m_vMain[i].m_time, m_vMain[i].m_qe, m_vMain[i].m_surprise, m_vMain[i].m_vix);
			
		fprintf(fp, "%.2f, %.2f, %.2f, %.2f, ",
			m_vMain[i].m_CL_b10, m_vMain[i].m_CL_a20, m_vMain[i].m_CL_b15, m_vMain[i].m_CL_a105);

		fprintf(fp, "%.4f, %.4f, %.4f, %.4f, ",
			m_vMain[i].m_HO_b10, m_vMain[i].m_HO_a20, m_vMain[i].m_HO_b15, m_vMain[i].m_HO_a105);	

		fprintf(fp, "%.3f, %.3f, %.3f, %.3f\n",
			m_vMain[i].m_NG_b10, m_vMain[i].m_NG_a20, m_vMain[i].m_NG_b15, m_vMain[i].m_NG_a105);
	}
	fclose(fp);
}

void MainFrame::processCL(wxArrayString& dataNames)
{
	char strline[200];
	vector<FILE*> vFp(dataNames.Count());
	for(int i=0; i<vFp.size(); i++){
		vFp[i] = fopen(dataNames[i].c_str(), "r");
		if(vFp[i] ==NULL) {
			ShowMessage("Open "+dataNames[i]+" error\n");	
			return;
		}
		fgets(strline, 200, vFp[i]);
	}
	
	int count = m_vMain.size();
	for(int i=0; i<count; i++) {
		int year = m_vMain[i].m_date /10000;
		FILE* fp = NULL;
		switch (year) {
			case 2008:
			case 2009:  fp = vFp[0]; break;
			case 2010:  fp = vFp[1]; break;
			case 2011:  fp = vFp[2]; break;
			case 2012:  fp = vFp[3]; break;
			case 2013:  fp = vFp[4]; break;	
			default:	
				ShowMessage("incorrect year %d \n", year);
				return;
		}
		
		int bFound = 0;
		while (!feof(fp)) {
			char* p = fgets(strline, 200, fp);
			if(p==NULL) break;

			PriceData  oneData;
			oneData.readLine(strline);
			
			if(m_vMain[i].m_date == oneData.m_date) {
				if(m_vMain[i].m_minute -10== oneData.m_minute ) {
					m_vMain[i].m_CL_b10 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute +20== oneData.m_minute ) {
					m_vMain[i].m_CL_a20 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute -15== oneData.m_minute ) {
					m_vMain[i].m_CL_b15 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute +105== oneData.m_minute ) {
					m_vMain[i].m_CL_a105 = oneData.m_price;			
					bFound++;
				}			
			}
			if(bFound==4) {
				ShowMessage("find CL %d %d %d -->%d, %d:%d, %d\n", 
					m_vMain[i].m_date, m_vMain[i].m_time, m_vMain[i].m_minute, 
					oneData.m_date, oneData.m_hh, oneData.m_mm, oneData.m_minute);
				break;
			}
			if(m_vMain[i].m_date < oneData.m_date)
				break;
		}
		if(bFound ==false) {
			ShowMessage("cannot not find CL record %d, %d\n", m_vMain[i].m_date, m_vMain[i].m_time );	
		}
		
	}
	
	for(int i=0; i<vFp.size(); i++)
		fclose(vFp[i]);
}


void MainFrame::processHO(wxArrayString& dataNames)
{
	char strline[200];
	vector<FILE*> vFp(dataNames.Count());
	for(int i=0; i<vFp.size(); i++){
		vFp[i] = fopen(dataNames[i].c_str(), "r");
		if(vFp[i] ==NULL) {
			ShowMessage("Open "+dataNames[i]+" error\n");	
			return;
		}
		fgets(strline, 200, vFp[i]);
	}
	
	int count = m_vMain.size();
	for(int i=0; i<count; i++) {
		int year = m_vMain[i].m_date /10000;
		FILE* fp = NULL;
		switch (year) {
			case 2008:
			case 2009:  fp = vFp[0]; break;
			case 2010:  fp = vFp[1]; break;
			case 2011:  fp = vFp[2]; break;
			case 2012:  fp = vFp[3]; break;
			case 2013:  fp = vFp[4]; break;	
			default:	
				ShowMessage("incorrect year %d \n", year);
				return;
		}
		
		int bFound = 0;
		while (!feof(fp)) {
			char* p = fgets(strline, 200, fp);
			if(p==NULL) break;

			PriceData  oneData;
			oneData.readLine(strline);
			
			if(m_vMain[i].m_date == oneData.m_date) {
				if(m_vMain[i].m_minute -10== oneData.m_minute ) {
					m_vMain[i].m_HO_b10 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute +20== oneData.m_minute ) {
					m_vMain[i].m_HO_a20 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute -15== oneData.m_minute ) {
					m_vMain[i].m_HO_b15 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute +105== oneData.m_minute ) {
					m_vMain[i].m_HO_a105 = oneData.m_price;			
					bFound++;
				}			
			}
			if(bFound==4) {
				//ShowMessage("find HO %d %d\n", m_vMain[i].m_date, m_vMain[i].m_time);
				break;
			}
			if(m_vMain[i].m_date < oneData.m_date)
				break;
		}
		if(bFound ==false) {
			ShowMessage("cannot not HO find record %d, %d\n", m_vMain[i].m_date, m_vMain[i].m_time );	
		}
		
	}
	
	for(int i=0; i<vFp.size(); i++)
		fclose(vFp[i]);
}


void MainFrame::processNG(wxArrayString& dataNames)
{
	char strline[200];
	vector<FILE*> vFp(dataNames.Count());
	for(int i=0; i<vFp.size(); i++){
		vFp[i] = fopen(dataNames[i].c_str(), "r");
		if(vFp[i] ==NULL) {
			ShowMessage("Open "+dataNames[i]+" error\n");	
			return;
		}
		fgets(strline, 200, vFp[i]);
	}
	
	int count = m_vMain.size();
	for(int i=0; i<count; i++) {
		int year = m_vMain[i].m_date /10000;
		FILE* fp = NULL;
		switch (year) {
			case 2008:
			case 2009:  fp = vFp[0]; break;
			case 2010:  fp = vFp[1]; break;
			case 2011:  fp = vFp[2]; break;
			case 2012:  fp = vFp[3]; break;
			case 2013:  fp = vFp[4]; break;	
			default:	
				ShowMessage("incorrect year %d \n", year);
				return;
		}
		
		int bFound = 0;
		while (!feof(fp)) {
			char* p = fgets(strline, 200, fp);
			if(p==NULL) break;

			PriceData  oneData;
			oneData.readLine(strline);
			
			if(m_vMain[i].m_date == oneData.m_date) {
				if(m_vMain[i].m_minute -10== oneData.m_minute ) {
					m_vMain[i].m_NG_b10 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute +20== oneData.m_minute ) {
					m_vMain[i].m_NG_a20 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute -15== oneData.m_minute ) {
					m_vMain[i].m_NG_b15 = oneData.m_price;
					bFound++;
				}
				if(m_vMain[i].m_minute +105== oneData.m_minute ) {
					m_vMain[i].m_NG_a105 = oneData.m_price;			
					bFound++;
				}			
			}
			if(bFound==4) {
				//ShowMessage("find NG %d %d\n", m_vMain[i].m_date, m_vMain[i].m_time);
				break;
			
			}
			if(m_vMain[i].m_date < oneData.m_date)
				break;
		}
		if(bFound ==false) {
			ShowMessage("cannot not find NG record %d, %d\n", m_vMain[i].m_date, m_vMain[i].m_time );	
		}
		
	}
	
	for(int i=0; i<vFp.size(); i++)
		fclose(vFp[i]);
}
void MainFrame::setDataFileName()
{
	m_CLnames.clear();
	m_CLnames.Add(m_strFolder+"0809CL.txt");
	m_CLnames.Add(m_strFolder+"2010CL.txt");
	m_CLnames.Add(m_strFolder+"2011CL.txt");
	m_CLnames.Add(m_strFolder+"2012CL.txt");
	m_CLnames.Add(m_strFolder+"2013CL.txt");


	m_HOnames.clear();
	m_HOnames.Add(m_strFolder+"HO0809HO.txt");
	m_HOnames.Add(m_strFolder+"HO2010HO.txt");
	m_HOnames.Add(m_strFolder+"HO2011HO.txt");
	m_HOnames.Add(m_strFolder+"HO2012HO.txt");
	m_HOnames.Add(m_strFolder+"HO2013HO.txt");

	m_NGnames.clear();
	m_NGnames.Add(m_strFolder+"NGO0809NG.txt");
	m_NGnames.Add(m_strFolder+"NG2010NG.txt");
	m_NGnames.Add(m_strFolder+"NG2011NG.txt");
	m_NGnames.Add(m_strFolder+"NG2012NG.txt");
	m_NGnames.Add(m_strFolder+"NG2013NG.txt");	
}
int MainFrame::readMainData(string pathName)
{
	wxFileInputStream input(pathName);
	wxTextInputStream text(input);
	m_strMainTitle = text.ReadLine();
	while (input.IsOk() && !input.Eof()) {
		wxString strline = text.ReadLine();
		if (strline.IsEmpty()) break;

		MainData  oneData;
		oneData.readLine(strline);
		m_vMain.push_back(oneData);
	}	
	int count = m_vMain.size();
	
	return count;
}
void MainFrame::OnSelectFolder(wxCommandEvent& event)
{
	m_strFolder = wxDirSelector("Choose a folder", m_strFolder);
	if ( !m_strFolder.empty() ){
		m_textCtrlFolder->SetValue(m_strFolder);
		setDataFileName();
	}
}

void MainFrame::OnTACheckTA(wxCommandEvent& event)
{
	string varName = string("TA");
	loadVariableData(varName);
}

void MainFrame::loadVariableData(string& varName)
{
	char strline[200];
	string filename = m_strFolder.ToStdString() + varName+".txt";
	
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}
	fgets(strline, 200, fp);
	
	vector<TA> vTA;
	int count = 0;
	int count0813 = 0;
	vector<TA> vTA0813;
	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;
		count++;
		TA  oneData;
		if(oneData.readLine(strline)==true) {
			if(oneData.fyear ==2008) {
				vTA0813.clear();
				vTA0813.push_back(oneData);
				count0813 = 1;
				break;
			}
		}
	} // read first record
	
	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;
		
		count++;
		TA  oneData;
		if(oneData.readLine(strline)==true) {
			if(oneData.tic.compare(vTA0813[0].tic)==0) {
				if( oneData.fyear == vTA0813[count0813-1].fyear+1) {
					vTA0813.push_back(oneData);
					count0813++;
				}
			}else {
				if(count0813==6) {
					for(int i=0; i<6; i++)
						vTA.push_back(vTA0813[i]);
					count0813 = 1;
				}
				if(oneData.fyear ==2008) {
					vTA0813.clear();
					vTA0813.push_back(oneData);
					count0813 = 1;
				}				
			}
		}
	}

			
	fclose(fp);
	ShowMessage("read %d/%d records\n", vTA.size(), count);
	
	// check tic match
	loadDataFile();

	for(int i=0; i<vTA.size(); i++) {
		bool bFound = false;
		for(int k=0; k<m_vTAMainData.size(); k++) {
			if(vTA[i].tic.compare(m_vTAMainData[k].firm_tic)==0) {
				bFound = true;
				break;
			}
		}
		if(bFound) vTA[i].match = true;
		else vTA[i].match = false;
	}
	
	filename = m_strFolder.ToStdString() + varName + "_clear.txt";
	fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<vTA.size(); i++) {
		if(vTA[i].match)
			fprintf(fp, "%d  %d  %d  %s  %.4f\n", 
				vTA[i].gvkey, vTA[i].datadate, vTA[i].fyear, vTA[i].tic.c_str(), vTA[i].at);
	}
	fclose(fp);
	
	for(int i=2008; i<=2013; i++) {
		wxString filename = m_strFolder+ varName;
		filename << "_" << i << ".txt";
		saveTAyear(i, filename, vTA);
	
	}
	
	
	wxString msg;
	msg<<"output var file " << vTA.size() << " :"<< filename <<"\n";
	ShowMessage(msg);	
}
void MainFrame::saveTAyear(int year, wxString& filename, vector<TA>& vTA)
{

	FILE* fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<vTA.size(); i++) {
		if(vTA[i].match)
			if(vTA[i].fyear==year)
				fprintf(fp, "%d  %d  %d  %s  %.4f\n", 
					vTA[i].gvkey, vTA[i].datadate, vTA[i].fyear, vTA[i].tic.c_str(), vTA[i].at);
	}
	fclose(fp);	
	
	wxString msg;
	msg<<"output var file  :"<< filename << "\n";
	ShowMessage(msg);	
}
void MainFrame::OnTASortAt(wxCommandEvent& event)
{
	string varName = string("TA");
	sortVarData(varName);

}

void MainFrame::sortVarData(string& varName)
{
	vector<TA> vTA;
	m_vTAYear.clear();
	
	for(int i=2008; i<=2013; i++) {
		wxString savename = m_strFolder + varName;
		savename << "_" << i << "sort.csv";
	
		sortTAByat(varName, i, vTA);	
		m_vTAYear.push_back(vTA);
	}	
}
void MainFrame::sortTAByat(string& varName, int year, vector<TA>& vTA)
{
	
	wxString filename = m_strFolder+ varName;
	filename << "_" << year << ".txt";
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}
	char strline[200];
	vTA.clear();
	int count = 0;

	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;

		TA  oneData;
		if(oneData.readLine(strline)==true) {
			vTA.push_back(oneData);
			count++;
		}
	} // read first record	
	fclose(fp);	
	wxString msg;
	msg << "Read " << filename << " " << count << " records, size " << vTA.size() << "\n";
	
	ShowMessage(msg);
	
    sort(vTA.begin(), vTA.end());

////////////////////////// save
	wxString savename = m_strFolder + varName;
	savename << "_" << year << "sort.csv";
	fp = fopen(savename.c_str(), "w");
	if(fp ==NULL) {
		ShowMessage("Open "+savename+" error\n");	
		return;
	}
	for(int i=0; i<vTA.size(); i++) {
		fprintf(fp, "%d, %d, %d, %s, %.4f\n", 
				vTA[i].gvkey, vTA[i].datadate, vTA[i].fyear, vTA[i].tic.c_str(), vTA[i].at);
	}
	fclose(fp);	
}
void MainFrame::loadDataFile()
{
	char strTitle[200];
	char strline[200];
	string filename = m_strFolder.ToStdString() + "\\data20160715.txt";
	
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}

	fgets(strTitle, 200, fp);
	strTitle[strlen(strTitle)-1] = 0;
//	strTitle[strlen(strTitle)-2] = 0;
	m_titleData = string(strTitle) + "  big33  mid33  small33";
	
	m_vTAMainData.clear();
	int count = 0;
	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;
		TAMainData  oneData;
		if(oneData.readLine(strline)==true) {
			m_vTAMainData.push_back(oneData);
			count++;
		}
	} 
	fclose(fp);	
	
	wxString msg;
	msg << "Read " << filename << " " << count << " records\n";
	ShowMessage(msg);	
}
void MainFrame::OnTACombineData(wxCommandEvent& event)
{
	string varName = string("TA");
	outputCombineData(varName);
}
	
void MainFrame::outputCombineData(string& varName)
{
	// combine TA ..............
	string oldTic;
	for(int i=0; i<m_vTAMainData.size(); i++) {
		int idx = getIdx(m_vTAYear[m_vTAMainData[i].year - 2008], m_vTAMainData[i].firm_tic);
		if(idx ==0) {
			if(m_vTAMainData[i].firm_tic.compare(oldTic)!=0) {
				wxString msg;
				msg << "Var Data ERR: " << i+1 << ", year " << m_vTAMainData[i].year << ", firm_tic:" << m_vTAMainData[i].firm_tic 
					<< ", date:" << m_vTAMainData[i].ddate << "\n";
				ShowMessage(msg);
				oldTic = m_vTAMainData[i].firm_tic;
			}
			m_vTAMainData[i].matched = 0;
		}else if(idx ==1) {
			m_vTAMainData[i].small33 = 1;
			m_vTAMainData[i].mid33 = 0;
			m_vTAMainData[i].big33 = 0;
		}else if(idx ==2) {
			m_vTAMainData[i].small33 = 0;
			m_vTAMainData[i].mid33 = 1;
			m_vTAMainData[i].big33 = 0;
		}else if(idx ==3) {
			m_vTAMainData[i].small33 = 0;
			m_vTAMainData[i].mid33 = 0;
			m_vTAMainData[i].big33 = 1;
		}
	}
	
	// output file
	wxString savename = m_strFolder + varName;
	savename << "_result.txt";	
	FILE* fp = fopen(savename.c_str(), "w");
	fprintf(fp, "%s\n", m_titleData.c_str());
	int match = 0;	
	for(int i=0; i<m_vTAMainData.size(); i++) {
		if(m_vTAMainData[i].matched ==0) continue;
		fprintf(fp, "%6d   %10s   %8d  %d  %s  %5d  %5d  %5d\n", 
		m_vTAMainData[i].firmID, m_vTAMainData[i].firm_tic.c_str(), m_vTAMainData[i].year, m_vTAMainData[i].ddate, m_vTAMainData[i].strLater.c_str(),
		m_vTAMainData[i].big33, m_vTAMainData[i].mid33, m_vTAMainData[i].small33);
		match ++;
	}
	fclose(fp);
	
	wxString msg;
	msg << "Read Data size " << m_vTAMainData.size() << ", match: " << match << "\n";
	ShowMessage(msg);	
	
	saveSeparatedYearData(varName);

}

void MainFrame::saveSeparatedYearData(string& varName)
{
	// save separated year data
	FILE* fp[6];
	wxString savename = m_strFolder + varName + "_result08.csv";	
	fp[0] = fopen(savename.c_str(), "w");
	
	savename = m_strFolder + varName + "_result09.csv";	
	fp[1] = fopen(savename.c_str(), "w");

	savename = m_strFolder + varName + "_result10.csv";	
	fp[2] = fopen(savename.c_str(), "w");

	savename = m_strFolder + varName + "_result11.csv";	
	fp[3] = fopen(savename.c_str(), "w");

	savename = m_strFolder + varName + "_result12.csv";	
	fp[4] = fopen(savename.c_str(), "w");

	savename = m_strFolder + varName + "_result13.csv";	
	fp[5] = fopen(savename.c_str(), "w");

	
	int match = 0;	
	for(int i=0; i<m_vTAMainData.size(); i++) {
		if(m_vTAMainData[i].matched ==0) continue;
		
		fprintf(fp[m_vTAMainData[i].year-2008], "%d, %s, %d, %d, %d, %d, %d\n", 
		m_vTAMainData[i].firmID, m_vTAMainData[i].firm_tic.c_str(), m_vTAMainData[i].year, m_vTAMainData[i].ddate,
		m_vTAMainData[i].big33, m_vTAMainData[i].mid33, m_vTAMainData[i].small33);
		match ++;
	}
	
	for(int i=0; i<6; i++)
		fclose(fp[i]);		
}
int MainFrame::getIdx(vector<TA> &vTA, string ftic)
{
	int ret = 0;
	int idx = -1; 
	int sz = vTA.size();
	for (int i=0; i<sz; i++){
		if(vTA[i].tic.compare(ftic)==0) {
			idx = i;
			break;
		}
	}

	if(idx >= 0) {
		float r = (float) idx / sz;
		if(r <= 1./3.) ret = 1;
		else if(r <= 2./3.) ret = 2;
		else ret = 3;
	}else ret = 0;

	return ret;
	
}
void MainFrame::OnProcessEMP(wxCommandEvent& event)
{
	string varName = string("EMP");
	loadVariableData(varName);
	sortVarData(varName);
	outputCombineData(varName);	
}
void MainFrame::OnProcessLCT(wxCommandEvent& event)
{
	string varName = string("LCT");
	loadVariableData(varName);
	sortVarData(varName);
	outputCombineData(varName);	
}
void MainFrame::OnProcessLT(wxCommandEvent& event)
{
	string varName = string("LT");
	loadVariableData(varName);
	sortVarData(varName);
	outputCombineData(varName);	
}
void MainFrame::OnProcessSALE(wxCommandEvent& event)
{
	string varName = string("SALES");
	loadVariableData(varName);
	sortVarData(varName);
	outputCombineData(varName);	
}
void MainFrame::OnProcessTA(wxCommandEvent& event)
{
	string varName = string("TA");
	loadVariableData(varName);
	sortVarData(varName);
	outputCombineData(varName);
}


void MainFrame::OnProcessMKVALT(wxCommandEvent& event)
{
	wxBeginBusyCursor ();

	string varName = string("MKVALT");
	loadVariableData(varName);
	sortVarData(varName);
	outputCombineData(varName);	
	wxEndBusyCursor ();

}
void MainFrame::OnProcessEPS(wxCommandEvent& event)
{
	wxBeginBusyCursor ();

	string varName = string("EPS");
	loadVariableData(varName);
	sortVarData(varName);
	outputCombineData(varName);	
	wxEndBusyCursor ();
	wxBell();
}

void MainFrame::OnDPRATIO(wxCommandEvent& event)
{
	wxBeginBusyCursor ();
	
	string varName = string("DPRATIO");
	Margin  var(varName);
	var.loadVariableData();
	var.sortVarData();
	var.outputCombineData();
	wxEndBusyCursor ();
	wxBell();		
}
void MainFrame::OnMARGINS(wxCommandEvent& event)
{
	wxBeginBusyCursor ();
	
	string varName = string("MARGINS");
	Margin  var(varName);
	var.loadVariableData();
	var.sortVarData();
	var.outputCombineData();
	wxEndBusyCursor ();
	wxBell();		
}
void MainFrame::OnRETURNONP(wxCommandEvent& event)
{
	wxBeginBusyCursor ();
	
	string varName = string("RETURNONP");
	Margin  var(varName);
	var.loadVariableData();
	var.sortVarData();
	var.outputCombineData();	
	wxEndBusyCursor ();
	wxBell();	
}
