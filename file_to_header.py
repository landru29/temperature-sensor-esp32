import os


def convert_file_to_header(input_file, output_file, var_name="data", with_zero=False):
    with open(input_file, "rb") as f:
        data = f.read()

    if with_zero:
        data = data + b"\x00"

    from pathlib import Path
    Path(output_file).mkdir(parents=True, exist_ok=True)

    canonical_path = os.path.join(output_file, os.path.basename(output_file))
    
    with_progmem = False

    max_per_line = 16
    
    with open(canonical_path+".cpp", "w") as f:
        f.write(f"#include \"{os.path.basename(output_file)}.hpp\"\n\n")
        if with_progmem:
            f.write("#include <Arduino.h>\n\n")
        f.write(f"const unsigned char {var_name}[] {'PROGMEM ' if with_progmem else ''}= {{\n")
        for i in range(0, len(data), max_per_line):
            ligne = data[i:i + max_per_line]
            f.write("    ")
            f.write(", ".join(f"0x{b:02x}" for b in ligne))
            f.write(",\n" if i + max_per_line < len(data) else "\n};\n")
        f.write(f"const unsigned int {var_name}_len = {len(data)};\n")
    
    with open(canonical_path+".hpp", "w") as f:
        f.write(f"#ifndef __{os.path.basename(output_file).upper()}_H__\n#define __{os.path.basename(output_file).upper()}_H__\n\n")
        f.write(f"extern const unsigned char {var_name}[];\n")
        f.write(f"extern const unsigned int {var_name}_len;\n")
        f.write("\n#endif\n")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 5:
        print("Usage: python convert_data.py <input_file> <output_file> <var_name>")
        sys.exit(1)
    convert_file_to_header(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]=='true')

