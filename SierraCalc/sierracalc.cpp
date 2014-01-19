/************************************************************************
Sierra Wireless Calculator (v1.00)

Copyright (C) 2014  B. Kerler
                             info <info@revskills.de>

This code is licensed to you under the terms of the GNU GPL, version 2;
see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*************************************************************************/

#include "sierracalc.h"
#include <qmessagebox.h>
#include <QtGui>
#include <QGridLayout>
#include <QtWidgets>

unsigned char tbl47BDE8[0x105]={0};

unsigned int SierraCalc1(unsigned int counter, unsigned char* prodkey, unsigned int intval, unsigned char& challengelen, unsigned int& mcount)
{
  unsigned int i;
  unsigned int tmp2;
  
  int tmp1;
  int tmp3;

  if ( counter )
  {
    tmp2 = 0;
    for ( i = 1; i < counter; i = 2 * i + 1 );
    do
    {
      tmp1 = mcount++;
      challengelen = (prodkey[tmp1] + tbl47BDE8[challengelen+5])&0xFF;
      if ( mcount >= intval )
      {
        mcount = 0;
        challengelen += intval;
      }
      tmp3 = (challengelen & i)&0xFF;
      tmp2++;
      if ( tmp2 > 0xB )
        //div(counter, tmp3); // Careful, that was in arm firmware
			tmp3 %= counter; //In new algo ...weird, results are the same
    }
    while ( (unsigned int)tmp3 > (unsigned int)counter );
    counter = (unsigned char)tmp3;
  }
  return counter;
}


int SierraCalcV2(unsigned char challenge)
{
  int v2; // r0@1
  int v3; // r4@1
  int v4; // r2@1
  int v5; // r5@1
  int v6; // r3@1
  int v7; // r6@1
  int v8; // r0@1
  int result; // r0@1

  v2 = tbl47BDE8[0];
  v3 = (tbl47BDE8[0]+1) & 0xFF;
  tbl47BDE8[0] = tbl47BDE8[0]++;
  v4 = (tbl47BDE8[v2+5] + tbl47BDE8[1]) & 0xFF;
  tbl47BDE8[1] += tbl47BDE8[v2+5];
  v5 = tbl47BDE8[4];
  v6 = tbl47BDE8[tbl47BDE8[4]+5];
  tbl47BDE8[v5+5] = tbl47BDE8[v4+5];
  v7 = tbl47BDE8[3];
  tbl47BDE8[v4+5] = tbl47BDE8[tbl47BDE8[3]+5];
  tbl47BDE8[v7+5] = tbl47BDE8[v3+5];
  tbl47BDE8[v3+5] = v6;
  v8 = (tbl47BDE8[v6+5] + tbl47BDE8[2])& 0xFF;
  tbl47BDE8[2] += tbl47BDE8[v6+5];
  int v=((tbl47BDE8[v7+5] + tbl47BDE8[v5+5] + tbl47BDE8[v8+5]) & 0xFF);
  int tmm=tbl47BDE8[v+5];
  int u=(tbl47BDE8[v4+5] + v6) & 0xFF;
  result = tbl47BDE8[tmm+5] ^ tbl47BDE8[u+5] ^ challenge;
  tbl47BDE8[3] = result;
  tbl47BDE8[4] = challenge;
  return result;
}

int SierraCalcV3(unsigned char challenge)
{
    int v2; // r0@1
  int v3; // r4@1
  int v4; // r2@1
  int v5; // r5@1
  int v6; // r3@1
  int v7; // r6@1
  int result; // r0@1

  int v1 = tbl47BDE8[3];
  v3 = (unsigned __int8)tbl47BDE8[2];
  v4 = tbl47BDE8[(unsigned __int8)tbl47BDE8[2]+5];
  tbl47BDE8[1] += tbl47BDE8[(unsigned __int8)tbl47BDE8[3]+5];
  v2 = tbl47BDE8[1];
  tbl47BDE8[(unsigned __int8)tbl47BDE8[2]+5] = tbl47BDE8[(unsigned __int8)tbl47BDE8[1]+5];
  ++v1;
  v5 = (unsigned __int8)tbl47BDE8[0];
  tbl47BDE8[(unsigned __int8)v2+5] = tbl47BDE8[(unsigned __int8)tbl47BDE8[0]+5];
  v6 = (unsigned __int8)v1;
  tbl47BDE8[3] = v1;
  tbl47BDE8[v5+5] = tbl47BDE8[(unsigned __int8)v1+5];
  v7 = tbl47BDE8[4];
  tbl47BDE8[v6+5] = v4;
  v3 = tbl47BDE8[v3+5];
  tbl47BDE8[4] = tbl47BDE8[(unsigned __int8)v4+5] + v7;
  result = challenge ^ tbl47BDE8[(unsigned __int8)(v4 + tbl47BDE8[(unsigned __int8)v2+5])+5] ^ tbl47BDE8[(unsigned __int8)tbl47BDE8[(unsigned __int8)((unsigned char)v3 + tbl47BDE8[v5+5] + tbl47BDE8[(unsigned __int8)tbl47BDE8[4]+5])+5]+5];
  tbl47BDE8[2] = result;
  tbl47BDE8[0] = challenge;
 
  return result;
}

int SierraInitV3(unsigned char* prodkey, unsigned int intval, unsigned int& mcount, unsigned char& challengelen)
{
  int result=0;

  if ( intval >= 1 && intval <= 0x20 )
  {
    for (int i=0;i<0x100;i++)
    {
      tbl47BDE8[i+5] = (unsigned char)i;
    }
    mcount = 0;
    //LOBYTE(challengelen) = 0;
	challengelen=00;

    for (int i=0xFF;i>=0;i--)
    {
      unsigned char t= SierraCalc1(i, prodkey, intval, challengelen, mcount);
      unsigned char m = tbl47BDE8[i+5];
      tbl47BDE8[i+5] = tbl47BDE8[t+5];
      tbl47BDE8[t+5] = m;
    }
    /*tbl47BDE8[0]=(unsigned char)tbl47BDE8[6]; //old algo
    tbl47BDE8[1]=(unsigned char)tbl47BDE8[8]; 
    tbl47BDE8[2]=(unsigned char)tbl47BDE8[0xA]; 
    tbl47BDE8[3]=(unsigned char)tbl47BDE8[0xC]; 
    tbl47BDE8[4]=(unsigned char)tbl47BDE8[challengelen+5]; */
	
	tbl47BDE8[3]=(unsigned char)tbl47BDE8[6]; //new algo
    tbl47BDE8[1]=(unsigned char)tbl47BDE8[8]; 
    tbl47BDE8[4]=(unsigned char)tbl47BDE8[0xA]; 
    tbl47BDE8[0]=(unsigned char)tbl47BDE8[0xC]; 
    tbl47BDE8[2]=(unsigned char)tbl47BDE8[challengelen+5]; 

    mcount = 0;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}


int SierraInitV2(unsigned char* prodkey, unsigned int intval, unsigned int& mcount, unsigned char& challengelen)
{
  int result=0;

  if ( intval >= 1 && intval <= 0x20 )
  {
    for (int i=0;i<0x100;i++)
    {
      tbl47BDE8[i+5] = (unsigned char)i;
    }
    mcount = 0;
    //LOBYTE(challengelen) = 0;
	challengelen=00;

    for (int i=0xFF;i>=0;i--)
    {
      unsigned char t= SierraCalc1(i, prodkey, intval, challengelen, mcount);
      unsigned char m = tbl47BDE8[i+5];
      tbl47BDE8[i+5] = tbl47BDE8[t+5];
      tbl47BDE8[t+5] = m;
    }
    
	tbl47BDE8[0]=(unsigned char)tbl47BDE8[6]; //old algo
    tbl47BDE8[1]=(unsigned char)tbl47BDE8[8]; 
    tbl47BDE8[2]=(unsigned char)tbl47BDE8[0xA]; 
    tbl47BDE8[3]=(unsigned char)tbl47BDE8[0xC]; 
    tbl47BDE8[4]=(unsigned char)tbl47BDE8[challengelen+5];
	
    mcount = 0;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

unsigned char *SierraFinish()
{
  unsigned char *result;

  for (int i=0;i<256;i++)
    tbl47BDE8[i + 5] = 0;

  result = tbl47BDE8;
  tbl47BDE8[4] = 0;
  tbl47BDE8[3] = 0;
  tbl47BDE8[2] = 0;
  tbl47BDE8[1] = 0;
  tbl47BDE8[0] = 0;
  return result;
}

unsigned char* SierraKeygen(unsigned char* challenge, /*unsigned char* resultbuffer,*/ unsigned char* prodtable, unsigned char challengelen, unsigned int mcount, int mode)
{
  int result;
  if (mode==2)  result = SierraInitV2(prodtable, (unsigned char)mcount, mcount, challengelen);
  else if (mode==3) result = SierraInitV3(prodtable, (unsigned char)mcount, mcount, challengelen);

  unsigned char* resultbuffer=(unsigned char*)malloc(challengelen);
  if ( result )
  {
    for ( int i = 0; i < challengelen; i++ )
	{
      if (mode==2) resultbuffer[i] = SierraCalcV2(challenge[i]);
	  else if (mode==3) resultbuffer[i] = SierraCalcV3(challenge[i]);
	}
    SierraFinish();
    result = 1;
  }
  return resultbuffer;
}

bool SierraCalc::GetChallenge(QString& challenge, int& mode, int& key)
{
	QDialog *h=new QDialog(this);
	QLabel *label=new QLabel(h);
	QPushButton *b=new QPushButton("&OK",h);
	QLineEdit *edit=new QLineEdit(h);
	QCheckBox *check=new QCheckBox("New Firmware V3",h);
	QComboBox *combobox=new QComboBox();
	QGridLayout* mainGrid = new QGridLayout;
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
	topLayout->addWidget(edit);
	topLayout->addWidget(b);
	topLayout->addWidget(check);
	topLayout->addWidget(combobox);
	
	combobox->addItem("MC8765V",0);
	combobox->addItem("MC8765 MEP",0x20);
	combobox->addItem("MC8765",0);
	combobox->addItem("MC8755V",0);
	combobox->addItem("MC8775",0);
	combobox->addItem("MC8775V",0);
	combobox->addItem("MC8775",0);
	combobox->addItem("AC850",0);
	combobox->addItem("AC860",0);
	combobox->addItem("AC875",0);
	combobox->addItem("AC881",0);
	combobox->addItem("AC881U",0);
	combobox->addItem("AC875",0);
	combobox->addItem("AC750",0x30);
	combobox->addItem("AC710",0x30);
	combobox->addItem("AC7XX",0x30);
	combobox->addItem("SB750A",0x30);
	combobox->addItem("SB750",0x30);
	combobox->addItem("PC7000",0x30);
	combobox->addItem("AC313u",0x30);

    mainGrid->addLayout(topLayout,0,0);
	h->setLayout(mainGrid);

	label->setText(tr("Enter challenge in hex bytes :"));
	edit->setText("39790946DB902479");
    
    connect(b,SIGNAL(clicked()),h,SLOT(accept()));
    if (h->exec()==QDialog::Rejected) return false;
	challenge=edit->text();
	if (check->isChecked()) mode=3;
	else mode=2;
	key=combobox->currentData().toInt();
	return true;
}

SierraCalc::SierraCalc(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	int mode=0;
	int key=0;
	QString challenge="";
	if (!GetChallenge(challenge,mode,key)) return;

	unsigned char challengearray[8]={0};
	
	for (int i=0;i<8;i++) challengearray[i]=(unsigned char)challenge.mid(i*2,2).toInt(0,16);

	unsigned char prodkey[]=
	{
		0xF0, 0x14, 0x55, 0x0D, 0x5E, 0xDA, 0x92, 0xB3, 0xA7, 0x6C, 0xCE, 0x84, 0x90, 0xBC, 0x7F, 0xED,//MC8775_H2.0.8.19 !OPENLOCK, !OPENCNT .. MC8765V,MC8765,MC8755V,MC8775,MC8775V,MC8775,AC850,AC860,AC875,AC881,AC881U,AC875
		0x22, 0x63, 0x48, 0x02, 0x24, 0x72, 0x27, 0x37, 0x10, 0x26, 0x37, 0x50, 0xBE, 0xEF, 0xCA, 0xFE,//MC8775_H2.0.8.19 
		0x61, 0x94, 0xCE, 0xA7, 0xB0, 0xEA, 0x4F, 0x0A, 0x73, 0xC5, 0xC3, 0xA6, 0x5E, 0xEC, 0x1C, 0xE2,//MC8775_H2.0.8.19 !OPENMEP
		0x39, 0xC6, 0x7B, 0x04, 0xCA, 0x50, 0x82, 0x1F, 0x19, 0x63, 0x36, 0xDE, 0x81, 0x49, 0xF0, 0xD7, //AC750,AC710,AC7XX,SB750A,SB750,PC7000,AC313u
		0xDE, 0xA5, 0xAD, 0x2E, 0xBE, 0xE1, 0xC9, 0xEF,	0xCA, 0xF9, 0xFE, 0x1F, 0x17, 0xFE, 0xED, 0x3B, //AC775,PC7200
		0x95, 0xA1, 0x02, 0x77, 0xCC, 0x34, 0x12, 0x3C, 0x17, 0x29, 0xAE, 0x91, 0x66, 0xCE, 0x75, 0xA5,
		0x61, 0x94, 0xCE, 0xA7, 0xB0, 0xEA, 0x4F, 0x0A, 0x73, 0xC5, 0xC3, 0xA6, 0x5E, 0xEC, 0x1C, 0xE2
	};


	unsigned char *resultbuffer=SierraKeygen(challengearray,&prodkey[key],sizeof(challengearray),16,mode);
	QString tmp="";
	QString answer="";
	for (int i=0;i<8;i++)
	{
		tmp.sprintf("%02X",resultbuffer[i]);
		answer+=tmp;
	}
	if (key!=0x20) answer="AT!OPENLOCK="+answer;
	else answer="AT!OPENMEP="+answer;
	QMessageBox::information(this,tr("Here are the results"),answer);
	exit(0);
}

SierraCalc::~SierraCalc()
{

}

