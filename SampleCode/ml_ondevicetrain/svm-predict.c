#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "svm.h"

struct svm_node *x;


//extern
struct svm_model* model;

int predict_probability=1;// test probability

static char *line = NULL;


void exit_input_error(int line_num)
{
    printf("Wrong input format at line %d\n", line_num);
    //exit(1);
}

void svm_predict_single(float answer, MY_TYPEDOUBLE* sp_prob)
{
    int correct = 0;
    int total = 0;
    MY_TYPEDOUBLE error = 0;
    MY_TYPEDOUBLE sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

    int svm_type=svm_get_svm_type(&gsmodel_train);
    int nr_class=svm_get_nr_class(&gsmodel_train);
    MY_TYPEDOUBLE *prob_estimates=NULL;
    int j;

    if(predict_probability)
    {
        if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
            printf("Prob. model for test data: target value =\n");// predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",svm_get_svr_probability(model));
        else if(svm_type==ONE_CLASS)
        {
            nr_class = 2; //for ONE_CLASS
            prob_estimates = (MY_TYPEDOUBLE *) malloc(nr_class*sizeof(MY_TYPEDOUBLE));
            printf("label normal outlier\n");
        }
        else
        {
            int *labels=(int *) malloc(nr_class*sizeof(int));
            svm_get_labels(&gsmodel_train,labels);
            prob_estimates = (MY_TYPEDOUBLE *) malloc(nr_class*sizeof(MY_TYPEDOUBLE));
            printf("labels");
            for(j=0; j<nr_class; j++)
                printf(" %d",labels[j]);
            printf("\n");
            free(labels);
        }
    }

    int i = 0;
    MY_TYPEDOUBLE target_label, predict_label;
    struct svm_node* x = Malloc(struct svm_node, PROB_X_FEATURE_WITH_ENDING_NUMS); //1 samples(1*7 features, with ending)

    for(i=0; i< 1; i++)
    {
        target_label = answer;

        for (j = 0; j < PROB_X_FEATURE_WITH_ENDING_NUMS; j++)
        {
            x[j].index = x_space[j].index;
            x[j].value = x_space[j].value;
            printf("x_space[%d].index:%d\n", j, x[j].index);
        }


        if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC || svm_type==ONE_CLASS))
        {
            predict_label = svm_predict_probability(&gsmodel_train,x,prob_estimates);
            printf("%f",predict_label);
            for(j=0; j<nr_class; j++)
           {
					          printf(" %f",prob_estimates[j]);
						        sp_prob[j] =  prob_estimates[j];
					 }
           
            printf("\n");
        }
        else
        {
            predict_label = svm_predict(&gsmodel_train,x);
            printf("%f\n",predict_label);
        }

    }


    free(x);
    if(predict_probability)
        free(prob_estimates);
}

