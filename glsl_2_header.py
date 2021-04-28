import os
from os import path

glsl_extension = [".glsl", ".vs", ".fs"]


def create_header(full_path: str):
  with open(full_path+".h", 'w') as header:
    glsl = open(full_path)
    basename_without_ext, _ = os.path.splitext(os.path.basename(full_path))
    header.write("// Don't edit this file, generated by glsl_2_header.py\n\n")
    header.write("#pragma once\n\n")
    header.write("namespace glsl {\n\n")

    header.write(
        f'[[maybe_unused]] constexpr auto {basename_without_ext} = R"(\n')
    for line in glsl.readlines():
      header.write(line)
    header.write(')";\n')
    header.write("}  // namespace glsl")


def main():
  for root, dirs, paths in os.walk(os.getcwd()):
    if "dependency" in root:
      continue
    for path in paths:
      file_name, ext = os.path.splitext(path)
      if ext in glsl_extension:
        create_header(os.path.join(root, path))
        # print(os.path.join(root, path))


if __name__ == "__main__":
  main()