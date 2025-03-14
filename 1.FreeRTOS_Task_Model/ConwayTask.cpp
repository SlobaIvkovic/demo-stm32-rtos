#include "stm32facade.h"

#include "ConwayTask.h"

#include <cstdlib>

#include "ssd1306.h"

#include "cmsis_os.h"

#include <cstdio>

//Mutex added
extern osMutexId I2CMutexHandle;

GameOfLife::GameOfLife(uint8_t w, uint8_t h):
     width(w),
     height(h),
     gen0(0),
     gen1(0) {
    /* --Allocate two buffers. */
    gen0=new uint8_t[width*height];
    gen1=new uint8_t[width*height];
}

GameOfLife::~GameOfLife() {
    /* --Free the memory. */
    delete [] gen0;
    delete [] gen1;
}

void GameOfLife::randomize() {
    /* --Just initialize the current generation. */
    for(uint32_t i=0;i<width*height;i++)
        gen0[i]=(rand()%20 < 3 ? 1 : 0);
}

void GameOfLife::clear() {
    /* --Initialize with zero. */
    for(uint32_t i=0;i<width*height;i++)
        gen0[i]=0;
}

void GameOfLife::setupGlider(uint8_t x, uint8_t y) {
    /* --Set up the glider. */
    if (x<width && y<height) {
        at(gen0,x, y==0 ? height-1 : y-1)=1;
        at(gen0,x==width-1 ? 0 : x+1,y)=1;
        at(gen0,x==0 ? width-1 : x-1, y==height-1 ? 0: y+1)=1;
        at(gen0,x, y==height-1 ? 0: y+1)=1;
        at(gen0,x==width-1 ? 0 : x+1, y==height-1 ? 0: y+1)=1;
    }
}

uint8_t& GameOfLife::at(uint8_t* base, uint8_t x, uint8_t y) const {
    /* --Jump the position. */
    return base[y*width+x];
}

bool GameOfLife::evolve() {
    bool isGameOver=false;

    for(uint8_t x=0; x<width; x++)
        for(uint8_t y=0; y<height; y++) {
            uint8_t counter=0;
            /* --Check the neighbors. */
            counter+=at(gen0, x==0 ? width-1 : x-1, y==0 ? height-1 : y-1);
            counter+=at(gen0, x,y==0 ? height-1 : y-1);
            counter+=at(gen0, x==width-1 ? 0 : x+1, y==0 ? height-1 : y-1);
            counter+=at(gen0, x==0 ? width-1 : x-1, y);
            counter+=at(gen0, x==width-1 ? 0 : x+1, y);
            counter+=at(gen0, x==0 ? width-1 : x-1, y==height-1 ? 0 : y+1);
            counter+=at(gen0, x, y==height-1 ? 0 : y+1);
            counter+=at(gen0, x==width-1 ? 0 : x+1, y==height-1 ? 0 : y+1);

            /* --Define next state. */
            bool isAlive=(at(gen0,x,y)==1);

            /* --Stay alive. */
            if (isAlive && (counter==2 || counter==3))
                at(gen1,x,y)=1;
            /* --Birth. */
            else if (!isAlive && counter==3)
                at(gen1,x,y)=1;
            /* --Death. */
            else
                at(gen1,x,y)=0;
        }

    /* --Switch the buffers. */
    uint8_t* tmp=gen0;
    gen0=gen1;
    gen1=tmp;

    return isGameOver;
}

void GameOfLife::run()
{

    Platform::BSP::SSD1306 oled(Platform::BSP::STM32Facade::instance(Platform::BSP::STM32Facade::I2C_Peripheral::UseI2C1));

    oled.initialize(width, height, 0x00);
    oled.showDemoPicture();
    oled.flush();

    clear();
    //randomize();
    setupGlider(1,1);
    setupGlider(10,10);
    setupGlider(20,20);

    oled.bitmap(get(),width,height);
    oled.flush();

    evolve();
    oled.bitmap(get(),width,height);
    oled.flush();

    evolve();
    oled.bitmap(get(),width,height);
    oled.flush();

    evolve();
    oled.bitmap(get(),width,height);
    oled.flush();

    while (true) {
        /* --Next generation. */
    	// FIXME: add output to OLED
        evolve();
        oled.bitmap(get(),width,height);
     //   oled.flush();

    }
}

void ConwayTask(const void * argument)
{
//		osThreadId id;
//		id = osThreadGetId();
	   GameOfLife gof(128,64);

	   Platform::BSP::SSD1306 oled(Platform::BSP::STM32Facade::instance(Platform::BSP::STM32Facade::I2C_Peripheral::UseI2C1));

	       oled.initialize(128, 64, 0x00);
	       oled.showDemoPicture();
	       oled.flush();

	       gof.clear();
	       gof.randomize();
	       gof.setupGlider(1,1);
	       gof.setupGlider(10,10);
	       gof.setupGlider(20,20);

	       for(;;)
	       {
	    	   gof.evolve();
	    	   oled.bitmap(gof.get(),128,64);

//	    	   osThreadSetPriority(id, osPriorityAboveNormal);
	    	   osMutexWait(I2CMutexHandle, osWaitForever);
	    	   oled.flush();
	    	   osMutexRelease(I2CMutexHandle);
//	    	   osThreadSetPriority(id, osPriorityNormal);

	    	   osDelay(347);
	       }

}
