#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define BitsForPageNo 7
#define BitsForPageOffset 9
#define ProcessSize 17800

void init_page_table(int table[], int no_page);
char* to_binary(unsigned short addr);
unsigned short logical2physical(unsigned short logical_addr, int page_table[]);

unsigned short logical2physical(unsigned short logical_addr, int page_table[]) {
	unsigned short page_num, page_offset, physical_addr, frame_num;

	page_num = logical_addr >> BitsForPageOffset;
	page_offset = logical_addr & 0x01ff;
	frame_num = page_table[page_num];
	physical_addr = (frame_num << BitsForPageOffset) | page_offset;
	
	return physical_addr;
}

int main() {
	int no_page = 0;
	int *page_table = NULL;
	unsigned short logical_addr[] = { 0x21dd, 0x0829, 0x08f0, 0x0ad1, 0x071e, 0x0431, 0x0fb8, 0x0b18, 0x38ab, 0x07a0 } ;

	int i = 0;
	if ((ProcessSize % (1 << BitsForPageOffset)) == 0)
		no_page = ProcessSize / (1 << BitsForPageOffset);
	else
		no_page = (ProcessSize / (1 << BitsForPageOffset)) + 1;
	page_table = (int*)malloc(no_page*sizeof(int));
	if (page_table == NULL) {
		printf("Failed to allocate memory!\n");
		exit(-1);
	}

	init_page_table(page_table, no_page);
	for (i = 0; i < 10; i++) {
		unsigned short physical_addr = logical2physical(logical_addr[i], page_table);
		printf("0x%04x (%s) ", logical_addr[i], to_binary(logical_addr[i]));
		printf("--> 0x%04x (%s)\n", physical_addr, to_binary(physical_addr));
	}
	free(page_table);

	return 0;
}

char* to_binary(unsigned short addr) {
	static char bits[17];
	int i = 0;
	for (i = 0; i < 16; i++) {
		bits[15 - i] = '0' + (addr & 1);
		addr >>= 1;
	}
	bits[16] = 0;

	return bits;
}

void init_page_table(int table[], int no_page) {
	int i = 0;

	for (i = 0; i < no_page; i++) {
		table[i] = i * 2 + 3;
	}

	return;
}

/* Q1: What is the maximum number of page table entries?
A1: Maximum number of page table entries is same as number of pages.
Q2: What is the size of a page in bytes?
A2: Since page osset is 9 bit, the size of a page is 2 to the power of 9, which is 512 bytes.
Q3: How many pages are necessary to store a process whose size is 17800 bytes?
A3: Since the size of a page is 512 bytes, 35 pages are needed to store a process of 17800 bytes. */ 
