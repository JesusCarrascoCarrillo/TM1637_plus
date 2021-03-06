//  Author:Frankie.Chu
//  Date:9 April,2012
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Modified record:
//  V2.0 (debido a que la libreria no se actuliza desde el 2012, he decidido crear
//    la libreria TM1637_plus partiendo de la versión 1.1 de la TM1637 de Frankie.Chu
//  Author: Jesus Carrasco Carrillo
//  Date: 9 April, 2018
//  Se añade la posibilidad de activar cualquiera de los puntos del display
//  Se añade la posibilidad de escribir simbolos, definidos por activación de segmentos
//     a
//    ---
// f |   | b
//    ---
// e | g | c
//    ---
//     d
//   se codifican en binario como 0gfedcba, por ejemplo el 0 seria 0b00111111 = 0x3f
/*******************************************************************************/
#include "TM1637_plus.h"
#include <Arduino.h>
static int8_t TubeTab[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f};//0~9                        
TM1637::TM1637(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}

void TM1637::init(void)
{
  clearDisplay();
}

void TM1637::writeByte(int8_t wr_data)
{
  uint8_t i,count1;   
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);      
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
    wr_data >>= 1;      
    digitalWrite(Clkpin,HIGH);
      
  }  
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);     
  pinMode(Datapin,INPUT);
  while(digitalRead(Datapin))    
  { 
    count1 +=1;
    if(count1 == 200)//
    {
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
     count1 =0;
    }
    pinMode(Datapin,INPUT);
  }
  pinMode(Datapin,OUTPUT);
  
}
//send start signal to TM1637
void TM1637::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1637
  digitalWrite(Datapin,HIGH); 
  digitalWrite(Datapin,LOW); 
  digitalWrite(Clkpin,LOW); 
} 
//End of transmission
void TM1637::stop(void)
{
  digitalWrite(Clkpin,LOW);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,HIGH);
  digitalWrite(Datapin,HIGH); 
}
//display function.Write to full-screen.
void TM1637::display(int8_t DispData[])
{
  int8_t SegData[4];
  uint8_t i;
  for(i = 0;i < 4;i ++)
  {
    SegData[i] = DispData[i];
  }
  coding(SegData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO);//
  stop();           //
  start();          //
  writeByte(Cmd_SetAddr);//
  for(i=0;i < 4;i ++)
  {
    writeByte(SegData[i]);        //
  }
  stop();           //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}
//******************************************
void TM1637::display(uint8_t BitAddr,int8_t DispData)
{
  int8_t SegData;


  SegData = coding(BitAddr,DispData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);//

  stop();           //
  start();          //
  writeByte(BitAddr|0xc0);//
  
  writeByte(SegData);//
  stop();            //
  start();          //
  
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

void TM1637::display_sym(uint8_t BitAddr,int8_t SegData)
{
  
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);//

  stop();           //
  start();          //
  writeByte(BitAddr|0xc0);//
  
  writeByte(SegData);//
  stop();            //
  start();          //
  
  writeByte(Cmd_DispCtrl);//
  stop();           //
}
void TM1637::clearDisplay(void)
{
  display(0x00,0x7f);
  display(0x01,0x7f);
  display(0x02,0x7f);
  display(0x03,0x7f);  
}
//To take effect the next time it displays.
void TM1637::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}

//Whether to light the clock point ":".
//To take effect the next time it displays.
void TM1637::point(boolean PointFlag)
{
  for(uint8_t i = 0;i < 4;i ++)
  {
	_PointFlag[i] = PointFlag;
  }
}
void TM1637::point(uint8_t BitAddr,boolean PointFlag)
{
  _PointFlag[BitAddr] = PointFlag;
}
void TM1637::coding(int8_t DispData[])
{
  uint8_t PointData;
  for(uint8_t i = 0;i < 4;i ++)
  {
	if(_PointFlag[i] == POINT_ON)PointData = 0x80;
	else PointData = 0; 
    if(DispData[i] == 0x7f)DispData[i] = 0x00;
    else DispData[i] = TubeTab[DispData[i]] + PointData;
  }
}
int8_t TM1637::coding(uint8_t BitAddr,int8_t DispData)
{
  uint8_t PointData;
  if(_PointFlag[BitAddr] == POINT_ON)PointData = 0x80;
  else PointData = 0; 
  if(DispData == 0x7f) DispData = 0x00 + PointData;//The bit digital tube off
  else DispData = TubeTab[DispData] + PointData;
  return DispData;
}