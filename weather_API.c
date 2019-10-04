#define F_CPU 16000000L
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>

void UART1_init(void);
void UART0_init(void);
void UART1_transmit(char data);
void UART1_print_string(char *str);
unsigned char UART1_receive(void);
unsigned char UART0_receive(void);
void Wifi_Init(void);
void connect_Server(void);
void disconnect_Server(void);
void UART1_wait_receive(void);
void UART1_reply_receive(char *buffer);
char reply_parsing(char* buffer, char* UUU);
void UART0_transmit(char data);

int main(void)
{
   UART0_init();
   UART1_init();
   Wifi_Init();
   char buffer[2200];
   char a[4]="UUU";
   while(1) {
      memset(buffer,0,2200);
      disconnect_Server();
      connect_Server();
      UART1_print_string("GET /service/SecndSrtpdFrcstInfoService2/ForecastSpaceData?ServiceKey=oKkRSmbYF6oWEeqgrxSkahZvUmDbY5AOESsWfRK87khxbjutCbvfrBcVHAJOUKP0W3ldMEEyM6Qn%2F1Dyz8zVKg%3D%3D&base_date=20181124&base_time=0200&nx=92&ny=131 HTTP/1.1\r\nHost: newsky2.kma.go.kr\r\n\r\n");
      UART1_reply_receive(buffer);
      _delay_ms(5000);
      char temp = reply_parsing(buffer,a);
       UART0_transmit(temp);
      _delay_ms(3000);   
   }
   return 0;
}
void UART1_init(void)
{
   UBRR1H = 0x00;         // 115200 보율로 설정
   UBRR1L = 16;
   
   UCSR1A |= _BV(U2X1);         // 2배속 모드
   // 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
   UCSR1C |= 0x06;
   
   UCSR1B |= _BV(RXEN1);      // 송수신 가능
   UCSR1B |= _BV(TXEN1);
}
void UART1_transmit(char data) {
   while( !(UCSR1A & (1 << UDRE1)) );   // 송신 가능 대기
   UDR1 = data;            // 데이터 전송
}
void UART1_print_string(char *str) {
   for(int i = 0; str[i]; i++)
   UART1_transmit(str[i]);
}
unsigned char UART1_receive(void) {
   int timeout = 65535;
   for(int i = 0; i < 100; i++) {
      while( !(UCSR1A & (1<<RXC1)) ) {
         timeout--;
         if(timeout == 0) break;
      }
      timeout = 65535;
   }
   while( !(UCSR1A & (1<<RXC1)) ) {
      timeout--;
      if(timeout == 0) return 0;
   }
   return UDR1;
}
void Wifi_Init(void) {
   UART1_print_string("AT+CWMODE=3\r\n");
   UART1_wait_receive();
   UART1_print_string("AT+CWJAP=\"Takeout1\",\"takeout0503\"\r\n");
   UART1_wait_receive();
}
void connect_Server(void) {
   UART1_print_string("AT+CIPSTART=\"TCP\",\"newsky2.kma.go.kr\",80\r\n");
   UART1_wait_receive();
   UART1_print_string("AT+CIPSEND=249\r\n");
   UART1_wait_receive();
}
void disconnect_Server(void) {
   UART1_print_string("AT+CIPCLOSE\r\n");
   UART1_wait_receive();
}
void UART1_wait_receive(void) {
   char temp;
   while(1) {
      temp = UART1_receive();
      if(temp == 0) break;
   }
}
void UART1_reply_receive(char *buffer) {
   int index=0;
   while(1) {
      buffer[index] = UART1_receive();
      if(buffer[index] == 0) break;
      index++;
   }
}
char reply_parsing(char* buffer, char* UUU)
{ 
   int index = 0;
   char temp=0;
   while(1)
   {
      if(buffer[index]=='U'){
         if(buffer[index+1]=='U'){
            if(buffer[index+2]=='U')
            {
               temp = buffer[index+79];
               for(int j=0; j < 30; j++) {
                  UART0_transmit(buffer[index+79+j]);
               }
               return temp;
            }
         }
      }
      index++;
    }
}

void UART0_init(void)
{
   UBRR0H = 0x00;         // 9600 보율로 설정
   UBRR0L = 207;
   
   UCSR0A |= _BV(U2X1);         // 2배속 모드
   // 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
   UCSR0C |= 0x06;
   
   UCSR0B |= _BV(RXEN1);      // 송수신 가능
   UCSR0B |= _BV(TXEN1);
}
void UART0_transmit(char data)
{
   while( !(UCSR0A & (1 << UDRE0)) );   // 송신 가능 대기
   UDR0 = data;            // 데이터 전송

}
void UART0_print_string(char *str) {
   for(int i = 0; str[i] != 0; i++)
      UART0_transmit(str[i]);
}
unsigned char UART0_receive(void)
{
   while( !(UCSR0A & (1<<RXC1)) );   // 데이터 수신 대기
   return UDR0;
}