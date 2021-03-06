

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>

#define F_CPU 16000000UL
#define baud 9600
#define baud_select (((F_CPU)/(16UL*baud))-1)

//new line
void USART_Init();
void USART_Transmit(unsigned char data);
unsigned char USART_Receive();
unsigned char Receive_Bit=0;
void pattern(int i);
char arr[10];

int layer[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };
int selection[] = { 0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C };
int font[95][8] = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 0 SPACE
		{ 0x7e, 0x3c, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18 }, // 1 !
		{ 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 2 "
		{ 0x66, 0xff, 0xff, 0x66, 0x66, 0xff, 0xff, 0x66 }, // 3 #
		{ 0x18, 0xff, 0x19, 0x19, 0xfe, 0x98, 0xff, 0x18 }, // 4 $
		{ 0x87, 0x45, 0x27, 0x10, 0x08, 0xe4, 0xa2, 0xe1 }, // 5 %
		{ 0x1e, 0x33, 0x33, 0xde, 0xee, 0x3b, 0x73, 0xdc }, // 6 &
		{ 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 7 '
		{ 0x18, 0x0c, 0x06, 0x03, 0x03, 0x06, 0x0c, 0x18 }, // 8 (
		{ 0x18, 0x38, 0x60, 0xc0, 0xc0, 0x60, 0x38, 0x18 }, // 9 )
		{ 0x99, 0x5a, 0x3c, 0xff, 0xff, 0x3c, 0x5a, 0x99 }, // 10 *
		{ 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18 }, // 11 +
		{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x0c }, // 12 ,
		{ 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00 }, // 13 -
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 }, // 14 .
		{ 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x07, 0x03 }, // 15 /

		{ 0x7e, 0xff, 0xe3, 0xd3, 0xcb, 0xc7, 0xff, 0x7e }, // 16 0
		{ 0x38, 0x3c, 0x36, 0x33, 0x31, 0x30, 0x30, 0x30 }, // 17 1
		{ 0x7c, 0x7e, 0x62, 0x30, 0x18, 0x0c, 0x7e, 0x7e }, // 18 2
		{ 0x7e, 0xff, 0xc0, 0x78, 0x78, 0xc0, 0xff, 0x7e }, // 19 3
		{ 0xc3, 0xc3, 0xc3, 0xff, 0xff, 0xc0, 0xc0, 0xc0 }, // 20 4
		{ 0xff, 0xff, 0x03, 0x3f, 0x7e, 0xc0, 0x7f, 0x3f }, // 21 5
		{ 0xfe, 0xff, 0x03, 0x7f, 0xff, 0xc3, 0xff, 0x7e }, // 22 6
		{ 0x7e, 0xff, 0xe0, 0x70, 0xfe, 0x1c, 0x0e, 0x07 }, // 23 7
		{ 0x3c, 0x7e, 0xc3, 0x7e, 0x7e, 0xc3, 0x7e, 0x3c }, // 24 8
		{ 0x7e, 0xff, 0xc3, 0xff, 0xbe, 0x80, 0xff, 0x7e }, // 25 9

		{ 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00 }, // 26 :
		{ 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c }, // 27 ;
		{ 0xe0, 0x38, 0x0e, 0x03, 0x03, 0x0e, 0x38, 0xe0 }, // 28 <
		{ 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00 }, // 29 =
		{ 0x07, 0x1c, 0x70, 0xc0, 0xc0, 0x70, 0x1c, 0x07 }, // 30 >
		{ 0x3c, 0x66, 0x60, 0x30, 0x18, 0x00, 0x18, 0x18 }, // 31 ?
		{ 0x7e, 0x80 , 0xbe, 0xa9, 0xb9,0x81,0x82, 0x7d}, // 32 @

		{ 0x7e, 0xff, 0xc3, 0xc3, 0xff, 0xff, 0xc3, 0xc3 }, // 33 A
		{ 0x7f, 0x83, 0x83, 0x83, 0x7f, 0x83, 0x83, 0x7f }, // 34 B
		{ 0x7e, 0xff, 0x03, 0x03, 0x03, 0x03, 0xff, 0x7e }, // 35 C
		{ 0x1e, 0x3f, 0x63, 0xc3, 0xc3, 0x63, 0x3f, 0x1e }, // 36 D
		{ 0xfe, 0xff, 0x03, 0x3f, 0x3f, 0x03, 0xff, 0xfe }, // 37 E
		{ 0xfe, 0xff, 0x03, 0x3f, 0x3f, 0x03, 0x03, 0x03 }, // 38 F
		{ 0xfe, 0xff, 0x03, 0x63, 0xf3, 0xc3, 0xff, 0x7e }, // 39 G
		{ 0xc3, 0xc3, 0xc3, 0xff, 0xff, 0xc3, 0xc3, 0xc3 }, // 40 H
		{ 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c }, // 41 I
		{ 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc6, 0x66, 0x3c }, // 42 J
		{ 0xc3, 0x63, 0x33, 0x1f, 0x1f, 0x33, 0x63, 0xc3 }, // 43 K
		{ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff }, // 44 L
		{ 0x66, 0xff, 0xdb, 0xdb, 0xdb, 0xdb, 0xc3, 0xc3 }, // 45 M
		{ 0xc7, 0xcf, 0xcf, 0xdb, 0xdb, 0xf3, 0xf3, 0xe3 }, // 46 N
		{ 0x7e, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0x7e }, // 47 O
		{ 0x3e, 0x7f, 0xc3, 0xc3, 0x7f, 0x3f, 0x03, 0x03 }, // 48 P
		{ 0x3c, 0x7e, 0xc3, 0xc3, 0xd3, 0xe3, 0x7e, 0xbc }, // 49 Q
		{ 0x3e, 0x7f, 0xc3, 0x7f, 0x3f, 0x1f, 0x3b, 0x73 }, // 50 R
		{ 0xfe, 0xff, 0x03, 0x7f, 0xfe, 0x80, 0xff, 0x7f }, // 51 S
		{ 0xff, 0xff, 0x99, 0x18, 0x18, 0x18, 0x18, 0x3c }, // 52 T
		{ 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0x7e }, // 53 U
		{ 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18 }, // 54 V
		{ 0xc3, 0xc3, 0xc3, 0xdb, 0xdb, 0xdb, 0xff, 0x7e }, // 55 W
		{ 0xc3, 0xe7, 0x7e, 0x3c, 0x3c, 0x7e, 0xe7, 0xc3 }, // 56 X
		{ 0x81, 0xc3, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x18 }, // 57 Y
		{ 0xfe, 0xff, 0x70, 0x38, 0x1c, 0x0e, 0xff, 0x7f }, // 58 Z

		{ 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff }, // 59 [
		{ 0x03, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0, 0xc0 }, // 60 Backslash
		{ 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff }, // 61 ]
		{ 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00 }, // 62 ^
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff }, // 63 _
		{ 0x0e, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 64 `

		{ 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00 }, // 65 a
		{ 0x07, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3D, 0x00 }, // 66 b
		{ 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00 }, // 67 c
		{ 0x38, 0x30, 0x30, 0x3E, 0x33, 0x33, 0x6E, 0x00 }, // 68 d
		{ 0x00, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00 }, // 69 e
		{ 0x1C, 0x36, 0x06, 0x0F, 0x06, 0x06, 0x0F, 0x00 }, // 70 f
		{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F }, // 71 g
		{ 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00 }, // 72 h
		{ 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 }, // 73 i
		{ 0x18, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x1B, 0x0E }, // 74 j
		{ 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00 }, // 75 k
		{ 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 }, // 76 l
		{ 0x00, 0x00, 0x37, 0x7F, 0x6B, 0x63, 0x63, 0x00 }, // 77 m
		{ 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00 }, // 78 n
		{ 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00 }, // 79 o
		{ 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F }, // 80 p
		{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78 }, // 81 q
		{ 0x00, 0x00, 0x1B, 0x36, 0x36, 0x06, 0x0F, 0x00 }, // 82 r
		{ 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00 }, // 83 s
		{ 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00 }, // 84 t
		{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00 }, // 85 u
		{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00 }, // 86 v
		{ 0x00, 0x00, 0x63, 0x63, 0x6B, 0x7F, 0x36, 0x00 }, // 87 w
		{ 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00 }, // 88 x
		{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F }, // 89 y
		{ 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00 }, // 90 z

		{ 0xf8, 0xfc, 0x0c, 0x07, 0x07, 0x0c, 0xfc, 0xf8 }, // 91 {
		{ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, // 92 |
		{ 0x1f, 0x3f, 0x30, 0xe0, 0xe0, 0x30, 0x3f, 0x1f }, // 93 }
		{ 0x00, 0x04, 0x0e, 0x0b, 0x99, 0xd0, 0x70, 0x20 } // 94 ~
};

void ioinit() {
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRD = 0xFF;
	DDRC = 0xFF;

	PORTA = 0x00;
	PORTD = 0x00;
	PORTC = 0xFF;
	PORTB = 0x00;

	//_delay_ms(2000);

}
int main(void) {
	ioinit();

	USART_Init();
	//USART_Transmit('');

	while (1) {
		int i=0;
		while(1){

			char a=USART_Receive();
			if(a!='\n'){
				arr[i++]=a;
			}else{
				break;
			}
		}

		//Receive_Bit = USART_Receive();
		int j;
		for(j=0;j<i;j++){
			switch (arr[j]) {
					case 'a':
						disp(65);
						break;

					case 'b':
						disp(66);
						break;

					case 'c':
						disp(67);
						break;
					case 'd':
						disp(68);
						break;

					case 'e':
						disp(69);
						break;

					case 'f':
						disp(70);
						break;
					case 'g':
						disp(71);
						break;

					case 'h':
						disp(72);
						break;

					case 'i':
						disp(73);
						break;

					case 'j':
						disp(74);
						break;

					case 'k':
						disp(75);
						break;

					case 'l':
						disp(76);
						break;
					case 'm':
						disp(77);
						break;

					case 'n':
						disp(78);
						break;

					case 'o':
						disp(79);
						break;
					case 'p':
						disp(80);
						break;

					case 'q':
						disp(81);
						break;

					case 'r':
						disp(82);
						break;
					case 's':
						disp(83);
						break;

					case 't':
						disp(84);
						break;

					case 'u':
						disp(85);
						break;
					case 'v':
						disp(86);
						break;

					case 'w':
						disp(87);
						break;

					case 'x':
						disp(88);
						break;
					case 'y':
						disp(89);
						break;

					case 'z':
						disp(90);
						break;
					case 'A':
						disp(33);
						break;

					case 'B':
						disp(34);
						break;

					case 'C':
						disp(35);
						break;
					case 'D':
						disp(36);
						break;

					case 'E':
						disp(37);
						break;

					case 'F':
						disp(38);
						break;
					case 'G':
						disp(39);
						break;

					case 'H':
						disp(40);
						break;

					case 'I':
						disp(41);
						break;

					case 'J':
						disp(42);
						break;

					case 'K':
						disp(43);
						break;

					case 'L':
						disp(44);
						break;
					case 'M':
						disp(45);
						break;

					case 'N':
						disp(46);
						break;

					case 'O':
						disp(47);
						break;
					case 'P':
						disp(48);
						break;

					case 'Q':
						disp(49);
						break;

					case 'R':
						disp(50);
						break;
					case 'S':
						disp(51);
						break;

					case 'T':
						disp(52);
						break;

					case 'U':
						disp(53);
						break;
					case 'V':
						disp(54);
						break;

					case 'W':
						disp(55);
						break;

					case 'X':
						disp(56);
						break;
					case 'Y':
						disp(57);
						break;

					case 'Z':
						disp(58);
						break;

					case ' ':
						disp(0);
						break;
					case '!':
						disp(1);
						break;
					case '"':
						disp(2);
						break;
					case '#':
						disp(3);
						break;
					case '$':
						disp(4);
						break;
					case '%':
						disp(5);
						break;
					case '&':
						disp(6);
						break;
					case '(':
						disp(8);
						break;
					case ')':
						disp(9);
						break;
					case '*':
						disp(10);
						break;
					case '+':
						disp(11);
						break;
					case ',':
						disp(12);
						break;
					case '-':
						disp(13);
						break;
					case '.':
						disp(14);
						break;
					case '/':
						disp(15);
						break;
					case '0':
						disp(16);
						break;
					case '1':
						disp(17);
						break;
					case '2':
						disp(18);
						break;
					case '3':
						disp(19);
						break;
					case '4':
						disp(20);
						break;
					case '5':
						disp(21);
						break;
					case '6':
						disp(22);
						break;
					case '7':
						disp(23);
						break;
					case '8':
						disp(24);
						break;
					case '9':
						disp(25);
						break;
					case ':':
						disp(26);
						break;
					case ';':
						disp(27);
						break;
					case '<':
						disp(28);
						break;
					case '=':
						disp(29);
						break;
					case '>':
						disp(30);
						break;
					case '?':
						disp(31);
						break;
					case '@':
						disp(32);
						break;

					}

		}

	}

}
void USART_Init() {
	UBRRH = (unsigned char) (baud_select >> 8);
	UBRRL = (unsigned char) baud_select;
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
	//2 stop bit and data bit length is 8-bit
}

void USART_Transmit(unsigned char data) {
	while (!(UCSRA & (1 << UDRE)))
		;
	UDR = data;
}

unsigned char USART_Receive() {
	while (!(UCSRA & (1 << RXC)))
		;
	return UDR;
}
void pattern(int i) {

	PORTB = layer[i];
	PORTD = selection[i];
	PORTC = 0x00;
	PORTC = 0xFF;
	_delay_ms(100);

}
void disp(int i) {

	int j = 0;

	for (j = 0; j < 8; j++) {
		PORTA = font[i][j];
		pattern(j);
		if (j == 7) {
			for (j = 7; j >= 0; j--) {
				pattern(j);
			}
			j = 7;
		}

	}
	//_delay_ms(500);

}

