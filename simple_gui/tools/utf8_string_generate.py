
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
