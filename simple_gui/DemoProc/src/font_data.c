#include "font_data.h"

static font_name_t g_current_set_font;

SGUI_INT get_char_index(SGUI_UINT32 uiCode);
SGUI_BOOL char_is_fullwidth(SGUI_UINT32 uiCode);
SGUI_CSZSTR step_next_char_code(SGUI_CSZSTR cszSrc, SGUI_UINT32* puiCode);
SGUI_SIZE get_char_data(SGUI_SIZE sStartAddr, SGUI_BYTE* pDataBuffer, SGUI_SIZE sReadSize);

const char_info_t* get_font(font_name_t name);
int get_font_size(font_name_t name);


font_t all_font_info[] = {
	{DENGXIAN_REGULAR_16, dengxian_regular_16, 8, 16, 2},
	{DENGXIAN_BOLD_20, dengxian_bold_20, 11, 20, 10},
	{DENGXIAN_REGULAR_22, dengxian_regular_22, 11, 22, 8},
	{INCONSOLATA_REGULAR_16, inconsolata_regular_16, 8, 16, 52},
	{LXGW_REGULAR_18, lxgw_regular_18, 9, 18, 11},
	{INCONSOLATA_REGULAR_12, inconsolata_regular_12, 6, 12, 95},
	{INCONSOLATA_BOLD_20, inconsolata_bold_20, 6, 20, 24},
};

const SGUI_FONT_RES font_dengxian_bold_20 = {
	11,
	21,
	20,
	get_char_index,
	get_char_data,
	step_next_char_code,
	char_is_fullwidth
};

const SGUI_FONT_RES font_dengxian_regular_16 = {
	8,
	16,
	16,
	get_char_index,
	get_char_data,
	step_next_char_code,
	char_is_fullwidth
};

const SGUI_FONT_RES font_dengxian_regular_22 = {
	11,
	22,
	22,
	get_char_index,
	get_char_data,
	step_next_char_code,
	char_is_fullwidth
};

const SGUI_FONT_RES font_lxgw_regular_18 = {
	9,
	18,
	18,
	get_char_index,
	get_char_data,
	step_next_char_code,
	char_is_fullwidth
};

const SGUI_FONT_RES font_inconsolata_regular_16 = {
	8,
	16,
	16,
	get_char_index,
	get_char_data,
	step_next_char_code,
	char_is_fullwidth
};

const SGUI_FONT_RES font_inconsolata_regular_12 = {
	6,
	12,
	12,
	get_char_index,
	get_char_data,
	step_next_char_code,
	char_is_fullwidth
};


int get_char_index(SGUI_UINT32 uiCode)
{
	int i;

	for (i = 0; i < get_font_size(g_current_set_font); i++) {
		if (uiCode == get_font(g_current_set_font)[i].char_code) {
			return i;
		}
	}
	return -1;
}

SGUI_CSZSTR step_next_char_code(SGUI_CSZSTR cszSrc, SGUI_UINT32* puiCode)
{
    const SGUI_CHAR* p;
    SGUI_UINT32 uiCode;
	int bytes = 0;

    p = cszSrc;
    uiCode = 0;


    if (NULL != p) {
        do {
			if ((*p & 0x80) == 0) {
            	// 1-byte character (ASCII)
            	uiCode = *p;
				bytes = 1;
			} else if ((*p & 0xE0) == 0xC0) {
				// 2-byte character
				uiCode = *p & 0x1F;
				bytes = 2;
			} else if ((*p & 0xF0) == 0xE0) {
				// 3-byte character
				uiCode = *p & 0x0F;
				bytes = 3;
			} else if ((*p & 0xF8) == 0xF0) {
				// 4-byte character
				uiCode = *p & 0x07;
				bytes = 4;
			} else {
				// Invalid UTF-8 byte
				printf("Invalid UTF-8 sequence\n");
				// return;
			}
			// Read the remaining bytes
			for (int i = 1; i < bytes; ++i) {
				p++;
				if ((*p & 0xC0) != 0x80) {
					printf("Invalid UTF-8 sequence\n");
					// return -1;
				}
				uiCode = (uiCode << 6) | (*p & 0x3F);
			}
			p++;
        } while(0);
    }
    *puiCode = uiCode;
	// printf("U+%04X ", uiCode);

    return p;
}

SGUI_BOOL char_is_fullwidth(SGUI_UINT32 uiCode)
{
    SGUI_BOOL bReturn;

    if (uiCode < 0x7F) {
        bReturn = SGUI_FALSE;
    } else {
        bReturn = SGUI_TRUE;
    }

    return bReturn;
}

const char_info_t* get_font(font_name_t name)
{
	return all_font_info[name].char_info;
}

int get_font_half_width(font_name_t name)
{
	return all_font_info[name].half_width;
}

int get_font_height(font_name_t name)
{
	return all_font_info[name].height;
}

int get_font_size(font_name_t name)
{
	return all_font_info[name].total_count;
}

SGUI_SIZE get_char_data(SGUI_SIZE sStartAddr, SGUI_BYTE* pDataBuffer, SGUI_SIZE sReadSize)
{
    SGUI_SIZE sReadCount;
    const SGUI_BYTE* pSrc;
    SGUI_BYTE* pDest;
	int index;

	index = sStartAddr / (get_font_half_width(g_current_set_font) * (((get_font_height(g_current_set_font) - 1) / 8) + 1));
	// printf("index = %d, sReadSize = %d,  %d %d, %d \n", index, sReadSize, sStartAddr, get_font_half_width(g_set_font), (get_font_height(g_set_font) - 1) / 8 + 1);

    pSrc = get_font(g_current_set_font)[index].data;
    pDest = pDataBuffer;

    if (NULL != pDataBuffer) {
        for (sReadCount = 0; sReadCount < sReadSize; sReadCount++) {
            *pDest++ = *pSrc++;
        }
    }
    return sReadCount;
}


void set_display_font(font_name_t name)
{
	g_current_set_font = name;
}


