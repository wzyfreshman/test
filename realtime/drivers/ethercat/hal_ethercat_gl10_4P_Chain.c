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

#include <ecrt.h>
#include <mxml.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define MAX_BUS 3	/* max number of parports (EPP busses) */

#define	EPSILON		1e-20

#define MAXLINELEN 1024
#define MAXPARLINE  500
#define MAX_EXTERNIO 2

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

static int extern_io_in_num[MAX_EXTERNIO] = {0,0};
RTAPI_MP_ARRAY_INT(extern_io_in_num,2, "extern_io_in_num");

static int extern_io_in16_num[MAX_EXTERNIO] = {3,0};
RTAPI_MP_ARRAY_INT(extern_io_in16_num,2, "extern_io_in16_num");

static int extern_io_out_num[MAX_EXTERNIO] = {0,0};
RTAPI_MP_ARRAY_INT(extern_io_out_num, 2,"extern_io_out_num");

static int extern_io_out16_num[MAX_EXTERNIO] = {8,0};
RTAPI_MP_ARRAY_INT(extern_io_out16_num,2, "extern_io_out16_num");



static int config_axis_num = 1;
RTAPI_MP_INT(config_axis_num, "config_axis_num");

static int config_externio_num = 1;
RTAPI_MP_INT(config_externio_num, "config_externio_num");

static char *config_file_path = NULL;
RTAPI_MP_STRING(config_file_path,"config_file_path");

static int motor_chose = 0;
RTAPI_MP_INT(motor_chose,"motor_chose");

static int board_type = 0;
RTAPI_MP_INT(board_type,"board_type");

static int link_type = 0;
RTAPI_MP_INT(link_type,"link_type");

#define SLAVE1  0, 0
#define TAIDA 0x1DD, 0x10305070

static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};
static ec_domain_t *domain = NULL;
static ec_domain_state_t domain_state = {};

static ec_slave_config_t **io_sc = NULL;
static ec_slave_config_t **motion_sc = NULL;

// process data
static uint8_t *domain_pd = NULL;

static unsigned int sync_ref_counter = 0;

static int errortimecount = 1500;

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
#define IO_MODULES_MAXNUM 2
#define IO_SLAVES_MAXNUM 16
#define IO_DOUTPUT_NUM 32
#define IO_DINPUT_NUM 32

void initEthercat();
/*

ec_pdo_entry_info_t slave_0_pdo_entries[] = {
    {0x6040, 0x00, 16},
    {0x607a, 0x00, 32},
    {0x6060, 0x00, 8},
    {0x6041, 0x00, 16},
    {0x6064, 0x00, 32},
};

ec_pdo_info_t slave_0_pdos[] = {
    {0x1601, 3, slave_0_pdo_entries + 0},
    {0x1a01, 2, slave_0_pdo_entries + 3},
};

ec_sync_info_t slave_0_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL, EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 0, NULL, EC_WD_DISABLE},
    {2, EC_DIR_OUTPUT, 1, slave_0_pdos + 0, EC_WD_DISABLE},
    {3, EC_DIR_INPUT, 1, slave_0_pdos + 1, EC_WD_DISABLE},
    {0xff}
};
*/

ec_pdo_entry_info_t slave_0_pdo_entries[] = {
    {0x607a, 0x00, 32}, /* Position demand value */
    {0x60ff, 0x00, 32}, /* Velocity demand value */
    {0x6040, 0x00, 16}, /* Control word */
    {0x6060, 0x00, 16}, /* Control word */
//    {0x6071, 0x00, 16}, /* Control word */
    {0x6064, 0x00, 32}, /* Position actual value */
    {0x606c, 0x00, 32}, /* Velocity actual value */
    {0x6041, 0x00, 16}, /* Status word */
    {0x6061, 0x00, 16}, /* Status word */
//    {0x6077, 0x00, 16}, /* Status word */

};

ec_pdo_info_t slave_0_pdos[] = {
    {0x1601, 4, slave_0_pdo_entries + 0}, /* receive pdo1 */
    {0x1a01, 4, slave_0_pdo_entries + 4}, /* transmit pdo1 */

};

ec_sync_info_t slave_0_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL, EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 0, NULL, EC_WD_DISABLE},
    {2, EC_DIR_OUTPUT, 1, slave_0_pdos + 0, EC_WD_DISABLE},
    {3, EC_DIR_INPUT, 1, slave_0_pdos + 1, EC_WD_DISABLE},
    {0xff}

};

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


    hal_bit_t *in[IO_DINPUT_NUM];
    hal_float_t infliter[IO_DINPUT_NUM];
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

    hal_bit_t *out[IO_DOUTPUT_NUM];

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
    hal_float_t flitercount[IO_DINPUT_NUM];

} paremeter_input_t;

typedef struct {

    hal_bit_t data_probe_status;

} last_paremeter_input_t;

typedef struct {

    hal_bit_t *out[IO_DOUTPUT_NUM];

} io_slave_input_t;

typedef struct {

    hal_bit_t *in[IO_DINPUT_NUM];
    hal_bit_t recordin[IO_DINPUT_NUM];
    hal_float_t infliter[IO_DINPUT_NUM];
    hal_float_t flitercount[IO_DINPUT_NUM];

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
    ec_sync_info_t *sync_info;
    ec_sdos_config_t *sdos_config;
    ec_pdo_info_t * pdo_info;
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
    ec_slave_info_t *slaveinfo; //add by hong 20170512 to read the slave state


    last_slave_input_t last_slave_input;
    last_paremeter_input_t last_paremeter_input;


    int serv_on_process_flag;
    int serv_on_process_count;

    int probe_process_flag;
    int probe_process_count;
    int isInited;           //是否已经初始化标志位
} slave_config_t;

static slave_config_t *motion_slaves;


typedef struct EXTERN_IO_slave_config
{
    int valid;
    int alias;
    int position;
    int vendor_id;
    int product_code;
    int activate_word;

    int linkstate;


    hal_u32_t ext_io_in_0[IO_SLAVES_MAXNUM];
    hal_u32_t ext_io_in_1[IO_SLAVES_MAXNUM];
    hal_u32_t ext_io_in_2[IO_SLAVES_MAXNUM];
    hal_u32_t ext_io_in_3[IO_SLAVES_MAXNUM];


    hal_u32_t ext_io_out_0[IO_SLAVES_MAXNUM];
    hal_u32_t ext_io_out_1[IO_SLAVES_MAXNUM];
    hal_u32_t ext_io_out_2[IO_SLAVES_MAXNUM];
    hal_u32_t ext_io_out_3[IO_SLAVES_MAXNUM];

    io_slave_input_t *slave_input[IO_DINPUT_NUM];
    io_slave_output_t *slave_output[IO_DOUTPUT_NUM];




    int off_extern_in_0[IO_SLAVES_MAXNUM];
    int off_extern_in_1[IO_SLAVES_MAXNUM];
    int off_extern_in_2[IO_SLAVES_MAXNUM];
    int off_extern_in_3[IO_SLAVES_MAXNUM];
    int off_extern_out_0[IO_SLAVES_MAXNUM];
    int off_extern_out_1[IO_SLAVES_MAXNUM];
    int off_extern_out_2[IO_SLAVES_MAXNUM];
    int off_extern_out_3[IO_SLAVES_MAXNUM];

} EXTERN_IO_slave_config;

static EXTERN_IO_slave_config *extern_io_slaves;

#define EXTERN_IO 0x100000, 0x10f40910
int init_externio_slaves() {

    int i = 0;

    int iostartnum = 0;
    if(link_type == 0) {
        iostartnum = config_axis_num;
    } else {
        iostartnum = 0;
    }

    for(i=0;i<config_externio_num;i++) {
        extern_io_slaves[i].valid = 0;


            printf("now I get extern  ionum:%d link:%d\n",iostartnum,link_type);
            if ( !(io_sc[i] = ecrt_master_slave_config(master,0,iostartnum+i, EXTERN_IO) ) ) {
                rtapi_print_msg(RTAPI_MSG_ERR, "EXT_IO_Borad: Failed to get slave configuration.\n");
                return -1;
            }



            int m = 0;
            for(m=0;m<IO_SLAVES_MAXNUM;m++) {
                extern_io_slaves[i].off_extern_in_0[m] = -1;
                extern_io_slaves[i].off_extern_in_1[m] = -1;
                extern_io_slaves[i].off_extern_in_2[m] = -1;
                extern_io_slaves[i].off_extern_in_3[m] = -1;
                extern_io_slaves[i].off_extern_out_0[m] = -1;
                extern_io_slaves[i].off_extern_out_1[m] = -1;
                extern_io_slaves[i].off_extern_out_2[m] = -1;
                extern_io_slaves[i].off_extern_out_3[m] = -1;
            }

            for(m=0;m<extern_io_in_num[i];m++) {
                int index = 0x6001+m*256;
                extern_io_slaves[i].off_extern_in_0[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 1, domain, NULL);
                if (extern_io_slaves[i].off_extern_in_0[m] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_in_1[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 2, domain, NULL);
                if (extern_io_slaves[i].off_extern_in_1[m] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_in_2[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 3, domain, NULL);
                if (extern_io_slaves[i].off_extern_in_2[m] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_in_3[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 4, domain, NULL);
                if (extern_io_slaves[i].off_extern_in_3[m] < 0) {
                    return -1;
                }
//                printf("now I get IN Index:%X   sub:%X  %d\n",index,1,extern_io_in_num);
            }
            for(m=0;m<extern_io_in16_num[i];m++) {
                int index = 0x6000+(m+extern_io_in_num[i])*256;
                extern_io_slaves[i].off_extern_in_0[m+extern_io_in_num[i]] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 1, domain, NULL);
                if (extern_io_slaves[i].off_extern_in_0[m+extern_io_in_num[i]] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_in_1[m+extern_io_in_num[i]] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 2, domain, NULL);
                if (extern_io_slaves[i].off_extern_in_1[m+extern_io_in_num[i]] < 0) {
                    return -1;
                }
            }
            for(m=0;m<extern_io_out_num[i];m++) {
                int index = 0x7003+(m+extern_io_in_num[i]+extern_io_in16_num[i])*256;
                extern_io_slaves[i].off_extern_out_0[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 1, domain, NULL);
                if (extern_io_slaves[i].off_extern_out_0[m] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_out_1[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 2, domain, NULL);
                if (extern_io_slaves[i].off_extern_out_1[m] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_out_2[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 3, domain, NULL);
                if (extern_io_slaves[i].off_extern_out_2[m] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_out_3[m] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 4, domain, NULL);
                if (extern_io_slaves[i].off_extern_out_3[m] < 0) {
                    return -1;
                }
//                printf("now I get out Index:%X   sub:%X \n",index,1);
            }
            for(m=0;m<extern_io_out16_num[i];m++) {
                int index = 0x7002+(m+extern_io_in_num[i]+extern_io_in16_num[i]+extern_io_out_num[i])*256;
                extern_io_slaves[i].off_extern_out_0[m+extern_io_out_num[i]] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 1, domain, NULL);
                if (extern_io_slaves[i].off_extern_out_0[m+extern_io_out_num[i]] < 0) {
                    return -1;
                }
                extern_io_slaves[i].off_extern_out_1[m+extern_io_out_num[i]] = ecrt_slave_config_reg_pdo_entry(io_sc[i],index, 2, domain, NULL);
                if (extern_io_slaves[i].off_extern_out_1[m+extern_io_out_num[i]] < 0) {
                    return -1;
                }
            }

        extern_io_slaves[i].valid = 1;
    }

    return 0;

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
            if( rx_i <= 1 ) {
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

            if( EntryCount < 1 )
                return -1;

            pdo_info[rx_i].n_entries = EntryCount;

            ec_pdo_entry_info_t *entries_rx = malloc(sizeof(ec_pdo_entry_info_t)*EntryCount);
            pdo_info[rx_i].entries = entries_rx;

            mxml_node_t *findNode;

//            printf("now I ge re entrycount:%d\n",EntryCount);
            for( i = 0 ;i < EntryCount; i++ ) {

                //find Entry
                if( i <1 ) {
                    if ((findNode = mxmlFindElement(rxPdoNode,tree,"Entry", NULL, NULL,
                                                MXML_DESCEND)) == NULL) {
                       return (-1);
                    }
                } else {
                    if ((findNode = mxmlFindElement(findNode, tree,"Entry", NULL, NULL,
                                                MXML_NO_DESCEND)) == NULL) {
                      return (-1);
                    }
                }


                //get Entry Child Node
                if( (valueNode = mxmlFindElement(findNode, tree,"Index", NULL, NULL,
                                            MXML_DESCEND)) == NULL) {
                    mxmlDelete(tree);
                    break;
                }
                if((text = mxmlGetText(valueNode, NULL)) == NULL ) {
                    break;
                }

                strncpy(strResult,text,sizeof(strResult));
                checkName(strResult);
                entries_rx[i].index = strtoul(strResult,&end_ptr,16);

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
                entries_rx[i].subindex = strtoul(strResult,&end_ptr,16);

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
                entries_rx[i].bit_length = strtoul(strResult,&end_ptr,10);

           }


        }

        for( tx_i = rxPdoCount; tx_i < (txPdoCount + rxPdoCount); tx_i++ ) {
            printf("now I get txc_i:%d\n",tx_i);
            const char	*text;
            mxml_node_t *valueNode;
            if( tx_i <= rxPdoCount ) {
                if ((txPdoNode = mxmlFindElement(descriptionsNode,tree,"TxPdo", NULL, NULL,MXML_DESCEND)) == NULL)
                  return -1;

            } else {
                if ((txPdoNode = mxmlFindElement(txPdoNode,tree,"TxPdo", NULL, NULL, MXML_NO_DESCEND)) == NULL)
                  return -1;
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

            printf("now I ge tx entrycount:%d\n",EntryCount);
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
     //   slave->sync_info = slave_0_syncs;
        slave->pdos_config_size = txPdoCount + rxPdoCount;
        slave->pdo_info = pdo_info;
        slave->valid = 1;


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
    t->off_data_2060 = -1;
    t->off_data_2061 = -1;
    t->off_data_2062 = -1;
//    t->off_data_200b = -1;

    t->off_data_606C = -1;
    t->off_data_6077 = -1;
    t->off_data_60FF = -1;
    t->off_data_6071 = -1;
    t->off_data_probe_status = -1;
    t->off_data_probe_ctl = -1;
    t->off_data_probe_pos = -1;

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
            else if(  entries[j].index == 0x2060 )
                domain_regs[count].offset = &t->off_data_2060;
            else if(  entries[j].index == 0x2061 )
                domain_regs[count].offset = &t->off_data_2061;
            else if(  entries[j].index == 0x2062 )
                domain_regs[count].offset = &t->off_data_2062;
//            else if(  entries[j].index == 0x200b )
//                domain_regs[count].offset = &t->off_data_200b;
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



           printf("Reg %d  %d index  %X sub %X vendor:%X  product:%d %d %d\n", domain_regs[count].alias,domain_regs[count].position,domain_regs[count].index,domain_regs[count].subindex,domain_regs[count].vendor_id,domain_regs[count].product_code,i,j);

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

        usleep(1000);
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
        printf("now I get code:%d index:%X\n",ret,s->index);
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

#ifdef USING_CIA402_HOME
        t->last_slave_input.homecmd = 0;
        t->last_slave_input.home_done_flag = 0;
        t->last_slave_input.home_reach_flag = 0;
        t->home_process_count = 0;
        t->home_process_flag = 0;
#endif

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

        t->pdo_info = NULL;
        t->pdos_config_size = 0;
        t->isInited = 0;


    }


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

        rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.mt.%02d.enable",i);
        retval = hal_pin_bit_new(buf, HAL_IN, &(input->enable), comp_id);
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

    for(i = 0;i < config_externio_num;i++) {

        int m =0 ;
        for(  n = 0;  n < extern_io_in_num[i];  n++ ) {
            extern_io_slaves[i].slave_output[n] = hal_malloc(sizeof(io_slave_output_t));
            for(m=0;m<32;m++) {
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.externio.%d.in.%02d",i,32*n+m);
                retval = hal_pin_bit_new(buf, HAL_OUT, &(extern_io_slaves[i].slave_output[n]->in[m]), comp_id);
                if (retval != 0) {
                    return -1;
                }
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.externio.%d.infliter.%02d",i,32*n+m);
                retval = hal_param_float_new(buf, HAL_RW, &(extern_io_slaves[i].slave_output[n]->infliter[m]), comp_id);
                if (retval != 0) {
                    return -1;
                }
                (extern_io_slaves[i].slave_output[n]->infliter[m]) = 5;
                (extern_io_slaves[i].slave_output[n]->flitercount[m]) = 0;
                (extern_io_slaves[i].slave_output[n]->recordin[m]) = 0;
            }
        }

        for(  n = 0;  n < extern_io_in16_num[i];  n++ ) {
            extern_io_slaves[i].slave_output[n+extern_io_in_num[i]] = hal_malloc(sizeof(io_slave_output_t));
            for(m=0;m<16;m++) {
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.externio.%d.in.%02d",i,16*n+m+32*extern_io_in_num[i]);
                retval = hal_pin_bit_new(buf, HAL_OUT, &(extern_io_slaves[i].slave_output[n+extern_io_in_num[i]]->in[m]), comp_id);
                if (retval != 0) {
                    return -1;
                }
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.externio.%d.infliter.%02d",i,16*n+m+32*extern_io_in_num[i]);
                retval = hal_param_float_new(buf, HAL_RW, &(extern_io_slaves[i].slave_output[n+extern_io_in_num[i]]->infliter[m]), comp_id);
                if (retval != 0) {
                    return -1;
                }
                (extern_io_slaves[i].slave_output[n+extern_io_in_num[i]]->infliter[m]) = 5;
                (extern_io_slaves[i].slave_output[n+extern_io_in_num[i]]->flitercount[m]) = 0;
                (extern_io_slaves[i].slave_output[n+extern_io_in_num[i]]->recordin[m]) = 0;
            }
        }

        for(  n = 0;  n < extern_io_out_num[i];  n++ ) {
            extern_io_slaves[i].slave_input[n] = hal_malloc(sizeof(io_slave_output_t));
            for(m=0;m<32;m++) {
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.externio.%d.out.%02d",i,32*n+m);
                retval = hal_pin_bit_new(buf, HAL_IN, &(extern_io_slaves[i].slave_input[n]->out[m]), comp_id);
                if (retval != 0) {
                    return -1;
                }
            }
        }
        for(  n = 0;  n < extern_io_out16_num[i];  n++ ) {
            extern_io_slaves[i].slave_input[n+extern_io_out_num[i]] = hal_malloc(sizeof(io_slave_output_t));
            for(m=0;m<16;m++) {
                rtapi_snprintf(buf, HAL_NAME_LEN, "ethercat.externio.%d.out.%02d",i,16*n+m+extern_io_out_num[i]*32);
                retval = hal_pin_bit_new(buf, HAL_IN, &(extern_io_slaves[i].slave_input[n+extern_io_out_num[i]]->out[m]), comp_id);
                if (retval != 0) {
                    return -1;
                }
            }
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
    static float local_scale[ETH_MAX_SLAVES];
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
        if(2 == motor_chose) {      //台达齿轮比为1比1
            local_scale[i] *= 128;
        }

        if(2 == motor_chose) {
            local_scale[i] *= 128;
        }

        if( testBit( s->par_input->data_6041 ,5) && \
            !testBit( s->par_input->data_6041 ,6) && \
            testBit( s->par_input->data_6041 ,0) )  { //servo run
            *(s->slave_output->enabled) = 1;
        } else {
            *(s->slave_output->enabled) = 0;
        }

    }

    //check ethercat state and port it to pins
//    printf("now I get responding:%d al_state:%d linkup:%d\n",master_state.slaves_responding,master_state.al_states,master_state.link_up);


//    static int errorcount = 0;
    if( master_state.slaves_responding != (config_axis_num +config_externio_num) ||\
        master_state.al_states != 0x08 || \
        master_state.link_up !=  1   ) {
        for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
            *(motion_slaves[i].slave_output->linkstate) = 0;
              motion_slaves[i].par_output->data_6040 = 0;  //断线的情况下停止伺服
        }
        for(i = 0;i<IO_MODULES_MAXNUM;i++){
            (extern_io_slaves[i].linkstate) = 0;
        }
    } else {
       // printf("wcstate:%d\n",domain_state.wc_state);
        if(domain_state.wc_state == EC_WC_COMPLETE) { //
            for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
            //    if(motion_slaves[i].slaveinfo->al_state == 0x08)
                    *(motion_slaves[i].slave_output->linkstate) = 1;
            }
            for(i = 0;i<IO_MODULES_MAXNUM;i++){
                (extern_io_slaves[i].linkstate) = 1;
            }
//            errorcount = 0;
        } else {
//            for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
//                motion_slaves[i].par_output->data_6040 = 0;  //交换数据出错情况下停止伺服
//                *(motion_slaves[i].slave_output->linkstate) = 0;
//            }
//            if(errorcount = 0) {
//                printf("交换数据出错情况下停止伺服\n");//sin
//                errorcount = 1;
//            }
        }

    }

    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        s = &motion_slaves[i];
        if( (testBit( s->par_input->data_6041,3) /*||  testBit( s->par_input->data_6041,7)*/ ) && s->isInited > 0 ) { //有故障，报警设为1
            *(s->slave_output->alarm) = 1;
        } else {
            *(s->slave_output->alarm) = 0;
        }

        if( *(s->slave_output->linkstate) == 0  && s->isInited > 0 ) {
            *(s->slave_output->alarm) = 1;
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


        //检查伺服使能指令：
        //默认打开伺服，并且设置成csp模式
        if( *(s->slave_input->enable) == 1 && \
                s->last_slave_input.enable == 0)  {

                    if(s->serv_on_process_flag == 0) {
//                        printf("now I in reset the servo 6064:%d  id:%d\n",s->par_input->data_6064,i);
//                        s->par_output->data_60B0 = s->par_input->data_6064; //read the encoder
 //                       s->par_output->data_60B0 -= (int)((*(s->slave_input->vel))*local_scale[i]);
                        s->par_output->data_60B0 = 0;
                        s->par_output->data_6060 = 0x08;
                        s->par_output->data_607A = (int)((*(s->slave_input->vel))*local_scale[i]);
                        s->serv_on_process_flag = 1;

                    }

     //           }

        }

        //使能过程 modify by hong on 20170209



        if( s->serv_on_process_flag == 1 ) {

            switch(s->serv_on_process_count) {
            case 0:s->par_output->data_6040 = 0;
                        s->serv_on_process_count = 1;

                break;
            case 1:
                    if((s->par_input->data_6041 & 0x040) != 0x0040) {  //电机驱动器有错误，直接跳转到错误处理
                    s->par_output->data_6040 = 0x80;
                    s->serv_on_process_count = 6;
                } else {
                    s->serv_on_process_count = 2;
                }
                break;
            case 2:s->par_output->data_6040 = 0x06;
//                printf("now I end start enable s->par_input->data_604 %X  %d   6040:%X\n",s->par_input->data_6041,i,s->par_output->data_6040);
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
                        printf("now I end enable  %d\n",i);
                 break;
            case 6:
                if((s->par_input->data_6041 & 0x040) != 0x0040) {
                    s->par_output->data_6040 = 0x80;
                    s->isInited = 1;   //出错也设置alarm信号
                } else {
                   s->serv_on_process_count = 2;
                }
                break;
             }

        }

        s->last_slave_input.enable = *(s->slave_input->enable);

        if( *(s->slave_output->alarm)!=1 && \
                *(s->slave_output->enabled) == 1 && \
                testBit(s->par_input->data_6041,0) == 1 && \
                testBit(s->par_input->data_6041,2) == 1 && \
                testBit(s->par_input->data_6041,3) == 0 ) {

            s->par_output->data_607A = (int)((*(s->slave_input->vel))*local_scale[i]);


        }
        //if(i == 0)
        //printf("now I get scale %ld   fb:%ld\n",s->par_input->data_6041,s->par_output->data_607A );


        if( *(s->slave_output->alarm)!=1 && \
                *(s->slave_output->enabled) == 1 && \
                testBit(s->par_input->data_6041,0) == 1 && \
                testBit(s->par_input->data_6041,2) == 1 && \
                testBit(s->par_input->data_6041,3) == 0 ) {

            s->par_output->data_607A = (int)((*(s->slave_input->vel))*local_scale[i]);


        }
        //if(i == 0)
        //printf("now I get scale %ld   fb:%ld\n",s->par_input->data_6041,s->par_output->data_607A );

        *(s->slave_output->fbposition) = (float)(s->par_input->data_6064 - s->par_output->data_60B0)/local_scale[i];

        *(s->slave_output->errorcode) = s->par_input->data_203F;

        *(s->slave_output->encoderpos) = s->par_input->data_2062 * (s->slave_output->encoderscale);

//        *(s->slave_output->mt_200b_0d) = s->par_input->data_200b;

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


        //驱动器IO刷新
        for(  n = 0;  n < IO_DOUTPUT_NUM;  n++ ) {

            tmp_int = (0x01<<n);
            if(  *(s->slave_output->out[n]) == 1 )
                s->par_output->data_2061 |= tmp_int;
            else
                s->par_output->data_2061 &= (~tmp_int);

        }
        for(  n = 0;  n < IO_DINPUT_NUM;  n++ ) {

            tmp_int = ( 0x01<<n );

            if( ( tmp_int & (s->par_input->data_2060) ) ) {     //滤波修改
                if(*(s->slave_input->in[n]) > 0) {
                    s->par_input->flitercount[n] = (s->slave_input->infliter[n]);
                } else {
                    if(s->par_input->flitercount[n] >= (s->slave_input->infliter[n])) {
                        *(s->slave_input->in[n])  = 1;
                    } else {
                        s->par_input->flitercount[n]++;
                    }
                }
            }else{
                if(*(s->slave_input->in[n]) == 0) {
                    s->par_input->flitercount[n] = 0;
                } else {
                    if(s->par_input->flitercount[n] <= 0) {
                        *(s->slave_input->in[n]) = 0;
                    } else {
                        s->par_input->flitercount[n]--;
                    }
                }
            }

        }

        //刷新探测位置
        //*(s->slave_output->probe_done) = testBit(s->par_input->data_probe_status,1);
    }


/*
    //extra_pos_data
    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {

        s = &motion_slaves[i];
        if( *(s->slave_output->alarm)!=1 && \
                s->par_input->data_6061 == 0x08 && \
                *(s->slave_output->enabled) == 1 && \
                testBit(s->par_input->data_6041,0) == 1 && \
                testBit(s->par_input->data_6041,2) == 1 && \
                testBit(s->par_input->data_6041,3) == 0 ) {

            s->par_output->data_607A = (int)((*(s->slave_input->vel))*local_scale[i]);

        }
        //if(i == 2)
        //rtapi_print("now I get scale %f   fb:%ld\n",local_scale[i],s->par_input->data_6064 );

        *(s->slave_output->fbposition) = (float)(s->par_input->data_6064 - s->par_output->data_60B0)/local_scale[i];

        *(s->slave_output->errorcode) = s->par_input->data_203F;
    }
*/
#ifdef USING_CIA402_HOME

    for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
        s = &motion_slaves[i];
        //检查回零指令；
        //回零只有在伺服使能已经打开了的情况下做
        if( *(s->slave_input->homecmd) == 1 && \
                s->last_slave_input.homecmd == 0 ) {
                if( *(s->slave_output->linkstate) == 0 || *(s->slave_output->alarm) == 1 ||
                    *(s->slave_output->enabled) == 0 ||  *(s->slave_input->homemethod) == 0 ) {
                } else {
                    rtapi_print("homecmd!!!++++++ homemethod=%d\n",*(s->slave_input->homemethod));
                    if(s->home_process_flag == 0) {
                        s->par_output->data_6060 = 0x06;
                        s->home_process_flag = 1;
                        s->home_process_count = 0;
                    }
                }
        }
        //启动回零
        if( s->home_process_flag == 1 ) {

            if(s->home_process_count ==  0) {
            } else if(s->home_process_count ==  1) {
                clearBit(s->par_output->data_6040,4);
                clearBit(s->par_output->data_6040,8);
            } else if(s->home_process_count ==  2) {
                setBit(s->par_output->data_6040,4);
            }  else if(s->home_process_count ==  3) {
                s->home_process_flag = 0;
                s->home_process_count = 0;
            }
            s->home_process_count ++;

        }
        s->last_slave_input.homecmd = *(s->slave_input->homecmd);

        //check home_done,only in home mode
        if( *(s->slave_output->homing) == 1 ) {

            if(  testBit(s->par_input->data_6041,10) == 1 && \
                 testBit(s->par_input->data_6041,12) == 1 &&
                s->last_slave_input.home_done_flag == 0) {
                  //target done ,may have errors or success,matchine stop
                  //回零完成，设置成CSP模式
                  rtapi_print("home is done!!!!\n");
                  if( testBit( s->par_input->data_6041,13) == 1)
                        *(s->slave_output->homeerror) = 1;//回零出错

                  *(s->slave_output->homedone) = 1;  //回零完成
                  s->par_output->data_60B0 = s->par_input->data_6064; //read the encoder
                  s->par_output->data_60B0 -= (int)((*(s->slave_input->vel))*local_scale[i]);
                  s->par_output->data_6060 = 0x08;
                  s->par_output->data_607A = (int)((*(s->slave_input->vel))*local_scale[i]);

             }

             if( testBit(s->par_input->data_6041,10) == 1 && \
                 testBit(s->par_input->data_6041,12) == 1 )
                    s->last_slave_input.home_done_flag = 1;
                else
                    s->last_slave_input.home_done_flag = 0;

        }

        //回零完成之后肯定会有一个清除坐标的一个过程
        //这个应该是motion 发下来的命令
        if( *(s->slave_input->clearpos) == 1 && \
            s->last_slave_input.clearpos == 0 ) {
            if( *(s->slave_output->linkstate) == 0 || *(s->slave_output->alarm) == 1 ||
                    *(s->slave_output->enabled) == 0  ) {
            } else {
                  s->par_output->data_60B0 = s->par_input->data_6064; //read the encoder
                  s->par_output->data_6060 = 0x08;
                  s->par_output->data_60B0 -= (int)((*(s->slave_input->vel))*local_scale[i]);
                  s->par_output->data_607A = (int)((*(s->slave_input->vel))*local_scale[i]);
            }
        }
        s->last_slave_input.clearpos = *(s->slave_input->clearpos);



        //回零过程中按了停止就进这里
        if( *(s->slave_input->homestop) == 1 && \
             s->last_slave_input.homestop == 0 && \
             *(s->slave_output->homing) == 1 ) {
            if( *(s->slave_output->linkstate) == 0 || *(s->slave_output->alarm) == 1 ||
                    *(s->slave_output->enabled) == 0  ) {
            } else {
                  clearBit(s->par_output->data_6040,4);
            }
        }
        s->last_slave_input.homestop = *(s->slave_input->homestop);

        //电机已经到达指定位置，完全停下来,防止电机跳一下的情况
        //注意这几个判断条件
        if( testBit(s->par_input->data_6041,10) == 1 && \
            s->last_slave_input.home_reach_flag == 0 && \
            *(s->slave_input->homestop) == 1 &&  *(s->slave_output->homing) == 1  ) {
              s->par_output->data_60B0 = s->par_input->data_6064; //read the encoder
              s->par_output->data_6060 = 0x08;
              s->par_output->data_60B0 -= (int)((*(s->slave_input->vel))*local_scale[i]);
              s->par_output->data_607A = (int)((*(s->slave_input->vel))*local_scale[i]);
         }
         s->last_slave_input.home_reach_flag = testBit(s->par_input->data_6041,10);

    }

#endif

    int k = 0;
    for(k=0;k<config_externio_num;k++) {
    if(extern_io_slaves[k].valid > 0) {
        int m = 0;
        for( m=0 ; m<extern_io_out_num[k]; m++) {
            for( n=0; n<32;n++) {
                if(  *(extern_io_slaves[k].slave_input[m]->out[n]) == 1 ) {
                    if(n<8)
                        extern_io_slaves[k].ext_io_out_0[m] |=  (0x01<<n);
                    else if(n>=8 && n<16)
                        extern_io_slaves[k].ext_io_out_1[m] |=  (0x01<<(n-8));
                    else if(n>=16 && n<24)
                        extern_io_slaves[k].ext_io_out_2[m] |=  (0x01<<(n-16));
                    else if(n>=24 && n<32)
                        extern_io_slaves[k].ext_io_out_3[m] |=  (0x01<<(n-24));
                }else {
                    if(n<8)
                        extern_io_slaves[k].ext_io_out_0[m] &=  ~(0x01<<n);
                    else if(n>=8 && n<16)
                        extern_io_slaves[k].ext_io_out_1[m] &=  ~(0x01<<(n-8));
                    else if(n>=16 && n<24)
                        extern_io_slaves[k].ext_io_out_2[m] &=  ~(0x01<<(n-16));
                    else if(n>=24 && n<32)
                        extern_io_slaves[k].ext_io_out_3[m] &=  ~(0x01<<(n-24));
                }
            }
        }

        for( m=0 ; m<extern_io_out16_num[k]; m++) {
            for( n=0; n<16;n++) {
                if(  *(extern_io_slaves[k].slave_input[m+extern_io_out_num[k]]->out[n]) == 1 ) {
                    if(n<8)
                        extern_io_slaves[k].ext_io_out_0[m+extern_io_out_num[k]] |=  (0x01<<n);
                    else if(n>=8 && n<16)
                        extern_io_slaves[k].ext_io_out_1[m+extern_io_out_num[k]] |=  (0x01<<(n-8));
                }else {
                    if(n<8)
                        extern_io_slaves[k].ext_io_out_0[m+extern_io_out_num[k]] &=  ~(0x01<<n);
                    else if(n>=8 && n<16)
                        extern_io_slaves[k].ext_io_out_1[m+extern_io_out_num[k]] &=  ~(0x01<<(n-8));
                }
            }
        }


//        printf("now I get input:%X %d %d \n",extern_io_slaves[k].ext_io_in_0[0],(extern_io_slaves[k].ext_io_in_0[m] &  (0x01<<n)));
           for( m=0 ; m<extern_io_in_num[k]; m++) {
            for( n=0; n<32;n++) {
                if(n<8) {
                    if(  (extern_io_slaves[k].ext_io_in_0[m] &  (0x01<<n)) > 0 )
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 1;
                     else
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 0;
                } else if(n>=8 && n<16) {
                    if(  (extern_io_slaves[k].ext_io_in_1[m] &  (0x01<<(n-8))) > 0 )
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 1;
                     else
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 0;
                } else if(n>=16 && n<24) {
                    if(  (extern_io_slaves[k].ext_io_in_2[m] &  (0x01<<(n-16))) > 0 )
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 1;
                     else
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 0;
                } else if(n>=24 && n<32) {
                    if(  (extern_io_slaves[k].ext_io_in_3[m] &  (0x01<<(n-24))) > 0 )
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 1;
                     else
                        (extern_io_slaves[k].slave_output[m]->recordin[n]) = 0;
                }

                if((extern_io_slaves[k].slave_output[m]->recordin[n]) > 0){
                    if(*(extern_io_slaves[k].slave_output[m]->in[n]) > 0) {
                        extern_io_slaves[k].slave_output[m]->flitercount[n] = extern_io_slaves[k].slave_output[m]->infliter[n];
                    } else {
                        if(extern_io_slaves[k].slave_output[m]->flitercount[n] >= extern_io_slaves[k].slave_output[m]->infliter[n]) {
                            *(extern_io_slaves[k].slave_output[m]->in[n])  = 1;
                        } else {
                            extern_io_slaves[k].slave_output[m]->flitercount[n] ++;
                        }
                    }
                } else {
                    if(*(extern_io_slaves[k].slave_output[m]->in[n]) == 0) {
                        extern_io_slaves[k].slave_output[m]->flitercount[n] = 0;
                    } else {
                        if(extern_io_slaves[k].slave_output[m]->flitercount[n] <= 0) {
                            *(extern_io_slaves[k].slave_output[m]->in[n])  = 0;
                        } else {
                            extern_io_slaves[k].slave_output[m]->flitercount[n] --;
                        }
                    }
                }

            }
        }

           for( m=0 ; m<extern_io_in16_num[k]; m++) {
            for( n=0; n<16;n++) {
                if(n<8) {
                    if(  (extern_io_slaves[k].ext_io_in_0[m+extern_io_in_num[k]] &  (0x01<<n)) > 0 )
                        (extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->recordin[n]) = 1;
                     else
                        (extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->recordin[n]) = 0;
                } else if(n>=8 && n<16) {
                    if(  (extern_io_slaves[k].ext_io_in_1[m+extern_io_in_num[k]] &  (0x01<<(n-8))) > 0 )
                        (extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->recordin[n]) = 1;
                     else
                        (extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->recordin[n]) = 0;
                }
                if((extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->recordin[n]) > 0){
                    if(*(extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->in[n]) > 0) {
                        extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->flitercount[n] = extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->infliter[n];
                    } else {
                        if(extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->flitercount[n] >= extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->infliter[n]) {
                            *(extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->in[n])  = 1;
                        } else {
                            extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->flitercount[n] ++;
                        }
                    }
                } else {
                    if(*(extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->in[n]) == 0) {
                        extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->flitercount[n] = 0;
                    } else {
                        if(extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->flitercount[n] <= 0) {
                            *(extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->in[n])  = 0;
                        } else {
                            extern_io_slaves[k].slave_output[m+extern_io_in_num[k]]->flitercount[n] --;
                        }
                    }
                }

            }
        }
    }
    }
    errorConnect = 0;
    for( i = 0; i < config_axis_num; i++ ) {
        if(*(motion_slaves[i].slave_output->linkstate) == 0) {
            errorConnect = 1;
            break;
        }
    }
    for(i=0;i<config_externio_num;i++) {
        if((extern_io_slaves[i].linkstate) == 0) {
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
    motion_sc = malloc( sizeof(ec_slave_config_t *) * ETH_MAX_SLAVES);
    extern_io_slaves = malloc(sizeof(EXTERN_IO_slave_config) * IO_MODULES_MAXNUM);
    io_sc = malloc( sizeof(ec_slave_config_t *) * IO_MODULES_MAXNUM);

    if(config_axis_num > ETH_MAX_SLAVES ) config_axis_num = ETH_MAX_SLAVES;

    initSlaveConfigStruct();
    initBusState();
    exportEthercatPar();
    exportAllPins();
    invalidAllSlaves();


    master = ecrt_request_master(0);
    int mostartnum = 0;
    if(link_type == 0) {
        mostartnum = 0;
    } else {
        mostartnum = config_externio_num;
    }

    if(master) {
        for( i = 0; i < config_axis_num; i++ ) {
            motion_slaves[i].valid = 0;
            motion_slaves[i].alias = 0;
            motion_slaves[i].position = mostartnum+i;
            if( config_file_path == NULL ) {
                rtapi_print_msg(RTAPI_MSG_ERR, "MOTION: config_file_path is empty \n");
                break;
            }
            rtapi_snprintf(buf, HAL_NAME_LEN, "%s/%d/axis-%02d.xml",config_file_path,motor_chose,i);
            MXml_read(&motion_slaves[i], buf);
        }
    }

    initEthercat();

    hal_ready(comp_id);



     return 0;
}

void rtapi_app_exit(void)
{
    slave_config_t *s;
    int j=0;
    for(j=0;j<ETH_MAX_SLAVES;j++) {
        s = &motion_slaves[j];
            s->par_output->data_6040 = 0x80;
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

             if(*(masterpins->slaveNum) > 0) {

                    if(SIMULATE == 0 )
                        printf("Now  reset master  %d\n",master_state.al_states);
                    ecrt_master_reset(master);

            }

        } else {

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

    *(masterpins->slaveNum) = ms.slaves_responding;
    master_state = ms;

    check_IO_state();

//    int i = 0;
//    if(ms.slaves_responding > 0) {
//        for( i = 0; i < ETH_MAX_SLAVES; i++ ) {
//            if( i < master_state.slaves_responding) {
//                ecrt_master_get_slave(master,i,motion_slaves[i].slaveinfo);
//                *(motion_slaves[i].slave_output->al_state) = motion_slaves[i].slaveinfo->al_state;
//                *(motion_slaves[i].slave_output->error_flag) = motion_slaves[i].slaveinfo->error_flag;
//            } else {
//                motion_slaves[i].slaveinfo->al_state = 0;
//                motion_slaves[i].slaveinfo->error_flag = 0;
//                *(motion_slaves[i].slave_output->al_state) = motion_slaves[i].slaveinfo->al_state;
//                *(motion_slaves[i].slave_output->error_flag) = motion_slaves[i].slaveinfo->error_flag;
//            }
//        }
//    }


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
    if( (!domain_pd) && (once_init == 1) ) return;

    // write application time to master



    timebefore =  rtapi_get_time();
    clock_gettime(CLOCK_TO_USE, &time);
    ecrt_master_application_time(master, TIMESPEC2NS(time));

    if( once_init == 0 ) {
        once_init = 1;
    }
    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[0]) < difftime) {
        *(masterpins->testtime[0]) = difftime;
    }



    timebefore =  rtapi_get_time();
    // receive process data
    ecrt_master_receive(master);
    ecrt_domain_process(domain);

    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[1]) < difftime) {
        *(masterpins->testtime[1]) = difftime;
    }


    // check process data state (optional)
    timebefore =  rtapi_get_time();
    check_bus_state();
    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[2]) < difftime) {
        *(masterpins->testtime[2]) = difftime;
    }


    timebefore =  rtapi_get_time();
    handleExchangeData();
    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
//    if(*(masterpins->testtime[3]) < difftime) {
        *(masterpins->testtime[3]) = difftime;
//    }

    //6040第9位设为1,复位驱动器滤波
    int rstmotor = 0;
    if(recordrstflag != *(masterpins->reset_data)) {
        if(*(masterpins->reset_data) == 1 && recordrstflag == 0) {
            rstmotor = 1;
        }
        recordrstflag = *(masterpins->reset_data);
    }

    timebefore =  rtapi_get_time();
    for( i = 0; i < config_axis_num; i++ ) {

        s = &(motion_slaves[i]);
        if(s->valid == 0) continue;

        if(rstmotor > 0) {
            motion_slaves[i].par_output->data_6040 = 0x20F;
        }

        if( s->off_data_6040 != -1) {
            EC_WRITE_U16(domain_pd + s->off_data_6040,\
                         motion_slaves[i].par_output->data_6040);
        }

        if( s->off_data_607A != -1 && s->slave_input->enable > 0) {
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

        if( s->off_data_2061 != -1) {
            EC_WRITE_U16(domain_pd + s->off_data_2061,\
                         motion_slaves[i].par_output->data_2061);
        }

        if( s->off_data_probe_ctl != -1 ) {
            EC_WRITE_U16(domain_pd + s->off_data_probe_ctl,\
                     motion_slaves[i].par_output->data_probe_ctl);
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
        if( s->off_data_2060 != -1 ) {
            s->par_input->data_2060 = EC_READ_U16(domain_pd + s->off_data_2060);
        }
        if( s->off_data_2062 != -1 ) {
            s->par_input->data_2062 = EC_READ_U16(domain_pd + s->off_data_2062);
        }
//        if( s->off_data_200b != -1 ) {
//            s->par_input->data_200b = EC_READ_U16(domain_pd + s->off_data_200b);
//        }

        if( s->off_data_probe_status != -1 ) {
            s->par_input->data_probe_status = EC_READ_U16(domain_pd + s->off_data_probe_status);
        }
        if( s->off_data_probe_pos != -1 ) {
            s->par_input->data_probe_pos = EC_READ_U32(domain_pd + s->off_data_probe_pos);
        }

    }

    int k=0;
    for(k=0;k<config_externio_num;k++) {
        if(extern_io_slaves[k].valid != 0) {
            for(i=0;i<IO_SLAVES_MAXNUM;i++) {
                if(extern_io_slaves[k].off_extern_out_0[i] != -1)
                    EC_WRITE_U8(domain_pd + extern_io_slaves[k].off_extern_out_0[i], extern_io_slaves[k].ext_io_out_0[i]);
                if(extern_io_slaves[k].off_extern_out_1[i] != -1)
                    EC_WRITE_U8(domain_pd + extern_io_slaves[k].off_extern_out_1[i], extern_io_slaves[k].ext_io_out_1[i]);
                if(extern_io_slaves[k].off_extern_out_2[i] != -1)
                    EC_WRITE_U8(domain_pd + extern_io_slaves[k].off_extern_out_2[i], extern_io_slaves[k].ext_io_out_2[i]);
                if(extern_io_slaves[k].off_extern_out_3[i] != -1)
                    EC_WRITE_U8(domain_pd + extern_io_slaves[k].off_extern_out_3[i], extern_io_slaves[k].ext_io_out_3[i]);

                if(extern_io_slaves[k].off_extern_in_0[i] != -1)
                    extern_io_slaves[k].ext_io_in_0[i] = EC_READ_U8(domain_pd + extern_io_slaves[k].off_extern_in_0[i]);
                if(extern_io_slaves[k].off_extern_in_1[i] != -1)
                    extern_io_slaves[k].ext_io_in_1[i] = EC_READ_U8(domain_pd + extern_io_slaves[k].off_extern_in_1[i]);
                if(extern_io_slaves[k].off_extern_in_2[i] != -1)
                    extern_io_slaves[k].ext_io_in_2[i] = EC_READ_U8(domain_pd + extern_io_slaves[k].off_extern_in_2[i]);
                if(extern_io_slaves[k].off_extern_in_3[i] != -1)
                    extern_io_slaves[k].ext_io_in_3[i] = EC_READ_U8(domain_pd + extern_io_slaves[k].off_extern_in_3[i]);
            }
        }
    }
    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[7]) < difftime) {
        *(masterpins->testtime[7]) = difftime;
    }

    if (sync_ref_counter) {
        sync_ref_counter--;
    } else {

        timebefore =  rtapi_get_time();

        sync_ref_counter = 1; // sync every cycle
        ecrt_master_sync_reference_clock(master);


        timeafter = rtapi_get_time();
        timeafter = rtapi_get_time();
        difftime = (long int)  (timeafter-timebefore);
        if(*(masterpins->testtime[4]) < difftime) {
            *(masterpins->testtime[4]) = difftime;
        }
    }

    timebefore =  rtapi_get_time();
    ecrt_master_sync_slave_clocks(master);
    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[5]) < difftime) {
        *(masterpins->testtime[5]) = difftime;
    }

    // send process data
    timebefore =  rtapi_get_time();
    ecrt_domain_queue(domain);
    ecrt_master_send(master);

    timeafter = rtapi_get_time();
    difftime = (long int)  (timeafter-timebefore);
    if(*(masterpins->testtime[6]) < difftime) {
        *(masterpins->testtime[6]) = difftime;
    }



    long long int now = rtapi_get_time();
    long int this_run = (long int) (now - last);
    *(masterpins->realruntime) = this_run;

    if(this_run > *(masterpins->maxtime)) {
        *(masterpins->maxtime) = this_run;
    }

}

void initEthercat() {
    int i,rv;

    if ( master ) {

        domain = ecrt_master_create_domain(master);

        int mostartnum = 0;
        if(link_type == 0) {
            mostartnum = 0;
        } else {
            mostartnum = config_externio_num;
        }

        if ( domain ) {

            if(init_externio_slaves() < 0 ) {
                rtapi_print_msg(RTAPI_MSG_ERR, _("ETHERCAT: ERROR: IO_Board activate failed\n"));
                invalidAllSlaves();
            }

            for( i = 0; i < config_axis_num; i++ ) {

//                printf("now get master:%d  %d %d %X\n",motion_slaves[i].alias,motion_slaves[i].position,motion_slaves[i].vendor_id,motion_slaves[i].product_code);
                if (!( motion_sc[i] = ecrt_master_slave_config(master,
                                motion_slaves[i].alias,motion_slaves[i].position,motion_slaves[i].vendor_id,motion_slaves[i].product_code))) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "AXIS:%d  Failed to get slave configuration.\n",i);
                   motion_slaves[i].valid = 0;
                } else motion_slaves[i].valid = 1;
                if(motion_slaves[i].valid == 0) continue;



                if (ecrt_slave_config_pdos(motion_sc[i], EC_END, motion_slaves[i].sync_info)) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "AXIS:%d  Failed to configure PDOs.\n",i);
                    motion_slaves[i].valid = 0;
                } else motion_slaves[i].valid = 1;
                if(motion_slaves[i].valid == 0) continue;


                rv = initSlaveDomainRegs(domain,&motion_slaves[i]) ;
                if( rv ) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "AXIS:%d  Failed to DomainRegs \n",i);
                    motion_slaves[i].valid = 0;
                } else motion_slaves[i].valid = 1;
                if(motion_slaves[i].valid == 0) continue;

                initSdosConfig(master,&motion_slaves[i]);


            }


            for( i = 0; i < config_externio_num; i++) {
//                printf("Now I in here ================= \n");
//                                ecrt_slave_config_dc(io_sc[i], 0, 0, 0, 0, 0);
//                ecrt_slave_config_dc(io_sc[i], 0X300, 1000000,  500000,0, 0);
            }

            for( i = 0; i < config_axis_num; i++ ) {
                if(motion_slaves[i].shifttime == 0)
                    motion_slaves[i].shifttime = 4400000;
                ecrt_slave_config_dc(motion_sc[i], motion_slaves[i].activate_word, PERIOD_NS, motion_slaves[i].shifttime, 0, 0);
            }

            if (ecrt_master_activate(master)) {
                rtapi_print_msg(RTAPI_MSG_ERR, _("ETHERCAT: ERROR: master activate failed\n"));
                invalidAllSlaves();
            }

            if (!(domain_pd = ecrt_domain_data(domain))) {
                rtapi_print_msg(RTAPI_MSG_ERR, _("ETHERCAT: ERROR: get domain data failed\n"));
                invalidAllSlaves();
            }

        }

    }

}

