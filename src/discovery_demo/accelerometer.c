#include "ch.h"
#include "hal.h"

#include "lis302dl.h"

#include "discovery_demo/accelerometer.h"

accelerometer_sample_t acc_sample;
accelerometer_callback acc_callback;


static THD_WORKING_AREA(waAcceleroThd, 128);
static THD_FUNCTION(AcceleroThd, arg) {
    static int32_t xbuf[4], ybuf[4], zbuf[4];
    systime_t time;

    (void)arg;
    chRegSetThreadName("Accelerometer");

    /* Reader thread loop.*/
    while (TRUE) {
        unsigned i;

        time = chVTGetSystemTime();

        /* Keeping an history of the latest four accelerometer readings.*/
        for (i = 3; i > 0; i--) {
            xbuf[i] = xbuf[i - 1];
            ybuf[i] = ybuf[i - 1];
            zbuf[i] = zbuf[i - 1];
        }

        /* Reading MEMS accelerometer X, Y and Z registers.*/
        xbuf[0] = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTX);
        ybuf[0] = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTY);
        zbuf[0] = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTZ);

        /* Calculating average of the latest four accelerometer readings.*/
        acc_sample.acceleration[0] = (xbuf[0] + xbuf[1] + xbuf[2] + xbuf[3]) / 4;
        acc_sample.acceleration[1] = (ybuf[0] + ybuf[1] + ybuf[2] + ybuf[3]) / 4;
        acc_sample.acceleration[2] = (zbuf[0] + zbuf[1] + zbuf[2] + zbuf[3]) / 4;

        acc_callback();

        /* Waiting until the next 100 milliseconds time interval.*/
        chThdSleepUntilWindowed(time, time + MS2ST(100));
    }
}

void demo_acc_start(accelerometer_callback callback)
{
    static const SPIConfig spi1cfg = {
        NULL,
        /* HW dependent part.*/
        GPIOE,
        GPIOE_CS_SPI,
        SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };
    spiStart(&SPID1, &spi1cfg);
    acc_callback = callback;

    chThdSleepMilliseconds(500);

    /* LIS302DL initialization.*/
    lis302dlWriteRegister(&SPID1, LIS302DL_CTRL_REG1, 0x43);
    lis302dlWriteRegister(&SPID1, LIS302DL_CTRL_REG2, 0x00);
    lis302dlWriteRegister(&SPID1, LIS302DL_CTRL_REG3, 0x00);

    chThdCreateStatic(waAcceleroThd,
                      sizeof(waAcceleroThd),
                      NORMALPRIO + 10,
                      AcceleroThd,
                      NULL);
}

void demo_acc_get_acc(float *acc)
{
    chSysLock();
    acc[0] = acc_sample.acceleration[0];
    acc[1] = acc_sample.acceleration[1];
    acc[2] = acc_sample.acceleration[2];
    chSysUnlock();
}
