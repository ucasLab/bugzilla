/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
/* Systick Counter */
static volatile uint32_t g_ul_ms_ticks = 0U;

void SysTick_Handler(void) {
	/* Increment counter necessary in delay(). */
	g_ul_ms_ticks++;
}

struct hx8347a_opt_t g_hx8347a_display_opt;

/* Convert 24-bits color to 16-bits color */
static hx8347a_color_t rgb24_to_rgb16(uint32_t ul_color) {
	hx8347a_color_t result_color;
	result_color = (((ul_color >> 8) & 0xF800) | ((ul_color >> 5) & 0x7E0)
			| ((ul_color >> 3) & 0x1F));
	return result_color;
}

/* Screen buffer in PSRAM memory */
static hx8347a_color_t *screenbuffer = (hx8347a_color_t *) PSRAM_BASE_ADDRESS;
/* Flames buffer in PSRAM*/
unsigned char *flamesbuffer = (unsigned char *) (PSRAM_BASE_ADDRESS + 0x50000);
/* Color palette */
unsigned short r[256]; // red
unsigned short g[256]; // green
unsigned short b[256]; // blue

/* Optimized flames generator */
void opt(void);

/* Nonoptimized flames generator */
void nonopt(void);

int main(void) {
	sysclk_init();
	board_init();
	/* Set up SysTick Timer for 1 msec interrupts. */
	if (SysTick_Config(SystemCoreClock / (uint32_t) 1000)) {
		/* Capture error. */
		while (1) {
		}
	}

	pmc_enable_periph_clk(ID_SMC);
	/* Configure and enable PSRAM memory */
	smc_set_setup_timing(SMC, 0,
			SMC_SETUP_NWE_SETUP(0) | SMC_SETUP_NCS_WR_SETUP(2)
					| SMC_SETUP_NRD_SETUP(2) | SMC_SETUP_NCS_RD_SETUP(2));
	smc_set_pulse_timing(SMC, 0,
			SMC_PULSE_NWE_PULSE(6) | SMC_PULSE_NCS_WR_PULSE(6)
					| SMC_PULSE_NRD_PULSE(6) | SMC_PULSE_NCS_RD_PULSE(6));
	smc_set_cycle_timing(SMC, 0,
			SMC_CYCLE_NWE_CYCLE(8) | SMC_CYCLE_NRD_CYCLE(8));
	smc_set_mode(SMC, 0,
			SMC_MODE_READ_MODE | SMC_MODE_WRITE_MODE | SMC_MODE_DBW_BIT_16);
	/* Configure and enable LCD display */
	smc_set_setup_timing(SMC, CONF_BOARD_HX8347A_LCD_CS,
			SMC_SETUP_NWE_SETUP(1) | SMC_SETUP_NCS_WR_SETUP(1)
					| SMC_SETUP_NRD_SETUP(9) | SMC_SETUP_NCS_RD_SETUP(9));
	smc_set_pulse_timing(SMC, CONF_BOARD_HX8347A_LCD_CS,
			SMC_PULSE_NWE_PULSE(4) | SMC_PULSE_NCS_WR_PULSE(4)
					| SMC_PULSE_NRD_PULSE(36) | SMC_PULSE_NCS_RD_PULSE(36));
	smc_set_cycle_timing(SMC, CONF_BOARD_HX8347A_LCD_CS,
			SMC_CYCLE_NWE_CYCLE(10) | SMC_CYCLE_NRD_CYCLE(45));
	smc_set_mode(SMC, CONF_BOARD_HX8347A_LCD_CS,
			SMC_MODE_READ_MODE | SMC_MODE_WRITE_MODE | SMC_MODE_DBW_BIT_16);

	/* Clear falmes buffer */
	for (int i = 0; i < 320 * 240; i++)
		flamesbuffer[i] = 0;

	/* Init falmes colors */
	int i;
	for (i = 0; i < 256; i++) {
		if (i < 32) {
			r[i] = (i * 7) << 8;
			g[i] = b[i] = 0;
		} else if (i < 64) {
			r[i] = 224 << 8;
			g[i] = ((i - 32) * 4) << 8;
			b[i] = 0;
		} else if (i < 96) {
			r[i] = (224 + (i - 64)) << 8;
			g[i] = (128 + (i - 64) * 3) << 8;
			b[i] = 0;
		} else {
			r[i] = g[i] = 255 << 8;
			b[i] = 128 << 8;
		}
	}
	/* Initialize display parameter */
	g_hx8347a_display_opt.ul_width = HX8347A_LCD_WIDTH;
	g_hx8347a_display_opt.ul_height = HX8347A_LCD_HEIGHT;
	g_hx8347a_display_opt.foreground_color = rgb24_to_rgb16(COLOR_BLACK);
	g_hx8347a_display_opt.background_color = rgb24_to_rgb16(COLOR_WHITE);

	/* Switch off backlight */
	aat31xx_disable_backlight();

	/* Initialize LCD */
	hx8347a_init(&g_hx8347a_display_opt);

	/* Set backlight level */
	aat31xx_set_backlight(AAT31XX_AVG_BACKLIGHT_LEVEL);

	/* Turn on LCD */
	hx8347a_display_on();
	
	/* Set color */
	hx8347a_set_foreground_color(rgb24_to_rgb16(COLOR_WHITE));
	
	/* Draw flames */
	opt();
}

/* Time counter string */
char str[32];

void nonopt() {
	while (1) {
		/* Start timer */
		g_ul_ms_ticks = 0;
		
		/* Init flames */
		for (int x = 1; x < 239; x++)
			flamesbuffer[x + 240 * 318] = rand() % 256;
		for (int x = 1; x < 239; x++)
			flamesbuffer[x + 240 * 317] = rand() % 256;

		/* Draw flames in flames buffer */
		for (int y = 1; y < 318; y++)
			for (int x = 1; x < 239; x++) {
				flamesbuffer[x + 240 * (y - 1)] = (flamesbuffer[x - 1 + 240 * y]
						+ flamesbuffer[x + 1 + 240 * y] + flamesbuffer[x + 240 * (y + 1)]
						+ flamesbuffer[x + 240 * y] + flamesbuffer[x + 240 * (y + 2)]) / 5;
				if (flamesbuffer[x + 240 * (y - 1)] > 0)
					flamesbuffer[x + 240 * (y - 1)]--;
			}
		
		/* Convert flames buffer to screen buffer */
		for (int i = 0; i < 320 * 240; i++) {
			screenbuffer[i] = rgb24_to_rgb16(
					(r[flamesbuffer[i]] << 16) + (g[flamesbuffer[i]] << 8) + b[flamesbuffer[i]]);
		}
		/* Convert time to string */
		itoa(g_ul_ms_ticks, str, 10);

		/* Display flames on screen */	
		hx8347a_draw_pixmap(0, 0, 240, 320, screenbuffer);

		/* Display time on screen */
		hx8347a_draw_string(10, 20, (unsigned char*) str);
	}
}

void opt() {
	
	unsigned char flamesbuffer_new[240*120];
	for (int i = 0; i < 210 * 240; i++) 
			screenbuffer[i] = 0; // rgb24_to_rgb16(0);
			
	int m1 = 240*118;
	int m2 = m1-240;
	int m3 = 200*240;
	int m4 = 120*240;
while (1) {
		/* Start timer */
		g_ul_ms_ticks = 0;
		
		/* Init flames */
		int k,kk;
		for (int x = 1; x < 239; x++){
			k = rand() % 256;
			kk = rand() % 256;
			flamesbuffer_new[x + m1] = k;
			flamesbuffer_new[x + m2] = kk;
		}
	
		/* Draw flames in flames buffer */
		for (int y = 1; y < 118; y++)
			for (int x = 1; x < 239; x++) {
				flamesbuffer_new[x + 240 * (y - 1)] = (flamesbuffer_new[x - 1 + 240 * y]
						+ flamesbuffer_new[x + 1 + 240 * y] + flamesbuffer_new[x + 240 * (y + 1)]
						+ flamesbuffer_new[x + 240 * y] + flamesbuffer_new[x + 240 * (y + 2)]) / 5 -1;
			}
		/* Convert flames buffer to screen buffer */
			
		
		for (int i = 0; i < m4; i++) {
			unsigned char  w = flamesbuffer_new[i];
			screenbuffer[i + m3] = rgb24_to_rgb16(
					(r[w] << 16) + (g[w] << 8) + b[w]);
		}
		/* Convert time to string */
		itoa(g_ul_ms_ticks, str, 10);

		/* Display flames on screen */	
		hx8347a_draw_pixmap(0, 0, 240, 320, screenbuffer);

		/* Display time on screen */
		hx8347a_draw_string(10, 20, (unsigned char*) str);
	}
}
