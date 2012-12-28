/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "binary.h"

int8_t binary_expand8(uint8_t value, uint8_t size) {
	uint8_t mask = 0x00;
	/* creates bit mask */
	mask = binary_mask8(size);
	/* if the value is a positive representation */
	if (((value >> (size - 1)) & 0x01) == 0x01)
		return (int8_t)(value & mask);
	/* if the value is a negative representation */
	else
		return (int8_t)((~value) & mask);
}

uint8_t binary_reduce8(int8_t value) {
	if (value == 0)
		/* reduced 0 representation */
		return (uint8_t)0x80;
	if (value < 0)
		/* negative representation */
		return (uint8_t)~abs(value);
	else
		/* positive representation */
		return (uint8_t)value;
}

int binary_size8(int8_t value) {
	int ret = 0;
	int flag = 1;
	int count = 0;
	/* avoids negative values */
	if (value < 0)
		value = (uint8_t)abs(value);
	/* loop to check first 1 bit */
	while ((flag) && (count < 8)) {
		if ((value & 0x80) == 0x80) {
			ret = (8 - count);
			flag = 0;
		}
		count++;
		value <<= 1;
	}
	return ret;
}

char *binary_print8(uint8_t value) {
	char *s;
	int i;
	s = (char *)malloc(sizeof(char) * 9);
	s[8] = 0;
	for (i = 0; i < 8; i++) {
		if ((value & 0x01) == 0x01)
			s[(7 - i)] = '1';
		else
			s[(7 - i)] = '0';
		value >>= 1;
	}
	return s;
}

uint8_t binary_mask8(uint8_t size) {
	uint8_t mask = 0x00;
	uint8_t i;
	for (i = size; i > 0; i--) {
		mask <<= 1;
		mask |= 0x01;
	}
	return mask;
}

uint8_t binary_extract8(uint8_t value, uint8_t offset, uint8_t size) {
	uint8_t mask = binary_mask8(size);
	uint8_t rotate = 8;
	uint8_t span = (offset + size);
	/* checks if span is bigger than buffer */
	if (span > 8)
		return 0;
	rotate -= span;
	return ((value >> rotate) & mask);
}


int32_t binary_expand32(uint32_t value, uint32_t size) {
	uint32_t mask = 0x00000000;
	/* creates bit mask */
	mask = binary_mask32(size);
	/* if the value is a positive representation */
	if (((value >> (size - 1)) & 0x00000001) == 0x00000001)
		return (int32_t)(value & mask);
	/* if the value is a negative representation */
	else
		return -((int32_t)((~value) & mask));
}

uint32_t binary_reduce32(int32_t value) {
	uint32_t mask;
	if (value == 0) {
		printf("reducing 0\n");
		/* reduced 0 representation */
		return (uint8_t)0x00000080;
	}
	mask = binary_mask32(binary_size32(value));
	if (value < 0)
		/* negative representation */
		return ((uint32_t)~abs(value) & mask);
	else
		/* positive representation */
		return ((uint32_t)value & mask);
}

int binary_size32(int32_t value) {
	int ret = 0;
	int flag = 1;
	int count = 0;
	/* avoids negative values */
	if (value < 0)
		value = (uint32_t)abs(value);
	/* loop to check first 1 bit */
	while ((flag) && (count < 32)) {
		if ((value & 0x80000000) == 0x80000000) {
			ret = (32 - count);
			flag = 0;
		}
		count++;
		value <<= 1;
	}
	return ret;
}

char *binary_print32(uint32_t value) {
	char *s;
	int i;
	s = (char *)malloc(sizeof(char) * 33);
	s[32] = 0;
	for (i = 0; i < 32; i++) {
		if ((value & 0x00000001) == 0x00000001)
			s[(31 - i)] = '1';
		else
			s[(31 - i)] = '0';
		value >>= 1;
	}
	return s;
}

uint32_t binary_mask32(uint32_t size) {
	uint32_t mask = 0x00000000;
	uint32_t i;
	for (i = size; i > 0; i--) {
		mask <<= 1;
		mask |= 0x00000001;
	}
	return mask;
}

uint32_t binary_extract32(uint32_t value, uint8_t offset, uint8_t size) {
	uint32_t mask = binary_mask32((uint32_t)size);
	uint8_t rotate = 32;
	uint8_t span = (offset + size);
	/* checks if span is bigger than buffer */
	if (span > 32)
		return 0;
	rotate -= span;
	return ((value >> rotate) & mask);
}
