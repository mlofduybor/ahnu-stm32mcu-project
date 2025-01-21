#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "main.h"
int stall=5;//清屏信号





int door_opentime = 0;
int door = 0;
int people_num = 0;
int cir = 0;
int x;
int y;
int i;
int tmp;
int floor_tag = 0;//楼层计数
int floor_cur = 1;
int floor_a[10]={0x7e,0x48,0x3d,0x6d,0x4b,0x67,0x77,0x4c,0x7f,0x6f};//数码管显示楼层
int people_a[10]={0x7e,0x48,0x3d,0x6d,0x4b,0x67,0x77,0x4c,0x7f,0x6f};//显示人数
int floor_reg[10];
int h=0,j=0,k=0,t=0;
int A[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//每一列
int B[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};//每一列

/*int Arrowhead[8]={0xE7,0xCF,0x9F,0x00,0x00,0x9F,0xCF,0xE7};*/
int Arrowhead[18]={0xE7,0xC3,0x81,0x24,0xE7,0xE7,0xE7,0xE7,0xff,0xE7,0xC3,0x81,0x24,0xE7,0xE7,0xE7,0xE7,0xff};//箭头向上

int ch[100]={0xFF,0xC1,0xD5,0x00,0x55,0x41,0x7F,0x1F,
                        0xE1,0x00,0xF9,0x45,0x94,0x01,0x54,0x11,
                        0xFB,0xED,0xDB,0xBF,0xBF,0xDB,0xED,0xFB,
                        0xFF,0x7E,0x7E,0x00,0x00,0x7E,0x7E,0xFF,
                        0xE3,0xDD,0xBD,0x7B,0x7B,0xBD,0xDD,0xE3,
    0xFF,0x03,0xED,0xED,0xED,0x03,0xFF,0xFF,
    0xFF,0x01,0xEF,0xEF,0xEF,0x01,0xFF,0xFF,
    0xFF,0x01,0xF7,0xEF,0xDF,0x01,0xFF,0xFF,
    0xFF,0x81,0x7F,0x7F,0x7F,0x81,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};


void elevatorup()
{
    for(j=0;j<100;j++)
  {
            k=j%8;
            for(t=0;t<10;t++)
            {
                    for(h=0;h<8;h++)
                    {
                            GPIOF->ODR=A[h];
                            GPIOE->ODR=Arrowhead[h+k];
                            delay_ms(1);
                            GPIOF->ODR=0x00;
                            GPIOE->ODR=0xff;
                    } 
            }
        }    
      
}

void elevatordown()
{
    for(j=0;j<100;j++)
    {
            k=j%8;
            for(t=0;t<10;t++)
            {
                for(h=0;h<8;h++)
                {
                        GPIOF->ODR=B[h];
                        GPIOE->ODR=Arrowhead[h+k];
                        delay_ms(1);
                        GPIOF->ODR=0x00;
                        GPIOE->ODR=0xff;
                } 
            }
    }
}

void Light()
{
    for(j=0;j<72;j++)
    {
        for(t=0;t<10;t++)
        {
            for(h=0;h<8;h++)
            {
                 GPIOF->ODR=A[h];
                 GPIOE->ODR=ch[h+j];
                 RCC_APB2ENR|=(1ul<<3)|(1ul<<6);
								 delay_ms(1);
                 GPIOF->ODR=0x00;
                 GPIOE->ODR=0xff;
            } 
        }
    }
}


void PD_1_2__intr_init(){ 
	RCC_APB2ENR |= (1ul<<5) | (1ul<<0);

	GPIOD->ODR  |= (1ul<<1);
	GPIOD->ODR  |= (1ul<<2);
	
	AFIO_EXTICR1 |= (3ul<<4);
	AFIO_EXTICR1 &= ~(12<<4);
	AFIO_EXTICR1 |= (3ul<<8);
	AFIO_EXTICR1 &= ~(12<<8);
	
	
	EXTI_IMR |= (1ul<<1);
	EXTI_FTSR |= (1ul<<1);
	EXTI_IMR |= (1ul<<2);
	EXTI_FTSR |= (1ul<<2);
	
	NVIC_ISR0 |= (1ul<<7);
	NVIC_ISR0 |= (1ul<<8);
	
}

void EXTI1_IRQHandler(){
		delay_ms(1000);
		people_num = people_num+1;
		if(people_num >= 5){
			GPIOB->ODR &= ~(1ul<<1);
		}
		else{
			GPIOB->ODR |= (1ul<<1);
		}	
		GPIOG->ODR = people_a[people_num];
		EXTI_PR|=(1ul<<1);
		NVIC_PR|=(1ul<<7);
}


void EXTI2_IRQHandler(){
	  delay_ms(100);
		door = 0;
		door_opentime = 0;
		GPIOD->ODR &= ~(1ul<<0);
		EXTI_PR|=(1ul<<2);
		NVIC_PR|=(1ul<<8);
		delay_ms(1000);
}

void PB_0_intr_init(){
	RCC_APB2ENR |= (1ul<<3) ;

	GPIOB->ODR  |= (1ul<<0);
	
	AFIO_EXTICR1 |= (1ul<<0);
	AFIO_EXTICR1 &= ~(14<<0);
	
	
	EXTI_IMR |= (1ul<<0);
	EXTI_FTSR |= (1ul<<0);
	
	NVIC_ISR0 |= (1ul<<6);
}

void EXTI0_IRQHandler(){
		delay_ms(1000);
		people_num = people_num-1;
		if(people_num >= 5){
			GPIOB->ODR &= ~(1ul<<1);
		}
		else{
			GPIOB->ODR |= (1ul<<1);
		}
		GPIOG->ODR = people_a[people_num];
		EXTI_PR|=(1ul<<0);
		NVIC_PR|=(1ul<<6);
}






int main(void)
{	
	
	u8 n=0;
	//delay_init();
  door_opentime = 0;
  door = 0;
  people_num = 0;
  cir = 0;
  x = 0;
  y = 0;
  i = 0;
  tmp = 0;
  floor_tag = 0;//楼层计数
  floor_cur = 1;
	
	
	delay_init();	    	 				//???????
	uart_init(9600);	 					//??????9600
	LCD_Init();
	POINT_COLOR=BLACK; 						//??????
	while(stall)//初始页面
		{
		for(i=0;i<14;i++)Chinese_Show_one(10+i*16,40,i,16,0);
		n=n+i;
		for(i=0;i<8;i++)Chinese_Show_one(20+i*16,60,n+i,16,0);
		n=n+i;
		for(i=0;i<3;i++)Chinese_Show_one(20+i*16,80,n+i,16,0);
		n=n+i;
		LCD_ShowString(40+i*16,80,200,16,16, "22111304042");	
		for(i=0;i<3;i++)Chinese_Show_one(20+i*16,100,n+i,16,0);
		n=n+i;
		LCD_ShowString(40+i*16,100,200,16,16, "22111304046");	
		LCD_ShowString(20,120,200,16,16, "2022");	
		for(i=0;i<10;i++)Chinese_Show_one(10+(i+3)*16,120,n+i,16,0);
		n=n+i;
		for(i=0;i<8;i++)Chinese_Show_one(20+i*16,140,n+i,16,0);
		n=n+i;
		LCD_ShowString(20,160,200,16,16, "2024-05-19");	
		n=0;
		stall--;
		delay_ms(1000);
	}
	
	
	RCC_APB2ENR|= (1ul<<5)|(1ul<<2)|(1ul<<0)|(1ul<<4)|(1ul<<8)|(1ul<<3)|(1ul<<8);
	GPIOC->CRL=0x11111111;
	GPIOA->CRL=0x11111111;
	GPIOD->CRL=0x11111881;
	GPIOB->CRL=0x11111111;
	GPIOG->CRL=0x11111111;
	
	GPIOB->ODR &= ~(1ul<<7);
	
	GPIOB->ODR &= ~(1ul<<2);
	GPIOB->ODR |= (1ul<<1);
	
	PB_0_intr_init();
	PD_1_2__intr_init();
	
	
	RCC_APB2ENR|=(1ul<<7)|(1ul<<6);
  GPIOF->CRL=0x11111111;//高电平
  GPIOE->CRL=0X11111111;//低电平
  GPIOF->ODR|=0x00;
  GPIOE->ODR|=0xff;
	Light();
	
	
	for (i = 1;i<11;i++){
		floor_reg[i] = 0;
	}
	GPIOA->ODR = floor_a[floor_cur];
	GPIOG->ODR = people_a[people_num];
	while(1){
		for (i = 0;i<20;i++){
			floor_tag = 0;
			x = 0;
			y = 0; 
			GPIOC->ODR = 0x0f;
			if (GPIOC->IDR&(1ul<<4))
					x = 4;
			if (GPIOC->IDR&(1ul<<5))
					x = 5;
			if (GPIOC->IDR&(1ul<<6))
					x = 6;
			if (GPIOC->IDR&(1ul<<7))
					x = 7;
			GPIOC->ODR = 0xf0;
			if (GPIOC->IDR&(1ul<<1))
					y = 1;
			if (GPIOC->IDR&(1ul<<2))
					y = 2;
			if (GPIOC->IDR&(1ul<<3))
					y = 3;
			if (x == 4 & y == 3)
					floor_tag = 1;
			if (x == 5 & y == 3)
					floor_tag = 2;
			if (x == 6 & y == 3)
					floor_tag = 3;
			if (x == 7 & y == 3)
					floor_tag = 4;
			if (x == 4 & y == 2)
					floor_tag = 5;
			if (x == 5 & y == 2)
					floor_tag = 6;
			if (x == 6 & y == 2)
					floor_tag = 7;
			if (x == 7 & y == 2)
					floor_tag = 8;  
			if (x == 4 & y == 1)
					floor_tag = 9;
			floor_reg[floor_tag] = 1;
			
			delay_ms(1000);
			printf("%d\n", floor_tag);
		}
		for (i = 10;i>=1;i--){
			if (floor_reg[i] == 1){
				if (floor_cur == i)
					continue;
				printf("door open\n");
				door = 1;//门已开启
				door_opentime = 0;//门开倒计时	
				
				//people_num = 0;
				GPIOD->ODR |= (1ul<<0);
				while(1){
					if (door == 0)
						break;
					//door_opentime = door_opentime+1;
					//delay_ms(1000);
					//if (door_opentime == 6)
						//break;
					
				}
				printf("door close\n");
				GPIOD->ODR &= ~(1ul<<0);
				//if (door == 0){
				//delay_ms(1000);
						if (floor_cur<i){
							cir = 0;
							GPIOD->ODR = 0x00;
							
							GPIOD->ODR  &= ~(1ul<<1);//运行时不能塞人进电梯
							GPIOD->ODR  &= ~(1ul<<2);//运行时不能开门
							
							elevatorup();
							while(1){
								
								for (tmp = 0;tmp<512;tmp++){

										GPIOD->ODR = 0x10;//00010000
										delay_ms(10);
										GPIOD->ODR = 0x20;//00100000
										delay_ms(10);
										GPIOD->ODR = 0x40;//01000000
										delay_ms(10);
										GPIOD->ODR = 0x80;//10000000
										delay_ms(10);

									}
									cir  = cir + 1;
									GPIOA->ODR = floor_a[floor_cur+cir];
									if (cir==i-floor_cur)
											break;
								}		

							GPIOD->ODR  |= (1ul<<1);
							GPIOD->ODR  |= (1ul<<2);
								
							}
						else{
							cir = 0;
							GPIOD->ODR = 0x00;   
							
							GPIOD->ODR  &= ~(1ul<<1);//运行时不能塞人进电梯
							GPIOD->ODR  &= ~(1ul<<2);//运行时不能开门
							
							elevatordown();
							
							while(1){
								
								for (tmp = 0;tmp<512;tmp++){
										GPIOD->ODR = 0x80;//10000000
										delay_ms(10);
										GPIOD->ODR = 0x10;//00010000
										delay_ms(10);
										GPIOD->ODR = 0x20;//00100000
										delay_ms(10);
										GPIOD->ODR = 0x40;//01000000
										delay_ms(10);

									}
									cir  = cir + 1;
									GPIOA->ODR = floor_a[floor_cur-cir];
									if (cir==floor_cur-i)
											break;
										}
										
										
								GPIOD->ODR  |= (1ul<<1);
								GPIOD->ODR  |= (1ul<<2);
										
							}
						floor_reg[i] = 0;
						floor_cur = i;
						GPIOD->ODR |= (1ul<<3);
						delay_ms(6000);
						GPIOD->ODR &= ~(1ul<<3);
							
							
						printf("door open\n");	
						door = 1;//门已开启	
						door_opentime = 0;//门开倒计时	
							
						GPIOD->ODR |= (1ul<<0);
						while(1){
							if (door == 0)
								break;
						door_opentime = door_opentime+1;
						delay_ms(1000);
						if (door_opentime == 6)
							break;
						}	
						printf("door close\n");
						GPIOD->ODR &= ~(1ul<<0);	
							
						floor_tag = 0;
				//}
			}
		}
	}

 }
 
