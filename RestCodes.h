/*
	UART1_init(); //UART1초기화
	
	char str[] = "reset completed";
	//uint8_t num = 255;
	
	while(1) {
		UART1_print_string(str); // 문자열 출력
		UART1_print_string("\n\r");
		_delay_ms(50);
		//UART1_print_1_byte_number(num); // 1바이트 크기 정수 출력
		//UART1_print_string("\n\r");
	}
	
	return 0;
*/