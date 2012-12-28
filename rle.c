/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "rle.h"

/* auxiliar function to encode repeat/value information from rle */
uint16_t encode(rle info) {
	uint16_t ret;
	ret = info.repeat << 8;
	if (info.value < 0) {
		ret |= 0x8000;
		ret |= abs(info.value);
	} else if (info.value != 0)
		ret |= info.value;
	return ret;
}

int rle_pre_encode(pixel_int32 vector[64], rle *info[3], int size[3]) {
	int i, k, f, count;
	for (k = 0; k < 3; k++) {
		info[k] = NULL;
		size[k] = 0;
		count = 0;
		for (i = 2; i < 64; i++) {
			switch (k) {
				case 0:
					f = (vector[i].r != vector[(i - 1)].r);
					break;
				case 1:
					f = (vector[i].g != vector[(i - 1)].g);
					break;
				case 2:
					f = (vector[i].b != vector[(i - 1)].b);
					break;
			}
			if (f) {
				info[k] = (rle *)realloc(info[k], (sizeof(rle) * (size[k] + 1)));
				if (info[k] == NULL)
					return RLE_MERR;
				info[k][size[k]].repeat = count;
				switch (k) {
					case 0:
						info[k][size[k]].value = vector[(i - 1)].r;
						break;
					case 1:
						info[k][size[k]].value = vector[(i - 1)].g;
						break;
					case 2:
						info[k][size[k]].value = vector[(i - 1)].b;
						break;
				}
				count = 0;
				size[k]++;
			} else
				count++;
		}
		/* last red element */
		info[k] = (rle *)realloc(info[k], (sizeof(rle) * (size[k] + 1)));
		if (info[k] == NULL)
			return RLE_MERR;
		info[k][size[k]].repeat = count;
		switch (k) {
			case 0:
				info[k][size[k]].value = vector[(i - 1)].r;
				break;
			case 1:
				info[k][size[k]].value = vector[(i - 1)].g;
				break;
			case 2:
				info[k][size[k]].value = vector[(i - 1)].b;
				break;
		}
		size[k]++;
	}
	return RLE_OK;
}

void rle_print(rle *info[3], int size[3]) {
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < size[i]; j++) {
			printf("info[%d][%d].repeat = %d\n", i, j, info[i][j].repeat);
			printf("info[%d][%d].value = %d\n", i, j, info[i][j].value);
		}
}

int rle_encode(FILE *handler, pixel_int32 **vector, int size) {
	int i, j, k;
	int max, val;
	list *huff = NULL;
	huffman *huff_tbl = NULL;
	int *freq, code_bits, huff_tbl_size;
	uint16_t num_items;
	uint32_t code;
	float align;
	rle_info *info;
	/* generates rle info for every vector entry */
	info = (rle_info *)malloc(sizeof(rle_info) * size);
	for (i = 0; i < size; i++)
		rle_pre_encode(vector[i], info[i].info, info[i].size);
	for (k = 0; k < 3; k++) {
		max = 0;
		for (i = 0; i < size; i++)
			for (j = 0; j < info[i].size[k]; j++) {
				val = encode(info[i].info[k][j]);
				if (max < val)
					max = val;
			}
		max++;
		freq = (int *)malloc(sizeof(int) * max);
		for (i = 0; i < max; i++)
			freq[i] = 0;
		for (i = 0; i < size; i++)
			for (j = 0; j < info[i].size[k]; j++) {
				val = encode(info[i].info[k][j]);
				freq[val]++;
			}
		num_items = 0;
		for (i = 0; i < max; i++)
			if (freq[i] > 0)
				num_items++;
		if (file_write16(handler, num_items) != FILE_OK)
			return RLE_WERR;
		for (i = 0; i < max; i++)
			if (freq[i] > 0) {
				if (file_write16(handler, i) != FILE_OK)
					return RLE_WERR;
				if (file_write16(handler, freq[i]) != FILE_OK)
					return RLE_WERR;
				huff_add(&huff, i, freq[i]);
			}
		huff_tbl_size = huff_code(huff, &huff_tbl);
		list_clean(&huff);
		align = 0.0;
		for (i = 0; i < size; i++)
			for (j = 0; j < info[i].size[k]; j++) {
				val = encode(info[i].info[k][j]);
				code_bits = huff_search(huff_tbl, huff_tbl_size, val, &code);
				if (code_bits == -1) {
					printf("searching %d\n", val);
					return RLE_HERR;
				}
				align += code_bits;
			}
		/* calculates alignment size */
		align = ceil((float)align / 32.0);
		/* writes alignment to file */
		if (file_write16(handler, (uint16_t)align) != FILE_OK)
			return RLE_WERR;
		for (i = 0; i < size; i++)
			for (j = 0; j < info[i].size[k]; j++) {
				val = encode(info[i].info[k][j]);
				code_bits = huff_search(huff_tbl, huff_tbl_size, val, &code);
				if (code_bits == -1)
					return RLE_HERR;
				if (file_write_buffered(handler, code, code_bits, 0) != FILE_OK)
					return RLE_WERR;
			}
		if (file_write_buffered(handler, 0, 0, 1) != FILE_OK)
			return RLE_WERR;

		huff_clean(&huff_tbl, huff_tbl_size);
		free(freq);
	}
	return RLE_OK;
}

int rle_decode(FILE *handler, pixel_int32 **vector, int size) {
	int i, j, pos;
	uint16_t num_items, align, item, freq;
	list *huff = NULL;
	huffman *huff_tbl = NULL;
	int huff_tbl_size;
	uint32_t *buffer;
	int flag = 1, value;
	uint8_t history, code_bits, offset;
	uint32_t code;
	int repeat, current, vc;
	for (i = 0; i < 3; i++) {
		if (file_read16(handler, &num_items) != FILE_OK)
			return RLE_RERR;
		for (j = 0; j < num_items; j++) {
			if (file_read16(handler, &item) != FILE_OK)
				return RLE_RERR;
			if (file_read16(handler, &freq) != FILE_OK)
				return RLE_RERR;
			huff_add(&huff, item, freq);
		}
		huff_tbl_size = huff_code(huff, &huff_tbl);
		list_clean(&huff);
		if (file_read16(handler, &align) != FILE_OK)
			return RLE_RERR;
		if (file_read_buffered(handler, &buffer, align) != FILE_OK)
			return RLE_RERR;
		flag = 1;
		pos = 0;
		offset = 0;
		history = 0;
		code_bits = 0;
		current = 1;
		vc = 0;
		while (flag) {
			code_bits++;
			/* code is splited into two different chunks */
			if ((offset + code_bits) > 0x20) {
				history = (0x20 - offset);
				code = binary_extract32(buffer[pos], offset, history);
				code_bits -= history;
				code <<= code_bits;
				offset = 0;
				pos++;
				code |= binary_extract32(buffer[pos], offset, code_bits);
			} else {
				if (history == 0)
					code = binary_extract32(buffer[pos], offset, code_bits);
				else {
					code = binary_extract32(buffer[(pos - 1)], (0x20 - history), history);
					code <<= code_bits;
					code |= binary_extract32(buffer[pos], offset, code_bits);
				}
			}
			if (huff_check(huff_tbl, huff_tbl_size, code, (code_bits + history), &value)) {
				repeat = ((value >> 8) & 0xFF);
				value &= 0x00FF;
				if ((repeat & 0x80) == 0x80) {
					repeat &= 0x7F;
					value *= -1;
				}
				while (repeat >= 0) {
					switch (i) {
						case 0:
							vector[vc][current].r = value;
							break;
						case 1:
							vector[vc][current].g = value;
							break;
						case 2:
							vector[vc][current].b = value;
							break;
					}
					current++;
					repeat--;
				}
				if (current == 64) {
					current = 1;
					vc++;
				}
				if (current > 64)
					return RLE_FERR;
				offset += code_bits;
				code_bits = 0;
				history = 0;
			}
			if (pos >= align)
				return RLE_FERR;
			/* checks if data was fully extracted */
			if (vc == size)
				flag = 0;
		}
		/* cleans data buffer */
		free(buffer);
		/* cleans huffman table */
		huff_clean(&huff_tbl, huff_tbl_size);
	}
	return RLE_OK;
}

void rle_clean(rle **table, int size) {
	
}

void rle_error(int code) {
	switch (code) {
		case RLE_MERR:
			printf("RLE: malloc error\n");
			break;
		case RLE_WERR:
			printf("RLE: write error\n");
			break;
		case RLE_FERR:
			printf("RLE: file format error\n");
			break;
		case RLE_RERR:
			printf("RLE: read error\n");
			break;
		case RLE_HERR:
			printf("RLE: huffman error\n");
			break;
	}
}
