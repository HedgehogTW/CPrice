#include "Rating.h"
#include "MainFrame.h"

RatingData::RatingData()
{
	match = false;
}

RatingData::~RatingData()
{
}

bool RatingData::readLine(char strline[])
{
	bool bRet;
	char s1[20], s2[20], s3[20];
	int n =sscanf(strline, "%s%s%s", s1, s2, s3);
	if(n!= 3)
		bRet = false;
	else
		bRet = true;	
	
	if(strcmp(s3, "99999")==0) 
		bRet = false;
		
	if(bRet ==false)  return bRet;
	
	n = sscanf(s1, "%4d%2d", &year, &month);
	if(n!= 2)
		bRet = false;
	else
		bRet = true;
	if(bRet ==false)  return bRet;
	
	datadate = string(s1);
	tic = string(s2);
	splticrm = string(s3);
	
//	MainFrame::ShowMessage("%s %s %s %d %d\n", datadate, tic, splticrm, year, month);
	return bRet;
	
}

Rating::Rating(string& varName)
{
	m_varName = varName;
	m_strFolder = "d:\\tmp\\data\\";
}

Rating::~Rating()
{
}


void Rating::loadDataFile()
{
	char strTitle[200];
	char strline[200];
	string filename = m_strFolder.ToStdString() + "\\data20160715.txt";
	
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}

	fgets(strTitle, 200, fp);
	strTitle[strlen(strTitle)-1] = 0;
//	strTitle[strlen(strTitle)-2] = 0;
	m_titleData = string(strTitle);
	m_titleData.insert(33, "month"); 
	
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
	MainFrame::ShowMessage(msg);	
}
void Rating::loadVariableData()
{
	char strline[200];
	string filename = m_strFolder.ToStdString() + m_varName+".txt";
	
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}
	fgets(strline, 200, fp);
	
	vector<RatingData> vTA;
	vector<RatingData> vTA0813;
	int count0813 = 0;	
	int count = 0;
	bool hasFirst = false;	
	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;

		RatingData  oneData;
		if(oneData.readLine(strline)==false) continue;
		
		if(hasFirst == false && oneData.year ==2008 && oneData.month==11) {
			vTA0813.clear();
			vTA0813.push_back(oneData);
			count0813 = 1;
			hasFirst = true;
		}else if(hasFirst) {
			if(oneData.tic.compare(vTA0813[0].tic)==0) {
				vTA0813.push_back(oneData);
				count0813++;
			}else {
				if(count0813==62) {
					for(int i=0; i<vTA0813.size(); i++)
						vTA.push_back(vTA0813[i]);

				}
				vTA0813.clear();
				count0813 = 0;
				hasFirst = false;					
			}				
		}
		count++;
	}
	fclose(fp);

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
	
	filename = m_strFolder.ToStdString() + m_varName + "_clear.txt";
	fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<vTA.size(); i++) {
		if(vTA[i].match)
			fprintf(fp, "%s  %s  %s  %d %3d\n", 
				vTA[i].datadate.c_str(), vTA[i].tic.c_str(), vTA[i].splticrm.c_str(), vTA[i].year, vTA[i].month);
	}
	fclose(fp);
	
	for(int i=2008; i<=2013; i++) {
		wxString filename = m_strFolder+ m_varName;
		filename << "_" << i << ".txt";
		saveByYear(i, filename, vTA);
	
	}
	
	
	wxString msg;
	msg<<"output var file " << vTA.size() << " :"<< filename <<"\n";
	MainFrame::ShowMessage(msg);	

}

void Rating::saveByYear(int year, wxString& filename, vector<RatingData>& vTA)
{

	FILE* fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<vTA.size(); i++) {
		if(vTA[i].match)
			if(vTA[i].year==year)
				fprintf(fp, "%s  %s  %s  %d  %4d\n", 
					vTA[i].datadate.c_str(), vTA[i].tic.c_str(), vTA[i].splticrm.c_str(), vTA[i].year, vTA[i].month);
	}
	fclose(fp);	
	
	wxString msg;
	msg<<"output var file  :"<< filename << "\n";
	MainFrame::ShowMessage(msg);	
}

void Rating::sortVarData()
{
	vector<RatingData> vTA;
	m_vTAYear.clear();
	
	for(int i=2008; i<=2013; i++) {
		wxString savename = m_strFolder + m_varName;
		savename << "_" << i << "sort.csv";
	
		sortRatio(i, vTA);	
		m_vTAYear.push_back(vTA);
	}	
}
void Rating::sortRatio(int year, vector<RatingData>& vTA)
{
	
	wxString filename = m_strFolder+ m_varName;
	filename << "_" << year << ".txt";
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}
	char strline[200];
	vTA.clear();
	int count = 0;

	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;

		RatingData  oneData;
		if(oneData.readLine(strline)==true) {
			vTA.push_back(oneData);
			count++;
		}
	} // read first record	
	fclose(fp);	
	wxString msg;
	msg << "Read " << filename << " " << count << " records, size " << vTA.size() << "\n";
	MainFrame::ShowMessage(msg);
/*	
    sort(vTA.begin(), vTA.end());

////////////////////////// save
	wxString savename = m_strFolder + m_varName;
	savename << "_" << year << "sort.csv";
	fp = fopen(savename.c_str(), "w");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+savename+" error\n");	
		return;
	}
	for(int i=0; i<vTA.size(); i++) {
		fprintf(fp, "%d,  %s,  %.3f,  %.3f,  %.3f, %d\n", 
				vTA[i].fyear, vTA[i].tic.c_str(), vTA[i].ib, vTA[i].sales, vTA[i].ratio, vTA[i].DIVD);
	}
	fclose(fp);	
	 **/ 
}


void Rating::outputCombineData()
{
	
	// combine TA ..............
	string oldTic;
	for(int i=0; i<m_vTAMainData.size(); i++) {
		int idx = getIdx(m_vTAMainData[i].year, m_vTAMainData[i].month, m_vTAMainData[i].firm_tic);
		if(idx ==-1) {
			/*
			if(m_vTAMainData[i].firm_tic.compare(oldTic)!=0) {
				wxString msg;
				msg << "Var Data ERR: " << i+1 << ", year " << m_vTAMainData[i].year << ", firm_tic:" << m_vTAMainData[i].firm_tic 
					<< ", month:" << m_vTAMainData[i].month << "\n";
				MainFrame::ShowMessage(msg);
				oldTic = m_vTAMainData[i].firm_tic;
			}*/
			m_vTAMainData[i].matched = 0;
		}else 
			m_vTAMainData[i].dd = idx;
		
	}
	
	// output file
	wxString savename = m_strFolder + m_varName;
	savename << "_result.txt";	
	FILE* fp = fopen(savename.c_str(), "w");
	fprintf(fp, "%s  DD\n", m_titleData.c_str());
	int match = 0;	
	for(int i=0; i<m_vTAMainData.size(); i++) {
		if(m_vTAMainData[i].matched ==0) continue;
		fprintf(fp, "%6d   %10s   %8d  %4d  %s  %3d\n", 
		m_vTAMainData[i].firmID, m_vTAMainData[i].firm_tic.c_str(), m_vTAMainData[i].year, m_vTAMainData[i].month, m_vTAMainData[i].strLater.c_str(),
		m_vTAMainData[i].dd);
		match ++;
	}
	fclose(fp);
	
	wxString msg;
	msg << "Read Data size " << m_vTAMainData.size() << ", match: " << match << "\n";
	MainFrame::ShowMessage(msg);		
}


int Rating::getIdx(int year, int month, string ftic)
{
	vector<RatingData>& vTA = m_vTAYear[year - 2008];
	
	int ret = 0;
	int idx = -1; 
	int sz = vTA.size();
	for (int i=0; i<sz; i++){
		if(vTA[i].tic.compare(ftic)==0 && month==vTA[i].month) {
			idx = i;
			break;
		}
	}
	int dd = -1;
	
	if(idx >= 0) {
		std::size_t found = vTA[idx].splticrm.find('A');
		if (found == std::string::npos)
			dd = 0;
		else 
			dd = 1;
	}

	return dd;
	
}