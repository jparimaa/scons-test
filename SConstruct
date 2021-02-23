import shutil

env = Environment()
debug = ARGUMENTS.get("debug", 0)
if int(debug):
    env.Append(CCFLAGS="-g -Wall -Werror -Wextra -pedantic")
else:
    env.Append(CCFLAGS="-O2")

Export("env")

SConscript("src/SConscript", variant_dir="build/", duplicate=False)
SConscript("tests/SConscript", variant_dir="build/tests/", duplicate=False)

shutil.copy("data/input.txt", "build/input.txt")
