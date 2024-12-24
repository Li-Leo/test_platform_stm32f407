from PIL import Image
from pathlib import Path
import time


def img_pixels_bytes(img):

    im = Image.open(img)
    im_data = im.getdata()

    # print(im.width, im.height, len(im_data))

    data = 0

    im_pixel_data_str = ''

    for i in range(im.height // 8):
        for j in range(im.width):
            for k in range(8):
                if im_data[j + k * im.width + i * 8 * im.width]:
                    data |= 1 << k
                else:
                    data |= 0 << k

            im_pixel_data_str += f"{data:#04x},"
            data = 0
        im_pixel_data_str += "\n"

    data = 0
    if (im.height % 8):
        for j in range(im.width):
            for k in range(8):
                if (k < im.width % 8):
                    if im_data[j + k * im.width + (im.height // 8) * 8 * im.width]:
                        data |= 1 << k
                    else:
                        data |= 0 << k
                else:
                    data |= 0 << k
            im_pixel_data_str += f"{data:#04x},"
            data = 0
    
    return im_pixel_data_str


def generate_img_data(path):

    with open('image_data.c', 'w', encoding='utf8') as f:
        f.write('#include "image_data.h"\n\n')

        # for im in path.glob('*.bmp'):

        for im in path.glob('*.bmp'):
            print(im.stem)
            f.write('const unsigned char img_' + im.stem +'[] = {\n')
            f.write(img_pixels_bytes(im))
            time.sleep(0.2)
            f.write('};\n\n')

        f.write('//const unsigned char *data, width, height\n')
        f.write('const static image_data_t image_data[] = {\n')

        for im in path.glob('*.bmp'):
            img = Image.open(im)
            if img.width % 8:
                width = (img.width // 8 + 1) * 8
            else:
                width = img.width
            f.write('	{' + f'img_{im.stem}, {img.width}, {img.height}' + '},\n')
        
        f.write('};\n')

        f.write( '\n//get image from the image id\n'\
            + 'const image_data_t* get_image(img_id_t img_id)\n'\
            + '{\n	if (img_id >= E_IMG_MAX) {\n'\
            + '		return NULL;\n	}\n'\
            + '	return &image_data[img_id];\n}\n')

    with open('image_data.h', 'w') as f:
        # f.write('#include "font_internal.h"\n#include "string.h"\n\n')
        f.write('#include "string.h"\n\n')
        f.write('#ifndef __IMAGE_DATA_H__\n#define __IMAGE_DATA_H__\n\n')
        f.write('typedef enum {\n')

        for im in path.glob('*.bmp'):
            f.write('	E_IMG_' + im.stem.upper() + ',\n')
        
        f.write('\n	E_IMG_MAX,\n')
        f.write('} img_id_t;\n\n')

        f.write('typedef struct {\n'
            + '	const unsigned char *data;\n'\
            + '	unsigned char width;	//in pixels\n'\
            + '	unsigned char height;	//in pixels\n'\
            + '} image_data_t;\n\n'

            + '//get image from the image id\n'\
            + 'const image_data_t* get_image(img_id_t img_id);\n\n'\
            + '#endif\n')


if __name__ == '__main__':

    input_str = input("please input image path: ")
    
    path = Path.cwd() / input_str
    generate_img_data(path)

    # for f in Path.cwd().glob('image_data.*'):
    # 	file_out = f.parent.parent / ('source\\kernel\\resource\\' + f.name)
    # 	if file_out.exists():
    # 		file_out.unlink()
    # 	f.rename(file_out)

