#include "inittcon.h"

volatile uint32_t* virt_addr;
void *map_base;
unsigned mapped_size;
int fd;

static int Map(uint32_t mem_address, uint32_t mem_size){
	unsigned page_size, offset_in_page;


	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(fd  == -1){
		printf("Error: could not open /dev/mem...\n");
		return -1;
	}
	printf("Opened /dev/mem\n");


	mapped_size = page_size = getpagesize();
	offset_in_page = (unsigned)mem_address & (page_size - 1);
	if (offset_in_page + mem_size > page_size) mapped_size *= 2;

	map_base = mmap(NULL,
			mapped_size,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd,
			mem_address & ~(page_size - 1)
	);


	if (map_base == MAP_FAILED){
		printf("Error: memory map failed...\n");
		return -1;
	}
	printf("Mapped memory\n");
	virt_addr = map_base + offset_in_page; 
	return 0;
}
static int UMap(){
	int unmap_result = munmap(map_base, mapped_size);
	close(fd);
	if (unmap_result)return -1;
	return 0;
}
static void printbin(uint32_t L){
	char data[37] = {'\0'};
 	uint32_t i = 0x80000000;
	int b = 0;
	while(i){
		if(((b+1)%9) == 0 && b){
			data[b] = ',';
			b++;
		}
		data[b] = (L&i)? '1':'0';
		b++;
		i >>= 1;	
	}
	printf("%s\n",data);
}
static void writeI80(uint8_t data){
	virt_addr[24] = 0xE2000000;
	virt_addr[25] = (((uint32_t)data&0xE0)<<5)|(((uint32_t)data&0x1f)<<3);
	while(virt_addr[24]&0x800000){}
}
static void commandI80(uint8_t data){
	virt_addr[24] = 0xE4000000;
	virt_addr[25] = (((uint32_t)data&0xE0)<<5)|(((uint32_t)data&0x1f)<<3);
	while(virt_addr[24]&0x800000){}
}
static void blank(){
	for(uint8_t y = 0;y < 160;y++){
      for(uint8_t x = 0;x < 128;x++){
            		writeI80(0x00);
            		writeI80(0x00);
      }
	}
}
extern int tcon_init(){
	//Map(0x01C20800,4096);
	//virt_addr[27] = 0x11111110;
	//virt_addr[28] = 0x00000001;
	//virt_addr[29] = 0x00111100;
	//UMap();
	Map(0x01C0C000,16384);
	virt_addr[24] = 0xE2000000;
	virt_addr[0] =  0x80000000;//  \/
	virt_addr[1] =  0x00000000;
	virt_addr[2] =  0x00000000;
	virt_addr[3] =  0x00000000;
	virt_addr[4] =  0x00000000;
	virt_addr[5] =  0x11111111;
	virt_addr[6] =  0x11111111;
	virt_addr[7] =  0x11111111;
	virt_addr[8] =  0x11111111;
	virt_addr[9] =  0x11111111;
	virt_addr[10] = 0x11111111;
	virt_addr[11] = 0x01010000;
	virt_addr[12] = 0x15151111;
	virt_addr[13] = 0x57575555;
	virt_addr[14] = 0x7F7F7777;// ^ static vars to enable all default

	virt_addr[16] = 0x810001E0; // set mode to 8080
	virt_addr[17] = 0x800000FF;// set clocks first 8 bit is clock div but idk the top
	virt_addr[18] = ((((uint32_t) 127)&0xFFF) << 16) | (((uint32_t) 159)&0xFFF);//width +1 height +1 0x48
	virt_addr[19] = 0x01C00001; // ??? 4c
	virt_addr[20] = 0;// ??? 50
	virt_addr[21] = 0;// ??? 54 /0x041A0022

	virt_addr[22] = 0;// HV not needed 58

	commandI80(0xfe);
	commandI80(0xfe);
	commandI80(0xef);

	commandI80(0xb3);  
	writeI80(0x03);
  
	commandI80(0xb6);  
	writeI80(0x01);
  
	commandI80(0xa3);  
	writeI80(0x11);
  
	commandI80(0x21);
  
	commandI80(0x36);  
	writeI80(0xd0);
  
	commandI80(0x3a);  
	writeI80(0x05);
  
	commandI80(0xb4);  
	writeI80(0x21);

  
	commandI80(0xF0);
	writeI80(0x31);   
	writeI80(0x4C);
	writeI80(0x24);   
	writeI80(0x58);
	writeI80(0xA8);   
	writeI80(0x26);
	writeI80(0x28);   
	writeI80(0x00);
	writeI80(0x2C);   
	writeI80(0x0C);
	writeI80(0x0C);   
	writeI80(0x15);
	writeI80(0x15);   
	writeI80(0x0f); 

	commandI80(0xF1);
	writeI80(0x0E);  
	writeI80(0x2D); 
	writeI80(0x24);   
	writeI80(0x3E);
	writeI80(0x99);   
	writeI80(0x12);
	writeI80(0x13);   
	writeI80(0x00);
	writeI80(0x0A);   
	writeI80(0x0D);
	writeI80(0x0D);   
	writeI80(0x14); 
	writeI80(0x13);   
	writeI80(0x0f);

	commandI80(0xfe);
	commandI80(0xff);

	commandI80(0x11);
	delay(120);
	commandI80(0x29);
	commandI80(0x2C);
	writeI80(0x00);
	writeI80(0x00);
	delay(200);
	virt_addr[17] = 0x80000006;// set clocks first 8 bit is clock div but idk the top
	virt_addr[24] = 0xF4000000; //set mode 8080 8bit 256 and auto 0x12000000 sets to command 0xC4000000 // E set without auto 
	UMap();

return 0;
}
