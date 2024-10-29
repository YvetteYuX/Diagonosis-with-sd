/**
 * @file app_control.c
 * @brief This script is to control the application related codes.
 * @version 1.0
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
 */

/**
 * @name INCLUDES
 *
 */
#include "diagnosis.h"
#include "app_control.h"
#include "bsp_memory.h"
#include <stdio.h>
#include "app_control.h"
#include "diagnosis.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
/**
 * @name MACROS
 *
 */



/**
 * @name VARIABLES
 *
 */

SensorData sensordata = {
    .battery_voltage = NULL,
    .temperature = NULL,
    .acceleration = NULL,
    .gyroscope = NULL,
    .data_size = 0,
};

/**
 * @name FUNCTIONS
 * @brief Sudden damage detection function: I - Data process, II - Edge computation: WT + ICA + NN
 *
 */

// Read the CSV file from the SD card and fill the SensorData structure
int read_data_from_sd(const char *filename, SensorData *sensor_data) {
    FIL file;
    FRESULT res;
    char buffer[128];
    int data_index = 0;
    int initial_size = 100; // initial allocation size

//    // Allocate initial memory for each array in SensorData
//    sensor_data->battery_voltage = malloc(initial_size * sizeof(float));
//    sensor_data->temperature = malloc(initial_size * sizeof(float));
//    sensor_data->acceleration = malloc(initial_size * sizeof(float));
//    sensor_data->gyroscope = malloc(initial_size * sizeof(float));

    sensor_data->battery_voltage = (float *)memory_alloc(data_index * sizeof(float));
    sensor_data->temperature = (float *)memory_alloc(data_index * sizeof(float));
    sensor_data->acceleration = (float *)memory_alloc(data_index * sizeof(float));
    sensor_data->gyroscope = (float *)memory_alloc(data_index * sizeof(float));
	


    if (!sensor_data->battery_voltage || !sensor_data->temperature ||
        !sensor_data->acceleration || !sensor_data->gyroscope) {
        printf("Memory allocation failed\n");
        return -1;
    }
				
	  printf("Reading...\n\r");
		
    // Open the file
    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        printf("Failed to open file %s\n", filename);
        return -1;
    }

    // Read file content line by line
    while (f_gets(buffer, sizeof(buffer), &file)) {
        if (data_index == 0) {
            // Skip the first line (header)
            data_index++;
            continue;
        }

        // Parse the read line
        int index;
        float battery, temp, accel, gyro;
        if (sscanf(buffer, "%d,%f,%f,%f,%f", &index, &battery, &temp, &accel, &gyro) == 5) {
            // If needed, reallocate memory to expand the arrays
            if (data_index >= initial_size) {
                initial_size *= 2;
                float *new_battery = realloc(sensor_data->battery_voltage, initial_size * sizeof(float));
                float *new_temp = realloc(sensor_data->temperature, initial_size * sizeof(float));
                float *new_accel = realloc(sensor_data->acceleration, initial_size * sizeof(float));
                float *new_gyro = realloc(sensor_data->gyroscope, initial_size * sizeof(float));

                // Check each allocation
                if (!new_battery || !new_temp || !new_accel || !new_gyro) {
                    printf("Memory reallocation failed\n");
                    f_close(&file);

                    // Free previously allocated memory
                    free(sensor_data->battery_voltage);
                    free(sensor_data->temperature);
                    free(sensor_data->acceleration);
                    free(sensor_data->gyroscope);
                    return -1;
                }

                // Update pointers after successful reallocation
                sensor_data->battery_voltage = new_battery;
                sensor_data->temperature = new_temp;
                sensor_data->acceleration = new_accel;
                sensor_data->gyroscope = new_gyro;
            }

            // Store data in dynamically resized arrays
            sensor_data->battery_voltage[data_index - 1] = battery;
            sensor_data->temperature[data_index - 1] = temp;
            sensor_data->acceleration[data_index - 1] = accel;
            sensor_data->gyroscope[data_index - 1] = gyro;
            data_index++;
        }
    }

    // Close the file
    f_close(&file);

    // Set data size
    sensor_data->data_size = data_index - 1;

    return 0;
}



// Initialize SensorData and perform diagnosis
void classify_and_diagnose_from_sd() {
    SensorData sensor_data;
    const char *filename = "/data/Data1.csv";

    // Read data from the SD card and populate sensor_data
    if (read_data_from_sd(filename, &sensor_data) != 0) {
        printf("Failed to read data from SD card\n");
        return;
    }

    // Print data size
    printf("Data size: %d\n", sensor_data.data_size);

    // Call the diagnostic process
    SensorMetrics metrics = calculate_metrics(sensor_data);
		metrics.acceleration_size = sensor_data.data_size;
		metrics.battery_size = sensor_data.data_size;
		metrics.temperature_size= sensor_data.data_size;
		metrics.gyroscope_size= sensor_data.data_size;

    printf("Acceleration size: %d\n", metrics.acceleration_size);
		printf("Battery size: %d\n", metrics.battery_size);

    for (int i = 0; i < 6; i++) {
        int choice;
        printf("Please select the fault type:\n");
        printf("0: Normal\n");
        printf("1: Missing\n");
        printf("2: Minor\n");
        printf("3: Outlier\n");
        printf("4: Square\n");
        printf("5: Trend\n");
        printf("6: Drift\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Normal data, no diagnosis needed.\n");
            break;
        }
        diagnose(sensor_data, metrics, choice);
        HAL_Delay(1000);  // Delay 1 second
    }
}


//// Initializes SensorData and invokes select_and_diagnose
//void classify_and_diagnose() {
//    SensorData sensor_data = {
//        {3.5, 3.3, 3.5, 3.4, 3.5},   // battery_voltage
//        {20, 21, 22, 23, 26},        // temperature
//        {-0.3, 0.1, 0.3, 0.8, 1.1},  // acceleration
//        {200, 200, 200, 200, 200},   // gyroscope
//        "variable"                   // previous_data
//    };

//    SensorMetrics metrics = calculate_metrics(sensor_data);
//    printf("Acceleration size: %d\n", metrics.acceleration_size);

//    for (int i = 0; i < 6; i++) {
//        int choice;
//        printf("Please select the fault type:\n");
//        printf("0: Normal\n");
//        printf("1: Missing\n");
//        printf("2: Minor\n");
//        printf("3: Outlier\n");
//        printf("4: Square\n");
//        printf("5: Trend\n");
//        printf("6: Drift\n");
//        printf("Enter your choice: ");
//        scanf("%d", &choice);

//        if (choice == 0) {
//            printf("Normal data, there is no need to diagnose and recover.\n");
//            break;
//        }
//        diagnose(sensor_data, metrics, choice);
//        HAL_Delay(1000); // 1-second delay
//    }
//}
