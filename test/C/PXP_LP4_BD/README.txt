
img_backdoor_2_ddr version 1.0.
Just for LPDDR5 module. not support inline ecc mode yet.

Run this app will crteate one script file "ddr_load_memory.qel", call this script if you need.
other output files need keep whith this script in same directory.

Usage: img_backdoor_2_ddr [OPTION]... [FILE]...

-h, --help              help
    --version           print current version
-e, --iecc              flag of inline ecc. default false
-l, --interleave_size   interleave size for interleave DDR space. default 256
-i, --input_file        file name of the image file. default "ddrc.bin"
-j, --json_file         json file name, configuration of address map. default "ddr_address_map.json"
-a, --address           soc address to load. default 0x3000000000

example: img_backdoor_2_ddr -l 512 -i kernel.img -a 0x3800010000

verify method: md5sum check.
