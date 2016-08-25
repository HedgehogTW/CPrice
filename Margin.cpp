#include "Margin.h"
#include "MainFrame.h"

MarginData::MarginData()
{
}

MarginData::~MarginData()
{
}

bool MarginData::readLine(char strline[])
{
	
	char stic[20];
	int n =sscanf(strline, "%d%s%f%f", &fyear, stic, &ib, &sales);
	if(sales ==0)  return false;
	tic = string(stic);
	ratio = ib/sales;
	//MainFrame::ShowMessage("%d %d %d %s %f\n", gvkey, datadate, fyear, stic, at);
	if(n!= 4)
		return false;
	else
		return true;
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

Margin::Margin(string& varName)
{
	m_varName = varName;
	m_strFolder = "d:\\tmp\\data\\";
}

Margin::~Margin()
{
}

void Margin::loadDataFile()
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
	MainFrame::ShowMessage(msg);	
}
void Margin::loadVariableData()
{
	char strline[200];
	string filename = m_strFolder.ToStdString() + m_varName+".txt";
	
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}
	fgets(strline, 200, fp);
	
	vector<MarginData> vTA;
	int count = 0;
	int count0813 = 0;
	vector<MarginData> vTA0813;
	while (!feof(fp)) {
		char* p = fgets(strline, 200, fp);
		if(p==NULL) break;
		count++;
		MarginData  oneData;
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
		MarginData  oneData;
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
	MainFrame::ShowMessage("read %d/%d records\n", vTA.size(), count);
	
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
			fprintf(fp, "%d  %s  %.3f  %.3f  %.3f\n", 
				vTA[i].fyear, vTA[i].tic.c_str(), vTA[i].ib, vTA[i].sales, vTA[i].ratio);
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

void Margin::saveByYear(int year, wxString& filename, vector<MarginData>& vTA)
{

	FILE* fp = fopen(filename.c_str(), "w");
	if(fp ==NULL) {
		MainFrame::ShowMessage("Open "+filename+" error\n");	
		return;
	}
	for(int i=0; i<vTA.size(); i++) {
		if(vTA[i].match)
			if(vTA[i].fyear==year)
			fprintf(fp, "%d  %s  %.3f  %.3f  %.3f\n", 
				vTA[i].fyear, vTA[i].tic.c_str(), vTA[i].ib, vTA[i].sales, vTA[i].ratio);
	}
	fclose(fp);	
	
	wxString msg;
	msg<<"output var file  :"<< filename << "\n";
	MainFrame::ShowMessage(msg);	
}

void Margin::sortVarData()
{
	vector<MarginData> vTA;
	m_vTAYear.clear();
	
	for(int i=2008; i<=2013; i++) {
		wxString savename = m_strFolder + m_varName;
		savename << "_" << i << "sort.csv";
	
		sortRatio(i, vTA);	
		m_vTAYear.push_back(vTA);
	}	
}
void Margin::sortRatio(int year, vector<MarginData>& vTA)
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

		MarginData  oneData;
		if(oneData.readLine(strline)==true) {
			vTA.push_back(oneData);
			count++;
		}
	} // read first record	
	fclose(fp);	
	wxString msg;
	msg << "Read " << filename << " " << count << " records, size " << vTA.size() << "\n";
	
	MainFrame::ShowMessage(msg);
	
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
		fprintf(fp, "%d,  %s,  %.3f,  %.3f,  %.3f\n", 
				vTA[i].fyear, vTA[i].tic.c_str(), vTA[i].ib, vTA[i].sales, vTA[i].ratio);
	}
	fclose(fp);	
}

void Margin::outputCombineData()
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
				MainFrame::ShowMessage(msg);
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
	wxString savename = m_strFolder + m_varName;
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
	MainFrame::ShowMessage(msg);	
	
//	saveSeparatedYearData(varName);

}

int Margin::getIdx(vector<MarginData> &vTA, string ftic)
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