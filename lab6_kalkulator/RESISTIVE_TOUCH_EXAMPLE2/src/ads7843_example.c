/**
 * \file
 *
 * \brief ads7843 touchscreen controller Example.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage ads7843 Touchscreen controller Example
 *
 * \section Purpose
 *
 * This example shows how to use SPI to control touchscreen controller (ADS7843).
 * It can also help you to get familiar with the touchscreen configurations
 * and usage, which can be used for fast implementation of your own touchscreen
 * driver and other applications related.
 *
 * \section Requirements
 *
 * This package can be used with SAM evaluation kits.
 *
 * \section Description
 *
 * This example first initialize LCD and touchscreen controller, then let
 * user do calibration for the touchscreen. After calibration is done, user may
 * touch the LCD and the pen position will be output on terminal.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application
 * -# In HyperTerminal, it will show something like
 *     \code
	-- SPI_Touchscreen Example xxx --
	-- xxxxxx-xx
	-- Compiled: xxx xx xxxx xx:xx:xx --
\endcode
 * -# Touch the dots on the LCD to calibrate the touchscreen, the calibration
 *    results will output on the terminal and LCD.
 * -# Touch the LCD, the pen position will output on the terminal
 *    if touchscreen is calibrated ok.
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "resistive_touch_board.h"
#include "rtouch_calibrate.h"
#include "asm_func.h"


/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

#define STRING_EOL    "\r"
#define STRING_HEADER "-- SPI_Touchscreen Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/** Tick Counter united by ms */
static volatile uint32_t g_ul_tick_count = 0;

typedef void (*fp)();


/**
 * \brief Handler for SysTick interrupt. Increments the timestamp counter.
 */
void SysTick_Handler(void)
{
	g_ul_tick_count++;

	/* Call TSD_TimerHandler per 10ms */
	if ((g_ul_tick_count % 10) == 0) {
		rtouch_process();
	}
}

// Variables for calculator

int op1 = 0, op2 = 0;
int *target_operand;

int calculator_mode = 1; //1 - dodawanie, 2 - odejmowanie, 3 - mno¿enie, 4 - dzielenie
int calculator_state = 0; // 0 - czekam na 1 sk³adnik, 1 - czekam na drugi skladni, 2 - czekam na wynik
void add_digit_to_operand(int a)
{
	if (*target_operand < 100000000) {
		*target_operand = 10 * (*target_operand) + a;
	}
}


void add_operands()
{	
	op1 = asm_add(op1, op2);
	op2 = 0;
}

void sub_operands()
{
	op1 = asm_sub(op1, op2);
	op2 = 0;
}

void mul_operands()
{
	op1 = asm_mul(op1, op2);
	op2 = 0;
}

void div_operands()
{
	op1 = asm_div(op1, op2);
	op2 = 0;
}

// Operacja
fp operacja = add_operands;


void draw_digit_screen()
{
		lcd_set_foreground_color(COLOR_GREEN);
		lcd_draw_filled_rectangle(8, 8, 232, 66);
}

void draw_button_strings() {
		lcd_set_foreground_color(COLOR_GREEN);
		lcd_draw_string(26, 108, "7");
		lcd_draw_string(84, 108, "8");
		lcd_draw_string(142, 108, "9");
		lcd_draw_string(200, 108, "X");
		
		lcd_draw_string(26, 166, "4");
		lcd_draw_string(84, 166, "5");
		lcd_draw_string(142, 166, "6");
		lcd_draw_string(200, 166, "/");
		
		lcd_draw_string(26, 224, "1");
		lcd_draw_string(84, 224, "2");
		lcd_draw_string(142, 224, "3");
		lcd_draw_string(200, 224, "-");

		lcd_draw_string(26, 282, "C");
		lcd_draw_string(84, 282, "0");
		lcd_draw_string(142, 282, "=");
		lcd_draw_string(200, 282, "+");
}

void draw_buttons() {
		lcd_set_foreground_color(COLOR_BLUE);
		lcd_draw_filled_rectangle(8, 90, 58, 140);
		lcd_draw_filled_rectangle(66, 90, 116, 140);
		lcd_draw_filled_rectangle(124, 90, 174, 140);
		
		lcd_draw_filled_rectangle(8, 148, 58, 198);
		lcd_draw_filled_rectangle(66, 148, 116, 198);
		lcd_draw_filled_rectangle(124, 148, 174, 198);

		lcd_draw_filled_rectangle(8, 206, 58, 256);
		lcd_draw_filled_rectangle(66, 206, 116, 256);
		lcd_draw_filled_rectangle(124, 206, 174, 256);
		
		lcd_draw_filled_rectangle(66, 264, 116, 314);
		
		lcd_set_foreground_color(COLOR_RED);
		lcd_draw_filled_rectangle(182, 90, 232, 140);
		lcd_draw_filled_rectangle(182, 148, 232, 198);
		lcd_draw_filled_rectangle(182, 206, 232, 256);
		lcd_draw_filled_rectangle(182, 264, 232, 314);
		lcd_draw_filled_rectangle(124, 264, 174, 314);
		lcd_draw_filled_rectangle(8, 264, 58, 314);

		
}

void drawScene()
{
	lcd_set_foreground_color(COLOR_GREEN);
	lcd_draw_filled_rectangle(0, 0, 239, 319);

	draw_digit_screen();
	draw_buttons();
	draw_button_strings();
	
}

void add_to_calc(int digit) {
	switch(calculator_state) {
		case 0:
			op1 = op1 * 10 + digit;
		break;
		
		case 1:
			op2 = op2 * 10 + digit;
		break;
	}
}

void calc() {
	
	switch(calculator_mode) {
		case 1:
			op1 = asm_add(op1, op2);
			op2 = 0;
		break;
		
		case 2:
		    op1 = asm_sub(op1, op2);
			op2 = 0;
		break;
		
		case 3:
		    op1 = asm_mul(op1, op2);
			op2 = 0;		
		break;
		
		case 4:
		    op1 = asm_div(op1, op2);
			op2 = 0;
		break;
	}
}

void button_event_handler(int number)
{
	switch(number) 
	{
		case 0:
			add_to_calc(7);
			break;
		case 1:
			add_to_calc(8);
			break;
		case 2:
			add_to_calc(9);
			break;
		case 3:
			//mno¿enie
			calculator_mode = 3;
			calculator_state = 1;
			break;
		case 4:
			add_to_calc(4);
			break;
		case 5:
			add_to_calc(5);
			break;
		case 6:
			add_to_calc(6);
			break;
		case 7:
			//dzielenie
			calculator_mode = 4;
			calculator_state = 1;		
			break;
		case 8:
			add_to_calc(1);
			break;
		case 9:
			add_to_calc(2);
			break;
		case 10:
			add_to_calc(3);
			break;
		case 11:
			//odejmowanie
			calculator_mode = 2;
			calculator_state = 1;
			break;
		case 12:
			//kasowanie
			calculator_mode = 1;
			calculator_state = 0;
			op1 = 0;
			op2 = 0;
			break;
		case 13:
			add_to_calc(0);
			break;
		case 14:
			calc();
			calculator_mode = 1;
			calculator_state = 0;
			break;
		case 15:
			//dodawanie 
			calculator_mode = 1;
			calculator_state = 1;
			break;		
	}
	
}

void press_button_handler(unsigned int x, unsigned int y)
{
	short bx, by;
	if(x >= 8 && x <= 58) {
		bx = 0;
	} else if(x >= 66 && x <= 116) {
		bx = 1;
	} else if(x >= 124 && x <= 174) {
		bx = 2;
	} else if(x >= 182 && x <= 232) {
		bx = 3;
	}
	
	if(y >= 90 && y <=140) {
		by = 0;
	} else if(y >= 148 && y <=198) {
		by = 1;
	} else if(y >= 206 && y <=256) {
		by = 2;
	} else if(y >= 264 && y <=314) {
		by = 3;
	}

	button_event_handler(4*by + bx);
	
	draw_digit_screen();
	lcd_set_foreground_color(COLOR_BLUE);
	
	char tab[20] = {};
		
	itoa(op1, tab, 10);
	lcd_draw_string(20, 20, tab);
	
	if(calculator_state == 1) {
		itoa(op2, tab, 10);
		lcd_draw_string(20, 40, tab);
	}

}

/**
 * \brief Callback called when the pen is pressed on the touchscreen.
 *
 * \param x horizontal position (in pixel if ts calibrated).
 * \param y vertical position (in pixel if ts calibrated).
 */
static void event_handler(rtouch_event_t const *event)
{
	switch(event->type) {
	case RTOUCH_MOVE:
		printf("Pen moved at     (%03u, %03u)\n\r", (unsigned int) event->panel.x, (unsigned int) event->panel.y);
		break;
	case RTOUCH_PRESS:
		press_button_handler((unsigned int) event->panel.x, (unsigned int) event->panel.y);
		printf("Pen pressed at   (%03u, %03u)\n\r", (unsigned int) event->panel.x, (unsigned int) event->panel.y);
		break;
	case RTOUCH_RELEASE:
		printf("Pen released at  (%03u, %03u)\n\r", (unsigned int) event->panel.x, (unsigned int) event->panel.y);
		break;
	default:
		break;
	}
}

/**
 *  \brief Configure the Console UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}


/**
 * \brief Application entry point for spi_touchscreen example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t uc_result;

	sysclk_init();
	board_init();

	configure_console();

	/* Enable clocks for push buttons management */
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_PIOC);

	/* Output example information */
	puts(STRING_HEADER);

	/* Configure systick for 1 ms. */
	puts("Configure system tick to get 1ms tick period.\r");
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		puts("Systick configuration error\r");
		while (1);
	}

	/* Switch off backlight */
	aat31xx_disable_backlight();

	lcd_init();

	lcd_set_foreground_color(UNI_COLOR_WHITE);
	lcd_draw_filled_rectangle(0, 0, LCD_WIDTH, LCD_HEIGHT);

	/* Turn on LCD */
	lcd_display_on();

	/* Switch on backlight */
	aat31xx_set_backlight(AAT31XX_MAX_BACKLIGHT_LEVEL);

	/* Initializes the PIO interrupt management for touchscreen driver */
	pio_handler_set_priority(PIOA, PIOA_IRQn, IRQ_PRIOR_PIO);
	pio_handler_set_priority(PIOB, PIOB_IRQn, IRQ_PRIOR_PIO);
	pio_handler_set_priority(PIOC, PIOC_IRQn, IRQ_PRIOR_PIO);

	/* Initialize touchscreen without calibration */
	rtouch_init(LCD_WIDTH, LCD_HEIGHT);
	rtouch_enable();
	rtouch_set_event_handler(event_handler);

	while (1) {
		uc_result = rtouch_calibrate();
		if (uc_result == 0) {
			puts("Calibration successful !\r");
			break;
		} else {
			puts("Calibration failed; error delta is too big ! Please retry calibration procedure...\r");
		}
	}

	target_operand = &op1;

	drawScene();

	while (1) {
		
	}
}

