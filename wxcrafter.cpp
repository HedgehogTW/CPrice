//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"


// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;


MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer1);
    
    m_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    boxSizer1->Add(m_mainPanel, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    m_mainPanel->SetSizer(boxSizer17);
    
    m_staticText19 = new wxStaticText(m_mainPanel, wxID_ANY, _("Data folder"), wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1,-1)), 0);
    
    boxSizer17->Add(m_staticText19, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlFolder = new wxTextCtrl(m_mainPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(300,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlFolder->SetHint(wxT(""));
    #endif
    
    boxSizer17->Add(m_textCtrlFolder, 0, wxALL, WXC_FROM_DIP(5));
    
    m_buttonSelect = new wxButton(m_mainPanel, wxID_ANY, _("Select"), wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1,-1)), 0);
    
    boxSizer17->Add(m_buttonSelect, 0, wxALL, WXC_FROM_DIP(5));
    
    m_richTextMsg = new wxRichTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTE_MULTILINE|wxTE_PROCESS_TAB|wxTE_PROCESS_ENTER|wxWANTS_CHARS|wxBORDER_SIMPLE);
    
    boxSizer1->Add(m_richTextMsg, 4, wxALL|wxEXPAND, WXC_FROM_DIP(2));
    
    m_menuBar = new wxMenuBar(0);
    this->SetMenuBar(m_menuBar);
    
    m_name6 = new wxMenu();
    m_menuBar->Append(m_name6, _("File"));
    
    m_menuItem7 = new wxMenuItem(m_name6, wxID_EXIT, _("Exit\tAlt-X"), _("Quit"), wxITEM_NORMAL);
    m_name6->Append(m_menuItem7);
    
    m_menuPrice = new wxMenu();
    m_menuBar->Append(m_menuPrice, _("Price"));
    
    m_menuItemProcess = new wxMenuItem(m_menuPrice, wxID_ANY, _("Process"), wxT(""), wxITEM_NORMAL);
    m_menuPrice->Append(m_menuItemProcess);
    
    m_menuTA = new wxMenu();
    m_menuBar->Append(m_menuTA, _("TA"));
    
    m_menuItemTACheckTA = new wxMenuItem(m_menuTA, wxID_ANY, _("Check TA"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemTACheckTA);
    
    m_menuItemSort = new wxMenuItem(m_menuTA, wxID_ANY, _("Sort at"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemSort);
    
    m_menuItemCombineData = new wxMenuItem(m_menuTA, wxID_ANY, _("Combine Data"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemCombineData);
    
    m_menuTA->AppendSeparator();
    
    m_menuItemTA = new wxMenuItem(m_menuTA, wxID_ANY, _("Process TA"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemTA);
    
    m_menuItemEMP = new wxMenuItem(m_menuTA, wxID_ANY, _("Process EMP"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemEMP);
    
    m_menuItemLT = new wxMenuItem(m_menuTA, wxID_ANY, _("Process LT"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemLT);
    
    m_menuItemLCT = new wxMenuItem(m_menuTA, wxID_ANY, _("Process LCT"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemLCT);
    
    m_menuItemSALE = new wxMenuItem(m_menuTA, wxID_ANY, _("Process SALES"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemSALE);
    
    m_menuItemMKVALT = new wxMenuItem(m_menuTA, wxID_ANY, _("Process MKVALT"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemMKVALT);
    
    m_menuItemEPS = new wxMenuItem(m_menuTA, wxID_ANY, _("Process EPS"), wxT(""), wxITEM_NORMAL);
    m_menuTA->Append(m_menuItemEPS);
    
    m_name8 = new wxMenu();
    m_menuBar->Append(m_name8, _("Help"));
    
    m_menuItem9 = new wxMenuItem(m_name8, wxID_ABOUT, _("About..."), wxT(""), wxITEM_NORMAL);
    m_name8->Append(m_menuItem9);
    
    m_mainToolbar = this->CreateToolBar(wxTB_FLAT, wxID_ANY);
    m_mainToolbar->SetToolBitmapSize(wxSize(16,16));
    
    SetName(wxT("MainFrameBaseClass"));
    SetSize(500,300);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    m_buttonSelect->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSelectFolder), NULL, this);
    this->Connect(m_menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Connect(m_menuItemProcess->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnPriceProcess), NULL, this);
    this->Connect(m_menuItemTACheckTA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTACheckTA), NULL, this);
    this->Connect(m_menuItemSort->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTASortAt), NULL, this);
    this->Connect(m_menuItemCombineData->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTACombineData), NULL, this);
    this->Connect(m_menuItemTA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessTA), NULL, this);
    this->Connect(m_menuItemEMP->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessEMP), NULL, this);
    this->Connect(m_menuItemLT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessLT), NULL, this);
    this->Connect(m_menuItemLCT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessLCT), NULL, this);
    this->Connect(m_menuItemSALE->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessSALE), NULL, this);
    this->Connect(m_menuItemMKVALT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessMKVALT), NULL, this);
    this->Connect(m_menuItemEPS->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessEPS), NULL, this);
    this->Connect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    m_buttonSelect->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSelectFolder), NULL, this);
    this->Disconnect(m_menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_menuItemProcess->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnPriceProcess), NULL, this);
    this->Disconnect(m_menuItemTACheckTA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTACheckTA), NULL, this);
    this->Disconnect(m_menuItemSort->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTASortAt), NULL, this);
    this->Disconnect(m_menuItemCombineData->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTACombineData), NULL, this);
    this->Disconnect(m_menuItemTA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessTA), NULL, this);
    this->Disconnect(m_menuItemEMP->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessEMP), NULL, this);
    this->Disconnect(m_menuItemLT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessLT), NULL, this);
    this->Disconnect(m_menuItemLCT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessLCT), NULL, this);
    this->Disconnect(m_menuItemSALE->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessSALE), NULL, this);
    this->Disconnect(m_menuItemMKVALT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessMKVALT), NULL, this);
    this->Disconnect(m_menuItemEPS->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProcessEPS), NULL, this);
    this->Disconnect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    
}
