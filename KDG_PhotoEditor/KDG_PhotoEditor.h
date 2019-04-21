#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "image.h"

enum{
	text_main=wxID_HIGHEST+1,
	menu_new,
	menu_open,
	menu_close,
	menu_save,
	menu_save_as,
	menu_quit
};

class PhotoEditor:public wxApp{
public:
	virtual bool OnInit();
};

class MainFrame:public wxFrame{
public:
	wxMenuBar* menu_bar;

	wxString current_file_path;

	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

	void quit(wxCommandEvent &event);
	void about(wxCommandEvent &event);

	void new_file(wxCommandEvent &event);
	void open_file(wxCommandEvent &event);
	void save_file(wxCommandEvent &event);
	void save_file_as(wxCommandEvent &event);
	void close_file(wxCommandEvent &event);

	DECLARE_EVENT_TABLE()
};


DECLARE_APP(PhotoEditor)