#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "svm.h"
void exit_with_help_scale()
{
    printf(
        "Usage: svm-scale [options] data_filename\n"
        "options:\n"
        "-l lower : x scaling lower limit (default -1)\n"
        "-u upper : x scaling upper limit (default +1)\n"
        "-y y_lower y_upper : y scaling limits (default: no y scaling)\n"
        "-s save_filename : save scaling parameters to save_filename\n"
        "-r restore_filename : restore scaling parameters from restore_filename\n"
    );
    exit(1);
}

char *line = NULL;
int max_line_len = 1024;
float lower=-1.0,upper=1.0,y_lower,y_upper;
int y_scaling = 0;
float *feature_max;
float *feature_min;
float y_max = -DBL_MAX;
float y_min = DBL_MAX;

long int num_nonzeros = 0;
long int new_num_nonzeros = 0;

#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))

void output_target(float value);
int svm_scale_prob(int ilen, float* max, float* min)
{
    int i,index;
    int max_index;
    int min_index;
    
    // For svm package data, svm_node.index ranges 1~7
    min_index = 1;
    
    //To deal with scaling, ending svm_node is not considered.
    max_index = min_index +  PROB_X_FEATURE_NUMS;
   

    // Prepare the space to keep the max and min value of each feature
    // Cautions: There is an array-index and svm_node.index offset shift.
    feature_max = (float *)malloc((max_index)* sizeof(float));
    feature_min = (float *)malloc((max_index)* sizeof(float));

    if(feature_max == NULL || feature_min == NULL)
    {
        printf("can't allocate enough memory\n");
        exit(1);
    }

    for (i = min_index; i < max_index; i++)
    {
        // Since the feature is also known , this max/min can be computed in advance
        // To align with svm_node.index, feature_max[i]  starts from index "1", Not "0"
  	    // See function "output" for more detail
        feature_max[i] = *max++;
        feature_min[i] = *min++;
    }

    if (new_num_nonzeros > num_nonzeros)
        printf(
            "WARNING: original #nonzeros %ld\n"
            "       > new      #nonzeros %ld\n"
            "If feature values are non-negative and sparse, use -l 0 rather than the default -l -1\n",
            num_nonzeros, new_num_nonzeros);

    for(i=0; i<ilen*PROB_X_FEATURE_WITH_ENDING_NUMS; i++)
    {

        if((int)(x_space[i].index)<min_index)
        {
            printf("ending node:%d \n", x_space[i].index);
            continue;//Not need to scale the ending node
        }
        else
        {
            //printf("[** be scale output**]:%d, %f \n", x_space[i].index, x_space[i].value);
            x_space[i].value = output((int)(x_space[i].index), x_space[i].value);
            //printf("[**af scale output**]:%d, %f \n", x_space[i].index, x_space[i].value);

        }

    }

    free(feature_max);
    free(feature_min);
    return 0;
}


void output_target(float value)
{
    if(y_scaling)
    {
        if(value == y_min)
            value = y_lower;
        else if(value == y_max)
            value = y_upper;
        else value = y_lower + (y_upper-y_lower) *
                         (value - y_min)/(y_max-y_min);
    }
    printf("%.17g ",value);
}

float output(int index, float value)
{
    /* skip single-valued attribute */
    if(feature_max[index] == feature_min[index])
        return 0;

    if(value <= feature_min[index])
        value = lower;
    else if(value >= feature_max[index])
        value = upper;
    else
        value = lower + (upper-lower) *
                (value-feature_min[index])/
                (feature_max[index]-feature_min[index]);

    if(value != 0)
    {
        //printf("%d:%g ",index, value);
        new_num_nonzeros++;
        return value;
    }
    return 0;
}

