/********************************************************************
* Description:  hal_ppmc.c
* genghaifei 2013.12.28
* Last changed: alidn.tu 2014-02-05
********************************************************************/
//#include <linux/slab.h>		/* kmalloc() */
#include "rtapi.h"		    /* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "hal.h"		    /* HAL public API decls */
//#include "parport_common.h"
#include "hal_parport.h"

#include<linux/errno.h>
//#include<linux/mm.h>
#include<linux/string.h>
#include<linux/kernel.h>
#include<linux/major.h>
#include<linux/fs.h>
//#include<linux/proc_fs.h>
#include<linux/nfs_fs.h>
//#include<linux/nfs_fs_sb.h>
#include<linux/nfs_mount.h>
//#include<linux/genhd.h>
//#include<linux/netdevice.h>
//#include<linux/if_arp.h>
//#include<linux/mc146818rtc.h>
//#include<linux/smp_lock.h>
//#include<linux/ide.h>
//#include<linux/delay.h>

#include <sys/uio.h>

#define MAX_BUS 3	/* max number of parports (EPP busses) */

#define	EPSILON		1e-20

/* module information */
MODULE_AUTHOR("CNEXT");
MODULE_DESCRIPTION("HAL driver for EPP Port Controller");
MODULE_LICENSE("GPL");

static int ab_pd_sel = 0;	/* default ab or dir+pluse */
RTAPI_MP_INT(ab_pd_sel, "default ab or dir+pluse sel");

int port_addr[MAX_BUS] = { 0x378, [1 ... MAX_BUS-1] = -1 };
    /* default, 1 bus at 0x0378 */
hal_parport_t port_registration[MAX_BUS];
RTAPI_MP_ARRAY_INT(port_addr, MAX_BUS, "port address(es) for EPP bus(es)");
int extradac[MAX_BUS*8] = { //实际没有用到，屏蔽了
        -1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1 };  /* default, no extra stuff */
RTAPI_MP_ARRAY_INT(extradac, MAX_BUS*8, "bus/slot locations of extra DAC modules");
int extradout[MAX_BUS*8] = { //实际没有用到，屏蔽了
        -1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1 };  /* default, no extra stuff */
RTAPI_MP_ARRAY_INT(extradout, MAX_BUS*8, "bus/slot locations of extra dig out modules");

/***********************************************************************
*                DEFINES (MOSTLY REGISTER ADDRESSES)                   *
************************************************************************/

#define SPPDATA(addr)       addr        //SPP数据只能写
#define STATUSPORT(addr)    addr+1      //SPP状态只能读
#define CONTROLPORT(addr)   addr+2      //SPP控制寄存器 只能写
#define ADDRPORT(addr)      addr+3      //EPP地址寄存器 可读写
#define DATAPORT(addr)      addr+4      //EPP数据寄存器 可读写

#define NUM_SLOTS      1            //原来是16
#define SLOT_SIZE      16

#define ENCISR      0x0C	/* index detect latch register (read only) */

/* Write defines base addr 0x40 ghf-2014/7/24 */
#define RATE_NUM_0      0x00    // pulse_0 generator
#define RATE_NUM_1      0x02    // pulse_1 generator
#define RATE_NUM_2      0x04    // pulse_2 generator
#define RATE_NUM_3      0x06    // pulse_3 generator
#define RATE_DIR        0x08    // bit7-bit4: encoder index reset (instead of ENCINDX,write only)
                                // bit3-bit0: dir of pulse generator

#define UxC_EXTRA       0x09    /* 16 bit DAC 0x09-0x0A */


#define SRV_ON_CLR      0x0B  //bit3-bit0:axis0~axis3  SRV-ON;
                              //bit7-bit4:axis0~axis3  SRV-CLR;

#define UxC_DOUTA       0x0C
#define UxC_DOUTB       0x0D
#define UxC_DOUTC       0x0E
#define UxC_DOUTD       0x0F


/* Read defines  base addr 0x20 ghf-2014/7/24 */
#define ENCCNT0         0x00	/* Encoder_0 count */
#define ENCCNT1         0x03    /* Encoder_1 count */
#define ENCCNT2         0x06    /* Encoder_2 count */
#define ENCCNT3         0x09    /* Encoder_3 count */

/* Handle */
#define HANDLE_STATE      	  0x0C  //bit7-bit4: index detect latch register (read only)
                                    //bit1-bit0: HANDLE_XYZ  00 x/ 01 y /10 z/11 4
                                    //bit3-bit2: HANDLE_SCALE
#define HANDLE_COUNT_LOW      0x0D
#define HANDLE_COUNT_HIGH     0x0E

#define SRVALM_HOMESW   0X0F //Bit3-bit0:axis3~axis0  SRV-ALM
                             //Bit7-bit4:axis3~axis0  home_sw

#define UxC_DINA        0x10
#define UxC_DINB        0x11
#define UxC_DINC        0x12
#define UxC_DIND        0x13
#define UxC_DINE        0x14
#define UxC_DINF        0x15
#define UxC_DING        0x16

#define FPGASTATUS      0x17

/*以下部分为一开始上电时读取一次的数据
#define CUSTOMID       0X80  //2个字节
#define RLEFTTIME      0X82  //4个字节，单位为10min
#define ENCRY_CHIPID   0X86  //8个字节，241B-4003-0203-9067
#define ENCRY_KEY      0X8E  //64个字节

#define ENCCNT_SAVER0    0XCE
#define ENCCNT_SAVER1    0XD1
#define ENCCNT_SAVER2    0XD4
#define ENCCNT_SAVER3    0XD7*/


/* codes to indicate if/how the extra port is being used */
#define EXTRA_UNUSED	0
#define	EXTRA_DAC		1

/***********************************************************************
*                       STRUCTURE DEFINITIONS                          *
************************************************************************/

/* this structure contains the runtime data for a digital output */
typedef struct {
    hal_bit_t *data;		/* output pin value */
    hal_bit_t *invert;		/* parameter to invert output pin */
} dout_t;

/* this structure contains the runtime data for a digital input */
typedef struct {
    hal_bit_t *data;		/* input pin value */
    hal_bit_t *data_not;	/* inverted input pin value */
    hal_bit_t *data_before;//ghf-2014/5/24
} din_t;

/* this structure contains the runtime data for 32bits digital input and output*/
typedef struct {
    hal_u32_t *data_in[2];		/* 56 bits input pin value */
    hal_u32_t *data_out[2];	/* 40 bits output pin value */
} dio_t;

/* this structure contains the runtime data for a step pulse generator */
typedef struct {
    hal_bit_t *enable;		/* enable pin for step generator */
    hal_float_t *vel;		/* velocity command pin (mm) */
    hal_float_t scale;		/* parameter: scaling for vel to Hz (pulse/mm) */
    hal_float_t scaledir;          /* parameter: scale dir (pulse/mm) */
    hal_float_t max_vel;	/* velocity limit */
    hal_float_t freq;		/* parameter: velocity cmd scaled to pulse number
                                freq = (long)( vel * scale) - pulse_add[n] */
    hal_float_t totleplusediff;
    hal_float_t *limitvel; //碰到硬限位时的速度值ghf-20150710
    hal_float_t *realvel;
} stepgen_t;

/* runtime data for a set of 4 step pulse generators */
typedef struct {
    stepgen_t sg[4];		    /* per generator data */
    hal_u32_t setup_time_ns;	/* setup time in nanoseconds */
    hal_u32_t pulse_width_ns;	/* pulse width in nanoseconds */
    hal_u32_t pulse_space_ns;	/* min pulse space in nanoseconds */
    hal_s32_t *tool_number;     //以下5个是QZ加的
    hal_s32_t *set_which_z;      //z1/z2轴标志ghf-2014/2/26
    hal_float_t maxerror;
    hal_float_t minerror;
    hal_float_t maxvel;	        //跟stepgen_t.max_vel有什么区别
    hal_u32_t ab_pd_sel;	/* select ab_phase or pulse+dir (2014-5-28)
                                2:ab_phase;1:pulse+dir  */
} stepgens_t;

/* this structure contains the runtime data for a 16-bit DAC */
typedef struct {
    hal_float_t *value;		/* value command pin */
    hal_float_t scale;		/* parameter: scaling */
} DAC_t;

/* runtime data for an "extra" port - can be either a DAC, or 8 digouts */
typedef union {
    DAC_t  dac;			/* if the port is used for a DAC */
} extra_t;

// handle
typedef struct {
    hal_s32_t *count[4];
    hal_bit_t dir[4];
    unsigned char *state;   //ghf-2014/5/9
    unsigned char *state_pre;   //alidn-20160227
    hal_bit_t *Z2_flag;                  //alidn-20160227
    signed long oldreading;    /* used to detect overflow / underflow of the counter */
    hal_bit_t handleztype;    //hong 2015-08-26 use for distinguish Z control mode
} handle_t;

/* runtime data for a single encoder */
typedef struct {
    hal_float_t *position;      /* output: scaled position pointer */
    hal_s32_t *count;           /* output: unscaled encoder counts */
    hal_s32_t *delta;		    /* output: delta counts since last read */
    hal_float_t scale;          /* parameter: scale factor (pulse/mm) */
    hal_float_t scaledir;          /* parameter: scale dir (pulse/mm) */
    hal_bit_t *index;           /* output: index flag (form FPGA) */
    hal_bit_t *index_enable;    /* enable index pulse to reset encoder count (send to FPGA) */
    hal_float_t *spindle_value; // spindle
    signed long oldreading;     /* used to detect overflow / underflow of the counter */
    unsigned int indres;        /* copy of reset-on-index register bits (only valid on 1st encoder of board)*/
    unsigned int indrescnt;     /* counts servo cycles since index reset was turned on */
    //hal_float_t *encoderdiff;
} encoder_t;

//ghf-2014/2/28
typedef struct {
    hal_s32_t *clock_count;
    hal_s32_t *clock_max;
    hal_s32_t *clock_min;
    hal_s32_t *times;
    hal_s32_t *times_max;
    hal_s32_t *times_min;
    hal_s32_t *times_avg;
    hal_u32_t *filtertimebuf[64];//ghf-2014/5/6
    //ghf-2014/7/24
    /*hal_bit_t *customid[2];//客户编号
    hal_bit_t *rlefttime[4];//剩余时间
    hal_bit_t *encrychipid[8];//加密芯片id
    hal_bit_t *encryptbuf[64];//密文密钥
    hal_bit_t *rencryflag;//刚上电初始化读写完成后置为1 ghf-2014/8/4

    hal_bit_t *cpuid[8];//cpuid
    hal_bit_t *hddid[4];//硬盘id
    hal_bit_t *wlefttime[4];//重置剩余时间
    hal_bit_t *wlefttimeflag;//为1,终止现有读写，写时间下去
    hal_bit_t *widflag;//为1,写id下去*/
    hal_u32_t *fpgastatus;//ghf-2014/8/25

} clocks_t;

#define MAX_FUNCT 10

struct slot_data_s;

typedef void (slot_funct_t)(struct slot_data_s *slot);

typedef struct slot_data_s {
    unsigned char slot_base;	/* base addr of this 16 byte slot */
    unsigned int port_addr; 	/* addr of parport */


    __u32 read_bitmap;		    /* map showing which registers to read */
    unsigned char num_rd_functs;/* number of read functions */
    unsigned char rd_buf[32];	/* cached data read from epp bus */
    slot_funct_t *rd_functs[MAX_FUNCT];	    /* array of read functions 读数据函数数组*/
    __u32 write_bitmap;		    /* map showing which registers to write */
    unsigned char num_wr_functs;/* number of write functions */
    unsigned char wr_buf[32];	/* cached data to be written to epp bus */
    slot_funct_t *wr_functs[MAX_FUNCT];	/* array of write functions */
    dout_t *digout;		/* ptr to shmem data for digital outputs */
    din_t *digin;		/* ptr to shmem data for digital inputs */
    dio_t *digio;       /* ptr to shmem data for digital inputs and outputs */
    stepgens_t *stepgen;    /* ptr to shmem data for step generators */
    encoder_t *encoder;     /* ptr to shmem data for encoders */
    clocks_t *clocks; //ghf-2014/2/28
    handle_t *handles;      /* ptr to shmem data for handles */
    int extra_mode;         /* indicates if/how "extra" port is used */
    extra_t *extra;         /* ptr to shmem for "extra" pvalueort */
} slot_data_t;

/* this structure contains the runtime data for a complete EPP bus */
typedef struct {
    int busnum;			/* bus number */
    unsigned char have_master;	/* true if a master has been configured */
    unsigned int last_digout;	/* used for numbering digital outputs */
    unsigned int last_digin;	/* used for numbering digital inputs */
    unsigned int last_stepgen;	/* used for numbering step generators */
    unsigned int last_encoder;	/* used for numbering encoders */
    unsigned int last_handle;   /* used for numbering handle */
    unsigned int last_extraDAC;	/* used for numbering DACs */
    char slot_valid[NUM_SLOTS];	/* tags for slots that are used NUM_SLOTS=1*/
    slot_data_t slot_data[NUM_SLOTS];  /* data for slots on EPP bus */
} bus_data_t;

/***********************************************************************
*                          GLOBAL VARIABLES                            *
************************************************************************/

static bus_data_t *bus_array[MAX_BUS];  //MAX_BUS=3
static int comp_id;		/* component ID */

/***********************************************************************
*                    REALTIME FUNCTION DECLARATIONS                    *
************************************************************************/

static void read_all(void *arg, long period);
static void write_all(void *arg, long period);

static void read_digins(slot_data_t *slot);
static void write_digouts(slot_data_t *slot);
static void write_stepgens(slot_data_t *slot);
static void read_encoders(slot_data_t *slot);
static void write_extraDAC(slot_data_t *slot);

/***********************************************************************
*                  REALTIME I/O FUNCTION DECLARATIONS                  *
************************************************************************/

static int ClrTimeout(unsigned int port_addr);  //EPP1.9 define STATUS_PORT bit0:Timeout
static unsigned short SelRead(unsigned char epp_addr, unsigned int port_addr);
static unsigned short ReadMore(unsigned int port_addr);
static void SelWrt(unsigned char byte, unsigned char epp_addr, unsigned int port_addr);
static void WrtMore(unsigned char byte, unsigned int port_addr);

/***********************************************************************
*                  LOCAL FUNCTION DECLARATIONS                         *
************************************************************************/

static __u32 block(int min, int max);
static int add_rd_funct(slot_funct_t *funct, slot_data_t *slot, __u32 cache_bitmap );
static int add_wr_funct(slot_funct_t *funct, slot_data_t *slot, __u32 cache_bitmap );

static int export_UxC_digin(slot_data_t *slot, bus_data_t *bus);
static int export_UxC_digout(slot_data_t *slot, bus_data_t *bus);
static int export_USC_stepgen(slot_data_t *slot, bus_data_t *bus);
static int export_encoders(slot_data_t *slot, bus_data_t *bus);
static int export_extra_dac(slot_data_t *slot, bus_data_t *bus);
static int export_handle(slot_data_t *slot,bus_data_t * bus);
static int export_clock(slot_data_t *slot,bus_data_t * bus);//ghf-2014/2/28

/***********************************************************************
*                       INIT AND EXIT CODE                             *
************************************************************************/

int rtapi_app_main(void)
{
    int msg, rv, rv1, busnum, slotnum, n, boards;
    bus_data_t *bus;
    slot_data_t *slot;
    char buf[HAL_NAME_LEN + 2];
    int i=0;

    /* connect to the HAL */
    comp_id = hal_init("hal_ppmc");
    if (comp_id < 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: hal_init() failed\n");
        return -1;
    }
    //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC: installing driver\n");

    msg = rtapi_get_msg_level();

    /* validate port addresses */
    n = 0;
    rv = 0;
    for ( busnum = 0 ; busnum < MAX_BUS ; busnum++ ) {
        /* init pointer to bus data */
        bus_array[busnum] = NULL;
        /* check to see if a port address was specified */
        if ( port_addr[busnum] == -1 ) {
            /* nope, skip it */
            continue;
        }
        /* is it legal? */
        if ( port_addr[busnum] > 65535 ) {  // ADDR less than 0xFFFF
            /* nope, complain and skip it */
            rtapi_print_msg(RTAPI_MSG_ERR,
                   "PPMC: ERROR: invalid port_addr: %0X\n", port_addr[busnum]);
            rv = -1;
            continue;
        }
        /* got a good one */
        n++; // number of the specified port(s)
    }
    if ( n == 0 ) {
        rtapi_print_msg(RTAPI_MSG_ERR,	"PPMC: ERROR: no ports specified\n");
        hal_exit(comp_id);
        return -1;
    }
    /* have valid config info */
    /* begin init - loop thru all busses */
    for ( busnum = 0 ; busnum < MAX_BUS ; busnum++ ) {
        /* check to see if a port address was specified */
        if ( port_addr[busnum] == -1 ) {
            /* nope, skip to next bus */
            continue;
        }
        //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC: checking EPP bus %d at port %04X\n",
       //           busnum, port_addr[busnum]);
        boards = 0;
        /* allocate memory for bus data - this is not shared memory */
        bus = malloc(sizeof(bus_data_t));
        if (bus == 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: kmalloc() failed\n");
            rv = -1;
            /* skip to next bus */
            continue;
        }

        /* clear the bus data structure */
        bus->busnum = busnum;
        bus->last_digout = 0;
        bus->last_digin = 0;
        bus->last_stepgen = 0;
        bus->last_encoder = 0;
        bus->last_extraDAC = 0;
        bus->last_handle = 0;

        /* clear the slot data structures (part of the bus struct) */
        for ( slotnum = 0 ; slotnum < NUM_SLOTS ; slotnum ++ ) {    // NUM_SLOTS = 1
            /* clear slot valid flag in bus struct */
            bus->slot_valid[slotnum] = 0;
            /* point to slot struct */
            slot = &(bus->slot_data[slotnum]);
            /* clear stuff */
            slot->slot_base = slotnum * SLOT_SIZE;
            slot->port_addr = port_addr[busnum];
            slot->read_bitmap = 0;
            slot->write_bitmap = 0;
            /* clear EPP read and write caches */
            for ( n = 0 ; n < 32 ; n++ ) {
                slot->rd_buf[n] = 0;
                slot->wr_buf[n] = 0;
            }
            /* clear function pointers */
            slot->num_rd_functs = 0;
            slot->num_wr_functs = 0;
            for ( n = 0 ; n < MAX_FUNCT ; n++ ) {
                slot->rd_functs[n] = NULL;
                slot->wr_functs[n] = NULL;
            }
            slot->digout = NULL;
            slot->digin = NULL;
            slot->stepgen = NULL;
            slot->encoder = NULL;
            slot->handles = NULL;
            slot->extra_mode = EXTRA_UNUSED;
            slot->extra = NULL;
        }

        /* scan the bus */
        for ( slotnum = 0 ; slotnum < NUM_SLOTS ; slotnum ++ ) {
            /* point to slot struct */
            slot = &(bus->slot_data[slotnum]);
            /* rv1 is used to flag errors that fail one bus */
            rv1 = 0;
            //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC: slot %d: ", slotnum);
            bus->slot_valid[slotnum] = 1;
            boards++;
            rv1 += export_USC_stepgen(slot, bus);
            rv1 += export_UxC_digout(slot, bus);
            rv1 += export_UxC_digin(slot, bus);
            rv1 += export_encoders(slot, bus);
            rv1 += export_extra_dac(slot, bus);
            rv1 += export_handle(slot, bus);
            rv1 += export_clock(slot,bus);//ghf-2014/2/28

            //rtapi_print_msg(RTAPI_MSG_INFO,"read cache bitmap: %08x\n", slot->read_bitmap );
            //rtapi_print_msg(RTAPI_MSG_INFO,"write cache bitmap: %08x\n", slot->write_bitmap );
        } /* end of slot loop */

        if ( rv1 != 0 ) {
            /* error during slot scan, already printed */
            rv = -1;
            /* skip to next bus */
            continue;
        }

        if ( boards == 0 ) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: no boards found on bus %d, port %04X\n",
                   busnum, port_addr[busnum] );
            rv = -1;
            /* skip to next bus */
            continue;
        }

        /*alidn.tu add for pulse+direction or a/b phase,2014-5-7*/

        /*	ADDR:	0x00
            bit1: 0--fwd;1--rev
            bit0: 0--a/b;1--pulse+dir	*/

        slot->stepgen->ab_pd_sel = (unsigned int)ab_pd_sel;

        //rtapi_print("in PPMC: slot->stepgen->ab_pd_sel = %d\n",slot->stepgen->ab_pd_sel);

        SelWrt(slot->stepgen->ab_pd_sel | 0x0C/*ab or dir*/, 0x02, slot->port_addr);

        /*#########add by ghf-2014/7/24#########*/
        /*ClrTimeout(slot->port_addr);
        outb(0x04,CONTROLPORT(slot->port_addr));
        outb(CPUID,ADDRPORT(slot->port_addr));
        for(i=0;i<8;i++) {
            outb(*(slot->clocks->cpuid[i]),DATAPORT(slot->port_addr));
        }

        ClrTimeout(slot->port_addr);
        outb(0x04,CONTROLPORT(slot->port_addr));
        outb(CUSTOMID,ADDRPORT(slot->port_addr));
        outb(0x24,CONTROLPORT(slot->port_addr));
        for(i=0;i<2;i++) {
            *(slot->clocks->customid[i]) = inb(DATAPORT(slot->port_addr));
        }*/
        //*(slot->clocks->customid[0]) = 0x05;
        //*(slot->clocks->customid[1]) = 0x06;

        //*(slot->clocks->rencryflag) = 0x01;//此处赋值给不过去motion
        /*#########add by ghf-2014/7/24#########*/


        /* export functions */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.read", busnum);
        rv1 = hal_export_funct(buf, read_all, &(bus_array[busnum]), 1, 0, comp_id);
        if (rv1 != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: read funct export failed\n");
            rv = -1;
            /* skip to next bus */
            continue;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.write", busnum);
        rv1 = hal_export_funct(buf, write_all, &(bus_array[busnum]), 1, 0, comp_id);
        if (rv1 != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: write funct export failed\n");
            rv = -1;
            /* skip to next bus */
            continue;
        }

        /* save pointer to bus data */
        bus_array[busnum] = bus;
        //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC: bus %d complete\n", busnum);
    }

    /* restore saved message level */
    rtapi_set_msg_level(msg);
    /* final check for errors */
    if ( rv != 0 ) {
        /* something went wrong, cleanup and exit */
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: shutting down\n");
        for ( busnum = 0 ; busnum < 1 ; busnum++ ) {
            /* check to see if memory was allocated for bus */
            if ( bus_array[busnum] != NULL ) {
                /* save ptr to memory block */
                bus = bus_array[busnum];
                /* mark it invalid so RT code won't access */
                bus_array[busnum] = NULL;
                /* and free the block */
                free(bus);
            }
        }
        /* disconnect from HAL */
        hal_exit(comp_id);
        return -1;
    }

    //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC: driver installed\n");
    hal_ready(comp_id);
    return 0;
}

void rtapi_app_exit(void)
{
    int busnum;
    bus_data_t *bus;

    //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC: shutting down\n");
    for ( busnum = 0 ; busnum < 1 ; busnum++ ) {
        /* check to see if memory was allocated for bus */
        if ( bus_array[busnum] != NULL ) {
            /* save ptr to memory block */
            bus = bus_array[busnum];
            /* mark it invalid so RT code won't access */
            bus_array[busnum] = NULL;
            /* free the memory block */
            free(bus);
        }
    }

    /* disconnect from HAL */
    hal_exit(comp_id);
}

/***********************************************************************
*                         REALTIME FUNCTIONS                           *
************************************************************************/

static void read_all(void *arg, long period)
{
    bus_data_t *bus;
    slot_data_t *slot;
    int slotnum, functnum, addr_ok;
    unsigned char n, eppaddr,xorcheck = 0,addcheck = 0,i=0,read_buf_ok =0;
    __u32 bitmap;

    int j=0;
    stepgen_t *sg;//ghf-2014/5/30

    static int err_count_read = 0;//局部静态变量在程序退出后仍然能把值保留下来，因为其存放在全局区域
    //alidn, change err_count1 to err_count_read
    unsigned int read_middle_buf[19];

    static long long int startclocks=0,last_startclocks=0;//ghf-2014/2/28
    static long long int times = 0;
    static int flag = 1;

    /* get pointer to bus data structure */
    bus = *(bus_data_t **)(arg);
    /* test to make sure it hasn't been freed */
    if ( bus == NULL ) {
        return;
    }
    /* loop thru all slots */
    for ( slotnum = 0 ; slotnum < NUM_SLOTS ; slotnum++ ) {
        /* check for anthing in slot */
        if ( bus->slot_valid[slotnum] ) {
            /* point at slot data */
            slot = &(bus->slot_data[slotnum]);

            i = 0;
            read_buf_ok = 0;

            startclocks = rtapi_get_time();//ghf

            do {
                i++;
                xorcheck = 0xAA;
                addcheck = 0;
                /* fetch data from EPP to cache */
                addr_ok = 0;
                bitmap = slot->read_bitmap;
                bitmap |= 0x1000000;
                bitmap |= 0x2000000;    //共26字节ghf-2014/8/19
                n = 0;


                while ( bitmap ) { // 26 bytes, with check code
                    if ( bitmap & 1 ) {
                        /* need to read register 'n' */
                        if ( addr_ok ) {
                            read_middle_buf[n] = inw(DATAPORT(slot->port_addr));//读两个字节

                            //rtapi_print("in read_all,read_middle_buf[%d]=%x,bitmap=%x\n",n,read_middle_buf[n],bitmap);

                            /* next register */
                            n++;
                            bitmap >>= 2;
                        } else {//第一次进来执行这里，只设置地址和方向，不发实际数据
                            /* need to specify address */
                            eppaddr = slot->slot_base + n + 0x20;   //Read base address = 0x40, alidn
                            ClrTimeout(slot->port_addr);
                            /* set port direction to output */
                            outb(0x04,CONTROLPORT(slot->port_addr));
                            /* write epp address to port */
                            outb(eppaddr,ADDRPORT(slot->port_addr));
                            /* set port direction to input */
                            outb(0x24,CONTROLPORT(slot->port_addr));
                            /* mark auto-incrementing address as valid */
                            addr_ok = 1;
                        }
                    } else {
                        /* don't need to read this register */
                        /* mark auto-incrementing address as invalid */
                        addr_ok = 0;
                    }
                }


                /*xor check*/
                for(n=0;n<12;n++)
                {
                    xorcheck ^= (read_middle_buf[n] & 0x000000FF);
                    xorcheck ^= ((read_middle_buf[n]>> 8) & 0x000000FF);
                    addcheck += (read_middle_buf[n] & 0x000000FF);
                    addcheck += ((read_middle_buf[n]>> 8) & 0x000000FF);
                }

                //第25个是异或校验,第26个是和校验
                if ((xorcheck != (read_middle_buf[12] & 0x000000FF))
                    || (addcheck != ((read_middle_buf[12]>>8) & 0x000000FF))) {
                    err_count_read ++;
                    if(err_count_read < 10) {
                        rtapi_print("err_count_read = %d read_middle_buf[12]=%x\n",err_count_read,read_middle_buf[12]);
                    }
                    else if(err_count_read >= 10) {
                        if(flag == 1)
                            rtapi_print_msg(RTAPI_MSG_ERR,"EPP read error,please check EPP mode in bios\n");
                        flag = -1;
                    }

                }else {
                    err_count_read = 0;
                    read_buf_ok = 1;
                }
            }while(((xorcheck != (read_middle_buf[12] & 0x000000FF))
                || (addcheck != ((read_middle_buf[12]>>8) & 0x000000FF))) && (i<3));
            //}while(i<1);
            //当校验出错小于3次时，重新接收

            last_startclocks =  rtapi_get_time();//ghf

            if(times != 0) {
                *(slot->clocks->times) =  last_startclocks - startclocks;
               *(slot->clocks->times_max) = (*(slot->clocks->times) > *(slot->clocks->times_max)) ?
                                   *(slot->clocks->times) : *(slot->clocks->times_max);
               *(slot->clocks->times_min) = *(slot->clocks->times);
                //*(slot->clocks->times_min) = (*(slot->clocks->times) < *(slot->clocks->times_min)) ?
                  //                 *(slot->clocks->times) : *(slot->clocks->times_min);
            }
            else {
                *(slot->clocks->times_min) = 2000000000;
                *(slot->clocks->times_max) = 0;
            }

            last_startclocks = startclocks;
            times++;
            if(times ==0xffffffff)
                times=0;

            if (read_buf_ok == 1){
                for (n=0; n<26; n++){
                    slot->rd_buf[n] = ((read_middle_buf[n/2] >> (n%2*8)) & 0x000000FF);
                }
            }

            if((xorcheck == slot->rd_buf[24])&&(addcheck == slot->rd_buf[25])) {
                for ( functnum = 0 ; functnum < slot->num_rd_functs ; functnum++ ) {
                    /* call function 将读回的数据通过调用各个函数反馈到上层*/
                    (slot->rd_functs[functnum])(slot);
                }
            }

        }//if ( bus->slot_valid[slotnum] )
    }//for ( slotnum = 0 ; slotnum < NUM_SLOTS ; slotnum++ )





}

static void write_all(void *arg, long period)
{
    bus_data_t *bus;
    slot_data_t *slot;
    int slotnum, functnum, addr_ok;
    unsigned char n, eppaddr, xorcheck_write, addcheck_write, i=0;
    __u32 bitmap;
    static int err_count_write = 0;
    unsigned int word, xor_tmp;

    int j=0;
    static int flag = 1;

    static long long int startclocks=0,last_startclocks=0;//ghf-2014/2/28
    static long long int times = 0;


    /* get pointer to bus data structure */
    bus = *(bus_data_t **)(arg);
    /* test to make sure it hasn't been freed */
    if ( bus == NULL ) {
        return;
    }
    /* loop thru all slots */
    for ( slotnum = 0 ; slotnum < NUM_SLOTS ; slotnum++ ) { //NUM_SLOTS = 1
        /* check for anthing in slot */
        if ( bus->slot_valid[slotnum] ) {   //init set 1
            /* point at slot data */
            slot = &(bus->slot_data[slotnum]);
            /* loop thru all functions associated with slot */
            for ( functnum = 0 ; functnum < slot->num_wr_functs ; functnum++ ) {
                /* call function */
                (slot->wr_functs[functnum])(slot); //填充wr_buf[i]
            }

            /*xor and add check*/
            xorcheck_write = 0xAA;
            addcheck_write = 0;
            for(n=0;n<16;n++) { //I think it should be a var like NUM_BYTE_WRITE, alidn 2014-02-07
                xorcheck_write ^= slot->wr_buf[n];
                addcheck_write += slot->wr_buf[n];
            }
            slot->wr_buf[16] = xorcheck_write;
            slot->wr_buf[17] = addcheck_write;

            i = 0;

            startclocks = rtapi_get_time();//ghf

            do{
                i++;
                /* write data from cache to EPP */
                addr_ok = 0;
                bitmap = slot->write_bitmap;
                bitmap |= 0x00010000;
                bitmap |= 0x00020000; //低18位都为1
                n = 0;
                while ( bitmap ) {
                    if ( bitmap & 1 ) {
                        /* need to write data register 'n' */
                        if ( addr_ok ) {    //连续执行9次，把数据连同校验码一起发下去
                            /* auto-increment address is usable */
                            word = slot->wr_buf[n+1];
                            word <<= 8;
                            word |= slot->wr_buf[n];
                            outw(word,DATAPORT(slot->port_addr));
                            /* next register */
                            n += 2;
                            bitmap >>= 2;
                            //rtapi_print("in write_all,word=%x\n",word);
                        } else {            //第一次进来addr_ok=0,写起始地址下去
                            /* need to specify address */
                            eppaddr = slot->slot_base + n + 0x40;   //Write base address = 0x10
                            ClrTimeout(slot->port_addr);
                            /* set port direction to output */
                            outb(0x04,CONTROLPORT(slot->port_addr));
                            /* write epp address to port */
                            outb(eppaddr,ADDRPORT(slot->port_addr));
                            /* mark auto-incrementing address as valid */
                            addr_ok = 1;
                        }
                    } else {
                        /* don't need to write this one */
                        /* mark auto-incrementing address as invalid */
                        addr_ok = 0;
                    }
                } //while

                /* Read the check code from FPGA */
                ClrTimeout(slot->port_addr);
                /* set port direction to output */
                outb(0x04,CONTROLPORT(slot->port_addr));
                /* write epp address to port */
                outb(0x50,ADDRPORT(slot->port_addr));//读fpga 0x50 0x51处的check code
                /* set port direction to input */
                outb(0x24,CONTROLPORT(slot->port_addr));
                xor_tmp = inw(DATAPORT(slot->port_addr));

                //rtapi_print("in write_all,xor_tmp = %x xorcheck_write=%x addcheck_write=%x\n",xor_tmp,xorcheck_write,addcheck_write);

                //发的时候发了16个字节，fpga收到后把校验码发上来，
                //上位机读这两个字节，跟xorcheck_write addcheck_write比较，这样校验才有用
                if (((xor_tmp & 0x000000FF) != xorcheck_write) ||
                        (((xor_tmp >> 8) & 0x000000FF) != addcheck_write)) {
                    err_count_write ++ ;

                    if(err_count_write < 10) {
                        rtapi_print("err_count_write = %d xor_tmp = %x\n",err_count_write,xor_tmp);
                    }
                    else if(err_count_write >= 10) {
                        if(flag == 1)
                            rtapi_print_msg(RTAPI_MSG_ERR,"EPP write error,please check EPP mode in bios\n");
                        flag = -1;
                    }
                }
                else{
                    err_count_write = 0;
                }
            } while ((((xor_tmp & 0x000000FF) != xorcheck_write) ||
                (((xor_tmp >> 8) & 0x000000FF) != addcheck_write)) && (i<3));//有错时，再发3次
            //}while(i<1);
            //当校验出错小于3次时，重新接收

            last_startclocks =  rtapi_get_time();//ghf

            if(times != 0) {
                *(slot->clocks->times) =  last_startclocks - startclocks;
               *(slot->clocks->times_max) = *(slot->clocks->times);
            }
            else {
                *(slot->clocks->times_min) = 2000000000;
                *(slot->clocks->times_max) = 0;
            }

            last_startclocks = startclocks;
            times++;
            if(times ==0xffffffff)
                times=0;

        }   //if ( bus->slot_valid[slotnum] )
    }       /* end of loop thru all slots */
    //last_startclocks = rtapi_get_time();//ghf
    //*(slot->clocks->times_max) = last_startclocks - startclocks;
}


//ghf-2014/8/4
static void read_digins(slot_data_t *slot) {
    unsigned char mask;
    static uint d_in[2] = { 0 };

    static uint iocount[65] = { 0 };
    static uint digin[65] = { 0 };
    static uint old_digin[65] = { 0 };
    static uint lvbo_time[65] = { 0 };
    int i = 0;

    //ghf-2014/5/6
    for (i = 0; i < 64; i++) {
        lvbo_time[i] = *(slot->clocks->filtertimebuf[i]);
    }

    d_in[1] += slot->rd_buf[UxC_DING];
    d_in[1] <<= 8;
    d_in[1] += slot->rd_buf[UxC_DINF];
    d_in[1] <<= 8;
    d_in[1] += slot->rd_buf[UxC_DINE];
    d_in[1] <<= 8;
    d_in[1] += slot->rd_buf[UxC_DIND];

    *(slot->digio->data_in[1]) = d_in[1];

    d_in[0] += slot->rd_buf[UxC_DINC];
    d_in[0] <<= 8;
    d_in[0] += slot->rd_buf[UxC_DINB];
    d_in[0] <<= 8;
    d_in[0] += slot->rd_buf[UxC_DINA];
    d_in[0] <<= 8;
    d_in[0] += slot->rd_buf[SRVALM_HOMESW];

    *(slot->digio->data_in[0]) = d_in[0];

    mask = 0x01;
    for (i = 0; i < 8; i++) {

        digin[i] = slot->rd_buf[SRVALM_HOMESW] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    mask = 0x01;
    for (i = 8; i < 16; i++) {

        digin[i] = slot->rd_buf[UxC_DINA] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    mask = 0x01;
    for (i = 16; i < 24; i++) {

        digin[i] = slot->rd_buf[UxC_DINB] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    mask = 0x01;
    for (i = 24; i < 32; i++) {

        digin[i] = slot->rd_buf[UxC_DINC] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    mask = 0x01;
    for (i = 32; i < 40; i++) {

        digin[i] = slot->rd_buf[UxC_DIND] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    mask = 0x01;
    for (i = 40; i < 48; i++) {

        digin[i] = slot->rd_buf[UxC_DINE] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    mask = 0x01;
    for (i = 48; i < 56; i++) {

        digin[i] = slot->rd_buf[UxC_DINF] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    mask = 0x01;
    for (i = 56; i < 64; i++) {

        digin[i] = slot->rd_buf[UxC_DING] & mask;
        mask <<= 1;

        *(slot->digin[i].data_before) = digin[i];

        if (old_digin[i] != digin[i]) {//有跳变
            old_digin[i] = digin[i];
            iocount[i] = 0;
        }
        if (iocount[i] < lvbo_time[i]) {
            iocount[i]++;
        }
        else {
            *(slot->digin[i].data) = digin[i];
            *(slot->digin[i].data_not) = !(digin[i]);
            iocount[i] = 0;
        }
    }

    *(slot->clocks->fpgastatus) = slot->rd_buf[FPGASTATUS];
}


static void write_digouts(slot_data_t *slot)
{
    int b;
    unsigned char outdata, mask;
    uint d_out=0;

    outdata = 0x00;
    mask = 0x01;

    /* assemble output byte from 8 source variables */
    for (b = 0; b < 8; b++) {
        /* get the data, add to output byte */



        /*if ((*(slot->digout[b].data)) && (!*(slot->digout[b].invert))) {
            outdata |= mask;
        }
        if ((!*(slot->digout[b].data)) && (*(slot->digout[b].invert))) {
            outdata |= mask;
        }*/

        if(*(slot->digout[b].data)) {
            outdata |= mask;
        }

        mask <<= 1;
    }
    /* write it to the hardware (cache) */
    slot->wr_buf[SRV_ON_CLR] = outdata;
    outdata = 0x00;
    mask = 0x01;
    for (b =8; b < 16; b++) {
        /* get the data, add to output byte */
        /*if ((*(slot->digout[b].data)) && (!*(slot->digout[b].invert))) {
            outdata |= mask;
        }
        if ((!*(slot->digout[b].data)) && (*(slot->digout[b].invert))) {
            outdata |= mask;
        }*/

        if(*(slot->digout[b].data)) {
            outdata |= mask;
        }
        mask <<= 1;
    }
    /* write it to the hardware (cache) */
    slot->wr_buf[UxC_DOUTA] = outdata;
    outdata = 0x00;
    mask = 0x01;
    for (b = 16; b < 24; b++) {
        /* get the data, add to output byte */
        /*if ((*(slot->digout[b].data)) && (!*(slot->digout[b].invert))) {
            outdata |= mask;
        }
        if ((!*(slot->digout[b].data)) && (*(slot->digout[b].invert))) {
            outdata |= mask;
        }*/

        if(*(slot->digout[b].data)) {
            outdata |= mask;
        }
        mask <<= 1;
    }
    /* write it to the hardware (cache) */
    slot->wr_buf[UxC_DOUTB] = outdata;

    outdata = 0x00;
    mask = 0x01;
    for (b = 24; b < 32; b++) {
        /* get the data, add to output byte */
        /*if ((*(slot->digout[b].data)) && (!*(slot->digout[b].invert))) {
            outdata |= mask;
        }
        if ((!*(slot->digout[b].data)) && (*(slot->digout[b].invert))) {
            outdata |= mask;
        }*/

        if(*(slot->digout[b].data)) {
            outdata |= mask;
        }
        mask <<= 1;
    }
    /* write it to the hardware (cache) */
    slot->wr_buf[UxC_DOUTC] = outdata;

    outdata = 0x00;
    mask = 0x01;
    for (b = 32; b < 40; b++) {
        /* get the data, add to output byte */
        /*if ((*(slot->digout[b].data)) && (!*(slot->digout[b].invert))) {
            outdata |= mask;
        }
        if ((!*(slot->digout[b].data)) && (*(slot->digout[b].invert))) {
            outdata |= mask;
        }*/

        if(*(slot->digout[b].data)) {
            outdata |= mask;
        }
        mask <<= 1;
    }
    /* write it to the hardware (cache) */
    slot->wr_buf[UxC_DOUTD] = outdata;

    d_out += slot->wr_buf[UxC_DOUTD];
    *(slot->digio->data_out[1]) = d_out;

    d_out=0;
    d_out += slot->wr_buf[UxC_DOUTC];
    d_out <<= 8;
    d_out += slot->wr_buf[UxC_DOUTB];
    d_out <<= 8;
    d_out += slot->wr_buf[UxC_DOUTA];
    d_out <<= 8;
    d_out += slot->wr_buf[SRV_ON_CLR];
    *(slot->digio->data_out[0]) = d_out;
}

static int mydelayflag=0;//ghf-2014/8/18
static void read_encoders(slot_data_t *slot)
{
    unsigned int i, byteindex;
    //static unsigned int times = 0;
    union pos_tag {
        signed int l;
        struct byte_tag {
            signed char b0;//低8位
            signed char b1;//中8位
            signed char b2;//高8位
            signed char b3;//没用，与b0、b1、b2凑成一个int型数据
        } byte;
    } pos, oldpos;

    static double old=0;
    static int ffirflag[4]={1,1,1,1};

    byteindex = ENCCNT0;        /* first encoder count register */
    for (i = 0; i < 4; i++) {
        slot->encoder[i].indrescnt++;  /* increment counter each servo cycle */
        oldpos.l = slot->encoder[i].oldreading;
        //if((*(slot->encoder[i].index)==1)){
          //rtapi_print("Y-pos.byte.b0 = %x \n",slot->rd_buf[ENCCNT0+3]);
          //rtapi_print("Y-pos.byte.b1 = %x \n",slot->rd_buf[ENCCNT0+4]);
          //rtapi_print("Y-pos.byte.b2 = %x \n",slot->rd_buf[ENCCNT0+5]);
        //}
        pos.byte.b0 = (signed char)slot->rd_buf[byteindex++];
        pos.byte.b1 = (signed char)slot->rd_buf[byteindex++];
        pos.byte.b2 = (signed char)slot->rd_buf[byteindex++];

        //add by ghf
        //pos.byte.b3 = 0;
        pos.byte.b3 = oldpos.byte.b3;
            /* check for - to + transition */
            if ((oldpos.byte.b2 & 0xc0) == 0xc0 && (pos.byte.b2 == 0))//编码器是21位的,c0是限制行程的?ffffff-c00000就是回零时的最大行程,也就是说负向从ffffff开始减1，最多能减小到c00000
           // if ((oldpos.byte.b2 & 0x80) == 0x80 && (pos.byte.b2 == 0))  //回零时负值跳到0,进这里，0又跳到负,进下面
            {
                /*if(i==0){
                rtapi_print("pos.byte.b0 = %x \n",slot->rd_buf[ENCCNT0]);
          rtapi_print("pos.byte.b1 = %x \n",slot->rd_buf[ENCCNT0+1]);
          rtapi_print("pos.byte.b2 = %x \n",slot->rd_buf[ENCCNT0+2]);
          rtapi_print("pos.l = %lx \n",pos.l);
                rtapi_print("1111oldpos.byte.b2 = %x pos.byte.b2 = %x pos.byte.b3 = %x\n",
                             oldpos.byte.b2,pos.byte.b2,pos.byte.b3);}*/
                //pos.byte.b3++;
                pos.byte.b3 = 0;

                //if(i==0)
                //  rtapi_print("pos.l = %lx \n\n",pos.l);
            }
            else if ((oldpos.byte.b2 == 0) && (pos.byte.b2 & 0xc0) == 0xc0){//刚上电时如果当前轴在负向的位置就会执行此处,此时pos.byte.b3是0,--后变为ffffffff，合并时取其最低的两个ff。
            //else if ((oldpos.byte.b2 == 0) && (pos.byte.b2 & 0x80) == 0x80){
               /* if(i==0){
                rtapi_print("pos.byte.b0 = %x \n",slot->rd_buf[ENCCNT0]);
          rtapi_print("pos.byte.b1 = %x \n",slot->rd_buf[ENCCNT0+1]);
          rtapi_print("pos.byte.b2 = %x \n",slot->rd_buf[ENCCNT0+2]);
          rtapi_print("pos.l = %lx \n",pos.l);
                rtapi_print("2222oldpos.byte.b2 = %x pos.byte.b2 = %x pos.byte.b3 = %x\n",
                             oldpos.byte.b2,pos.byte.b2, pos.byte.b3);	}*/
                //pos.byte.b3--;
                pos.byte.b3 = 0xff;

               // if(i==0)
               //   rtapi_print("pos.l = %lx \n\n",pos.l);

                  /*其中一次的打印如下所示：
                  pos.byte.b0 = 88
[  508.767882] pos.byte.b1 = 4c
[  508.767886] pos.byte.b2 = ff
[  508.767889] pos.l = ff4c88 ,fpga的ffffff表示-1,ff4c88表示的值为-1-(ffffff-ff4c88)=-45944,转换的目的就是要将ff4c88转为-45944
-45944在32位计算机中的存储就是45944(0x0000b378)按位取反，再加1,也即：ffff4c87+1,即:ffff4c88


[  508.767892] 2222oldpos.byte.b2 = 0 pos.byte.b2 = ffffffff pos.byte.b3 = 0
[  508.767896] pos.l = ffff4c88，当前坐标值为-22.972,slot->encoder[i].count=-45944,/2000就是-22.972
                  */
            }
        //add by ghf

        //rtapi_print_msg(RTAPI_MSG_ERR, "pos.byte.b1 = %x \n",pos.byte.b1);
        //rtapi_print_msg(RTAPI_MSG_ERR, "pos.byte.b0 = %x \n",pos.byte.b0);
        //rtapi_print_msg(RTAPI_MSG_ERR, "pos.l = %x \n",pos.l);



        *(slot->encoder[i].delta) = pos.l - slot->encoder[i].oldreading;

        static int delaytimes[4]={0,0,0,0};

        //rd_buf[ENCISR]的高4位相应位为1,证明有零点信号输出了，置位index;
        /* index processing */
        if ( (slot->rd_buf[ENCISR] & ( 0x10 << i )) != 0  ) {//ENCISR和手轮的state都是一个字节，高4位零点输出，低4位手轮状态

            /* index edge occurred since last time this code ran */
            *(slot->encoder[i].index) = 1; //置位，表示有零点信号输出了,这个变为1之后就一直为1了，下面不清吗？2014/3/15

            delaytimes[i] = 0;//ghf-20150623

            /* were we looking for an index edge? */
            if ( ((slot->encoder[0].indres & ( 1 << i )) != 0) ) {//这个条件成立，表明当前为回零最后一步，零点脉冲到来ghf-2014/8/18
                //times++;
                //rtapi_print_msg(RTAPI_MSG_ERR, "times = %d \n",times);
                //if(times > 2) { //这里持续的时间不确定，所以一检测到，马上清零就行了ghf-2014/4/19
                    *(slot->encoder[i].index_enable) = 0;
                    mydelayflag = 1;
                     //这里打印0就对了ghf-2014/5/15
                    //rtapi_print("pos.byte.b0 = %x \n",slot->rd_buf[ENCCNT0+i*3]);
                    //rtapi_print("pos.byte.b1 = %x \n",slot->rd_buf[ENCCNT0+1+i*3]);
                    //rtapi_print("pos.byte.b2 = %x \n\n",slot->rd_buf[ENCCNT0+2+i*3]);
                //}
            }

                //ghf-2014/4/1,copy from spi,实际上这一段可有可无，加上的话更安全一些
                /* need to properly set the 24->32 bit extension byte */
                if ( pos.byte.b2 < 0 ) {//b3要么是ff，要么是0，上面++ --也就在这两个之间切换,这里一闪而过，没什么用
                    pos.byte.b3 = 0xFF;
                } else {
                    pos.byte.b3 = 0;
                 }
                oldpos.byte.b3 = pos.byte.b3;

        } else {
            /* no index edge since last check */
            //ghf-20150623
            if(delaytimes[i]<250) {
                delaytimes[i]++;
            }
            else {
                *(slot->encoder[i].index) = 0;
            }
        }

        slot->encoder[i].oldreading = pos.l;
        *(slot->encoder[i].count) = pos.l;

        if(ffirflag[i]==1){
            slot->encoder[i].scale = fabs(1/slot->encoder[i].scale);//ghf-20150623
            if(slot->encoder[i].scaledir != -1)
                slot->encoder[i].scaledir = 1;
            slot->encoder[i].scale *= slot->encoder[i].scaledir;
            ffirflag[i]=0;
        }


        if (slot->encoder[i].scale < 0.0)
        {
            if (slot->encoder[i].scale > -EPSILON)
            slot->encoder[i].scale = -1.0;
        }
        else
        {
            if (slot->encoder[i].scale < EPSILON)
                slot->encoder[i].scale = 1.0;
        }
        *(slot->encoder[i].position) = pos.l / slot->encoder[i].scale;//scale之后的值，单位是mm

        /*if(i==0){
            if(old!=*(slot->encoder[i].position)){
                rtapi_print("pos.byte.b0 = %x \n",slot->rd_buf[ENCCNT0+i*3]);
                rtapi_print("pos.byte.b1 = %x \n",slot->rd_buf[ENCCNT0+1+i*3]);
                rtapi_print("pos.byte.b2 = %x \n",slot->rd_buf[ENCCNT0+2+i*3]);
                rtapi_print("pos.byte.b3 = %x \n",pos.byte.b3);
                rtapi_print("pos.l = %lx \n\n",pos.l);
                rtapi_print("*(slot->encoder[3].position) = %d \n",(int)(*(slot->encoder[i].position)*1000));
            }
            old=*(slot->encoder[i].position);
        }*/

        signed int y1posl;
        if(i==1){
            y1posl = pos.l;
        }

        if(i==3){
             //*(slot->encoder[i].encoderdiff) = pos.l-y1posl;
        }


        if((pos.l > 0x00ff0000)&&(oldpos.l < 0))
        {
            rtapi_print_msg(RTAPI_MSG_ERR, "in ppmc,fashengtubian\n");
            rtapi_print("pos.byte.b0 = %x \n",slot->rd_buf[ENCCNT0+i*3]);
            rtapi_print("pos.byte.b1 = %x \n",slot->rd_buf[ENCCNT0+1+i*3]);
            rtapi_print("pos.byte.b2 = %x \n",slot->rd_buf[ENCCNT0+2+i*3]);
            rtapi_print("pos.byte.b3 = %x \n",pos.byte.b3);
            rtapi_print("pos.l = %lx \n\n",pos.l);

            rtapi_print("oldpos.byte.b0 = %x \n",oldpos.byte.b0);
            rtapi_print("oldpos.byte.b1 = %x \n",oldpos.byte.b1);
            rtapi_print("oldpos.byte.b2 = %x \n",oldpos.byte.b2);
            rtapi_print("oldpos.byte.b3 = %x \n",oldpos.byte.b3);
            rtapi_print("oldpos.l = %lx \n\n",oldpos.l);
        }

    }
    //rtapi_print_msg(RTAPI_MSG_ERR,"in read_encoders,%d\n",(int)(*(slot->encoder[3].position)*1000));
}

static void write_encoders(slot_data_t *slot)
{
    int i;

    for (i = 0; i < 4; i++) {
        if ( *(slot->encoder[i].index_enable) ) {//这个为1,零脉冲来后编码器的计数值要复位
            /* all 4 control bits are packed into the same register */
            if ((slot->encoder[0].indres & (1 << i)) == 0) {//如果当前位是0，要置为1.
                slot->encoder[i].indrescnt = 0; /* clear counter first time only */
                /* set bit to force reset on index pulse */
                (slot->encoder[0].indres) |= (1 << i);
            }
        }else { //否则清零忽略
            /* clear bit to ignore index pulses */
            (slot->encoder[0].indres) &= ~(1 << i);
        }
    }
    /* put the control bits in cache for write to hardware */
    slot->wr_buf[RATE_DIR] |= (slot->encoder[0].indres << 4);//fpga读这个寄存器如果相应位是1，零点脉冲产生后复位计数器，否则忽略
    //slot->wr_buf[13] |= (slot->encoder[0].indres << 4);
}


//这个要改ghf-2014/4/1
static void read_handle(slot_data_t *slot)
{
    unsigned char state,state_Z2;
    signed long delta;
    union handle_tag {
        signed int l;
        struct byte_tag {
            signed char b0;
            signed char b1;
            signed char b2;
            signed char b3;
        } byte;
    } handle,oldhandle;

    *(slot->handles->state_pre) = *(slot->handles->state);    //alidn-20160227

    state = slot->rd_buf[HANDLE_STATE];//和ENCISR是同一个值,但用的是低4位

    *(slot->handles->state) = state;

    oldhandle.l = slot->handles->oldreading;

    handle.byte.b0 = (signed char)slot->rd_buf[HANDLE_COUNT_LOW];
    handle.byte.b1 = (signed char)slot->rd_buf[HANDLE_COUNT_HIGH];

    //ghf-2014/12/17
    if ( handle.byte.b1 < 0 ) {//b1要么是ff，要么是0
        handle.byte.b2 = 0xFF;
        handle.byte.b3 = 0xFF;
    } else {
        handle.byte.b2 = 0x0;
        handle.byte.b3 = 0x0;
    }

    delta = handle.l - slot->handles->oldreading;

//alidn-20160227
    if(((*(slot->handles->state_pre) & 0x03) == 0x03) && ((state&0x03)==0x00)){
        *(slot->handles->Z2_flag) = 1;
    }else if(((*(slot->handles->state_pre) & 0x03) == 0x01) && ((state&0x03)==0x00)){
        *(slot->handles->Z2_flag) = 0;
    }else if((state&0x03)!=0x00){
        *(slot->handles->Z2_flag) = 0;
    }

    if((state&0x0C)==0x04)
        delta = delta*10;
    else if((state&0x0C)==0x08)
        delta = delta*100;

    slot->handles->oldreading = handle.l;

    if((state&0x03)==0x01) {
        if(slot->handles->dir[0] == 0)
            *(slot->handles->count[0]) += delta;
        else
            *(slot->handles->count[0]) -= delta;
    }else if((state&0x03)==0x02) {
        if(slot->handles->dir[1] == 0)
            *(slot->handles->count[1]) += delta;
        else
            *(slot->handles->count[1]) -= delta;
    } else {
        if(slot->handles->handleztype == 0) {
            if((state&0x03)==0x03) {
                if(*(slot->stepgen->set_which_z) == 1){ //ghf-2014/5/13
                    if(slot->handles->dir[2] == 0)
                     *(slot->handles->count[3]) += delta;
                    else
                     *(slot->handles->count[3]) -= delta;
                } else if(*(slot->stepgen->set_which_z) == 0){
                    if(slot->handles->dir[2] == 0)
                     *(slot->handles->count[2]) += delta;
                    else
                     *(slot->handles->count[2]) -= delta;
                } else if (*(slot->stepgen->set_which_z) == 2) {
                    if(slot->handles->dir[2] == 0) {
                        *(slot->handles->count[2]) += delta;
                        *(slot->handles->count[3]) += delta;
                    } else {
                        *(slot->handles->count[2]) -= delta;
                        *(slot->handles->count[3]) -= delta;
                    }
                }
            }
        } else {
             if((state&0x03)==0x03) {
                    if(slot->handles->dir[2] == 0)
                        *(slot->handles->count[2]) += delta;
                    else
                        *(slot->handles->count[2]) -= delta;
            }else if((state&0x03)==0x00) {
                 if(*(slot->handles->Z2_flag)  > 0){
                         if(slot->handles->dir[3] == 0)
                             *(slot->handles->count[3]) += delta;
                        else
                             *(slot->handles->count[3]) -= delta;
                 }
             }
        }
    }

}

static void write_stepgens(slot_data_t *slot)
{
    int n, reverse, run;
    unsigned int pulse;

    stepgen_t *sg;
    double ch_max_freq, abs_scale,freq;// bd_max_freq;
    unsigned char control_byte;
    static double pos[4]={0};
    static long pulse_add[4]={0};

    static unsigned char pre_index_enable[4]={0};
    static unsigned char index_delay_flag[4] = {0};
    static int index_delay_count[4] = {0};
    static int errorprint[4]={0};
    static int firflag[4]={1,1,1,1};

    static double oldvel[4]={0};

    control_byte = 0; //xyz1z2
    for ( n = 0 ; n < 4 ; n++ ) {
        /* point to the specific stepgen */
        sg = &(slot->stepgen->sg[n]);

        if(firflag[n]==1){
            sg->scale = fabs(1/sg->scale);//ghf-20150623
            if(sg->scaledir != -1)
               sg->scaledir = 1;
            sg->scale *= sg->scaledir;
            firflag[n]=0;
        }

        /* validate the scale value */
        if ( sg->scale < 0.0 ) { //sg->scale是hal层的可读写参数,这里的scale是axis_mm.ini中的OUTPUT_SCALE
            if ( sg->scale > -EPSILON ) { //-1.0< sg->scale < -EPSILON
            /* too small, divide by zero is bad */
            sg->scale = -1.0;
            }
            abs_scale = -sg->scale;
        } else {
            if ( sg->scale < EPSILON ) {
                sg->scale = 1.0; //EPSILON< sg->scale < 1.0
            }
            abs_scale = sg->scale;
        }
        ch_max_freq = 25000000; //25MHz

        *(sg->realvel) = *(sg->vel) - oldvel[n];
        oldvel[n] = *(sg->vel);

        /* should we be running? */
        if ( *(sg->enable) != 0 ) { //hal的一个位输入管教，就是epp_io.hal中的motion.motion-enabled，也就是各轴的 *_SRV_ON 的 IO
            //ghf-20150713
            //run = 1;
            if( ((*(slot->digio->data_in[0])) & 0xff0000) == 0 ){
                run=1;
            }
            else {
                if(*(sg->limitvel)>0){
                    if(*(sg->realvel)>0){
                        run = 0;
                        pulse_add[n] = *(sg->vel)* sg->scale;//ghf-2015-4-30
                    }
                    else {
                        run=1;
                        //rtapi_print("*(sg->vel)=%d\n",(int)(*(sg->vel)*1000));
                    }
                }
                else if(*(sg->limitvel)<0){
                    if(*(sg->realvel)<0){
                        run = 0;
                        pulse_add[n] = *(sg->vel)* sg->scale;//ghf-2015-4-30
                    }
                    else {
                        run=1;
                    }
                }
                else {
                    run = 1;
                }
            }
        } else {//这句只有在停止的时候才会执行，也就是上电后默认手动模式时，当前编码器的值就记下来了
            run = 0;
            pulse_add[n] = *(sg->vel)* sg->scale;//ghf-2015-4-30
            //pulse_add[n] = (*(slot->encoder[n].position)) * sg->scale;//？？和encoder[i].scale的区别,pulse_add[n]应该为当前编码器的计数值，这里应该*encoder[n].scale才对啊！！？ghf-2014/3/21,保证拔掉再插上，还可以走ghf-2014/5/29
            //pos[n] = (*(slot->encoder[n].position)) * sg->scale;//ghf-2014/5/7
            //pulse_add[n] = (*(slot->encoder[n].position)) * slot->encoder[n].scale; //ghf-2014/5/22,ab项时此值不等于outputscale的值
            //rtapi_print_msg(RTAPI_MSG_ERR, "run = 0,pulse_add[%d]=%d\n",n,(int)(pulse_add[n]*1000));
        }


        //pulse_add[n] = (*(slot->encoder[n].position)) * sg->scale;//ghf-2014/4/2，这种搞法不行


        if(((*(sg->vel)-pos[n])!=0.0) && (run==1)) {//sg->vel就是pos_cmd，记录着当前编码器应该走到的位置，pos[n]记着上一次编码器的位置，*(sg->vel)-pos[n]就是每次要走的mm值，freq要换算成实际的脉冲个数
            freq =(long)(*(sg->vel)* sg->scale) - pulse_add[n];//编码器应该走过的脉冲个数

            //freq = *(sg->vel) * sg->scale - pos[n] * sg->scale; //ghf-2014/5/7

        } else
            freq =0.0;


        pos[n] =*(sg->vel);

        /* calculate desired frequency */

        //added on 08.5.10. to do homing follow error.没这个确实容易报跟随误差,index到来后，pos_fb已经为0,而pos_cmd不为0,此时如果继续发脉冲下去，很容易报跟随误差，所以要延时，直到home.c中将poscmd也清为0. 此时home处在HOME_INDEX_SEARCH_WAIT状态，经过0.1s后切到HOME_SET_INDEX_POSITION，这个中执行清零操作。
        if ((pre_index_enable[n] == 1) && (*(slot->encoder[n].index_enable)) == 0) {
            index_delay_flag[n] = 1;
            if(mydelayflag == 1)//ghf-2014/8/18,只有再回零脉冲到来后，fpga执行了编码器清零操作，才需要延时,使得清零前的脉冲全部走完
                index_delay_count[n] = 220; //500 means 500ms,这个值一定要大于home中回零最后一步的100ms的延时，此100ms的延时又要大于fpga的50ms的延时清零. 此延时值在100-200ms就行,xinban中回零无需延时，下面index信号一到，编码器的值立马就清零了。ghf-2014/5/15	,新版也要等待，等脉冲真正走完，再执行HOME_SET_INDEX_POSITION的内容。
        }

        pre_index_enable[n] = *(slot->encoder[n].index_enable);

        if (index_delay_count[n] <= 0 )
            index_delay_flag[n] = 0;

        if (index_delay_flag[n] == 1) { //延时处理回零时的跟随误差
            index_delay_count[n] --;
            pulse_add[n] = (*(slot->encoder[n].position)) * sg->scale;
            //pos[n] = (*(slot->encoder[n].position)) * sg->scale; //ghf-2014/5/7
            freq = 0;//这个才是停止运动的关键
        }

        if (abs(freq/sg->scale) > 0.3 ) {//跟随误差应该是pos_cmd-pos_feedback才对阿？
            if (errorprint[n] == 0) {
                //rtapi_print(
                //  "in ppmc,freq =(long)(*(sg->vel)* sg->scale) - pulse_add[n]\n");

                //rtapi_print(
                //  "in ppmc, joint %d following error,freq=%f,sg->vel=%f,sg->scale=%f,pulse_add[n]=%d\n",
                //     n,freq,*(sg->vel),sg->scale,pulse_add[n]);

                errorprint[n] = 1;
            }
            freq = 0;
        } else {
            errorprint[n] = 0;
        }

        /* deal with special cases - negative and very low frequency */
        reverse = 0;
        if ( freq < 0 ) {
            /* negative */
            freq = -freq;
            reverse = 1;
        }
        /* apply limits */
        if ( freq > ch_max_freq / 1000 ) {
            freq = ch_max_freq / 1000;
        } else if ( freq < 1 ) {

            freq = 0;//ghf-2014/5/7
            /* only way to get zero is to turn it off */
            run = 0;//ghf-2014/5/7
        }



        pulse = (unsigned int)(freq);

        //ghf-2014/4/19，为缓解跟随误差
        /*pointpulse += ((int)(freq*1000))%1000;
        if(pointpulse >= 1000)
        {
            pulse += 1;
            pointpulse = pointpulse - 1000;
        }*/


        if (pulse != 0) {
            //rtapi_print( "111pluse=%d\n",(int)(pulse*1000));
        }

        /* set dir bit in the control byte, and save the frequency */
        if ( reverse ) {
            pulse_add[n] -= pulse;

            sg->freq = -freq;
            //sg->freq = -pulse; //ghf-2014/5/7
            control_byte |=( 0x01<<n);
        } else {
            pulse_add[n] += pulse;//pulse_add[n]应该记录的是编码器的理论值，我认为这个应该为编码器读回的值ghf-2014/4/2，这种想法不行
            sg->freq = freq;
            //sg->freq = pulse; //ghf-2014/5/7
        }

        if(n==3){
            sg->totleplusediff = pulse_add[3] - pulse_add[1];
        }


        slot->wr_buf[RATE_NUM_0+(n*2)] = pulse & 0xff;
        slot->wr_buf[RATE_NUM_0+(n*2)+1] = (pulse>>8) & 0xff;
        slot->wr_buf[RATE_DIR]=control_byte;
    }//for ( n = 0 ; n < 4 ; n++ )
}

static void write_extraDAC(slot_data_t *slot)
{
    DAC_t *pg;
    long dc;
    float volts;

    /* point to the DAC */
    pg = &(slot->extra->dac);
    /* validate the scale value */
    if ( pg->scale < 0.0 ) {
        if ( pg->scale > -EPSILON ) {
            /* too small, divide by zero is bad */
            pg->scale = -1.0;
        }
    } else {
        if ( pg->scale < EPSILON ) {
            pg->scale = 1.0;
        }
    }

    volts = abs(*(pg->value)) / pg->scale;  //

    dc = (long) ((volts / 5.0) * 0xFFF);
    if (dc > 0xFFF) {
        dc = 0xFFF;
    }
    if (dc < 0 ) {
        dc = 0;
    }
    slot->wr_buf[UxC_EXTRA] = dc & 0xff;  // put in cache一共16位两个字节

    slot->wr_buf[UxC_EXTRA+1] = (dc>>8) & 0xff;
}

/***********************************************************************
*                   LOCAL FUNCTION DEFINITIONS                         *
************************************************************************/

/* this function converts a range of EPP addresses into a bitmap
to be passed to add_rd_funct() or add_wr_funct()
*/

static __u32 block(int min, int max)
{
    int n;
    __u32 mask;

    mask = 0;
    for ( n = min ; n <= max ; n++ ) {
        mask |= ( 1 << n );
    }
    return mask;
}

/* these functions are used to register a runtime function to be called
by either read_all or write_all.  'cache_bitmap' defines the EPP
addresses that the function needs.  All addresses needed by all
functions associated with the slot will be sequentially read into
the rd_buf cache (or written from the wr_buf cache) by read_all
or write_all respectively, to minimize the number of slow inb
and outb operations needed.
*/

static int add_rd_funct(slot_funct_t *funct, slot_data_t *slot,
__u32 cache_bitmap )
{
    if ( slot->num_rd_functs >= MAX_FUNCT ) {
        rtapi_print_msg(RTAPI_MSG_ERR,
        "PPMC: ERROR: too many read functions\n");
        return -1;
    }
    slot->rd_functs[slot->num_rd_functs++] = funct;
    slot->read_bitmap |= cache_bitmap;
    return 0;
}

static int add_wr_funct(slot_funct_t *funct, slot_data_t *slot,
__u32 cache_bitmap )
{
    if ( slot->num_wr_functs >= MAX_FUNCT ) {
        rtapi_print_msg(RTAPI_MSG_ERR,
        "PPMC: ERROR: too many write functions\n");
        return -1;
    }
    slot->wr_functs[slot->num_wr_functs++] = funct;
    slot->write_bitmap |= cache_bitmap;
    return 0;
}

static int export_UxC_digin(slot_data_t *slot, bus_data_t *bus)//32 bit digital inputs
{
    int retval, n;
    char buf[HAL_NAME_LEN + 2];

    //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC:  exporting UxC digital inputs\n");

    /* do hardware init */
    /* allocate shared memory for the digital input data */

    slot->digin = hal_malloc(64 * sizeof(din_t));
    if (slot->digin == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
        "PPMC: ERROR: hal_malloc() failed\n");
        return -1;
    }
    for ( n = 0 ; n < 64 ; n++ ) {
        /* export pins for input data */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.din.%02d.in",
                 bus->busnum, bus->last_digin);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(slot->digin[n].data), comp_id);
        if (retval != 0) {
            return retval;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.din.%02d.in-not",
                 bus->busnum, bus->last_digin);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(slot->digin[n].data_not), comp_id);
        if (retval != 0) {
            return retval;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.din.%02d.in-before",
                 bus->busnum, bus->last_digin);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(slot->digin[n].data_before), comp_id);
        if (retval != 0) {
            return retval;
        }


        /* increment number to prepare for next output */
        bus->last_digin++;
    }
    add_rd_funct(read_digins, slot, block(SRVALM_HOMESW, UxC_DING+1));//ghf-2014/8/25,+1表示读取fpgastatus
    return 0;
}

static int export_UxC_digout(slot_data_t *slot, bus_data_t *bus)//24 bit digital outputs
{
    int retval, n;
    char buf[HAL_NAME_LEN + 2];

    //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC:  exporting UxC digital outputs\n");

    /* allocate shared memory for the digital output data */
    slot->digout = hal_malloc(40 * sizeof(dout_t));
    slot->digio = hal_malloc(sizeof(dio_t));
    if (slot->digout == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: hal_malloc() failed\n");
        return -1;
    }
    if (slot->digio == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: hal_malloc() failed\n");
        return -1;
    }
    /* export pin for output data */
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.dio.data-out.00", bus->busnum);
    retval = hal_pin_u32_new(buf, HAL_OUT, &(slot->digio->data_out[0]), comp_id);
    if (retval != 0) {
        return retval;
    }
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.dio.data-out.01", bus->busnum);
    retval = hal_pin_u32_new(buf, HAL_OUT, &(slot->digio->data_out[1]), comp_id);
    if (retval != 0) {
        return retval;
    }
    /* export pin for input data */
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.dio.data-in.00", bus->busnum);
    retval = hal_pin_u32_new(buf, HAL_OUT, &(slot->digio->data_in[0]), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.dio.data-in.01", bus->busnum);
    retval = hal_pin_u32_new(buf, HAL_OUT, &(slot->digio->data_in[1]), comp_id);
    if (retval != 0) {
        return retval;
    }

    //*(slot->digio->data_in[0]) = 0x35;
    //*(slot->digio->data_in[1]) = 0x55;

    for ( n = 0 ; n < 40 ; n++ ) {
        /* export pin for output data */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.dout.%02d.out",
        bus->busnum, bus->last_digout);
        retval = hal_pin_bit_new(buf, HAL_IN, &(slot->digout[n].data), comp_id);
        if (retval != 0) {
            return retval;
        }
        /* export parameter for inversion */
        //rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.dout.%02d.invert",
                // bus->busnum, bus->last_digout);
        //retval = hal_pin_bit_new(buf, HAL_IN, &(slot->digout[n].invert), comp_id);
        //if (retval != 0) {
        //	return retval;
        //}

        /* increment number to prepare for next output */
        bus->last_digout++;
    }
    add_wr_funct(write_digouts, slot, block(SRV_ON_CLR, UxC_DOUTD));
    //把数据放到wr_buf中，在writeall中才真正通过并口发送
    return 0;
}



static int export_USC_stepgen(slot_data_t *slot, bus_data_t *bus)
{
    int retval, n;
    char buf[HAL_NAME_LEN + 2];
    stepgen_t *sg;

    //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC:  exporting step generators\n");

    /* do hardware init */
    /* allocate shared memory for the digital output data */
    slot->stepgen = hal_malloc(sizeof(stepgens_t));
    if (slot->stepgen == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: hal_malloc() failed in stepgen\n");
        return -1;
    }

    /* export per-stepgen pins and params */
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.tool-number", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_IN, &(slot->stepgen->tool_number), comp_id);
    if (retval != 0) {
        return retval;
    }
    //ghf-2014/2/26
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.set_which_z", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_IO, &(slot->stepgen->set_which_z), comp_id);
    if (retval != 0) {
        return retval;
    }
    slot->stepgen->maxerror = 30;   // not used now, alidn(2014-02-07)
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.maxerror", bus->busnum);
    retval = hal_param_float_new(buf, HAL_RW, &(slot->stepgen->maxerror), comp_id);
    if (retval != 0) {
        return retval;
    }
    slot->stepgen->minerror = 1;    // not used now, alidn(2014-02-07)
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.minerror", bus->busnum);
    retval = hal_param_float_new(buf, HAL_RW, &(slot->stepgen->minerror), comp_id);
    if (retval != 0) {
        return retval;
    }
    slot->stepgen->maxvel = 0;      // not used now, alidn(2014-02-07)
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.maxvel",	bus->busnum);
    retval = hal_param_float_new(buf, HAL_RW, &(slot->stepgen->maxvel), comp_id);
    if (retval != 0) {
        return retval;
    }
    slot->stepgen->ab_pd_sel = 0;      // alidn(2014-05-28)
    rtapi_snprintf(buf, HAL_NAME_LEN, "stepgen.mode");
    retval = hal_param_u32_new(buf, HAL_RW, &(slot->stepgen->ab_pd_sel), comp_id);
    if (retval != 0) {
        return retval;
    }

    for ( n = 0 ; n < 4 ; n++ ) {
        /* pointer to the stepgen struct */
        sg = &(slot->stepgen->sg[n]);
        /* enable pin */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.enable",
                 bus->busnum, bus->last_stepgen);
        retval = hal_pin_bit_new(buf, HAL_IN, &(sg->enable), comp_id);
        if (retval != 0) {
            return retval;
        }
        /* velocity command pin */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.velocity",
                 bus->busnum, bus->last_stepgen);
        retval = hal_pin_float_new(buf, HAL_IN, &(sg->vel), comp_id);
        if (retval != 0) {
            return retval;
        }

        //ghf-20150710
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.limitvel",
                 bus->busnum, bus->last_stepgen);
        retval = hal_pin_float_new(buf, HAL_IN, &(sg->limitvel), comp_id);
        if (retval != 0) {
            return retval;
        }
        *(sg->limitvel) = 0;


        //ghf-20150713
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.realvel",
                 bus->busnum, bus->last_stepgen);
        retval = hal_pin_float_new(buf, HAL_IN, &(sg->realvel), comp_id);
        if (retval != 0) {
            return retval;
        }
        *(sg->realvel) = 0;

        /* velocity scaling parameter */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.scale",
                 bus->busnum, bus->last_stepgen);
        retval = hal_param_float_new(buf, HAL_RW, &(sg->scale), comp_id);
        if (retval != 0) {
            return retval;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.scaledir",
                 bus->busnum, bus->last_stepgen);
        retval = hal_param_float_new(buf, HAL_RW, &(sg->scaledir), comp_id);
        if (retval != 0) {
            return retval;
        }
        sg->scale = 1.0;
        sg->scaledir = 1;
        /* maximum velocity parameter */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.max-vel",
                 bus->busnum, bus->last_stepgen);
        retval = hal_param_float_new(buf, HAL_RW, &(sg->max_vel), comp_id);
        if (retval != 0) {
            return retval;
        }
        sg->max_vel = 0.0;
        /* actual frequency parameter */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.stepgen.%02d.freq",
                 bus->busnum, bus->last_stepgen);
        retval = hal_param_float_new(buf, HAL_RO, &(sg->freq), comp_id);
        if (retval != 0) {
            return retval;
        }

        //ghf-20150701
        if(n == 3){
            rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.totleplusediff",
                     bus->busnum);
            retval = hal_param_float_new(buf, HAL_RO, &(sg->totleplusediff), comp_id);
            if (retval != 0) {
                return retval;
            }
        }

        /* increment number to prepare for next output */
        bus->last_stepgen++;
    }
    add_wr_funct(write_stepgens, slot, block(RATE_NUM_0, RATE_DIR));
    return 0;
}

/* Each of the encoders has the following:
params:
ppmc.n.encoder.m.scale      float
pins:

ppmc.n.encoder.m.position   float
ppmc.n.encoder.m.counts     s32
ppmc.n.encoder.m.index      bit

the output value is position= counts / scale
Additionally, the encoder registers are zeroed, and the mode is set to latch */

static int export_encoders(slot_data_t *slot, bus_data_t *bus)
{
    int retval, n;
    char buf[HAL_NAME_LEN+2];

    //rtapi_print_msg(RTAPI_MSG_INFO, "PPMC: exporting encoder pins / params\n");

    /* allocate shared memory for the encoder data */
    slot->encoder = hal_malloc(4 * sizeof(encoder_t));
    if (slot->encoder == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: hal_malloc() failed\n");
        return -1;
    }
    slot->encoder[0].indres = 0;  /* clear reset-on-index register copy */
    /* export per-encoder pins and params */
    for ( n = 0 ; n < 4 ; n++ ) {
        /* scale input parameter */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoder.%02d.scale",
                 bus->busnum, bus->last_encoder);
        retval = hal_param_float_new(buf, HAL_RW, &(slot->encoder[n].scale), comp_id);
        if (retval != 0) {
            return retval;
        }

        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoder.%02d.scaledir",
                 bus->busnum, bus->last_encoder);
        retval = hal_param_float_new(buf, HAL_RW, &(slot->encoder[n].scaledir), comp_id);
        if (retval != 0) {
            return retval;
        }
        slot->encoder[n].scaledir=1;
        /* scaled encoder position */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoder.%02d.position",
                 bus->busnum, bus->last_encoder);
        retval = hal_pin_float_new(buf, HAL_OUT, &(slot->encoder[n].position), comp_id);
        if (retval != 0) {
            return retval;
        }
        /* raw encoder position */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoder.%02d.count",
                 bus->busnum, bus->last_encoder);
        retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->encoder[n].count), comp_id);
        if (retval != 0) {
            return retval;
        }
        /* raw encoder delta */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoder.%02d.delta",
                  bus->busnum, bus->last_encoder);
        retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->encoder[n].delta), comp_id);
        if (retval != 0) {
            return retval;
        }
        /* encoder index bit */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoder.%02d.index",
                 bus->busnum, bus->last_encoder);
        retval = hal_pin_bit_new(buf, HAL_OUT, &(slot->encoder[n].index), comp_id);
        if (retval != 0) {
            return retval;
        }
        /* encoder index enable bit */
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoder.%02d.index-enable",
                 bus->busnum, bus->last_encoder);
        retval = hal_pin_bit_new(buf, HAL_IO, &(slot->encoder[n].index_enable), comp_id);
        if (retval != 0) {
            return retval;
        }

        //ghf-20150701
        /*if(n == 3){
            rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.encoderdiff",
                     bus->busnum);
            retval = hal_param_float_new(buf, HAL_RO, &(slot->encoder[n].encoderdiff), comp_id);
            if (retval != 0) {
                return retval;
            }
        }*/

        /* increment number to prepare for next output */
        bus->last_encoder++;
    }

    add_rd_funct(read_encoders, slot, block(ENCCNT0, ENCCNT3+2));
    add_wr_funct(write_encoders, slot, block(RATE_DIR, RATE_DIR));
    return 0;
}

static int export_extra_dac(slot_data_t *slot, bus_data_t *bus)
{
    int retval;
    char buf[HAL_NAME_LEN+2];
    DAC_t *pg;

    /* allocate shared memory for the DAC */
    slot->extra = hal_malloc(sizeof(extra_t));
    if (slot->extra == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: hal_malloc() failed\n");
        return -1;
    }
    slot->extra_mode = EXTRA_DAC;   //EXTRA_DAC=1

    /* export DAC pins and params, and set initial values */
    /* pointer to the DAC struct */
    pg = &(slot->extra->dac);   // 16 bit DAC
    /* value command pin */
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.DAC8.%02d.value",
            bus->busnum, bus->last_extraDAC);
    retval = hal_pin_float_new(buf, HAL_IN, &(pg->value), comp_id);
    if (retval != 0) {
        return retval;
    }
    /* output scaling parameter */
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.DAC8.%02d.scale",
                bus->busnum, bus->last_extraDAC);
    retval = hal_param_float_new(buf, HAL_RW, &(pg->scale), comp_id);
    if (retval != 0) {
        return retval;
    }
    pg->scale = 1.0;    // Why? alidn
    /* increment number to prepare for next output */
    bus->last_extraDAC++;
    add_wr_funct(write_extraDAC, slot, block(UxC_EXTRA, UxC_EXTRA+1));
    return 0;
}

static int export_handle(slot_data_t *slot,bus_data_t * bus)
{
    int retval, n;
    char buf[HAL_NAME_LEN+2];

    slot->handles = hal_malloc(sizeof(handle_t));
    if (slot->handles == 0) {
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.handle.state",
                 bus->busnum);
    retval = hal_pin_u32_new(buf, HAL_OUT, &(slot->handles->state), comp_id);
    if (retval != 0) {
        return retval;
    }
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.handle.state_pre",
                 bus->busnum);
    retval = hal_pin_u32_new(buf, HAL_OUT, &(slot->handles->state_pre), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.handle.Z2_flag",
                 bus->busnum);
    retval = hal_pin_bit_new(buf, HAL_OUT, &(slot->handles->Z2_flag), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.handle.ztype",
                 bus->busnum);
    retval = hal_param_bit_new(buf, HAL_RW, &(slot->handles->handleztype), comp_id);
    if (retval != 0) {
        return retval;
    }


    for ( n = 0 ; n < 4 ; n++ ) {
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.handle.%02d.dir",
                 bus->busnum,bus->last_handle);
        retval = hal_param_bit_new(buf, HAL_RW, &(slot->handles->dir[n]), comp_id);
        if (retval != 0) {
            return retval;
        }
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.handle.%02d.count",
                 bus->busnum, bus->last_handle);
        retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->handles->count[n]), comp_id);
        if (retval != 0) {
            return retval;
        }

        bus->last_handle++;//00是第一个 01是第二个
    }
    add_rd_funct(read_handle, slot, block(HANDLE_STATE, HANDLE_COUNT_HIGH));
    //0x0C-0x0E 设置read_bitmap位图，同时将read_handle添加到读函数数组中
    return 0;
}

//ghf-2014/2/28
static int export_clock(slot_data_t *slot,bus_data_t * bus)
{
    int retval;
    char buf[HAL_NAME_LEN+2];
    int n = 0;

    slot->clocks = hal_malloc(sizeof(clocks_t));
    if (slot->clocks == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PPMC: ERROR: hal_malloc() failed\n");
        return -1;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.clock-count", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->clocks->clock_count), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.clock-max", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->clocks->clock_max), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.clock-min", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->clocks->clock_min), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.times", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->clocks->times), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.times-max", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->clocks->times_max), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.times-min", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->clocks->times_min), comp_id);
    if (retval != 0) {
        return retval;
    }

    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.times-avg", bus->busnum);
    retval = hal_pin_s32_new(buf, HAL_OUT, &(slot->clocks->times_avg), comp_id);
    if (retval != 0) {
        return retval;
    }

    //ghf-2014/5/6
    for ( n = 0 ; n < 64 ; n++ ) {
        rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.times-filtertimebuf.%02d",
                 bus->busnum,n);
        retval = hal_pin_u32_new(buf, HAL_IN, &(slot->clocks->filtertimebuf[n]), comp_id);
        if (retval != 0) {
            return retval;
        }
    }

    //ghf-2014/8/25
    rtapi_snprintf(buf, HAL_NAME_LEN, "ppmc.%d.fpgastatus", bus->busnum);
    retval = hal_pin_u32_new(buf, HAL_OUT, &(slot->clocks->fpgastatus), comp_id);
    if (retval != 0) {
        return retval;
    }

    return 0;
}

/* tests for an EPP bus timeout, and clears it if so */
static int ClrTimeout(unsigned int port_addr)
{
    unsigned char r;

    r = inb(STATUSPORT(port_addr));

    if  (!(r & 0x01)) { //如果r的最低位为0，直接返回
    return 0;
    }

    r = inb(STATUSPORT(port_addr));//如果r的最低位为1，写1清零
    outb(r | 0x01, STATUSPORT(port_addr)); /* Some reset by writing 1 */
    r = inb(STATUSPORT(port_addr));

    return !(r & 0x01);
}

/* sets the EPP address and then reads one byte from that address */
static unsigned short SelRead(unsigned char epp_addr, unsigned int port_addr)
{
    unsigned char b;

    ClrTimeout(port_addr);
    /* set port direction to output */
    outb(0x04,CONTROLPORT(port_addr));//设置方向为输出
    /* write epp address to port */
    outb(epp_addr,ADDRPORT(port_addr)); //写地址
    /* set port direction to input */
    outb(0x24,CONTROLPORT(port_addr)); //设置方向为输入
    /* read data value */
    b = inb(DATAPORT(port_addr)); //读数据
    return b;

}

/* reads one byte from EPP, use only after SelRead, and only
when hardware has auto-increment address cntr */
static unsigned short ReadMore(unsigned int port_addr)
{
    unsigned short b;
    b = inb(DATAPORT(port_addr));
    return b;

}

/* sets the EPP address and then writes one byte to that address */
static void SelWrt(unsigned char byte, unsigned char epp_addr, unsigned int port_addr)
{
    ClrTimeout(port_addr);
    /* set port direction to output */
    outb(0x04,CONTROLPORT(port_addr));//设置方向为输出
    /* write epp address to port */
    outb(epp_addr,ADDRPORT(port_addr)); //写地址
    /* write data to port */
    outb(byte,DATAPORT(port_addr)); //写数据
    return;
}

/* writes one byte to EPP, use only after SelWrt, and only
when hardware has auto-increment address cntr */
static void WrtMore(unsigned char byte, unsigned int port_addr)
{
    outb(byte,DATAPORT(port_addr));
    return;
}
