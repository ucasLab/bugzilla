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
#include <math.h>
#include "fixed_point.h"
/* Systick Counter */
static int Q4 = 4;
static int Q = 4;
static int Q8 = 4;
static volatile uint32_t g_ul_ms_ticks = 0U;

typedef struct {
	short x;
	short y;
	short z;
	} Point;
	
 Point p1, p2, p3, p4, p5, p6, p7, p8;
 Point r1, r2, r3, r4, r5, r6, r7, r8;
void SysTick_Handler(void) {
	g_ul_ms_ticks++;
}

struct hx8347a_opt_t g_hx8347a_display_opt;

static hx8347a_color_t rgb24_to_rgb16(uint32_t ul_color) {
	hx8347a_color_t result_color;
	result_color = (((ul_color >> 8) & 0xF800) | ((ul_color >> 5) & 0x7E0)
			| ((ul_color >> 3) & 0x1F));
	return result_color;
}

static short d;

short persx(Point p) {
	return TOFLT(FADD(TOFIX(120, Q4), FDIV(FMUL(p.x, d, Q4), FADD(p.z, d), Q4)), Q4);
}

short persy(Point p) {
	return TOFLT(FADD(TOFIX(150, Q4), FDIV(FMUL(p.y, d, Q4), FADD(p.z, d), Q4)), Q4);
}

void printCube() {
	
	hx8347a_set_foreground_color(rgb24_to_rgb16(COLOR_WHITE));
	hx8347a_draw_filled_rectangle(0, 0, 240, 320);
	
	hx8347a_set_foreground_color(rgb24_to_rgb16(COLOR_RED));
	
	//front
	hx8347a_draw_line(persx(p1), persy(p1), persx(p2), persy(p2));
	hx8347a_draw_line(persx(p2), persy(p2), persx(p3), persy(p3));
	hx8347a_draw_line(persx(p3), persy(p3), persx(p4), persy(p4));
	hx8347a_draw_line(persx(p4), persy(p4), persx(p1), persy(p1));

	//back
	hx8347a_draw_line(persx(p5), persy(p5), persx(p6), persy(p6));
	hx8347a_draw_line(persx(p6), persy(p6), persx(p7), persy(p7));
	hx8347a_draw_line(persx(p7), persy(p7), persx(p8), persy(p8));
	hx8347a_draw_line(persx(p8), persy(p8), persx(p5), persy(p5));
	
	//sides
	hx8347a_draw_line(persx(p1), persy(p1), persx(p5), persy(p5));
	hx8347a_draw_line(persx(p2), persy(p2), persx(p6), persy(p6));
	hx8347a_draw_line(persx(p3), persy(p3), persx(p7), persy(p7));
	hx8347a_draw_line(persx(p4), persy(p4), persx(p8), persy(p8));
}

void calc_in_x(Point *t, Point *p, double alpha) {
	p->x = t->x;
	p->y = FSUB(FMULG(t->y, TOFIX(cos(alpha), Q8), Q4, Q8, Q4) , FMULG(t->z, TOFIX(sin(alpha), Q8), Q4, Q8, Q4));
	p->z = FADD(FMULG(t->y, TOFIX(sin(alpha), Q8), Q4, Q8, Q4) , FMULG(t->z, TOFIX(cos(alpha), Q8), Q4, Q8, Q4));
}


void calc_in_y(Point *t, Point *p, double alpha) {
	p->x = FADD(FMULG(t->z, TOFIX(sin(alpha), Q8), Q4, Q8, Q4) , FMULG(t->x, TOFIX(cos(alpha), Q8), Q4, Q8, Q4));
	p->y = t->y;
	p->z = FSUB(FMULG(t->z, TOFIX(cos(alpha), Q8), Q4, Q8, Q4), FMULG(t->x, TOFIX(sin(alpha), Q8), Q4, Q8, Q4));
}

void calc_in_z(Point *t, Point *p, double alpha) {
	p->x = FSUB(FMULG(t->x, TOFIX(cos(alpha), Q8), Q4, Q8, Q4), FMULG(t->y, TOFIX(sin(alpha), Q8), Q4, Q8, Q4));
	p->y = FADD(FMULG(t->x, TOFIX(sin(alpha), Q8), Q4, Q8, Q4), FMULG(t->y, TOFIX(cos(alpha), Q8), Q4, Q8, Q4));
	p->z = t->z;
}

void calc_in_xy(Point *t, Point *p, double alpha, double beta) {
	double a = sin(alpha) * sin(beta);
	double b = sin(alpha)*cos(beta);
	double c = sin(beta) * cos(alpha);
	double d = cos(alpha) + cos(beta);
	
	
	p->x = FMULG(t->x, TOFIX(cos(beta), Q8), Q4, Q8, Q4);
	p->x += FMULG(t->z, TOFIX(sin(beta), Q8), Q4, Q8, Q4);
	
	p->y = FMULG(t->x, TOFIX(a, Q8), Q4, Q8, Q4);
	p->y += FMULG(t->y, TOFIX(cos(alpha), Q8), Q4, Q8, Q4);
	p->y -= FMULG(t->z, TOFIX(b, Q8), Q4, Q8, Q4);
	
	p->z = -FMULG(t->x, TOFIX(c, Q8), Q4, Q8, Q4);
	p->z += FMULG(t->y, TOFIX(sin(alpha), Q8), Q4, Q8, Q4);
	p->z += FMULG(t->z, TOFIX(d, Q8), Q4, Q8, Q4);

}

void opt() {

short start = -40.0;
short length = 80.0;

short param = 0.0;
double alpha, beta, gamma;

int x, y, z;

x = 1;
y = 1;
z = 1;

p1.x = TOFIX(start, Q4);
p1.y = TOFIX(start, Q4);
p1.z = TOFIX(-0.5 * length, Q4);

p2.x = TOFIX(start + length, Q4);
p2.y = TOFIX(start, Q4);
p2.z = TOFIX(-0.5 * length, Q4);

p3.x = TOFIX(start + length, Q4);
p3.y = TOFIX(start + length, Q4);
p3.z = TOFIX(-0.5 * length, Q4);

p4.x = TOFIX(start, Q4);
p4.y = TOFIX(start + length, Q4);
p4.z = TOFIX(-0.5 * length, Q4);

p5.x = TOFIX(start, Q4);
p5.y = TOFIX(start, Q4);
p5.z = TOFIX(0.5 * length, Q4);

p6.x = TOFIX(start + length, Q4);
p6.y = TOFIX(start, Q4);
p6.z = TOFIX(0.5 * length, Q4);

p7.x = TOFIX(start + length, Q4);
p7.y = TOFIX(start + length, Q4);
p7.z = TOFIX(0.5 * length, Q4);

p8.x = TOFIX(start, Q4);
p8.y = TOFIX(start + length, Q4);
p8.z = TOFIX(0.5 * length, Q4);

Point t1,t2,t3,t4,t5,t6,t7,t8;
t1=p1;t2=p2;t3=p3;t4=p4;t5=p5;t6=p6;t7=p7;t8=p8;


while(1) {
	
	if(x == 1) {
		calc_in_x(&t1, &p1, alpha);
		calc_in_x(&t2, &p2, alpha);
		calc_in_x(&t3, &p3, alpha);
		calc_in_x(&t4, &p4, alpha);
		calc_in_x(&t5, &p5, alpha);
		calc_in_x(&t6, &p6, alpha);
		calc_in_x(&t7, &p7, alpha);
		calc_in_x(&t8, &p8, alpha);
	}

	if(y == 1) {
		calc_in_y(&t1, &r1, alpha);
		calc_in_y(&t2, &r2, alpha);
		calc_in_y(&t3, &r3, alpha);
		calc_in_y(&t4, &r4, alpha);
		calc_in_y(&t5, &r5, alpha);
		calc_in_y(&t6, &r6, alpha);
		calc_in_y(&t7, &r7, alpha);
		calc_in_y(&t8, &r8, alpha);
		
		calc_in_z(&r1, &p1, alpha);
		calc_in_z(&r2, &p2, alpha);
		calc_in_z(&r3, &p3, alpha);
		calc_in_z(&r4, &p4, alpha);
		calc_in_z(&r5, &p5, alpha);
		calc_in_z(&r6, &p6, alpha);
		calc_in_z(&r7, &p7, alpha);
		calc_in_z(&r8, &p8, alpha);
	}

	if(z == 1) {
		calc_in_x(&t1, &r1, alpha);
		calc_in_x(&t2, &r2, alpha);
		calc_in_x(&t3, &r3, alpha);
		calc_in_x(&t4, &r4, alpha);
		calc_in_x(&t5, &r5, alpha);
		calc_in_x(&t6, &r6, alpha);
		calc_in_x(&t7, &r7, alpha);
		calc_in_x(&t8, &r8, alpha);
		
		calc_in_y(&r1, &p1, alpha);
		calc_in_y(&r2, &p2, alpha);
		calc_in_y(&r3, &p3, alpha);
		calc_in_y(&r4, &p4, alpha);
		calc_in_y(&r5, &p5, alpha);
		calc_in_y(&r6, &p6, alpha);
		calc_in_y(&r7, &p7, alpha);
		calc_in_y(&r8, &p8, alpha);
	}
	
 printCube();
 param += 1.5;
 alpha = (param * 3.14159) / 180.0;
 beta = (param *3.14159) / 180.0;
}

}


int main(void) {
	
	 d = TOFIX(150, Q4);
	
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



/*
hx8347a_set_foreground_color(rgb24_to_rgb16(COLOR_RED));
	hx8347a_draw_line(50,50, 50, 150);
	hx8347a_draw_line(50,150, 150,150);
	hx8347a_draw_line(150,150, 150,50);
	hx8347a_draw_line(150,50, 50,50);

	hx8347a_draw_line(150,50, persx(150, 100, 250), persy(50, 100, 250));
	hx8347a_draw_line(150,150, persx(150, 100, 250), persy(150, 100, 250));
	
	hx8347a_draw_line(persx(150, 100, 250), persy(50, 100, 250), persx(150, 100, 250), persy(150, 100, 250));
	
	
	hx8347a_draw_line(50,50, persx(50, 100, 250), persy(50, 100, 250));
	hx8347a_draw_line(50,150, persx(50, 100, 250), persy(150, 100, 250));

	hx8347a_draw_line(persx(50, 100, 250), persy(50, 100, 250), persx(50, 100, 250), persy(150, 100, 250));
	
	hx8347a_draw_line(persx(50, 100, 250), persy(50, 100, 250), persx(150, 100, 250), persy(50, 100, 250));
	hx8347a_draw_line(persx(50, 100, 250), persy(150, 100, 250), persx(150, 100, 250), persy(150, 100, 250));

*/


