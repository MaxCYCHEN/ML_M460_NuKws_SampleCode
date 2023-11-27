#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "svm.h"

void exit_with_help_train()
{
    printf(
        "Usage: svm-train [options] training_set_file [model_file]\n"
        "options:\n"
        "-s svm_type : set type of SVM (default 0)\n"
        "	0 -- C-SVC		(multi-class classification)\n"
        "	1 -- nu-SVC		(multi-class classification)\n"
        "	2 -- one-class SVM\n"
        "	3 -- epsilon-SVR	(regression)\n"
        "	4 -- nu-SVR		(regression)\n"
        "-t kernel_type : set type of kernel function (default 2)\n"
        "	0 -- linear: u'*v\n"
        "	1 -- polynomial: (gamma*u'*v + coef0)^degree\n"
        "	2 -- radial basis function: exp(-gamma*|u-v|^2)\n"
        "	3 -- sigmoid: tanh(gamma*u'*v + coef0)\n"
        "	4 -- precomputed kernel (kernel values in training_set_file)\n"
        "-d degree : set degree in kernel function (default 3)\n"
        "-g gamma : set gamma in kernel function (default 1/num_features)\n"
        "-r coef0 : set coef0 in kernel function (default 0)\n"
        "-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)\n"
        "-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)\n"
        "-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)\n"
        "-m cachesize : set cache memory size in MB (default 100)\n"
        "-e epsilon : set tolerance of termination criterion (default 0.001)\n"
        "-h shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)\n"
        "-b probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)\n"
        "-wi weight : set the parameter C of class i to weight*C, for C-SVC (default 1)\n"
        "-v n: n-fold cross validation mode\n"
        "-q : quiet mode (no outputs)\n"
    );
    //exit(1);
}



void do_cross_validation();
void set_prob_xy(struct svm_node* psvm_node, int idex, float label);
void set_param_default(void);

struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *gpmodel;
struct svm_model  gsmodel_train;       ///add to rplace keeping model info in file
struct svm_node *x_space;
int cross_validation;
int nr_fold;
int ma_cnt;
int fr_cnt;

static char *line = NULL;
static int max_line_len;

int svm_train_init(struct svm_problem* psvm_prob, int ilen)
{
    ma_cnt = 0;
    fr_cnt =0;

    //Replace a CLI-like interface to feed SVM param and desired model file name
    set_param_default();
    prob.l = PROB_X_DATAIN_NUMS;
    prob.y = Malloc(MY_TYPEDOUBLE, PROB_X_DATAIN_NUMS);
    prob.x = Malloc(struct svm_node*, 160);
    x_space = Malloc(struct svm_node, 160*6);

    return 0;
}


int svm_train_start(struct svm_problem* psvm_prob, int ilen)
{
    const char *error_msg;

    error_msg = svm_check_parameter(&prob,&param);

    if(error_msg)
    {
        printf("ERROR: %s\n",error_msg);
        //exit(1);
    }

    if(cross_validation)
    {
        do_cross_validation();
    }
    else
    {
        gpmodel = svm_train(&prob,&param);

        memcpy(&gsmodel_train, gpmodel, sizeof(struct svm_model));//Save model in global model structure

    }

    free(line);

    return 0;
}

void do_cross_validation()
{
    int i;
    int total_correct = 0;
    MY_TYPEDOUBLE total_error = 0;
    MY_TYPEDOUBLE sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
    MY_TYPEDOUBLE *target = Malloc(MY_TYPEDOUBLE,prob.l);

    svm_cross_validation(&prob,&param,nr_fold,target);
    if(param.svm_type == EPSILON_SVR ||
            param.svm_type == NU_SVR)
    {
        for(i=0; i<prob.l; i++)
        {
            MY_TYPEDOUBLE y = prob.y[i];
            MY_TYPEDOUBLE v = target[i];
            total_error += (v-y)*(v-y);
            sumv += v;
            sumy += y;
            sumvv += v*v;
            sumyy += y*y;
            sumvy += v*y;
        }
        printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
        printf("Cross Validation Squared correlation coefficient = %g\n",
               ((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
               ((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
              );
    }
    else
    {
        for(i=0; i<prob.l; i++)
            if(target[i] == prob.y[i])
                ++total_correct;
        printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/prob.l);
    }
    free(target);
}

void set_param_default(void)
{

    // default values
    param.svm_type = NU_SVC;
    param.kernel_type = RBF;
    param.degree = 2;
    param.gamma = 0;	// 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 4e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    cross_validation = 0;

    exit_with_help_train();
    svm_set_print_string_function(NULL);
}

int train_feed_prob(struct svm_node* psvm_node, int idex, float label)
{
    int max_index, inst_max_index, i, ii, j;

    max_index = PROB_X_FEATURE_NUMS;
    prob.l = PROB_X_DATAIN_NUMS;
    prob.y[idex] = label;

    //Pointer value update
    prob.x[idex] = &x_space[PROB_X_FEATURE_WITH_ENDING_NUMS*idex];
    
    //Update the content that the poiter points to
    for (i =0; i < PROB_X_FEATURE_WITH_ENDING_NUMS; i ++)
    {
        // Fill g-sensor 6 features into corresponding svm_node, and need one more ending svm_node with index "-1"
        // PROB_X_FEATURE_WITH_ENDING_NUMS = 6+1 =7

        x_space[PROB_X_FEATURE_WITH_ENDING_NUMS*idex+i].index = psvm_node[i].index;
        x_space[PROB_X_FEATURE_WITH_ENDING_NUMS*idex+i].value =  psvm_node[i].value;

    }

    //Check Feed datainput
    //if (prob.x[idex][0].index != 0)	printf("Wrong input format: first column must be 0:sample_serial_number\n");

    if(param.gamma == 0 && max_index > 0)param.gamma = 1.0/max_index;

    return PROB_X_FEATURE_WITH_ENDING_NUMS*idex;
}
