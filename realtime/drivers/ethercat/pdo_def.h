/*
 * SOEM EtherCAT exmaple
 * Ported to raspberry pi by Ho Tam - thanhtam.h[at]gmail.com
 */


#ifndef _PDO_DEF_
#define _PDO_DEF_

#include "servo_def.h"
#include "osal.h"

//MAXPOS PDO mapping
#define LINKUP_CNT_TIME 500

#define EH_CMD_POOL_SIZE 5
#define EH_MAX_AXIS_NUM 64
#define EH_MAX_IO_DATA 32

typedef union _mapping_obj{
    uint32_t obj;
    struct {
        uint16_t index;
        uint8_t subindex;
        uint8_t size;
    };
} mapping_obj;

//0x1600 RxPDO
typedef struct PACKED_PDO
{
    volatile    uint16_t	ControlWord;		//0x6040
    volatile    int32_t	TargetPosition;		//0x607A
    volatile    uint8_t	ModeOfOperation;	//0x6060
    volatile    uint16_t	TouchProbeFunction;	//0x60B8


    volatile    uint32_t	PhysicalOutput;		//0x60FE
    volatile    int32_t	PositionOffset;		//0x60B0
    volatile    int32_t	VelocityOffset;		//0x60B1
    volatile    uint16_t	TorqueOffset;		//0x60B2

} __attribute__ ((packed)) MAXPOS_DRIVE_RxPDO_t;

//0x1A00 TxPDO
typedef struct PACKED_PDO2
{

    volatile    uint16_t StatusWord;				//0x6041
    volatile    int32_t	 PositionActualValue;		//0x6064
    volatile    uint16_t TouchProbeStatus;			//0x60B9
    volatile    int32_t	 TouchProbePosition1;		//0x60BA
    volatile    int16_t  ServoErrorCode;			//0x603f


    volatile    int32_t  VelocityActualValue;		//0x606C
    volatile    uint8_t	 ModeOfOperationDisplay;		//0x6061
    volatile    uint32_t DigitalInput;				//0x60FD
    volatile    int32_t	 TouchProbePosition2;		//0x60BB


} __attribute__ ((packed)) MAXPOS_DRIVE_TxPDO_t;


typedef struct _MAXPOS_ServoDrive
{
    volatile MAXPOS_DRIVE_RxPDO_t 	OutParam;
    volatile MAXPOS_DRIVE_TxPDO_t 	InParam;
} MAXPOS_ServoDrive_t;

typedef struct _MAXPOS_Drive_pt
{
    volatile MAXPOS_DRIVE_RxPDO_t 	*ptOutParam;
    volatile MAXPOS_DRIVE_TxPDO_t 	*ptInParam;
    volatile uint8 valid;
} MAXPOS_Drive_pt;




//0x1600 RxPDO
typedef struct PACKED_PDOIO
{
    //uint16_t    uused;
    volatile   uint32_t	output[EH_MAX_IO_DATA];		//
    //volatile   uint32_t	output2;		//
    //volatile   uint32_t	output3;		//

} __attribute__ ((packed)) MAXPOS_IO_RxPDO_t;

//0x1A00 TxPDO
typedef struct PACKED_PDOIO2
{
    //uint16_t    uused;
    volatile  uint32_t	intput[EH_MAX_IO_DATA];		//
    //volatile  uint32_t	intput2;		//

} __attribute__ ((packed)) MAXPOS_IO_TxPDO_t;


typedef struct _MAXPOS_IO_pt
{

    volatile MAXPOS_IO_RxPDO_t 	*ptOutParam;
    volatile MAXPOS_IO_TxPDO_t 	*ptInParam;
    volatile uint8 valid;
    volatile int32 Osize;
    volatile int32 Isize;

} MAXPOS_IO_pt;




typedef struct _MAXPOS_FrameData
{

    volatile    uint64_t dirty;
    volatile    MAXPOS_DRIVE_RxPDO_t servoOut[EH_MAX_AXIS_NUM];
    volatile    MAXPOS_DRIVE_TxPDO_t servoIn[EH_MAX_AXIS_NUM];
    volatile    MAXPOS_IO_RxPDO_t ioOut[EH_MAX_AXIS_NUM];
    volatile    MAXPOS_IO_TxPDO_t ioIn[EH_MAX_AXIS_NUM];
    volatile    uint8 crcdata[2];

} __attribute__ ((packed)) MAXPOS_FrameData;


//for communication from hal
typedef struct PACKED_HAL1
{

    volatile  uint64_t heartbeat; //if detect no hearbeat //hal  dead
    volatile  uint8_t cmd_set_link;//hal set this to 1 //make communication
    volatile  uint8_t com_error; //if detect no data in the pool //realtime system is bad

    volatile  uint8_t expect_num_of_io;
    volatile  uint8_t expect_num_of_servo;
    volatile  uint8_t num_of_io;
    volatile  uint8_t num_of_servo;
    volatile  uint8_t link_status; //0 offline 1 allready op

    volatile    MAXPOS_FrameData frames[EH_CMD_POOL_SIZE];

    volatile  uint8_t cmd_sdo_option; //1 write 2 read
    volatile  uint8_t cmd_sdo_return;
    volatile  int32   cmd_sdo_slave;
    volatile  uint16  cmd_sdo_index;
    volatile  uint8   cmd_sdo_subindex;
    volatile  uint32  cmd_sdo_data;
    volatile  int32   cmd_sdo_dateLen;

    volatile  uint8 cmd_info_num;
    volatile  char cmd_info_name[EH_MAX_AXIS_NUM*2][128];

} __attribute__ ((packed)) COM_STATUS_t;


#endif //_PDO_DEF_

