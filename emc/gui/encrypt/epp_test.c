#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/io.h>
#include <sys/time.h>

#include <linux/rtc.h>
#include <sys/ioctl.h>  

#define BASEADDR            0X378
#define STATUSPORT(addr)    addr+1      //SPP状态只能读
#define CONTROLPORT(addr)   addr+2      //SPP控制寄存器 只能写
#define ADDRPORT(addr)      addr+3      //EPP地址寄存器 可读写
#define DATAPORT(addr)      addr+4      //EPP数据寄存器 可读写
static int myClrTimeout(unsigned int port_addr)
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

static unsigned char mySelRead(unsigned char epp_addr, unsigned int port_addr)
{
    myClrTimeout(port_addr);
    /* set port direction to output */
    outb(0x04,CONTROLPORT(port_addr));//设置方向为输出
    /* write epp address to port */
    outb(epp_addr,ADDRPORT(port_addr)); //写地址
    /* set port direction to input */
    outb(0x24,CONTROLPORT(port_addr)); //设置方向为输入

    return 0;
}

static unsigned char myReadMore(unsigned int port_addr)
{
    unsigned char b;
    b = inb(DATAPORT(port_addr));
    return b;
}

static void mySelWrt(unsigned char epp_addr, unsigned int port_addr)
{
    myClrTimeout(port_addr);
    /* set port direction to output */
    outb(0x04,CONTROLPORT(port_addr));//设置方向为输出
    /* write epp address to port */
    outb(epp_addr,ADDRPORT(port_addr)); //写地址
    return;
}

static void myWrtMore(unsigned char byte, unsigned int port_addr)
{
    outb(byte,DATAPORT(port_addr));
    return;
}

int main(void) {

    int fd, retval;  
    struct rtc_time rtc_tm; 
    int ret;
        	
    unsigned char fpgaok = 0;
    unsigned char fpgaversion=0;
    
    	ret = iopl(3);
	if(ret)
	{
		printf("iopl set error=%i\n",ret);
		return -1;
	}

    //复位fpga
    mySelWrt(0x02,BASEADDR);//写地址并设置为输出
    myWrtMore(0x0c,BASEADDR);//写数据

    usleep(100000);//延时2000*50us
 
    mySelRead(0x03,BASEADDR);//设置要读的地址
    fpgaok =  myReadMore(BASEADDR);
    printf("fpgaok=%x\n",fpgaok);

    if(0xa5!=fpgaok) {

        printf("init epp error\n");
        return -1; //ghf-2014/8/26,出机打开
    }


    //复位fpga
    mySelWrt(0x02,BASEADDR);//写地址并设置为输出
    myWrtMore(0x0c,BASEADDR);//写数据

    usleep(100000);//延时2000*50us

    mySelRead(0x01,BASEADDR);//设置要读的地址
    fpgaversion =  myReadMore(BASEADDR);
    printf("emcmotStatus->fpgaversion=%x\n",fpgaversion);
    
	fd = open("/dev/rtc", O_RDONLY);  
	if (fd == -1) {  
		perror("/dev/rtc");  
		return -1;
	}  

	/* Read the RTC time/date */  
	retval = ioctl(fd, RTC_RD_TIME , &rtc_tm);  
	if (retval == -1) {  
		perror("ioctl");  
		return -1;
	}  
	close(fd);
	
	printf( "RTC date/time: %d/%d/%d %02d:%02d:%02d\n", rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);  
        
        

    return 0;
}



