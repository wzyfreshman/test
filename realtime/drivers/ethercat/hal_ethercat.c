#include "rtapi.h"		    /* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "hal.h"		    /* HAL public API decls */

#include<linux/errno.h>
#include<linux/string.h>
#include<linux/kernel.h>
#include<linux/major.h>
#include<linux/fs.h>
#include<linux/nfs_fs.h>
#include<linux/nfs_mount.h>
#include <bits/time.h>
#include <math.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#include <stdio.h>
#include <sys/mman.h>
#include "pdo_def.h"


#define MAX_BUS 3	/* max number of parports (EPP busses) */
#define RTDM_PATH "/proc/EH_Master"

#define	EPSILON		1e-20

#define MAXLINELEN 1024
#define MAXPARLINE  500
#define MAX_EXTERNIO 2

static int ethercatVersion = 2;

/* module information */
MODULE_AUTHOR("CNEXT");
MODULE_DESCRIPTION("HAL driver for EtherCAT Controller");
MODULE_LICENSE("GPL");

//#define USING_CIA402_HOME
#define NOT_USING_CIA402_HOME

#define _(s) (s)
/***********************************************************/
/***********************************************************/
// EtherCAT
static int errorConnect = 0;
static int reConnectcount = 0;
static int recordrstflag = 0;

COM_STATUS_t *g_com_status;

static int MASTER_TRY_COUNT = 5;
RTAPI_MP_INT(MASTER_TRY_COUNT, "MASTER_TRY_COUNT");


static int IO_DOUTPUT_NUM = 32;
RTAPI_MP_INT(IO_DOUTPUT_NUM, "IO_DOUTPUT_NUM");

static int IO_DINPUT_NUM = 32;
RTAPI_MP_INT(IO_DINPUT_NUM, "IO_DINPUT_NUM");

static int config_io_num = 1;
RTAPI_MP_INT(config_io_num, "config_io_num");

static int config_axis_num = 1;
RTAPI_MP_INT(config_axis_num, "config_axis_num");


static char *config_file_path = NULL;
RTAPI_MP_STRING(config_file_path,"config_file_path");

static int motor_chose = 0;
RTAPI_MP_INT(motor_chose,"motor_chose");

static int board_type = 0;
RTAPI_MP_INT(board_type,"board_type");


static int io_type = 0;
RTAPI_MP_INT(io_type,"io_type");


static int link_type = 0;
RTAPI_MP_INT(link_type,"link_type");

static int io_board_num = 0;
RTAPI_MP_INT(io_board_num, "io_board_num");


static char *config_axis_channel = NULL;
RTAPI_MP_STRING(config_axis_channel,"config_axis_channel");


#define SLAVE1  0, 0
#define TAIDA 0x1DD, 0x10305070


// process data
static uint8_t *domain_pd = NULL;

static unsigned int sync_ref_counter = 0;

static int errortimecount = 1500;


int g_rtdm_fd = 0;

#define FREQUENCY 500
#define CLOCK_TO_USE CLOCK_REALTIME
#define NSEC_PER_SEC (1000000000L)
#define PERIOD_NS (NSEC_PER_SEC / FREQUENCY)
#define DIFF_NS(A, B) (((B).tv_sec - (A).tv_sec) * NSEC_PER_SEC + \
    (B).tv_nsec - (A).tv_nsec)
#define TIMESPEC2NS(T) ((uint64_t) (T).tv_sec * NSEC_PER_SEC + (T).tv_nsec)

static void exchange_data(void *args,long peri);
static void getMotorData();
#define ETH_MAX_SLAVES 50
#define IO_SLAVES_MAXNUM 20


#define IO_DOUTPUT_NUM_MAX  1024
#define IO_DINPUT_NUM_MAX 1024



typedef struct version_struct {
    char errorbuf[1024*10];
    int ethercatVersion;
    int IOboardVersion;
    int segVersion;
} version_struct;
struct version_struct *versiondata;

static void handleError(char *errmsg){
    int i = 9;
    for(i = 9;i > 0;i--) {
        memcpy(&versiondata->errorbuf[i*1024],&versiondata->errorbuf[(i-1)*1024],sizeof(char)*1024);
    }
    memcpy(&versiondata->errorbuf[0],errmsg,sizeof(char)*1024);
//    char tmp[1024];
//    for(i = 0;i <10;i++) {
//        memcpy(&tmp[0],&versiondata->errorbuf[i*1024],sizeof(char)*1024);
//        printf("now I insert %d:%s\n",i,tmp);
//    }
}


void initEthercat();


typedef struct {
    hal_bit_t *ready;        //ethercat就绪标志
    hal_u32_t *handleVal;
    hal_u32_t *slaveNum; //子站个数

    hal_u32_t *maxtime;
    hal_u32_t *realruntime;


    hal_u32_t *testtime[10];

    hal_bit_t *read_data;
    hal_bit_t *write_data;


    hal_bit_t *reset_data;

    hal_float_t *DAVol[IO_SLAVES_MAXNUM];
    hal_float_t DAScale[IO_SLAVES_MAXNUM];

    hal_u32_t *plane_in0;
    hal_u32_t *plane_in1;

} master_pins;
static master_pins *masterpins;

typedef struct {

    hal_bit_t *enable;		/* enable pin for step generator */
    hal_float_t *vel;		/* velocity command pin (mm) */
    hal_float_t scale;		/* parameter: scaling for vel to Hz (pulse/mm) */
    hal_float_t scaledir;          /* parameter: scale dir (pulse/mm) */

    hal_bit_t *start_probe;
    hal_bit_t *set_zero;
    hal_bit_t *index_enable;


    hal_bit_t *in[IO_DINPUT_NUM_MAX];
    hal_float_t infliter[IO_DINPUT_NUM_MAX];
} slave_input_t;

typedef struct {

    hal_bit_t enable;		  //保存使能命令，取上升沿
    hal_bit_t start_probe;
    hal_bit_t set_zero;
    hal_bit_t index_enable;

    hal_float_t vel;
} last_slave_input_t;


typedef struct {

    hal_bit_t *alarm;
    hal_bit_t *linkstate;
    hal_bit_t *enabled;
    hal_bit_t *probe_done;

    hal_float_t *fbposition;
    hal_u32_t *encoderpos;
    hal_float_t encoderscale;
    hal_float_t scale;


    hal_u32_t *error_flag;
    hal_u32_t *al_state;
    hal_u32_t *errorcode;

    hal_bit_t *out[IO_DOUTPUT_NUM_MAX];

    hal_u32_t *mt_200b_0d;

} slave_output_t;

static hal_s32_t **thread_lat;

typedef struct {
    hal_u32_t data_6040;    //伺服控制字
    hal_s32_t data_607A;    //目标位置
    hal_s32_t data_60B0;    //位置偏置
    hal_u32_t data_6060;    //操作模式
    hal_u32_t data_2061;        //IO输出
    hal_u32_t data_probe_ctl;

} paremeter_output_t;

typedef struct {
    hal_u32_t data_6041;                //伺服状态字
    hal_s32_t data_6064;            //位置反馈
    hal_u32_t data_603F;            //错误码
    hal_u32_t data_6061;            //模式显示
    hal_u32_t data_60FD;        //数字输入
    hal_u32_t data_203F;        //数字输入
    hal_u32_t data_2060;        //IO输入
    hal_u32_t data_2062;        //编码器读取值
//    hal_u32_t data_200b;        //平均负载率
    hal_u32_t data_probe_status;
    hal_s32_t data_probe_pos;
    hal_float_t flitercount[IO_DINPUT_NUM_MAX];

} paremeter_input_t;

typedef struct {

    hal_bit_t data_probe_status;

} last_paremeter_input_t;


typedef struct {
    hal_u32_t data_7010;
    hal_u32_t data_7010_1;
    hal_u32_t data_7010_2;
    hal_u32_t data_7010_3;
    hal_u32_t data_7010_4;

    hal_u32_t data_7014[8];
    hal_u32_t axis_type[8];
    hal_u32_t output[IO_DINPUT_NUM_MAX];		//origin
} io_paremeter_output_t;

typedef struct {
    hal_u32_t data_6000;
    hal_u32_t data_6000_1;
    hal_u32_t data_6000_2;
    hal_u32_t data_6000_3;
    hal_u32_t data_6000_4;
    hal_u32_t data_6003[8];
    hal_float_t flitercount[IO_DINPUT_NUM_MAX];
    hal_u32_t intput[IO_DINPUT_NUM_MAX];		//origin
} io_paremeter_input_t;

typedef struct {

    hal_bit_t *out[IO_DOUTPUT_NUM_MAX];

    hal_bit_t *svrclr[8];          //clr管脚
    hal_bit_t *svron[8];        //enable官脚
    hal_bit_t *indexEnable[8];        //indexEnable 信号
    hal_float_t *pulse[8];		/* 接收到的命令值 */
    hal_float_t *recordpulse[8];		/* 接收到的命令值 */
    hal_float_t pulsescale[8];		/* 接收命令值的scale*/
    hal_float_t fbpulsescale[8];		/* 接收命令值的scale*/
    hal_float_t fbscaledir[8];		/* 反馈命令值的scale方向*/
    hal_float_t posscaledir[8];		/* 命令值的scale方向*/
    hal_bit_t *notmove[8];        //index信号后不再移动状态
    hal_u32_t *delta[8];         //计算出来的脉冲值
    hal_u32_t *totaldelta[8];         //累计的脉冲值
    hal_float_t *speed[8];		/* 接收到的命令值 */
    hal_u32_t *move_mode[8];


} io_slave_input_t;

typedef struct {

    hal_bit_t *in[IO_DINPUT_NUM_MAX];
    hal_float_t infliter[IO_DINPUT_NUM_MAX];

    hal_bit_t *svralm[8];          //驱动器报警信号
    hal_bit_t *svrindex[8];        //index信号

    hal_float_t *fbpulse[8];		/* 接收到的反馈值 */
    hal_float_t *fbspeed[8];		/* 接收到的命令值 */
    hal_float_t *record_fbpulse[8]; //记录的反馈值

    hal_u32_t *fpgaVer;         //FPGA传上来的版本
    hal_u32_t *mcuVer;         //FPGA传上来的版本
    hal_u32_t *checkBoard;         //FPGA传上来的版本


} io_slave_output_t;


typedef struct ec_sdos_config {

    uint16_t index;
    uint8_t subindex;
    uint8_t bit_length;
    uint32_t value;

} ec_sdos_config_t;



typedef struct slave_config
{

    int valid;
    int alias;
    int position;
    int vendor_id;
    int product_code;
    int activate_word;
    int shifttime;
    //ec_sync_info_t *sync_info;
    ec_sdos_config_t *sdos_config;
    //ec_pdo_info_t * pdo_info;
    int pdos_config_size;
    int sdos_config_size;

    unsigned int off_data_6040;
    unsigned int off_data_607A;
    unsigned int off_data_60B0;
    unsigned int off_data_6060;
    unsigned int off_data_probe_status;
    unsigned int off_data_probe_ctl;
    unsigned int off_data_probe_pos;
    unsigned int off_data_6041;
    unsigned int off_data_6064;
    unsigned int off_data_6061;
    unsigned int off_data_603F;
    unsigned int off_data_60FD;
    unsigned int off_data_203F;
    unsigned int off_data_2061;
    unsigned int off_data_2060;
    unsigned int off_data_2062;
//    unsigned int off_data_200b;

    unsigned int off_data_606C;
    unsigned int off_data_6077;
    unsigned int off_data_60FF;
    unsigned int off_data_6071;

    paremeter_output_t *par_output;
    paremeter_input_t *par_input;
    slave_input_t *slave_input;
    slave_output_t *slave_output;
    //ec_slave_info_t *slaveinfo; //add by hong 20170512 to read the slave state


    last_slave_input_t last_slave_input;
    last_paremeter_input_t last_paremeter_input;

    int last_alarm;
    int serv_on_process_flag;
    int serv_on_process_count;

    int probe_process_flag;
    int probe_process_count;
    int isInited;           //是否已经初始化标志位

} slave_config_t;

static slave_config_t *motion_slaves;

/******************************************************/
/*********************IO Board**************************/

typedef struct IO_slave_config
{
    int valid;
    int alias;
    int position;
    int vendor_id;
    int product_code;
    int activate_word;

    double recordpos[8]; //记录上一周期的命令值，算脉冲用的
    int indexdelay[8];

    io_paremeter_output_t *par_output;
    io_paremeter_input_t *par_input;
    io_slave_input_t *slave_input;
    io_slave_output_t *slave_output;

    unsigned int off_data_6000; //io_input
    unsigned int off_data_6001; //io_input
    unsigned int off_data_6002; //io_input
    unsigned int off_data_6000_1; //handle
    unsigned int off_data_6000_2; //handle
    unsigned int off_data_6000_3; //handle
    unsigned int off_data_6000_4; //handle
    unsigned int off_data_6000_5; //handle
    unsigned int off_data_6000_6; //handle
    unsigned int off_data_6000_7; //handle
    unsigned int off_data_6000_8; //handle
    unsigned int off_data_7010;//io_output
    unsigned int off_data_7011;//io_output
    unsigned int off_data_7012;//io_output
    unsigned int off_data_7013;//io_output
    unsigned int off_data_7010_1;//io_output
    unsigned int off_data_7010_2;//io_output
    unsigned int off_data_7010_3;//io_output
    unsigned int off_data_7010_4;//io_output
    unsigned int off_data_7010_5;//io_output
    unsigned int off_data_7010_6;//io_output
    unsigned int off_data_7010_7;//io_output
    unsigned int off_data_7010_8;//io_output


} io_slave_config_t;

static io_slave_config_t *io_slaves;


static int comp_id;		/* component ID */

static void initBusState() {

}

/**
src 源字符串的首地址(buf的地址)
separator 指定的分割字符
dest 接收子字符串的数组
num 分割后子字符串的个数
*/
void split(char *src, const char *separator, char **dest, int *num) {
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止
        return;
    if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止
        return;
    pNext = (char *)strtok(src, separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
    while (pNext != NULL) {
        *dest++ = pNext;
        ++count;
        pNext = (char *)strtok(NULL, separator);  //必须使用(char *)进行强制类型转换
    }
    *num = count;
}


static void initSlaveConfigStruct() {

    slave_config_t *t;
    int i;

    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
        t = &motion_slaves[i];
        t->valid = 0;

        t->last_slave_input.set_zero = 0;
        t->last_slave_input.start_probe = 0;
        t->last_paremeter_input.data_probe_status = 0;

        t->probe_process_count = 0;
        t->probe_process_flag = 0;


        t->last_slave_input.enable = 0;
        t->last_slave_input.vel = 0;
        t->serv_on_process_flag = 0;
        t->serv_on_process_count = 0;


        t->off_data_6040= 0;
        t->off_data_607A = 0;
        t->off_data_60B0 = 0;
   //     t->off_data_6060 = 0;
        t->off_data_6041 = 0;
        t->off_data_6064 = 0;
     //   t->off_data_6061 = 0;
        t->off_data_603F = 0;
        t->off_data_60FD = 0;
        t->off_data_203F = 0;
        t->off_data_2060 = 0;
        t->off_data_2061 = 0;
        t->off_data_2062 = 0;
//        t->off_data_200b = 0;
        t->off_data_probe_status = 0;
        t->off_data_probe_ctl = 0;
        t->sdos_config = NULL;
        t->sdos_config_size = 0;

        //t->pdo_info = NULL;
        t->pdos_config_size = 0;
        t->isInited = 0;


    }


}


static hal_bit_t **try_connect_ethercat;

static int exportAllPins() {

    int i,retval,n;
    char buf[HAL_NAME_LEN+2];

    thread_lat = hal_malloc( sizeof(hal_s32_t*) * MAX_THREAD_LATENCY_NUM );

    for( i = 0; i < MAX_THREAD_LATENCY_NUM; i++ ) {
        rtapi_snprintf(buf, HAL_NAME_LEN, "system.%02d.thread_latency",i);
        retval = hal_pin_s32_new(buf, HAL_OUT, &(thread_lat[i]), comp_id);
        if (retval != 0) {
            return -1;
        }
    }

    try_connect_ethercat  = hal_malloc( sizeof(hal_bit_t*) );
    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.try_connect");
    retval = hal_pin_bit_new(buf, HAL_IN, &(*try_connect_ethercat), comp_id);
    if (retval != 0) {
        return -1;
    }


    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        motion_slaves[i].slave_input = hal_malloc(sizeof(slave_input_t));
        motion_slaves[i].slave_output = hal_malloc(sizeof(slave_output_t));
        //motion_slaves[i].slaveinfo = hal_malloc(sizeof(ec_slave_info_t));

        slave_input_t * input = motion_slaves[i].slave_input;
        slave_output_t * output = motion_slaves[i].slave_output;

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.al_state",i);
        retval = hal_pin_u32_new(buf, HAL_OUT, &(output->al_state), comp_id);
        if (retval != 0) {
            return -1;
        }


        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.errorflag",i);
        retval = hal_pin_u32_new(buf, HAL_OUT, &(output->error_flag), comp_id);
        if (retval != 0) {
            return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.velocity",i);
        retval = hal_pin_float_new(buf, HAL_IN, &(input->vel), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.enable",i);
        retval = hal_pin_bit_new(buf, HAL_IN, &(input->enable), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.start-probe",i);
        retval = hal_pin_bit_new(buf, HAL_IO, &(input->start_probe), comp_id);
        if (retval != 0) {
            return -1;
        }
        *input->start_probe = 0;

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.set_zero",i);
        retval = hal_pin_bit_new(buf, HAL_IN, &(input->set_zero), comp_id);
        if (retval != 0) {
            return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.index_enable",i);
        retval = hal_pin_bit_new(buf, HAL_IN, &(input->index_enable), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.scale",i);
        retval = hal_param_float_new(buf, HAL_RW, &(input->scale), comp_id);
        if (retval != 0) {
            return -1;
        }
        input->scale = 1;
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.scaledir",i);
        retval = hal_param_float_new(buf, HAL_RW, &(input->scaledir), comp_id);
        if (retval != 0) {
            return -1;
        }
        input->scaledir = 1;

        for(  n = 0;  n < IO_DINPUT_NUM;  n++ ) {

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.din.%02d.in",i,n);
            retval = hal_pin_bit_new(buf, HAL_OUT, &(input->in[n]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.din.%02d.infliter",i,n);
            retval = hal_param_float_new(buf, HAL_RW, &(input->infliter[n]), comp_id);
            if (retval != 0) {
                return -1;
            }
            motion_slaves[i].par_input->flitercount[n] = 0;
            (input->infliter[n]) = 5;

        }


        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.alarm",i);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(output->alarm), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.outscale",i);
        retval = hal_param_float_new(buf, HAL_RW, &(output->scale), comp_id);
        if (retval != 0) {
            return -1;
        }
        output->scale = 1;

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.linkstate",i);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(output->linkstate), comp_id);
        if (retval != 0) {
            return -1;
        }



        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.enabled",i);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(output->enabled), comp_id);
        if (retval != 0) {
            return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.probe_done",i);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(output->probe_done), comp_id);
        if (retval != 0) {
            return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.fbposition",i);
        retval = hal_pin_float_new(buf, HAL_OUT, &(output->fbposition), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.errorcode",i);
        retval = hal_pin_s32_new(buf, HAL_IN, &(output->errorcode), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.encoderpos",i);
        retval = hal_pin_u32_new(buf, HAL_OUT, &(output->encoderpos), comp_id);
        if (retval != 0) {
            return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.encoderscale",i);
        retval = hal_param_float_new(buf, HAL_RW, &(output->encoderscale), comp_id);
        if (retval != 0) {
            return -1;
        }
        output->encoderscale = 1;

        for(  n = 0;  n < IO_DOUTPUT_NUM;  n++ ) {

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.dout.%02d.out",i,n);
            retval = hal_pin_bit_new(buf, HAL_IN, &(output->out[n]), comp_id);
            if (retval != 0) {
                return -1;
            }

        }


        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.200b",i);
        retval = hal_pin_u32_new(buf, HAL_OUT, &(output->mt_200b_0d), comp_id);
        if (retval != 0) {
            return -1;
        }


    }


    for( i = 0; i < config_io_num; i++ ) {

        io_slaves[i].slave_output = hal_malloc(sizeof(io_slave_output_t));
        for(  n = 0;  n < IO_DINPUT_NUM;  n++ ) {

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%d.din.%02d.in",i,n);
            retval = hal_pin_bit_new(buf, HAL_OUT, &(io_slaves[i].slave_output->in[n]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%d.din.%02d.infliter",i,n);
            retval = hal_param_float_new(buf, HAL_RW, &(io_slaves[i].slave_output->infliter[n]), comp_id);
            if (retval != 0) {
                return -1;
            }
            (io_slaves[i].slave_output->infliter[n]) = 5;
            io_slaves[i].par_input->flitercount[n] = 0;

        }

        io_slaves[i].slave_input = hal_malloc(sizeof(io_slave_input_t));
        for(  n = 0;  n < IO_DOUTPUT_NUM;  n++ ) {

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%d.dout.%02d.out",i,n);
            retval = hal_pin_bit_new(buf, HAL_IN, &(io_slaves[i].slave_input->out[n]), comp_id);
            if (retval != 0) {
                return -1;
            }

        }


        int j=0;
        for(j=0;j<8;j++) {

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.pulse_%d",i,j);
            retval = hal_pin_float_new(buf, HAL_IN, &(io_slaves[i].slave_input->pulse[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.speed_%d",i,j);
            retval = hal_pin_float_new(buf, HAL_IN, &(io_slaves[i].slave_input->speed[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.move_mode_%d",i,j);
            retval = hal_pin_u32_new(buf, HAL_IN, &(io_slaves[i].slave_input->move_mode[j]), comp_id);
            if (retval != 0) {
                return -1;
            }
            *(io_slaves[i].slave_input->move_mode[j]) = 0;


            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.recordpulse_%d",i,j);
            retval = hal_pin_float_new(buf, HAL_IN, &(io_slaves[i].slave_input->recordpulse[j]), comp_id);
            if (retval != 0) {
                return -1;
            }


            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.delta_%d",i,j);
            retval = hal_pin_u32_new(buf, HAL_OUT, &(io_slaves[i].slave_input->delta[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.totaldelta_%d",i,j);
            retval = hal_pin_u32_new(buf, HAL_OUT, &(io_slaves[i].slave_input->totaldelta[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.pulsescale_%d",i,j);
            retval = hal_param_float_new(buf, HAL_RW, &(io_slaves[i].slave_input->pulsescale[j]), comp_id);
            if (retval != 0) {
                return -1;
            }
            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.fbpulsescale_%d",i,j);
            retval = hal_param_float_new(buf, HAL_RW, &(io_slaves[i].slave_input->fbpulsescale[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.posscaledir_%d",i,j);
            retval = hal_param_float_new(buf, HAL_RW, &(io_slaves[i].slave_input->posscaledir[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.fbscaledir_%d",i,j);
            retval = hal_param_float_new(buf, HAL_RW, &(io_slaves[i].slave_input->fbscaledir[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.svron_%d",i,j);
            retval = hal_pin_bit_new(buf, HAL_IN, &(io_slaves[i].slave_input->svron[j]), comp_id);
            if (retval != 0) {
                return -1;
            }
            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.svrclr_%d",i,j);
            retval = hal_pin_bit_new(buf, HAL_IN, &(io_slaves[i].slave_input->svrclr[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.indexenable_%d",i,j);
            retval = hal_pin_bit_new(buf, HAL_IO, &(io_slaves[i].slave_input->indexEnable[j]), comp_id);
            if (retval != 0) {
                return retval;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.notmove_%d",i,j);
            retval = hal_pin_bit_new(buf, HAL_IO, &(io_slaves[i].slave_input->notmove[j]), comp_id);
            if (retval != 0) {
                return retval;
            }


            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.svralm_%d",i,j);
            retval = hal_pin_bit_new(buf, HAL_OUT, &(io_slaves[i].slave_output->svralm[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.svrindex_%d",i,j);
            retval = hal_pin_bit_new(buf, HAL_OUT, &(io_slaves[i].slave_output->svrindex[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.fbpulse_%d",i,j);
            retval = hal_pin_float_new(buf, HAL_IN, &(io_slaves[i].slave_output->fbpulse[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.recordfbspeed_%d",i,j);
            retval = hal_pin_float_new(buf, HAL_IN, &(io_slaves[i].slave_output->record_fbpulse[j]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.fbspeed_%d",i,j);
            retval = hal_pin_float_new(buf, HAL_IN, &(io_slaves[i].slave_output->fbspeed[j]), comp_id);
            if (retval != 0) {
                return -1;
            }
        }


        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.fpgaver",i);
        retval = hal_pin_u32_new(buf, HAL_OUT, &(io_slaves[i].slave_output->fpgaVer), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.mcuver",i);
        retval = hal_pin_u32_new(buf, HAL_OUT, &(io_slaves[i].slave_output->mcuVer), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.chkboard",i);
        retval = hal_pin_u32_new(buf, HAL_OUT, &(io_slaves[i].slave_output->checkBoard), comp_id);
        if (retval != 0) {
            return -1;
        }




    }

    //  build the handleval pin  here
    masterpins = hal_malloc(sizeof(master_pins) );

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.resetdata");
    retval = hal_pin_bit_new(buf, HAL_IN, &(masterpins->reset_data), comp_id);
    if (retval != 0) {
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.readdata");
    retval = hal_pin_bit_new(buf, HAL_IN, &(masterpins->read_data), comp_id);
    if (retval != 0) {
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.writedata");
    retval = hal_pin_bit_new(buf, HAL_IN, &(masterpins->write_data), comp_id);
    if (retval != 0) {
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.ready");
    retval = hal_pin_bit_new(buf, HAL_OUT, &(masterpins->ready), comp_id);
    if (retval != 0) {
        return -1;
    }


    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.handleVal");
    retval = hal_pin_u32_new(buf, HAL_IN, &(masterpins->handleVal), comp_id);
    if (retval != 0) {
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.slavenum");
    retval = hal_pin_u32_new(buf, HAL_IN, &(masterpins->slaveNum), comp_id);
    if (retval != 0) {
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.maxtime");
    retval = hal_pin_u32_new(buf, HAL_OUT, &(masterpins->maxtime), comp_id);
    if (retval != 0) {
        return -1;
    }
    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.realruntime");
    retval = hal_pin_u32_new(buf, HAL_OUT, &(masterpins->realruntime), comp_id);
    if (retval != 0) {
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.planein0");
    retval = hal_pin_u32_new(buf, HAL_OUT, &(masterpins->plane_in0), comp_id);
    if (retval != 0) {
        return -1;
    }
    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.planein1");
    retval = hal_pin_u32_new(buf, HAL_OUT, &(masterpins->plane_in1), comp_id);
    if (retval != 0) {
        return -1;
    }

    for( i = 0; i < IO_SLAVES_MAXNUM; i++ ) {
        //DA
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.DAScale.%d", i);
        retval = hal_param_float_new(buf, HAL_RW,
                &(masterpins->DAScale[i]), comp_id);
        if (retval != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.DAVol.%d",i);
        retval = hal_pin_float_newf(HAL_IN, &(masterpins->DAVol[i]), comp_id,buf);
        if (retval != 0) {
            return -1;
        }
    }
    for( i = 0; i < 10; i++ ) {
        //TestTime
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.master.TestTime.%d", i);
        retval = hal_pin_u32_new(buf, HAL_OUT,
                                 &(masterpins->testtime[i]), comp_id);
        if (retval != 0) {
             return -1;
        }
    }


    return 0;

}

static int exportEthercatPar() {

    int i,rv;
    char buf[HAL_NAME_LEN+2];


    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        motion_slaves[i].par_output = hal_malloc(sizeof(paremeter_output_t));
        motion_slaves[i].par_input = hal_malloc(sizeof(paremeter_input_t));

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6040", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_6040), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_607A", i);
        rv = hal_param_s32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_607A), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_60B0", i);
        rv = hal_param_s32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_60B0), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6060", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_6060), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_2061", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_2061), comp_id);
        if (rv != 0) {
             return -1;
        }


        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_probe_ctl", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_probe_ctl), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_probe_status", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_probe_status), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_probe_pos", i);
        rv = hal_param_s32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_probe_pos), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6041", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6041), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6064", i);
        rv = hal_param_s32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6064), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6061", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6061), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_603F", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_603F), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_60FD", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_60FD), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_203F", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_203F), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_2060", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_2060), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_2062", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_2062), comp_id);
        if (rv != 0) {
             return -1;
        }

//        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_200b", i);
//        rv = hal_param_u32_new(buf, HAL_RO,
//                &(motion_slaves[i].par_input->data_200b), comp_id);
//        if (rv != 0) {
//             return -1;
//        }

    }


    for( i = 0; i < IO_SLAVES_MAXNUM; i++ ) {

        io_slaves[i].par_output = hal_malloc(sizeof(io_paremeter_output_t));
        io_slaves[i].par_input = hal_malloc(sizeof(io_paremeter_input_t));

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_6000", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(io_slaves[i].par_input->data_6000), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_6000_1", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(io_slaves[i].par_input->data_6000_1), comp_id);
        if (rv != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_6000_2", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(io_slaves[i].par_input->data_6000_2), comp_id);
        if (rv != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_6000_3", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(io_slaves[i].par_input->data_6000_3), comp_id);
        if (rv != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_6000_4", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(io_slaves[i].par_input->data_6000_4), comp_id);
        if (rv != 0) {
             return -1;
        }



        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_7010", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(io_slaves[i].par_output->data_7010), comp_id);
        if (rv != 0) {
             return -1;
        }


        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_7010_1", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(io_slaves[i].par_output->data_7010_1), comp_id);
        if (rv != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_7010_2", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(io_slaves[i].par_output->data_7010_2), comp_id);
        if (rv != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_7010_3", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(io_slaves[i].par_output->data_7010_3), comp_id);
        if (rv != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_7010_4", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(io_slaves[i].par_output->data_7010_4), comp_id);
        if (rv != 0) {
             return -1;
        }


    }
    return 0;


}

int testBit(unsigned int data,int bit) {
    unsigned int tmp = data & ((unsigned int )( 1 << bit));
    if(tmp == 0)
        return 0;
    else
        return 1;
}
#define setBit(data,bit) ( (data) |= (1 << (bit) ))
#define clearBit(data,bit) ( (data) &= (~(1 << (bit) ) ))


/*****************************************************************/
int ServoOn_GetCtrlWrd(uint16_t StatusWord, uint16_t *ControlWord)
{
    int  _enable=0;
    //printf("STS %X CTL:%X\n",StatusWord,*ControlWord);
    if (bit_is_clear(StatusWord, STATUSWORD_OPERATION_ENABLE_BIT)) //Not ENABLED yet
    {
        if (bit_is_clear(StatusWord, STATUSWORD_SWITCHED_ON_BIT)) //Not SWITCHED ON yet
        {
            if (bit_is_clear(StatusWord, STATUSWORD_READY_TO_SWITCH_ON_BIT)) //Not READY to SWITCH ON yet
            {
                if (bit_is_set(StatusWord, STATUSWORD_FAULT_BIT)) //FAULT exist
                {
                    (*ControlWord)=0x80;	//FAULT RESET command
                }
                else //NO FAULT
                {
                    (*ControlWord)=0x06;	//SHUTDOWN command (transition#2)
                }
            }
            else //READY to SWITCH ON
            {
                (*ControlWord)=0x07;	//SWITCH ON command (transition#3)
            }
        }
        else //has been SWITCHED ON
        {
            (*ControlWord)=0x0F;	//ENABLE OPETATION command (transition#4)
            _enable=1;
        }
    }
    else //has been ENABLED
    {
        (*ControlWord)=0x0F;	//maintain OPETATION state
        _enable=1;
    }
    return _enable;
}

static int triggle_try_connect = 1;
static uint64 countForSetlink = 0;
static void handleExchangeData() {

    int i,n,tmp_int,servok,mainIn,subIn;
    uint16_t controlword;
    uint16_t statusWD;
    slave_config_t *s;
    static double local_scale[ETH_MAX_SLAVES];
    float tmp;


    //check servo IO
    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        s = &motion_slaves[i];
        tmp = fabs(motion_slaves[i].slave_input->scale);
        if(  tmp <= EPSILON  )
             local_scale[i] = 1;
        else
             local_scale[i] = motion_slaves[i].slave_input->scale;
        if(motion_slaves[i].slave_input->scaledir == -1)
            local_scale[i] = -local_scale[i];

        local_scale[i] = 1/local_scale[i];

    }


    if( g_com_status && g_com_status->link_status == 1 && g_com_status->com_error == 0 ) {
        for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
            *(motion_slaves[i].slave_output->linkstate) = 1;
        }
    } else {
        for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
            *(motion_slaves[i].slave_output->linkstate) = 0;
              motion_slaves[i].par_output->data_6040 = 0;  //断线的情况下停止伺服
        }
    }
    //if(g_com_status) printf("g_com_status====%d %d\n",g_com_status->link_status,g_com_status->com_error);

    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {


        s = &motion_slaves[i];
        if( (testBit( s->par_input->data_6041,3) )  ) { //有故障，报警设为1

            *(s->slave_output->alarm) = 1;
            if( *(s->slave_output->alarm) != s->last_alarm && g_com_status) {

                if( i < config_axis_num ) {
                    snprintf( &(g_com_status->cmd_info_name[100][1]), 126, "站号%d报警==>报警代码%x\n", i, s->par_input->data_603F);
                    printf("站号%d报警==>报警代码%x\n",i,s->par_input->data_603F);
                    g_com_status->cmd_info_name[100][0]++;
                }

            }
            if(s->isInited == 0)
                s->par_output->data_6040 = 0x80;
            else {
                s->par_output->data_6040 = 0x00;
                *(s->slave_input->enable) = 0;
            }

        } else {
            *(s->slave_output->alarm) = 0;
        }
        s->last_alarm = *(s->slave_output->alarm);

        /*if( g_com_status == NULL )
            *(s->slave_output->alarm) = 1;
        else if( g_com_status->link_status == 0  || g_com_status->com_error != 0 )
            *(s->slave_output->alarm) = 1;*/
        motion_slaves[i].par_output->data_6060 = 8;

    }

    //check Actions
    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        if( i >= config_axis_num ) continue;

        s = &motion_slaves[i];
        if( *(s->slave_input->enable) == 0 && \
                s->last_slave_input.enable == 1 ) {
            s->par_output->data_6040 = 0;
        }


        if( *(s->slave_input->enable) == 1 && \
              s->last_slave_input.enable == 0 )  {
            s->serv_on_process_count = 0;
        }
        if( *(s->slave_input->enable) == 1)  s->serv_on_process_count++; //delay 200ms

        //检查伺服使能指令：
        //默认打开伺服，并且设置成csp模式
        if( *(s->slave_input->enable) == 1 && s->serv_on_process_count>100  && countForSetlink > 500 )  {

            int toset = (int)( ((*(s->slave_input->vel))*local_scale[i]) + (float)0.5);
            if( abs( s->par_input->data_6064 - toset ) < 200 || s->isInited == 1 ) {

                controlword=0;
                statusWD = s->par_input->data_6041;
                servok = ServoOn_GetCtrlWrd(statusWD, &controlword);
                s->par_output->data_6040 = controlword;
                *(s->slave_output->enabled) = servok;

                if( servok )
                    s->isInited = 1;

            } else {

                if( i < config_axis_num ) {
                    snprintf( &(g_com_status->cmd_info_name[100][1] ),126,"请确认驱动器%d反馈已经打开\n",i);
                    printf("请确认驱动器%d反馈已经打开\n",i);
                    g_com_status->cmd_info_name[100][0]++;
                }

                *(s->slave_output->alarm) = 1;
                //if( countForSetlink%2000 == 0 ) {
                //}

            }


        }
        s->last_slave_input.enable = *(s->slave_input->enable);


        if( *(s->slave_output->alarm)!=1 && \
                *(s->slave_output->enabled) == 1 && \
                testBit(s->par_input->data_6041,0) == 1 && \
                testBit(s->par_input->data_6041,2) == 1 && \
                testBit(s->par_input->data_6041,3) == 0 ) {

             //s->par_output->data_607A = (int)( ((*(s->slave_input->vel))*local_scale[i]) + (float)0.5);
             s->par_output->data_607A = ((long long)((*(s->slave_input->vel))*local_scale[i]) & 0xffffffff);

        } else {

            s->par_output->data_60B0 = 0;
            s->par_output->data_607A = s->par_input->data_6064;

        }

        *(s->slave_output->fbposition) = (float)(s->par_input->data_6064 - s->par_output->data_60B0)/local_scale[i];
        *(s->slave_output->errorcode) = s->par_input->data_603F & 0xFFFF;
        *(s->slave_output->encoderpos) = s->par_input->data_2062 * (s->slave_output->encoderscale);


        //关闭探测
        if( *(s->slave_input->start_probe) == 0 && \
                s->last_slave_input.start_probe == 1 ) {
                if( *(s->slave_output->linkstate) == 0 ) {
                } else {
                    s->par_output->data_probe_ctl = 0x00;
                }
        }
        //检查开启检测功能使能
        if( *(s->slave_input->start_probe) == 1 && \
                s->last_slave_input.start_probe == 0 ) {
                if( *(s->slave_output->linkstate) == 0 ) {
                } else {
                    if(s->probe_process_flag == 0) {
                         s->probe_process_flag = 1;
                         *(s->slave_output->probe_done) = 0;
                    }
                }
        }
        s->last_slave_input.start_probe = *(s->slave_input->start_probe);


        //open检测过程
        if( s->probe_process_flag == 1 ) {

            if(s->probe_process_count ==  0) {
                s->par_output->data_probe_ctl = 0x00;
            }  else if(s->probe_process_count ==  1) {
                s->par_output->data_probe_ctl = 0x17;
            } else if(s->probe_process_count ==  2) {
                s->probe_process_flag = 0;
                s->probe_process_count = 0;
                *(s->slave_output->probe_done) = 0;
            }
            s->probe_process_count ++;
        }

        //刷新探测信号
        if( s->last_paremeter_input.data_probe_status == 0 && \
            testBit(s->par_input->data_probe_status,1) == 1 ) {

            s->probe_process_flag = 1;
            s->par_output->data_probe_ctl = 0x00;
            *(s->slave_output->probe_done) = 1;
            *(s->slave_input->start_probe) = 0;

         }
        s->last_paremeter_input.data_probe_status = testBit(s->par_input->data_probe_status,1);

        //刷新探测位置
        //*(s->slave_output->probe_done) = testBit(s->par_input->data_probe_status,1);

    }

    int firstRegIoIndex = -1;
    for( i = 0; i < config_io_num; i++ ) {

        //if(io_slaves[i].valid == 0) continue;
        if(masterpins->DAScale[0] == 0.0)
            masterpins->DAScale[0] = 1.0;
        if(masterpins->DAScale[1] == 0.0)
            masterpins->DAScale[1] = 1.0;
        int vol = 10000 * (*(masterpins->DAVol[0])/60000) * (masterpins->DAScale[0]);
        int vol2 = 10000 * (*(masterpins->DAVol[1])/60000) * (masterpins->DAScale[1]);
        //vol2 = vol/2;
        if(vol > 10000) vol = 10000;
        if(vol2 > 10000) vol2 = 10000;
        io_slaves[i].par_output->data_7010_1 = vol;
        io_slaves[i].par_output->data_7010_1 |= vol2 << 16;
        //计算DA电压输出
        io_slaves[i].par_output->output[1] = io_slaves[i].par_output->data_7010_1;


        //E68C DATA Fill
        for( n = 0; n < 8; n++ ) {
            io_slaves[i].par_input->data_6003[n] = io_slaves[i].par_input->intput[n+2];
        }

        int j=0;float localscale = 1,localfbscale = 1;
        int fbscaledir = 1;
        int posscaledir = 1;
        for(j=0;j<8;j++) {
            localscale =io_slaves[i].slave_input->pulsescale[j];
            localfbscale =io_slaves[i].slave_input->fbpulsescale[j];
            if(   fabs(localscale) <= EPSILON  )
                 localscale = 1;
            if(localscale < 0)
                localscale = -localscale;
             localscale = 1/localscale;

             if(fabs(localfbscale) <= EPSILON || fabs(localfbscale) > 1) {
                 localfbscale = 1;
             } else {
                 localfbscale = 1/localfbscale;
             }

             fbscaledir = io_slaves[i].slave_input->fbscaledir[j];
             posscaledir = io_slaves[i].slave_input->posscaledir[j];
             if(posscaledir == 0)
                 posscaledir = 1;
             localscale = localscale * posscaledir;

             int delta = (int)(fmod(*(io_slaves[i].slave_input->pulse[j]) * localscale , 2147483647)) - (int)io_slaves[i].recordpos[j];
             delta = (int)(fmod(*(io_slaves[i].slave_input->pulse[j]) * localscale , 2147483647)) - (int)io_slaves[i].recordpos[j];

             *(io_slaves[i].slave_input->delta[j]) = delta;
            *(io_slaves[i].slave_input->recordpulse[j]) = io_slaves[i].recordpos[j];


             //测试recordpos为float型时表达不全 (*(io_slaves[i].slave_input->pulse[j]) * localscale 导致一直有差值
             if(abs(delta) > 0) {
 //                printf("-xx now I in here get delta:%d pulse %f  record:%f   scale:%f   local:%f  axis:%d \n",delta,*(io_slaves[i].slave_input->pulse[j])* localscale,io_slaves[i].recordpos[j], localscale,*(io_slaves[i].slave_input->pulse[j]),j);
             }


             if(abs(delta) > 10000) {
                 io_slaves[i].recordpos[j] = (int)(fmod(*(io_slaves[i].slave_input->pulse[j]) * localscale , 2147483647));
                break;
             }


             if(*(io_slaves[i].slave_input->svron[j]) > 0) {
                 if( *(io_slaves[i].slave_input->indexEnable[j]) > 0) {
                     io_slaves[i].par_output->data_7014[j] |= (0x01<<17); //第17位为indexenable信号
                 } else {
                     io_slaves[i].par_output->data_7014[j] &= ~(0x01<<17); //第17位为indexenable信号
                 }
                 if( *(io_slaves[i].slave_input->notmove[j]) > 0)
                    delta = 0;
                 io_slaves[i].par_output->data_7014[j] &= ~(0x01<<18); //第18位为svron信号
                 io_slaves[i].par_output->data_7014[j] &= 0xFFFF0000;
                 io_slaves[i].par_output->data_7014[j] |= (abs(delta) & 0xFFFF); //低16位为脉冲數


                 *(io_slaves[i].slave_input->totaldelta[j]) += delta;

                 if(delta < 0) {
                     io_slaves[i].par_output->data_7014[j] |= (0x01<<16); //第16位为方向
                 } else {
                     io_slaves[i].par_output->data_7014[j] &= ~(0x01<<16); //第16位为方向
                 }
             }else {
                 io_slaves[i].par_output->data_7014[j] &= 0xFFFF0000; //低16位为脉冲數
                 io_slaves[i].par_output->data_7014[j] |= (0x01<<18); //第18位为svron信号
                 *(io_slaves[i].slave_output->checkBoard) =  (io_slaves[i].par_input->data_6003[5] >> 20) & 0x0F;          //使能一断，恢复正常通讯
             }
             //判断是脉冲还是步进
             if(io_slaves[i].par_output->axis_type[j] == 1) {
                 io_slaves[i].par_output->data_7014[j] |= (0x01<<19); //第19位为轴类型 （步进）
             } else {
                io_slaves[i].par_output->data_7014[j] &= ~(0x01<<19); //第19位为轴类型 （伺服）
             }


             static totalcount = 0;
             io_slaves[i].recordpos[j] = (int)(fmod(*(io_slaves[i].slave_input->pulse[j]) * localscale , 2147483647));

             //处理输入的数据

             union pos_tag {
                 signed int l;
                 struct byte_tag {
                     signed char b0;//低8位
                     signed char b1;//中8位
                     signed char b2;//高8位
                     signed char b3;//没用，与b0、b1、b2凑成一个int型数据
                 } byte;
             } pos;

             pos.l = (io_slaves[i].par_input->data_6003[j] & 0xFFFFFF);
             if((pos.byte.b2 & 0xC0) == 0xC0) {
                 pos.byte.b3 = 0xff;
             } else {
                 pos.byte.b3 = 0;
             }

             if(abs(delta) > 0.00001 ||( j==0 && *(io_slaves[i].slave_input->notmove[j]) == 1)) {
                //printf("nwo get %d data %d  data_7014:%d %d  %d  input:%.3f\n",j,delta,io_slaves[i].par_output->data_7014[j],pos.l ,io_slaves[i].par_input->data_6003[j],*(io_slaves[i].slave_input->pulse[j]));
             }

             if(fbscaledir == 0)
                 fbscaledir = 1;
             *(io_slaves[i].slave_output->fbpulse[j]) = (pos.l)/localfbscale*fbscaledir;
             *(io_slaves[i].slave_output->svralm[j]) = (io_slaves[i].par_input->data_6003[j] >> 24) & 0x01;
             *(io_slaves[i].slave_output->svrindex[j]) =  (io_slaves[i].par_input->data_6003[j] >> 25) & 0x01;


             *(io_slaves[i].slave_output->fbspeed[j]) = pos.l - *(io_slaves[i].slave_output->record_fbpulse[j]);
             *(io_slaves[i].slave_output->record_fbpulse[j]) = pos.l;
             //编码器零点清零操作
             if(*(io_slaves[i].slave_input->indexEnable[j]) == 1 && \
                     *(io_slaves[i].slave_output->svrindex[j]) == 1 ) {
                 printf("now I in clear index\n");
                 *(io_slaves[i].slave_input->notmove[j]) = 1;
                 *(io_slaves[i].slave_input->indexEnable[j])= 0;
                 io_slaves[i].indexdelay[j] = 450;
             }

             if(*(io_slaves[i].slave_input->notmove[j]) > 0) {
                 if(io_slaves[i].indexdelay[j] > 0) {
                     io_slaves[i].indexdelay[j] -- ;
                 } else {
                     io_slaves[i].indexdelay[j] = 0;
                    *(io_slaves[i].slave_input->notmove[j]) = 0;
                     printf("now I in wait over clear index\n");
                 }
             }


        }


        *(io_slaves[i].slave_output->fpgaVer) =  (io_slaves[i].par_input->data_6003[5] >> 28) & 0x0F;
        *(io_slaves[i].slave_output->mcuVer) =  (io_slaves[i].par_input->data_6003[5] >> 16) & 0x0F;
        if(*(io_slaves[i].slave_output->checkBoard) != 0x0f)
            *(io_slaves[i].slave_output->checkBoard) =  (io_slaves[i].par_input->data_6003[5] >> 20) & 0x0F;


        //E68C DATA Fill
        for( n = 0; n < 8; n++ ) {
            io_slaves[i].par_output->output[n+3] = io_slaves[i].par_output->data_7014[n];
        }
        //E68C DATA Fill
        for(  n = 0;  n < IO_DOUTPUT_NUM;  n++ ) {
            int index=0,port=0;
            index = n/8;
            port = n%8;
            if( (index+3) < (IO_DINPUT_NUM_MAX-1) ) {
                if( *(io_slaves[i].slave_input->out[n]) == 1 ) {
                    io_slaves[i].par_output->output[index+3] |= (0x01<<(24+port));
                } else {
                    io_slaves[i].par_output->output[index+3] &= ~(0x01<<(24+port));
                }
            }
        }


        for(  n = 0;  n < IO_DOUTPUT_NUM;  n++ ) {

            /*normal io board
            tmp_int = (0x01<<n);
            if(  *(io_slaves[i].slave_input->out[n]) == 1 )
                io_slaves[i].par_output->data_7010 |= tmp_int;
            else
                io_slaves[i].par_output->data_7010 &= (~tmp_int);
            */
            mainIn = n/32;
            subIn = n%32;
            tmp_int = (0x01<<subIn);
            if(  *(io_slaves[i].slave_input->out[n]) == 1 )
                io_slaves[i].par_output->output[mainIn] |= tmp_int;
            else
                io_slaves[i].par_output->output[mainIn] &= (~tmp_int);

        }




        for(  n = 0;  n < IO_DINPUT_NUM;  n++ ) {


            mainIn = n/32;
            subIn = n%32;
            tmp_int = (0x01<<subIn);

            if( ( tmp_int & (io_slaves[i].par_input->intput[mainIn]/*io_slaves[i].par_input->data_6000*/) ) ) {

                if(*(io_slaves[i].slave_output->in[n]) > 0) {
                    io_slaves[i].par_input->flitercount[n] = (io_slaves[i].slave_output->infliter[n]);
                } else {
                    if(io_slaves[i].par_input->flitercount[n] >= (io_slaves[i].slave_output->infliter[n])) {
                        *(io_slaves[i].slave_output->in[n])  = 1;
                    } else {
                        io_slaves[i].par_input->flitercount[n]++;
                    }
                }

            } else {

                if(*(io_slaves[i].slave_output->in[n]) == 0) {
                    io_slaves[i].par_input->flitercount[n] = 0;
                } else {
                    if(io_slaves[i].par_input->flitercount[n] <= 0) {
                        *(io_slaves[i].slave_output->in[n]) = 0;
                    } else {
                        io_slaves[i].par_input->flitercount[n]--;
                    }
                }

            }

        }

        if( io_board_num > 0 && (io_board_num-1) == i ) {
            *(masterpins->plane_in0) = 0xFF & (io_slaves[i].par_input->intput[0]);
            *(masterpins->plane_in1) = 0xFF & (io_slaves[i].par_input->intput[0]>>8);
        } else {
            firstRegIoIndex ++;
        }
        //获取手轮变化值
        if( firstRegIoIndex == 0 ) {
            //firstIndex
            *(masterpins->handleVal) = io_slaves[i].par_input->intput[1];
            /*
            timeafter = rtapi_get_time();
            difftime = (long int)  (timeafter-timebefore);
            if(*(masterpins->testtime[7]) < difftime) {
                *(masterpins->testtime[7]) = difftime;
            }
            */
        }
        //if( countForSetlink%500 == 0)
        //    printf("OutPutDa %X  v1 %X v2 %X  %.1f %.1f  %.1f %.1f\n",io_slaves[i].par_output->output[1],vol,vol2,(*(masterpins->DAVol[0])),*(masterpins->DAVol[1]),masterpins->DAScale[0],masterpins->DAScale[1]);
    }

    errorConnect = 0;
    for( i = 0; i < config_axis_num+config_io_num; i++ ) {
        if(*(motion_slaves[i].slave_output->linkstate) == 0) {
            errorConnect = 1;
            break;
        }
    }

    if(errorConnect > 0) {
        *(masterpins->ready) = 0;
        reConnectcount = 0;
    }else {
        if(reConnectcount > 10) {
            *(masterpins->ready) = 1;
        } else {
            reConnectcount++;
        }
    }

}

static void invalidAllSlaves() {
    int i;
    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
        motion_slaves[i].valid = 0;
        *(motion_slaves[i].slave_output->linkstate) = 0;
    }
}

int rtapi_app_main(void)
{

    char buf[HAL_NAME_LEN+2];
    int i,rv;
    slave_config_t *s;

    comp_id = hal_init("hal_ethercat");
    if (comp_id < 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "ETHERCAT: ERROR: hal_init() failed\n");
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.00.exchange_data");
    rv = hal_export_funct(buf, exchange_data, NULL, 0, 0, comp_id);
    if (rv != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "ETHERCAT: ERROR: exchange_data funct export failed\n");
        hal_exit(comp_id);
        return -1;
    }

    motion_slaves = malloc(sizeof(slave_config_t) * ETH_MAX_SLAVES);
    io_slaves = malloc(sizeof(io_slave_config_t) * IO_SLAVES_MAXNUM);
    //io_sc = malloc( sizeof(ec_slave_config_t *) * IO_SLAVES_MAXNUM);
    //motion_sc = malloc( sizeof(ec_slave_config_t *) * ETH_MAX_SLAVES);

    int errorid = rtapi_shmem_new(22222,comp_id,sizeof(version_struct));
      int retval = rtapi_shmem_getptr(errorid,(void **)&versiondata);
      memset(versiondata->errorbuf,0,sizeof(versiondata->errorbuf));

      versiondata->ethercatVersion = ethercatVersion;

    if(config_axis_num > ETH_MAX_SLAVES ) config_axis_num = ETH_MAX_SLAVES;
    if(config_io_num > IO_SLAVES_MAXNUM ) config_io_num = IO_SLAVES_MAXNUM;

    initSlaveConfigStruct();
    initBusState();
    exportEthercatPar();
    exportAllPins();
    invalidAllSlaves();

    initEthercat();
    hal_ready(comp_id);

     return 0;

}

void rtapi_app_exit(void)
{

    printf("now I get in abort now\n");
    slave_config_t *s;
    int j=0;
    if( g_com_status )
        g_com_status->link_status=0;

    if( g_rtdm_fd > 0 && g_com_status ) {
        munmap(g_com_status,sizeof(COM_STATUS_t));
        close(g_rtdm_fd);
    }
    //for(j=0;j<ETH_MAX_SLAVES;j++) {
        //s = &motion_slaves[j];
        //    s->par_output->data_6040 = 0x80;
        //    exchange_data(0,0);
    //}
    hal_exit(comp_id);

}

void check_IO_state() {


}

void check_bus_state(void) {


}

#define uchar unsigned char
#define uint unsigned int


static unsigned char auchCRCHi[2048] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
static unsigned char auchCRCLo[2048] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


void calculateCrc16(unsigned char *puchMsg,unsigned char *out,unsigned short usDataLen ) {

    unsigned char uchCRCHi = 0xFF ;
    unsigned char uchCRCLo = 0xFF ;
    unsigned uIndex ;
    while (usDataLen--)
    {

        uIndex = uchCRCHi ^ *puchMsg++ ;
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex] ;

    }
    out[0] = uchCRCHi;
    out[1] = uchCRCLo;

}

uint32_t TargetPosition[64];
static int emptyPoolWarninng = 0;
static void exchange_data(void *args,long peri)
{

    int i = 0,usi,j,nk;
    int haseEmpty = 0;
    int cc = 0;

    countForSetlink ++;
    if( g_com_status == NULL ) return;

    g_com_status->heartbeat++;
    g_com_status->expect_num_of_io = config_io_num;
    g_com_status->expect_num_of_servo = config_axis_num;

    /*
    if( countForSetlink >= 200 && emptyPoolWarninng == 0 ) {
        g_com_status->cmd_set_link = 1;
    } else {
        g_com_status->cmd_set_link = 0;
    }*/

    //这段尝试5次连接
    static int try_connect_count = 0;
    static int old_triggle_try_connect = 0;
    static uint64 countFortryConnect = 0;
    int try_time = MASTER_TRY_COUNT;

    if( countForSetlink >= 200  && \
        g_com_status->link_status == 0 ) {

        //printf("triggle_try_connect %d %d %d\n",triggle_try_connect,try_connect_count,countFortryConnect);
        if( triggle_try_connect == 1 && old_triggle_try_connect == 0 ) {

            printf("triggle_try_connect!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!======\n");
            countFortryConnect=0;
            try_connect_count=0;

        } else if( triggle_try_connect == 1 && try_connect_count < try_time ) {

            if( countFortryConnect <= 200  ) {

                g_com_status->cmd_set_link = 0;

            } else if( countFortryConnect <= 4900 ) {

                g_com_status->cmd_set_link = 1;

            } else if( countFortryConnect <= 5000 ) {

                //zhe li kan lianjie qingkuang
            } else {
                countFortryConnect = 0;
                printf("try to connect slaves ==================> %d \n",try_connect_count);
                try_connect_count++;
            }

        } else if( try_connect_count >=try_time ) {
            //尝试5次都不行就算了
            triggle_try_connect=0;
            //printf("try to connect slaves fail!!!!!!  \n",try_connect_count);
        }
        old_triggle_try_connect = triggle_try_connect;
        countFortryConnect++;

    } else if (countForSetlink < 200) {
        g_com_status->link_status = 0;
    }

    static hal_bit_t old_try_connect_ethercat = 0;
    if( **try_connect_ethercat != old_try_connect_ethercat) {
        printf("get msg try to connect slaves  \n");
        if( **try_connect_ethercat ) {
            triggle_try_connect = 1;
        }
    }
    old_try_connect_ethercat = **try_connect_ethercat;


    handleExchangeData();

    if( g_com_status->com_error == 0 &&
        g_com_status->link_status==1 ) {

        haseEmpty = 1;
        cc = 0;
        while( haseEmpty && cc < 10000   ) {//1s

            for( usi = 0; usi < EH_CMD_POOL_SIZE; usi++ ) {

                if( g_com_status->frames[usi].dirty != 0 ) continue;

                i = usi;
                for( j = 0; j < config_axis_num; j++ ) {

                    g_com_status->frames[i].servoOut[j].ControlWord = (uint16_t)(motion_slaves[j].par_output->data_6040);
                    g_com_status->frames[i].servoOut[j].TargetPosition = motion_slaves[j].par_output->data_607A;
                    g_com_status->frames[i].servoOut[j].ModeOfOperation = (uint8_t)(motion_slaves[j].par_output->data_6060);
                    g_com_status->frames[i].servoOut[j].TouchProbeFunction = motion_slaves[j].par_output->data_probe_ctl;
                    //if( TargetPosition[j]!=g_com_status->frames[i].servoOut[j].TargetPosition)
                    //    printf("SND %d %d %d\n",i,j,g_com_status->frames[i].servoOut[j].TargetPosition);
                    TargetPosition[j]=g_com_status->frames[i].servoOut[j].TargetPosition;
                }
                for( j = 0; j < config_io_num; j++ ) {

                    //g_com_status->frames[i].ioOut[j].output[0] = io_slaves[j].par_output->data_7010_2;
                    //g_com_status->frames[i].ioOut[j].output[1] = io_slaves[j].par_output->data_7010_1;
                    //g_com_status->frames[i].ioOut[j].output[2] = io_slaves[j].par_output->data_7010;
                    for( nk = 0; nk < EH_MAX_IO_DATA; nk++ ) {
                        g_com_status->frames[i].ioOut[j].output[nk] = io_slaves[j].par_output->output[nk];
                    }
                    //printf("\n");

                    //printf("g_com_status->frames[i].ioOut[j].output %d %d %d\n",g_com_status->frames[i].ioOut[j].output1,g_com_status->frames[i].ioOut[j].output2,g_com_status->frames[i].ioOut[j].output3);
                }
                for( j = 0; j < config_axis_num; j++ ) {
                    slave_config_t *s = &(motion_slaves[j]);
                    s->par_input->data_6041 = g_com_status->frames[i].servoIn[j].StatusWord;
                    s->par_input->data_6064 = g_com_status->frames[i].servoIn[j].PositionActualValue;
                    s->par_input->data_6061 = g_com_status->frames[i].servoIn[j].ModeOfOperationDisplay;
                    s->par_input->data_603F = g_com_status->frames[i].servoIn[j].ServoErrorCode;
                }
                for( j = 0; j < config_io_num; j++ ) {

                    io_slaves[j].par_input->data_6000 = g_com_status->frames[i].ioIn[j].intput[0];
                    io_slaves[j].par_input->data_6000_1 = g_com_status->frames[i].ioIn[j].intput[1];

                    for( nk = 0; nk < EH_MAX_IO_DATA; nk++ ) {
                        io_slaves[j].par_input->intput[nk] = g_com_status->frames[i].ioIn[j].intput[nk];
                        //if( countForSetlink%500 == 0)
                        //    printf("%X ",io_slaves[j].par_input->intput[nk]);
                    }
                    //if( countForSetlink%500 == 0) printf("\n");

                }

                calculateCrc16((unsigned char*)g_com_status->frames[i].servoOut,g_com_status->frames[i].crcdata,sizeof(MAXPOS_DRIVE_RxPDO_t)*EH_MAX_AXIS_NUM);

                g_com_status->frames[i].dirty = countForSetlink;
                haseEmpty = 0;
                break;

            }
            cc++;
            g_com_status->heartbeat++;
            rtapi_delay(100*1000);//100us

        }

        if( haseEmpty && countForSetlink > 1000*60 ) {
            emptyPoolWarninng = 1;
            printf("WARNNING!!!!!: empty haseEmpty %d emptyPoolWarninng!!!!!!!!!\n",cc);
        }



    }

    //if( g_com_status->servoIn[0].StatusWord != lastData )
    //    printf("g_com_status->com_error g_com_status->link_status %d %d\n",g_com_status->com_error,g_com_status->servoIn[0].StatusWord);
    //lastData = g_com_status->servoIn[0].StatusWord;
    //handleExchangeData();

    /*
    //获取手轮变化值
    *(masterpins->handleVal) = io_slaves[0].par_input->data_6000_2;
    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[7]) < difftime) {
        *(masterpins->testtime[7]) = difftime;
    }*/

}

//config_axis_channel
static char *config_axis_str[256];
void initEthercat() {


    //if( dataType ) printf("ETGET slave=%d %s index=%d subindex=%d\n",slaveIndex,dataType,dataIndex,dataSubIndex);
    char path[] = RTDM_PATH;
    DIR *d = NULL;
    struct dirent *dp = NULL;
    struct stat st;
    char p[256] = {0};
    //unsigned long long phyaddr = 0;
    unsigned int physize = 0;

    if(!(d = opendir(path))) {
        printf("opendir[%s] error: %m\n", path);
        return -1;
    }

    while((dp = readdir(d)) != NULL) {
        if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2)))
            continue;
        snprintf(p, sizeof(p) - 1, "%s/%s", path, dp->d_name);
        stat(p, &st);
        if(!S_ISDIR(st.st_mode)) {
            //printf("%s\n", dp->d_name);
            strncpy(p,dp->d_name,256);
            char *strv=p;
            char*token=strsep( &strv,"-");
            int ii = 0;
            while(token!=NULL){
              //printf("%s\t",token);
              if(ii==0)
                 ;//phyaddr = strtoll(token, nullptr, 16);
              else
                 physize = atoi(token);
              ii++;
              token=strsep( &strv,"-");
            }
        }
    }

    int g_rtdm_fd = open("/dev/ehmaster",O_RDWR|O_SYNC);
    //printf("Hello World! rt=%d %d :: %lx %d\n",rt,sizeof(COM_STATUS_t),phyaddr,physize);
    if( g_rtdm_fd > 0 && physize>=sizeof(COM_STATUS_t) ) {
        g_com_status = (COM_STATUS_t *) mmap(0,physize,PROT_READ|PROT_WRITE,MAP_SHARED,g_rtdm_fd,0);
        if( g_com_status != MAP_FAILED ) {

            mlockall(MCL_CURRENT|MCL_FUTURE);
            printf("initEthercat= success\n");
            g_com_status->cmd_set_link = 0;
            g_com_status->expect_num_of_io = config_io_num;
            g_com_status->expect_num_of_servo = config_axis_num;
            countForSetlink = 0;


            for(int i=0;i<EH_CMD_POOL_SIZE;i++) {
                g_com_status->frames[i].dirty = 0;
                for(int j=0;j<EH_MAX_AXIS_NUM;j++) {
                    for(int k=0;k<EH_MAX_IO_DATA;k++) {
                        g_com_status->frames[i].ioOut[j].output[k] = 0;
                        g_com_status->frames[i].ioIn[j].intput[k] = 0;
                    }
                }
            }
            g_com_status->cmd_sdo_option = 0; //1 write 2 read
            g_com_status->cmd_sdo_return = 0;
            g_com_status->cmd_sdo_slave = 0;
            g_com_status->cmd_sdo_index = 0;
            g_com_status->cmd_sdo_subindex = 0;
            g_com_status->cmd_sdo_data = 0;
            g_com_status->cmd_sdo_dateLen = 0;
            g_com_status->cmd_info_num = 0;
            for(int i=0;i<EH_MAX_AXIS_NUM*2;i++) {
                for(int j=0;j<128;j++) {
                    g_com_status->cmd_info_name[i][j] = 0;
                }
            }

            char *flag = &(g_com_status->cmd_info_name[EH_MAX_AXIS_NUM*2-1][0]);
            for(int j=0;j<128;j++) flag[j] = 1;

            int getNum = 0;
            split(config_axis_channel, "-", config_axis_str, &getNum);
            printf("config_axis_channel\n");
            for(int j = 0; j < getNum && j < 128; j++ ) {
                int ff = atoi(config_axis_str[j]);
                if( ff > 0 && ff < 16 ) {
                    flag[j] = ff;
                }
                printf("%d ",ff);
            }
            printf("\n");



        }
    }

    /*
    g_rtdm_fd = open(RTDM_PATH,0);
    printf("Hello World! rt=%d %d\n",g_rtdm_fd,sizeof(COM_STATUS_t));
    if( g_rtdm_fd > 0 ) {
        g_com_status = (COM_STATUS_t *) mmap(NULL,sizeof(COM_STATUS_t),PROT_READ|PROT_WRITE,MAP_SHARED,g_rtdm_fd,0);
        if( g_com_status ) {
            mlockall(MCL_CURRENT|MCL_FUTURE);
            printf("initEthercat= success\n");
            g_com_status->cmd_set_link = 0;
            g_com_status->expect_num_of_io = config_io_num;
            g_com_status->expect_num_of_servo = config_axis_num;
            countForSetlink = 0;
            //memset(g_com_status->frames,sizeof(MAXPOS_FrameData)*EH_CMD_POOL_SIZE,0);
            //frames
        }
    }*/



}
