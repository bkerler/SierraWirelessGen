#include "sierracalc.h"

SierraCalc::SierraCalc(QWidget *parent) :
    QMainWindow(parent)

{
    setupUi(this);
    this->setWindowTitle(QApplication::applicationName()+" V1.2 (c) B.Kerler 2018");
    comboBox->addItem("MC8765V",0);
    comboBox->addItem("MC8765 MEP",0x20);
    comboBox->addItem("MC8765",0);
    comboBox->addItem("MC8755V",0);
    comboBox->addItem("MC8775",0);
    comboBox->addItem("MC8775V",0);
    comboBox->addItem("MC8775",0);
    comboBox->addItem("MC7430",0); //5
    comboBox->addItem("MC7455",0); //5
    comboBox->addItem("MC7710",0); //5
    comboBox->addItem("MC7304",0); //5, 9x15
    comboBox->addItem("AC850",0);
    comboBox->addItem("AC860",0);
    comboBox->addItem("AC875",0);
    comboBox->addItem("AC881",0);
    comboBox->addItem("AC881U",0);
    comboBox->addItem("AC875",0);
    comboBox->addItem("AC810",0x30);
    comboBox->addItem("AC790",0x30);
    comboBox->addItem("AC785",0x30);
    comboBox->addItem("AC750",0x30);
    comboBox->addItem("AC710",0x30);
    comboBox->addItem("AC7XX",0x30);
    comboBox->addItem("SB750A",0x30);
    comboBox->addItem("SB750",0x30);
    comboBox->addItem("PC7000",0x30);
    comboBox->addItem("AC313u",0x30);
    mode2->setChecked(true);
}

SierraCalc::~SierraCalc()
{

}

static unsigned char tbl[0x105]={0};

unsigned int SierraCalc1(unsigned int counter, unsigned char* prodkey, unsigned int intval, unsigned char& challengelen, unsigned int& mcount)
{
  unsigned int i;
  unsigned int tmp2;
  unsigned int tmp3;

  if ( counter )
  {
    tmp2 = 0;
    for ( i = 1; i < counter; i = 2 * i + 1 );
    do
    {
      challengelen = (prodkey[mcount] + tbl[challengelen+5])&0xFF;
      mcount++;
      if ( mcount >= intval )
      {
        mcount = 0;
        challengelen += intval;
      }
      tmp3 = (challengelen & i)&0xFF;
      tmp2++;
      if ( tmp2 >= 0xB )
        //div(counter, tmp3); // Careful, that was in arm firmware
            tmp3 %= counter; //In new algo ...weird, results are the same
    }
    while ( tmp3 > counter );
    counter = tmp3;
  }
  return counter;
}

unsigned int SierraCalc5(unsigned int counter, unsigned char* prodkey, unsigned int intval, unsigned char& challengelen, unsigned int& mcount)
{
  unsigned int i;
  unsigned int tmp2;

  unsigned int tmp3;

  if ( counter )
  {
    tmp2 = 0;
    i = 1;
    if (counter>1)
    {
        for ( i = 1; i < counter; i = (2 * i) + 1 );
    }
    
    do
    {
      challengelen = (prodkey[mcount] + tbl[challengelen+8])&0xFF;
      mcount++;
      if ( mcount >= intval )
      {
        mcount = 0;
        challengelen += intval&0xFF;
      }
      tmp2++;
      tmp3 = (challengelen & i)&0xFF;
      if ( tmp2 >= 0xB )
        //div(counter, tmp3); // Careful, that was in arm firmware
            tmp3 %= counter; //In new algo ...weird, results are the same
    }
    while ( tmp3 > counter );
    counter = tmp3;
  }
  return counter;
}


int SierraCalcV2(unsigned char challenge)
{
  int v2; // r0@1
  int v3; // r4@1
  int v4; // r2@1
  int v5; // r5@1
  unsigned char v6; // r3@1
  int v7; // r6@1
  int v8; // r0@1
  unsigned char result; // r0@1

  v2 = tbl[0];
  v3 = (tbl[0]+1) & 0xFF;
  tbl[0] = tbl[0]++;
  v4 = (tbl[v2+5] + tbl[1]) & 0xFF;
  tbl[1] += tbl[v2+5];
  v5 = tbl[4];
  v6 = tbl[tbl[4]+5];
  tbl[v5+5] = tbl[v4+5];
  v7 = tbl[3];
  tbl[v4+5] = tbl[tbl[3]+5];
  tbl[v7+5] = tbl[v3+5];
  tbl[v3+5] = v6;
  v8 = (tbl[v6+5] + tbl[2])& 0xFF;
  tbl[2] += tbl[v6+5];
  int v=((tbl[v7+5] + tbl[v5+5] + tbl[v8+5]) & 0xFF);
  int tmm=tbl[v+5];
  int u=(tbl[v4+5] + v6) & 0xFF;
  result = tbl[tmm+5] ^ tbl[u+5] ^ challenge;
  tbl[3] = result;
  tbl[4] = challenge;
  return result;
}

int SierraCalcV3(unsigned char challenge)
{
  int v2; // r0@1
  int v3; // r4@1
  unsigned char v4; // r2@1
  int v5; // r5@1
  int v6; // r3@1
  unsigned char v7; // r6@1
  unsigned char result; // r0@1

  unsigned char v1 = tbl[3];
  v3 = tbl[2];
  v4 = tbl[tbl[2]+5];
  tbl[1] += tbl[tbl[3]+5];
  v2 = tbl[1];
  tbl[tbl[2]+5] = tbl[tbl[1]+5];
  ++v1;
  v5 = tbl[0];
  tbl[v2+5] = tbl[tbl[0]+5];
  v6 = v1;
  tbl[3] = v1;
  tbl[v5+5] = tbl[v1+5];
  v7 = tbl[4];
  tbl[v6+5] = v4;
  v3 = tbl[v3+5];
  tbl[4] = tbl[v4+5] + v7;
  result = challenge ^ tbl[(v4 + tbl[v2+5])+5] ^ tbl[tbl[(v3 + tbl[v5+5] + tbl[tbl[4]+5])+5]+5];
  tbl[2] = result;
  tbl[0] = challenge;

  return result;
}

int SierraCalcV5(unsigned char challenge)
{
    unsigned char cl = tbl[3];
    unsigned char dl = tbl[1] + tbl[cl + 8];
    unsigned char bl = tbl[tbl[2] + 8];
    tbl[1] = dl;
    tbl[tbl[2] + 8] = tbl[dl + 8];
    cl = cl + 1;
    tbl[dl + 8] = tbl[tbl[0] + 8];
    tbl[3] = cl;
    tbl[tbl[0] + 8] = tbl[cl + 8];
    tbl[cl + 8] = bl;
    unsigned char cl1 = tbl[4] + tbl[bl + 8];
    tbl[4] = cl1;
    unsigned char result = tbl[tbl[(((tbl[cl1 + 8] + tbl[tbl[0] + 8]) + tbl[tbl[2] + 8]) & 0xff)+8]+8] ^ tbl[((tbl[dl + 8] + bl) & 0xff) + 8] ^ challenge;
    tbl[2] = result;
    tbl[0] = challenge;
    return result;
}

int SierraCalcV4(unsigned char challenge)
{
  unsigned char r2;
  int r5;
  unsigned char r3;
  unsigned char r4;
  unsigned char r6;

  r6 = tbl[4]+1;
  r5 = tbl[1];
  r2 = (tbl[r6] + tbl[2])&0xFF;
  r6 = r6 & 0xFF;
  r3 = tbl[r5];
  tbl[2] = r2;
  tbl[r5] = tbl[r2];
  tbl[4] = r6;
  tbl[1] = challenge;
  tbl[r2] = tbl[tbl[3]];
  tbl[tbl[3]] = tbl[r6];
  tbl[r6] = r3;
  r4 = (tbl[r3]+tbl[0]) & 0xFF;
  tbl[0] = r4;
  r2 = (tbl[r4]+tbl[tbl[r4]]+tbl[r4])&0xFF;
  r2 = (tbl[tbl[r2]] ^ (challenge^tbl[(tbl[r2]+r3)&0xFF]))&0xFF;
  tbl[3] = r2;
  return r2;
}

int SierraInitV3(unsigned char* prodkey, unsigned int intval, unsigned int& mcount, unsigned char& challengelen)
{
  int result=0;

  if ( intval >= 1 && intval <= 0x20 )
  {
    for (int i=0;i<0x100;i++)
    {
      tbl[i+5] = i&0xFF;
    }
    mcount = 0;
    //LOBYTE(challengelen) = 0;
    challengelen=0;

    for (int i=0xFF;i>=0;i--)
    {
      unsigned char t= static_cast<unsigned char>(SierraCalc1(static_cast<unsigned char>(i), prodkey, intval, challengelen, mcount));
      unsigned char m = tbl[i+5];
      tbl[i+5] = tbl[t+5];
      tbl[t+5] = m;
    }
    /*tbl[0]=(unsigned char)tbl[6]; //old algo
    tbl[1]=(unsigned char)tbl[8];
    tbl[2]=(unsigned char)tbl[0xA];
    tbl[3]=(unsigned char)tbl[0xC];
    tbl[4]=(unsigned char)tbl[challengelen+5]; */

    tbl[3]=tbl[6]; //new algo
    tbl[1]=tbl[8];
    tbl[4]=tbl[0xA];
    tbl[0]=tbl[0xC];
    tbl[2]=tbl[challengelen+5];

    mcount = 0;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

int SierraInitV4(unsigned char* prodkey, unsigned int intval, unsigned int& mcount, unsigned char& challengelen)
{
  int result=0;

  if ( intval >= 1 && intval <= 0x20 )
  {
    for (int i=0;i<0x100;i++)
    {
      tbl[i+5] = i&0xFF;
    }
    mcount = 0;
    //LOBYTE(challengelen) = 0;
    challengelen=0;

    for (int i=0xFF;i>=0;i--)
    {
      unsigned char t= static_cast<unsigned char>(SierraCalc1(static_cast<unsigned char>(i), prodkey, intval, challengelen, mcount));
      unsigned char m = tbl[i+5];
      tbl[i+5] = tbl[t+5];
      tbl[t+5] = m;
    }

    tbl[0]=tbl[5];
    tbl[1]=tbl[challengelen+7];
    tbl[2]=tbl[3];
    tbl[3]=tbl[7];
    tbl[4]=tbl[1];

    mcount = 0;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

int SierraInitV5(unsigned char* prodkey, unsigned int intval, unsigned int& mcount, unsigned char& challengelen)
{
  int result=0;

  if ( intval >= 1 && intval <= 0x20 )
  {
    for (int i=0;i<0x100;i++)
    {
      tbl[i+8] = i&0xFF;
    }
    mcount = 0;
    //LOBYTE(challengelen) = 0;
    challengelen=0;

    for (int i=0xFF;i>=0;i--)
    {
      unsigned char t= static_cast<unsigned char>(SierraCalc5(static_cast<unsigned char>(i), prodkey, intval, challengelen, mcount));
      unsigned char m = tbl[i+8];
      tbl[i+8] = tbl[t+8];
      tbl[t+8] = m;
    }

    tbl[0]=tbl[0xF];
    tbl[1]=tbl[0xB];
    tbl[2]=tbl[challengelen+8];
    tbl[3]=tbl[9];
    tbl[4]=tbl[0xD];

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
      tbl[i+5] = i&0xFF;
    }
    mcount = 0;
    //LOBYTE(challengelen) = 0;
    challengelen=0;

    for (int i=0xFF;i>=0;i--)
    {
      unsigned char t= static_cast<unsigned char>(SierraCalc1(static_cast<unsigned int>(i), prodkey, intval, challengelen, mcount));
      unsigned char m = tbl[i+5];
      tbl[i+5] = tbl[t+5];
      tbl[t+5] = m;
    }

    tbl[0]=tbl[6]; //old algo
    tbl[1]=tbl[8];
    tbl[2]=tbl[0xA];
    tbl[3]=tbl[0xC];
    tbl[4]=tbl[challengelen+5];

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
    tbl[i + 5] = 0;

  result = tbl;
  tbl[4] = 0;
  tbl[3] = 0;
  tbl[2] = 0;
  tbl[1] = 0;
  tbl[0] = 0;
  return result;
}

unsigned char* SierraKeygen(unsigned char* challenge, /*unsigned char* resultbuffer,*/ unsigned char* prodtable, unsigned char challengelen, unsigned int mcount, int mode)
{
  int result=0;
  if (mode==2)  result = SierraInitV2(prodtable, mcount, mcount, challengelen);
  else if (mode==3) result = SierraInitV3(prodtable, mcount, mcount, challengelen);
  else if (mode==4) result = SierraInitV4(prodtable, mcount, mcount, challengelen);
  else if (mode==5) result = SierraInitV5(prodtable, mcount, mcount, challengelen);

  unsigned char* resultbuffer=static_cast<unsigned char*>(malloc(challengelen));
  if ( result )
  {
    for ( int i = 0; i < challengelen; i++ )
    {
      if (mode==2) resultbuffer[i] = static_cast<unsigned char>(SierraCalcV2(challenge[i]));
      else if (mode==3) resultbuffer[i] = static_cast<unsigned char>(SierraCalcV3(challenge[i]));
      else if (mode==4) resultbuffer[i] = static_cast<unsigned char>(SierraCalcV4(challenge[i]));
      else if (mode==5) resultbuffer[i] = static_cast<unsigned char>(SierraCalcV5(challenge[i]));
    }
    SierraFinish();
    result = 1;
  }
  return resultbuffer;
}

void SierraCalc::on_calcbutton_clicked()
{
    int mode=0;
    int key=0;
    QString challenge=challengeedit->text();
    mode=2;
    if (mode2->isChecked()) mode=2;
    else if (mode3->isChecked()) mode=3;
    else if (mode4->isChecked()) mode=4;
    else if (mode5->isChecked()) mode=5;
    key=comboBox->currentData().toInt();

    unsigned char challengearray[8]={0};

    for (int i=0;i<8;i++) challengearray[i]=static_cast<unsigned char>(challenge.mid(i*2,2).toInt(nullptr,16));

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

    textEdit->setText(answer);
}
