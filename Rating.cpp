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
	vector<RatingData> vTAClear;
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
	MainFrame::ShowMessage("read %d/%d records\n", vTA.size(), count);

	// clear data
	for(int i=0; i<vTA.size(); i++) {
		if(vTA[i].splticrm.compare("99999")!=0)
			vTAClear.push_back(vTA[i]);
	}
	MainFrame::ShowMessage("read %d/%d records, clear %d\n", vTA.size(), count, vTAClear.size());	
	/*
	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;
		
		count++;
		RatingData  oneData;
		if(oneData.readLine(strline)==true) {
			if(oneData.tic.compare(vTA0813[0].tic)==0) {
				if( oneData.year == vTA0813[count0813-1].year+1) {
					vTA0813.push_back(oneData);
					count0813++;
				}
			}else {
				if(count0813==6) {
					for(int i=0; i<6; i++)
						vTA.push_back(vTA0813[i]);
					count0813 = 1;
				}
				if(oneData.year ==2008) {
					vTA0813.clear();
					vTA0813.push_back(oneData);
					count0813 = 1;
				}				
			}
		}
	}
*/
			

/*	
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
	*/
	filename = m_strFolder.ToStdString() + m_varName + "_clear.txt";
	fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<vTAClear.size(); i++) {
//		if(vTA[i].match)
			fprintf(fp, "%s  %s  %s  %d %3d\n", 
				vTAClear[i].datadate.c_str(), vTAClear[i].tic.c_str(), vTAClear[i].splticrm.c_str(), vTAClear[i].year, vTAClear[i].month);
	}
	fclose(fp);
/*	
	for(int i=2008; i<=2013; i++) {
		wxString filename = m_strFolder+ m_varName;
		filename << "_" << i << ".txt";
		saveByYear(i, filename, vTA);
	
	}
	
	
	wxString msg;
	msg<<"output var file " << vTA.size() << " :"<< filename <<"\n";
	MainFrame::ShowMessage(msg);	
*/	
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
				fprintf(fp, "%s  %s  %sf  %d  %4d\n", 
					vTA[i].datadate.c_str(), vTA[i].tic.c_str(), vTA[i].splticrm.c_str(), vTA[i].year, vTA[i].month);
	}
	fclose(fp);	
	
	wxString msg;
	msg<<"output var file  :"<< filename << "\n";
	MainFrame::ShowMessage(msg);	
}


void Rating::outputCombineData()
{
	
}