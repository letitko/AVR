#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include "UART0.h"
#include "UART1.h"

void UART0_init(void);
void UART0_transmit(char data);
unsigned char UART0_receive(void);
void UART0_print_string(char *str);

void UART1_init(void);
void UART1_transmit(char data);
unsigned char UART1_receive(void);
void UART1_print_string(char *str);

void Umbrella_unfolding(void);
void Umbrella_folding(void);
void Umbrella_staying(void);

void Make_Umbrella_Folding_mode(void);
void Make_Umbrella_Unfolding_mode(void);

int main() {

	DDRA = 0xff; //출력 모터드라이버 출력 설정
	UART1_init(); //UART1초기화
	
	unsigned char fore_state=NULL;
	unsigned char umbrella_state=NULL;
	
	//char str[] = "I want a take U to a gay Bar";
	//uint8_t num = 255;

	UART1_init(); // UART1 초기화
	
	while(1) {

			//UART1_print_string(str); // 문자열 출력
			//UART1_print_string("\n\r");
			//_delay_ms(50);
			//UART1_print_1_byte_number(num); // 1바이트 크기 정수 출력
			//UART1_print_string("\n\r");

				fore_state = UART1_receive();
				if((int)umbrella_state != (int)fore_state) {

					if(fore_state == 's') {
						umbrella_state ='s';
						Make_Umbrella_Folding_mode();
					}
					
					else if(fore_state == 'w') {
						umbrella_state = 'w';
						Make_Umbrella_Unfolding_mode();
					}
					
				}
				
			}
			
	return 0;
}

// code level

// umbrella func

void Umbrella_staying(void) {
	PORTA = 0b11111111; // 대기
}

void Umbrella_folding(void) {
	PORTA = 0b01010101; // 역방향
}
void Umbrella_unfolding(void) {
	PORTA = 0b10101010; //순방향
}

void Make_Umbrella_Folding_mode(void) {
	Umbrella_folding();
	_delay_ms(3000);
	Umbrella_staying();
}

void Make_Umbrella_Unfolding_mode(void) {
	Umbrella_unfolding();
	_delay_ms(3000);
	Umbrella_staying();
}


// UART1

void UART1_init(void)  // 통신초기화 함수
{
	UBRR1H = 0x00;
	UBRR1L = 207; //보율 207 == 9600 / 16 == 115200설정
	
	UCSR1A = _BV(U2X1); //2배속 모드
	UCSR1C = 0x06; //8비트의 데이터, 비동기식
	
	UCSR1B |= _BV(RXEN1);
	UCSR1B |= _BV(TXEN1);  //송수신 가능
}

void UART1_transmit(char data)
{
	while(!(UCSR1A & (1 << UDRE1))); //송신가능대기
	UDR1 = data;
}

unsigned char UART1_receive(void)
{
	while(!(UCSR1A & (1<<RXC1))); //데이터 수신 대기
	return UDR1;
}

void UART1_print_string(char *str) // 문자열 송신
{
	for(int i =0;str[i];i++) // '\0' 문자를 만날 때까지 반복
	UART1_transmit(str[i]); // 바이트 단위 출력
}

void UART1_print_1_byte_number(uint8_t n)
{
	char numString[4] = "0";
	int i, index = 0;
	
	if(n>0){ // 문자열 변환
		for(i=0; n!=0;i++){
			numString[i] = n % 10 +'0';
			n = n/10;
		}
		
		numString[i] = '\0';
		index = i - 1;
	
	}
	
	for(i=index; i>=0;i--) // 변환된 문자열 역순으로 출력
		UART1_transmit(numString[i]);
	
}

// UART0

void UART0_init(void)  // 통신초기화 함수
{
	UBRR0H = 0x00;
	UBRR0L = 207; //보율 207 == 9600 / 16 == 115200설정
	
	UCSR0A = _BV(U2X1); //2배속 모드
	UCSR0C = 0x06; //8비트의 데이터, 비동기식
	
	UCSR0B |= _BV(RXEN1);
	UCSR0B |= _BV(TXEN1);  //송수신 가능
}

void UART0_transmit(char data)
{
	while(!(UCSR0A & (1 << UDRE0))); //송신가능대기
	UDR0 = data;
}

unsigned char UART0_receive(void)
{
	while(!(UCSR0A & (1<<RXC1))); //데이터 수신 대기
	return UDR0;
}

void UART0_print_string(char *str) // 문자열 송신
{
	for(int i =0;str[i];i++) // '\0' 문자를 만날 때까지 반복
	UART0_transmit(str[i]); // 바이트 단위 출력
}

void UART0_print_1_byte_number(uint8_t n)
{
	char numString[4] = "0";
	int i, index = 0;
	
	if(n>0){ // 문자열 변환
		for(i=0; n!=0;i++){
			numString[i] = n % 10 +'0';
			n = n/10;
		}
		
		numString[i] = '\0';
		index = i - 1;
	
	}
	
	for(i=index; i>=0;i--) // 변환된 문자열 역순으로 출력
		UART0_transmit(numString[i]);
	
}
