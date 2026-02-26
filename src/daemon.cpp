/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   daemon.cpp
 * inspired by works of matega
 * Author: Mark Herbert
 *
 * Created on 26 February 2026, 10:37
 */

#include <cstdlib>
#include <sys/io.h>
#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <signal.h>


using namespace std;

static volatile int quit;
    int temp_cpu,batt_curr, fan_rpm;
    

void ReadSysfs()
{


    string therm;
    ifstream inFile1("/sys/class/hwmon/hwmon5/temp1_input");
    if (inFile1.fail()){
      cout <<"Data could not be read, please install extended HuaweiWMI module"<< endl;
      exit(1);
    }  
   inFile1 >> therm;
    inFile1.close();
    temp_cpu = (int)stof(therm);

    string curr;
    ifstream inFile2("/sys/class/hwmon/hwmon2/curr1_input");
    if (inFile2.fail()){
      cout <<"Data could not be read, please install extended HuaweiWMI module"<< endl;
      exit(1);
    }  
   inFile2 >> curr;
    inFile2.close();
    batt_curr = (int)stof(curr);

    string rpm;
    ifstream inFile3("/sys/class/hwmon/hwmon5/fan1_input");
    if (inFile3.fail()){
      cout <<"Data could not be read, please install extended HuaweiWMI module"<< endl;
      exit(1);
    }  
   inFile3 >> rpm;
    inFile3.close();
    fan_rpm = (int)stof(rpm);   

}


static void sig_handler(int i) { 
	ofstream outFile("/proc/acpi/call");
    if (outFile.fail()){
      cout <<"Acpi call not accessible, please install acpi_call module"<< endl;
      exit(1);
    }  
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020C20 0x00"<<endl;
   outFile.close();
   quit = i;
 }
 
static void FanOff() {
	ofstream outFile("/proc/acpi/call");
    if (outFile.fail()){
      cout <<"Acpi call not accessible, please install acpi_call module"<< endl;
      exit(1);
    }  
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020C20 0x0a"<<endl;
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020A07 0" <<endl;
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020A08 0" <<endl;
   
   outFile.close();
}

static void FanLow() {
	ofstream outFile("/proc/acpi/call");
    if (outFile.fail()){
      cout <<"Acpi call not accessible, please install acpi_call module"<< endl;
      exit(1);
    }  
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020C20 0x0a"<<endl;
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020A07 24" <<endl;
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020A08 24" <<endl;
   
   outFile.close();
}

static void FanAuto() {
	ofstream outFile("/proc/acpi/call");
    if (outFile.fail()){
      cout <<"Acpi call not accessible, please install acpi_call module"<< endl;
      exit(1);
    }  
   outFile <<"\\_SB.PC00.LPCB.H_EC.WTER 0x20020C20 0x00"<<endl;
   outFile.close();
}

int main(int argc, char** argv) {

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
   
    while(!quit) {
		ReadSysfs();
		if ((fan_rpm > 0) && (batt_curr < 2500) && (temp_cpu < 58000)) FanOff();  
		if (((fan_rpm == 0) || (fan_rpm > 2800)) && (batt_curr < 2500) && (temp_cpu >= 58000) && (temp_cpu < 62000)) FanLow(); 		
		if (((fan_rpm < 3000) && (temp_cpu >= 62000))||(batt_curr > 2500)) FanAuto();		


    
     sleep(3); 
}

    return 0;
}

