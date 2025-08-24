import os


def convert_file_to_header(input_file, output_file, var_name="data"):
    with open(input_file, "rb") as f:
        data = f.read()
    
    with open(output_file, "w") as f:
        f.write(f"#pragma once\n")
        f.write("#include <Arduino.h>\n\n")
        f.write(f"const unsigned char {var_name}[] PROGMEM = {{\n")
        f.write(", ".join(f"0x{b:02x}" for b in data))
        f.write("\n};\n")
        f.write(f"const unsigned int {var_name}_len = {len(data)};\n")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 4:
        print("Usage: python convert_data.py <input_file> <output_file> <var_name>")
        sys.exit(1)
    convert_file_to_header(sys.argv[1], sys.argv[2], sys.argv[3])

