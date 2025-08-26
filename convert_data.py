Import("env")

print("Current CLI targets", COMMAND_LINE_TARGETS)
print("Current Build targets", BUILD_TARGETS)

def convert_data(source, target, env):
    env.Execute(f"python file_to_header.py $PROJECT_DIR/page.html $PROJECT_DIR/lib/html html true")
    env.Execute(f"python file_to_header.py $PROJECT_DIR/favicon.ico $PROJECT_DIR/lib/favicon favicon false")


# env.AddPreAction("buildprog", convert_data)
convert_data(None, None, env)
