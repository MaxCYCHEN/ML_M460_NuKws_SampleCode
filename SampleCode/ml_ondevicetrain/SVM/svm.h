#ifndef _LIBSVM_H
#define _LIBSVM_H

#define LIBSVM_VERSION 331

#ifdef __cplusplus
extern "C" {
#endif
#define MY_TYPEDOUBLE float
#define PROB_X_FEATURE_NUMS     6
#define PROB_X_FEATURE_ENDING_NUM     1
#define PROB_X_FEATURE_WITH_ENDING_NUMS     ( PROB_X_FEATURE_NUMS + PROB_X_FEATURE_ENDING_NUM )
#define PROB_X_DATAIN_NUMS         32
#define PROB_X_FEATURE_INDEX_OFFSET    1
#define PROB_X_FEATURE_INDEX_MIN     1
#define PROB_X_FEATURE_INDEX_MAX    ( PROB_X_FEATURE_INDEX_MIN + PROB_X_FEATURE_NUMS )

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))


//Structure Definition
extern int libsvm_version;
//extern struct svm_model *model;
extern struct svm_model  gsmodel_train;
extern struct svm_problem prob;		// set by read_problem
extern struct svm_node *x_space;
extern struct svm_parameter param;		// set by parse_command_line

extern const MY_TYPEDOUBLE prob_xarr[270*13*2] ;
extern const MY_TYPEDOUBLE prob_yarr[270];

extern int ma_cnt;
extern int fr_cnt;

struct svm_node
{
    int index;
    MY_TYPEDOUBLE value;
};

struct svm_problem
{
    int l;
    MY_TYPEDOUBLE *y;
    struct svm_node **x;
};

enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; /* kernel_type */

struct svm_parameter
{
    int svm_type;
    int kernel_type;
    int degree;	/* for poly */
    MY_TYPEDOUBLE gamma;	/* for poly/rbf/sigmoid */
    MY_TYPEDOUBLE coef0;	/* for poly/sigmoid */

    /* these are for training only */
    MY_TYPEDOUBLE cache_size; /* in MB */
    MY_TYPEDOUBLE eps;	/* stopping criteria */
    MY_TYPEDOUBLE C;	/* for C_SVC, EPSILON_SVR and NU_SVR */
    int nr_weight;		/* for C_SVC */
    int *weight_label;	/* for C_SVC */
    MY_TYPEDOUBLE* weight;		/* for C_SVC */
    MY_TYPEDOUBLE nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
    MY_TYPEDOUBLE p;	/* for EPSILON_SVR */
    int shrinking;	/* use the shrinking heuristics */
    int probability; /* do probability estimates */
};

//
// svm_model
//
struct svm_model
{
    struct svm_parameter param;	/* parameter */
    int nr_class;		/* number of classes, = 2 in regression/one class svm */
    int l;			/* total #SV */
    struct svm_node **SV;		/* SVs (SV[l]) */
    MY_TYPEDOUBLE **sv_coef;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
    MY_TYPEDOUBLE *rho;		/* constants in decision functions (rho[k*(k-1)/2]) */
    MY_TYPEDOUBLE *probA;		/* pariwise probability information */
    MY_TYPEDOUBLE *probB;
    MY_TYPEDOUBLE *prob_density_marks;	/* probability information for ONE_CLASS */
    int *sv_indices;        /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

    /* for classification only */

    int *label;		/* label of each class (label[k]) */
    int *nSV;		/* number of SVs for each class (nSV[k]) */
    /* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
    /* XXX */
    int free_sv;		/* 1 if svm_model is created by svm_load_model*/
    /* 0 if svm_model is created by svm_train */
};

struct svm_model *svm_train(const struct svm_problem *prob, const struct svm_parameter *param);
void svm_cross_validation(const struct svm_problem *prob, const struct svm_parameter *param, int nr_fold, MY_TYPEDOUBLE *target);

//int svm_save_model(const char *model_file_name, const struct svm_model *model);
//struct svm_model *svm_load_model(const char *model_file_name);

int svm_get_svm_type(const struct svm_model *model);
int svm_get_nr_class(const struct svm_model *model);
void svm_get_labels(const struct svm_model *model, int *label);
void svm_get_sv_indices(const struct svm_model *model, int *sv_indices);
int svm_get_nr_sv(const struct svm_model *model);
MY_TYPEDOUBLE svm_get_svr_probability(const struct svm_model *model);
MY_TYPEDOUBLE svm_predict_values(const struct svm_model *model, const struct svm_node *x, MY_TYPEDOUBLE* dec_values);
MY_TYPEDOUBLE svm_predict(const struct svm_model *model, const struct svm_node *x);
MY_TYPEDOUBLE svm_predict_probability(const struct svm_model *model, const struct svm_node *x, MY_TYPEDOUBLE* prob_estimates);

void svm_free_model_content(struct svm_model *model_ptr);
void svm_free_and_destroy_model(struct svm_model **model_ptr_ptr);
void svm_destroy_param(struct svm_parameter *param);

const char *svm_check_parameter(const struct svm_problem *prob, const struct svm_parameter *param);
int svm_check_probability_model(const struct svm_model *model);
void svm_set_print_string_function(void (*print_func)(const char *));
void exit_input_error(int line_num);
void print_null(const char *s) ;
int train_feed_prob(struct svm_node* psvm_node, int ilen, float label);
int svm_train_init(struct svm_problem* psvm_prob, int ilen);
int svm_train_start(struct svm_problem* psvm_prob, int ilen);
int svm_scale_prob(int ilen, float* max, float* min);
float output(int index, float value);
//void svm_predict_single(float answer);
void svm_predict_single(float answer, MY_TYPEDOUBLE* sp_prob);
#ifdef __cplusplus
}
#endif

#endif /* _LIBSVM_H */
