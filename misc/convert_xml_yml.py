#
# Short script for converting GNU Radio XML blocks to YAML blocks
# without having to start GRC
#
# Please note that this program _WILL_ overwrite files.
# 
# How to use:
# 1. Save this file to grc/converter/cmdline_converter.py
# 2. Navigate back to the GNU Radio project root
# 3. Run: python3 -m grc.converter.cmdline_converter [name.xml]
# 4. Your YAML file has been created, called [name.block.yml]
# 
# You can also specify the output filename:
#   python3 -m grc.converter.cmdline_converter [input.xml] [output.block.yml]
#

import sys
from gnuradio.grc.converter import block

if not (1 < len(sys.argv) < 4):
    print("Navigate to the project root and run the following:")
    print("  python3 -m grc.converter.cmdline_converter [input.xml]")
    print("")
    exit()

if len(sys.argv) == 3:
    output = open(sys.argv[-1], "w")
    input = block.from_xml(sys.argv[-2])
else:
    output_filename = sys.argv[-1].split(".")[0] + ".block.yml"
    output = open(output_filename, "w")
    input = block.from_xml(sys.argv[-1])


block.dump(input, output)
