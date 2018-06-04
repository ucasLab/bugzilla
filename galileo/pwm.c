/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Example usage: Generates PWM signal of period 200us with variable dutycyle
 *                repeately. Press Ctrl+C to exit
 */

/* standard headers */
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "CUnit/Basic.h"
/* mraa header */
#include "mraa/pwm.h"
#include "mraa/gpio.h"
/* PWM declaration */
#define PWM 3

/* PWM period in us */
#define PWM_FREQ 10000

#define BLUE_LED 06
#define RED_LED 04
#define GREEN_LED 07
mraa_gpio_context red, blue,green;
mraa_pwm_context pwm;
volatile sig_atomic_t flag = 1;
 mraa_result_t status = MRAA_SUCCESS;


int init_suite1(void)
{
    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();
 
    blue = mraa_gpio_init(BLUE_LED);
    if (blue == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", BLUE_LED);
        mraa_deinit();
        return EXIT_FAILURE;
    }
 
    /* set GPIO to output */
    status = mraa_gpio_dir(blue, MRAA_GPIO_IN);
    if (status != MRAA_SUCCESS) {
       return EXIT_FAILURE;
    }
  
    red = mraa_gpio_init(RED_LED);
    if (red == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", RED_LED);
        mraa_deinit();
        return EXIT_FAILURE;
    }
 
    /* set GPIO to output */
    status = mraa_gpio_dir(red, MRAA_GPIO_IN);
    if (status != MRAA_SUCCESS) {
       return EXIT_FAILURE;
    }

   green = mraa_gpio_init(GREEN_LED);
    if (green == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", GREEN_LED);
        mraa_deinit();
        return EXIT_FAILURE;
    }
 
     
    status = mraa_gpio_dir(green, MRAA_GPIO_IN);
    if (status != MRAA_SUCCESS) {
         return EXIT_FAILURE;
    }
   //! [Interesting]
    pwm = mraa_pwm_init(PWM);
    if (pwm == NULL) {
        fprintf(stderr, "Failed to initialize PWM\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* set PWM period */
    status = mraa_pwm_period_us(pwm, PWM_FREQ);
    if (status != MRAA_SUCCESS) {
        return EXIT_FAILURE;
    }

    /* enable PWM */
    status = mraa_pwm_enable(pwm, 1);
    if (status != MRAA_SUCCESS) {
        return EXIT_FAILURE;
    }
    return 0;
   
}
int clean_suite1(void)
{
  /* close PWM */
    mraa_pwm_close(pwm);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

   return 0;
}


void sig_handler(int signum)
{
    if (signum == SIGINT) {
        fprintf(stdout, "Exiting...\n");
        flag = 0;
    }
}


void testLowerTemp(){
    mraa_result_t status = MRAA_SUCCESS;
    status = mraa_pwm_write(pwm, 000.0f);
        
        if (status != MRAA_SUCCESS) {
            return EXIT_FAILURE;
        }
        usleep(1000000);
    int redL,blueL,greenL=0;
    redL =   mraa_gpio_read(red);
    blueL =   mraa_gpio_read(blue);
    greenL =   mraa_gpio_read(green);
    CU_ASSERT(redL == 1 );
    CU_ASSERT(blueL == 0 );
    CU_ASSERT(greenL == 0 );

}
void testHigerTemp(){
    mraa_result_t status = MRAA_SUCCESS;
    status = mraa_pwm_write(pwm, 100.0f);
        
        if (status != MRAA_SUCCESS) {
            return EXIT_FAILURE;
        }
        usleep(1000000);
    int redL,blueL,greenL=0;
    redL =   mraa_gpio_read(red);
    blueL =   mraa_gpio_read(blue);
    greenL =   mraa_gpio_read(green);
    CU_ASSERT(redL == 0 );
    CU_ASSERT(blueL == 1 );
    CU_ASSERT(greenL == 0 );
}

void testMidTemp(){
    mraa_result_t status = MRAA_SUCCESS;
    status = mraa_pwm_write(pwm, 0.50f);
        
        if (status != MRAA_SUCCESS) {
            return EXIT_FAILURE;
        }
        usleep(10000000);
   int redL,blueL,greenL=0;
    redL =   mraa_gpio_read(red);
    blueL =   mraa_gpio_read(blue);
    greenL =   mraa_gpio_read(green);
    CU_ASSERT(redL == 0 );
    CU_ASSERT(blueL == 0 );
    CU_ASSERT(greenL == 1 );
}
int main(void)
{
    CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test of heating", testLowerTemp))|| (NULL == CU_add_test(pSuite, "test of heating2", testMidTemp))|| (NULL == CU_add_test(pSuite, "test of heating3", testHigerTemp)) )
   {
      CU_cleanup_registry();
      goto err_exit;
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
     mraa_pwm_close(pwm);
   return CU_get_error();

  
    return EXIT_SUCCESS;

    err_exit:
    CU_cleanup_registry();
    mraa_result_print(status);

    /* close PWM */
    mraa_pwm_close(pwm);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return CU_get_error();
}
