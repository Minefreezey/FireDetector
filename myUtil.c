#include <xc.h>
#include "myUtil.h"

void myitoa(int val,char* str){
    int negative = 0;
    if (val < 0){
        //LCDDisplayString("Ne");
        negative = 1;
        val = -val;
    }    
    else if (val == 0){
        //LCDDisplayString("ZE");
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
   
    int i =0;
    char tempstr[16];
    
    while (val != 0){
        tempstr[i] = (val%10) + 48;
        //sendLCDData(tempstr[i]);
        val = val/10;
        i++;
    }
    tempstr[i] = '\0';
    i--;
    int j = 0;
    while (i >= 0){
        str[j] = tempstr[i];
        i--;
        j++;
    }
    
    str[j] = '\0';
}
