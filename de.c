/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "de.h"

void de_print(de_table *table, int size) {
	int i;
	printf("printing de table (%d items)\n", size);
	printf("diff\tbits\tcode\tbits\n");
	for (i = 0; i < size; i++)
		printf("%d\t%d\t%d\t%d\n", table[i].diff, table[i].diff_bits, table[i].code, table[i].code_bits);
	printf("\n");
}

int de_encode(FILE *handler, pixel_int32 *vector, int size) {
	int i, k, pos, max = 0, num_items;
	int *freq;
	list *huff = NULL;
	huffman *huff_tbl = NULL;
	uint32_t code;
	int huff_tbl_size, code_bits;
	float align;
	de_table *tbl = (de_table *)malloc(sizeof(de_table) * (size * 3));
	if (tbl == NULL)
		return DE_MERR;
	for (k = 0; k < 3; k++) {
		for (i = (size - 1); i >= 0; i--) {
			pos = (i + (k * size));
			switch (k) {
				case 0:
					if (i > 0)
						tbl[pos].diff = (vector[i].r - vector[(i - 1)].r);
					else
						tbl[pos].diff = vector[i].r;
					break;
				case 1:
					if (i > 0)
						tbl[pos].diff = (vector[i].g - vector[(i - 1)].g);
					else
						tbl[pos].diff = vector[i].g;
					break;
				case 2:
					if (i > 0)
						tbl[pos].diff = (vector[i].b - vector[(i - 1)].b);
					else
						tbl[pos].diff = vector[i].b;
					break;
			}
			if (tbl[pos].diff == 0)
				tbl[pos].diff_bits = 0;
			else
				tbl[pos].diff_bits = binary_size32(tbl[pos].diff);
			if (max < tbl[pos].diff_bits)
				max = tbl[pos].diff_bits;
		}
	}
	max++;
	freq = (int *)malloc(sizeof(int) * (size_t)max);
	if (freq == NULL)
		return DE_MERR;
	for (i = 0; i < max; i++)
		freq[i] = 0;
	for (pos = 0; pos < (size * 3); pos++)
		freq[tbl[pos].diff_bits]++;
	num_items = 0;
	for (pos = 0; pos < max; pos++)
		if (freq[pos] > 0) {
			num_items++;
			huff_add(&huff, pos, freq[pos]);
		}
	if (file_write16(handler, num_items) != FILE_OK)
		return DE_WERR;
	for (pos = 0; pos < max; pos++)
		if (freq[pos] > 0) {
			if (file_write8(handler, pos) != FILE_OK)
				return DE_WERR;
			if (file_write16(handler, freq[pos]) != FILE_OK)
				return DE_WERR;
		}
	free(freq);
	huff_tbl_size = huff_code(huff, &huff_tbl);
	align = 0.0;
	for (pos = 0; pos < (size * 3); pos++) {
		code_bits = huff_search(huff_tbl, huff_tbl_size, tbl[pos].diff_bits, &code);
		if (code_bits == -1)
			return DE_HERR;
		tbl[pos].code = code;
		tbl[pos].code_bits = code_bits;
		if (tbl[pos].diff != 0) {
			tbl[pos].code <<= tbl[pos].diff_bits;
			tbl[pos].code |= binary_reduce32(tbl[pos].diff);
			tbl[pos].code_bits += tbl[pos].diff_bits;
		}
		align += tbl[pos].code_bits;
	}
	/* calculates alignment size */
	align = ceil((float)align / 32.0);
	/* writes alignment to file */
	if (file_write16(handler, (uint16_t)align) != FILE_OK)
		return DE_WERR;
	for (pos = 0; pos < (size * 3); pos++)
		if (file_write_buffered(handler, tbl[pos].code, tbl[pos].code_bits, 0) != FILE_OK)
			return DE_WERR;
	if (file_write_buffered(handler, 0, 0, 1) != FILE_OK)
		return DE_WERR;
	huff_clean(&huff_tbl, huff_tbl_size);
	free(tbl);
	return DE_OK;
}

int de_decode(FILE *handler, pixel_int32 *vector, int size) {
	int pos, i;
	uint8_t item;
	uint16_t num_items, freq, align;
	list *huff = NULL;
	huffman *huff_tbl = NULL;
	int huff_tbl_size;
	uint32_t *buffer;
	int flag = 1, value, extracted;
	uint8_t history, code_bits, offset;
	uint32_t code, content;
	int diff;
	if (file_read16(handler, &num_items) != FILE_OK)
		return DE_RERR;
	for (pos = 0; pos < num_items; pos++) {
		if (file_read8(handler, &item) != FILE_OK)
			return DE_RERR;
		if (file_read16(handler, &freq) != FILE_OK)
			return DE_RERR;
		huff_add(&huff, item, freq);
	}
	huff_tbl_size = huff_code(huff, &huff_tbl);
	if (file_read16(handler, &align) != FILE_OK)
		return DE_RERR;
	if (file_read_buffered(handler, &buffer, align) != FILE_OK)
		return DE_RERR;
	/* decodes buffer read from file */
	flag = 1;
	i = 0;
	offset = 0;
	history = 0;
	code_bits = 0;
	extracted = 0;
	diff = 0;
	while (flag) {
		code_bits++;
		/* code is splited into two different chunks */
		if ((offset + code_bits) > 0x20) {
			history = (0x20 - offset);
			code = binary_extract32(buffer[i], offset, history);
			code_bits -= history;
			code <<= code_bits;
			offset = 0;
			i++;
			code |= binary_extract32(buffer[i], offset, code_bits);
		} else {
			if (history == 0)
				code = binary_extract32(buffer[i], offset, code_bits);
			else {
				code = binary_extract32(buffer[(i - 1)], (0x20 - history), history);
				code <<= code_bits;
				code |= binary_extract32(buffer[i], offset, code_bits);
			}
		}
		if (huff_check(huff_tbl, huff_tbl_size, code, (code_bits + history), &value)) {
			offset += code_bits;
			code_bits = 0;
			/* buffer is splited into two different chunks */
			if ((offset + value) > 0x20) {
				history = (0x20 - offset);
				content = binary_extract32(buffer[i], offset, history);
				value -= history;
				content <<= value;
				offset = 0;
				i++;
				content |= binary_extract32(buffer[i], offset, value);
				content = binary_expand32(content, (history + value));
			} else {
				content = binary_extract32(buffer[i], offset, value);
				content = binary_expand32(content, value);
			}
			history = 0;
			offset += value;
			if ((extracted == size) || (extracted == (2 * size)))
				diff = 0;
			diff += content;
			if (extracted < size)
				vector[extracted].r = diff;
			else if (extracted < (2 * size))
				vector[(extracted - size)].g = diff;
			else
				vector[(extracted - (2 * size))].b = diff;
			extracted++;
		}
		if (i >= align)
			return DE_FERR;
		/* checks if buffer was fully extracted */
		if (extracted == (3 * size))
			flag = 0;
	}
	return DE_OK;
}

void de_error(int code) {
	switch (code) {
		case DE_MERR:
			printf("DE: malloc error\n");
			break;
		case DE_HERR:
			printf("DE: huffman error\n");
			break;
		case DE_WERR:
			printf("DE: write error\n");
			break;
		case DE_RERR:
			printf("DE: read error\n");
			break;
		case DE_FERR:
			printf("DE: fatal error\n");
			break;
	}
}
