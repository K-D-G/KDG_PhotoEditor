#include "KDG_PhotoEditor.h"

//using namespace KDG_PhotoEditor;

IMPLEMENT_APP(PhotoEditor)

bool PhotoEditor::OnInit(){
	MainFrame* main_window=new MainFrame(_("KDG Photo Editor"), wxDefaultPosition, wxSize(600, 600));
	main_window->Show(true);
	SetTopWindow(main_window);
	return TRUE;
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_EXIT, MainFrame::quit)
EVT_MENU(wxID_ABOUT, MainFrame::about)
EVT_MENU(menu_new, MainFrame::new_file)
EVT_MENU(menu_open, MainFrame::open_file)
EVT_MENU(menu_close, MainFrame::close_file)
EVT_MENU(menu_save, MainFrame::save_file)
EVT_MENU(menu_save_as, MainFrame::save_file_as)
EVT_MENU(menu_quit, MainFrame::quit)
END_EVENT_TABLE()
MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size):wxFrame((wxFrame*)NULL, -1, title, pos, size){
	CreateStatusBar(2); //Arg says how many segments there is
	SetStatusText("Hello World");
	menu_bar=new wxMenuBar();

	wxMenu* menu_file=new wxMenu();
	menu_file->Append(menu_new, wxT("&New"), wxT("Create new file"));
	menu_file->Append(menu_open, wxT("&Open"), wxT("Open a file"));
	menu_file->Append(menu_close, wxT("&Close"), wxT("Close current file"));
	menu_file->Append(menu_save, wxT("&Save"), wxT("Save the file"));
	menu_file->Append(menu_save_as, wxT("Save &As"), wxT("Save the file under a new file name"));
	menu_file->Append(menu_quit, wxT("Quit"), wxT("Quit the program"));

	menu_bar->Append(menu_file, wxT("File"));
	SetMenuBar(menu_bar);

	Maximize(); //Maximise the window	
}

void MainFrame::quit(wxCommandEvent &event){
	Close(TRUE);
}

void MainFrame::about(wxCommandEvent &event){
	wxMessageBox("Hello World example", "About KDG Photo Editor", wxOK|wxICON_INFORMATION);
}

void MainFrame::new_file(wxCommandEvent &WXUNUSED(event)){
	current_file_path=wxT("C:/");
	SetTitle(_("Edit-untitled *"));
}
void MainFrame::open_file(wxCommandEvent &WXUNUSED(event)){
	wxFileDialog* open_dialog=new wxFileDialog(this, _("Choose file to open"), wxEmptyString, wxEmptyString, _("Text files (*.txt)|*.txt||C++ Source Files (*.cpp, *.cxx)|*.cpp;*.cxx|C Source files (*.c)|*.c|C header files (*.h)|*.h"), wxFD_OPEN|wxFD_FILE_MUST_EXIST/*, wxDefaultPosition*/);
	if(open_dialog->ShowModal()==wxID_OK){
		current_file_path=open_dialog->GetPath();
		//main_edit_box->LoadFile(current_file_path);
		SetTitle(wxString("Edit-")<<open_dialog->GetFilename());
	}
}
void MainFrame::save_file(wxCommandEvent &WXUNUSED(event)){
	//main_edit_box->SaveFile(wxT("KDG_PhotoEditor.h"));
}
void MainFrame::save_file_as(wxCommandEvent &WXUNUSED(event)){
	wxFileDialog* save_dialog=new wxFileDialog(this, _("Save File As _?"), wxEmptyString, wxEmptyString, _("Text files (*.txt)|*.txt|C++ Source Files (*.cpp)|*.cpp|C Source files (*.c)|*.c|C header files (*.h)|*.h"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
	if(save_dialog->ShowModal()==wxID_OK){
		current_file_path=save_dialog->GetPath();
		//main_edit_box->SaveFile(current_file_path);
		SetTitle(wxString("Edit-")<<save_dialog->GetFilename());
	}
	save_dialog->Destroy();
}
void MainFrame::close_file(wxCommandEvent &WXUNUSED(event)){
	//main_edit_box->Clear();
	current_file_path=wxT("C:/");
	SetTitle(_("Edit-untitled *"));
}