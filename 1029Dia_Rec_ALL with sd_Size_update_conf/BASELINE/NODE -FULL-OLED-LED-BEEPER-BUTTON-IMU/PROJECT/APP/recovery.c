/**
 * @file recovery.c
 * @brief This script is to recover the trend, outlier and bias.
 * @version 1.0
 * @date 2024-10-18
 * @copyright Copyright (c) 2024
 */

/**
 * @name INCLUDES
 *
 */

#include "diagnosis.h"
#include "app_control.h"

/**
 * @name MACROS
 *
 */


/**
 * @name VARIABLES
 *
 */



/**
 * @name FUNCTIONS
 * @brief Sudden damage detection function: I - Data process, II - Edge computation: WT + ICA + NN
 *
 */
 

// Function to calculate the moving average
void moving_average_filter(float* data, int size, int window_size, float* filtered_data) {
	
    for (int i = 0; i < size; i++) {
        int start = (i - window_size / 2 < 0) ? 0 : i - window_size / 2;
        int end = (i + window_size / 2 >= size) ? size - 1 : i + window_size / 2;
        
        float sum = 0.0;
        int count = 0;
        for (int j = start; j <= end; j++) {
            sum += data[j];
            count++;
        }
        filtered_data[i] = sum / count; // calculate the average
    }
}

// Function for trend recovery using moving average window filter
void trend_recovery(float* data, int size, int window_size) {
    float filtered_data[size];
    float original_mean = 0.0;

    // Calculate the moving average
    moving_average_filter(data, size, window_size, filtered_data);

    // Calculate the mean of the original data
    for (int i = 0; i < size; i++) {
        original_mean += data[i];
    }
    original_mean /= size; // overall mean

    // Perform trend recovery
    for (int i = 0; i < size; i++) {
        data[i] = data[i] - filtered_data[i] + original_mean; // trend recovery formula
    }
}
// Function to detect and recover from outliers
void outlier_recovery(float* data, int size, int window_size) {
    float filtered_data[size];
    moving_average_filter(data, size, window_size, filtered_data);

    // Step 1: Calculate the overall mean and standard deviation of the filtered data
    float sum = 0.0, sum_sq = 0.0, mean = 0.0, std_dev = 0.0;
    
    for (int i = 0; i < size; i++) {
        sum += filtered_data[i];
        sum_sq += filtered_data[i] * filtered_data[i];
    }
    mean = sum / size;
    std_dev = sqrt((sum_sq / size) - (mean * mean));

    // Step 2: Set the threshold (four times the standard deviation)
    float threshold = 4 * std_dev;

    // Step 3: Detect outliers by checking differences between consecutive window means
    for (int i = 1; i < size; i++) {
        if (fabs(filtered_data[i] - filtered_data[i - 1]) > threshold) {
            // Outlier detected, perform outlier recovery on this window
            printf("Outlier detected at index %d, performing recovery...\n", i);

            // Step 4: Replace values larger than (mean + 3 * std_dev)
            for (int j = i; j < size && fabs(data[j] - mean) > 3 * std_dev; j++) {
                if (data[j] > mean + 3 * std_dev) {
                    data[j] = mean + 3 * std_dev; // replace with corrected value
                }
            }
        }
    }
}


//void outlier_recovery(float* data, int size) {
//    // Step 1: Calculate the mean and standard deviation of the acceleration data
//    float sum = 0.0, sum_sq = 0.0, mean = 0.0, std_dev = 0.0;
//    
//    for (int i = 0; i < size; i++) {
//        sum += data[i];
//        sum_sq += data[i] * data[i];
//    }
//    mean = sum / size;
//    std_dev = sqrt((sum_sq / size) - (mean * mean));

//    // Step 2: Set threshold (four times the standard deviation)
//    float upper_bound = 4 * std_dev;

//    // Step 3: Detect the segment with spike fault
//    for (int i = 0; i < size; i++) {
//        if (fabs(data[i] - mean) > upper_bound) {
//            // Outlier detected, perform outlier recovery on this segment
//            printf("Outlier detected at index %d, performing recovery...\n", i);
//            
//            // Step 4: Replace values larger than three standard deviations + mean
//            for (int j = i; j < size && fabs(data[j] - mean) > (4 * std_dev); j++) {
//                if (fabs(data[j] - mean) > (4 * std_dev)) {
//                    data[j] = mean + 3 * std_dev ; // replace with mean
//                }
//            }
//        }
//    }
//}

void correct_bias(float* acceleration, int acceleration_size, float* smoothed_accel, 
	int bias_index, int max_derivative_index, int window_size) {
      // This calculates the bias index

    float mean_before_bias = 0.0;
    int count_before = 0;

    // Step 1: Calculate mean of data before the bias
    for (int i = 0; i < bias_index; i++) {
        mean_before_bias += acceleration[i];
        count_before++;
    }
    if (count_before > 0) {
        mean_before_bias /= count_before;
    }

    // Step 2: Bias correction by shifting post-bias data to align with pre-bias mean
    for (int i = bias_index; i < acceleration_size; i++) {
        acceleration[i] -= (smoothed_accel[max_derivative_index] - mean_before_bias);
    }
}



