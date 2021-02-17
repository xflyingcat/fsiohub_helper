/***************************************************************
 * Name:      loggerMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2019-02-12
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef LOGGERMAIN_H
#define LOGGERMAIN_H

//(*Headers(loggerDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

#include "Serial.h"
#include "UDPServer.h"

typedef struct {
  unsigned char event_type;
  unsigned char port_id;
  unsigned short event_id;
  long value;
} EVENT_STRUCT;


enum {
    CONTROL_TYPE_NONE = 0,
    CONTROL_TYPE_BUTTON,
    CONTROL_TYPE_CYCLIC_BUTTON,
    CONTROL_TYPE_TOGGLE_SWITCH,
    CONTROL_TYPE_ROTARY_SWITCH_POSITION,
    CONTROL_TYPE_ENCODER
};

enum {
    DATA_TYPE_NONE = 0,
    DATA_TYPE_COMMAND,
    DATA_TYPE_DATAREF_FLOAT,
    DATA_TYPE_DATAREF_INTEGER,
    DATA_TYPE_ARRAY_FLOAT,
    DATA_TYPE_ARRAY_INTEGER
};

enum
{
    TYPE_NONE = 0,
    TYPE_INTEGER,
    TYPE_FLOAT
};

enum
{
    CONTROL_NONE = 0,
    TOGGLE_SWITCH,
    CYCLIC_BUTTON,
    ROTARY_SWITCH_POSITION,
    ENCODER
};


enum
{
    INDICATION_NONE = 0,
    NUMERIC,
    DISCRETE
};

enum
{
    IND_TYPE_NONE = 0,
    IND_DATAREF_INT,
    IND_DATAREF_FLOAT,
    IND_ARRAY_INT,
    IND_ARRAY_FLOAT
};



class loggerDialog: public wxDialog
{

    public:
        Serial serial_port;
        UDPServer udp_server;
        void controls_params_enable_all(void);
        void controls_params_disable_all(void);
        void indication_controls_enable_all(void);
        void indication_controls_disable_all(void);
        loggerDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~loggerDialog();
        void LoadPortsList(void);
        int put_config(void);
        int get_config(void);
        int GetMaxLen(wxTextCtrl *ed);
        int controls_params_selector(void);
        int indication_params_selector(void);
        void AddHashLine(wxTextCtrl *ed, int symbol, int number);
        void SetLedColor(int curr_cass, char *data,
                               wxStaticText *st1,
                               wxStaticText *st2,
                               wxStaticText *st3
                              );
        void put_text_data(int curr_cassette, wxString str,
                                 wxStaticText *st1,
                                 wxStaticText *st2,
                                 wxStaticText *st3,
                                 wxStaticText *st4
                                );
        void SetCommLedColor(int curr_cass, int com_state,
                               wxStaticText *st1,
                               wxStaticText *st2,
                               wxStaticText *st3,
                               wxStaticText *st4
                              );
        void SetInactive(int curr_cass);
        int connection_flag;
        int connection_timer;
        int console_flag;
        //int rx_call_back(char *buf, int len);
    //private:

        //(*Handlers(loggerDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnChoice1Select(wxCommandEvent& event);
        void OnButton4Click(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnCheckBox1Click(wxCommandEvent& event);
        void OnChoice2Select(wxCommandEvent& event);
        void OnChoice1Select1(wxCommandEvent& event);
        void OnButton5Click(wxCommandEvent& event);
        void OnChoice3Select(wxCommandEvent& event);
        void OnChoice4Select(wxCommandEvent& event);
        void OnButton6Click(wxCommandEvent& event);
        void OnButton7Click(wxCommandEvent& event);
        void OnComboBox1Selected(wxCommandEvent& event);
        void OnButton8Click(wxCommandEvent& event);
        void OnTimer2Trigger(wxTimerEvent& event);
        void OnButton9Click(wxCommandEvent& event);
        void OnButton10Click(wxCommandEvent& event);
        void OnButton9Click1(wxCommandEvent& event);
        void OnButton10Click1(wxCommandEvent& event);
        void OnCheckBox2Click(wxCommandEvent& event);
        void OnNotebook1PageChanged(wxNotebookEvent& event);
        void OnChoice5Select(wxCommandEvent& event);
        void OnButton12Click(wxCommandEvent& event);
        void OnButton13Click(wxCommandEvent& event);
        void OnButton14Click(wxCommandEvent& event);
        void OnCheckBox3Click(wxCommandEvent& event);
        void OnChoice6Select(wxCommandEvent& event);
        void OnButton10Click2(wxCommandEvent& event);
        void OnButton15Click(wxCommandEvent& event);
        void OnButton16Click(wxCommandEvent& event);
        void OnButton17Click(wxCommandEvent& event);
        void OnButton18Click(wxCommandEvent& event);
        void OnButton19Click(wxCommandEvent& event);
        void OnButton20Click(wxCommandEvent& event);
        void OnButton21Click(wxCommandEvent& event);
        void OnCheckBox4Click(wxCommandEvent& event);
        void OnChoice7Select(wxCommandEvent& event);
        void OnPanel6MouseEnter(wxMouseEvent& event);
        void OnPanel6Paint(wxPaintEvent& event);
        void OnButton22Click(wxCommandEvent& event);
        void OnButton23Click(wxCommandEvent& event);
        void OnTextCtrl47Text(wxCommandEvent& event);
        void OnButton24Click(wxCommandEvent& event);
        void OnButton25Click(wxCommandEvent& event);
        void OnButton27Click(wxCommandEvent& event);
        void OnButton26Click(wxCommandEvent& event);
        void OnButton28Click(wxCommandEvent& event);
        void OnButton29Click(wxCommandEvent& event);
        void OnButton30Click(wxCommandEvent& event);
        void OnButton31Click(wxCommandEvent& event);
        void OnButton32Click(wxCommandEvent& event);
        void OnButton33Click(wxCommandEvent& event);
        void OnComboBox4Selected(wxCommandEvent& event);
        void OnComboBox3Selected(wxCommandEvent& event);
        //*)

        //(*Identifiers(loggerDialog)
        static const long ID_STATICTEXT6;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICTEXT12;
        static const long ID_TEXTCTRL7;
        static const long ID_STATICLINE5;
        static const long ID_STATICTEXT15;
        static const long ID_TEXTCTRL25;
        static const long ID_STATICTEXT68;
        static const long ID_TEXTCTRL10;
        static const long ID_STATICLINE2;
        static const long ID_CHOICE2;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT27;
        static const long ID_BUTTON31;
        static const long ID_CHOICE1;
        static const long ID_STATICTEXT3;
        static const long ID_CHOICE8;
        static const long ID_STATICTEXT4;
        static const long ID_CHOICE11;
        static const long ID_STATICTEXT110;
        static const long ID_STATICLINE18;
        static const long ID_STATICTEXT113;
        static const long ID_STATICLINE13;
        static const long ID_STATICTEXT14;
        static const long ID_TEXTCTRL9;
        static const long ID_STATICTEXT13;
        static const long ID_TEXTCTRL8;
        static const long ID_STATICTEXT11;
        static const long ID_TEXTCTRL6;
        static const long ID_STATICTEXT8;
        static const long ID_TEXTCTRL3;
        static const long ID_STATICTEXT9;
        static const long ID_TEXTCTRL4;
        static const long ID_STATICTEXT10;
        static const long ID_TEXTCTRL5;
        static const long ID_STATICLINE19;
        static const long ID_RADIOBOX1;
        static const long ID_BUTTON5;
        static const long ID_STATICLINE17;
        static const long ID_STATICTEXT109;
        static const long ID_BUTTON26;
        static const long ID_BUTTON27;
        static const long ID_BUTTON28;
        static const long ID_STATICTEXT19;
        static const long ID_CHECKBOX5;
        static const long ID_STATICTEXT78;
        static const long ID_CHECKBOX6;
        static const long ID_STATICTEXT79;
        static const long ID_CHECKBOX7;
        static const long ID_STATICTEXT80;
        static const long ID_CHECKBOX8;
        static const long ID_STATICTEXT81;
        static const long ID_CHECKBOX9;
        static const long ID_STATICTEXT82;
        static const long ID_CHECKBOX10;
        static const long ID_STATICTEXT83;
        static const long ID_CHECKBOX11;
        static const long ID_STATICTEXT84;
        static const long ID_CHECKBOX12;
        static const long ID_STATICTEXT85;
        static const long ID_CHECKBOX13;
        static const long ID_STATICTEXT86;
        static const long ID_CHECKBOX14;
        static const long ID_STATICTEXT87;
        static const long ID_CHECKBOX15;
        static const long ID_STATICTEXT88;
        static const long ID_CHECKBOX16;
        static const long ID_STATICTEXT89;
        static const long ID_CHECKBOX17;
        static const long ID_STATICTEXT90;
        static const long ID_CHECKBOX18;
        static const long ID_STATICTEXT91;
        static const long ID_CHECKBOX19;
        static const long ID_STATICTEXT92;
        static const long ID_CHECKBOX20;
        static const long ID_STATICTEXT93;
        static const long ID_CHECKBOX21;
        static const long ID_STATICTEXT94;
        static const long ID_CHECKBOX22;
        static const long ID_STATICTEXT95;
        static const long ID_CHECKBOX23;
        static const long ID_STATICTEXT96;
        static const long ID_CHECKBOX24;
        static const long ID_STATICTEXT97;
        static const long ID_CHECKBOX25;
        static const long ID_STATICTEXT98;
        static const long ID_CHECKBOX26;
        static const long ID_STATICTEXT99;
        static const long ID_CHECKBOX27;
        static const long ID_STATICTEXT100;
        static const long ID_CHECKBOX28;
        static const long ID_STATICTEXT101;
        static const long ID_CHECKBOX29;
        static const long ID_STATICTEXT102;
        static const long ID_CHECKBOX30;
        static const long ID_STATICTEXT103;
        static const long ID_CHECKBOX31;
        static const long ID_STATICTEXT104;
        static const long ID_CHECKBOX32;
        static const long ID_STATICTEXT105;
        static const long ID_CHECKBOX33;
        static const long ID_STATICTEXT106;
        static const long ID_CHECKBOX34;
        static const long ID_STATICTEXT107;
        static const long ID_CHECKBOX35;
        static const long ID_STATICTEXT108;
        static const long ID_CHECKBOX36;
        static const long ID_BUTTON29;
        static const long ID_BUTTON30;
        static const long ID_PANEL1;
        static const long ID_STATICTEXT61;
        static const long ID_TEXTCTRL59;
        static const long ID_STATICTEXT64;
        static const long ID_TEXTCTRL60;
        static const long ID_STATICTEXT62;
        static const long ID_TEXTCTRL61;
        static const long ID_STATICTEXT63;
        static const long ID_TEXTCTRL62;
        static const long ID_STATICLINE3;
        static const long ID_TEXTCTRL17;
        static const long ID_STATICTEXT71;
        static const long ID_BUTTON32;
        static const long ID_TEXTCTRL67;
        static const long ID_STATICTEXT114;
        static const long ID_CHOICE9;
        static const long ID_STATICTEXT72;
        static const long ID_CHOICE7;
        static const long ID_STATICTEXT31;
        static const long ID_CHOICE12;
        static const long ID_STATICTEXT111;
        static const long ID_STATICLINE4;
        static const long ID_TEXTCTRL49;
        static const long ID_STATICTEXT75;
        static const long ID_TEXTCTRL66;
        static const long ID_STATICTEXT76;
        static const long ID_BUTTON21;
        static const long ID_STATICLINE11;
        static const long ID_BUTTON18;
        static const long ID_TEXTCTRL50;
        static const long ID_STATICTEXT57;
        static const long ID_TEXTCTRL51;
        static const long ID_BUTTON19;
        static const long ID_TEXTCTRL56;
        static const long ID_STATICTEXT58;
        static const long ID_TEXTCTRL57;
        static const long ID_BUTTON20;
        static const long ID_TEXTCTRL58;
        static const long ID_STATICTEXT60;
        static const long ID_CHECKBOX4;
        static const long ID_BUTTON25;
        static const long ID_PANEL6;
        static const long ID_CHOICE4;
        static const long ID_STATICTEXT18;
        static const long ID_TEXTCTRL11;
        static const long ID_STATICTEXT16;
        static const long ID_CHOICE3;
        static const long ID_STATICTEXT17;
        static const long ID_CHOICE10;
        static const long ID_STATICTEXT34;
        static const long ID_CHOICE13;
        static const long ID_STATICTEXT112;
        static const long ID_STATICLINE14;
        static const long ID_STATICTEXT20;
        static const long ID_TEXTCTRL13;
        static const long ID_BUTTON6;
        static const long ID_STATICTEXT21;
        static const long ID_TEXTCTRL14;
        static const long ID_STATICTEXT22;
        static const long ID_TEXTCTRL15;
        static const long ID_STATICTEXT23;
        static const long ID_TEXTCTRL16;
        static const long ID_STATICTEXT25;
        static const long ID_TEXTCTRL18;
        static const long ID_STATICTEXT26;
        static const long ID_TEXTCTRL19;
        static const long ID_STATICTEXT28;
        static const long ID_TEXTCTRL20;
        static const long ID_CHECKBOX1;
        static const long ID_STATICLINE15;
        static const long ID_STATICTEXT73;
        static const long ID_TEXTCTRL21;
        static const long ID_STATICTEXT29;
        static const long ID_TEXTCTRL46;
        static const long ID_STATICTEXT74;
        static const long ID_TEXTCTRL65;
        static const long ID_STATICTEXT56;
        static const long ID_TEXTCTRL64;
        static const long ID_BUTTON24;
        static const long ID_STATICLINE16;
        static const long ID_STATICTEXT7;
        static const long ID_TEXTCTRL23;
        static const long ID_BUTTON15;
        static const long ID_TEXTCTRL24;
        static const long ID_BUTTON16;
        static const long ID_TEXTCTRL45;
        static const long ID_BUTTON17;
        static const long ID_STATICTEXT77;
        static const long ID_TEXTCTRL12;
        static const long ID_STATICLINE12;
        static const long ID_STATICTEXT115;
        static const long ID_COMBOBOX4;
        static const long ID_BUTTON33;
        static const long ID_STATICTEXT69;
        static const long ID_COMBOBOX3;
        static const long ID_PANEL2;
        static const long ID_GRID1;
        static const long ID_BUTTON7;
        static const long ID_PANEL4;
        static const long ID_GRID2;
        static const long ID_BUTTON11;
        static const long ID_PANEL5;
        static const long ID_STATICTEXT35;
        static const long ID_TEXTCTRL30;
        static const long ID_STATICTEXT33;
        static const long ID_TEXTCTRL26;
        static const long ID_STATICTEXT36;
        static const long ID_TEXTCTRL27;
        static const long ID_STATICTEXT37;
        static const long ID_TEXTCTRL28;
        static const long ID_STATICTEXT38;
        static const long ID_TEXTCTRL29;
        static const long ID_STATICTEXT39;
        static const long ID_TEXTCTRL31;
        static const long ID_STATICTEXT40;
        static const long ID_TEXTCTRL32;
        static const long ID_STATICTEXT41;
        static const long ID_TEXTCTRL33;
        static const long ID_STATICTEXT42;
        static const long ID_TEXTCTRL34;
        static const long ID_STATICTEXT43;
        static const long ID_TEXTCTRL35;
        static const long ID_STATICTEXT44;
        static const long ID_TEXTCTRL36;
        static const long ID_STATICTEXT45;
        static const long ID_TEXTCTRL37;
        static const long ID_STATICTEXT46;
        static const long ID_TEXTCTRL38;
        static const long ID_STATICTEXT47;
        static const long ID_TEXTCTRL39;
        static const long ID_STATICTEXT48;
        static const long ID_TEXTCTRL40;
        static const long ID_STATICTEXT49;
        static const long ID_TEXTCTRL41;
        static const long ID_CHOICE6;
        static const long ID_STATICTEXT52;
        static const long ID_CHOICE5;
        static const long ID_STATICTEXT50;
        static const long ID_STATICTEXT53;
        static const long ID_TEXTCTRL42;
        static const long ID_STATICTEXT54;
        static const long ID_TEXTCTRL43;
        static const long ID_STATICTEXT59;
        static const long ID_TEXTCTRL48;
        static const long ID_BUTTON12;
        static const long ID_TEXTCTRL44;
        static const long ID_STATICTEXT65;
        static const long ID_TEXTCTRL54;
        static const long ID_BUTTON13;
        static const long ID_TEXTCTRL52;
        static const long ID_STATICTEXT66;
        static const long ID_TEXTCTRL55;
        static const long ID_BUTTON14;
        static const long ID_TEXTCTRL53;
        static const long ID_STATICTEXT67;
        static const long ID_CHECKBOX3;
        static const long ID_BUTTON10;
        static const long ID_PANEL3;
        static const long ID_NOTEBOOK1;
        static const long ID_STATICLINE1;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL22;
        static const long ID_STATICTEXT2;
        static const long ID_STATICLINE7;
        static const long ID_STATICTEXT24;
        static const long ID_BUTTON4;
        static const long ID_COMBOBOX1;
        static const long ID_STATICTEXT51;
        static const long ID_COMBOBOX2;
        static const long ID_BUTTON1;
        static const long ID_BUTTON8;
        static const long ID_BUTTON2;
        static const long ID_STATICLINE8;
        static const long ID_STATICTEXT55;
        static const long ID_STATICTEXT32;
        static const long ID_TEXTCTRL47;
        static const long ID_STATICTEXT70;
        static const long ID_TEXTCTRL63;
        static const long ID_BUTTON22;
        static const long ID_BUTTON23;
        static const long ID_STATICLINE9;
        static const long ID_STATICLINE10;
        static const long ID_STATICTEXT30;
        static const long ID_BUTTON9;
        static const long ID_CHECKBOX2;
        static const long ID_STATICLINE6;
        static const long ID_BUTTON3;
        static const long ID_TIMER1;
        static const long ID_TIMER2;
        //*)

        //(*Declarations(loggerDialog)
        wxButton* Button10;
        wxButton* Button11;
        wxButton* Button12;
        wxButton* Button13;
        wxButton* Button14;
        wxButton* Button15;
        wxButton* Button16;
        wxButton* Button17;
        wxButton* Button18;
        wxButton* Button19;
        wxButton* Button1;
        wxButton* Button20;
        wxButton* Button21;
        wxButton* Button22;
        wxButton* Button23;
        wxButton* Button24;
        wxButton* Button25;
        wxButton* Button26;
        wxButton* Button27;
        wxButton* Button28;
        wxButton* Button29;
        wxButton* Button2;
        wxButton* Button30;
        wxButton* Button31;
        wxButton* Button32;
        wxButton* Button33;
        wxButton* Button3;
        wxButton* Button4;
        wxButton* Button5;
        wxButton* Button6;
        wxButton* Button7;
        wxButton* Button8;
        wxButton* Button9;
        wxCheckBox* CheckBox10;
        wxCheckBox* CheckBox11;
        wxCheckBox* CheckBox12;
        wxCheckBox* CheckBox13;
        wxCheckBox* CheckBox14;
        wxCheckBox* CheckBox15;
        wxCheckBox* CheckBox16;
        wxCheckBox* CheckBox17;
        wxCheckBox* CheckBox18;
        wxCheckBox* CheckBox19;
        wxCheckBox* CheckBox1;
        wxCheckBox* CheckBox20;
        wxCheckBox* CheckBox21;
        wxCheckBox* CheckBox22;
        wxCheckBox* CheckBox23;
        wxCheckBox* CheckBox24;
        wxCheckBox* CheckBox25;
        wxCheckBox* CheckBox26;
        wxCheckBox* CheckBox27;
        wxCheckBox* CheckBox28;
        wxCheckBox* CheckBox29;
        wxCheckBox* CheckBox2;
        wxCheckBox* CheckBox30;
        wxCheckBox* CheckBox31;
        wxCheckBox* CheckBox32;
        wxCheckBox* CheckBox33;
        wxCheckBox* CheckBox34;
        wxCheckBox* CheckBox35;
        wxCheckBox* CheckBox36;
        wxCheckBox* CheckBox3;
        wxCheckBox* CheckBox4;
        wxCheckBox* CheckBox5;
        wxCheckBox* CheckBox6;
        wxCheckBox* CheckBox7;
        wxCheckBox* CheckBox8;
        wxCheckBox* CheckBox9;
        wxChoice* Choice10;
        wxChoice* Choice11;
        wxChoice* Choice12;
        wxChoice* Choice13;
        wxChoice* Choice1;
        wxChoice* Choice2;
        wxChoice* Choice3;
        wxChoice* Choice4;
        wxChoice* Choice5;
        wxChoice* Choice6;
        wxChoice* Choice7;
        wxChoice* Choice8;
        wxChoice* Choice9;
        wxComboBox* ComboBox1;
        wxComboBox* ComboBox2;
        wxComboBox* ComboBox3;
        wxComboBox* ComboBox4;
        wxGrid* Grid1;
        wxGrid* Grid2;
        wxNotebook* Notebook1;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        wxPanel* Panel4;
        wxPanel* Panel5;
        wxPanel* Panel6;
        wxRadioBox* RadioBox1;
        wxStaticLine* StaticLine10;
        wxStaticLine* StaticLine11;
        wxStaticLine* StaticLine12;
        wxStaticLine* StaticLine13;
        wxStaticLine* StaticLine14;
        wxStaticLine* StaticLine15;
        wxStaticLine* StaticLine16;
        wxStaticLine* StaticLine17;
        wxStaticLine* StaticLine18;
        wxStaticLine* StaticLine19;
        wxStaticLine* StaticLine1;
        wxStaticLine* StaticLine2;
        wxStaticLine* StaticLine3;
        wxStaticLine* StaticLine4;
        wxStaticLine* StaticLine5;
        wxStaticLine* StaticLine6;
        wxStaticLine* StaticLine7;
        wxStaticLine* StaticLine8;
        wxStaticLine* StaticLine9;
        wxStaticText* StaticText100;
        wxStaticText* StaticText101;
        wxStaticText* StaticText102;
        wxStaticText* StaticText103;
        wxStaticText* StaticText104;
        wxStaticText* StaticText105;
        wxStaticText* StaticText106;
        wxStaticText* StaticText107;
        wxStaticText* StaticText108;
        wxStaticText* StaticText109;
        wxStaticText* StaticText10;
        wxStaticText* StaticText110;
        wxStaticText* StaticText111;
        wxStaticText* StaticText112;
        wxStaticText* StaticText113;
        wxStaticText* StaticText114;
        wxStaticText* StaticText115;
        wxStaticText* StaticText11;
        wxStaticText* StaticText12;
        wxStaticText* StaticText13;
        wxStaticText* StaticText14;
        wxStaticText* StaticText15;
        wxStaticText* StaticText16;
        wxStaticText* StaticText17;
        wxStaticText* StaticText18;
        wxStaticText* StaticText19;
        wxStaticText* StaticText1;
        wxStaticText* StaticText20;
        wxStaticText* StaticText21;
        wxStaticText* StaticText22;
        wxStaticText* StaticText23;
        wxStaticText* StaticText24;
        wxStaticText* StaticText25;
        wxStaticText* StaticText26;
        wxStaticText* StaticText27;
        wxStaticText* StaticText28;
        wxStaticText* StaticText29;
        wxStaticText* StaticText2;
        wxStaticText* StaticText30;
        wxStaticText* StaticText31;
        wxStaticText* StaticText32;
        wxStaticText* StaticText33;
        wxStaticText* StaticText34;
        wxStaticText* StaticText35;
        wxStaticText* StaticText36;
        wxStaticText* StaticText37;
        wxStaticText* StaticText38;
        wxStaticText* StaticText39;
        wxStaticText* StaticText3;
        wxStaticText* StaticText40;
        wxStaticText* StaticText41;
        wxStaticText* StaticText42;
        wxStaticText* StaticText43;
        wxStaticText* StaticText44;
        wxStaticText* StaticText45;
        wxStaticText* StaticText46;
        wxStaticText* StaticText47;
        wxStaticText* StaticText48;
        wxStaticText* StaticText49;
        wxStaticText* StaticText4;
        wxStaticText* StaticText50;
        wxStaticText* StaticText51;
        wxStaticText* StaticText52;
        wxStaticText* StaticText53;
        wxStaticText* StaticText54;
        wxStaticText* StaticText55;
        wxStaticText* StaticText56;
        wxStaticText* StaticText57;
        wxStaticText* StaticText58;
        wxStaticText* StaticText59;
        wxStaticText* StaticText5;
        wxStaticText* StaticText60;
        wxStaticText* StaticText61;
        wxStaticText* StaticText62;
        wxStaticText* StaticText63;
        wxStaticText* StaticText64;
        wxStaticText* StaticText65;
        wxStaticText* StaticText66;
        wxStaticText* StaticText67;
        wxStaticText* StaticText68;
        wxStaticText* StaticText69;
        wxStaticText* StaticText6;
        wxStaticText* StaticText70;
        wxStaticText* StaticText71;
        wxStaticText* StaticText72;
        wxStaticText* StaticText73;
        wxStaticText* StaticText74;
        wxStaticText* StaticText75;
        wxStaticText* StaticText76;
        wxStaticText* StaticText77;
        wxStaticText* StaticText78;
        wxStaticText* StaticText79;
        wxStaticText* StaticText7;
        wxStaticText* StaticText80;
        wxStaticText* StaticText81;
        wxStaticText* StaticText82;
        wxStaticText* StaticText83;
        wxStaticText* StaticText84;
        wxStaticText* StaticText85;
        wxStaticText* StaticText86;
        wxStaticText* StaticText87;
        wxStaticText* StaticText88;
        wxStaticText* StaticText89;
        wxStaticText* StaticText8;
        wxStaticText* StaticText90;
        wxStaticText* StaticText91;
        wxStaticText* StaticText92;
        wxStaticText* StaticText93;
        wxStaticText* StaticText94;
        wxStaticText* StaticText95;
        wxStaticText* StaticText96;
        wxStaticText* StaticText97;
        wxStaticText* StaticText98;
        wxStaticText* StaticText99;
        wxStaticText* StaticText9;
        wxTextCtrl* TextCtrl10;
        wxTextCtrl* TextCtrl11;
        wxTextCtrl* TextCtrl12;
        wxTextCtrl* TextCtrl13;
        wxTextCtrl* TextCtrl14;
        wxTextCtrl* TextCtrl15;
        wxTextCtrl* TextCtrl16;
        wxTextCtrl* TextCtrl17;
        wxTextCtrl* TextCtrl18;
        wxTextCtrl* TextCtrl19;
        wxTextCtrl* TextCtrl1;
        wxTextCtrl* TextCtrl20;
        wxTextCtrl* TextCtrl21;
        wxTextCtrl* TextCtrl22;
        wxTextCtrl* TextCtrl23;
        wxTextCtrl* TextCtrl24;
        wxTextCtrl* TextCtrl25;
        wxTextCtrl* TextCtrl26;
        wxTextCtrl* TextCtrl27;
        wxTextCtrl* TextCtrl28;
        wxTextCtrl* TextCtrl29;
        wxTextCtrl* TextCtrl2;
        wxTextCtrl* TextCtrl30;
        wxTextCtrl* TextCtrl31;
        wxTextCtrl* TextCtrl32;
        wxTextCtrl* TextCtrl33;
        wxTextCtrl* TextCtrl34;
        wxTextCtrl* TextCtrl35;
        wxTextCtrl* TextCtrl36;
        wxTextCtrl* TextCtrl37;
        wxTextCtrl* TextCtrl38;
        wxTextCtrl* TextCtrl39;
        wxTextCtrl* TextCtrl3;
        wxTextCtrl* TextCtrl40;
        wxTextCtrl* TextCtrl41;
        wxTextCtrl* TextCtrl42;
        wxTextCtrl* TextCtrl43;
        wxTextCtrl* TextCtrl44;
        wxTextCtrl* TextCtrl45;
        wxTextCtrl* TextCtrl46;
        wxTextCtrl* TextCtrl47;
        wxTextCtrl* TextCtrl48;
        wxTextCtrl* TextCtrl49;
        wxTextCtrl* TextCtrl4;
        wxTextCtrl* TextCtrl50;
        wxTextCtrl* TextCtrl51;
        wxTextCtrl* TextCtrl52;
        wxTextCtrl* TextCtrl53;
        wxTextCtrl* TextCtrl54;
        wxTextCtrl* TextCtrl55;
        wxTextCtrl* TextCtrl56;
        wxTextCtrl* TextCtrl57;
        wxTextCtrl* TextCtrl58;
        wxTextCtrl* TextCtrl59;
        wxTextCtrl* TextCtrl5;
        wxTextCtrl* TextCtrl60;
        wxTextCtrl* TextCtrl61;
        wxTextCtrl* TextCtrl62;
        wxTextCtrl* TextCtrl63;
        wxTextCtrl* TextCtrl64;
        wxTextCtrl* TextCtrl65;
        wxTextCtrl* TextCtrl66;
        wxTextCtrl* TextCtrl67;
        wxTextCtrl* TextCtrl6;
        wxTextCtrl* TextCtrl7;
        wxTextCtrl* TextCtrl8;
        wxTextCtrl* TextCtrl9;
        wxTimer Timer1;
        wxTimer Timer2;
        //*)

        DECLARE_EVENT_TABLE()
};



#endif // LOGGERMAIN_H
