#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define base  1193180//у программируемого таймера есть несколько каналов (3) на входы, которых кварцевый генератор подаЄт тактовые импульсы с частотой 1193180√ц!!!
//— выхода 0-го канала сигнал поступает на линию IRQ0 контролера прерываний. 
//0-й канал таймера запрограммирован на генерацию меандра с коэффициентом делени€ 65536 (1193180 / 65536 ~ 18,20...)

void generateSound(int frequency, int d)
{
	int kd;
	outp(0x43, 0xB6);//10110110 канал 2, операци€ 4, режим 3 , формат 0
					 //обращение к порту регистра, который управл€ет таймером. ƒл€ этого необходио
					 //послать в командный регистр 43h байт, представл€ющий цепочку битов, которые
					 //выбирают канал (канал є2 таймера), статус чтени€/записи, режим операции и
					 //форму представлени€ чисел
	kd = base / frequency;
	/*ƒл€ генерации звуковой частоты обычно используетс€ режим 3 Ц режим делени€ частоты.
	¬ программе вычисл€етс€ требуемое значение счетчика дл€ регистра -
	защЄлки таймера(делитель частоты), дл€ этого входную частоту - 1193000 √ц дел€т на требуемую частоту в герцах.*/
	outp(0x42, kd % 256); //младша€ часть
						  /*“аймеру соответствуют четыре порта ввода / вывода со следующими адресами :
						  40h - канал 0;
						  41h - канал 1;
						  42h - канал 2;
						  43h - управл€ющий регистр.*/
	kd /= 256;
	outp(0x42, kd);//старша€ часть

	outp(0x61, inp(0x61) | 3);//61ый - порт динамика. 3 Ц включение динамика
	delay(d);
	outp(0x61, inp(0x61) & 0xFC);//0xfc - выключение динамика.
}

void print(int state)
{
	int i;
	char temp[8];
	for (i = 7; i >= 0; i--)
	{
		temp[i] = state % 2 + '0';
		state /= 2;
	}
	for (i = 0; i < 8; i++)
	{
		printf("%c", temp[i]);
	}
	printf("\n");
}
void strcan()
{
	int state;

	outp(0x43, 0xE2); //1110 0010
	state = inp(0x40);
	print(state);
	outp(0x43, 0xE4); //1110 0100
	state = inp(0x41);
	print(state);

	outp(0x43, 0xE8); //1110 1000
	state = inp(0x42);
	print(state);
}

void getRandomValue(long val)
{
	long kdLow, kdHigh, kdNew;
	outp(0x43, 0xB4); // 1011 0100
	outp(0x42, val % 256);
	val /= 256;
	outp(0x42, val);
	outp(0x61, inp(0x61) | 0x01);
	int i = 0;
	while (i < 10)
	{
		i++;
		delay(i);
		outp(0x43, 0x86); // 1000 0110
		kdLow = inp(0x42);
		kdHigh = inp(0x42);
		kdNew = kdHigh * 256 + kdLow;
		printf("%ld\n", kdNew);
	}
	outp(0x61, inp(0x61) & 0xFC);
}


void kdcan()
{
	long kd, kdHigh, kdLow, kdMax;

	kdMax = 0;

	for (long i = 0; i < 65535; i++)
	{
		outp(0x43, 0x00); //0000 0000 первые два бита определ€ют с каким каналом работаем
		kdLow = inp(0x40); // получение слова состо€ни€ канала
		kdHigh = inp(0x40);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);
	kdMax = 0;
	for (i = 0; i < 65535; i++)
	{
		outp(0x43, 0x40); // 0100 0000
		kdLow = inp(0x41);
		kdHigh = inp(0x41);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);
	kdMax = 0;
	for (i = 0; i < 65535; i++)
	{
		outp(0x43, 0x80); //1000 0000 
		kdLow = inp(0x42);
		kdHigh = inp(0x42);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);

}

int main()
{
	int kdLow, kdHigh, kd;

	generateSound(349, 600);
	generateSound(392, 300);
	generateSound(440, 600);
	generateSound(349, 300);
	generateSound(440, 300);
	generateSound(440, 300);
	generateSound(392, 300);
	generateSound(349, 300);
	generateSound(392, 300);

	strcan();

	printf("\n");

	kdcan();

	int maximum;
	scanf("%d", &maximum);
	getRandomValue(maximum);

	return 0;
}

