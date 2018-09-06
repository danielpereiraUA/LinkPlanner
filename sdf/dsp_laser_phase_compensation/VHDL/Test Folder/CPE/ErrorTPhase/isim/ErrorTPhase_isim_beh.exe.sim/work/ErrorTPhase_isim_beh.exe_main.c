/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

#include "xsi.h"

struct XSI_INFO xsi_info;

char *IEEE_P_2592010699;
char *IEEE_P_0774719531;
char *IEEE_P_1242562249;
char *WORK_P_0335708415;
char *STD_TEXTIO;
char *IEEE_P_3499444699;
char *IEEE_P_3620187407;
char *STD_STANDARD;
char *IEEE_P_3972351953;


int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    ieee_p_2592010699_init();
    ieee_p_3499444699_init();
    ieee_p_3620187407_init();
    ieee_p_1242562249_init();
    ieee_p_3972351953_init();
    ieee_p_0774719531_init();
    std_textio_init();
    work_a_0706909500_3212880686_init();
    work_a_2234717365_3212880686_init();
    work_a_2351964920_3212880686_init();
    work_a_2364045760_3212880686_init();
    work_a_2002755820_3212880686_init();
    work_a_3286651523_3212880686_init();
    work_a_1745227551_3212880686_init();
    work_a_4244046128_3708392848_init();
    work_p_0335708415_init();
    work_a_0038700431_3708392848_init();
    work_a_4151722702_3708392848_init();
    work_a_3126879665_3708392848_init();
    work_a_4057165897_3212880686_init();
    work_a_0846138825_2372691052_init();
    work_a_4062007603_3212880686_init();
    work_a_4241114093_3212880686_init();
    work_a_3066306361_3212880686_init();
    work_a_2295724934_3212880686_init();
    work_a_1625662130_3212880686_init();
    work_a_3235046672_3708392848_init();
    work_a_0678925455_3708392848_init();
    work_a_0336031864_3212880686_init();
    work_a_3347761410_3212880686_init();
    work_a_3143311068_3708392848_init();


    xsi_register_tops("work_a_3143311068_3708392848");

    IEEE_P_2592010699 = xsi_get_engine_memory("ieee_p_2592010699");
    xsi_register_ieee_std_logic_1164(IEEE_P_2592010699);
    IEEE_P_0774719531 = xsi_get_engine_memory("ieee_p_0774719531");
    IEEE_P_1242562249 = xsi_get_engine_memory("ieee_p_1242562249");
    WORK_P_0335708415 = xsi_get_engine_memory("work_p_0335708415");
    STD_TEXTIO = xsi_get_engine_memory("std_textio");
    IEEE_P_3499444699 = xsi_get_engine_memory("ieee_p_3499444699");
    IEEE_P_3620187407 = xsi_get_engine_memory("ieee_p_3620187407");
    STD_STANDARD = xsi_get_engine_memory("std_standard");
    IEEE_P_3972351953 = xsi_get_engine_memory("ieee_p_3972351953");

    return xsi_run_simulation(argc, argv);

}