#ifndef MPU9250_H
#define MPU9250_H

#include <hal.h>
#include <stdint.h>
#include "mpu9250_registers.h"

// mpu60X0_setup() config options
#define MPU9250_ACC_FULL_RANGE_2G           (0 << 0)
#define MPU9250_ACC_FULL_RANGE_4G           (1 << 0)
#define MPU9250_ACC_FULL_RANGE_8G           (2 << 0)
#define MPU9250_ACC_FULL_RANGE_16G          (3 << 0)
#define MPU9250_GYRO_FULL_RANGE_250DPS      (0 << 2)
#define MPU9250_GYRO_FULL_RANGE_500DPS      (1 << 2)
#define MPU9250_GYRO_FULL_RANGE_1000DPS     (2 << 2)
#define MPU9250_GYRO_FULL_RANGE_2000DPS     (3 << 2)
#define MPU9250_SAMPLE_RATE_DIV(x)          ((0xff & x) << 8) // sample rate is gyro Fs divided by x+1, x in [0, 255]
#define MPU60X0_LOW_PASS_FILTER_0           (0 << 16) // acc: BW=260Hz, delay=   0ms, Fs=1kHz gyro: BW=256Hz, delay=0.98ms, Fs=8kHz
#define MPU60X0_LOW_PASS_FILTER_1           (1 << 16) // acc: BW=184Hz, delay= 2.0ms, Fs=1kHz gyro: BW=188Hz, delay= 1.9ms, Fs=1kHz
#define MPU60X0_LOW_PASS_FILTER_2           (2 << 16) // acc: BW= 94Hz, delay= 3.0ms, Fs=1kHz gyro: BW= 98Hz, delay= 2.8ms, Fs=1kHz
#define MPU60X0_LOW_PASS_FILTER_3           (3 << 16) // acc: BW= 44Hz, delay= 4.9ms, Fs=1kHz gyro: BW= 42Hz, delay= 4.8ms, Fs=1kHz
#define MPU60X0_LOW_PASS_FILTER_4           (4 << 16) // acc: BW= 21Hz, delay= 8.5ms, Fs=1kHz gyro: BW= 20Hz, delay= 8.3ms, Fs=1kHz
#define MPU60X0_LOW_PASS_FILTER_5           (5 << 16) // acc: BW= 10Hz, delay=13.8ms, Fs=1kHz gyro: BW= 10Hz, delay=13.4ms, Fs=1kHz
#define MPU60X0_LOW_PASS_FILTER_6           (6 << 16) // acc: BW=  5Hz, delay=19.0ms, Fs=1kHz gyro: BW=  5Hz, delay=18.6ms, Fs=1kHz

int8_t mpu9250_setup(int config);
bool mpu9250_ping(void);
void mpu9250_read(float *gyro, float *acc, float *temp, int16_t *gyro_raw, int16_t *acc_raw);

#endif // MPU9250_H