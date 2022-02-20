import argparse
import os
import subprocess


def should_foramt(file: str):
  if file.endswith(".cpp"):
    return True
  if file.endswith(".h"):
    return True
  return False


def main(path):
  abs_path = os.path.abspath(path)
  print(f"format {abs_path}...")

  for root, dirs, files in os.walk(path):
    for file in files:
      if should_foramt(file):
        file_path = os.path.join(root, file)
        # print(file_path)
        subprocess.run(["clang-format", "-i", "-style=file", file_path])


if __name__ == "__main__":
  parser = argparse.ArgumentParser(
      description="format all files with clang-format with style=file")
  parser.add_argument("path")
  args = parser.parse_args()
  main(args.path)