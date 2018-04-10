#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define COLOR_COUNT 7

struct VIDEO
{
	unsigned char symbol;
	unsigned char attribute;
};

unsigned char colors[COLOR_COUNT] = { 0x71,0x62,0x43,0x54,0x35,0x26,0x17 };
char color = 0x89;														  

void changeColor()                                      
{
	color = colors[rand() % COLOR_COUNT];               
	return;
}

void print()
{
	char temp;                                          
	int i, val;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);  

	val = inp(0x21);                                    
	for (i = 0; i < 8; i++)                             
	{
		temp = val % 2;                                 
		val /= 2;                                 
		screen->symbol = temp + '0';                    
		screen->attribute = color;                      
		screen++;                                       
	}
	screen++;                                           

	val = inp(0xA1);                                    
	for (i = 0; i < 8; i++)								
	{
		temp = val % 2;									 
		val /= 2;
		screen->symbol = temp + '0';					 
		screen->attribute = color;						 
		screen++;										 
	}
	screen += 63;									     
	outp(0x20, 0x0A);                                    

	val = inp(0x20);                                     
	for (i = 0; i < 8; i++)								 
	{
		temp = val % 2; 
		val /= 2;
		screen->symbol = temp + '0';					 
		screen->attribute = color;						 
		screen++;										 
	}
	screen++;                                            

	outp(0xA0, 0x0A);                                    
	val = inp(0xA0);                                     
	for (i = 0; i < 8; i++)								 
	{
		temp = val % 2;									 
		val /= 2;
		screen->symbol = temp + '0';					 
		screen->attribute = color;						 
		screen++;										 
	}
	screen += 63;                                        

	outp(0x20, 0x0B);                                    
	val = inp(0x20);                                     
	for (i = 0; i < 8; i++)                              
	{
		temp = val % 2;									 
		val /= 2;
		screen->symbol = temp + '0';					  
		screen->attribute = color;						  
		screen++;										 
	}
	screen++;                                            

	outp(0xA0, 0x0B);                                    
	val = inp(0xA0);                                     
	for (i = 0; i < 8; i++)                              
	{
		temp = val % 2;								     
		val /= 2;
		screen->symbol = temp + '0';					  
		screen->attribute = color;						 
		screen++;										 
	}
}


void interrupt(*oldint8) (...); 
void interrupt(*oldint9) (...); 
void interrupt(*oldint10) (...);
void interrupt(*oldint11) (...);
void interrupt(*oldint12) (...);
void interrupt(*oldint13) (...);
void interrupt(*oldint14) (...);
void interrupt(*oldint15) (...);

								

void interrupt(*oldint70) (...);
void interrupt(*oldint71) (...);
void interrupt(*oldint72) (...);
void interrupt(*oldint73) (...);
void interrupt(*oldint74) (...);
void interrupt(*oldint75) (...);
void interrupt(*oldint76) (...);
void interrupt(*oldint77) (...);
 
								

void interrupt  newint08(...) { print(); oldint8(); }                     
void interrupt  newint09(...) { changeColor(); print(); oldint9(); }	  
void interrupt  newint0A(...) { changeColor(); print(); oldint10(); }	  
void interrupt  newint0B(...) { changeColor(); print(); oldint11(); }	  
void interrupt  newint0C(...) { changeColor(); print(); oldint12(); }	  
void interrupt  newint0D(...) { changeColor(); print(); oldint13(); }	  
void interrupt  newint0E(...) { changeColor(); print(); oldint14(); }	  
void interrupt  newint0F(...) { changeColor(); print(); oldint15(); }	  

void interrupt  newint70(...) { changeColor(); print(); oldint70(); }	  
void interrupt  newint71(...) { changeColor(); print(); oldint71(); }	  
void interrupt  newint72(...) { changeColor(); print(); oldint72(); }	  
void interrupt  newint73(...) { changeColor(); print(); oldint73(); }	  
void interrupt  newint74(...) { changeColor(); print(); oldint74(); }	  
void interrupt  newint75(...) { changeColor(); print(); oldint75(); }	  
void interrupt  newint76(...) { changeColor(); print(); oldint76(); }	  
void interrupt  newint77(...) { changeColor(); print(); oldint77(); }	  

void initialize()
{
	
	oldint8 = getvect(0x08);  
	oldint9 = getvect(0x09);  
	oldint10 = getvect(0x0A); 
	oldint11 = getvect(0x0B); 
	oldint12 = getvect(0x0C); 
	oldint13 = getvect(0x0D); 
	oldint14 = getvect(0x0E); 
	oldint15 = getvect(0x0F); 

							  
	oldint70 = getvect(0x70); 
	oldint71 = getvect(0x71); 
	oldint72 = getvect(0x72); 
	oldint73 = getvect(0x73); 
	oldint74 = getvect(0x74); 
	oldint75 = getvect(0x75); 
	oldint76 = getvect(0x76); 
	oldint77 = getvect(0x77); 

							  
	setvect(0x08, newint08);  
	setvect(0x09, newint09);  
	setvect(0x0A, newint0A);  
	setvect(0x0B, newint0B);  
	setvect(0x0C, newint0C);  
	setvect(0x0D, newint0D);  
	setvect(0x0E, newint0E);  
	setvect(0x0F, newint0F);  

							  
	setvect(0x78, newint70);  
	setvect(0x79, newint71);  
	setvect(0x7A, newint72);  
	setvect(0x7B, newint73);  
	setvect(0x7C, newint74);  
	setvect(0x7D, newint75);  
	setvect(0x7E, newint76);  
	setvect(0x7F, newint77);  

	_disable(); 
				
	outp(0x20, 0x11);	 
	outp(0x21, 0x08);   
	outp(0x21, 0x04);	
	outp(0x21, 0x01);	

						
	outp(0xA0, 0x11);   
	outp(0xA1, 0x78);	
	outp(0xA1, 0x02);   
	outp(0xA1, 0x01);   

	_enable(); 
}

int main()
{
	unsigned far *fp;                           
	initialize();                               
	system("cls");                              
	printf("                  - mask\n");       
	printf("                  - prepare\n");    
	printf("                  - service\n");    
	printf("Master   Slave\n");
	FP_SEG(fp) = _psp;                          
	FP_OFF(fp) = 0x2c;                          
	_dos_freemem(*fp);                          

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1); 
												
	return 0;
}
