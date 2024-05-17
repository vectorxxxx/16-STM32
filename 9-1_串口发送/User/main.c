#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

int main(void)
{
    OLED_Init();

    Serial_Init();
    
    Serial_SendByte(0x41);
    
    uint8_t Array[4] = {0x42, 0x43, 0x44, 0x45};
    Serial_SendArray(Array, 4);
    
    Serial_SendString("\r\nNum1=");
    
    Serial_SendNumber(123456, 6);
    
    
    /*下述3种方法可实现printf的效果*/
    
    /*方法1：直接重定向printf，但printf函数只有一个，此方法不能在多处使用*/
    //需要重定向fputc函数，并在工程选项里勾选Use MicroLIB
    printf("\r\nNum2=%d", 111);
    
    /*方法2：使用sprintf打印到字符数组，再用串口发送字符数组，此方法打印到字符数组，之后想怎么处理都可以，可在多处使用*/
    char String[100];
    sprintf(String, "\r\nNum3=%d", 222);
    Serial_SendString(String);

	/*方法3：将sprintf函数封装起来，实现专用的printf，此方法就是把方法2封装起来，更加简洁实用，可在多处使用*/
    Serial_Printf("\r\nNum4=%d", 333);
    
    
    // Options for Target > C/C++ > Misc Controls: --no-multibyte-chars
    Serial_SendString("\r\n你好，世界");
    
    while(1)
    {

    }
}
