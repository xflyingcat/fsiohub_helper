/***************************************************************
 * Name:      loggerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2019-02-12
 * Copyright:  ()
 * License:
 **************************************************************/

#include "loggerMain.h"
#include <wx/msgdlg.h>
#include "Serial.h"
#include "serial_enum.h"
#include "logger.h"
#include "wx/config.h"
#include "console_utils.h"

wxString float_to_string(double val);


#define __CONFIG_NAME__  "FSIOHubConfigHelper"

#define clYellow     wxColour(255,255,0)
#define clAqua       wxColour(0,255,255)
#define clWhite      wxColour(255,255,255)
#define clLime       wxColour(0,255,0)
#define clRed        wxColour(255,0,0)
#define clBlue        wxColour(0,0,255)
#define clGray        wxColour(176,176,176)
#define clDarkGreen   wxColour(0,176,0)
#define clDarkYellow  wxColour(128,128,0)
#define clBlack       wxColour(0,0,0)

loggerDialog *logger_dialog;
int current_panel = 0;

char display_array[48*8+5];
char led_array[16*8*8+5];

int timer_enabled = 0;

wxTextCtrl *axes[16];

wxCheckBox *mask32[32];

int rx_call_back(char *rxbuf, int rxlen);
int udp_call_back(char *rxbuf, int rxlen);

//(*InternalHeaders(loggerDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat
{
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(loggerDialog)
const long loggerDialog::ID_STATICTEXT6 = wxNewId();
const long loggerDialog::ID_TEXTCTRL2 = wxNewId();
const long loggerDialog::ID_STATICTEXT12 = wxNewId();
const long loggerDialog::ID_TEXTCTRL7 = wxNewId();
const long loggerDialog::ID_STATICLINE5 = wxNewId();
const long loggerDialog::ID_STATICTEXT15 = wxNewId();
const long loggerDialog::ID_TEXTCTRL25 = wxNewId();
const long loggerDialog::ID_STATICTEXT68 = wxNewId();
const long loggerDialog::ID_TEXTCTRL10 = wxNewId();
const long loggerDialog::ID_STATICLINE2 = wxNewId();
const long loggerDialog::ID_CHOICE2 = wxNewId();
const long loggerDialog::ID_STATICTEXT5 = wxNewId();
const long loggerDialog::ID_TEXTCTRL1 = wxNewId();
const long loggerDialog::ID_STATICTEXT27 = wxNewId();
const long loggerDialog::ID_BUTTON31 = wxNewId();
const long loggerDialog::ID_CHOICE1 = wxNewId();
const long loggerDialog::ID_STATICTEXT3 = wxNewId();
const long loggerDialog::ID_CHOICE8 = wxNewId();
const long loggerDialog::ID_STATICTEXT4 = wxNewId();
const long loggerDialog::ID_CHOICE11 = wxNewId();
const long loggerDialog::ID_STATICTEXT110 = wxNewId();
const long loggerDialog::ID_STATICLINE18 = wxNewId();
const long loggerDialog::ID_STATICTEXT113 = wxNewId();
const long loggerDialog::ID_STATICLINE13 = wxNewId();
const long loggerDialog::ID_STATICTEXT14 = wxNewId();
const long loggerDialog::ID_TEXTCTRL9 = wxNewId();
const long loggerDialog::ID_STATICTEXT13 = wxNewId();
const long loggerDialog::ID_TEXTCTRL8 = wxNewId();
const long loggerDialog::ID_STATICTEXT11 = wxNewId();
const long loggerDialog::ID_TEXTCTRL6 = wxNewId();
const long loggerDialog::ID_STATICTEXT8 = wxNewId();
const long loggerDialog::ID_TEXTCTRL3 = wxNewId();
const long loggerDialog::ID_STATICTEXT9 = wxNewId();
const long loggerDialog::ID_TEXTCTRL4 = wxNewId();
const long loggerDialog::ID_STATICTEXT10 = wxNewId();
const long loggerDialog::ID_TEXTCTRL5 = wxNewId();
const long loggerDialog::ID_STATICLINE19 = wxNewId();
const long loggerDialog::ID_RADIOBOX1 = wxNewId();
const long loggerDialog::ID_BUTTON5 = wxNewId();
const long loggerDialog::ID_STATICLINE17 = wxNewId();
const long loggerDialog::ID_STATICTEXT109 = wxNewId();
const long loggerDialog::ID_BUTTON26 = wxNewId();
const long loggerDialog::ID_BUTTON27 = wxNewId();
const long loggerDialog::ID_BUTTON28 = wxNewId();
const long loggerDialog::ID_STATICTEXT19 = wxNewId();
const long loggerDialog::ID_CHECKBOX5 = wxNewId();
const long loggerDialog::ID_STATICTEXT78 = wxNewId();
const long loggerDialog::ID_CHECKBOX6 = wxNewId();
const long loggerDialog::ID_STATICTEXT79 = wxNewId();
const long loggerDialog::ID_CHECKBOX7 = wxNewId();
const long loggerDialog::ID_STATICTEXT80 = wxNewId();
const long loggerDialog::ID_CHECKBOX8 = wxNewId();
const long loggerDialog::ID_STATICTEXT81 = wxNewId();
const long loggerDialog::ID_CHECKBOX9 = wxNewId();
const long loggerDialog::ID_STATICTEXT82 = wxNewId();
const long loggerDialog::ID_CHECKBOX10 = wxNewId();
const long loggerDialog::ID_STATICTEXT83 = wxNewId();
const long loggerDialog::ID_CHECKBOX11 = wxNewId();
const long loggerDialog::ID_STATICTEXT84 = wxNewId();
const long loggerDialog::ID_CHECKBOX12 = wxNewId();
const long loggerDialog::ID_STATICTEXT85 = wxNewId();
const long loggerDialog::ID_CHECKBOX13 = wxNewId();
const long loggerDialog::ID_STATICTEXT86 = wxNewId();
const long loggerDialog::ID_CHECKBOX14 = wxNewId();
const long loggerDialog::ID_STATICTEXT87 = wxNewId();
const long loggerDialog::ID_CHECKBOX15 = wxNewId();
const long loggerDialog::ID_STATICTEXT88 = wxNewId();
const long loggerDialog::ID_CHECKBOX16 = wxNewId();
const long loggerDialog::ID_STATICTEXT89 = wxNewId();
const long loggerDialog::ID_CHECKBOX17 = wxNewId();
const long loggerDialog::ID_STATICTEXT90 = wxNewId();
const long loggerDialog::ID_CHECKBOX18 = wxNewId();
const long loggerDialog::ID_STATICTEXT91 = wxNewId();
const long loggerDialog::ID_CHECKBOX19 = wxNewId();
const long loggerDialog::ID_STATICTEXT92 = wxNewId();
const long loggerDialog::ID_CHECKBOX20 = wxNewId();
const long loggerDialog::ID_STATICTEXT93 = wxNewId();
const long loggerDialog::ID_CHECKBOX21 = wxNewId();
const long loggerDialog::ID_STATICTEXT94 = wxNewId();
const long loggerDialog::ID_CHECKBOX22 = wxNewId();
const long loggerDialog::ID_STATICTEXT95 = wxNewId();
const long loggerDialog::ID_CHECKBOX23 = wxNewId();
const long loggerDialog::ID_STATICTEXT96 = wxNewId();
const long loggerDialog::ID_CHECKBOX24 = wxNewId();
const long loggerDialog::ID_STATICTEXT97 = wxNewId();
const long loggerDialog::ID_CHECKBOX25 = wxNewId();
const long loggerDialog::ID_STATICTEXT98 = wxNewId();
const long loggerDialog::ID_CHECKBOX26 = wxNewId();
const long loggerDialog::ID_STATICTEXT99 = wxNewId();
const long loggerDialog::ID_CHECKBOX27 = wxNewId();
const long loggerDialog::ID_STATICTEXT100 = wxNewId();
const long loggerDialog::ID_CHECKBOX28 = wxNewId();
const long loggerDialog::ID_STATICTEXT101 = wxNewId();
const long loggerDialog::ID_CHECKBOX29 = wxNewId();
const long loggerDialog::ID_STATICTEXT102 = wxNewId();
const long loggerDialog::ID_CHECKBOX30 = wxNewId();
const long loggerDialog::ID_STATICTEXT103 = wxNewId();
const long loggerDialog::ID_CHECKBOX31 = wxNewId();
const long loggerDialog::ID_STATICTEXT104 = wxNewId();
const long loggerDialog::ID_CHECKBOX32 = wxNewId();
const long loggerDialog::ID_STATICTEXT105 = wxNewId();
const long loggerDialog::ID_CHECKBOX33 = wxNewId();
const long loggerDialog::ID_STATICTEXT106 = wxNewId();
const long loggerDialog::ID_CHECKBOX34 = wxNewId();
const long loggerDialog::ID_STATICTEXT107 = wxNewId();
const long loggerDialog::ID_CHECKBOX35 = wxNewId();
const long loggerDialog::ID_STATICTEXT108 = wxNewId();
const long loggerDialog::ID_CHECKBOX36 = wxNewId();
const long loggerDialog::ID_BUTTON29 = wxNewId();
const long loggerDialog::ID_BUTTON30 = wxNewId();
const long loggerDialog::ID_PANEL1 = wxNewId();
const long loggerDialog::ID_STATICTEXT61 = wxNewId();
const long loggerDialog::ID_TEXTCTRL59 = wxNewId();
const long loggerDialog::ID_STATICTEXT64 = wxNewId();
const long loggerDialog::ID_TEXTCTRL60 = wxNewId();
const long loggerDialog::ID_STATICTEXT62 = wxNewId();
const long loggerDialog::ID_TEXTCTRL61 = wxNewId();
const long loggerDialog::ID_STATICTEXT63 = wxNewId();
const long loggerDialog::ID_TEXTCTRL62 = wxNewId();
const long loggerDialog::ID_STATICLINE3 = wxNewId();
const long loggerDialog::ID_TEXTCTRL17 = wxNewId();
const long loggerDialog::ID_STATICTEXT71 = wxNewId();
const long loggerDialog::ID_BUTTON32 = wxNewId();
const long loggerDialog::ID_TEXTCTRL67 = wxNewId();
const long loggerDialog::ID_STATICTEXT114 = wxNewId();
const long loggerDialog::ID_CHOICE9 = wxNewId();
const long loggerDialog::ID_STATICTEXT72 = wxNewId();
const long loggerDialog::ID_CHOICE7 = wxNewId();
const long loggerDialog::ID_STATICTEXT31 = wxNewId();
const long loggerDialog::ID_CHOICE12 = wxNewId();
const long loggerDialog::ID_STATICTEXT111 = wxNewId();
const long loggerDialog::ID_STATICLINE4 = wxNewId();
const long loggerDialog::ID_TEXTCTRL49 = wxNewId();
const long loggerDialog::ID_STATICTEXT75 = wxNewId();
const long loggerDialog::ID_TEXTCTRL66 = wxNewId();
const long loggerDialog::ID_STATICTEXT76 = wxNewId();
const long loggerDialog::ID_BUTTON21 = wxNewId();
const long loggerDialog::ID_STATICLINE11 = wxNewId();
const long loggerDialog::ID_BUTTON18 = wxNewId();
const long loggerDialog::ID_TEXTCTRL50 = wxNewId();
const long loggerDialog::ID_STATICTEXT57 = wxNewId();
const long loggerDialog::ID_TEXTCTRL51 = wxNewId();
const long loggerDialog::ID_BUTTON19 = wxNewId();
const long loggerDialog::ID_TEXTCTRL56 = wxNewId();
const long loggerDialog::ID_STATICTEXT58 = wxNewId();
const long loggerDialog::ID_TEXTCTRL57 = wxNewId();
const long loggerDialog::ID_BUTTON20 = wxNewId();
const long loggerDialog::ID_TEXTCTRL58 = wxNewId();
const long loggerDialog::ID_STATICTEXT60 = wxNewId();
const long loggerDialog::ID_CHECKBOX4 = wxNewId();
const long loggerDialog::ID_BUTTON25 = wxNewId();
const long loggerDialog::ID_PANEL6 = wxNewId();
const long loggerDialog::ID_CHOICE4 = wxNewId();
const long loggerDialog::ID_STATICTEXT18 = wxNewId();
const long loggerDialog::ID_TEXTCTRL11 = wxNewId();
const long loggerDialog::ID_STATICTEXT16 = wxNewId();
const long loggerDialog::ID_CHOICE3 = wxNewId();
const long loggerDialog::ID_STATICTEXT17 = wxNewId();
const long loggerDialog::ID_CHOICE10 = wxNewId();
const long loggerDialog::ID_STATICTEXT34 = wxNewId();
const long loggerDialog::ID_CHOICE13 = wxNewId();
const long loggerDialog::ID_STATICTEXT112 = wxNewId();
const long loggerDialog::ID_STATICLINE14 = wxNewId();
const long loggerDialog::ID_STATICTEXT20 = wxNewId();
const long loggerDialog::ID_TEXTCTRL13 = wxNewId();
const long loggerDialog::ID_BUTTON6 = wxNewId();
const long loggerDialog::ID_STATICTEXT21 = wxNewId();
const long loggerDialog::ID_TEXTCTRL14 = wxNewId();
const long loggerDialog::ID_STATICTEXT22 = wxNewId();
const long loggerDialog::ID_TEXTCTRL15 = wxNewId();
const long loggerDialog::ID_STATICTEXT23 = wxNewId();
const long loggerDialog::ID_TEXTCTRL16 = wxNewId();
const long loggerDialog::ID_STATICTEXT25 = wxNewId();
const long loggerDialog::ID_TEXTCTRL18 = wxNewId();
const long loggerDialog::ID_STATICTEXT26 = wxNewId();
const long loggerDialog::ID_TEXTCTRL19 = wxNewId();
const long loggerDialog::ID_STATICTEXT28 = wxNewId();
const long loggerDialog::ID_TEXTCTRL20 = wxNewId();
const long loggerDialog::ID_CHECKBOX1 = wxNewId();
const long loggerDialog::ID_STATICLINE15 = wxNewId();
const long loggerDialog::ID_STATICTEXT73 = wxNewId();
const long loggerDialog::ID_TEXTCTRL21 = wxNewId();
const long loggerDialog::ID_STATICTEXT29 = wxNewId();
const long loggerDialog::ID_TEXTCTRL46 = wxNewId();
const long loggerDialog::ID_STATICTEXT74 = wxNewId();
const long loggerDialog::ID_TEXTCTRL65 = wxNewId();
const long loggerDialog::ID_STATICTEXT56 = wxNewId();
const long loggerDialog::ID_TEXTCTRL64 = wxNewId();
const long loggerDialog::ID_BUTTON24 = wxNewId();
const long loggerDialog::ID_STATICLINE16 = wxNewId();
const long loggerDialog::ID_STATICTEXT7 = wxNewId();
const long loggerDialog::ID_TEXTCTRL23 = wxNewId();
const long loggerDialog::ID_BUTTON15 = wxNewId();
const long loggerDialog::ID_TEXTCTRL24 = wxNewId();
const long loggerDialog::ID_BUTTON16 = wxNewId();
const long loggerDialog::ID_TEXTCTRL45 = wxNewId();
const long loggerDialog::ID_BUTTON17 = wxNewId();
const long loggerDialog::ID_STATICTEXT77 = wxNewId();
const long loggerDialog::ID_TEXTCTRL12 = wxNewId();
const long loggerDialog::ID_STATICLINE12 = wxNewId();
const long loggerDialog::ID_STATICTEXT115 = wxNewId();
const long loggerDialog::ID_COMBOBOX4 = wxNewId();
const long loggerDialog::ID_BUTTON33 = wxNewId();
const long loggerDialog::ID_STATICTEXT69 = wxNewId();
const long loggerDialog::ID_COMBOBOX3 = wxNewId();
const long loggerDialog::ID_PANEL2 = wxNewId();
const long loggerDialog::ID_GRID1 = wxNewId();
const long loggerDialog::ID_BUTTON7 = wxNewId();
const long loggerDialog::ID_PANEL4 = wxNewId();
const long loggerDialog::ID_GRID2 = wxNewId();
const long loggerDialog::ID_BUTTON11 = wxNewId();
const long loggerDialog::ID_PANEL5 = wxNewId();
const long loggerDialog::ID_STATICTEXT35 = wxNewId();
const long loggerDialog::ID_TEXTCTRL30 = wxNewId();
const long loggerDialog::ID_STATICTEXT33 = wxNewId();
const long loggerDialog::ID_TEXTCTRL26 = wxNewId();
const long loggerDialog::ID_STATICTEXT36 = wxNewId();
const long loggerDialog::ID_TEXTCTRL27 = wxNewId();
const long loggerDialog::ID_STATICTEXT37 = wxNewId();
const long loggerDialog::ID_TEXTCTRL28 = wxNewId();
const long loggerDialog::ID_STATICTEXT38 = wxNewId();
const long loggerDialog::ID_TEXTCTRL29 = wxNewId();
const long loggerDialog::ID_STATICTEXT39 = wxNewId();
const long loggerDialog::ID_TEXTCTRL31 = wxNewId();
const long loggerDialog::ID_STATICTEXT40 = wxNewId();
const long loggerDialog::ID_TEXTCTRL32 = wxNewId();
const long loggerDialog::ID_STATICTEXT41 = wxNewId();
const long loggerDialog::ID_TEXTCTRL33 = wxNewId();
const long loggerDialog::ID_STATICTEXT42 = wxNewId();
const long loggerDialog::ID_TEXTCTRL34 = wxNewId();
const long loggerDialog::ID_STATICTEXT43 = wxNewId();
const long loggerDialog::ID_TEXTCTRL35 = wxNewId();
const long loggerDialog::ID_STATICTEXT44 = wxNewId();
const long loggerDialog::ID_TEXTCTRL36 = wxNewId();
const long loggerDialog::ID_STATICTEXT45 = wxNewId();
const long loggerDialog::ID_TEXTCTRL37 = wxNewId();
const long loggerDialog::ID_STATICTEXT46 = wxNewId();
const long loggerDialog::ID_TEXTCTRL38 = wxNewId();
const long loggerDialog::ID_STATICTEXT47 = wxNewId();
const long loggerDialog::ID_TEXTCTRL39 = wxNewId();
const long loggerDialog::ID_STATICTEXT48 = wxNewId();
const long loggerDialog::ID_TEXTCTRL40 = wxNewId();
const long loggerDialog::ID_STATICTEXT49 = wxNewId();
const long loggerDialog::ID_TEXTCTRL41 = wxNewId();
const long loggerDialog::ID_CHOICE6 = wxNewId();
const long loggerDialog::ID_STATICTEXT52 = wxNewId();
const long loggerDialog::ID_CHOICE5 = wxNewId();
const long loggerDialog::ID_STATICTEXT50 = wxNewId();
const long loggerDialog::ID_STATICTEXT53 = wxNewId();
const long loggerDialog::ID_TEXTCTRL42 = wxNewId();
const long loggerDialog::ID_STATICTEXT54 = wxNewId();
const long loggerDialog::ID_TEXTCTRL43 = wxNewId();
const long loggerDialog::ID_STATICTEXT59 = wxNewId();
const long loggerDialog::ID_TEXTCTRL48 = wxNewId();
const long loggerDialog::ID_BUTTON12 = wxNewId();
const long loggerDialog::ID_TEXTCTRL44 = wxNewId();
const long loggerDialog::ID_STATICTEXT65 = wxNewId();
const long loggerDialog::ID_TEXTCTRL54 = wxNewId();
const long loggerDialog::ID_BUTTON13 = wxNewId();
const long loggerDialog::ID_TEXTCTRL52 = wxNewId();
const long loggerDialog::ID_STATICTEXT66 = wxNewId();
const long loggerDialog::ID_TEXTCTRL55 = wxNewId();
const long loggerDialog::ID_BUTTON14 = wxNewId();
const long loggerDialog::ID_TEXTCTRL53 = wxNewId();
const long loggerDialog::ID_STATICTEXT67 = wxNewId();
const long loggerDialog::ID_CHECKBOX3 = wxNewId();
const long loggerDialog::ID_BUTTON10 = wxNewId();
const long loggerDialog::ID_PANEL3 = wxNewId();
const long loggerDialog::ID_NOTEBOOK1 = wxNewId();
const long loggerDialog::ID_STATICLINE1 = wxNewId();
const long loggerDialog::ID_STATICTEXT1 = wxNewId();
const long loggerDialog::ID_TEXTCTRL22 = wxNewId();
const long loggerDialog::ID_STATICTEXT2 = wxNewId();
const long loggerDialog::ID_STATICLINE7 = wxNewId();
const long loggerDialog::ID_STATICTEXT24 = wxNewId();
const long loggerDialog::ID_BUTTON4 = wxNewId();
const long loggerDialog::ID_COMBOBOX1 = wxNewId();
const long loggerDialog::ID_STATICTEXT51 = wxNewId();
const long loggerDialog::ID_COMBOBOX2 = wxNewId();
const long loggerDialog::ID_BUTTON1 = wxNewId();
const long loggerDialog::ID_BUTTON8 = wxNewId();
const long loggerDialog::ID_BUTTON2 = wxNewId();
const long loggerDialog::ID_STATICLINE8 = wxNewId();
const long loggerDialog::ID_STATICTEXT55 = wxNewId();
const long loggerDialog::ID_STATICTEXT32 = wxNewId();
const long loggerDialog::ID_TEXTCTRL47 = wxNewId();
const long loggerDialog::ID_STATICTEXT70 = wxNewId();
const long loggerDialog::ID_TEXTCTRL63 = wxNewId();
const long loggerDialog::ID_BUTTON22 = wxNewId();
const long loggerDialog::ID_BUTTON23 = wxNewId();
const long loggerDialog::ID_STATICLINE9 = wxNewId();
const long loggerDialog::ID_STATICLINE10 = wxNewId();
const long loggerDialog::ID_STATICTEXT30 = wxNewId();
const long loggerDialog::ID_BUTTON9 = wxNewId();
const long loggerDialog::ID_CHECKBOX2 = wxNewId();
const long loggerDialog::ID_STATICLINE6 = wxNewId();
const long loggerDialog::ID_BUTTON3 = wxNewId();
const long loggerDialog::ID_TIMER1 = wxNewId();
const long loggerDialog::ID_TIMER2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(loggerDialog,wxDialog)
    //(*EventTable(loggerDialog)
    //*)
END_EVENT_TABLE()

loggerDialog::loggerDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(loggerDialog)
    wxBoxSizer* BoxSizer10;
    wxBoxSizer* BoxSizer11;
    wxBoxSizer* BoxSizer12;
    wxBoxSizer* BoxSizer13;
    wxBoxSizer* BoxSizer14;
    wxBoxSizer* BoxSizer15;
    wxBoxSizer* BoxSizer16;
    wxBoxSizer* BoxSizer17;
    wxBoxSizer* BoxSizer18;
    wxBoxSizer* BoxSizer19;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer20;
    wxBoxSizer* BoxSizer21;
    wxBoxSizer* BoxSizer22;
    wxBoxSizer* BoxSizer23;
    wxBoxSizer* BoxSizer24;
    wxBoxSizer* BoxSizer25;
    wxBoxSizer* BoxSizer26;
    wxBoxSizer* BoxSizer27;
    wxBoxSizer* BoxSizer28;
    wxBoxSizer* BoxSizer29;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer30;
    wxBoxSizer* BoxSizer31;
    wxBoxSizer* BoxSizer32;
    wxBoxSizer* BoxSizer33;
    wxBoxSizer* BoxSizer34;
    wxBoxSizer* BoxSizer35;
    wxBoxSizer* BoxSizer36;
    wxBoxSizer* BoxSizer37;
    wxBoxSizer* BoxSizer38;
    wxBoxSizer* BoxSizer39;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer40;
    wxBoxSizer* BoxSizer41;
    wxBoxSizer* BoxSizer42;
    wxBoxSizer* BoxSizer43;
    wxBoxSizer* BoxSizer44;
    wxBoxSizer* BoxSizer45;
    wxBoxSizer* BoxSizer46;
    wxBoxSizer* BoxSizer47;
    wxBoxSizer* BoxSizer48;
    wxBoxSizer* BoxSizer49;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer9;
    wxFlexGridSizer* FlexGridSizer10;
    wxFlexGridSizer* FlexGridSizer11;
    wxFlexGridSizer* FlexGridSizer12;
    wxFlexGridSizer* FlexGridSizer13;
    wxFlexGridSizer* FlexGridSizer14;
    wxFlexGridSizer* FlexGridSizer15;
    wxFlexGridSizer* FlexGridSizer16;
    wxFlexGridSizer* FlexGridSizer17;
    wxFlexGridSizer* FlexGridSizer18;
    wxFlexGridSizer* FlexGridSizer19;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer20;
    wxFlexGridSizer* FlexGridSizer21;
    wxFlexGridSizer* FlexGridSizer22;
    wxFlexGridSizer* FlexGridSizer23;
    wxFlexGridSizer* FlexGridSizer24;
    wxFlexGridSizer* FlexGridSizer25;
    wxFlexGridSizer* FlexGridSizer26;
    wxFlexGridSizer* FlexGridSizer27;
    wxFlexGridSizer* FlexGridSizer28;
    wxFlexGridSizer* FlexGridSizer29;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer30;
    wxFlexGridSizer* FlexGridSizer31;
    wxFlexGridSizer* FlexGridSizer32;
    wxFlexGridSizer* FlexGridSizer33;
    wxFlexGridSizer* FlexGridSizer34;
    wxFlexGridSizer* FlexGridSizer35;
    wxFlexGridSizer* FlexGridSizer36;
    wxFlexGridSizer* FlexGridSizer37;
    wxFlexGridSizer* FlexGridSizer38;
    wxFlexGridSizer* FlexGridSizer39;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer40;
    wxFlexGridSizer* FlexGridSizer41;
    wxFlexGridSizer* FlexGridSizer42;
    wxFlexGridSizer* FlexGridSizer43;
    wxFlexGridSizer* FlexGridSizer44;
    wxFlexGridSizer* FlexGridSizer45;
    wxFlexGridSizer* FlexGridSizer46;
    wxFlexGridSizer* FlexGridSizer47;
    wxFlexGridSizer* FlexGridSizer48;
    wxFlexGridSizer* FlexGridSizer49;
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer50;
    wxFlexGridSizer* FlexGridSizer51;
    wxFlexGridSizer* FlexGridSizer52;
    wxFlexGridSizer* FlexGridSizer53;
    wxFlexGridSizer* FlexGridSizer54;
    wxFlexGridSizer* FlexGridSizer55;
    wxFlexGridSizer* FlexGridSizer56;
    wxFlexGridSizer* FlexGridSizer57;
    wxFlexGridSizer* FlexGridSizer58;
    wxFlexGridSizer* FlexGridSizer59;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer60;
    wxFlexGridSizer* FlexGridSizer61;
    wxFlexGridSizer* FlexGridSizer62;
    wxFlexGridSizer* FlexGridSizer63;
    wxFlexGridSizer* FlexGridSizer64;
    wxFlexGridSizer* FlexGridSizer65;
    wxFlexGridSizer* FlexGridSizer66;
    wxFlexGridSizer* FlexGridSizer67;
    wxFlexGridSizer* FlexGridSizer68;
    wxFlexGridSizer* FlexGridSizer69;
    wxFlexGridSizer* FlexGridSizer6;
    wxFlexGridSizer* FlexGridSizer70;
    wxFlexGridSizer* FlexGridSizer71;
    wxFlexGridSizer* FlexGridSizer72;
    wxFlexGridSizer* FlexGridSizer73;
    wxFlexGridSizer* FlexGridSizer74;
    wxFlexGridSizer* FlexGridSizer75;
    wxFlexGridSizer* FlexGridSizer76;
    wxFlexGridSizer* FlexGridSizer77;
    wxFlexGridSizer* FlexGridSizer78;
    wxFlexGridSizer* FlexGridSizer79;
    wxFlexGridSizer* FlexGridSizer7;
    wxFlexGridSizer* FlexGridSizer80;
    wxFlexGridSizer* FlexGridSizer81;
    wxFlexGridSizer* FlexGridSizer82;
    wxFlexGridSizer* FlexGridSizer83;
    wxFlexGridSizer* FlexGridSizer84;
    wxFlexGridSizer* FlexGridSizer85;
    wxFlexGridSizer* FlexGridSizer86;
    wxFlexGridSizer* FlexGridSizer8;
    wxFlexGridSizer* FlexGridSizer9;
    wxGridSizer* GridSizer1;
    wxGridSizer* GridSizer2;
    wxGridSizer* GridSizer3;
    wxGridSizer* GridSizer4;
    wxGridSizer* GridSizer5;
    wxGridSizer* GridSizer6;
    wxGridSizer* GridSizer7;
    wxGridSizer* GridSizer8;

    Create(parent, wxID_ANY, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(3, 6, 0, 0);
    FlexGridSizer25 = new wxFlexGridSizer(5, 1, 0, 0);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Remarks"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer25->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl2 = new wxTextCtrl(this, ID_TEXTCTRL2, _("Some comment (remove all to omit comments)"), wxDefaultPosition, wxSize(284,64), wxTE_AUTO_SCROLL|wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    FlexGridSizer25->Add(TextCtrl2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Result section for ini file"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    FlexGridSizer25->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl7 = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxSize(282,407), wxTE_AUTO_SCROLL|wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    FlexGridSizer25->Add(TextCtrl7, 1, wxALL|wxEXPAND, 5);
    StaticLine5 = new wxStaticLine(this, ID_STATICLINE5, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE5"));
    FlexGridSizer25->Add(StaticLine5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(FlexGridSizer25, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer3 = new wxFlexGridSizer(22, 1, 0, 0);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    FlexGridSizer27 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer51 = new wxFlexGridSizer(0, 10, 0, 0);
    StaticText15 = new wxStaticText(Panel1, ID_STATICTEXT15, _("Last control event:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    FlexGridSizer51->Add(StaticText15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl25 = new wxTextCtrl(Panel1, ID_TEXTCTRL25, wxEmptyString, wxDefaultPosition, wxSize(126,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL25"));
    FlexGridSizer51->Add(TextCtrl25, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer51->Add(8,34,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    StaticText68 = new wxStaticText(Panel1, ID_STATICTEXT68, _("EvID(decimal)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT68"));
    FlexGridSizer51->Add(StaticText68, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl10 = new wxTextCtrl(Panel1, ID_TEXTCTRL10, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
    FlexGridSizer51->Add(TextCtrl10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer51->Add(6,34,1, wxALL|wxEXPAND, 0);
    FlexGridSizer27->Add(FlexGridSizer51, 1, wxALL|wxEXPAND, 0);
    StaticLine2 = new wxStaticLine(Panel1, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
    FlexGridSizer27->Add(StaticLine2, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer7 = new wxFlexGridSizer(10, 1, 0, 0);
    FlexGridSizer17 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice2 = new wxChoice(Panel1, ID_CHOICE2, wxDefaultPosition, wxSize(326,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    Choice2->Append(wxEmptyString);
    Choice2->SetSelection( Choice2->Append(_("SPDT Switch / Button")) );
    Choice2->Append(_("Cyclic SPST Button"));
    Choice2->Append(_("SPST Button, Switch / Encoder dir pulse/ Rotary switch pole"));
    Choice2->Append(_("Encoder"));
    FlexGridSizer17->Add(Choice2, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Control type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer17->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer7->Add(FlexGridSizer17, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer81 = new wxFlexGridSizer(1, 3, 0, 0);
    FlexGridSizer82 = new wxFlexGridSizer(10, 1, 0, 0);
    FlexGridSizer61 = new wxFlexGridSizer(0, 3, 0, 0);
    TextCtrl1 = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    FlexGridSizer61->Add(TextCtrl1, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 6);
    StaticText27 = new wxStaticText(Panel1, ID_STATICTEXT27, _("    Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
    FlexGridSizer61->Add(StaticText27, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button31 = new wxButton(Panel1, ID_BUTTON31, _("<Ctrls"), wxDefaultPosition, wxSize(64,23), 0, wxDefaultValidator, _T("ID_BUTTON31"));
    FlexGridSizer61->Add(Button31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer82->Add(FlexGridSizer61, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer18 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice1 = new wxChoice(Panel1, ID_CHOICE1, wxDefaultPosition, wxSize(125,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice1->SetSelection( Choice1->Append(_("Integer")) );
    Choice1->Append(_("Unsigned Integer"));
    Choice1->Append(_("Float"));
    Choice1->Append(_("String"));
    FlexGridSizer18->Add(Choice1, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Data[Offset] type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer18->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer82->Add(FlexGridSizer18, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer60 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice8 = new wxChoice(Panel1, ID_CHOICE8, wxDefaultPosition, wxSize(70,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE8"));
    Choice8->Append(_("1"));
    Choice8->SetSelection( Choice8->Append(_("2")) );
    Choice8->Append(_("4"));
    Choice8->Append(_("8"));
    FlexGridSizer60->Add(Choice8, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 4);
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Data[Offset] size"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer60->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer82->Add(FlexGridSizer60, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer78 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice11 = new wxChoice(Panel1, ID_CHOICE11, wxDefaultPosition, wxSize(70,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE11"));
    Choice11->SetSelection( Choice11->Append(_("Normal")) );
    Choice11->Append(_("BCD"));
    FlexGridSizer78->Add(Choice11, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 4);
    StaticText110 = new wxStaticText(Panel1, ID_STATICTEXT110, _("Encoding"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT110"));
    FlexGridSizer78->Add(StaticText110, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer82->Add(FlexGridSizer78, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer81->Add(FlexGridSizer82, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine18 = new wxStaticLine(Panel1, ID_STATICLINE18, wxDefaultPosition, wxSize(2,44), wxLI_VERTICAL, _T("ID_STATICLINE18"));
    FlexGridSizer81->Add(StaticLine18, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer83 = new wxFlexGridSizer(2, 1, 0, 0);
    StaticText113 = new wxStaticText(Panel1, ID_STATICTEXT113, _("SPST - Single Pole Single Throw\nSPDT - Single Pole Double Throw"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT113"));
    FlexGridSizer83->Add(StaticText113, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    FlexGridSizer81->Add(FlexGridSizer83, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
    FlexGridSizer7->Add(FlexGridSizer81, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer27->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 0);
    StaticLine13 = new wxStaticLine(Panel1, ID_STATICLINE13, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE13"));
    FlexGridSizer27->Add(StaticLine13, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer5 = new wxFlexGridSizer(10, 1, 0, 0);
    FlexGridSizer8 = new wxFlexGridSizer(1, 3, 0, 0);
    FlexGridSizer12 = new wxFlexGridSizer(6, 1, 0, 0);
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    StaticText14 = new wxStaticText(Panel1, ID_STATICTEXT14, _("0:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT14"));
    BoxSizer6->Add(StaticText14, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    TextCtrl9 = new wxTextCtrl(Panel1, ID_TEXTCTRL9, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    BoxSizer6->Add(TextCtrl9, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer12->Add(BoxSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    StaticText13 = new wxStaticText(Panel1, ID_STATICTEXT13, _("1:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT13"));
    BoxSizer5->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    TextCtrl8 = new wxTextCtrl(Panel1, ID_TEXTCTRL8, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
    BoxSizer5->Add(TextCtrl8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer12->Add(BoxSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, _("Mask:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT11"));
    BoxSizer4->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    TextCtrl6 = new wxTextCtrl(Panel1, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    BoxSizer4->Add(TextCtrl6, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer12->Add(BoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer8->Add(FlexGridSizer12, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
    FlexGridSizer23 = new wxFlexGridSizer(3, 1, 0, 0);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, _("Min:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT8"));
    BoxSizer1->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    TextCtrl3 = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    BoxSizer1->Add(TextCtrl3, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer23->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT9, _("Max:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT9"));
    BoxSizer2->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    TextCtrl4 = new wxTextCtrl(Panel1, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    BoxSizer2->Add(TextCtrl4, 1, wxALL|wxALIGN_TOP, 5);
    FlexGridSizer23->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    StaticText10 = new wxStaticText(Panel1, ID_STATICTEXT10, _("Increment:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT10"));
    BoxSizer3->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    TextCtrl5 = new wxTextCtrl(Panel1, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    BoxSizer3->Add(TextCtrl5, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer23->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer8->Add(FlexGridSizer23, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
    FlexGridSizer5->Add(FlexGridSizer8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer27->Add(FlexGridSizer5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine19 = new wxStaticLine(Panel1, ID_STATICLINE19, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE19"));
    FlexGridSizer27->Add(StaticLine19, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer11 = new wxFlexGridSizer(1, 3, 0, 0);
    wxString __wxRadioBoxChoices_1[2] =
    {
    	_("Loop  (e.g. HDG, CRS)"),
    	_("Limits (e.g. IAS, ALT) ")
    };
    RadioBox1 = new wxRadioBox(Panel1, ID_RADIOBOX1, _("Cycling options"), wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
    RadioBox1->SetSelection(0);
    FlexGridSizer11->Add(RadioBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer11->Add(57,20,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button5 = new wxButton(Panel1, ID_BUTTON5, _("Add control section"), wxDefaultPosition, wxSize(131,43), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    FlexGridSizer11->Add(Button5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer27->Add(FlexGridSizer11, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine17 = new wxStaticLine(Panel1, ID_STATICLINE17, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE17"));
    FlexGridSizer27->Add(StaticLine17, 1, wxALL|wxEXPAND, 0);
    StaticText109 = new wxStaticText(Panel1, ID_STATICTEXT109, _("Mask calculator"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT109"));
    wxFont StaticText109Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    StaticText109->SetFont(StaticText109Font);
    FlexGridSizer27->Add(StaticText109, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer75 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer76 = new wxFlexGridSizer(3, 1, 0, 0);
    Button26 = new wxButton(Panel1, ID_BUTTON26, _("All clear"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON26"));
    FlexGridSizer76->Add(Button26, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button27 = new wxButton(Panel1, ID_BUTTON27, _("All set"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON27"));
    FlexGridSizer76->Add(Button27, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button28 = new wxButton(Panel1, ID_BUTTON28, _("Invert"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON28"));
    FlexGridSizer76->Add(Button28, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer75->Add(FlexGridSizer76, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer74 = new wxFlexGridSizer(4, 1, 0, 0);
    BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer15 = new wxBoxSizer(wxVERTICAL);
    StaticText19 = new wxStaticText(Panel1, ID_STATICTEXT19, _("31"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
    BoxSizer15->Add(StaticText19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox5 = new wxCheckBox(Panel1, ID_CHECKBOX5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBox5->SetValue(false);
    BoxSizer15->Add(CheckBox5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer16 = new wxBoxSizer(wxVERTICAL);
    StaticText78 = new wxStaticText(Panel1, ID_STATICTEXT78, _("30"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT78"));
    BoxSizer16->Add(StaticText78, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox6 = new wxCheckBox(Panel1, ID_CHECKBOX6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    CheckBox6->SetValue(false);
    BoxSizer16->Add(CheckBox6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer17 = new wxBoxSizer(wxVERTICAL);
    StaticText79 = new wxStaticText(Panel1, ID_STATICTEXT79, _("29"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT79"));
    BoxSizer17->Add(StaticText79, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox7 = new wxCheckBox(Panel1, ID_CHECKBOX7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
    CheckBox7->SetValue(false);
    BoxSizer17->Add(CheckBox7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer18 = new wxBoxSizer(wxVERTICAL);
    StaticText80 = new wxStaticText(Panel1, ID_STATICTEXT80, _("28"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT80"));
    BoxSizer18->Add(StaticText80, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox8 = new wxCheckBox(Panel1, ID_CHECKBOX8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
    CheckBox8->SetValue(false);
    BoxSizer18->Add(CheckBox8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer19 = new wxBoxSizer(wxVERTICAL);
    StaticText81 = new wxStaticText(Panel1, ID_STATICTEXT81, _("27"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT81"));
    BoxSizer19->Add(StaticText81, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox9 = new wxCheckBox(Panel1, ID_CHECKBOX9, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
    CheckBox9->SetValue(false);
    BoxSizer19->Add(CheckBox9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer20 = new wxBoxSizer(wxVERTICAL);
    StaticText82 = new wxStaticText(Panel1, ID_STATICTEXT82, _("26"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT82"));
    BoxSizer20->Add(StaticText82, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox10 = new wxCheckBox(Panel1, ID_CHECKBOX10, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX10"));
    CheckBox10->SetValue(false);
    BoxSizer20->Add(CheckBox10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer21 = new wxBoxSizer(wxVERTICAL);
    StaticText83 = new wxStaticText(Panel1, ID_STATICTEXT83, _("25"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT83"));
    BoxSizer21->Add(StaticText83, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox11 = new wxCheckBox(Panel1, ID_CHECKBOX11, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX11"));
    CheckBox11->SetValue(false);
    BoxSizer21->Add(CheckBox11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer21, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer22 = new wxBoxSizer(wxVERTICAL);
    StaticText84 = new wxStaticText(Panel1, ID_STATICTEXT84, _("24"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT84"));
    BoxSizer22->Add(StaticText84, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox12 = new wxCheckBox(Panel1, ID_CHECKBOX12, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX12"));
    CheckBox12->SetValue(false);
    BoxSizer22->Add(CheckBox12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(BoxSizer22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer74->Add(BoxSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer24 = new wxBoxSizer(wxVERTICAL);
    StaticText85 = new wxStaticText(Panel1, ID_STATICTEXT85, _("23"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT85"));
    BoxSizer24->Add(StaticText85, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox13 = new wxCheckBox(Panel1, ID_CHECKBOX13, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX13"));
    CheckBox13->SetValue(false);
    BoxSizer24->Add(CheckBox13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer24, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer25 = new wxBoxSizer(wxVERTICAL);
    StaticText86 = new wxStaticText(Panel1, ID_STATICTEXT86, _("22"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT86"));
    BoxSizer25->Add(StaticText86, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox14 = new wxCheckBox(Panel1, ID_CHECKBOX14, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX14"));
    CheckBox14->SetValue(false);
    BoxSizer25->Add(CheckBox14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer25, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer26 = new wxBoxSizer(wxVERTICAL);
    StaticText87 = new wxStaticText(Panel1, ID_STATICTEXT87, _("21"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT87"));
    BoxSizer26->Add(StaticText87, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox15 = new wxCheckBox(Panel1, ID_CHECKBOX15, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX15"));
    CheckBox15->SetValue(false);
    BoxSizer26->Add(CheckBox15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer26, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer27 = new wxBoxSizer(wxVERTICAL);
    StaticText88 = new wxStaticText(Panel1, ID_STATICTEXT88, _("20"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT88"));
    BoxSizer27->Add(StaticText88, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox16 = new wxCheckBox(Panel1, ID_CHECKBOX16, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX16"));
    CheckBox16->SetValue(false);
    BoxSizer27->Add(CheckBox16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer27, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer28 = new wxBoxSizer(wxVERTICAL);
    StaticText89 = new wxStaticText(Panel1, ID_STATICTEXT89, _("19"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT89"));
    BoxSizer28->Add(StaticText89, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox17 = new wxCheckBox(Panel1, ID_CHECKBOX17, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX17"));
    CheckBox17->SetValue(false);
    BoxSizer28->Add(CheckBox17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer28, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer29 = new wxBoxSizer(wxVERTICAL);
    StaticText90 = new wxStaticText(Panel1, ID_STATICTEXT90, _("18"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT90"));
    BoxSizer29->Add(StaticText90, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox18 = new wxCheckBox(Panel1, ID_CHECKBOX18, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX18"));
    CheckBox18->SetValue(false);
    BoxSizer29->Add(CheckBox18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer29, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer30 = new wxBoxSizer(wxVERTICAL);
    StaticText91 = new wxStaticText(Panel1, ID_STATICTEXT91, _("17"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT91"));
    BoxSizer30->Add(StaticText91, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox19 = new wxCheckBox(Panel1, ID_CHECKBOX19, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX19"));
    CheckBox19->SetValue(false);
    BoxSizer30->Add(CheckBox19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer31 = new wxBoxSizer(wxVERTICAL);
    StaticText92 = new wxStaticText(Panel1, ID_STATICTEXT92, _("16"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT92"));
    BoxSizer31->Add(StaticText92, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox20 = new wxCheckBox(Panel1, ID_CHECKBOX20, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX20"));
    CheckBox20->SetValue(false);
    BoxSizer31->Add(CheckBox20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer23->Add(BoxSizer31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer74->Add(BoxSizer23, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer33 = new wxBoxSizer(wxVERTICAL);
    StaticText93 = new wxStaticText(Panel1, ID_STATICTEXT93, _("15"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT93"));
    BoxSizer33->Add(StaticText93, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox21 = new wxCheckBox(Panel1, ID_CHECKBOX21, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX21"));
    CheckBox21->SetValue(false);
    BoxSizer33->Add(CheckBox21, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer33, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer34 = new wxBoxSizer(wxVERTICAL);
    StaticText94 = new wxStaticText(Panel1, ID_STATICTEXT94, _("14"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT94"));
    BoxSizer34->Add(StaticText94, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox22 = new wxCheckBox(Panel1, ID_CHECKBOX22, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX22"));
    CheckBox22->SetValue(false);
    BoxSizer34->Add(CheckBox22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer34, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer35 = new wxBoxSizer(wxVERTICAL);
    StaticText95 = new wxStaticText(Panel1, ID_STATICTEXT95, _("13"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT95"));
    BoxSizer35->Add(StaticText95, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox23 = new wxCheckBox(Panel1, ID_CHECKBOX23, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX23"));
    CheckBox23->SetValue(false);
    BoxSizer35->Add(CheckBox23, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer35, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer36 = new wxBoxSizer(wxVERTICAL);
    StaticText96 = new wxStaticText(Panel1, ID_STATICTEXT96, _("12"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT96"));
    BoxSizer36->Add(StaticText96, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox24 = new wxCheckBox(Panel1, ID_CHECKBOX24, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX24"));
    CheckBox24->SetValue(false);
    BoxSizer36->Add(CheckBox24, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer36, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer37 = new wxBoxSizer(wxVERTICAL);
    StaticText97 = new wxStaticText(Panel1, ID_STATICTEXT97, _("11"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT97"));
    BoxSizer37->Add(StaticText97, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox25 = new wxCheckBox(Panel1, ID_CHECKBOX25, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX25"));
    CheckBox25->SetValue(false);
    BoxSizer37->Add(CheckBox25, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer37, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer38 = new wxBoxSizer(wxVERTICAL);
    StaticText98 = new wxStaticText(Panel1, ID_STATICTEXT98, _("10"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT98"));
    BoxSizer38->Add(StaticText98, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox26 = new wxCheckBox(Panel1, ID_CHECKBOX26, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX26"));
    CheckBox26->SetValue(false);
    BoxSizer38->Add(CheckBox26, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer38, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer39 = new wxBoxSizer(wxVERTICAL);
    StaticText99 = new wxStaticText(Panel1, ID_STATICTEXT99, _("9"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT99"));
    BoxSizer39->Add(StaticText99, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox27 = new wxCheckBox(Panel1, ID_CHECKBOX27, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX27"));
    CheckBox27->SetValue(false);
    BoxSizer39->Add(CheckBox27, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer39, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer40 = new wxBoxSizer(wxVERTICAL);
    StaticText100 = new wxStaticText(Panel1, ID_STATICTEXT100, _("8"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT100"));
    BoxSizer40->Add(StaticText100, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox28 = new wxCheckBox(Panel1, ID_CHECKBOX28, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX28"));
    CheckBox28->SetValue(false);
    BoxSizer40->Add(CheckBox28, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer32->Add(BoxSizer40, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer74->Add(BoxSizer32, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer42 = new wxBoxSizer(wxVERTICAL);
    StaticText101 = new wxStaticText(Panel1, ID_STATICTEXT101, _("7"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT101"));
    BoxSizer42->Add(StaticText101, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox29 = new wxCheckBox(Panel1, ID_CHECKBOX29, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX29"));
    CheckBox29->SetValue(false);
    BoxSizer42->Add(CheckBox29, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer42, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer43 = new wxBoxSizer(wxVERTICAL);
    StaticText102 = new wxStaticText(Panel1, ID_STATICTEXT102, _("6"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT102"));
    BoxSizer43->Add(StaticText102, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox30 = new wxCheckBox(Panel1, ID_CHECKBOX30, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX30"));
    CheckBox30->SetValue(false);
    BoxSizer43->Add(CheckBox30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer43, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer44 = new wxBoxSizer(wxVERTICAL);
    StaticText103 = new wxStaticText(Panel1, ID_STATICTEXT103, _("5"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT103"));
    BoxSizer44->Add(StaticText103, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox31 = new wxCheckBox(Panel1, ID_CHECKBOX31, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX31"));
    CheckBox31->SetValue(false);
    BoxSizer44->Add(CheckBox31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer44, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer45 = new wxBoxSizer(wxVERTICAL);
    StaticText104 = new wxStaticText(Panel1, ID_STATICTEXT104, _("4"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT104"));
    BoxSizer45->Add(StaticText104, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox32 = new wxCheckBox(Panel1, ID_CHECKBOX32, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX32"));
    CheckBox32->SetValue(false);
    BoxSizer45->Add(CheckBox32, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer45, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer46 = new wxBoxSizer(wxVERTICAL);
    StaticText105 = new wxStaticText(Panel1, ID_STATICTEXT105, _("3"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT105"));
    BoxSizer46->Add(StaticText105, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox33 = new wxCheckBox(Panel1, ID_CHECKBOX33, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX33"));
    CheckBox33->SetValue(false);
    BoxSizer46->Add(CheckBox33, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer46, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer47 = new wxBoxSizer(wxVERTICAL);
    StaticText106 = new wxStaticText(Panel1, ID_STATICTEXT106, _("2"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT106"));
    BoxSizer47->Add(StaticText106, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox34 = new wxCheckBox(Panel1, ID_CHECKBOX34, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX34"));
    CheckBox34->SetValue(false);
    BoxSizer47->Add(CheckBox34, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer47, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer48 = new wxBoxSizer(wxVERTICAL);
    StaticText107 = new wxStaticText(Panel1, ID_STATICTEXT107, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT107"));
    BoxSizer48->Add(StaticText107, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox35 = new wxCheckBox(Panel1, ID_CHECKBOX35, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX35"));
    CheckBox35->SetValue(false);
    BoxSizer48->Add(CheckBox35, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer48, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer49 = new wxBoxSizer(wxVERTICAL);
    StaticText108 = new wxStaticText(Panel1, ID_STATICTEXT108, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT108"));
    BoxSizer49->Add(StaticText108, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    CheckBox36 = new wxCheckBox(Panel1, ID_CHECKBOX36, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX36"));
    CheckBox36->SetValue(false);
    BoxSizer49->Add(CheckBox36, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer41->Add(BoxSizer49, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer74->Add(BoxSizer41, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer75->Add(FlexGridSizer74, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer77 = new wxFlexGridSizer(2, 1, 0, 0);
    Button29 = new wxButton(Panel1, ID_BUTTON29, _("> Mask"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON29"));
    FlexGridSizer77->Add(Button29, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button30 = new wxButton(Panel1, ID_BUTTON30, _("< Mask"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON30"));
    FlexGridSizer77->Add(Button30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer75->Add(FlexGridSizer77, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer27->Add(FlexGridSizer75, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    Panel1->SetSizer(FlexGridSizer27);
    FlexGridSizer27->Fit(Panel1);
    FlexGridSizer27->SetSizeHints(Panel1);
    Panel6 = new wxPanel(Notebook1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    FlexGridSizer4 = new wxFlexGridSizer(11, 1, 0, 0);
    FlexGridSizer56 = new wxFlexGridSizer(0, 12, 0, 0);
    StaticText61 = new wxStaticText(Panel6, ID_STATICTEXT61, _("Last control event:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT61"));
    FlexGridSizer56->Add(StaticText61, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl59 = new wxTextCtrl(Panel6, ID_TEXTCTRL59, wxEmptyString, wxDefaultPosition, wxSize(126,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL59"));
    FlexGridSizer56->Add(TextCtrl59, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer56->Add(8,34,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    StaticText64 = new wxStaticText(Panel6, ID_STATICTEXT64, _("EvId(decimal)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT64"));
    FlexGridSizer56->Add(StaticText64, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl60 = new wxTextCtrl(Panel6, ID_TEXTCTRL60, wxEmptyString, wxDefaultPosition, wxSize(35,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL60"));
    FlexGridSizer56->Add(TextCtrl60, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer56->Add(6,34,1, wxALL|wxEXPAND, 0);
    FlexGridSizer4->Add(FlexGridSizer56, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer57 = new wxFlexGridSizer(0, 12, 0, 0);
    StaticText62 = new wxStaticText(Panel6, ID_STATICTEXT62, _("Event filter"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT62"));
    FlexGridSizer57->Add(StaticText62, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl61 = new wxTextCtrl(Panel6, ID_TEXTCTRL61, wxEmptyString, wxDefaultPosition, wxSize(63,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL61"));
    FlexGridSizer57->Add(TextCtrl61, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer57->Add(77,34,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    StaticText63 = new wxStaticText(Panel6, ID_STATICTEXT63, _("Current value"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT63"));
    FlexGridSizer57->Add(StaticText63, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl62 = new wxTextCtrl(Panel6, ID_TEXTCTRL62, wxEmptyString, wxDefaultPosition, wxSize(65,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL62"));
    FlexGridSizer57->Add(TextCtrl62, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer57, 1, wxALL|wxEXPAND, 0);
    StaticLine3 = new wxStaticLine(Panel6, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
    FlexGridSizer4->Add(StaticLine3, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer48 = new wxFlexGridSizer(8, 1, 0, 0);
    FlexGridSizer62 = new wxFlexGridSizer(0, 5, 0, 0);
    TextCtrl17 = new wxTextCtrl(Panel6, ID_TEXTCTRL17, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL17"));
    FlexGridSizer62->Add(TextCtrl17, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    StaticText71 = new wxStaticText(Panel6, ID_STATICTEXT71, _("    Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT71"));
    FlexGridSizer62->Add(StaticText71, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    Button32 = new wxButton(Panel6, ID_BUTTON32, _("<Ctrls"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON32"));
    FlexGridSizer62->Add(Button32, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl67 = new wxTextCtrl(Panel6, ID_TEXTCTRL67, wxEmptyString, wxDefaultPosition, wxSize(88,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL67"));
    FlexGridSizer62->Add(TextCtrl67, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText114 = new wxStaticText(Panel6, ID_STATICTEXT114, _("ControlID"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT114"));
    FlexGridSizer62->Add(StaticText114, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer48->Add(FlexGridSizer62, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer63 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice9 = new wxChoice(Panel6, ID_CHOICE9, wxDefaultPosition, wxSize(134,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE9"));
    Choice9->SetSelection( Choice9->Append(_("Integer")) );
    Choice9->Append(_("Unsigned Integer"));
    Choice9->Append(_("Float"));
    Choice9->Append(_("String"));
    FlexGridSizer63->Add(Choice9, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticText72 = new wxStaticText(Panel6, ID_STATICTEXT72, _("Data[Offset] type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT72"));
    FlexGridSizer63->Add(StaticText72, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer48->Add(FlexGridSizer63, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer54 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice7 = new wxChoice(Panel6, ID_CHOICE7, wxDefaultPosition, wxSize(70,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE7"));
    Choice7->Append(_("1"));
    Choice7->SetSelection( Choice7->Append(_("2")) );
    Choice7->Append(_("4"));
    Choice7->Append(_("8"));
    FlexGridSizer54->Add(Choice7, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 4);
    StaticText31 = new wxStaticText(Panel6, ID_STATICTEXT31, _("Data[Offset] size"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT31"));
    FlexGridSizer54->Add(StaticText31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer48->Add(FlexGridSizer54, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer79 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice12 = new wxChoice(Panel6, ID_CHOICE12, wxDefaultPosition, wxSize(70,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE12"));
    Choice12->SetSelection( Choice12->Append(_("Normal")) );
    Choice12->Append(_("BCD"));
    FlexGridSizer79->Add(Choice12, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 4);
    StaticText111 = new wxStaticText(Panel6, ID_STATICTEXT111, _("Encoding"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT111"));
    FlexGridSizer79->Add(StaticText111, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer48->Add(FlexGridSizer79, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer4->Add(FlexGridSizer48, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine4 = new wxStaticLine(Panel6, ID_STATICLINE4, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE4"));
    FlexGridSizer4->Add(StaticLine4, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer68 = new wxFlexGridSizer(8, 1, 0, 0);
    FlexGridSizer69 = new wxFlexGridSizer(0, 3, 0, 0);
    TextCtrl49 = new wxTextCtrl(Panel6, ID_TEXTCTRL49, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL49"));
    FlexGridSizer69->Add(TextCtrl49, 1, wxTOP|wxBOTTOM|wxEXPAND, 0);
    StaticText75 = new wxStaticText(Panel6, ID_STATICTEXT75, _("    factor"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT75"));
    FlexGridSizer69->Add(StaticText75, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer68->Add(FlexGridSizer69, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
    FlexGridSizer70 = new wxFlexGridSizer(0, 3, 0, 0);
    TextCtrl66 = new wxTextCtrl(Panel6, ID_TEXTCTRL66, wxEmptyString, wxDefaultPosition, wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL66"));
    FlexGridSizer70->Add(TextCtrl66, 1, wxTOP|wxBOTTOM|wxEXPAND, 0);
    StaticText76 = new wxStaticText(Panel6, ID_STATICTEXT76, _("    zero offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT76"));
    FlexGridSizer70->Add(StaticText76, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer68->Add(FlexGridSizer70, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    FlexGridSizer4->Add(FlexGridSizer68, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Button21 = new wxButton(Panel6, ID_BUTTON21, _("Add control section"), wxDefaultPosition, wxSize(148,32), 0, wxDefaultValidator, _T("ID_BUTTON21"));
    FlexGridSizer4->Add(Button21, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine11 = new wxStaticLine(Panel6, ID_STATICLINE11, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE11"));
    FlexGridSizer4->Add(StaticLine11, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer4->Add(20,18,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer7 = new wxGridSizer(3, 4, 0, 0);
    Button18 = new wxButton(Panel6, ID_BUTTON18, _("DeviceMax >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON18"));
    GridSizer7->Add(Button18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl50 = new wxTextCtrl(Panel6, ID_TEXTCTRL50, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL50"));
    GridSizer7->Add(TextCtrl50, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText57 = new wxStaticText(Panel6, ID_STATICTEXT57, _("SimMax:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT57"));
    GridSizer7->Add(StaticText57, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl51 = new wxTextCtrl(Panel6, ID_TEXTCTRL51, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL51"));
    GridSizer7->Add(TextCtrl51, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    Button19 = new wxButton(Panel6, ID_BUTTON19, _("DevCenter >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON19"));
    Button19->Disable();
    GridSizer7->Add(Button19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl56 = new wxTextCtrl(Panel6, ID_TEXTCTRL56, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL56"));
    TextCtrl56->Disable();
    GridSizer7->Add(TextCtrl56, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText58 = new wxStaticText(Panel6, ID_STATICTEXT58, _("SimMin:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT58"));
    GridSizer7->Add(StaticText58, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl57 = new wxTextCtrl(Panel6, ID_TEXTCTRL57, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL57"));
    GridSizer7->Add(TextCtrl57, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    Button20 = new wxButton(Panel6, ID_BUTTON20, _("DeviceMin >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON20"));
    GridSizer7->Add(Button20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl58 = new wxTextCtrl(Panel6, ID_TEXTCTRL58, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL58"));
    GridSizer7->Add(TextCtrl58, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText60 = new wxStaticText(Panel6, ID_STATICTEXT60, _("Has center:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT60"));
    GridSizer7->Add(StaticText60, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    CheckBox4 = new wxCheckBox(Panel6, ID_CHECKBOX4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBox4->SetValue(false);
    GridSizer7->Add(CheckBox4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(GridSizer7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Button25 = new wxButton(Panel6, ID_BUTTON25, _("Calculate\nfactor and zero offset"), wxDefaultPosition, wxSize(168,44), 0, wxDefaultValidator, _T("ID_BUTTON25"));
    FlexGridSizer4->Add(Button25, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Panel6->SetSizer(FlexGridSizer4);
    FlexGridSizer4->Fit(Panel6);
    FlexGridSizer4->SetSizeHints(Panel6);
    Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    FlexGridSizer9 = new wxFlexGridSizer(11, 1, 0, 0);
    FlexGridSizer13 = new wxFlexGridSizer(1, 3, 0, 0);
    FlexGridSizer16 = new wxFlexGridSizer(8, 1, 0, 0);
    FlexGridSizer10 = new wxFlexGridSizer(1, 2, 0, 0);
    Choice4 = new wxChoice(Panel2, ID_CHOICE4, wxDefaultPosition, wxSize(250,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE4"));
    Choice4->Append(_("Numeric ( LCD, 7-seg ind. Servo/Stepper motors )"));
    Choice4->SetSelection( Choice4->Append(_("Discrete ( LEDs, Lamps, Power keys )")) );
    FlexGridSizer10->Add(Choice4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText18 = new wxStaticText(Panel2, ID_STATICTEXT18, _("Indication type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
    FlexGridSizer10->Add(StaticText18, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer16->Add(FlexGridSizer10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer14 = new wxFlexGridSizer(8, 1, 0, 0);
    FlexGridSizer64 = new wxFlexGridSizer(0, 3, 0, 0);
    TextCtrl11 = new wxTextCtrl(Panel2, ID_TEXTCTRL11, wxEmptyString, wxDefaultPosition, wxSize(119,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
    FlexGridSizer64->Add(TextCtrl11, 1, wxTOP|wxBOTTOM|wxEXPAND, 0);
    StaticText16 = new wxStaticText(Panel2, ID_STATICTEXT16, _("    Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    FlexGridSizer64->Add(StaticText16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer14->Add(FlexGridSizer64, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer65 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice3 = new wxChoice(Panel2, ID_CHOICE3, wxDefaultPosition, wxSize(151,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
    Choice3->SetSelection( Choice3->Append(_("Integer")) );
    Choice3->Append(_("Unsigned Integer"));
    Choice3->Append(_("Float"));
    Choice3->Append(_("String"));
    FlexGridSizer65->Add(Choice3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticText17 = new wxStaticText(Panel2, ID_STATICTEXT17, _("Data[Offset] type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
    FlexGridSizer65->Add(StaticText17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer14->Add(FlexGridSizer65, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer66 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice10 = new wxChoice(Panel2, ID_CHOICE10, wxDefaultPosition, wxSize(80,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE10"));
    Choice10->Append(_("1"));
    Choice10->SetSelection( Choice10->Append(_("2")) );
    Choice10->Append(_("4"));
    Choice10->Append(_("8"));
    FlexGridSizer66->Add(Choice10, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 4);
    StaticText34 = new wxStaticText(Panel2, ID_STATICTEXT34, _("Data[Offset] size"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT34"));
    FlexGridSizer66->Add(StaticText34, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer14->Add(FlexGridSizer66, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer80 = new wxFlexGridSizer(0, 3, 0, 0);
    Choice13 = new wxChoice(Panel2, ID_CHOICE13, wxDefaultPosition, wxSize(80,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE13"));
    Choice13->SetSelection( Choice13->Append(_("Normal")) );
    Choice13->Append(_("BCD"));
    FlexGridSizer80->Add(Choice13, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 4);
    StaticText112 = new wxStaticText(Panel2, ID_STATICTEXT112, _("Encoding"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT112"));
    FlexGridSizer80->Add(StaticText112, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer14->Add(FlexGridSizer80, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer16->Add(FlexGridSizer14, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer13->Add(FlexGridSizer16, 1, wxALL|wxEXPAND, 3);
    FlexGridSizer9->Add(FlexGridSizer13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine14 = new wxStaticLine(Panel2, ID_STATICLINE14, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE14"));
    FlexGridSizer9->Add(StaticLine14, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer24 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText20 = new wxStaticText(Panel2, ID_STATICTEXT20, _("[section name]"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT20"));
    FlexGridSizer24->Add(StaticText20, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl13 = new wxTextCtrl(Panel2, ID_TEXTCTRL13, wxEmptyString, wxDefaultPosition, wxSize(210,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL13"));
    FlexGridSizer24->Add(TextCtrl13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer9->Add(FlexGridSizer24, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer19 = new wxFlexGridSizer(1, 4, 0, 0);
    FlexGridSizer20 = new wxFlexGridSizer(4, 1, 0, 0);
    FlexGridSizer20->Add(20,47,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button6 = new wxButton(Panel2, ID_BUTTON6, _("Add indication \nsection"), wxDefaultPosition, wxSize(95,49), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    FlexGridSizer20->Add(Button6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer19->Add(FlexGridSizer20, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    FlexGridSizer21 = new wxFlexGridSizer(5, 1, 0, 0);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    StaticText21 = new wxStaticText(Panel2, ID_STATICTEXT21, _("*Device:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT21"));
    BoxSizer8->Add(StaticText21, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl14 = new wxTextCtrl(Panel2, ID_TEXTCTRL14, wxEmptyString, wxDefaultPosition, wxSize(70,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL14"));
    BoxSizer8->Add(TextCtrl14, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer21->Add(BoxSizer8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    StaticText22 = new wxStaticText(Panel2, ID_STATICTEXT22, _("**Position:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT22"));
    BoxSizer9->Add(StaticText22, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl15 = new wxTextCtrl(Panel2, ID_TEXTCTRL15, wxEmptyString, wxDefaultPosition, wxSize(70,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL15"));
    BoxSizer9->Add(TextCtrl15, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer21->Add(BoxSizer9, 1, wxALL|wxEXPAND, 0);
    BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    StaticText23 = new wxStaticText(Panel2, ID_STATICTEXT23, _("Length:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT23"));
    BoxSizer10->Add(StaticText23, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl16 = new wxTextCtrl(Panel2, ID_TEXTCTRL16, wxEmptyString, wxDefaultPosition, wxSize(70,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL16"));
    BoxSizer10->Add(TextCtrl16, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer21->Add(BoxSizer10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer19->Add(FlexGridSizer21, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
    FlexGridSizer22 = new wxFlexGridSizer(5, 1, 0, 0);
    BoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    StaticText25 = new wxStaticText(Panel2, ID_STATICTEXT25, _("Factor:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT25"));
    BoxSizer12->Add(StaticText25, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl18 = new wxTextCtrl(Panel2, ID_TEXTCTRL18, wxEmptyString, wxDefaultPosition, wxSize(70,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL18"));
    BoxSizer12->Add(TextCtrl18, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer22->Add(BoxSizer12, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    StaticText26 = new wxStaticText(Panel2, ID_STATICTEXT26, _("Zero Offset:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT26"));
    BoxSizer13->Add(StaticText26, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl19 = new wxTextCtrl(Panel2, ID_TEXTCTRL19, wxEmptyString, wxDefaultPosition, wxSize(70,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL19"));
    BoxSizer13->Add(TextCtrl19, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer22->Add(BoxSizer13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    StaticText28 = new wxStaticText(Panel2, ID_STATICTEXT28, _("Mask:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT28"));
    BoxSizer14->Add(StaticText28, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl20 = new wxTextCtrl(Panel2, ID_TEXTCTRL20, wxEmptyString, wxDefaultPosition, wxSize(70,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL20"));
    BoxSizer14->Add(TextCtrl20, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer22->Add(BoxSizer14, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    CheckBox1 = new wxCheckBox(Panel2, ID_CHECKBOX1, _("Show leading zeroes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(false);
    BoxSizer11->Add(CheckBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer22->Add(BoxSizer11, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    FlexGridSizer19->Add(FlexGridSizer22, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    FlexGridSizer9->Add(FlexGridSizer19, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine15 = new wxStaticLine(Panel2, ID_STATICLINE15, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE15"));
    FlexGridSizer9->Add(StaticLine15, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer67 = new wxFlexGridSizer(0, 3, 0, 0);
    GridSizer8 = new wxGridSizer(2, 4, 0, 0);
    StaticText73 = new wxStaticText(Panel2, ID_STATICTEXT73, _("IndicMax"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT73"));
    GridSizer8->Add(StaticText73, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl21 = new wxTextCtrl(Panel2, ID_TEXTCTRL21, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL21"));
    GridSizer8->Add(TextCtrl21, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText29 = new wxStaticText(Panel2, ID_STATICTEXT29, _("SimMax:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT29"));
    GridSizer8->Add(StaticText29, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl46 = new wxTextCtrl(Panel2, ID_TEXTCTRL46, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL46"));
    GridSizer8->Add(TextCtrl46, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticText74 = new wxStaticText(Panel2, ID_STATICTEXT74, _("IndicMin"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT74"));
    GridSizer8->Add(StaticText74, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl65 = new wxTextCtrl(Panel2, ID_TEXTCTRL65, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL65"));
    GridSizer8->Add(TextCtrl65, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText56 = new wxStaticText(Panel2, ID_STATICTEXT56, _("SimMin:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT56"));
    GridSizer8->Add(StaticText56, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl64 = new wxTextCtrl(Panel2, ID_TEXTCTRL64, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL64"));
    GridSizer8->Add(TextCtrl64, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer67->Add(GridSizer8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    Button24 = new wxButton(Panel2, ID_BUTTON24, _("Calculate \nfactor and zero_offset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON24"));
    FlexGridSizer67->Add(Button24, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer9->Add(FlexGridSizer67, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine16 = new wxStaticLine(Panel2, ID_STATICLINE16, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE16"));
    FlexGridSizer9->Add(StaticLine16, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer71 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer72 = new wxFlexGridSizer(2, 1, 0, 0);
    StaticText7 = new wxStaticText(Panel2, ID_STATICTEXT7, _("Prepared data to send to device"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer72->Add(StaticText7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer6 = new wxGridSizer(3, 2, 0, 0);
    TextCtrl23 = new wxTextCtrl(Panel2, ID_TEXTCTRL23, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL23"));
    GridSizer6->Add(TextCtrl23, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    Button15 = new wxButton(Panel2, ID_BUTTON15, _("Value 1 >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON15"));
    GridSizer6->Add(Button15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl24 = new wxTextCtrl(Panel2, ID_TEXTCTRL24, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL24"));
    GridSizer6->Add(TextCtrl24, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    Button16 = new wxButton(Panel2, ID_BUTTON16, _("Value 2 >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON16"));
    GridSizer6->Add(Button16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl45 = new wxTextCtrl(Panel2, ID_TEXTCTRL45, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL45"));
    GridSizer6->Add(TextCtrl45, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    Button17 = new wxButton(Panel2, ID_BUTTON17, _("Value 3 >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON17"));
    GridSizer6->Add(Button17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer72->Add(GridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer71->Add(FlexGridSizer72, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer73 = new wxFlexGridSizer(4, 1, 0, 0);
    StaticText77 = new wxStaticText(Panel2, ID_STATICTEXT77, _("Data for indic. test"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT77"));
    FlexGridSizer73->Add(StaticText77, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl12 = new wxTextCtrl(Panel2, ID_TEXTCTRL12, _("0"), wxDefaultPosition, wxSize(117,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL12"));
    FlexGridSizer73->Add(TextCtrl12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer71->Add(FlexGridSizer73, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer9->Add(FlexGridSizer71, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine12 = new wxStaticLine(Panel2, ID_STATICLINE12, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE12"));
    FlexGridSizer9->Add(StaticLine12, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer84 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer86 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText115 = new wxStaticText(Panel2, ID_STATICTEXT115, _("Numeric output buffer"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT115"));
    FlexGridSizer86->Add(StaticText115, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    ComboBox4 = new wxComboBox(Panel2, ID_COMBOBOX4, wxEmptyString, wxDefaultPosition, wxSize(65,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX4"));
    ComboBox4->SetSelection( ComboBox4->Append(_("48")) );
    ComboBox4->Append(_("96"));
    ComboBox4->Append(_("144"));
    ComboBox4->Append(_("192"));
    ComboBox4->Append(_("240"));
    ComboBox4->Append(_("288"));
    ComboBox4->Append(_("336"));
    ComboBox4->Append(_("384"));
    FlexGridSizer86->Add(ComboBox4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer84->Add(FlexGridSizer86, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    Button33 = new wxButton(Panel2, ID_BUTTON33, _("Clear all"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON33"));
    FlexGridSizer84->Add(Button33, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer85 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText69 = new wxStaticText(Panel2, ID_STATICTEXT69, _("Discrete output buffer"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT69"));
    FlexGridSizer85->Add(StaticText69, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    ComboBox3 = new wxComboBox(Panel2, ID_COMBOBOX3, wxEmptyString, wxDefaultPosition, wxSize(56,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX3"));
    ComboBox3->Append(_("64"));
    ComboBox3->SetSelection( ComboBox3->Append(_("128")) );
    ComboBox3->Append(_("256"));
    ComboBox3->Append(_("384"));
    ComboBox3->Append(_("512"));
    ComboBox3->Append(_("640"));
    ComboBox3->Append(_("768"));
    ComboBox3->Append(_("896"));
    ComboBox3->Append(_("1024"));
    FlexGridSizer85->Add(ComboBox3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer84->Add(FlexGridSizer85, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer9->Add(FlexGridSizer84, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel2->SetSizer(FlexGridSizer9);
    FlexGridSizer9->Fit(Panel2);
    FlexGridSizer9->SetSizeHints(Panel2);
    Panel4 = new wxPanel(Notebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    FlexGridSizer49 = new wxFlexGridSizer(0, 3, 0, 0);
    Grid1 = new wxGrid(Panel4, ID_GRID1, wxDefaultPosition, wxSize(268,408), 0, _T("ID_GRID1"));
    Grid1->CreateGrid(16,8);
    Grid1->EnableEditing(true);
    Grid1->EnableGridLines(true);
    Grid1->SetColLabelValue(0, _("0"));
    Grid1->SetColLabelValue(1, _("1"));
    Grid1->SetColLabelValue(2, _("2"));
    Grid1->SetColLabelValue(3, _("3"));
    Grid1->SetColLabelValue(4, _("4"));
    Grid1->SetColLabelValue(5, _("5"));
    Grid1->SetColLabelValue(6, _("6"));
    Grid1->SetColLabelValue(7, _("7"));
    Grid1->SetRowLabelValue(0, _("0"));
    Grid1->SetRowLabelValue(1, _("1"));
    Grid1->SetRowLabelValue(2, _("2"));
    Grid1->SetRowLabelValue(3, _("3"));
    Grid1->SetRowLabelValue(4, _("4"));
    Grid1->SetRowLabelValue(5, _("5"));
    Grid1->SetRowLabelValue(6, _("6"));
    Grid1->SetRowLabelValue(7, _("7"));
    Grid1->SetRowLabelValue(8, _("8"));
    Grid1->SetRowLabelValue(9, _("9"));
    Grid1->SetRowLabelValue(10, _("10"));
    Grid1->SetRowLabelValue(11, _("11"));
    Grid1->SetRowLabelValue(12, _("12"));
    Grid1->SetRowLabelValue(13, _("13"));
    Grid1->SetRowLabelValue(14, _("14"));
    Grid1->SetRowLabelValue(15, _("15"));
    Grid1->SetDefaultCellFont( Grid1->GetFont() );
    Grid1->SetDefaultCellTextColour( Grid1->GetForegroundColour() );
    FlexGridSizer49->Add(Grid1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer52 = new wxFlexGridSizer(5, 1, 0, 0);
    FlexGridSizer52->Add(20,244,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button7 = new wxButton(Panel4, ID_BUTTON7, _("Clear outputs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    Button7->SetFocus();
    FlexGridSizer52->Add(Button7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer49->Add(FlexGridSizer52, 1, wxALL|wxEXPAND, 5);
    Panel4->SetSizer(FlexGridSizer49);
    FlexGridSizer49->Fit(Panel4);
    FlexGridSizer49->SetSizeHints(Panel4);
    Panel5 = new wxPanel(Notebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    FlexGridSizer50 = new wxFlexGridSizer(0, 3, 0, 0);
    Grid2 = new wxGrid(Panel5, ID_GRID2, wxDefaultPosition, wxSize(260,363), 0, _T("ID_GRID2"));
    Grid2->CreateGrid(7,10);
    Grid2->EnableEditing(true);
    Grid2->EnableGridLines(true);
    Grid2->SetColLabelValue(0, _("0"));
    Grid2->SetColLabelValue(1, _("1"));
    Grid2->SetColLabelValue(2, _("2"));
    Grid2->SetColLabelValue(3, _("3"));
    Grid2->SetColLabelValue(4, _("4"));
    Grid2->SetColLabelValue(5, _("5"));
    Grid2->SetColLabelValue(6, _("6"));
    Grid2->SetColLabelValue(7, _("7"));
    Grid2->SetColLabelValue(8, _("8"));
    Grid2->SetColLabelValue(9, _("9"));
    Grid2->SetRowLabelValue(0, _("0"));
    Grid2->SetRowLabelValue(1, _("10"));
    Grid2->SetRowLabelValue(2, _("20"));
    Grid2->SetRowLabelValue(3, _("30"));
    Grid2->SetRowLabelValue(4, _("40"));
    Grid2->SetRowLabelValue(5, _("50"));
    Grid2->SetRowLabelValue(6, _("60"));
    Grid2->SetDefaultCellFont( Grid2->GetFont() );
    Grid2->SetDefaultCellTextColour( Grid2->GetForegroundColour() );
    FlexGridSizer50->Add(Grid2, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer53 = new wxFlexGridSizer(5, 1, 0, 0);
    FlexGridSizer53->Add(20,246,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button11 = new wxButton(Panel5, ID_BUTTON11, _("Clear outputs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON11"));
    Button11->SetFocus();
    FlexGridSizer53->Add(Button11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer50->Add(FlexGridSizer53, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel5->SetSizer(FlexGridSizer50);
    FlexGridSizer50->Fit(Panel5);
    FlexGridSizer50->SetSizeHints(Panel5);
    Panel3 = new wxPanel(Notebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    FlexGridSizer6 = new wxFlexGridSizer(7, 1, 0, 0);
    GridSizer1 = new wxGridSizer(1, 4, 0, 0);
    FlexGridSizer28 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText35 = new wxStaticText(Panel3, ID_STATICTEXT35, _(" 0:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT35"));
    FlexGridSizer28->Add(StaticText35, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl30 = new wxTextCtrl(Panel3, ID_TEXTCTRL30, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL30"));
    FlexGridSizer28->Add(TextCtrl30, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(FlexGridSizer28, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer29 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText33 = new wxStaticText(Panel3, ID_STATICTEXT33, _(" 1:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT33"));
    FlexGridSizer29->Add(StaticText33, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl26 = new wxTextCtrl(Panel3, ID_TEXTCTRL26, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL26"));
    FlexGridSizer29->Add(TextCtrl26, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(FlexGridSizer29, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer30 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText36 = new wxStaticText(Panel3, ID_STATICTEXT36, _(" 2:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT36"));
    FlexGridSizer30->Add(StaticText36, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl27 = new wxTextCtrl(Panel3, ID_TEXTCTRL27, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL27"));
    FlexGridSizer30->Add(TextCtrl27, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(FlexGridSizer30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer31 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText37 = new wxStaticText(Panel3, ID_STATICTEXT37, _(" 3:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT37"));
    FlexGridSizer31->Add(StaticText37, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl28 = new wxTextCtrl(Panel3, ID_TEXTCTRL28, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL28"));
    FlexGridSizer31->Add(TextCtrl28, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(FlexGridSizer31, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer6->Add(GridSizer1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    GridSizer2 = new wxGridSizer(1, 4, 0, 0);
    FlexGridSizer32 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText38 = new wxStaticText(Panel3, ID_STATICTEXT38, _(" 4:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT38"));
    FlexGridSizer32->Add(StaticText38, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl29 = new wxTextCtrl(Panel3, ID_TEXTCTRL29, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL29"));
    FlexGridSizer32->Add(TextCtrl29, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2->Add(FlexGridSizer32, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer33 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText39 = new wxStaticText(Panel3, ID_STATICTEXT39, _(" 5:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT39"));
    FlexGridSizer33->Add(StaticText39, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl31 = new wxTextCtrl(Panel3, ID_TEXTCTRL31, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL31"));
    FlexGridSizer33->Add(TextCtrl31, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2->Add(FlexGridSizer33, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer34 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText40 = new wxStaticText(Panel3, ID_STATICTEXT40, _(" 6:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT40"));
    FlexGridSizer34->Add(StaticText40, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl32 = new wxTextCtrl(Panel3, ID_TEXTCTRL32, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL32"));
    FlexGridSizer34->Add(TextCtrl32, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2->Add(FlexGridSizer34, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer35 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText41 = new wxStaticText(Panel3, ID_STATICTEXT41, _(" 7:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT41"));
    FlexGridSizer35->Add(StaticText41, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl33 = new wxTextCtrl(Panel3, ID_TEXTCTRL33, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL33"));
    FlexGridSizer35->Add(TextCtrl33, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2->Add(FlexGridSizer35, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer6->Add(GridSizer2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    GridSizer3 = new wxGridSizer(1, 4, 0, 0);
    FlexGridSizer36 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText42 = new wxStaticText(Panel3, ID_STATICTEXT42, _(" 8:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT42"));
    FlexGridSizer36->Add(StaticText42, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl34 = new wxTextCtrl(Panel3, ID_TEXTCTRL34, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL34"));
    FlexGridSizer36->Add(TextCtrl34, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer3->Add(FlexGridSizer36, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer37 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText43 = new wxStaticText(Panel3, ID_STATICTEXT43, _("9:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT43"));
    FlexGridSizer37->Add(StaticText43, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl35 = new wxTextCtrl(Panel3, ID_TEXTCTRL35, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL35"));
    FlexGridSizer37->Add(TextCtrl35, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer3->Add(FlexGridSizer37, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer38 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText44 = new wxStaticText(Panel3, ID_STATICTEXT44, _("10:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT44"));
    FlexGridSizer38->Add(StaticText44, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl36 = new wxTextCtrl(Panel3, ID_TEXTCTRL36, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL36"));
    FlexGridSizer38->Add(TextCtrl36, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer3->Add(FlexGridSizer38, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer39 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText45 = new wxStaticText(Panel3, ID_STATICTEXT45, _("11:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT45"));
    FlexGridSizer39->Add(StaticText45, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl37 = new wxTextCtrl(Panel3, ID_TEXTCTRL37, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL37"));
    FlexGridSizer39->Add(TextCtrl37, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer3->Add(FlexGridSizer39, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer6->Add(GridSizer3, 1, wxTOP|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 7);
    GridSizer4 = new wxGridSizer(1, 4, 0, 0);
    FlexGridSizer40 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText46 = new wxStaticText(Panel3, ID_STATICTEXT46, _("12:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT46"));
    FlexGridSizer40->Add(StaticText46, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl38 = new wxTextCtrl(Panel3, ID_TEXTCTRL38, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL38"));
    FlexGridSizer40->Add(TextCtrl38, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer4->Add(FlexGridSizer40, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer41 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText47 = new wxStaticText(Panel3, ID_STATICTEXT47, _("13:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT47"));
    FlexGridSizer41->Add(StaticText47, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl39 = new wxTextCtrl(Panel3, ID_TEXTCTRL39, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL39"));
    FlexGridSizer41->Add(TextCtrl39, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer4->Add(FlexGridSizer41, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer42 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText48 = new wxStaticText(Panel3, ID_STATICTEXT48, _("14:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT48"));
    FlexGridSizer42->Add(StaticText48, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl40 = new wxTextCtrl(Panel3, ID_TEXTCTRL40, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL40"));
    FlexGridSizer42->Add(TextCtrl40, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer4->Add(FlexGridSizer42, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer43 = new wxFlexGridSizer(1, 2, 0, 0);
    StaticText49 = new wxStaticText(Panel3, ID_STATICTEXT49, _("15:"), wxDefaultPosition, wxSize(20,-1), wxALIGN_RIGHT, _T("ID_STATICTEXT49"));
    FlexGridSizer43->Add(StaticText49, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl41 = new wxTextCtrl(Panel3, ID_TEXTCTRL41, _("12345"), wxDefaultPosition, wxSize(55,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL41"));
    FlexGridSizer43->Add(TextCtrl41, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer4->Add(FlexGridSizer43, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer6->Add(GridSizer4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer44 = new wxFlexGridSizer(8, 1, 0, 0);
    FlexGridSizer46 = new wxFlexGridSizer(1, 5, 0, 0);
    Choice6 = new wxChoice(Panel3, ID_CHOICE6, wxDefaultPosition, wxSize(103,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE6"));
    Choice6->Append(wxEmptyString);
    Choice6->SetSelection( Choice6->Append(_("Dataref int")) );
    Choice6->Append(_("Dataref float"));
    Choice6->Append(_("Array int"));
    Choice6->Append(_("Array float"));
    FlexGridSizer46->Add(Choice6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText52 = new wxStaticText(Panel3, ID_STATICTEXT52, _("Dataref type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT52"));
    FlexGridSizer46->Add(StaticText52, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
    FlexGridSizer46->Add(45,9,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Choice5 = new wxChoice(Panel3, ID_CHOICE5, wxDefaultPosition, wxSize(48,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE5"));
    Choice5->SetSelection( Choice5->Append(_("0")) );
    Choice5->Append(_("1"));
    Choice5->Append(_("2"));
    Choice5->Append(_("3"));
    Choice5->Append(_("4"));
    Choice5->Append(_("5"));
    Choice5->Append(_("6"));
    Choice5->Append(_("7"));
    Choice5->Append(_("8"));
    Choice5->Append(_("9"));
    Choice5->Append(_("10"));
    Choice5->Append(_("11"));
    Choice5->Append(_("12"));
    Choice5->Append(_("13"));
    Choice5->Append(_("14"));
    Choice5->Append(_("15"));
    Choice5->SetFocus();
    FlexGridSizer46->Add(Choice5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText50 = new wxStaticText(Panel3, ID_STATICTEXT50, _("Axis"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT50"));
    FlexGridSizer46->Add(StaticText50, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer44->Add(FlexGridSizer46, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    StaticText53 = new wxStaticText(Panel3, ID_STATICTEXT53, _("Dataref"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT53"));
    FlexGridSizer44->Add(StaticText53, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl42 = new wxTextCtrl(Panel3, ID_TEXTCTRL42, wxEmptyString, wxDefaultPosition, wxSize(359,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL42"));
    FlexGridSizer44->Add(TextCtrl42, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer47 = new wxFlexGridSizer(0, 5, 0, 0);
    StaticText54 = new wxStaticText(Panel3, ID_STATICTEXT54, _("[section name]"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT54"));
    FlexGridSizer47->Add(StaticText54, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 6);
    TextCtrl43 = new wxTextCtrl(Panel3, ID_TEXTCTRL43, wxEmptyString, wxDefaultPosition, wxSize(129,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL43"));
    FlexGridSizer47->Add(TextCtrl43, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 4);
    FlexGridSizer47->Add(41,7,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText59 = new wxStaticText(Panel3, ID_STATICTEXT59, _("Index:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT59"));
    FlexGridSizer47->Add(StaticText59, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl48 = new wxTextCtrl(Panel3, ID_TEXTCTRL48, wxEmptyString, wxDefaultPosition, wxSize(27,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL48"));
    TextCtrl48->Disable();
    FlexGridSizer47->Add(TextCtrl48, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer44->Add(FlexGridSizer47, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer6->Add(FlexGridSizer44, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer45 = new wxFlexGridSizer(1, 3, 0, 0);
    FlexGridSizer45->Add(8,20,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer5 = new wxGridSizer(3, 4, 0, 0);
    Button12 = new wxButton(Panel3, ID_BUTTON12, _("DeviceMax >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON12"));
    GridSizer5->Add(Button12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl44 = new wxTextCtrl(Panel3, ID_TEXTCTRL44, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL44"));
    GridSizer5->Add(TextCtrl44, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText65 = new wxStaticText(Panel3, ID_STATICTEXT65, _("SimMax:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT65"));
    GridSizer5->Add(StaticText65, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl54 = new wxTextCtrl(Panel3, ID_TEXTCTRL54, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL54"));
    GridSizer5->Add(TextCtrl54, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    Button13 = new wxButton(Panel3, ID_BUTTON13, _("DevCenter >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON13"));
    Button13->Disable();
    GridSizer5->Add(Button13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl52 = new wxTextCtrl(Panel3, ID_TEXTCTRL52, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL52"));
    TextCtrl52->Disable();
    GridSizer5->Add(TextCtrl52, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText66 = new wxStaticText(Panel3, ID_STATICTEXT66, _("SimMin:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT66"));
    GridSizer5->Add(StaticText66, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl55 = new wxTextCtrl(Panel3, ID_TEXTCTRL55, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL55"));
    GridSizer5->Add(TextCtrl55, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    Button14 = new wxButton(Panel3, ID_BUTTON14, _("DeviceMin >"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON14"));
    GridSizer5->Add(Button14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    TextCtrl53 = new wxTextCtrl(Panel3, ID_TEXTCTRL53, wxEmptyString, wxDefaultPosition, wxSize(50,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL53"));
    GridSizer5->Add(TextCtrl53, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    StaticText67 = new wxStaticText(Panel3, ID_STATICTEXT67, _("Has center:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT67"));
    GridSizer5->Add(StaticText67, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
    CheckBox3 = new wxCheckBox(Panel3, ID_CHECKBOX3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBox3->SetValue(false);
    GridSizer5->Add(CheckBox3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer45->Add(GridSizer5, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    FlexGridSizer6->Add(FlexGridSizer45, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    Button10 = new wxButton(Panel3, ID_BUTTON10, _("Add axis section"), wxDefaultPosition, wxSize(148,32), 0, wxDefaultValidator, _T("ID_BUTTON10"));
    Button10->Disable();
    FlexGridSizer6->Add(Button10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel3->SetSizer(FlexGridSizer6);
    FlexGridSizer6->Fit(Panel3);
    FlexGridSizer6->SetSizeHints(Panel3);
    Notebook1->AddPage(Panel1, _("DiscrCtrls"), false);
    Notebook1->AddPage(Panel6, _("NumCtrls"), false);
    Notebook1->AddPage(Panel2, _("Indication"), false);
    Notebook1->AddPage(Panel4, _("AlphNum Indic"), false);
    Notebook1->AddPage(Panel5, _("Discrete Indic"), false);
    Notebook1->AddPage(Panel3, _("Axes"), false);
    FlexGridSizer3->Add(Notebook1, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL, _T("ID_STATICLINE1"));
    FlexGridSizer2->Add(StaticLine1, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer15 = new wxFlexGridSizer(11, 1, 0, 0);
    FlexGridSizer26 = new wxFlexGridSizer(40, 1, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Device"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    wxFont StaticText1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    StaticText1->SetFont(StaticText1Font);
    FlexGridSizer26->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    TextCtrl22 = new wxTextCtrl(this, ID_TEXTCTRL22, wxEmptyString, wxDefaultPosition, wxSize(32,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL22"));
    FlexGridSizer26->Add(TextCtrl22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Disconnected"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    wxFont StaticText2Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    StaticText2->SetFont(StaticText2Font);
    FlexGridSizer26->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine7 = new wxStaticLine(this, ID_STATICLINE7, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE7"));
    FlexGridSizer26->Add(StaticLine7, 1, wxALL|wxEXPAND, 0);
    StaticText24 = new wxStaticText(this, ID_STATICTEXT24, _("Serial port"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
    wxFont StaticText24Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    StaticText24->SetFont(StaticText24Font);
    FlexGridSizer26->Add(StaticText24, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button4 = new wxButton(this, ID_BUTTON4, _("Refresh list"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    FlexGridSizer26->Add(Button4, 1, wxALL|wxEXPAND, 1);
    ComboBox1 = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxSize(69,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    FlexGridSizer26->Add(ComboBox1, 1, wxALL|wxEXPAND, 1);
    StaticText51 = new wxStaticText(this, ID_STATICTEXT51, _("Baudrate"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT51"));
    FlexGridSizer26->Add(StaticText51, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    ComboBox2 = new wxComboBox(this, ID_COMBOBOX2, wxEmptyString, wxDefaultPosition, wxSize(79,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX2"));
    ComboBox2->Append(_("9600"));
    ComboBox2->Append(_("19200"));
    ComboBox2->Append(_("38400"));
    ComboBox2->Append(_("57600"));
    ComboBox2->SetSelection( ComboBox2->Append(_("115200")) );
    ComboBox2->Disable();
    FlexGridSizer26->Add(ComboBox2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button1 = new wxButton(this, ID_BUTTON1, _("Open"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer26->Add(Button1, 1, wxALL|wxEXPAND, 1);
    Button8 = new wxButton(this, ID_BUTTON8, _("Reset (Arduino)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    FlexGridSizer26->Add(Button8, 1, wxALL|wxEXPAND, 1);
    Button2 = new wxButton(this, ID_BUTTON2, _("Close"), wxDefaultPosition, wxSize(62,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button2->Disable();
    FlexGridSizer26->Add(Button2, 1, wxALL|wxEXPAND, 1);
    FlexGridSizer26->Add(91,4,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine8 = new wxStaticLine(this, ID_STATICLINE8, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE8"));
    FlexGridSizer26->Add(StaticLine8, 1, wxALL|wxEXPAND, 0);
    StaticText55 = new wxStaticText(this, ID_STATICTEXT55, _("UDP server"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT55"));
    wxFont StaticText55Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    StaticText55->SetFont(StaticText55Font);
    FlexGridSizer26->Add(StaticText55, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer55 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText32 = new wxStaticText(this, ID_STATICTEXT32, _("UDP port"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT32"));
    wxFont StaticText32Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    StaticText32->SetFont(StaticText32Font);
    FlexGridSizer55->Add(StaticText32, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl47 = new wxTextCtrl(this, ID_TEXTCTRL47, _("50000"), wxDefaultPosition, wxSize(47,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL47"));
    FlexGridSizer55->Add(TextCtrl47, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer26->Add(FlexGridSizer55, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer59 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText70 = new wxStaticText(this, ID_STATICTEXT70, _("ID filter"), wxDefaultPosition, wxSize(54,16), 0, _T("ID_STATICTEXT70"));
    wxFont StaticText70Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    StaticText70->SetFont(StaticText70Font);
    FlexGridSizer59->Add(StaticText70, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl63 = new wxTextCtrl(this, ID_TEXTCTRL63, _("0"), wxDefaultPosition, wxSize(47,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL63"));
    FlexGridSizer59->Add(TextCtrl63, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer26->Add(FlexGridSizer59, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer58 = new wxFlexGridSizer(0, 3, 0, 0);
    Button22 = new wxButton(this, ID_BUTTON22, _("Start"), wxDefaultPosition, wxSize(52,23), 0, wxDefaultValidator, _T("ID_BUTTON22"));
    FlexGridSizer58->Add(Button22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button23 = new wxButton(this, ID_BUTTON23, _("Stop"), wxDefaultPosition, wxSize(49,23), 0, wxDefaultValidator, _T("ID_BUTTON23"));
    FlexGridSizer58->Add(Button23, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer26->Add(FlexGridSizer58, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    StaticLine9 = new wxStaticLine(this, ID_STATICLINE9, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE9"));
    FlexGridSizer26->Add(StaticLine9, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer26->Add(91,9,1, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine10 = new wxStaticLine(this, ID_STATICLINE10, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE10"));
    FlexGridSizer26->Add(StaticLine10, 1, wxALL|wxEXPAND, 0);
    StaticText30 = new wxStaticText(this, ID_STATICTEXT30, _("Console"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT30"));
    wxFont StaticText30Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    StaticText30->SetFont(StaticText30Font);
    FlexGridSizer26->Add(StaticText30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    Button9 = new wxButton(this, ID_BUTTON9, _("Console ON/OFF"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    FlexGridSizer26->Add(Button9, 1, wxALL|wxEXPAND, 5);
    CheckBox2 = new wxCheckBox(this, ID_CHECKBOX2, _("Inputs only"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBox2->SetValue(false);
    CheckBox2->Disable();
    FlexGridSizer26->Add(CheckBox2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine6 = new wxStaticLine(this, ID_STATICLINE6, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE6"));
    FlexGridSizer26->Add(StaticLine6, 1, wxTOP|wxBOTTOM|wxEXPAND, 4);
    FlexGridSizer26->Add(91,8,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button3 = new wxButton(this, ID_BUTTON3, _("Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    FlexGridSizer26->Add(Button3, 1, wxALL|wxEXPAND, 1);
    FlexGridSizer15->Add(FlexGridSizer26, 1, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
    FlexGridSizer2->Add(FlexGridSizer15, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 0);
    SetSizer(FlexGridSizer1);
    Timer1.SetOwner(this, ID_TIMER1);
    Timer1.Start(20, false);
    Timer2.SetOwner(this, ID_TIMER2);
    Timer2.Start(999999, true);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_CHOICE2,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice2Select);
    Connect(ID_BUTTON31,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton31Click);
    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice1Select1);
    Connect(ID_CHOICE8,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice1Select1);
    Connect(ID_CHOICE11,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice1Select1);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton5Click);
    Connect(ID_BUTTON26,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton26Click);
    Connect(ID_BUTTON27,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton27Click);
    Connect(ID_BUTTON28,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton28Click);
    Connect(ID_BUTTON29,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton29Click);
    Connect(ID_BUTTON30,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton30Click);
    Connect(ID_BUTTON32,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton32Click);
    Connect(ID_CHOICE9,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice1Select1);
    Connect(ID_CHOICE12,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice1Select1);
    Connect(ID_BUTTON21,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton21Click);
    Connect(ID_BUTTON18,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton18Click);
    Connect(ID_BUTTON19,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton19Click);
    Connect(ID_BUTTON20,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton20Click);
    Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&loggerDialog::OnCheckBox4Click);
    Connect(ID_BUTTON25,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton25Click);
    Panel6->Connect(wxEVT_PAINT,(wxObjectEventFunction)&loggerDialog::OnPanel6Paint,0,this);
    Panel6->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&loggerDialog::OnPanel6MouseEnter,0,this);
    Connect(ID_CHOICE4,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice4Select);
    Connect(ID_CHOICE3,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice1Select1);
    Connect(ID_CHOICE13,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice1Select1);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton6Click);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&loggerDialog::OnCheckBox1Click);
    Connect(ID_BUTTON24,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton24Click);
    Connect(ID_BUTTON15,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton15Click);
    Connect(ID_BUTTON16,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton16Click);
    Connect(ID_BUTTON17,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton17Click);
    Connect(ID_COMBOBOX4,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&loggerDialog::OnComboBox4Selected);
    Connect(ID_BUTTON33,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton33Click);
    Connect(ID_COMBOBOX3,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&loggerDialog::OnComboBox3Selected);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton7Click);
    Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton7Click);
    Connect(ID_CHOICE6,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice6Select);
    Connect(ID_CHOICE5,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&loggerDialog::OnChoice5Select);
    Connect(ID_BUTTON12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton12Click);
    Connect(ID_BUTTON13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton13Click);
    Connect(ID_BUTTON14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton14Click);
    Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&loggerDialog::OnCheckBox3Click);
    Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton10Click2);
    Connect(ID_NOTEBOOK1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&loggerDialog::OnNotebook1PageChanged);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton4Click);
    Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&loggerDialog::OnComboBox1Selected);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton1Click);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton8Click);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton2Click);
    Connect(ID_BUTTON22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton22Click);
    Connect(ID_BUTTON23,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton23Click);
    Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton9Click1);
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&loggerDialog::OnCheckBox2Click);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&loggerDialog::OnButton3Click);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&loggerDialog::OnTimer1Trigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&loggerDialog::OnTimer2Trigger);
    //*)

    axes[0] = TextCtrl30;
    axes[1] = TextCtrl26;
    axes[2] = TextCtrl27;
    axes[3] = TextCtrl28;
    axes[4] = TextCtrl29;
    axes[5] = TextCtrl31;
    axes[6] = TextCtrl32;
    axes[7] = TextCtrl33;
    axes[8] = TextCtrl34;
    axes[9] = TextCtrl35;
    axes[10] = TextCtrl36;
    axes[11] = TextCtrl37;
    axes[12] = TextCtrl38;
    axes[13] = TextCtrl39;
    axes[14] = TextCtrl40;
    axes[15] = TextCtrl41;

    mask32[0] = CheckBox36;  mask32[8] = CheckBox28;   mask32[16] = CheckBox20;  mask32[24] = CheckBox12;
    mask32[1] = CheckBox35;  mask32[9] = CheckBox27;   mask32[17] = CheckBox19;  mask32[25] = CheckBox11;
    mask32[2] = CheckBox34;  mask32[10] = CheckBox26;  mask32[18] = CheckBox18;  mask32[26] = CheckBox10;
    mask32[3] = CheckBox33;  mask32[11] = CheckBox25;  mask32[19] = CheckBox17;  mask32[27] = CheckBox9;
    mask32[4] = CheckBox32;  mask32[12] = CheckBox24;  mask32[20] = CheckBox16;  mask32[28] = CheckBox8;
    mask32[5] = CheckBox31;  mask32[13] = CheckBox23;  mask32[21] = CheckBox15;  mask32[29] = CheckBox7;
    mask32[6] = CheckBox30;  mask32[14] = CheckBox22;  mask32[22] = CheckBox14;  mask32[30] = CheckBox6;
    mask32[7] = CheckBox29;  mask32[15] = CheckBox21;  mask32[23] = CheckBox13;  mask32[31] = CheckBox5;

    SetTitle("FSIOHUB Config Helper 0.4.0");
    StaticText2->SetForegroundColour(clDarkYellow);
    connection_flag = 0;
    connection_timer = 0;
    StaticText2->SetLabel("Disconnected");
    LoadPortsList();
    get_config();
    logger_dialog = this;
    controls_params_selector();
    indication_params_selector();
    memset(display_array,' ',sizeof(display_array));
    memset(led_array,'0',sizeof(led_array));
    Button8->Enable(false);
    Timer2.Stop();
    console_flag = 0;
    Choice5->SetSelection(0);

    Grid1->AutoSizeColumns(true);
    Grid2->AutoSizeColumns(true);

    wxFont fnt;
    fnt.MakeBold();
    axes[0]->SetForegroundColour(wxColor(255,255,255));
    axes[0]->SetBackgroundColour(wxColor(0,0,0));
    axes[0]->SetFont(fnt);
    Notebook1->Refresh();
    udp_server.SetCallback((void*)udp_call_back);
}

loggerDialog::~loggerDialog()
{
    //(*Destroy(loggerDialog)
    //*)
}


int loggerDialog::get_config(void)
{
    wxConfig *config = new wxConfig(__CONFIG_NAME__);
    wxString tmp;

    tmp = config->Read("SerialPort", "Select port");
    ComboBox1->SetValue(tmp);

    tmp = config->Read("BaudRate", "115200");
    ComboBox2->SetValue(tmp);

    delete config;
}


int loggerDialog::put_config(void)
{
    wxString tmp;
    wxConfig *config = new wxConfig(__CONFIG_NAME__);
    config->Write("SerialPort", ComboBox1->GetValue());
    config->Write("BaudRate", ComboBox2->GetValue());
    delete config;
}

int loggerDialog::GetMaxLen(wxTextCtrl *ed)
{
    int len_max = 0,
        i,
        cur_len,
        lines_number = ed->GetNumberOfLines();

    for(i = 0; i < lines_number; i++)
    {
        cur_len = ed->GetLineText(i).Len();
        if(cur_len > len_max)
            len_max = cur_len;

    }

    return len_max;
}


void loggerDialog::OnQuit(wxCommandEvent& event)
{
    put_config();
    serial_port.Close();
    Close();
}

void loggerDialog::OnAbout(wxCommandEvent& event)
{
    //s.SetCallback((void*)rx_call_back);
    //s.Open("COM10",115200);
    //wxString msg = wxbuildinfo(long_f);
    //wxMessageBox(msg, _("Welcome to..."));
}

void loggerDialog::OnButton3Click(wxCommandEvent& event)
{
    if(timer_enabled)
    {
        put_config();
        serial_port.Close();
        printf("Port %s closed\n",(char*)ComboBox1->GetValue().c_str().AsChar());
    }

    printf("Program closed\n");
    Close();
}

void loggerDialog::LoadPortsList(void)
{
    char buf[2048];
    char cur_line[100],*p,*s;
    int cnt,i,line_cnt;
    wxString tmp;
    ComboBox1->Clear();
    cnt = get_port_list(buf);
    if(cnt)
    {
        p = buf;
        s = cur_line;
        for(i=0; i<strlen(buf); i++)
        {
            if(*p == '\r')
            {
                continue;
            }
            else if(*p == '\n')
            {
                *s = 0;
                tmp.Printf("%s",cur_line);
                ComboBox1->Append(_(tmp));
                s = cur_line;
            }
            else
                *s++ = *p;

            p++;
        }

    }
}


void loggerDialog::OnChoice1Select(wxCommandEvent& event)
{
    //get_port_list(NULL);
}

void loggerDialog::OnButton4Click(wxCommandEvent& event)
{
    LoadPortsList();
}

void loggerDialog::OnButton1Click(wxCommandEvent& event)
{
    StaticText2->SetForegroundColour(clDarkYellow);
    StaticText2->SetLabel("Disconnected");
    serial_port.SetCallback((void*)rx_call_back);
    long baudrate;
    ComboBox2->GetValue().ToLong(&baudrate);
    if(serial_port.Open(ComboBox1->GetValue().c_str(),baudrate))
    {
        timer_enabled = 50;
        put_config();
        Button1->Enable(false);
        Button2->Enable(true);
        Button8->Enable(true);
        ComboBox1->Enable(false);
        set_text_color(BRIGHT_GREEN);
        printf("Port %s opened at %d\n",(char*)ComboBox1->GetValue().c_str().AsChar(),baudrate);
        set_text_color(WHITE);
    }
    else
    {
        set_text_color(BRIGHT_RED);
        printf("Could not open port %s\n",(char*)ComboBox1->GetValue().c_str().AsChar());
        set_text_color(WHITE);
    }



}

void loggerDialog::OnButton2Click(wxCommandEvent& event)
{
    StaticText2->SetForegroundColour(clDarkYellow);
    StaticText2->SetLabel("Disconnected");
    timer_enabled = 0;
    Button2->Enable(false);
    Button1->Enable(true);
    Button8->Enable(false);
    ComboBox1->Enable(true);

    serial_port.Close();
    printf("Port %s closed\n",(char*)ComboBox1->GetValue().c_str().AsChar());

}

static unsigned char nibble(int hex)
{
    if(hex>='0' && hex<='9')
        return (hex - '0');
    if(hex>='A' && hex<='F')
        return (hex - 'A' + 10);
    if(hex>='a' && hex<='f')
        return (hex - 'a' + 10);
    return 0;
}

static int hex2int(char *hh)
{
    int nib1 = nibble(hh[0]&0xFF);
    int nib2 = nibble(hh[1]&0xFF);
    return  (nib1 << 4) + nib2;
}

int part_line_to_int(char *buf, int len)
{
    int val;
    char tmp_buf[100];
    memcpy(tmp_buf, buf, len);
    tmp_buf[len] = 0;
    sscanf(tmp_buf,"%x",&val);
    return val;
}


int udp_call_back(char *rxbuf, int rxlen)
{
    static char app_buf[2000];
    char tmp_buf[100];
    static int current_ndx = 0;
    long rport_id,fport_id, ev_filter;
    int ev_id;
    int ev_type;
    int value;
    wxString tmp_string;
    int i;
    for(i=0; i<rxlen; i++)
    {
        switch(rxbuf[i])
        {

        case '!':
            if(!logger_dialog->CheckBox2->IsChecked())
            {
                printf("\nACK\n");
            }
            break;

        case '{':
            current_ndx = 0;

            break;

        case '}':
            //финализация работы с пакетом
            app_buf[current_ndx++] = 0;
            rport_id = part_line_to_int(app_buf, 2);

            logger_dialog->TextCtrl63->GetValue().ToLong(&fport_id);

            if(rport_id == fport_id)
            {
                tmp_string.Printf("%d",rport_id);
                logger_dialog->TextCtrl22->SetValue(tmp_string);
                memcpy(&(logger_dialog->udp_server.addr_by_id),
                       &(logger_dialog->udp_server.addr),
                       sizeof(logger_dialog->udp_server.addr)
                      );

                switch(strlen(&app_buf[2]))
                {
                /* 234 */
                case 3: /* 801 */

                    ev_id = part_line_to_int(app_buf+3, 2);
                    ev_type = part_line_to_int(app_buf+2, 1);
                    value = 0;
                    break;
                /* 2345 */
                case 4: /* 8001 */
                    ev_id = part_line_to_int(app_buf+3, 3);
                    ev_type = part_line_to_int(app_buf+2, 1);
                    value = 0;

                    break;
                /* 23456 */
                case 5: /* 80001*/
                    ev_id = part_line_to_int(app_buf+3, 4);
                    ev_type = part_line_to_int(app_buf+2, 1);
                    value = 0;
                    break;
                /* 2345678 */
                case 7: /* A01ABCD*/
                    ev_id = part_line_to_int(app_buf+3, 2);
                    ev_type = part_line_to_int(app_buf+2, 1);
                    value = part_line_to_int(app_buf+5, 4);
                    break;
                /* 23456789 */
                case 8: /* A001ABCD*/
                    ev_id = part_line_to_int(app_buf+3, 3);
                    ev_type = part_line_to_int(app_buf+2, 1);
                    value = part_line_to_int(app_buf+6, 4);
                    break;

                /* 234567890 */
                case 9: /* A0001ABCD*/
                    ev_id = part_line_to_int(app_buf+3, 4);
                    ev_type = part_line_to_int(app_buf+2, 1);
                    value = part_line_to_int(app_buf+7, 4);
                    break;
                }


                switch(current_panel)
                {
                case 0:
                    if(ev_type != 0x0A)
                    {
                        set_text_color(BRIGHT_YELLOW);
                        printf("{%s}  %d\n",app_buf,ev_id);
                        set_text_color(WHITE);

                        tmp_string.Printf("%d",ev_id);
                        logger_dialog->TextCtrl10->SetValue(tmp_string);
                        tmp_string.Printf("{%s}",app_buf);
                        logger_dialog->TextCtrl25->SetValue(tmp_string);
                    }
                    break;

                case 1:
                    if(ev_type == 0x0A)
                    {
                        set_text_color(BRIGHT_YELLOW);
                        printf("{%s}  %d   %d\n",app_buf, ev_id, value);
                        set_text_color(WHITE);

                        logger_dialog->TextCtrl61->GetValue().ToLong(&ev_filter);
                        tmp_string.Printf("%d",ev_id);
                        logger_dialog->TextCtrl60->SetValue(tmp_string);
                        tmp_string.Printf("{%s}",app_buf);
                        logger_dialog->TextCtrl59->SetValue(tmp_string);

                        if(logger_dialog->TextCtrl61->GetValue().Len() > 0)
                        {
                            if(ev_id == ev_filter)
                            {
                                tmp_string.Printf("%d",value);
                                logger_dialog->TextCtrl62->SetValue(tmp_string);
                            }
                            else
                            {
                                logger_dialog->TextCtrl62->SetValue("-----");
                            }

                        }
                        else
                        {
                            tmp_string.Printf("%d",value);
                            logger_dialog->TextCtrl62->SetValue(tmp_string);
                        }
                    }

                    break;

                case 2:
                    break;

                case 3:
                    break;

                case 4:
                    break;

                case 5:
                    break;

                default:
                    if(ev_type == 0xFF)
                    {
                        tmp_string.Printf("%d",ev_id);
                        logger_dialog->TextCtrl22->SetValue(tmp_string);

                        if(!logger_dialog->CheckBox2->IsChecked())
                        {
                            set_text_color(BRIGHT_MAGENTA);
                            printf("Device id: {%s}\n",app_buf);
                            set_text_color(WHITE);
                        }

                    }

                }

            }



            break;

        default:
            if(current_ndx<1000)
            {
                app_buf[current_ndx++] = rxbuf[i];
            }

        }

    }
    //logger_dialog->udp_server.Send(rxbuf, rxlen);

    return 0;
}

unsigned long event2hex(const char *in)
{
unsigned long result  = 0;
  sscanf(in,"%X",&result);
  return result;
}

int rx_call_back(char *rxbuf, int rxlen)
{
    static char app_buf[2000];
    //char id_txt[8], val_txt[8];
    static int current_ndx = 0;
    //int dev,id,val;
    long ev_filter;
    wxString tmp;
    int pars_cnt;
    char tmpstr[10];
    unsigned long event;
    ULONG64 ev_container;
    EVENT_STRUCT *ev = (EVENT_STRUCT*)&ev_container;


#ifdef PARANOIC_TEST
    printf("C");
#endif
    int i;
    for(i=0; i<rxlen; i++)
    {
        if(rxlen)
        {
            for(i=0; i<rxlen; i++)
            {

                switch(rxbuf[i])
                {

                case '!':
                    if(!logger_dialog->CheckBox2->IsChecked())
                    {
                        printf("\nACK\n");
                    }
                    break;

                case '{':
                    current_ndx = 0;

                    break;

                case '}':
#if 0
                 {
                    //финализация работы с пакетом
                    app_buf[current_ndx++] = 0;
                    if(app_buf[0] == 'A' && app_buf[1] == 'X')
                    {
                        if(current_panel != 3)
                            break;
                        set_text_color(BRIGHT_GREEN);
                        printf("{%s}\n",app_buf);
                        set_text_color(WHITE);
                        char *p = app_buf + 2;
                        for(pars_cnt = 0; pars_cnt<16; pars_cnt++)
                        {
                            memcpy(tmpstr,p,5);
                            tmpstr[5] = 0;
                            tmp.Printf("%s",tmpstr);
                            p+= 5;
                            axes[pars_cnt]->SetValue(tmp);

                        }
                        break;
                    }

                    logger_dialog->StaticText2->SetForegroundColour(clDarkGreen);
                    logger_dialog->StaticText2->SetLabel("Connected");
                    logger_dialog->connection_timer = 0;
                    logger_dialog->connection_flag = 1;

                    memcpy(id_txt,app_buf,3);
                    id_txt[3] = 0;
                    sscanf(id_txt,"%x",&id);

                    if(strlen(app_buf) == 7)
                    {
                        memcpy(val_txt,&app_buf[3],4);
                        val_txt[4] = 0;
                        sscanf(val_txt,"%x",&val);
                    }
                    else
                        val_txt[4] = 0;

                    dev = id & 0xF00;
                    id &= 0xFF;
                    tmp.Printf("%d",id);
                    if(dev == 0xF00 )
                    {
                        logger_dialog->TextCtrl22->SetValue(tmp);
                        if(!logger_dialog->CheckBox2->IsChecked())
                        {
                            set_text_color(BRIGHT_MAGENTA);
                            printf("Device detect response: {%s}\n",app_buf);
                            set_text_color(WHITE);
                        }
                    }
                    else
                    {
                        if(current_panel == 0 && dev != 0xA00)
                        {
                            set_text_color(BRIGHT_YELLOW);
                            printf("{%s}  %d\n",app_buf,id);
                            set_text_color(WHITE);

                            tmp.Printf("%d",id);
                            logger_dialog->TextCtrl10->SetValue(tmp);
                            tmp.Printf("{%s}",app_buf);
                            logger_dialog->TextCtrl25->SetValue(tmp);
                        }
                        if(current_panel == 1 && dev == 0xA00)
                        {
                            set_text_color(BRIGHT_YELLOW);
                            printf("{%s}  %d   %d\n",app_buf, id, val);
                            set_text_color(WHITE);

                            logger_dialog->TextCtrl61->GetValue().ToLong(&ev_filter);
                            tmp.Printf("%d",id);
                            logger_dialog->TextCtrl60->SetValue(tmp);
                            tmp.Printf("{%s}",app_buf);
                            logger_dialog->TextCtrl59->SetValue(tmp);

                            if(logger_dialog->TextCtrl61->GetValue().Len() > 0)
                            {
                                if(id == ev_filter)
                                {
                                    tmp.Printf("%d",val);
                                    logger_dialog->TextCtrl62->SetValue(tmp);
                                }
                                else
                                {
                                    tmp.Printf("%d",val);
                                    logger_dialog->TextCtrl62->SetValue("-----");
                                }

                            }
                            else
                            {
                                tmp.Printf("%d",val);
                                logger_dialog->TextCtrl62->SetValue(tmp);
                            }
                        }
                    }
                }
#endif
           app_buf[current_ndx] = 0x00;

           switch(current_ndx)
           {
              case 3:  /* 801 */
                 event = event2hex(app_buf);
                 ev->event_type =  event >> 8;
                 ev->event_id = event & 0xFF;
                 //ev->port_id = sws->port_id;
                 ev->value = 0;
              break;

              case 4:   /* 8001*/
                 event = event2hex(app_buf);
                 ev->event_type =  event >> 12;
                 ev->event_id = event & 0xFFF;
                 //ev->port_id = sws->port_id;
                 ev->value = 0;
              break;

              case 7: /* A011234*/
                 //app_buf[7] = '0';
                 //app_buf[7] = 0x00;
                 event = event2hex(app_buf);
                 //event >>= 4;
                 ev->event_type =  event >> 24;
                 ev->event_id = (event >> 16) & 0xFF;
                 //ev->port_id = sws->port_id;
                 ev->value = event & 0xFFFF;
              break;

              case 8:  /* A0011234*/
                 event = event2hex(app_buf);
                 ev->event_type =  event >> 28;
                 ev->event_id = (event >> 16) & 0xFFF;
                 //ev->port_id = sws->port_id;
                 ev->value = event & 0xFFFF;
              break;

           }

                    logger_dialog->StaticText2->SetForegroundColour(clDarkGreen);
                    logger_dialog->StaticText2->SetLabel("Connected");
                    logger_dialog->connection_timer = 0;
                    logger_dialog->connection_flag = 1;

                    if(ev->event_type == 0x0F)
                    {
                        tmp.Printf("%d",ev->event_id);
                        logger_dialog->TextCtrl22->SetValue(tmp);
                        if(!logger_dialog->CheckBox2->IsChecked())
                        {
                            set_text_color(BRIGHT_MAGENTA);
                            printf("Device detect response: {%s}\n",app_buf);
                            set_text_color(WHITE);
                        }
                    }
                    else
                    {
                        if(current_panel == 0 && ev->event_type != 0x0A)
                        {
                            set_text_color(BRIGHT_YELLOW);
                            printf("{%s}  %d\n",app_buf,ev->event_id);
                            set_text_color(WHITE);

                            tmp.Printf("%d",ev->event_id);
                            logger_dialog->TextCtrl10->SetValue(tmp);
                            tmp.Printf("{%s}",app_buf);
                            logger_dialog->TextCtrl25->SetValue(tmp);
                        }
                        if(current_panel == 1 && ev->event_type == 0x0A)
                        {
                            set_text_color(BRIGHT_YELLOW);
                            printf("{%s}  %d   %d\n",app_buf, ev->event_id, ev->value);
                            set_text_color(WHITE);

                            logger_dialog->TextCtrl61->GetValue().ToLong(&ev_filter);
                            tmp.Printf("%d",ev->event_id);
                            logger_dialog->TextCtrl60->SetValue(tmp);
                            tmp.Printf("{%s}",app_buf);
                            logger_dialog->TextCtrl59->SetValue(tmp);

                            if(logger_dialog->TextCtrl61->GetValue().Len() > 0)
                            {
                                if(ev->event_id == ev_filter)
                                {
                                    tmp.Printf("%d",ev->value);
                                    logger_dialog->TextCtrl62->SetValue(tmp);
                                }
                                else
                                {
                                    tmp.Printf("%d",ev->value);
                                    logger_dialog->TextCtrl62->SetValue("-----");
                                }

                            }
                            else
                            {
                                tmp.Printf("%d",ev->value);
                                logger_dialog->TextCtrl62->SetValue(tmp);
                            }
                        }
                    }

            break;

                default:
                    if(current_ndx<1000)
                    {
                        app_buf[current_ndx++] = rxbuf[i];
                    }


                }
            }
        }
    }
#ifdef PARANOIC_TEST
    printf("c");
#endif
    return 0;
}

int min_(int x, int y)
{
    if(x>y)
        return y;
    return x;
}

void loggerDialog::OnTimer1Trigger(wxTimerEvent& event)
{
    static int state = 0;
    int i,res = 0;
    wxString output_tmp;
    char tmp_array[256],*ptmp_array;
    long device,position,length,index,factor,offset,mask,options,dataref_type,indication_type;

     long numeric_tape_len;
     ComboBox4->GetValue().ToLong(&numeric_tape_len);
     long discrete_tape_len;
     ComboBox3->GetValue().ToLong(&discrete_tape_len);
     //discrete_tape_len *= 8;
     int devs = numeric_tape_len / 8;


    if(timer_enabled == 0)
        return;

    if(timer_enabled > 1)
    {
        timer_enabled--;
        return;
    }
#ifdef PARANOIC_TEST
    printf("T");
#endif
    if(!TextCtrl14->GetValue().ToLong(&device))
        device = 0;

    if(!TextCtrl15->GetValue().ToLong(&position))
        position = 0;
    if(!TextCtrl16->GetValue().ToLong(&length))
        length = 0;
//    if(!TextCtrl21->GetValue().ToLong(&index))
//        index = 0;
    if(!TextCtrl18->GetValue().ToLong(&factor))
        factor = 0;
    if(!TextCtrl19->GetValue().ToLong(&offset))
        offset = 0;
    if(!TextCtrl20->GetValue().ToLong(&mask))
        mask = 0;
    options = 0;
    if(CheckBox1->IsChecked())
        options += 1;

    indication_type =  Choice4->GetSelection();
    dataref_type = Choice3->GetSelection();
    wxString tmpwxstr = TextCtrl12->GetValue();
    const char *data = tmpwxstr.c_str().AsChar();
    int data_len = strlen(data);
    data_len = min_(length,data_len);

    switch(indication_type)
    {
    case 0: /*NUMERIC*/
        if(device >=0 && device < devs && position >=0 && position <8 && data_len > 0 && data_len <= 8)
            memcpy(&display_array[4+8*device + position],data,data_len);
        break;

    case 1: /*DISCRETE*/
        int value = atoi(data);
        if(position >=0 && position < discrete_tape_len)
            led_array[position+4] = '0' + (value & 1);
        break;
    }


#if 1
    //ComboBox3->GetValue().ToLong(&val);

    switch(++state)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:

    case 2:
    case 4:
    case 6:
    case 8:



        if(current_panel == 2 || current_panel == 3)
            ;
        else
            break;

        display_array[0] = '{';
        display_array[1] = '7';
        display_array[2] = 'S';
        display_array[3] = 'G';
        display_array[numeric_tape_len + 4] = '}';
        display_array[numeric_tape_len + 5] = 0;

        res = serial_port.SerialWrite((unsigned char*)display_array, numeric_tape_len + 5);


        if(current_panel == 2 || current_panel == 4)
            ;
        else
            break;

        led_array[0] = '{';
        led_array[1] = 'L';
        led_array[2] = 'E';
        led_array[3] = 'D';
        led_array[discrete_tape_len + 4] = '}';
        led_array[discrete_tape_len + 5] = 0;


        res = serial_port.SerialWrite((unsigned char*)led_array, discrete_tape_len + 5);

        break;
    case 10:
#ifdef PARANOIC_TEST
        printf("D");
#endif
        //if(current_panel != 0)
        //    break;
        res = serial_port.SerialWrite((unsigned char*)"{DET}", 5);
        if(!CheckBox2->IsChecked())
        {
            set_text_color(BRIGHT_CYAN);
            printf("{DET}\n");
            set_text_color(BRIGHT_WHITE);
        }
        state = 0;
    }


#endif
    if(res == -1)
    {
        connection_flag = 0;
        StaticText2->SetForegroundColour(clRed);
        StaticText2->SetLabel("Comm.Error");

    }
    else if(++connection_timer == 20)
    {
        connection_flag = 0;
        StaticText2->SetForegroundColour(clDarkYellow);
        StaticText2->SetLabel("Disconnected");
    }
#ifdef PARANOIC_TEST
    printf("t");
#endif
}

void loggerDialog::OnClose(wxCloseEvent& event)
{

}

void loggerDialog::OnCheckBox1Click(wxCommandEvent& event)
{
}

void loggerDialog::OnChoice1Select1(wxCommandEvent& event)
{
    controls_params_selector();
}


void loggerDialog::OnChoice2Select(wxCommandEvent& event)
{
    controls_params_selector();
}

void loggerDialog::controls_params_disable_all(void)
{
    TextCtrl9->Disable();
    TextCtrl8->Disable();
    TextCtrl3->Disable();
    TextCtrl4->Disable();
    TextCtrl5->Disable();
    TextCtrl6->Disable();
    RadioBox1->Disable();
}

void loggerDialog::controls_params_enable_all(void)
{
    TextCtrl9->Enable();
    TextCtrl8->Enable();
    TextCtrl3->Enable();
    TextCtrl4->Enable();
    TextCtrl5->Enable();
    TextCtrl6->Enable();
    RadioBox1->Enable();
}



int loggerDialog::controls_params_selector(void)
{
    int data_type = Choice1->GetSelection();
    int control_type = Choice2->GetSelection();

    controls_params_disable_all();

    switch(control_type)
    {
    case TOGGLE_SWITCH:
        TextCtrl9->Enable();
        TextCtrl8->Enable();
        TextCtrl6->Enable();
        //Button31->Enable();
        break;

    case ENCODER:
    case CYCLIC_BUTTON:
        TextCtrl3->Enable();
        TextCtrl4->Enable();
        TextCtrl5->Enable();
        RadioBox1->Enable();
        RadioBox1->SetSelection(0);
        //Button31->Disable();
        break;

    case ROTARY_SWITCH_POSITION:
        TextCtrl8->Enable();
        TextCtrl6->Enable();
        Button31->Enable();
        break;

    }

#if 0
    const int selection_table[6][6] =
    {
        /*                    NONE CMD   DREF_FL  DREF_INT  ARRAY_FL  ARRAY_INT */
        /*NONE           */ { 0,   0,    0,     0,      0,      0    },
        /*BUTTON         */ { 0,   0x80, 0x83,  0x83,   0xA3,   0xA3 },
        /*CYCLIC_BUTTON  */ { 0,   0x00, 0x9C,  0x9C,   0xBC,   0xBC },
        /*TOGGLE_SWITCH  */ { 0,   0xC0, 0x83,  0x83,   0xA3,   0xA3 },
        /*ROTARY_SW_POS  */ { 0,   0x80, 0x82,  0x82,   0xA2,   0xA2 },
        /*ENCODER        */ { 0,   0x80, 0x9C,  0x9C,   0xBC,   0xBC },
    };

    int mask = 0xff & selection_table[control_type][data_type];

    if(mask & 1)
        TextCtrl9->Enable();
    else
        TextCtrl9->Disable();
    if(mask & 2)
        TextCtrl8->Enable();
    else
        TextCtrl8->Disable();
    if(mask & 4)
        TextCtrl3->Enable();
    else
        TextCtrl3->Disable();
    if(mask & 8)
        TextCtrl4->Enable();
    else
        TextCtrl4->Disable();
    if(mask & 16)
        TextCtrl5->Enable();
    else
        TextCtrl5->Disable();
    if(mask & 32)
        TextCtrl6->Enable();
    else
        TextCtrl6->Disable();
    if(mask & 64)
        TextCtrl17->Enable();
    else
        TextCtrl17->Disable();
    if(mask & 128)
        TextCtrl1->Enable();
    else
        TextCtrl1->Disable();
#endif

    return 0;
}

int loggerDialog::indication_params_selector(void)
{
    int indication_type = Choice4->GetSelection();
    {
#if 0
        int indication_type = Choice4->GetSelection();
        const int selection_table[3] =
        {
            /*                  NONE  DISCR  NUMB  */
            /*NONE           */  0,   0x5F,  0x22
        };
        int mask = 0xff & selection_table[indication_type];

        if(data_type == IND_ARRAY_INT || data_type == IND_ARRAY_FLOAT)
            mask |= 0x80;

        if(mask & 1)
            TextCtrl14->Enable();
        else
            TextCtrl14->Disable();
        if(mask & 2)
            TextCtrl15->Enable();
        else
            TextCtrl15->Disable();
        if(mask & 4)
            TextCtrl16->Enable();
        else
            TextCtrl16->Disable();
        if(mask & 8)
            TextCtrl18->Enable();
        else
            TextCtrl18->Disable();
        if(mask & 16)
            TextCtrl19->Enable();
        else
            TextCtrl19->Disable();
        if(mask & 32)
            TextCtrl20->Enable();
        else
            TextCtrl20->Disable();
        if(mask & 64)
            CheckBox1->Enable();
        else
            CheckBox1->Disable();

        if(mask & 128)
            TextCtrl21->Enable();
        else
            TextCtrl21->Disable();

        return mask;
#endif
    }

    switch(indication_type)
    {
    case 0:
        indication_controls_enable_all();
        TextCtrl20->Disable();
        break;


    case 1:
        indication_controls_disable_all();
        //TextCtrl14->Enable();
        TextCtrl15->Enable();
        TextCtrl20->Enable();
        break;
    }

    return 0;
}

void loggerDialog::OnButton5Click(wxCommandEvent& event)
{
    const char *printable_types[] = {"int","uint","float","string"};
    const char *cycling[] = {"discrete_loop_event","discrete_limit_event"};
    const int   data_sizes[4] = {1, 2, 4, 8};
    wxString str = "", wxstr_dataref;

    long ctrl_event, index, min_value, max_value, increment, _0,_1;
    const char *data_offset = (wxstr_dataref = TextCtrl1->GetValue()).c_str().AsChar();
    unsigned long mask;

    int data_type = Choice1->GetSelection() & 3;
    int control_type = Choice2->GetSelection();
    int data_size = Choice8->GetSelection();
    int mask_present = 1 && TextCtrl9->GetValue().Len();

    if(!TextCtrl10->GetValue().ToLong(&ctrl_event))
        ctrl_event = 0;
    if(!TextCtrl9->GetValue().ToLong(&_0))
        _0 = 0;
    if(!TextCtrl8->GetValue().ToLong(&_1))
        _1 = 0;

    if(!TextCtrl3->GetValue().ToLong(&min_value))
        min_value = 0;
    if(!TextCtrl4->GetValue().ToLong(&max_value))
        max_value = 0;
    if(!TextCtrl5->GetValue().ToLong(&increment))
        increment = 0;


    switch(control_type)
    {
////////////////////////////////////////////////////////////
/////////////////////////// BUTTON /////////////////////////
////////////////////////////////////////////////////////////
    case TOGGLE_SWITCH:


        str.Printf("\
[Key%04d]\r\n\
mode: discrete_event\r\n\
offset: %s\r\n\
size:%d\r\n\
type: %s\r\n\
0: %d\r\n\
1: %d\r\n",
                   ctrl_event,
                   data_offset,
                   data_sizes[data_size & 0x03],
                   printable_types[data_type],
                   _0,
                   _1
                  );

        break;

////////////////////////////////////////////////////////////////////
//////////////////////////// CYCLIC BUTTON /////////////////////////
////////////////////////////////////////////////////////////////////

    case CYCLIC_BUTTON:

        str.Printf("\
[Key%04d]\r\n\
mode: %s\r\n\
offset: %s\r\n\
size: %d\r\n\
type: %s\r\n\
min: %d\r\n\
max: %d\r\n\
increment: %d\r\n",
                   ctrl_event,
                   cycling[RadioBox1->GetSelection() & 1],
                   data_offset,
                   data_sizes[data_size & 0x03],
                   printable_types[data_type],
                   min_value,
                   max_value,
                   increment
                  );

        break;


/////////////////////////////////////////////////////////////////////////////
//////////////////////////// ROTARY SWITCH POSITION /////////////////////////
/////////////////////////////////////////////////////////////////////////////

    case ROTARY_SWITCH_POSITION:

        str.Printf("\
[Key%04d]\r\n\
mode: discrete_event\r\n\
offset: %s\r\n\
size: %d\r\n\
type: %s\r\n\
1: %d\r\n",
                   ctrl_event,
                   data_offset,
                   data_sizes[data_size & 0x03],
                   printable_types[data_type],
                   _1
                  );

        break;

//////////////////////////////////////////////////////////////
//////////////////////////// ENCODER /////////////////////////
//////////////////////////////////////////////////////////////
    case ENCODER:

        str.Printf("\
[Key%04d]\r\n\
mode: %s\r\n\
offset: %s\r\n\
size: %d\r\n\
type: %s\r\n\
min: %d\r\n\
max: %d\r\n\
increment: %d\r\n",
                   ctrl_event,
                   cycling[RadioBox1->GetSelection() & 1],
                   data_offset,
                   data_sizes[data_size & 0x03],
                   printable_types[data_type],
                   min_value,
                   max_value,
                   increment
                  );


        break;
    }


        int rem_max_len = GetMaxLen(TextCtrl2);
        if(!TextCtrl2->GetValue().Find((const char*)"Some comment (remove all to omit comments)"))
            rem_max_len = 0;

        wxString tmp;
        if(rem_max_len)
        {
            AddHashLine(TextCtrl7, '#', rem_max_len+3);
            int i;
            for(i=0; i < TextCtrl2->GetNumberOfLines(); i++)
            {
                tmp = "# " + TextCtrl2->GetLineText(i) + "\r\n";
                TextCtrl7->AppendText(tmp);
            }
            AddHashLine(TextCtrl7, '#', rem_max_len+3);

        }


    TextCtrl7->AppendText(str);

    if(! TextCtrl6->GetValue().ToULong(&mask))
        mask = 0;

    if(mask_present)
    {
        str.Printf("mask: %u\r\n",mask);
        TextCtrl7->AppendText(str);
    }

    if(1 == Choice11->GetSelection())
    {
       str.Printf("encoding: bcd\r\n");
       TextCtrl7->AppendText(str);
    }

    str.Printf("\r\n");
    TextCtrl7->AppendText(str);

}

void loggerDialog::AddHashLine(wxTextCtrl *ed, int symbol, int number)
{
    int i;
    wxString str = "",s;
    s.Printf("%c",symbol);

    for(i = 0; i < number; i++)
        str += s;
    str += "\r\n";
    ed->AppendText(str);
}


void loggerDialog::OnChoice3Select(wxCommandEvent& event)
{
    indication_params_selector();
}

void loggerDialog::OnChoice4Select(wxCommandEvent& event)
{
    indication_params_selector();
}

void loggerDialog::indication_controls_enable_all(void)
{
    TextCtrl14->Enable();
    TextCtrl15->Enable();
    TextCtrl16->Enable();
    TextCtrl18->Enable();
    TextCtrl19->Enable();
    TextCtrl20->Enable();
    CheckBox1->Enable();
}

void loggerDialog::indication_controls_disable_all(void)
{
    TextCtrl14->Disable();
    TextCtrl15->Disable();
    TextCtrl16->Disable();
    TextCtrl18->Disable();
    TextCtrl19->Disable();
    TextCtrl20->Disable();
    CheckBox1->Disable();
}


void loggerDialog::OnButton6Click(wxCommandEvent& event)
{
    const char *data_type_printable[] = {"int","uint","float","string"};
    const int   data_sizes[4] = {1, 2, 4, 8};
    wxString str,wxstr_offset = "";
    const char *data_offset = (wxstr_offset = TextCtrl11->GetValue()).c_str().AsChar();
    long index, device, position, length, mask,options = 0;
    int data_type = Choice3->GetSelection() & 0x03;
    int indication_type = Choice4->GetSelection() & 0x01;
    int data_size = data_sizes[Choice10->GetSelection() & 0x03];
    double factor;
    double zero_offset;

    int device_present = 1 && TextCtrl14->GetValue().Len(),
        position_present = 1 && TextCtrl15->GetValue().Len(),
        length_present = 1 && TextCtrl16->GetValue().Len(),
        factor_present = 1 && TextCtrl18->GetValue().Len(),
        offset_present = 1 && TextCtrl19->GetValue().Len(),
        mask_present = 1 && TextCtrl20->GetValue().Len();

    if(!TextCtrl14->GetValue().ToLong(&device))
        device = 0;

    if(!TextCtrl15->GetValue().ToLong(&position))
        position = 0;
    if(!TextCtrl16->GetValue().ToLong(&length))
        length = 0;

    if(!TextCtrl18->GetValue().ToDouble(&factor))
        factor = 0;
    if(!TextCtrl19->GetValue().ToDouble(&zero_offset))
        zero_offset = 0;
    if(!TextCtrl20->GetValue().ToLong(&mask))
        mask = 0;

    if(CheckBox1->IsChecked())
        options |= 1;


    int rem_max_len = GetMaxLen(TextCtrl2);
    wxString tmp;

    if(!TextCtrl2->GetValue().Find((const char*)"Some comment (remove all to omit comments)"))
        rem_max_len = 0;


    if(rem_max_len)
    {
        AddHashLine(TextCtrl7, '#', rem_max_len+3);
        int i;
        for(i=0; i < TextCtrl2->GetNumberOfLines(); i++)
        {
            tmp = "# " + TextCtrl2->GetLineText(i) + "\r\n";
            TextCtrl7->AppendText(tmp);
        }
        AddHashLine(TextCtrl7, '#', rem_max_len+3);

    }


    switch(indication_type)
    {
    case 0:  /* NUMERIC */

        str.Printf("\
[Ind %s]\r\n\
mode: numeric_polling\r\n\
offset: %s\r\n\
size: %d\r\n\
type: %s\r\n\
dev: %d\r\n\
pos: %d\r\n\
len: %d\r\n\
",
                   TextCtrl13->GetValue().c_str().AsChar(),
                   data_offset,
                   data_size,
                   data_type_printable[data_type],
                   device,
                   position,
                   length
                  );

        TextCtrl7->AppendText(str);

        if(factor_present)
        {
            TextCtrl7->AppendText("factor: "+float_to_string(factor)+"\r\n");
        }

        if(offset_present)
        {

            TextCtrl7->AppendText("zero_offset: "+float_to_string(zero_offset)+"\r\n");
        }

        if(options)
        {
            str.Printf("options: %d\r\n",options);
            TextCtrl7->AppendText(str);
        }

        if(1 == Choice13->GetSelection())
        {
           TextCtrl7->AppendText("encoding: bcd\r\n");
        }

        TextCtrl7->AppendText("\r\n");
        break;



    case 1: /*DISCRETE */


        str.Printf("\
[Ind %s]\r\n\
mode: discrete_polling\r\n\
array: %s\r\n\
index: %d\r\n\
type: %s\r\n\
pos: %d\r\n\
",
                   TextCtrl13->GetValue().c_str().AsChar(),
                   data_offset,
                   data_size,
                   data_type_printable[data_type],
                   position
                  );

        TextCtrl7->AppendText(str);

        if(mask_present)
        {
            str.Printf("mask: %d\r\n",mask);
            TextCtrl7->AppendText(str);
        }

        if(1 == Choice13->GetSelection())
        {
           TextCtrl7->AppendText("encoding: bcd\r\n");
        }

        TextCtrl7->AppendText("\r\n");

        break;
    }

}



void loggerDialog::OnButton7Click(wxCommandEvent& event)
{
    memset(display_array+4,' ',128);
    memset(led_array+4,'0',64);


}

void loggerDialog::OnComboBox1Selected(wxCommandEvent& event)
{


}

void loggerDialog::OnButton8Click(wxCommandEvent& event)
{
    StaticText2->SetForegroundColour(clDarkYellow);
    connection_flag = 0;
    connection_timer = 0;
    StaticText2->SetLabel("Disconnected");
    timer_enabled = 50;
    serial_port.DTR_off();
    Timer2.Start(500);
}

void loggerDialog::OnTimer2Trigger(wxTimerEvent& event)
{
    serial_port.DTR_on();
    Timer2.Stop();
}



void loggerDialog::OnButton9Click1(wxCommandEvent& event)
{
    console_flag ^= 1;

    if(console_flag&1)
    {
        AllocConsole();
        //freopen("conin$",  "r", stdin); //console input buffer
        freopen("conout$", "w", stdout);//console output buffer
        freopen("conout$", "w", stderr); //console error device
        console_init("Debug console");
        CheckBox2->Enable(true);
    }
    else
    {
        fclose(stderr);
        fclose(stdout);
        console_close();
        FreeConsole();
        CheckBox2->Enable(false);

    }
}



void loggerDialog::OnCheckBox2Click(wxCommandEvent& event)
{
}

void loggerDialog::OnNotebook1PageChanged(wxNotebookEvent& event)
{

    TextCtrl67->Disable();
    StaticText114->Disable();
    current_panel = Notebook1->GetSelection();

    //  int sel = Choice6->GetSelection();
    // switch(sel)
    // {
    // case 0:
    //     break;
    // case 1:
    // case 2:
    //     TextCtrl48->Enable(false);
    //     break;
    // case 3:
    // case 4:
    //     TextCtrl48->Enable(true);
    //     break;
    // }

}

void loggerDialog::OnChoice5Select(wxCommandEvent& event)
{
    wxColor fgcl,bgcl;
    wxFont fnt0,fnt1;
    fgcl = TextCtrl42->GetForegroundColour();
    bgcl = TextCtrl42->GetBackgroundColour();
    fnt0 == TextCtrl42->GetFont();
    fnt1 = fnt0;
    fnt1.MakeBold();
    int sel = Choice5->GetSelection();
    int i;
    for(i=0; i<16; i++)
    {
        if(sel == i)
        {
            axes[i]->SetForegroundColour(wxColor(255,255,255));
            axes[i]->SetBackgroundColour(wxColor(0,0,0));
            axes[i]->SetFont(fnt1);
        }
        else
        {
            axes[i]->SetForegroundColour(fgcl);
            axes[i]->SetBackgroundColour(bgcl);
            axes[i]->SetFont(fnt0);
        }
        axes[i]->Refresh();
    }


}

void loggerDialog::OnButton12Click(wxCommandEvent& event)
{
    int sel = Choice5->GetSelection();
    TextCtrl44->SetValue(axes[sel]->GetValue());
}

void loggerDialog::OnButton13Click(wxCommandEvent& event)
{
    int sel = Choice5->GetSelection();
    TextCtrl52->SetValue(axes[sel]->GetValue());
}

void loggerDialog::OnButton14Click(wxCommandEvent& event)
{
    int sel = Choice5->GetSelection();
    TextCtrl53->SetValue(axes[sel]->GetValue());
}

void loggerDialog::OnCheckBox3Click(wxCommandEvent& event)
{
    TextCtrl52->Enable(CheckBox3->IsChecked());
    Button13->Enable(CheckBox3->IsChecked());
}

void loggerDialog::OnChoice6Select(wxCommandEvent& event)
{
    int sel = Choice6->GetSelection();
    switch(sel)
    {
    case 0:
        break;
    case 1:
    case 2:
        TextCtrl48->Enable(false);
        break;
    case 3:
    case 4:
        TextCtrl48->Enable(true);
        break;
    }
}

void loggerDialog::OnButton10Click2(wxCommandEvent& event)
{
    const char *data_type_name[5] = {"", "int", "float", "int", "float"};
    wxString str;
    long index, devmax, devcenter, devmin;
    double simmax,simmin;
    int data_type = Choice6->GetSelection();
    int has_center = 1 && CheckBox3->IsChecked();
    int axis_ndx = Choice5->GetSelection();
    double factor = 1;
    double offset = 0;
    int index_present =  1 && TextCtrl48->GetValue().Len(),
        devmax_present = 1 && TextCtrl44->GetValue().Len(),
        devcenter_present = 1 && TextCtrl52->GetValue().Len(),
        devmin_present = 1 && TextCtrl53->GetValue().Len(),
        simmin_present = 1 && TextCtrl54->GetValue().Len(),
        simmax_present = 1 && TextCtrl55->GetValue().Len();

    if(!TextCtrl48->GetValue().ToLong(&index))
        index = 0;

    if(!TextCtrl44->GetValue().ToLong(&devmax))
        devmax = 0;

    if(!TextCtrl52->GetValue().ToLong(&devcenter))
        devcenter = 0;

    if(!TextCtrl53->GetValue().ToLong(&devmin))
        devmin = 0;

    if(!TextCtrl54->GetValue().ToDouble(&simmax))
        simmax = 0;

    if(!TextCtrl55->GetValue().ToDouble(&simmin))
        simmin = 0;


    if(CheckBox3->IsChecked())
    {
        factor = (simmax - simmin) / (devmax - devmin);
        offset = 1.0 * devcenter;
    }
    else
    {
        factor = (simmax - simmin) / (1.0*(devmax - devmin));
        offset = 1.0 * devmax - simmax/factor;
    }


    int rem_max_len = GetMaxLen(TextCtrl2);
    wxString tmp;
    if(rem_max_len)
    {
        AddHashLine(TextCtrl7, '#', rem_max_len+3);
        int i;
        for(i=0; i < TextCtrl2->GetNumberOfLines(); i++)
        {
            tmp = "# " + TextCtrl2->GetLineText(i) + "\r\n";
            TextCtrl7->AppendText(tmp);
        }
        AddHashLine(TextCtrl7, '#', rem_max_len+3);

    }

    switch(data_type)
    {
    case 0:
        break;
    case 1:
    case 2:
        str.Printf("\
[Axis %s]\r\n\
mode: input\r\n\
dataref: %s\r\n\
type: %s\r\n\
axis: %d\r\n\
factor: %11.5f\r\n\
offset: %6.0f\r\n\r\n\
",
                   TextCtrl43->GetValue().c_str().AsChar(),
                   TextCtrl42->GetValue().c_str().AsChar(),
                   data_type_name[data_type],
                   axis_ndx,
                   factor,
                   offset
                  );
        break;

    case 3:
    case 4:
        str.Printf("\
[Axis %s]\r\n\
mode: input\r\n\
array: %s\r\n\
index: %s\r\n\
type: %s\r\n\
axis: %d\r\n\
factor: %11.5f\r\n\
offset: %6.0f\r\n\r\n\
",
                   TextCtrl43->GetValue().c_str().AsChar(),
                   TextCtrl42->GetValue().c_str().AsChar(),
                   index,
                   data_type_name[data_type],
                   axis_ndx,
                   factor,
                   offset
                  );

        break;
    }
    TextCtrl7->AppendText(str);

}



/*
https://www.onlinegdb.com/

#####################
# Whitout center
#####################
simmin = 40
simmax = 240
devmin = 1023
devmax = 200

factor = (simmax-simmin) / (devmax - devmin)
offset = devmax - simmax/factor


print("factor=",factor)
print("offset=",offset)
print("sim(devmax)=",(devmax-offset)*factor)
print("sim(devmin)=",(devmin-offset)*factor)


*/

void loggerDialog::OnButton15Click(wxCommandEvent& event)
{
    TextCtrl12->SetValue(TextCtrl23->GetValue());
}

void loggerDialog::OnButton16Click(wxCommandEvent& event)
{
    TextCtrl12->SetValue(TextCtrl24->GetValue());
}

void loggerDialog::OnButton17Click(wxCommandEvent& event)
{
    TextCtrl12->SetValue(TextCtrl45->GetValue());
}

void loggerDialog::OnButton18Click(wxCommandEvent& event)
{
    TextCtrl50->SetValue(TextCtrl62->GetValue());
}

void loggerDialog::OnButton19Click(wxCommandEvent& event)
{
    TextCtrl56->SetValue(TextCtrl62->GetValue());
}

void loggerDialog::OnButton20Click(wxCommandEvent& event)
{
    TextCtrl58->SetValue(TextCtrl62->GetValue());
}

void loggerDialog::OnButton21Click(wxCommandEvent& event)
{
    const char *data_type_printable[] = {"int","uint","float","string"};
    const int   data_sizes[4] = {1, 2, 4, 8};
    wxString str,wxstr_offset = "";
    double factor = 1;
    double zero_offset = 0;

    const char *data_offset = (wxstr_offset = TextCtrl17->GetValue()).c_str().AsChar();
    int data_type = Choice9->GetSelection() & 0x03;
    int data_size = data_sizes[Choice7->GetSelection() & 0x03];

    int factor_present = 1 && TextCtrl49->GetValue().Len();
    int offset_present = 1 && TextCtrl66->GetValue().Len();

    int rem_max_len = GetMaxLen(TextCtrl2);
    wxString tmp;

    if(!TextCtrl2->GetValue().Find((const char*)"Some comment (remove all to omit comments)"))
        rem_max_len = 0;

    if(rem_max_len)
    {
        AddHashLine(TextCtrl7, '#', rem_max_len+3);
        int i;
        for(i=0; i < TextCtrl2->GetNumberOfLines(); i++)
        {
            tmp = "# " + TextCtrl2->GetLineText(i) + "\r\n";
            TextCtrl7->AppendText(tmp);
        }
        AddHashLine(TextCtrl7, '#', rem_max_len+3);

    }

    long ev_id;
    if(! TextCtrl60->GetValue().ToLong(&ev_id))
       ev_id = 0;

    str.Printf("\
[Key%04d]\r\n\
mode: numeric_event\r\n\
offset: %s\r\n\
type: %s\r\n\
size: %d\r\n\
",
               ev_id,
               TextCtrl17->GetValue().c_str().AsChar(),
               data_type_printable[data_type],
               data_size
              );
    TextCtrl7->AppendText(str);

    if(0 == TextCtrl17->GetValue().Find("3110") && TextCtrl67->GetValue().Len())
    {
       str.Printf("control: %s\r\n",TextCtrl67->GetValue().c_str().AsChar());
       TextCtrl7->AppendText(str);
    }

    if(factor_present)
    {
        if(!TextCtrl49->GetValue().ToDouble(&factor))
            factor = 1;
        TextCtrl7->AppendText("factor: "+float_to_string(factor)+"\r\n");
    }

    if(offset_present)
    {
        if(!TextCtrl66->GetValue().ToDouble(&zero_offset))
            zero_offset = 0;

        TextCtrl7->AppendText("zero_offset: "+float_to_string(zero_offset)+"\r\n");
    }

    if(1 == Choice12->GetSelection())
    {
        str.Printf("encoding: bcd\r\n");
        TextCtrl7->AppendText(str);
    }


    TextCtrl7->AppendText("\r\n");

}

void loggerDialog::OnCheckBox4Click(wxCommandEvent& event)
{
    TextCtrl56->Enable(CheckBox4->IsChecked());
    Button19->Enable(CheckBox4->IsChecked());
}



void loggerDialog::OnPanel6MouseEnter(wxMouseEvent& event)
{

}

void loggerDialog::OnPanel6Paint(wxPaintEvent& event)
{

}

void loggerDialog::OnButton22Click(wxCommandEvent& event)
{
    long port;
    TextCtrl47->GetValue().ToLong(&port);
    if(0 == udp_server.Open(port))
    {
        Button22->Enable(false);
        TextCtrl47->Enable(false);
        Button23->Enable(true);
    }
}

void loggerDialog::OnButton23Click(wxCommandEvent& event)
{
    udp_server.Close();
    Button22->Enable(true);
    TextCtrl47->Enable(true);
    Button23->Enable(false);
}

void loggerDialog::OnTextCtrl47Text(wxCommandEvent& event)
{
}

void loggerDialog::OnButton24Click(wxCommandEvent& event)
{
    wxString tmp;
    double factor = 1;
    double zero_offset = 0;
    int devmax_present = 1 && TextCtrl21->GetValue().Len(),
        devmin_present = 1 && TextCtrl65->GetValue().Len(),
        simmax_present = 1 && TextCtrl46->GetValue().Len(),
        simmin_present = 1 && TextCtrl64->GetValue().Len();

    double devmax,devmin;
    double simmax,simmin;


    if(!TextCtrl21->GetValue().ToDouble(&devmax))
        devmax = 0;

    if(!TextCtrl65->GetValue().ToDouble(&devmin))
        devmin = 0;

    if(!TextCtrl46->GetValue().ToDouble(&simmax))
        simmax = 0;

    if(!TextCtrl64->GetValue().ToDouble(&simmin))
        simmin = 0;


    if(devmax_present &&
            devmin_present &&
            simmin_present &&
            simmax_present )
    {
        factor =  (1.0*(devmax - devmin)) / (simmax - simmin);
        zero_offset = 1.0 * devmax - simmax * factor;



        TextCtrl18->SetValue(float_to_string(factor));
        TextCtrl19->SetValue(float_to_string(zero_offset));
    }


}

void loggerDialog::OnButton25Click(wxCommandEvent& event)
{

    wxString tmp;
    int has_center = 1 && CheckBox4->IsChecked();
    double devmax, devcenter = 0, devmin;
    double simmax,simmin;
    double factor = 1;
    long long zero_offset = 0;
    int devmax_present = 1 && TextCtrl50->GetValue().Len(),
        devcenter_present = 1 && TextCtrl56->GetValue().Len(),
        devmin_present = 1 && TextCtrl58->GetValue().Len(),
        simmin_present = 1 && TextCtrl57->GetValue().Len(),
        simmax_present = 1 && TextCtrl51->GetValue().Len();


    if(!TextCtrl50->GetValue().ToDouble(&devmax))
        devmax = 0;

    if(!TextCtrl56->GetValue().ToDouble(&devcenter))
        devcenter = 0;

    if(!TextCtrl58->GetValue().ToDouble(&devmin))
        devmin = 0;

    if(!TextCtrl51->GetValue().ToDouble(&simmax))
        simmax = 0;

    if(!TextCtrl57->GetValue().ToDouble(&simmin))
        simmin = 0;


    if(devmax_present &&
            devmin_present &&
            simmin_present &&
            simmax_present )
    {
        if(CheckBox4->IsChecked())
        {
            if(devcenter_present && devmax != devmin)
            {
                factor = (simmax - simmin) / (devmax - devmin);
                zero_offset = 1.0 * devcenter;
            }
            else
            {
                factor = 1;
                zero_offset = 0;
            }
        }
        else
        {
            factor = (simmax - simmin) / (devmax - devmin);
            zero_offset = devmax - simmax/factor;
        }
    }
    else
    {
        factor = 1;
        zero_offset = 0;
    }

    TextCtrl49->SetValue(float_to_string(factor));
    TextCtrl66->SetValue(float_to_string(zero_offset));

}

void loggerDialog::OnButton27Click(wxCommandEvent& event)
{
    int i;
    for(i=0;i<32;i++)
    {
      mask32[i]->SetValue(true);
    }
}

void loggerDialog::OnButton26Click(wxCommandEvent& event)
{
    int i;
    for(i=0;i<32;i++)
    {
      mask32[i]->SetValue(false);
    }
}

void loggerDialog::OnButton28Click(wxCommandEvent& event)
{
    int i;
    for(i=0;i<32;i++)
    {
      mask32[i]->SetValue(! mask32[i]->IsChecked());
    }

}

void loggerDialog::OnButton29Click(wxCommandEvent& event)
{
    unsigned long mask = 0;
    int i;
    unsigned long w = 1;
    for(i=0;i<32;i++)
    {
         mask  += (w * mask32[i]->IsChecked());
         w *= 2;
    }
    wxString tmp;
    tmp.Printf("%u",mask);
    TextCtrl6->SetValue(tmp);
}

void loggerDialog::OnButton30Click(wxCommandEvent& event)
{
    unsigned long mask;
    if(!TextCtrl6->GetValue().ToULong(&mask))
       mask = 0;
    int i;
    unsigned long w = 1;

    for(i=0;i<32;i++)
    {
        mask32[i]->SetValue(mask & w);
        w *= 2;
    }
}

void loggerDialog::OnButton31Click(wxCommandEvent& event)
{
    TextCtrl1->SetValue("3110");
    Choice1->SetSelection(0);
    Choice8->SetSelection(3);
    Choice11->SetSelection(0);
    TextCtrl6->Enable(false);
    TextCtrl6->SetValue("");
}

void loggerDialog::OnButton32Click(wxCommandEvent& event)
{
    TextCtrl17->SetValue("3110");
    Choice9->SetSelection(0);
    Choice7->SetSelection(3);
    Choice12->SetSelection(0);
    TextCtrl67->Enable();
    StaticText114->Enable();
    //TextCtrl6->Enable(false);
    //TextCtrl6->SetValue("");


}


wxString float_to_string(double val)
{
   wxString tmp;
   char buf[100];
   sprintf(buf,"%e",val);
   tmp.Printf(buf);
   return tmp;
}



void loggerDialog::OnButton33Click(wxCommandEvent& event)
{
   memset(display_array,' ',sizeof(display_array));
   memset(led_array,'0',sizeof(led_array));
}

void loggerDialog::OnComboBox4Selected(wxCommandEvent& event)
{
    memset(display_array,' ',sizeof(display_array));
    memset(led_array,'0',sizeof(led_array));
}

void loggerDialog::OnComboBox3Selected(wxCommandEvent& event)
{
    memset(display_array,' ',sizeof(display_array));
    memset(led_array,'0',sizeof(led_array));
}
