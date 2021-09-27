#include <common.h>
#include <env.h>
#include <i2c.h>
#include <i2c_eeprom.h>
#include <eeprom.h>
#include <eeprom_layout.h>
#include "i2c_eeprom_myir.h"

static struct id_eeprom eeprom;

int show_eeprom(void)
{
    char safe_string[24];
    char sn[20];
    int i, len;
    u8 *p;
    u8 *m;
    puts("Module INFO:\n");
    /* pn */
len=sizeof(eeprom.pn);
//len=strlen(eeprom.pn);
//printf("pn0:%d address:i:%c\n",len,eeprom.pn[16]);
    if(len > 32)//eliminate eeprom initial values
	return -1;
    else{
    	for (i = 0; i < 24; i++){
		safe_string[i] = eeprom.pn[i];
    	}
		safe_string[i] = '\0';
		//len = strlen(safe_string);
     	if (len > 0) {
        printf(">>>PN=%s\n", safe_string);
        env_set("PN", safe_string);
    	} else {
        	puts("unknown hardware variant\n");
    	}
    }
    /* Serial number */
len=sizeof(eeprom.sn);
//printf("sn0:%d c:%c,c2:%c\n",len,eeprom.sn[0],eeprom.sn[1]);
   if(len > 32)
	   return -1;
   else{
   	for (i = 0; i < 17; i++){
		sn[i] = eeprom.sn[i];
	}
	
		sn[i] = '\0';
//printf("eeprom:%c %c\n",eeprom.sn[16],eeprom.sn[17]);
    	if (len > 0){
        printf(">>>SN=%s\n", sn);
        env_set("SN", sn);
    	} else {
        	puts("nunknown serial number\n");
    	}
   }
    /* MAC address  */
    p = eeprom.mac0;
//printf("\nmac:%02x\n",p[0]);
    if (!is_valid_ethaddr(p)) {
        printf("Not valid MAC address in eeprom!\n");
        return 0;
    }

    printf(">>>MAC0=%02x:%02x:%02x:%02x:%02x:%02x\n",
           p[0], p[1], p[2], p[3], p[4], p[5]);

    eth_env_set_enetaddr("eth0ddr", p);
    m = eeprom.mac1;
    if (!is_valid_ethaddr(m)) {
        printf("Not valid MAC1 address in eeprom!\n");
        return 0;
    }
     printf(">>>MAC1=%02x:%02x:%02x:%02x:%02x:%02x\n",
           m[0], m[1], m[2], m[3], m[4], m[5]);
     eth_env_set_enetaddr("eth2addr", m);
    return 0;
}

int read_eeprom(void)
{
    struct udevice *dev;
    int ret = 0;
    //at24LC32
    
    ret = i2c_get_chip_for_busnum(0,/*i2c0 = &i2c4*/
                                  0x50,
                                  2, &dev);
    if (ret) {
        printf("Cannot find EEPROM !\n");
        return ret;
    }

    i2c_set_chip_offset_len(dev, 2);
    ret = dm_i2c_read(dev, 0x00, (uchar *)&eeprom, sizeof(eeprom));
    if(ret){
    	printf("Read eeprom fail !\n");
    }
    
    return ret;
}
		//}

