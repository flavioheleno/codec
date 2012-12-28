/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "codec.h"

int codec_write_header(FILE *handler, uint32_t width, uint32_t height) {
	if (file_write32(handler, CODEC_ID) != FILE_OK)
		return COD_WERR;
	if (file_write16(handler, (uint16_t)width) != FILE_OK)
		return COD_WERR;
	if (file_write16(handler, (uint16_t)height) != FILE_OK)
		return COD_WERR;
	return COD_OK;
}

int codec_read_header(FILE *handler, uint32_t *width, uint32_t *height) {
	uint32_t id;
	uint16_t tmp;
	if (file_read32(handler, &id) != FILE_OK)
		return COD_RERR;
	if (id != CODEC_ID)
		return COD_INV;
	if (file_read16(handler, &tmp) != FILE_OK)
		return COD_RERR;
	*width = (uint32_t)tmp;
	if (file_read16(handler, &tmp) != FILE_OK)
		return COD_RERR;
	*height = (uint32_t)tmp;
	return COD_OK;
}

void codec_error(int code) {
	switch (code) {
		case COD_WERR:
			printf("CODEC: write error\n");
			break;
		case COD_RERR:
			printf("CODEC: read error\n");
			break;
		case COD_INV:
			printf("CODEC: invalid file format\n");
			break;
	}
}
