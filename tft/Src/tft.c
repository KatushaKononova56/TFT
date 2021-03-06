/*
 * tft.c
 *
 *  Created on: Oct 23, 2021
 *      Author: Maria
 */


#include "tft.h"

static uint16_t count_pixels;

void TFT_init(){
	GPIOA_init_spi_TFT();

	spi1_master_init(8);
	LED_on();
	Micro_tick_delay(100);

	RESET_ACTIVE();
	Micro_tick_delay(20000);

	tft_display_normal_mode();
	tft_pixel_format();

	tft_sleep_out();
	Micro_tick_delay(120);
	tft_display_on();
	Micro_tick_delay(120);
	tft_color_ALL(GREEN);
}

void Micro_tick_delay(uint32_t tick){
	for(uint32_t i=0;i<tick;i++){}
}

void tft_write_cmd(uint8_t cmd, uint8_t *data, uint8_t size) {
	if (size == 0) {
		return;
	}

	DC_COMMAND();
	spi1_SendDataDMA_1Byte(&cmd, 1);
	Micro_tick_delay(10);

	DC_DATA();
	spi1_SendDataDMA_1Byte(&data[0], size);
	Micro_tick_delay(10);
}

void tft_sleep_out() {
	DC_COMMAND();
	uint8_t data=Sleep_out;
	spi1_SendDataDMA_1Byte(&data, 1);
	Micro_tick_delay(10000);
}

void tft_display_off() {
	tft_write_cmd(Display_OFF, 0, 0);
}

void tft_display_on() {
	uint8_t data=ROTATION;
	tft_write_cmd(Display_ON, &data, 1);
}

void tft_display_normal_mode() {
	DC_COMMAND();
	uint8_t data=Normal_Display_mode;
	spi1_SendDataDMA_1Byte(&data, 1);
}

void tft_pixel_format() {
	uint8_t data = Pixel_16_bits;
	tft_write_cmd(Pixel_Format_Set, &data, 1);
}


void tft_set_column(uint16_t col_start, uint16_t col_end) {
	uint8_t data_column[4] = {	(uint8_t)((col_start >> 8) & 0xFF),
								(uint8_t)(col_start & 0xFF),
								(uint8_t)((col_end >> 8) & 0xFF),
								(uint8_t)(col_end & 0xFF)};

	tft_write_cmd(Column_Address_Set, &data_column[0], 4);
}

void tft_set_row(uint16_t row_start, uint16_t row_end) {
	uint8_t data_row[4] = {	(uint8_t)(row_start >> 8),
							(uint8_t)(row_start & 0xFF),
							(uint8_t)(row_end >> 8),
							(uint8_t)(row_end & 0xFF)};

	tft_write_cmd(Page_Adress_Set, &data_row[0], 4);
}

void tft_ram_write(uint8_t *data, uint8_t size) {
	tft_write_cmd(Memory_Write, &data[0], size);
}



void tft_set_region(uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end) { // выбор области
	count_pixels = (row_end - row_start) * (col_end - col_start);
	uint8_t data = 0x00;
	tft_write_cmd(Memory_Access_Control, &data, 1);

	tft_set_column(col_start, col_end);
	tft_set_row(row_start, row_end);
	tft_ram_write(&data, 1);
}

void tft_color_ALL(uint16_t color) {
	tft_set_region(0,MAX_X-1,0,MAX_Y-1);
	DC_DATA();
	spi1_SendDataDMA_2byteNTimes(color, count_pixels);
}

void tft_color_XY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color) {
	if (x1>=MAX_X || y1>=MAX_Y || x2>=MAX_X || y2>=MAX_Y) return;
	if (x1>x2) swap(&x1,&x2);
	if (y1>y2) swap(&y1,&y2);
	tft_set_region(x1,x2,y1,y2);
	DC_DATA();
	spi1_SendDataDMA_2byteNTimes(color, count_pixels);
}

void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color){
	RESET_CS();
	tft_set_region(x, x+1, y, y+1);
	SET_CS();
	DC_DATA();
	spi1_SendDataDMA_2byteNTimes(color, 1);
}
void tft_SetRotation(uint8_t r){
	uint8_t data;
	switch(r){
		case 0:
			data= 0x48;
			tft_write_cmd(Memory_Access_Control, &data, 1);
			X_SIZE = 240;
			Y_SIZE = 320;
			break;
		case 1:
			data= 0x28;
			tft_write_cmd(Memory_Access_Control, &data, 1);
			X_SIZE = 320;
			Y_SIZE = 240;
			break;
		case 2:
			data=0x88;
			tft_write_cmd(Memory_Access_Control, &data, 1);
			X_SIZE = 240;
			Y_SIZE = 320;
			break;
		case 3:
			data=0xE8;
			tft_write_cmd(Memory_Access_Control, &data, 1);
			X_SIZE = 320;
			Y_SIZE = 240;
			break;
	}

}

void swap(uint16_t *a, uint16_t *b) {
	uint16_t t;
	t=*a;
	*a=*b;
	*b=t;
}
