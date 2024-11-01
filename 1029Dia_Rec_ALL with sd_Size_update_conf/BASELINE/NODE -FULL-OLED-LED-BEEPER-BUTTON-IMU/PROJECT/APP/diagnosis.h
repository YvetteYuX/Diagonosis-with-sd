/**
 * @file diagnosis.h
 * @brief This script is to perform wavelet transform on the generated signal.
 * @version 1.0
 * @date 2024-08-12
 * @copyright Copyright (c) 2024
 */

#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H
//typedef struct SensorData SensorData;
/**
 * @name INCLUDES
 *
 */
#include "app_control.h"
#include <stdbool.h> 




/**
 * @name MACROS
 *
 */


/**
 * @name VARIABLES
 *
 */


// define sensor data struct
//typedef struct {
//    float battery_voltage[5];
//    float temperature[5];
//    float acceleration[5];
//	  float gyroscope[5];
//    char previous_data[10];
////	  bool is_active;
//} SensorData;

// Define calculated metrics struct
typedef struct {
    float max_voltage;
    float min_voltage;
    float avg_voltage;
    float max_temp;
    float min_temp;
    float avg_temp;
    float max_accel;
    float min_accel;
    float avg_accel;
    float resolution;
	  float resolution_noise;
    float range; // Range total
    int battery_size;
    int temperature_size;
    int acceleration_size;
	  int gyroscope_size;
	  int acceleration_index;
} SensorMetrics;

//// define trend state
//typedef enum {
//    NO_TREND,
//    TEMPERATURE_TREND,
//    ACTIVATION_TREND
//} TrendState;

//TrendState current_trend_state = NO_TREND;

//typedef struct {
//    float battery_voltage[5];
//    float temperature[5];
//    float acceleration[5];
//    char previous_data[10];
//} SensorData;

/**
 * @name FUNCTIONS
 *
 */

//SensorMetrics calculate_metrics(SensorData sensor_data) {
//    SensorMetrics metrics = {0};
SensorMetrics calculate_metrics(SensorData sensor_data);
char* check_drift(SensorData sensor_data, SensorMetrics metrics);
char* check_trend(SensorData sensor_data, SensorMetrics metrics);
char* check_minor(SensorData sensor_data, SensorMetrics metrics);
char* check_square(SensorData sensor_data, SensorMetrics metrics);
char* check_bias(SensorData sensor_data, SensorMetrics metrics);
char* check_sudden_rotation(SensorData sensor_data, SensorMetrics metrics);
char* check_outlier(SensorData sensor_data, SensorMetrics metrics);
char* check_missing(SensorData sensor_data, SensorMetrics metrics);


void diagnose(SensorData sensor_data, SensorMetrics metrics, int choice);

	
//char* check_drift(SensorData sensor_data);
//char* check_trend(SensorData sensor_data);
//char* check_minor(SensorData sensor_data);
//char* check_square(SensorData sensor_data);
//char* check_bias(SensorData sensor_data);
//char* check_sudden_rotation(SensorData sensor_data);
//char* check_outlier(SensorData sensor_data);
//char* check_missing(SensorData sensor_data);
//void diagnose(SensorData sensor_data, int choice);

// void test(void);
// float max_value(float *array, int len);
// float min_value(float *array, int len);
// float mean_value(float *array, int len);

// void check_drift(SensorData *data, char *result);
// void check_trend(SensorData *data, char *result);
// void check_minor(SensorData *data, char *result);
// void check_square(SensorData *data, char *result);
// void check_bias(SensorData *data, char *result);
// void check_sudden_rotation(SensorData *data, char *result);
// void check_spike(SensorData *data, char *result);
// void check_missing(SensorData *data, char *result);
// void diagnose(SensorData *data);
// void check_noise();


























#endif /* DIAGNOSIS_H */