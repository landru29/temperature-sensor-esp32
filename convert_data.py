Import("env")

print("Current CLI targets", COMMAND_LINE_TARGETS)
print("Current Build targets", BUILD_TARGETS)

def convert_data(source, target, env):
    input_file = "$PROJECT_DIR/page.html"
    output_file = "$PROJECT_DIR/lib/html"
    var_name = "html"
    env.Execute(f"python file_to_header.py {input_file} {output_file} {var_name}")


# env.AddPreAction("buildprog", convert_data)
convert_data(None, None, env)
