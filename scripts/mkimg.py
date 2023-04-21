import os
import sys

if int(sys.version_info[0]) < 3:
    raise Exception("use python3 instead")

# output
img = 'jos.img'
block_size = '1024k'
count = 128

# fill the mbr
_start_sector = 2
_end_sector = 0

if __name__ == '__main__':
    boot_path = sys.argv[1]

    # Create jos.img
    if os.path.exists(img):
        os.system('rm {}'.format(img))
    print('create jos.img...')
    cmd = 'dd if=/dev/zero of={} bs={} count={}'.format(img, block_size, count)
    if os.system(cmd+' status="none"'):
        exit(1)
    print('completed')

    with open(os.path.join(boot_path, 'mbr.bin'), 'rb+') as f:
        mbr_data = f.read()
        mbr_array = bytearray(mbr_data)
        # _start_sector: a word with the offset 8 in mbr.bin
        mbr_array[8] = 2
        mbr_array[9] = 0
        mbr_array[10] = 0
        mbr_array[11] = 0
        # _end_sector: offset 14
        mbr_array[12] = 3
        mbr_array[13] = 0
        mbr_array[14] = 0
        mbr_array[15] = 0
        f.close()
    
    tmpfile = 'tmp.bin'
    with open(tmpfile, 'wb+') as f:
        f.write(mbr_array)
        f.close()

    # write mbr
    cmd = 'dd if={} of={} bs={} count={}'.format(tmpfile, img, 512, 1)
    if os.system(cmd+' status="none"'):
        exit(1)
    print('write mbr to jos.img...completed')