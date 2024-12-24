from PIL import Image, ImageDraw, ImageFont
from pathlib import Path
import subprocess
import time


def generate_ch_img(ch_set, dir, width, height, font_name, font_style):

    with open('matrix_input', 'w', encoding='utf-16-le') as f:
        if font_name == 'dengxian':
            f.write('font:等线\n')
        elif font_name == 'Inconsolata':
            f.write('font:Inconsolata\n')
        elif font_name == 'LXGW':
            f.write('font:霞鹜文楷等宽\n')

        f.write('style:'+ font_style + '\n')
        f.write('height:'+ f'{height}' + '\n')
        f.write('average_width:0\n')
        f.write('output:' + str(dir) + '\n')

        text = ''.join(ch_set)
        f.write('text:' + text + '\n')

    res = subprocess.run(['type_matrix.exe', 'matrix_input'], shell=True)
    Path('matrix_input').unlink()


def read_char():

    with open('string_list.txt', 'r', encoding ='utf8') as f:
        char_set = set()
        for ch in f.read():
            if ch != '\n':
                char_set.add(ch)

    return char_set


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
                if (k < im.height % 8):
                    value =  im_data[j + k * im.width + (im.height // 8) * 8 * im.width]
                    if value:
                        data |= 1 << k
                    else:
                        data |= 0 << k
                else:
                    data |= 0 << k
            im_pixel_data_str += f"{data:#04x},"
            data = 0

    im.close()
    
    return im_pixel_data_str


# generate C code files
def generate_font_data_file(path, height, font_name, font_style):

    count = 0
    with open(f'{font_name.lower()}_{font_style}_{height}.c', 'w', encoding='utf8') as f:
        
        f.write('#include "font_data.h"\n\n')
        # f.write('#include "font_internal.h"\n\n')
        
        for im in path.glob('*.bmp'):
            chinese_ch = chr(int(im.stem, 16))
            f.write('static const unsigned char font_' + im.stem +'[] = {//' + chinese_ch + '\n')
            f.write(img_pixels_bytes(im) + '\n};\n\n')

        f.write(f'const char_info_t {font_name.lower()}_{font_style}_{height}[] = {{\n')
        for im in path.glob('*.bmp'):
            img = Image.open(im)
            utf8_encode_str  = str(chr(int(im.stem, 16)).encode('utf8')).strip('b')
            unicode  = int(im.stem, 16)

            chinese_ch = chr(int(im.stem, 16))
            # print(str(chr(int(im.stem, 16))))
            f.write('	{'  + f'{unicode:#06x}, font_{im.stem}, {img.width}, {img.height}' + '}, //' + chinese_ch + f' {utf8_encode_str}\n')
            img.close()
            count += 1
        f.write('};\n')
    return count


if __name__ == '__main__':
    
    path = Path('font_img')

    with open('string_list.txt', 'r', encoding ='utf8') as f:
        w = 0
        h = 0
        name = ''
        s = ''

        with open('font_info.h', 'w', encoding='utf8') as write_file:
            write_file.write(f'// font name, height, total count\n\n')
            for line in f:
                if not line.startswith('#') and line != '\n':

                    if line.startswith(';'):
                        s = ''
                        w = int(line[1:-1].split(',')[0])
                        h = int(line[1:-1].split(',')[1])
                        name = line[1:-1].split(',')[2]
                        style = line[1:-1].split(',')[3]
                        # print(w,h,name)
                        if path.exists():
                            for i in path.glob('*.*'):
                                i.unlink()
                        else:
                            path.mkdir()

                    elif line.startswith('$$'):
                        char_set = set()
                        for ch in s:
                            if ch != '\n':
                                char_set.add(ch)

                        # for ch in char_set:
                        # 	if ord(ch) != 0xfeff:
                        generate_ch_img(char_set, path, w, h, name, style)
                        time.sleep(1)
                        
                        # for im in path.glob('*.bmp'):
                        # 	if im.name == '002e.bmp' and h == 20:
                        # 		im = Image.open(im)
                        # 		im = im.crop((0,0,8,20)).save(path / '002e.bmp')

                        print(f'generate font {w}, {h}, {name}, {style}')
                        count = generate_font_data_file(path, h, name, style)
                        write_file.write(f'// {name.upper()}_{style.upper()}_{h}, {name.lower()}_{style.lower()}_{h}, {h}, {count}\n')

                    else:
                        s += line

        # delete font img
        # for i in path.glob('*.*'):
        # 	i.unlink()
        # path.rmdir()

    # convert to utf8 string
    with open('string_list.txt', 'r', encoding ='utf8') as f:
        with open ('utf8_string.txt', 'w', encoding ='utf8') as out_file:
            for line in f:
                if not line.startswith('#') and line != '\n':
                    if line.startswith(';'):
                        out_file.write(line)
                    elif line.startswith('$$'):
                        out_file.write('\n')
                    else:
                        byte_string = bytes(line, 'utf-8')
                        out_file.write('"')
                        for i in byte_string:
                            out_file.write(f'\\x{i:02x}')
                        out_file.write('"      ')
                        out_file.write(line)


    # with open('generated_font_data.h', 'w', encoding='utf8') as f:
    # 	f.write("//generated by generate_font_data_file.py, don't modify it\n")
    # 	for file in Path.cwd().glob('*.c'):
    # 		f.write(f'extern const TS_FONT g_font_{file.stem};\n')

    # 	f.write('\nstatic const TS_FONT_DATA g_font_data_simplified_chinese[] = {\n')
        
    # 	for file in Path.cwd().glob('*.c'):
    # 		f.write('	{"' + file.stem.split('_')[0] + '", "'  + file.stem.split('_')[1] + '", ' + 
    # 				"&g_font_" + file.stem + '},\n')
        
    # 	f.write('\n	{NULL, NULL, NULL}\n')
    # 	f.write('};\n\n')

    # 	f.write('const static TS_LANG_FONT_DATA g_lang_font_data[] = {\n'\
    # 		+ '	{E_LANG_SIMPLIFIED_CHINESE, g_font_data_simplified_chinese},\n'\
    # 		+ '};\n')

    # delete old font files
    # old_file_path = Path.cwd().parent / ('source\\kernel\\resource\\font\\')
    # for f in old_file_path.glob('*.c'):
    # 	f.unlink()

    # for f in Path.cwd().glob('*.c'):
    # 	file_out = f.parent.parent / ('source\\kernel\\resource\\font\\' + f.name)
    # 	if file_out.exists():
    # 		file_out.unlink()
    # 	f.rename(file_out)

    # file_out = Path.cwd().parent / ('source\\kernel\\resource\\generated_font_data.h')
    # file_in = Path.cwd() / 'generated_font_data.h'

    # if file_in.exists():
    # 	if file_out.exists():
    # 		file_out.unlink()

    # 	file_in.rename(file_out)
