#include <DAVE.h>

int main(void)
{

DAVE_Init();
PWM_Init (&PWM_0);

uint8_t Recv_Data[6];
uint8_t oldValue=0;
uint8_t newValue=0;
uint8_t index=0;
uint8_t primit=0;
uint8_t p=0;
uint32_t i;

   UART_SetRXFIFOTriggerLimit(&UART_0, 4); // buffer 3 caractere
   PWM_SetDutyCycle(&PWM_0,0);
     while(1U)
        {

    	 index=0;
    	 while(!((UART_GetRXFIFOStatus(&UART_0) & (XMC_USIC_CH_RXFIFO_EVENT_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_ALTERNATE))))
    	 {
    		 Recv_Data[index] = UART_GetReceivedWord(&UART_0);
    		 index++;
    		 if( index==4)
    		 {
    			 UART_ClearRXFIFOStatus(&UART_0, 1);
    			 primit=1;
    			 break;
    		 }
    	 } //we have new data
    	 if(primit==1) //daca am primit data schimbam valoarea la PWM
    	 {

    		 newValue=Recv_Data[0]*100+Recv_Data[1]*10+Recv_Data[2]; //compunem numarul
    		 if(oldValue>newValue)  //Noua valoare PWM mai mica decat cea setata
    		 {
    			 p=oldValue;
    			 while (p!=newValue)  //schimbarea DutyCycle o facem sa fie gradual
    			 {
    				 PWM_SetDutyCycle(&PWM_0,p*10); //Setam noua valoare la DutyCycle
    				 for(i = 0;i<0x1FFFF;i++); //wait aprox 0.5s
    				 p=p-10;
    			 }
    			 oldValue=newValue;//memoram noua valoare setata
    		 }
    		 else if(oldValue<newValue)//Noua valoare PWM mai mare decat cea setata
    		 {
    			 p=oldValue;
    			 while (p!=newValue)//schimbarea DutyCycle o facem sa fie gradual
    			 {
    			    PWM_SetDutyCycle(&PWM_0,p*10);//Setam noua valoare la DutyCycle
    			    for(i = 0;i<0x1FFFF;i++);     //wait aprox 0.5s
    			    p=p+10;
    			  }
    			  oldValue=newValue; //memoram noua valoare setata
    		 }
    		 primit=0;
    	 }
    	} //sf While(1U)




return 1;
}
