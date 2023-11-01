/************************************************
* 文件:     Ethercat通用版驱动
* 时间:     20210825
* 作者:     HONG-广州四合
* 描述:     连接顺序描述 当 link_type = 1 io_board_num -> config_io_num -> config_exio_num -> config_axis_num
*                                       当 link_type = 0 io_board_num -> config_axis_num -> config_io_num -> config_exio_num
*   config_io_num       //配置的主IO板数量，当board_type 为 0 适配的是ET1100，为 1 适配LAN9252或其它 ，当 io_type 为1 ,则主板为E68C型号，为0则为E68
*   config_exio_num   //扩展IO板数量 ， 只能连在主IO板后面
*   io_board_num        //小面板数量，只能作为第一个站
*   motor_chose          //电机配置类型
*   config_axis_num     //电机轴数量
*   20220323    1.新增第二路DA的发送
************************************************/



#include "rtapi.h"
#include "rtapi_app.h"
#include "hal.h"

#include<linux/errno.h>
#include<linux/string.h>
#include<linux/kernel.h>
#include<linux/major.h>
#include<linux/fs.h>
#include<linux/nfs_fs.h>
#include<linux/nfs_mount.h>
#include <bits/time.h>
#include <math.h>

#include <ecrt.h>
#include <mxml.h>
#include <fcntl.h>

#include <time.h>
#define MAX_BUS 3	/* max number of parports (EPP busses) */

#define	EPSILON		1e-20

#define MAXLINELEN 1024
#define MAXPARLINE  500

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

static int config_io_num = 1;
RTAPI_MP_INT(config_io_num, "config_io_num");

static int config_exio_num = 0;
RTAPI_MP_INT(config_exio_num, "config_exio_num");

static int io_board_num = 0;
RTAPI_MP_INT(io_board_num, "io_board_num");

static int config_axis_num = 0;
RTAPI_MP_INT(config_axis_num, "config_axis_num");

static char *config_file_path = NULL;
RTAPI_MP_STRING(config_file_path,"config_file_path");

static int motor_chose = 0;
RTAPI_MP_INT(motor_chose,"motor_chose");

static int board_type = 1;
RTAPI_MP_INT(board_type,"board_type");

static int io_type = 0;
RTAPI_MP_INT(io_type,"io_type");

static int link_type = 0;
RTAPI_MP_INT(link_type,"link_type");

#define TAIDA 0x1DD, 0x10305070
#define PLANE_IO 0xE00004D8,0x0000000D
#define AX58100_EVB_DIO_16  0x00000b95,0x00010200

static int piane_in0;
static int piane_in1;

static ec_master_info_t *master_info;
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};
static ec_domain_t *domain = NULL;
static ec_domain_state_t domain_state = {};

static ec_slave_config_t **io_sc = NULL;
static ec_slave_config_t **motion_sc = NULL;

static ec_slave_config_t **panel_sc = NULL;

// process data
static uint8_t *domain_pd = NULL;

static unsigned int sync_ref_counter = 0;

static int errortimecount = 1500;


static int ethercatVersion = 2;

static int boardvalid = 0;

#define FREQUENCY 500
#define CLOCK_TO_USE CLOCK_REALTIME
#define NSEC_PER_SEC (1000000000L)
#define PERIOD_NS (NSEC_PER_SEC / FREQUENCY)
#define DIFF_NS(A, B) (((B).tv_sec - (A).tv_sec) * NSEC_PER_SEC + \
    (B).tv_nsec - (A).tv_nsec)
#define TIMESPEC2NS(T) ((uint64_t) (T).tv_sec * NSEC_PER_SEC + (T).tv_nsec)

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

static void exchange_data(void *args,long peri);

#define ETH_MAX_SLAVES 40
#define IO_SLAVES_MAXNUM 16
#define IO_DOUTPUT_NUM 32
#define IO_DINPUT_NUM 32


//轴口板PDO - E68C
ec_pdo_entry_info_t slave_2_pdo_entries[] = {
    {0x7010, 0x01, 1}, /* LED 1 */
    {0x7010, 0x02, 1}, /* LED 2 */
    {0x7010, 0x03, 1}, /* LED 3 */
    {0x7010, 0x04, 1}, /* LED 4 */
    {0x7010, 0x05, 1}, /* LED 5 */
    {0x7010, 0x06, 1}, /* LED 6 */
    {0x7010, 0x07, 1}, /* LED 7 */
    {0x7010, 0x08, 1}, /* LED 8 */
    {0x0000, 0x00, 8},
    {0x7011, 0x00, 32}, /* SubIndex 000 */
    {0x7012, 0x00, 32}, /* SubIndex 000 */
    {0x7013, 0x00, 32}, /* SubIndex 000 */
    {0x7014, 0x01, 32}, /* SubIndex 001 */
    {0x7014, 0x02, 32}, /* SubIndex 002 */
    {0x7014, 0x03, 32}, /* SubIndex 003 */
    {0x7014, 0x04, 32}, /* SubIndex 004 */
    {0x7014, 0x05, 32}, /* SubIndex 005 */
    {0x7014, 0x06, 32}, /* SubIndex 006 */
    {0x7014, 0x07, 32}, /* SubIndex 007 */
    {0x7014, 0x08, 32}, /* SubIndex 008 */
    {0x6000, 0x01, 1}, /* Switch 1 */
    {0x6000, 0x02, 1}, /* Switch 2 */
    {0x6000, 0x03, 1}, /* Switch 3 */
    {0x6000, 0x04, 1}, /* Switch 4 */
    {0x6000, 0x05, 1}, /* Switch 5 */
    {0x6000, 0x06, 1}, /* Switch 6 */
    {0x6000, 0x07, 1}, /* Switch 7 */
    {0x6000, 0x08, 1}, /* Switch 8 */
    {0x0000, 0x00, 8}, /* Gap */
    {0x6001, 0x00, 32}, /* SubIndex 000 */
    {0x6002, 0x00, 32}, /* SubIndex 000 */
    {0x6003, 0x01, 32}, /*  */
    {0x6003, 0x02, 32}, /* */
    {0x6003, 0x03, 32}, /*  */
    {0x6003, 0x04, 32}, /*  */
    {0x6003, 0x05, 32}, /*  */
    {0x6003, 0x06, 32}, /* G */
    {0x6003, 0x07, 32}, /*  */
    {0x6003, 0x08, 32}, /*  */
};

ec_pdo_info_t slave_2_pdos[] = {
    {0x1601, 20, slave_2_pdo_entries + 0}, /* DO RxPDO-Map */
    {0x1a00, 19, slave_2_pdo_entries + 20}, /* DI TxPDO-Map */
};

ec_sync_info_t slave_2_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL, EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 0, NULL, EC_WD_DISABLE},
    {2, EC_DIR_OUTPUT, 1, slave_2_pdos + 0, EC_WD_ENABLE},
    {3, EC_DIR_INPUT, 1, slave_2_pdos + 1, EC_WD_DISABLE},
    {0xff}
};


//轴口板PDO - A6D6
ec_pdo_entry_info_t slave_3_pdo_entries[] = {
    {0x7011, 0x00, 32},
    {0x7012, 0x00, 32},
    {0x7013, 0x00, 32},
    {0x7014, 0x01, 32},
    {0x7014, 0x02, 32},
    {0x7014, 0x03, 32},
    {0x7014, 0x04, 32},
    {0x7014, 0x05, 32},
    {0x7014, 0x06, 32},
    {0x7014, 0x07, 32},
    {0x7014, 0x08, 32},
    {0x6001, 0x00, 32},
    {0x6002, 0x00, 32},
    {0x6003, 0x01, 32},
    {0x6003, 0x02, 32},
    {0x6003, 0x03, 32},
    {0x6003, 0x04, 32},
    {0x6003, 0x05, 32},
    {0x6003, 0x06, 32},
    {0x6003, 0x07, 32},
    {0x6003, 0x08, 32},
};

ec_pdo_info_t slave_3_pdos[] = {
    {0x1601, 11, slave_3_pdo_entries + 0},
    {0x1a00, 10, slave_3_pdo_entries + 11},
};

ec_sync_info_t slave_3_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL, EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 0, NULL, EC_WD_DISABLE},
    {2, EC_DIR_OUTPUT, 1, slave_3_pdos + 0, EC_WD_ENABLE},
    {3, EC_DIR_INPUT, 1, slave_3_pdos + 1, EC_WD_DISABLE},
    {0xff}
};


//小面板PDO
ec_pdo_entry_info_t slave_0_plane_entries[] = {
    {0x3001, 0x01, 8}, /* Input */
    {0x3001, 0x02, 8}, /* Input */
};

ec_pdo_info_t slave_0_planes[] = {
    {0x1600, 1, slave_0_plane_entries + 0}, /* Byte 0 */
    {0x1601, 1, slave_0_plane_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_0_plane[] = {
    {0, EC_DIR_INPUT, 1, slave_0_planes + 0, EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 1, slave_0_planes + 1, EC_WD_DISABLE},
    {0xff}
};

//扩展板PDO
ec_pdo_entry_info_t slave_exio_pdo_entries[] = {
    {0x3101, 0x01, 8}, /* Output */
    {0x3101, 0x02, 8}, /* Output */
    {0x3001, 0x01, 8}, /* Input */
    {0x3001, 0x02, 8}, /* Input */
};

ec_pdo_info_t slave_exio_pdos[] = {
    {0x1a00, 1, slave_exio_pdo_entries + 0}, /* Byte 0 */
    {0x1a01, 1, slave_exio_pdo_entries + 1}, /* Byte 1 */
    {0x1600, 1, slave_exio_pdo_entries + 2}, /* Byte 0 */
    {0x1601, 1, slave_exio_pdo_entries + 3}, /* Byte 1 */
};

ec_sync_info_t slave_exio_syncs[] = {
    {0, EC_DIR_OUTPUT, 1, slave_exio_pdos + 0, EC_WD_ENABLE},
    {1, EC_DIR_OUTPUT, 1, slave_exio_pdos + 1, EC_WD_ENABLE},
    {2, EC_DIR_INPUT, 2, slave_exio_pdos + 2, EC_WD_DISABLE},
    {0xff}
};


typedef struct {
    hal_bit_t *ready;        //ethercat就绪标志
    hal_u32_t *handleVal;

    hal_u32_t *maxtime;
    hal_u32_t *realruntime;


    hal_u32_t *testtime[10];

    hal_bit_t *read_data;
    hal_bit_t *write_data;

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

    hal_bit_t *enablech2;		/* enable pin for step generator */
    hal_float_t *velch2;		/* velocity command pin (mm) */
    hal_float_t scalech2;		/* parameter: scaling for vel to Hz (pulse/mm) */
    hal_float_t scaledirch2;          /* parameter: scale dir (pulse/mm) */

    hal_bit_t *start_probe;
    hal_bit_t *set_zero;
    hal_bit_t *index_enable;

} slave_input_t;

typedef struct {

    hal_bit_t enable;		  //保存使能命令，取上升沿
    hal_bit_t enablech2;		  //保存使能命令，取上升沿
    hal_bit_t start_probe;
    hal_bit_t set_zero;
    hal_bit_t index_enable;

    hal_float_t vel;
    hal_float_t velch2;
} last_slave_input_t;


typedef struct {

    hal_bit_t *alarm;
    hal_bit_t *linkstate;
    hal_bit_t *enabled;
    hal_bit_t *probe_done;
    hal_bit_t *alarmch2;
    hal_bit_t *enabledch2;
    hal_float_t *fbpositionch2;
    hal_float_t scalech2;

    hal_float_t *fbposition;
    hal_float_t scale;


    hal_u32_t *error_flag;
    hal_u32_t *al_state;
    hal_u32_t *errorcode;

} slave_output_t;

static hal_s32_t **thread_lat;

typedef struct {
    hal_u32_t data_6040;    //伺服控制字
    hal_s32_t data_607A;    //目标位置
    hal_s32_t data_60B0;    //位置偏置
    hal_u32_t data_6060;    //操作模式


    hal_u32_t data_6840;    //伺服控制字
    hal_s32_t data_687A;    //目标位置
    hal_s32_t data_68ff;    //位置偏置
    hal_u32_t data_6860;    //操作模式

    hal_u32_t data_probe_ctl;

} paremeter_output_t;

typedef struct {
    hal_u32_t data_6841;                //伺服状态字
    hal_s32_t data_6864;            //位置反馈
    hal_u32_t data_6861;            //模式显示
    hal_s32_t data_686c;            //位置反馈
    hal_u32_t data_6041;                //伺服状态字
    hal_s32_t data_6064;            //位置反馈
    hal_u32_t data_603F;            //错误码
    hal_u32_t data_6061;            //模式显示
    hal_u32_t data_60FD;        //数字输入
    hal_u32_t data_203F;        //数字输入
    hal_u32_t data_probe_status;
    hal_s32_t data_probe_pos;

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
} io_paremeter_output_t;

typedef struct {
    hal_u32_t data_6000;
    hal_u32_t data_6000_1;
    hal_u32_t data_6000_2;
    hal_u32_t data_6000_3;
    hal_u32_t data_6000_4;
    hal_u32_t data_6003[8];
    hal_float_t flitercount[IO_DINPUT_NUM];
} io_paremeter_input_t;

typedef struct {

    hal_bit_t *out[IO_DOUTPUT_NUM];


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

    hal_bit_t *in[IO_DINPUT_NUM];
    hal_float_t *infliter[IO_DINPUT_NUM];		/* 接收到的输入滤波 */

    hal_bit_t *svralm[8];          //驱动器报警信号
    hal_bit_t *svrindex[8];        //index信号

    hal_float_t *fbpulse[8];		/* 接收到的反馈值 */
    hal_float_t *fbspeed[8];		/* 接收到的命令值 */
    hal_float_t *record_fbpulse[8]; //记录的反馈值

    hal_u32_t *fpgaVer;         //FPGA传上来的版本
    hal_u32_t *mcuVer;         //FPGA传上来的版本
    hal_u32_t *checkBoard;         //FPGA传上来的版本
} io_slave_output_t;

/*
6003_0的bit31-bit28：FPGA版本；--------当前FPGA版本：A2
6003_1的bit31-bit28：MCU版本；---------当前MCU版本：A2
6003_2的bit31-bit28：通信出错信息，若为F，则说明3次通信还是出错
*/

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
    ec_sync_info_t *sync_info;
    ec_sdos_config_t *sdos_config;
    ec_pdo_info_t * pdo_info;
    int pdos_config_size;
    int sdos_config_size;

    unsigned int off_data_6040;
    unsigned int off_data_607A;
    unsigned int off_data_60B0;
    unsigned int off_data_6060;
    unsigned int off_data_6840;
    unsigned int off_data_687a;
    unsigned int off_data_6860;
    unsigned int off_data_68ff;
    unsigned int off_data_probe_status;
    unsigned int off_data_probe_ctl;
    unsigned int off_data_probe_pos;
    unsigned int off_data_6041;
    unsigned int off_data_6064;
    unsigned int off_data_6061;
    unsigned int off_data_603F;
    unsigned int off_data_60FD;
    unsigned int off_data_203F;
    unsigned int off_data_6841;
    unsigned int off_data_686c;
    unsigned int off_data_6861;
    unsigned int off_data_6864;

    unsigned int off_data_606C;
    unsigned int off_data_6077;
    unsigned int off_data_60FF;
    unsigned int off_data_6071;

    unsigned int off_data_60F4;
    unsigned int off_data_6078;
    unsigned int off_data_60FE;
    unsigned int off_data_3047;

    paremeter_output_t *par_output;
    paremeter_input_t *par_input;
    slave_input_t *slave_input;
    slave_output_t *slave_output;
    ec_slave_info_t *slaveinfo; //add by hong 20170512 to read the slave state


    last_slave_input_t last_slave_input;
    last_paremeter_input_t last_paremeter_input;


    int serv_on_process_flag;
    int serv_on_process_flag_ch2;
    int serv_on_process_count;
    int serv_on_process_count_ch2;

    int probe_process_flag;
    int probe_process_count;
    int isInited;           //是否已经初始化标志位
    int isInitedch2;           //是否已经初始化标志位
    int errorcount;
    int errorcountch2;
    int enablecount;
    int enablecountch2;
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
    unsigned int off_data_6003[8];//io_output
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

    unsigned int off_data_7014[8];//io_output

    unsigned int off_data_7010_1;//io_output
    unsigned int off_data_7010_2;//io_output
    unsigned int off_data_7010_3;//io_output
    unsigned int off_data_7010_4;//io_output
    unsigned int off_data_7010_5;//io_output
    unsigned int off_data_7010_6;//io_output
    unsigned int off_data_7010_7;//io_output
    unsigned int off_data_7010_8;//io_output

    ec_slave_info_t *slaveinfo; //add by hong


} io_slave_config_t;

static io_slave_config_t *io_slaves;


#define CNEXT_OLD_IO 0x9, 0x26483052
#define CNEXT_IO 0x9, 0x00009252
int init_io_slaves() {

    int i = 0;
    int iostartnum = 0;
    if(link_type == 0) {
        iostartnum = config_axis_num+io_board_num;
    } else {
        iostartnum = io_board_num;
    }

    for( i = 0; i < config_io_num+config_exio_num; i++) {

        if(io_slaves[i].valid == 1) continue;

        if(i < config_io_num) {
            if(board_type == 1){

                if ( !(io_sc[i] = ecrt_master_slave_config(master,0,iostartnum+i, CNEXT_IO) ) ) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "IO_Borad: Failed to get slave configuration.\n");
                    return -1;
                }

                if(io_type == 1) {

                    printf("now get io slave position:%d name:%s\n",io_slaves[i].slaveinfo->position,io_slaves[i].slaveinfo->name);
                    if(strcmp(io_slaves[i].slaveinfo->name,"CNEXT-E68") == 0) //默认值，没有扫描到
                        ecrt_slave_config_pdos(io_sc[i], EC_END, slave_2_syncs);
                    else
                        ecrt_slave_config_pdos(io_sc[i], EC_END, slave_3_syncs);
                    ec_pdo_entry_reg_t domain_regs[] = {

                        { 0,iostartnum+i,  CNEXT_IO, 0x7011, 0, &io_slaves[i].off_data_7011  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7012, 0, &io_slaves[i].off_data_7012  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7013, 0, &io_slaves[i].off_data_7013  },

                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 1, &io_slaves[i].off_data_7014[0]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 2, &io_slaves[i].off_data_7014[1]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 3, &io_slaves[i].off_data_7014[2]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 4, &io_slaves[i].off_data_7014[3]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 5, &io_slaves[i].off_data_7014[4]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 6, &io_slaves[i].off_data_7014[5]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 7, &io_slaves[i].off_data_7014[6]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x7014, 8, &io_slaves[i].off_data_7014[7]  },

                        { 0,iostartnum+i,  CNEXT_IO, 0x6001, 0, &io_slaves[i].off_data_6001  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6002, 0, &io_slaves[i].off_data_6002  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 1, &io_slaves[i].off_data_6003[0]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 2, &io_slaves[i].off_data_6003[1]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 3, &io_slaves[i].off_data_6003[2]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 4, &io_slaves[i].off_data_6003[3]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 5, &io_slaves[i].off_data_6003[4]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 6, &io_slaves[i].off_data_6003[5]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 7, &io_slaves[i].off_data_6003[6]  },
                        { 0,iostartnum+i,  CNEXT_IO, 0x6003, 8, &io_slaves[i].off_data_6003[7]  },
                        {}
                    };
                    if (ecrt_domain_reg_pdo_entry_list(domain, domain_regs)) {
                        rtapi_print_msg(RTAPI_MSG_ERR, _("IO_Borad: PDO entry registration failed!\n"));
                        return -1;
                    }
                } else if(io_type == 0) {
                    ecrt_slave_config_pdos(io_sc[i], EC_END, slave_3_syncs);              //如果是A6D6则完全适用
                    ec_pdo_entry_reg_t domain_regs[] = {
                                    { 0,iostartnum+i,  CNEXT_IO, 0x7011, 0, &io_slaves[i].off_data_7011  },
                                    { 0,iostartnum+i,  CNEXT_IO, 0x7012, 0, &io_slaves[i].off_data_7012  },
                                    { 0,iostartnum+i,  CNEXT_IO, 0x7013, 0, &io_slaves[i].off_data_7013  },

                                    { 0,iostartnum+i,  CNEXT_IO, 0x6001, 0, &io_slaves[i].off_data_6001  },
                                    { 0,iostartnum+i,  CNEXT_IO, 0x6002, 0, &io_slaves[i].off_data_6002  },
                                    {}
                                };
                        if (ecrt_domain_reg_pdo_entry_list(domain, domain_regs)) {
                            rtapi_print_msg(RTAPI_MSG_ERR, _("IO_Borad: PDO entry registration failed!\n"));

                            return -1;
                        }
                }
            }
        } else {
            if ( !(io_sc[i] = ecrt_master_slave_config(master,0,iostartnum+i, AX58100_EVB_DIO_16) ) ) {
                rtapi_print_msg(RTAPI_MSG_ERR, "EX_IO_Borad: Failed to get slave configuration.\n");
                return -1;
            }
           ecrt_slave_config_pdos(io_sc[i], EC_END, slave_exio_syncs);

          io_slaves[i].off_data_7012 = ecrt_slave_config_reg_pdo_entry(io_sc[i], 0x3101, 1, domain, NULL);
          if ( io_slaves[i].off_data_7012 < 0)
              return -1;
          io_slaves[i].off_data_7013 = ecrt_slave_config_reg_pdo_entry(io_sc[i], 0x3101, 2, domain, NULL);
          if ( io_slaves[i].off_data_7013 < 0)
              return -1;

          io_slaves[i].off_data_6001 = ecrt_slave_config_reg_pdo_entry(io_sc[i], 0x3001, 1, domain, NULL);
          if (io_slaves[i].off_data_6001 <0)
              return -1;
          io_slaves[i].off_data_6002 = ecrt_slave_config_reg_pdo_entry(io_sc[i], 0x3001, 2, domain, NULL);
          if (io_slaves[i].off_data_6002 <0)
              return -1;
        }
        io_slaves[i].valid = 1;


//                    ecrt_slave_config_dc(io_sc[i], 0x300, 2000000, 4400000, 0, 0);
    }

    return 0;

}

int init_io_board() {
    if(boardvalid == 1)     //已经初始化成功
        return 0;
    int iostartnum = 0;

    if ( !(panel_sc = ecrt_master_slave_config(master, 0,iostartnum, PLANE_IO) ) ) {
        handleError("小面板配置获取失败");
        return -1;
    }
    if (ecrt_slave_config_pdos(panel_sc, EC_END, slave_0_plane)) {
        handleError("小面板配置PDO失败");
    }
    piane_in0 = ecrt_slave_config_reg_pdo_entry(panel_sc, 0x3001, 1, domain, NULL);
    if (piane_in0 < 0)
        return -1;

    piane_in1 = ecrt_slave_config_reg_pdo_entry(panel_sc, 0x3001, 2, domain, NULL);
    if (piane_in1 < 0)
        return -1;

    boardvalid = 1;
}


/********************XML Files**************************/

inline void checkName( char *name) {
    int i,size;
    size = strlen(name);
    for( i = 0; i < size; i++ ) {
        if(name[i] == '#')
            name[i] = '0';
    }
}

static int getChildNodeCount(mxml_node_t *topTree , mxml_node_t * parentNode , const char* name)
{

//    const char	*text;			//Text value
    mxml_node_t * findNode;
    int count = 0;
    if( name == NULL)
    {

    }
    else
    {
        if(parentNode == NULL)
        {
        }
        else
        {
            if ((findNode = mxmlFindElement(parentNode, topTree,name, NULL, NULL,
                                        MXML_DESCEND)) == NULL)
            {
              fprintf(stderr, " Unable to find  < %s > element in XML tree!\n", name );
              return (1);
            }
            count ++;
           while(findNode != NULL)
           {
               if ((findNode = mxmlFindElement(findNode, topTree,name, NULL, NULL,
                                           MXML_NO_DESCEND)) == NULL)
               {
                 //fprintf(stderr, " Unable to find  < %s > element in XML tree!\n", name );
                 break;
               }
               count ++;
           }
        }

    }
    return count;

}

static int MXml_read(slave_config_t *slave,const char * file) {

    char strResult[100];
    char *end_ptr;
    int	i;                          /* Looping var */
    FILE *fp;                       /* File to read */
    mxml_node_t *tree, *node;		/* Node which should be in test.xml */

    tree = mxmlLoadString(NULL, file, MXML_NO_CALLBACK);
    if ( (fp = fopen(file, "rb") ) == NULL) {
        char *tmp;
        sprintf(tmp,"没有对应的XML配置文件:%s",file);
        handleError(tmp);
        printf("------No Such XML File %s\n",file);
        return (1);
    } else {
        tree = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
        fclose(fp);
    }

     if (tree) {

         //-----------------------------------get slave info---------------------------------
         const char	*text;			/* Text value */
         if ( (node = mxmlFindPath(tree, "EtherCATInfo/Vendor/Id")) == NULL) {
           fputs("Unable to find EtherCATInfo/Vendor/Id element in XML tree!\n", stderr);
           mxmlDelete(tree);
           return (1);
         }

         if (node->type != MXML_TEXT) {
           fputs("No child node of EtherCATInfo/Vendor/Id!\n", stderr);
           mxmlDelete(tree);
           return (1);
         }

         if( (text = mxmlGetText(node, NULL)) == NULL) {
           mxmlDelete(tree);
           return (1);
         }
         strncpy(strResult,text,sizeof(strResult));
         checkName(strResult);
         slave->vendor_id = strtoul(strResult,&end_ptr,16);
         printf("vendor_id = %x\n",slave->vendor_id);

         //get ProductCode
         if ((node = mxmlFindPath(tree,"EtherCATInfo/Descriptions")) == NULL) {
            printf("-----------get Node EtherCATInfo/Descriptions Fail-------\n");
            return (-1);
         }

         //get Type ProductCode
         if ((node = mxmlFindElement(node,tree,"Type",NULL,NULL,MXML_DESCEND)) == NULL) {
            printf("-----------get Node EtherCATInfo/Descriptions/Type Fail-------\n");
            return (-1);
         }

         if ((text = mxmlElementGetAttr(node,"ProductCode")) == NULL)
            text = node->value.element.attrs->value;

         strncpy(strResult,text,sizeof(strResult));
         checkName(strResult);
         slave->product_code = strtoul(strResult,&end_ptr,16);

         printf("productCode = %x\n",slave->product_code);

         //get activate_word
         if((node = mxmlFindPath(tree,"EtherCATInfo/Descriptions/Dc/OpMode/AssignActivate")) == NULL) {

            fputs("Unable to find EtherCATInfo/Descriptions/Devices/Device/Dc/OpMode/AssignActivate element in XML tree!\n", stderr);
            mxmlDelete(tree);
            return (1);

         }
         if (node->type != MXML_TEXT) {

            fputs("No child node of EtherCATInfo/Descriptions/Dc/OpMode/AssignActivate!\n", stderr);
            mxmlDelete(tree);
            return (1);

         }
         if ((text = mxmlGetText(node, NULL)) == NULL) {

            printf("EtherCATInfo/Descriptions/Dc/OpMode/AssignActivate Is Not Text\n");
            mxmlDelete(tree);
            return (1);

         }
         strncpy(strResult,text,sizeof(strResult));
         checkName(strResult);
         slave->activate_word = strtoul(strResult,&end_ptr,16);

         printf("activateWord = %x\n",slave->activate_word);

        //-------------------------------------------get shifttime--------------------------------------
         if((node = mxmlFindPath(tree,"EtherCATInfo/Descriptions/Dc/OpMode/ShiftTimeSync0")) == NULL) {
            fputs("Unable to find EtherCATInfo/Descriptions/Devices/Device/Dc/OpMode/ShiftTimeSync0 element in XML tree!\n", stderr);
            mxmlDelete(tree);
            return (1);

         }
         if (node->type != MXML_TEXT) {
            fputs("No child node of EtherCATInfo/Descriptions/Dc/OpMode/ShiftTimeSync0!\n", stderr);
            mxmlDelete(tree);
            return (1);

         }
         if ((text = mxmlGetText(node, NULL)) == NULL) {
            printf("EtherCATInfo/Descriptions/Dc/OpMode/ShiftTimeSync0 Is Not Text\n");
            mxmlDelete(tree);
            return (1);
         }
         strncpy(strResult,text,sizeof(strResult));
         checkName(strResult);
         slave->shifttime = strtoul(strResult,&end_ptr,16);

         printf("ShiftTimeSync0 = %x\n",slave->shifttime);

        //-----------------------------------start get Entry data------------------------------------
        mxml_node_t * descriptionsNode;
        mxml_node_t *txPdoNode;
        mxml_node_t *rxPdoNode;
        int txPdoCount;
        int rxPdoCount;
        int tx_i,rx_i;

        if((descriptionsNode = mxmlFindPath(tree,"EtherCATInfo/Descriptions")) == NULL) {
           fputs("Unable to find EtherCATInfo/Descriptions\n", stderr);
           return (1);
        }

        txPdoCount = getChildNodeCount(tree,descriptionsNode,"TxPdo");
        rxPdoCount = getChildNodeCount(tree,descriptionsNode,"RxPdo");

        printf("txPdoCount and rxPdoCount = %d %d\n",txPdoCount,rxPdoCount);

        ec_pdo_info_t * pdo_info = malloc( sizeof(ec_pdo_info_t)*(txPdoCount + rxPdoCount) );


        //先读RXPDO，再读TXPDO
        for( rx_i = 0; rx_i < rxPdoCount; rx_i++ ) {

            const char	*text;
            mxml_node_t *valueNode;
            if( rx_i <= 0 ) {
                if ((rxPdoNode = mxmlFindElement(descriptionsNode,tree,"RxPdo", NULL, NULL,MXML_DESCEND)) == NULL) {
                    printf("Find descriptionsNode/RxPdo0 Fail\n");
                    return -1;
                }
            } else {
                if ((rxPdoNode = mxmlFindElement(rxPdoNode,tree,"RxPdo", NULL, NULL, MXML_NO_DESCEND)) == NULL) {
                    printf("Find descriptionsNode/RxPdo1 Fail\n");
                    return -1;
                }
            }


            mxml_node_t *RxPdoIndexNode;
            if ((RxPdoIndexNode = mxmlFindElement(rxPdoNode,tree,"Index", NULL, NULL,MXML_DESCEND)) == NULL) {
                return -1;
            }
            if ((text = mxmlGetText(RxPdoIndexNode, NULL)) == NULL ) {
                return -1;
            }


            strncpy(strResult,text,sizeof(strResult));
            checkName(strResult);
            pdo_info[rx_i].index = strtoul(strResult,&end_ptr,16);

            int EntryCount = getChildNodeCount(tree,rxPdoNode ,"Entry" );

            printf("now I ge re entrycount:%d  %s\n",EntryCount,strResult);
            if( EntryCount < 1 )
                return -1;

            pdo_info[rx_i].n_entries = EntryCount;

            ec_pdo_entry_info_t *entries_rx = malloc(sizeof(ec_pdo_entry_info_t)*EntryCount);
            pdo_info[rx_i].entries = entries_rx;

            mxml_node_t *findNode;

            for( i = 0 ;i < EntryCount; i++ ) {

                //find Entry
                if( i <1 ) {
                    if ((findNode = mxmlFindElement(rxPdoNode,tree,"Entry", NULL, NULL,MXML_DESCEND)) == NULL) {
                       return (-1);
                    }
                } else {
                    if ((findNode = mxmlFindElement(findNode, tree,"Entry", NULL, NULL,MXML_NO_DESCEND)) == NULL) {
                      return (-1);
                    }
                }


                //get Entry Child Node
                if( (valueNode = mxmlFindElement(findNode, tree,"Index", NULL, NULL,MXML_DESCEND)) == NULL) {
                    mxmlDelete(tree);
                    break;
                }
                if((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                    break;
                }

                strncpy(strResult,text,sizeof(strResult));
                checkName(strResult);
                entries_rx[i].index = strtoul(strResult,&end_ptr,16);

                if ((valueNode = mxmlFindElement(findNode, tree,"SubIndex", NULL, NULL,MXML_DESCEND)) == NULL) {
                   mxmlDelete(tree);
                   break;
                }
                if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                   break;
                }

                printf("now Get Rx: text:%s %s\n",strResult,text);
                strncpy(strResult,text,sizeof(strResult));
                checkName(strResult);
                entries_rx[i].subindex = strtoul(strResult,&end_ptr,16);

                if ((valueNode = mxmlFindElement(findNode, tree,"BitLen", NULL, NULL,MXML_DESCEND)) == NULL) {
                    mxmlDelete(tree);
                    break;
                }
                if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                    break;
                }

                strncpy(strResult,text,sizeof(strResult));
                checkName(strResult);
                entries_rx[i].bit_length = strtoul(strResult,&end_ptr,10);

           }


        }

        for( tx_i = rxPdoCount; tx_i < (txPdoCount + rxPdoCount); tx_i++ ) {
            printf("now I get txc_i:%d\n",tx_i);
            const char	*text;
            mxml_node_t *valueNode;
            if( tx_i <= rxPdoCount ) {
                if ((txPdoNode = mxmlFindElement(descriptionsNode,tree,"TxPdo", NULL, NULL,MXML_DESCEND)) == NULL) {
                  return -1;
                 }

            } else {
                if ((txPdoNode = mxmlFindElement(txPdoNode,tree,"TxPdo", NULL, NULL, MXML_NO_DESCEND)) == NULL) {
                  return -1;
                }
            }

            mxml_node_t *TxPdoIndexNode;
            if ((TxPdoIndexNode = mxmlFindElement(txPdoNode,tree,"Index",NULL,NULL,MXML_DESCEND)) == NULL) {
                return -1;
            }
            if ((text = mxmlGetText(TxPdoIndexNode, NULL)) == NULL ) {
                return -1;
            }

            strncpy(strResult,text,sizeof(strResult));
            checkName(strResult);
            pdo_info[tx_i].index =  strtoul(strResult,&end_ptr,16);

            int EntryCount = getChildNodeCount(tree,txPdoNode,"Entry");

            printf("now I ge tx entrycount:%d  %s\n",EntryCount,strResult);
            if(EntryCount <1)
                 return -1;

             pdo_info[tx_i].n_entries = EntryCount;

             ec_pdo_entry_info_t *entries = malloc( sizeof(ec_pdo_entry_info_t)*EntryCount );//new ec_pdo_entry_info_t[EntryCount];
             pdo_info[tx_i].entries = entries;

             mxml_node_t *findNode;
             for( i = 0 ;i< EntryCount ;i++) {

                 //find Entry
                 if( i <1 ) {
                     if ((findNode = mxmlFindElement(txPdoNode,tree,"Entry", NULL, NULL,MXML_DESCEND)) == NULL) {
                       return (-1);
                     }
                 } else {
                     if ((findNode = mxmlFindElement(findNode,tree,"Entry", NULL, NULL, MXML_NO_DESCEND)) == NULL) {
                       return (-1);
                     }
                 }

                 //get Entry Child Node
                 if ((valueNode = mxmlFindElement(findNode, tree,"Index", NULL, NULL,
                                             MXML_DESCEND)) == NULL) {
                   mxmlDelete(tree);
                   break;
                 }
                  if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                    break;
                  }

                  strncpy(strResult,text,sizeof(strResult));
                  checkName(strResult);
                  entries[i].index = strtoul(strResult,&end_ptr,16);

                  if ((valueNode = mxmlFindElement(findNode,tree,"SubIndex", NULL, NULL,
                                              MXML_DESCEND)) == NULL) {
                    mxmlDelete(tree);
                    break;
                  }
                   if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                     break;
                   }

                   printf("now Get Tx: text:%s %s\n",strResult,text);
                   strncpy(strResult,text,sizeof(strResult));
                   checkName(strResult);
                   entries[i].subindex = strtoul(strResult,&end_ptr,16);

                   if ((valueNode = mxmlFindElement(findNode, tree,"BitLen", NULL, NULL,
                                               MXML_DESCEND)) == NULL) {
                     mxmlDelete(tree);
                     break;
                   }
                    if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                      break;
                    }


                    strncpy(strResult,text,sizeof(strResult));
                    checkName(strResult);
                    entries[i].bit_length = strtoul(strResult,&end_ptr,10);

             }


        }


        ec_sync_info_t *sync_info = malloc( sizeof(ec_sync_info_t) * (5) );

        //txPdoCount + rxPdoCount

        sync_info[0].index = 0;
        sync_info[0].dir = EC_DIR_OUTPUT;
        sync_info[0].n_pdos = 0;
        sync_info[0].pdos = NULL;
        sync_info[0].watchdog_mode = EC_WD_DISABLE;

        sync_info[1].index = 1;
        sync_info[1].dir = EC_DIR_INPUT;
        sync_info[1].n_pdos = 0;
        sync_info[1].pdos = NULL;
        sync_info[1].watchdog_mode = EC_WD_DISABLE;

        sync_info[2].index = 2;
        sync_info[2].dir = EC_DIR_OUTPUT;
        sync_info[2].n_pdos = rxPdoCount;
        sync_info[2].pdos = &pdo_info[0];
        sync_info[2].watchdog_mode = EC_WD_DEFAULT;

        sync_info[3].index = 3;
        sync_info[3].dir = EC_DIR_INPUT;
        sync_info[3].n_pdos = txPdoCount;
        sync_info[3].pdos = &pdo_info[rxPdoCount];
        sync_info[3].watchdog_mode = EC_WD_DEFAULT;


        sync_info[4].index = 0xff;

        slave->sync_info = sync_info;
//        slave->sync_info = slave_0_syncs;
        slave->pdos_config_size = txPdoCount + rxPdoCount;
        slave->pdo_info = pdo_info;
        slave->valid = 1;

        printf("Get txPdoCount and rxPdoCount = %d %d\n",txPdoCount,rxPdoCount);

        for( i = 0; i < txPdoCount + rxPdoCount; i++) {
            printf("index = %X n_entries = %d\n",pdo_info[i].index,pdo_info[i].n_entries);
            ec_pdo_entry_info_t *entries = pdo_info[i].entries;
            int num_entry = pdo_info[i].n_entries;
            int j;
            for( j = 0; j<num_entry; j++) {
                printf("    index  %X sub %X len %d\n",entries[j].index,entries[j].subindex,entries[j].bit_length);
            }
        }


         //SDO
         if((node = mxmlFindPath(tree,"EtherCATInfo/Descriptions/Sdo")) == NULL) {
           fputs("Unable to find EtherCATInfo/Descriptions/Sdo element in XML tree!\n", stderr);
           mxmlDelete(tree);
           return (1);
         }

         int EntryCount = getChildNodeCount(tree,node ,"Entry" );
         if( EntryCount < 1 )
             return -1;

         ec_sdos_config_t *sdos_cfg = malloc(sizeof(ec_sdos_config_t)*EntryCount);
         slave->sdos_config_size = EntryCount;
         slave->sdos_config =  sdos_cfg;
         if( EntryCount<=0 ) slave->sdos_config = NULL;


        for( i = 0 ;i< EntryCount ;i++) {

          const char	*text;
          mxml_node_t *valueNode;
          mxml_node_t *findNode;

          //find Entry
          if( i <1 ) {
              if ((findNode = mxmlFindElement(node, tree,"Entry", NULL, NULL,MXML_DESCEND)) == NULL) {
                 return (-1);
              }
          } else {
              if ((findNode = mxmlFindElement(findNode, tree,"Entry", NULL, NULL,MXML_NO_DESCEND)) == NULL) {
                return (-1);
              }
          }

          //get Entry Child Node
          if ((valueNode = mxmlFindElement(findNode, tree,"Index", NULL, NULL,
                                      MXML_DESCEND)) == NULL) {
            mxmlDelete(tree);
            break;
          }
           if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
             break;
           }

           strncpy(strResult,text,sizeof(strResult));
           checkName(strResult);
           sdos_cfg[i].index = strtoul(strResult,&end_ptr,16);

           if ((valueNode = mxmlFindElement(findNode, tree,"SubIndex", NULL, NULL,
                                       MXML_DESCEND)) == NULL) {
             mxmlDelete(tree);
             break;
           }
            if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
              break;
            }

            strncpy(strResult,text,sizeof(strResult));
            checkName(strResult);
            sdos_cfg[i].subindex = strtoul(strResult,&end_ptr,16);

            if ((valueNode = mxmlFindElement(findNode, tree,"BitLen", NULL, NULL,
                                        MXML_DESCEND)) == NULL) {
              mxmlDelete(tree);
              break;
            }
             if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
               break;
             }
             strncpy(strResult,text,sizeof(strResult));
             checkName(strResult);
             sdos_cfg[i].bit_length = strtoul(strResult,&end_ptr,10);

             if ((valueNode = mxmlFindElement(findNode, tree,"Value", NULL, NULL,
                                         MXML_DESCEND)) == NULL) {
               mxmlDelete(tree);
               break;
             }
              if ((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                break;
              }
              strncpy(strResult,text,sizeof(strResult));
              checkName(strResult);
              sdos_cfg[i].value = strtoul(strResult,&end_ptr,10);

        }

        for( i = 0; i < slave->sdos_config_size; i++) {
            printf("sdo index = %X sub = %d bitlen %d value %d\n",\
            slave->sdos_config[i].index,\
            slave->sdos_config[i].subindex,\
            slave->sdos_config[i].bit_length,\
            slave->sdos_config[i].value);
        }


        mxmlDelete(tree);

     }
     return 0;

}



static int initSlaveDomainRegs(ec_domain_t *domain,slave_config_t *t) {

    int ret = 0;

    t->off_data_6040 = -1;
    t->off_data_607A = -1;
    t->off_data_60B0 = -1;
    t->off_data_6060 = -1;
    t->off_data_6041 = -1;
    t->off_data_6064 = -1;
    t->off_data_6061 = -1;
    t->off_data_603F = -1;
    t->off_data_60FD = -1;
    t->off_data_203F = -1;
    t->off_data_606C = -1;
    t->off_data_6077 = -1;
    t->off_data_60FF = -1;
    t->off_data_6071 = -1;
    t->off_data_probe_status = -1;
    t->off_data_probe_ctl = -1;
    t->off_data_probe_pos = -1;


    t->off_data_60F4 = -1;
    t->off_data_6078 = -1;
    t->off_data_60FE = -1;
    t->off_data_3047 = -1;

    t->off_data_6840 = -1;
    t->off_data_6841 = -1;
    t->off_data_686c = -1;
    t->off_data_68ff = -1;
    t->off_data_6860 = -1;
    t->off_data_6861 = -1;
    t->off_data_687a = -1;
    t->off_data_6864 = -1;

    ec_pdo_entry_reg_t *domain_regs;
    ec_pdo_info_t *pdo_info = t->pdo_info;
    int i;
    int count = 0;

    if( pdo_info == NULL )
        return 1;

    for( i = 0; i < t->pdos_config_size; i++) {
        int num_entry = pdo_info[i].n_entries;
        int j;
        for( j = 0; j<num_entry; j++)
            count ++;
    }
    domain_regs = malloc( sizeof(ec_pdo_entry_reg_t)*(count + 1) );

    count = 0;
    for( i = 0; i < t->pdos_config_size; i++) {

        ec_pdo_entry_info_t *entries = pdo_info[i].entries;
        int num_entry = pdo_info[i].n_entries;
        int j;
        for( j = 0; j<num_entry; j++) {

        //    printf("Reg %d  %d index  %X sub %X len %d  vendor:%d  product:%d\n", t->alias,t->position,entries[j].index,entries[j].subindex,entries[j].bit_length,t->vendor_id,t->product_code);

            domain_regs[count].alias = t->alias;
            domain_regs[count].position = t->position;
            domain_regs[count].vendor_id = t->vendor_id;
            domain_regs[count].product_code = t->product_code;
            domain_regs[count].index = entries[j].index;
            domain_regs[count].subindex = entries[j].subindex;
            domain_regs[count].bit_position = NULL;

            if( entries[j].index == 0x6040 )
                domain_regs[count].offset = &t->off_data_6040;
            else if(  entries[j].index == 0x607A )
                domain_regs[count].offset = &t->off_data_607A;
            else if(  entries[j].index == 0x60B0 )
                domain_regs[count].offset = &t->off_data_60B0;
            else if(  entries[j].index == 0x6060 )
                domain_regs[count].offset = &t->off_data_6060;
            else if(  entries[j].index == 0x6041 )
                domain_regs[count].offset = &t->off_data_6041;
            else if(  entries[j].index == 0x6064 )
                domain_regs[count].offset = &t->off_data_6064;
            else if(  entries[j].index == 0x6061 )
                domain_regs[count].offset = &t->off_data_6061;
            else if(  entries[j].index == 0x603F )
                domain_regs[count].offset = &t->off_data_603F;
            else if(  entries[j].index == 0x60FD )
                domain_regs[count].offset = &t->off_data_60FD;
            else if(  entries[j].index == 0x203F )
                domain_regs[count].offset = &t->off_data_203F;
            else if(  entries[j].index == 0x60B9 )
                domain_regs[count].offset = &t->off_data_probe_status;
            else if(  entries[j].index == 0x60B8 )
                domain_regs[count].offset = &t->off_data_probe_ctl;
            else if(  entries[j].index == 0x60BA )
                domain_regs[count].offset = &t->off_data_probe_pos;
            else if(  entries[j].index == 0x606C )
                domain_regs[count].offset = &t->off_data_606C;
            else if(  entries[j].index == 0x6077 )
                domain_regs[count].offset = &t->off_data_6077;
            else if(  entries[j].index == 0x60FF )
                domain_regs[count].offset = &t->off_data_60FF;
            else if(  entries[j].index == 0x6071 )
                domain_regs[count].offset = &t->off_data_6071;
            else if(  entries[j].index == 0x60FE )
                domain_regs[count].offset = &t->off_data_60FE;
            else if(  entries[j].index == 0x6078 )
                domain_regs[count].offset = &t->off_data_6078;
            else if(  entries[j].index == 0x60F4 )
                domain_regs[count].offset = &t->off_data_60F4;
            else if(  entries[j].index == 0x3047 )
                domain_regs[count].offset = &t->off_data_3047;
            else if(  entries[j].index == 0x6840 )
                domain_regs[count].offset = &t->off_data_6840;
            else if(  entries[j].index == 0x6841 )
                domain_regs[count].offset = &t->off_data_6841;
            else if(  entries[j].index == 0x6860 )
                domain_regs[count].offset = &t->off_data_6860;
            else if(  entries[j].index == 0x6861 )
                domain_regs[count].offset = &t->off_data_6861;
            else if(  entries[j].index == 0x6864 )
                domain_regs[count].offset = &t->off_data_6864;
            else if(  entries[j].index == 0x687a )
                domain_regs[count].offset = &t->off_data_687a;
            else if(  entries[j].index == 0x686c )
                domain_regs[count].offset = &t->off_data_686c;
            else if(  entries[j].index == 0x68ff )
                domain_regs[count].offset = &t->off_data_68ff;




            count ++;

        }
    }

    domain_regs[count].alias = 0;
    domain_regs[count].position = 0;
    domain_regs[count].vendor_id = 0;
    domain_regs[count].product_code = 0;
    domain_regs[count].index = 0;
    domain_regs[count].subindex = 0;
    domain_regs[count].bit_position = NULL;
    domain_regs[count].offset = NULL;



    ret = ecrt_domain_reg_pdo_entry_list(domain, domain_regs);

    return ret;

}

static void initSdosConfig(ec_master_t *master,slave_config_t *t) {

    int i;
    ec_sdos_config_t *s;
    uint8_t data_8;
    uint16_t data_16;
    uint32_t data_32;
    uint32_t ret;
    if( t->sdos_config == NULL ) return;

    for( i = 0; i < t->sdos_config_size; i++ ) {

        s = &( t->sdos_config[i] );

        if( s->bit_length == 8 ) {
            data_8 = s->value;
            ecrt_master_sdo_download(master,t->position,\
                                 s->index,s->subindex,(uint8_t*)&data_8,sizeof(data_8),&ret);
        } else if( s->bit_length == 16 ) {
            data_16 = s->value;
            ecrt_master_sdo_download(master,t->position,\
                             s->index,s->subindex,(uint8_t*)&data_16,sizeof(data_16),&ret);
        } else if( s->bit_length == 32 ) {
            data_32 = s->value;
            ecrt_master_sdo_download(master,t->position,\
                             s->index,s->subindex,(uint8_t*)&data_32,sizeof(data_32),&ret);
        }

    }

}

static int comp_id;		/* component ID */

static void initBusState() {

    master_state.al_states = 0;
    master_state.link_up = 0;
    domain_state.wc_state = EC_WC_ZERO;

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

        t->last_slave_input.enablech2 = 0;
        t->last_slave_input.velch2 = 0;
        t->last_slave_input.enable = 0;
        t->last_slave_input.vel = 0;
        t->serv_on_process_flag = 0;
        t->serv_on_process_flag_ch2 = 0;
        t->serv_on_process_count = 0;
        t->serv_on_process_count_ch2 = 0;

        t->off_data_6040= 0;
        t->off_data_6840= 0;
        t->off_data_607A = 0;
        t->off_data_687a = 0;
        t->off_data_60B0 = 0;
        t->off_data_6060 = 0;
        t->off_data_6041 = 0;
        t->off_data_6841 = 0;
        t->off_data_6064 = 0;
        t->off_data_6864 = 0;
        t->off_data_6061 = 0;
        t->off_data_603F = 0;
        t->off_data_60FD = 0;
        t->off_data_203F = 0;
        t->off_data_probe_status = 0;
        t->off_data_probe_ctl = 0;
        t->sdos_config = NULL;
        t->sdos_config_size = 0;

        t->pdo_info = NULL;
        t->pdos_config_size = 0;
        t->isInited = 0;
        t->isInitedch2 = 0;
        t->errorcount =  0;
        t->errorcountch2 =  0;
        t->enablecount = 0;
        t->enablecountch2 = 0;

    }

    for( i = 0; i < config_io_num+config_exio_num; i++) {
        t = &io_slaves[i];
        t->valid = 0;
    }

    boardvalid = 0;
}

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

    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        motion_slaves[i].slave_input = hal_malloc(sizeof(slave_input_t));
        motion_slaves[i].slave_output = hal_malloc(sizeof(slave_output_t));
        motion_slaves[i].slaveinfo = hal_malloc(sizeof(ec_slave_info_t));

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

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.velocitych2",i);
        retval = hal_pin_float_new(buf, HAL_IN, &(input->velch2), comp_id);
        if (retval != 0) {
            return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.enable",i);
        retval = hal_pin_bit_new(buf, HAL_IN, &(input->enable), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.enablech2",i);
        retval = hal_pin_bit_new(buf, HAL_IN, &(input->enablech2), comp_id);
        if (retval != 0) {
            return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.start_probe",i);
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
        /**************************通道二参数*************************************/
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.scalech2",i);
                retval = hal_param_float_new(buf, HAL_RW, &(input->scalech2), comp_id);
                if (retval != 0) {
                    return -1;
                }
                input->scalech2 = 1;
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.scaledirch2",i);
                retval = hal_param_float_new(buf, HAL_RW, &(input->scaledirch2), comp_id);
                if (retval != 0) {
                    return -1;
                }
                input->scaledirch2 = 1;


                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.alarmch2",i);
                retval = hal_pin_bit_new(buf, HAL_OUT, &(output->alarmch2), comp_id);
                if (retval != 0) {
                    return -1;
                }

                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.outscalech2",i);
                retval = hal_param_float_new(buf, HAL_RW, &(output->scalech2), comp_id);
                if (retval != 0) {
                    return -1;
                }
                output->scalech2 = 1;

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
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.enabledch2",i);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(output->enabledch2), comp_id);
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

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.fbpositionch2",i);
        retval = hal_pin_float_new(buf, HAL_OUT, &(output->fbpositionch2), comp_id);
        if (retval != 0) {
            return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.errorcode",i);
        retval = hal_pin_s32_new(buf, HAL_IN, &(output->errorcode), comp_id);
        if (retval != 0) {
            return -1;
        }

    }


    for( i = 0; i < config_io_num+config_exio_num; i++ ) {

        io_slaves[i].slave_output = hal_malloc(sizeof(io_slave_output_t));
        io_slaves[i].slaveinfo = hal_malloc(sizeof(ec_slave_info_t));
        for(  n = 0;  n < IO_DINPUT_NUM;  n++ ) {

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%d.din.%02d.in",i,n);
            retval = hal_pin_bit_new(buf, HAL_OUT, &(io_slaves[i].slave_output->in[n]), comp_id);
            if (retval != 0) {
                return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%d.din.%02d.infliter",i,n);
            retval = hal_pin_float_new(buf, HAL_IN, &(io_slaves[i].slave_output->infliter[n]), comp_id);
            if (retval != 0) {
                return -1;
            }

            io_slaves[i].par_input->flitercount[n] = 0;
            *io_slaves[i].slave_output->infliter[n] = 5;

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

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6840", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_6840), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_607A", i);
        rv = hal_param_s32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_607A), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_687A", i);
        rv = hal_param_s32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_687A), comp_id);
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
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6860", i);
        rv = hal_param_u32_new(buf, HAL_RW,
                &(motion_slaves[i].par_output->data_6860), comp_id);
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
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6841", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6841), comp_id);
        if (rv != 0) {
             return -1;
        }


        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6064", i);
        rv = hal_param_s32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6064), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6864", i);
        rv = hal_param_s32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6864), comp_id);
        if (rv != 0) {
             return -1;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6061", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6061), comp_id);
        if (rv != 0) {
             return -1;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.data_6861", i);
        rv = hal_param_u32_new(buf, HAL_RO,
                &(motion_slaves[i].par_input->data_6861), comp_id);
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


        int j=0;
        for(j=0;j<8;j++) {
            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_7014_%d",i,j);
            rv = hal_param_u32_new(buf, HAL_RW,
                                   &(io_slaves[i].par_output->data_7014[j]), comp_id);
            if (rv != 0) {
                 return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.data_6003_%d",i,j);
            rv = hal_param_u32_new(buf, HAL_RO,
                                   &(io_slaves[i].par_input->data_6003[j]), comp_id);
            if (rv != 0) {
                 return -1;
            }

            rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.io.%02d.axis_type_%d",i,j);
            rv = hal_param_u32_new(buf, HAL_RW,
                                   &(io_slaves[i].par_output->axis_type[j]), comp_id);
            if (rv != 0) {
                 return -1;
            }
            io_slaves[i].par_output->axis_type[j] = 0;

        }

    }
    return 0;


}

int inline testBit(unsigned int data,int bit) {
    unsigned int tmp = data & ((unsigned int )( 1 << bit));
    if(tmp == 0)
        return 0;
    else
        return 1;
}
#define setBit(data,bit) ( (data) |= (1 << (bit) ))
#define clearBit(data,bit) ( (data) &= (~(1 << (bit) ) ))

static void handleExchangeData() {

    int i,n,tmp_int;
    slave_config_t *s;
    static double local_scale[ETH_MAX_SLAVES];
    static double local_scalech2[ETH_MAX_SLAVES];
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

        tmp = fabs(motion_slaves[i].slave_input->scalech2);
        if(  tmp <= EPSILON  )
             local_scalech2[i] = 1;
        else
             local_scalech2[i] = motion_slaves[i].slave_input->scalech2;
        if(motion_slaves[i].slave_input->scaledirch2 == -1)
            local_scalech2[i] = -local_scalech2[i];
        local_scalech2[i] = 1/local_scalech2[i];

        if( testBit( s->par_input->data_6041 ,5) && \
                !testBit( s->par_input->data_6041 ,6) && \
                testBit( s->par_input->data_6041 ,0) && \
                testBit( s->par_input->data_6041 ,2) && \
                !testBit( s->par_input->data_6041 ,3))  { //servo run
            *(s->slave_output->enabled) = 1;
        } else {
            *(s->slave_output->enabled) = 0;
        }

        if(  testBit( s->par_input->data_6841 ,5) && \
           !testBit( s->par_input->data_6841 ,6) && \
           testBit( s->par_input->data_6841 ,0) && \
             testBit( s->par_input->data_6841 ,2) && \
             !testBit( s->par_input->data_6841 ,3))  { //servo run
            *(s->slave_output->enabledch2) = 1;
        } else {
            *(s->slave_output->enabledch2) = 0;
        }

    }

    //check ethercat state and port it to pins
    if( master_state.slaves_responding != (config_axis_num + config_io_num + config_exio_num + io_board_num) ||\
        master_state.al_states != 0x08 || \
        master_state.link_up !=  1   ) {
        for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
            *(motion_slaves[i].slave_output->linkstate) = 0;
              motion_slaves[i].par_output->data_6040 = 0;  //断线的情况下停止伺服
              motion_slaves[i].par_output->data_6840 = 0;  //断线的情况下停止伺服
        }
    } else {

        if(domain_state.wc_state == EC_WC_COMPLETE) { //
            for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
            //    if(motion_slaves[i].slaveinfo->al_state == 0x08)
                    *(motion_slaves[i].slave_output->linkstate) = 1;
            }
        } else {
//            for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
//                motion_slaves[i].par_output->data_6040 = 0;  //交换数据出错情况下停止伺服
//                *(motion_slaves[i].slave_output->linkstate) = 0;
//            }
        }
    }

    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        s = &motion_slaves[i];
        if( testBit( s->par_input->data_6041,3) ) { //有故障，报警设为1
            *(s->slave_output->alarm) = 1;
        } else {
            *(s->slave_output->alarm) = 0;
        }



        if( *(s->slave_output->linkstate) == 0 && s->isInited > 0) {
            *(s->slave_output->alarm) = 1;
        }

        if( (testBit( s->par_input->data_6841,3)) ) { //有故障，报警设为1
            *(s->slave_output->alarmch2) = 1;
        } else {
            *(s->slave_output->alarmch2) = 0;
        }

        if( *(s->slave_output->linkstate) == 0  && s->isInitedch2 > 0 ) {
            *(s->slave_output->alarmch2) = 1;
        }
    }

    //check Actions
    u_int32_t ret = 0;
    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        s = &motion_slaves[i];
        if( *(s->slave_input->enable) == 0 && \
                s->last_slave_input.enable == 1 ) {
            s->par_output->data_6040 = 0;
        }
        if( *(s->slave_input->enablech2) == 0 && \
                s->last_slave_input.enablech2 == 1 ) {
            s->par_output->data_6840 = 0;
        }



        //检查伺服使能指令：
        //默认打开伺服，并且设置成csp模式
        if( *(s->slave_input->enable) == 1 && *(s->slave_output->enabled) == 0)  {
                if(s->serv_on_process_flag == 0) {
                        s->par_output->data_60B0 = 0;
                        s->par_output->data_6060 = 0x08;
                        s->par_output->data_607A = ((long long)((*(s->slave_input->vel))*local_scale[i]) & 0xffffffff);
                        s->serv_on_process_flag = 1;
                        s->serv_on_process_count = 0;
                }
        }
        if( *(s->slave_input->enablech2) == 1 && *(s->slave_output->enabledch2) == 0)  {

            if(s->serv_on_process_flag_ch2 == 0) {
                        s->par_output->data_6860 = 0x08;
                        s->par_output->data_687A = (int)((*(s->slave_input->velch2))*local_scalech2[i]);
                        s->serv_on_process_flag_ch2 = 1;
                        s->serv_on_process_count_ch2 = 0;
//                        printf("now start enable =============================%d=======================ch2 \n",s->par_output->data_6860);
            }
        }

        //使能过程 modify by hong on 20170209
        if( s->serv_on_process_flag == 1 ) {
            switch(s->serv_on_process_count) {
            case 0:s->par_output->data_6040 = 0;
                        s->serv_on_process_count = 1;
                break;
            case 1:
                    if((s->par_input->data_6041 & 0x08) == 0x08) {  //电机驱动器有错误，直接跳转到错误处理
                    s->par_output->data_6040 = 0x80;
                    s->serv_on_process_count = 6;
                } else {
                    s->serv_on_process_count = 2;
                }
                break;
            case 2:
                if(s->enablecount < 500) {
                    s->enablecount++;
                    break;
                }
                s->enablecount = 0;
                s->par_output->data_6040 = 0x06;
                if((s->par_input->data_6041 & 0x7f) == 0x31 || (s->par_input->data_6041 & 0x7f) == 0x21) {
                        s->serv_on_process_count = 3;
                }break;
            case 3:s->par_output->data_6040 = 0x07;
                        if((s->par_input->data_6041 & 0x7f) == 0x33 || (s->par_input->data_6041 & 0x7f) == 0x23) {
                            s->serv_on_process_count = 4;
                }break;
            case 4:s->par_output->data_6040 = 0x0f;
                if((s->par_input->data_6041 & 0x7f) == 0x37 || (s->par_input->data_6041 & 0x7f) == 0x27) {
                        s->serv_on_process_count = 5;
                }break;
            case 5:s->serv_on_process_flag = 0;
                        s->serv_on_process_count = 0;
                        s->isInited = 1;   //第一次使能完后才开始设置alarm信号
                        printf("now I end enable motor %d\n",i);
                 break;
            case 6:
                if((s->par_input->data_6041 & 0x08) == 0x08) {
                    s->par_output->data_6040 = 0x80;
                    s->errorcount++;
                    if(s->errorcount > 5) {
                        s->errorcount = 0;
                        s->isInited = 1;
                        s->serv_on_process_flag = 0;
                    }
                } else {
                   s->serv_on_process_count = 2;
                }
                break;
            }
        }
        if( s->serv_on_process_flag_ch2 == 1 ) {
            switch(s->serv_on_process_count_ch2) {
            case 0:s->par_output->data_6840 = 0;
                        s->serv_on_process_count_ch2 = 1;
                break;
            case 1:
                    if((s->par_input->data_6841 & 0x08) > 0) {  //电机驱动器有错误，直接跳转到错误处理
                    s->par_output->data_6840 = 0x80;
                    s->serv_on_process_count_ch2 = 6;
                } else {
                    s->serv_on_process_count_ch2 = 2;
                }
                break;
            case 2:
                if(s->enablecountch2 < 500) {
                    s->enablecountch2++;
                    break;
                }
                s->enablecountch2 = 0;
                s->par_output->data_6840 = 0x06;
                if((s->par_input->data_6841 & 0x7f) == 0x31 || (s->par_input->data_6841 & 0x7f) == 0x21) {
                        s->serv_on_process_count_ch2 = 3;
                }break;
            case 3:s->par_output->data_6840 = 0x07;
                        if((s->par_input->data_6841 & 0x7f) == 0x33 || (s->par_input->data_6841 & 0x7f) == 0x23) {
                            s->serv_on_process_count_ch2 = 4;
                }break;
            case 4:s->par_output->data_6840 = 0x0f;
                if((s->par_input->data_6841 & 0x7f) == 0x37 || (s->par_input->data_6841 & 0x7f) == 0x27) {
                        s->serv_on_process_count_ch2 = 5;
                }break;
            case 5:s->serv_on_process_flag_ch2 = 0;
                        s->serv_on_process_count_ch2 = 0;
                        s->isInitedch2 = 1;   //第一次使能完后才开始设置alarm信号
                        printf("now I end enable motor_ch2 %d\n",i);
                 break;
            case 6:
                if((s->par_input->data_6841 & 0x08) > 0) {
                    s->par_output->data_6840 = 0x80;
                    s->errorcountch2++;
                    if(s->errorcountch2 > 5) {
                        s->errorcountch2 = 0;
                        s->isInitedch2 = 1;
                        s->serv_on_process_flag_ch2 = 0;
                    }
                } else {
                   s->serv_on_process_count_ch2 = 2;
                }
                break;
            }
        }

        s->last_slave_input.enable = *(s->slave_input->enable);
        s->last_slave_input.enablech2 = *(s->slave_input->enablech2);

//        if( *(s->slave_output->alarm)!=1 && \
//                testBit(s->par_input->data_6041,3) == 0 ) {

            s->par_output->data_6060 = 0x08;
            s->par_output->data_607A =  ((long long)((*(s->slave_input->vel))*local_scale[i]) & 0xffffffff);
//        }

            if(i == 0) {
//                printf("Now I get 607A:%d  %f %f \n",s->par_output->data_607A ,*(s->slave_input->vel),local_scale[i]);
            }

        if( *(s->slave_output->alarmch2)!=1 && \
                testBit(s->par_input->data_6841,3) == 0 ) {

            s->par_output->data_687A = ((long long)((*(s->slave_input->velch2))*local_scalech2[i]) & 0xffffffff);


        }

        *(s->slave_output->fbposition) = (float)(s->par_input->data_6064 - s->par_output->data_60B0)/local_scale[i];
        *(s->slave_output->fbpositionch2) = (float)(s->par_input->data_6864)/local_scalech2[i];

        *(s->slave_output->errorcode) = s->par_input->data_203F;

    }


    for( i = 0; i < config_io_num + config_exio_num; i++ ) {
        if(io_slaves[i].valid == 0) continue;
        if(i < config_io_num) {
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

            for(  n = 0;  n < IO_DOUTPUT_NUM;  n++ ) {
                //E68版本
                if(io_type == 0) {
                    tmp_int = (0x01<<n);
                    if(  *(io_slaves[i].slave_input->out[n]) == 1 )
                        io_slaves[i].par_output->data_7010 |= tmp_int;
                    else
                        io_slaves[i].par_output->data_7010 &= (~tmp_int);
                } else if( io_type == 1 ) {     //E68C
                    int index=0,port=0;
                    index = n/8;
                    port = n%8;
                    if(  *(io_slaves[i].slave_input->out[n]) == 1 ) {
                        io_slaves[i].par_output->data_7014[index] |= (0x01<<(24+port));         //E68C扩展的IO输出口
                    }else
                        io_slaves[i].par_output->data_7014[index] &= ~(0x01<<(24+port));
                }

            }

            for(  n = 0;  n < IO_DINPUT_NUM;  n++ ) {
                //E68版本
                int iosignal = 0;
                if(io_type == 0) {
                    tmp_int = ( 0x01<<n );
                    iosignal =  ((tmp_int & (io_slaves[i].par_input->data_6000))>0);
                } else if(io_type == 1) {
                    int index=0;
                    int port=0;
                    index = n/6;
                    port = n%6;
                    tmp_int = (0x01<<(26+port));
                    iosignal =  ((tmp_int & (io_slaves[i].par_input->data_6003[index]))>0);
                }
                    if( iosignal) {
                        if(*(io_slaves[i].slave_output->in[n]) > 0) {
                            io_slaves[i].par_input->flitercount[n]  = *(io_slaves[i].slave_output->infliter[n]);
                        } else {
                            if(io_slaves[i].par_input->flitercount[n] >= *(io_slaves[i].slave_output->infliter[n])) {            //新增IO滤波
                                *(io_slaves[i].slave_output->in[n])  = 1;
                            } else {
                                io_slaves[i].par_input->flitercount[n]++;
                            }
                        }
                    }else {
                         if(*(io_slaves[i].slave_output->in[n]) == 0) {
                             io_slaves[i].par_input->flitercount[n] = 0;
                         } else {
                            if(io_slaves[i].par_input->flitercount[n] <= 0 ) {
                                *(io_slaves[i].slave_output->in[n]) = 0;
                            } else {
                                io_slaves[i].par_input->flitercount[n]--;
                            }
                         }
                    }
            }


            //计算DA电压输出
            int vol = 10000 * (*(masterpins->DAVol[0])/60000) * (masterpins->DAScale[0]);
            int vol2 = 10000 * (*(masterpins->DAVol[1])/60000) * (masterpins->DAScale[1]);
            if(vol > 10000) vol = 10000;
            if(vol2 > 10000) vol2 = 10000;
            io_slaves[i].par_output->data_7010_1 = vol;
            io_slaves[i].par_output->data_7010_1 |= vol2 << 16;


        } else {            //扩展IO小板
            for(  n = 0;  n < IO_DOUTPUT_NUM;  n++ ) {
                //输出
                if(n < 8) {         //  0x3101_0
                    tmp_int = (0x01<<n);
                    if(  *(io_slaves[i].slave_input->out[n]) == 1 )
                        io_slaves[i].par_output->data_7010 |= tmp_int;
                    else
                        io_slaves[i].par_output->data_7010 &= (~tmp_int);
                } else if(n < 16) {     //  0x3101_1
                    tmp_int = (0x01<<(n-8));
                    if(  *(io_slaves[i].slave_input->out[n]) == 1 )
                        io_slaves[i].par_output->data_7010_1 |= tmp_int;
                    else
                        io_slaves[i].par_output->data_7010_1 &= (~tmp_int);
                }
            }

            for(  n = 0;  n < IO_DINPUT_NUM;  n++ ) {
                //输入
                int iosignal = 0;
                if( n < 8) {                                // 0x3001_0
                    tmp_int = ( 0x01<<n );
                    iosignal =  ((tmp_int & (io_slaves[i].par_input->data_6000))>0);
                } else if (n < 16) {                    // 0x3001_1
                    tmp_int = ( 0x01<<(n-8) );
                    iosignal =  ((tmp_int & (io_slaves[i].par_input->data_6000_2))>0);
                }
                    if( iosignal) {
                        if(*(io_slaves[i].slave_output->in[n]) > 0) {
                            io_slaves[i].par_input->flitercount[n]  = *(io_slaves[i].slave_output->infliter[n]);
                        } else {
                            if(io_slaves[i].par_input->flitercount[n] >= *(io_slaves[i].slave_output->infliter[n])) {            //新增IO滤波
                                *(io_slaves[i].slave_output->in[n])  = 1;
                            } else {
                                io_slaves[i].par_input->flitercount[n]++;
                            }
                        }
                    }else {
                         if(*(io_slaves[i].slave_output->in[n]) == 0) {
                             io_slaves[i].par_input->flitercount[n] = 0;
                         } else {
                            if(io_slaves[i].par_input->flitercount[n] <= 0 ) {
                                *(io_slaves[i].slave_output->in[n]) = 0;
                            } else {
                                io_slaves[i].par_input->flitercount[n]--;
                            }
                         }
                    }
            }
        }
    }
    errorConnect = 0;
    for( i = 0; i < config_axis_num+config_io_num+config_exio_num+io_board_num; i++ ) {
        if(*(motion_slaves[i].slave_output->linkstate) == 0) {
            errorConnect = 1;
            break;
        }
    }

    if(errorConnect > 0)
        *(masterpins->ready) = 0;
    else
        *(masterpins->ready) = 1;

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


    master_info = hal_malloc(sizeof(ec_master_info_t));
    motion_slaves = malloc(sizeof(slave_config_t) * ETH_MAX_SLAVES);
    io_slaves = malloc(sizeof(io_slave_config_t) * IO_SLAVES_MAXNUM);
    io_sc = malloc( sizeof(ec_slave_config_t *) * IO_SLAVES_MAXNUM);
    motion_sc = malloc( sizeof(ec_slave_config_t *) * ETH_MAX_SLAVES);

    if(config_axis_num > ETH_MAX_SLAVES ) config_axis_num = ETH_MAX_SLAVES;
    if(config_io_num > IO_SLAVES_MAXNUM ) config_io_num = IO_SLAVES_MAXNUM;

    int errorid = rtapi_shmem_new(22222,comp_id,sizeof(version_struct));
    int retval = rtapi_shmem_getptr(errorid,(void **)&versiondata);
    memset(versiondata->errorbuf,0,sizeof(versiondata->errorbuf));



    initSlaveConfigStruct();
    initBusState();
    exportEthercatPar();
    exportAllPins();
    invalidAllSlaves();

    versiondata->ethercatVersion = ethercatVersion;


    master = ecrt_request_master(0);
    if ( master ) {

        domain = ecrt_master_create_domain(master);
    }






    hal_ready(comp_id);
    return 0;


}

int initSlaves(){
    int ret = 0;

    char buf[HAL_NAME_LEN+2];
    int i,rv;
    if ( domain ) {

        int mostartnum = 0;
        if(link_type == 0) {
            mostartnum = 0;
        } else {
            mostartnum = config_io_num+config_exio_num;
        }

        if( init_io_slaves() < 0 ) {
            rtapi_print_msg(RTAPI_MSG_ERR, _("ETHERCAT: ERROR: IO_Board activate failed\n"));
            invalidAllSlaves();
            ret = -1;
        }
        if(io_board_num > 0) {
            if(init_io_board() < 0) {
                ret = -1;
            }
        }

        for( i = 0; i < config_axis_num; i++ ) {

            if(motion_slaves[i].valid == 1)         //已经初始化成功，不需要再初始化
                continue;

            motion_slaves[i].alias = 0;
            motion_slaves[i].position = i+io_board_num+mostartnum;



            rtapi_snprintf(buf, HAL_NAME_LEN, "%s/%d/axis-%02d.xml",config_file_path,motor_chose,i);
            if(access(buf, F_OK) != 0) {
                char error[256];
                sprintf(error,"类型%d中驱动%d的配置文件不存在",motor_chose,i);
                handleError(error);
                break;
            }

            MXml_read(&motion_slaves[i], buf);

            printf("now get master:%d  %d %d %X\n",motion_slaves[i].alias,motion_slaves[i].position,motion_slaves[i].vendor_id,motion_slaves[i].product_code);
            if (!( motion_sc[i] = ecrt_master_slave_config(master,
                            motion_slaves[i].alias,motion_slaves[i].position,motion_slaves[i].vendor_id,motion_slaves[i].product_code))) {
                rtapi_print_msg(RTAPI_MSG_ERR, "AXIS:%d  Failed to get slave configuration.\n",i);
               motion_slaves[i].valid = 0;
            } else motion_slaves[i].valid = 1;
            if(motion_slaves[i].valid == 0)  {
                ret = -1;
                continue;
            }

            if (ecrt_slave_config_pdos(motion_sc[i], EC_END, motion_slaves[i].sync_info)) {
                rtapi_print_msg(RTAPI_MSG_ERR, "AXIS:%d  Failed to configure PDOs.\n",i);
                motion_slaves[i].valid = 0;
            } else motion_slaves[i].valid = 1;
            if(motion_slaves[i].valid == 0)   {
                ret = -1;
                continue;
            }

            rv = initSlaveDomainRegs(domain,&motion_slaves[i]) ;
            if( rv ) {
                rtapi_print_msg(RTAPI_MSG_ERR, "AXIS:%d  Failed to DomainRegs \n",i);
                motion_slaves[i].valid = 0;
            } else motion_slaves[i].valid = 1;
            if(motion_slaves[i].valid == 0)  {
                ret = -1;
                continue;
            }

            initSdosConfig(master,&motion_slaves[i]);

            ecrt_slave_config_dc(motion_sc[i], motion_slaves[i].activate_word, PERIOD_NS, 4400000, 0, 0);

        }

    }


    return ret;
}
int initSlavesOK() {
    if (ecrt_master_activate(master)) {
        rtapi_print_msg(RTAPI_MSG_ERR, _("ETHERCAT: ERROR: master activate failed\n"));
        invalidAllSlaves();
    }

    if (!(domain_pd = ecrt_domain_data(domain))) {
        rtapi_print_msg(RTAPI_MSG_ERR, _("ETHERCAT: ERROR: get domain data failed\n"));
        invalidAllSlaves();
    }
}

void rtapi_app_exit(void)
{
    printf("now I get in abort now\n");
    slave_config_t *s;
    int j=0;
    for(j=0;j<ETH_MAX_SLAVES;j++) {
        s = &motion_slaves[j];
            s->par_output->data_6040 = 0x80;
            s->par_output->data_6840 = 0x80;
            exchange_data(0,0);
    }
    hal_exit(comp_id);
}

void check_IO_state() {
    if(errortimecount > 0) {
        errortimecount--;
    } else {
        errortimecount = 1500;
        if(master_state.al_states != 0x08) {
            if(SIMULATE == 0 )
                printf("Now I in here to reset master  %d\n",master_state.al_states);

            ecrt_master_reset(master);
        }
    }
}

void check_bus_state(void) {

    //int i;
    ec_domain_state_t ds;
    ec_master_state_t ms;

    ecrt_domain_state(domain, &ds);
    if ( ds.working_counter != domain_state.working_counter ) {
    }
    if (ds.wc_state != domain_state.wc_state) {
    }
    domain_state = ds;

    ecrt_master_state(master, &ms);
    if (ms.slaves_responding != master_state.slaves_responding)
        rtapi_print("%u slave(s).\n", ms.slaves_responding);
    if (ms.al_states != master_state.al_states)
        rtapi_print("AL states: 0x%02X.\n", ms.al_states);
    if (ms.link_up != master_state.link_up)
        rtapi_print("Link is %s.\n", ms.link_up ? "up" : "down");

    master_state = ms;

    ecrt_master(master,master_info);

    int i = 0;

    if(master_state.slaves_responding == (config_io_num+config_exio_num+io_board_num+config_axis_num)   && master_info->slave_count != 0  && master_info->scan_busy == 0) {
        int iostartnum = 0;
        if(link_type == 0) {
            iostartnum = config_axis_num+io_board_num;
        } else {
            iostartnum = io_board_num;
        }
         for( i = 0; i < config_io_num+config_exio_num; i++ ) {
            ecrt_master_get_slave(master,i+iostartnum,io_slaves[i].slaveinfo);
         }
         int mostartnum = 0;
         if(link_type == 0) {
             mostartnum = io_board_num;
         } else {
             mostartnum = config_io_num+config_exio_num+io_board_num;
         }
        for( i = 0; i < config_axis_num; i++ ) {
            ecrt_master_get_slave(master,i+mostartnum,motion_slaves[i].slaveinfo);
            *(motion_slaves[i].slave_output->al_state) = motion_slaves[i].slaveinfo->al_state;
            *(motion_slaves[i].slave_output->error_flag) = motion_slaves[i].slaveinfo->error_flag;
        }
    }

    check_IO_state();


}

/*****************************************************************/
static void exchange_data(void *args,long peri)
{

    static int once_init = 0;
    int i;
    struct timespec time;
    slave_config_t *s;

    long long int last = rtapi_get_time();
    long long int timebefore;
    long long int timeafter;
    long int difftime ;

    for( i = 0; i < MAX_THREAD_LATENCY_NUM; i++ ) {
        *thread_lat[i] = thread_max_latency[i];
    }


    if( !master ) return;
    if( !domain ) return;

    //获取当前主站状态信息
    check_bus_state();

    if( once_init == 0 ) {
//        printf("master info:%d  %d   responding:%d\n",master_info->slave_count,master_info->scan_busy,master_state.slaves_responding);
        if(master_info->scan_busy == 0  && master_state.slaves_responding  == (config_io_num+config_exio_num+io_board_num+config_axis_num)) {
            printf("now scanning slaves  done ......\n");
            if(initSlaves() == 0 ) {
                initSlavesOK();
                once_init = 1;
            }
        } else {
//            printf("now scanning slaves ...... busy:%d ?  %d   responing:%d\n",master_info->scan_busy,master_info->slave_count,master_state.slaves_responding);
        }
    }

    if( (!domain_pd) ) return;

    timebefore =  rtapi_get_time();
    clock_gettime(CLOCK_TO_USE, &time);
    ecrt_master_application_time(master, TIMESPEC2NS(time));



    // receive process data
    ecrt_master_receive(master);
    ecrt_domain_process(domain);


    if(once_init == 0) return;


    timebefore =  rtapi_get_time();
    handleExchangeData();
    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[3]) < difftime) {
        *(masterpins->testtime[3]) = difftime;
    }


    for( i = 0; i < config_axis_num; i++ ) {

        s = &(motion_slaves[i]);
        if(s->valid == 0) continue;


        if( s->off_data_6040 != -1) {
            EC_WRITE_U16(domain_pd + s->off_data_6040,\
                         motion_slaves[i].par_output->data_6040);
        }

        if( s->off_data_607A != -1
//                && s->slave_input->enable > 0
                ) {
            EC_WRITE_S32(domain_pd + s->off_data_607A, \
                motion_slaves[i].par_output->data_607A);
        }

        if( s->off_data_60B0 != -1 ) {
            EC_WRITE_S32(domain_pd + s->off_data_60B0,\
                motion_slaves[i].par_output->data_60B0);
        }

        if( s->off_data_6060 != -1 ) {
            EC_WRITE_U8(domain_pd + s->off_data_6060, \
                motion_slaves[i].par_output->data_6060);
        }

        if( s->off_data_probe_ctl != -1 ) {
            EC_WRITE_U16(domain_pd + s->off_data_probe_ctl,\
                     motion_slaves[i].par_output->data_probe_ctl);
        }

        if( s->off_data_6860 != -1 ) {
            EC_WRITE_U8(domain_pd + s->off_data_6860, \
                motion_slaves[i].par_output->data_6860);
        }
        if( s->off_data_6840 != -1) {
            EC_WRITE_U16(domain_pd + s->off_data_6840,\
                         motion_slaves[i].par_output->data_6840);
        }
        if( s->off_data_687a != -1 && s->slave_input->enablech2 > 0) {
            EC_WRITE_S32(domain_pd + s->off_data_687a, \
                motion_slaves[i].par_output->data_687A);
        }
        if( s->off_data_68ff != -1 && s->slave_input->enablech2 > 0) {
            EC_WRITE_S32(domain_pd + s->off_data_68ff, \
                motion_slaves[i].par_output->data_68ff);
        }

        if( s->off_data_6841 != -1 ) {
            s->par_input->data_6841 = EC_READ_U16(domain_pd + s->off_data_6841);
        }
        if( s->off_data_6864 != -1 ) {
            s->par_input->data_6864 = EC_READ_S32(domain_pd + s->off_data_6864);
        }
        if( s->off_data_6861 != -1 ) {
            s->par_input->data_6861 = EC_READ_U8(domain_pd + s->off_data_6861);
        }
        if( s->off_data_686c != -1 ) {
            s->par_input->data_686c = EC_READ_S32(domain_pd + s->off_data_686c);
        }

        if( s->off_data_6041 != -1 ) {
            s->par_input->data_6041 = EC_READ_U16(domain_pd + s->off_data_6041);
        }
        if( s->off_data_6064 != -1 ) {
            s->par_input->data_6064 = EC_READ_S32(domain_pd + s->off_data_6064);
        }
        if( s->off_data_6061 != -1 ) {
            s->par_input->data_6061 = EC_READ_U8(domain_pd + s->off_data_6061);
        }
        if( s->off_data_603F != -1 ) {
            s->par_input->data_603F = EC_READ_U16(domain_pd + s->off_data_603F);
        }
        if( s->off_data_60FD != -1 ) {
            s->par_input->data_60FD = EC_READ_U32(domain_pd + s->off_data_60FD);
        }
        if( s->off_data_203F != -1 ) {
            s->par_input->data_203F = EC_READ_U16(domain_pd + s->off_data_203F);
        }
        if( s->off_data_probe_status != -1 ) {
            s->par_input->data_probe_status = EC_READ_U16(domain_pd + s->off_data_probe_status);
        }
        if( s->off_data_probe_pos != -1 ) {
            s->par_input->data_probe_pos = EC_READ_U32(domain_pd + s->off_data_probe_pos);
        }

    }

    for( i = 0; i < config_io_num+config_exio_num; i++ ) {

        if(io_slaves[i].valid == 0) continue;

        if(i < config_io_num) {         //主IO板
            //board_type 值为0代表ET1100  值为1为LAN9252
                if(board_type == 0) {
                    EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7010, \
                            io_slaves[i].par_output->data_7010);
                    EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7010_1, \
                            io_slaves[i].par_output->data_7010_1);
                } else if(board_type == 1) {

                    EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7013, \
                            ~(io_slaves[i].par_output->data_7010));
                    EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7012, \
                            io_slaves[i].par_output->data_7010_1);

                    int j=0;
                    for(j=0;j<8;j++) {
                        EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7014[j], \
                                     io_slaves[i].par_output->data_7014[j]);
                    }
                }
                EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7010_2, \
                        io_slaves[i].par_output->data_7010_2);
                EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7010_3, \
                        io_slaves[i].par_output->data_7010_3);
                EC_WRITE_U32(domain_pd + io_slaves[i].off_data_7010_4, \
                        io_slaves[i].par_output->data_7010_4);

                if(board_type == 0) {
                    io_slaves[i].par_input->data_6000 = \
                            EC_READ_U32(domain_pd + io_slaves[i].off_data_6000);
                    io_slaves[i].par_input->data_6000_2 = \
                        EC_READ_U32(domain_pd + io_slaves[i].off_data_6000_2);
                } else if(board_type == 1) {

                    io_slaves[i].par_input->data_6000 = \
                        EC_READ_U32(domain_pd + io_slaves[i].off_data_6001);
                    io_slaves[i].par_input->data_6000_2 = \
                        EC_READ_U32(domain_pd + io_slaves[i].off_data_6002);

                    int j=0;    //读feedback数据上来
                    for(j=0;j<8;j++) {
                        io_slaves[i].par_input->data_6003[j] = \
                                EC_READ_U32(domain_pd + io_slaves[i].off_data_6003[j]);
                    }
                }
                io_slaves[i].par_input->data_6000_1 = \
                    EC_READ_U32(domain_pd + io_slaves[i].off_data_6000_1);
                io_slaves[i].par_input->data_6000_3 = \
                    EC_READ_U32(domain_pd + io_slaves[i].off_data_6000_3);
                io_slaves[i].par_input->data_6000_4 = \
                    EC_READ_U32(domain_pd + io_slaves[i].off_data_6000_4);
        } else {
            EC_WRITE_U8(domain_pd + io_slaves[i].off_data_7012, \
                    (io_slaves[i].par_output->data_7010));
            EC_WRITE_U8(domain_pd + io_slaves[i].off_data_7013, \
                    io_slaves[i].par_output->data_7010_1);

            io_slaves[i].par_input->data_6000 = \
                EC_READ_U8(domain_pd + io_slaves[i].off_data_6001);
            io_slaves[i].par_input->data_6000_2 = \
                EC_READ_U8(domain_pd + io_slaves[i].off_data_6002);
        }
    }
    if(io_board_num > 0) {
        *(masterpins->plane_in0) = EC_READ_U8(domain_pd + piane_in0);
        *(masterpins->plane_in1) =  EC_READ_U8(domain_pd + piane_in1);
    }
    //获取手轮变化值
    *(masterpins->handleVal) = io_slaves[0].par_input->data_6000_2;  //E68C获取第一块IO板数据


    if (sync_ref_counter) {
        sync_ref_counter--;
    } else {

        sync_ref_counter = 1; // sync every cycle
        ecrt_master_sync_reference_clock(master);

    }

    ecrt_master_sync_slave_clocks(master);


    // send process data
    ecrt_domain_queue(domain);
    ecrt_master_send(master);



    long long int now = rtapi_get_time();
    long int this_run = (long int) (now - last);
    *(masterpins->realruntime) = this_run;

    if(this_run > *(masterpins->maxtime)) {
        *(masterpins->maxtime) = this_run;
    }

}
