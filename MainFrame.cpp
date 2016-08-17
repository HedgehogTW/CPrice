#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <wx/dirdlg.h> 
#include <wx/string.h>
#include <wx/wfstream.h>
#include <algorithm>

MainFrame *MainFrame::m_pThis=NULL;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
	m_pThis = this;

	m_strFolder = "d:\\tmp\\data";
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
	m_CLnames.Add(m_strFolder+"\\0809CL.txt");
	m_CLnames.Add(m_strFolder+"\\2010CL.txt");
	m_CLnames.Add(m_strFolder+"\\2011CL.txt");
	m_CLnames.Add(m_strFolder+"\\2012CL.txt");
	m_CLnames.Add(m_strFolder+"\\2013CL.txt");


	m_HOnames.clear();
	m_HOnames.Add(m_strFolder+"\\HO0809HO.txt");
	m_HOnames.Add(m_strFolder+"\\HO2010HO.txt");
	m_HOnames.Add(m_strFolder+"\\HO2011HO.txt");
	m_HOnames.Add(m_strFolder+"\\HO2012HO.txt");
	m_HOnames.Add(m_strFolder+"\\HO2013HO.txt");

	m_NGnames.clear();
	m_NGnames.Add(m_strFolder+"\\NGO0809NG.txt");
	m_NGnames.Add(m_strFolder+"\\NG2010NG.txt");
	m_NGnames.Add(m_strFolder+"\\NG2011NG.txt");
	m_NGnames.Add(m_strFolder+"\\NG2012NG.txt");
	m_NGnames.Add(m_strFolder+"\\NG2013NG.txt");	
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
	char strline[200];
	string filename = m_strFolder.ToStdString() + "\\TA.txt";
	
	
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}
	fgets(strline, 200, fp);
	m_vTA.clear();
	int count = 0;
	int count0813 = 0;
	vector<TA> vTA0813;
	while (!feof(fp)) {
		fgets(strline, 200, fp);
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
						m_vTA.push_back(vTA0813[i]);
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
	ShowMessage("read %d/%d records\n", m_vTA.size(), count);
	
	filename = m_strFolder.ToStdString() + "\\TA_clear.txt";
	fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<m_vTA.size(); i++) {
		fprintf(fp, "%d  %d  %d  %s  %.4f\n", 
			m_vTA[i].gvkey, m_vTA[i].datadate, m_vTA[i].fyear, m_vTA[i].tic.c_str(), m_vTA[i].at);
	}
	fclose(fp);
	
	for(int i=2008; i<=2013; i++) {
		wxString filename = m_strFolder;
		filename << "\\TA_" << i << ".txt";
		saveTAyear(i, filename);
	
	}
	
	
	wxString msg;
	msg<<"output TA file " << m_vTA.size() << " :"<< filename <<"\n";
	ShowMessage(msg);
}
void MainFrame::saveTAyear(int year, wxString& filename)
{

	FILE* fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<m_vTA.size(); i++) {
		if(m_vTA[i].fyear==year)
			fprintf(fp, "%d  %d  %d  %s  %.4f\n", 
				m_vTA[i].gvkey, m_vTA[i].datadate, m_vTA[i].fyear, m_vTA[i].tic.c_str(), m_vTA[i].at);
	}
	fclose(fp);	
	
	wxString msg;
	msg<<"output TA file  :"<< filename << "\n";
	ShowMessage(msg);	
}
void MainFrame::OnTASortAt(wxCommandEvent& event)
{
	vector<TA> vTA;

	for(int i=2008; i<=2013; i++) {
		wxString savename = m_strFolder;
		savename << "\\TA_" << i << "sort.csv";
	
		sortTAByat(i, vTA);	
	}

}
void MainFrame::sortTAByat(int year, vector<TA>& vTA)
{
	
	wxString filename = m_strFolder;
	filename << "\\TA_" << year << ".txt";
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
	wxString savename = m_strFolder;
	savename << "\\TA_" << year << "sort.csv";
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
void MainFrame::OnTACombineData(wxCommandEvent& event)
{
	char strTitle[200];
	char strline[200];
	string title;
	string filename = m_strFolder.ToStdString() + "\\data20160715.txt";
	
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		ShowMessage("Open "+filename+" error\n");	
		return;
	}

	fgets(strTitle, 200, fp);
	strTitle[strlen(strTitle)-1] = 0;
//	strTitle[strlen(strTitle)-2] = 0;
	title = string(strTitle) + "  big33  mid33  small33";
	
	m_vTAData.clear();
	int count = 0;
	int count0813 = 0;

	while (!feof(fp)) {
		fgets(strline, 200, fp);
		count++;
		TAData  oneData;
		if(oneData.readLine(strline)==true) {

			m_vTAData.push_back(oneData);
			count++;
		}
	} // read first record	
	
	fclose(fp);
	
	fp = fopen("_test.txt", "w");
	fprintf(fp, "%s\n", title.c_str());
	for(int i=0; i<m_vTAData.size(); i++) {
		fprintf(fp, "%5d  %6s  %8d %s", 
		m_vTAData[i].firmID, m_vTAData[i].firm_tic.c_str(), m_vTAData[i].year , m_vTAData[i].strLater.c_str());
	}
	fclose(fp);
}
