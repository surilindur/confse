#!/bin/python

from os import getcwd
from pathlib import Path
from fileinput import input

SUFFIXES = set((".cpp", ".hpp", ".c", ".h"))


def change_imports(path: Path) -> None:
    try:
        lines = []
        lines_changed = 0
        with open(path, "r") as file:
            for line in file:
                if line.lstrip().startswith("#include "):
                    lines.append(line.replace("\\", "/"))
                    lines_changed += 1
                else:
                    lines.append(line)
        if lines_changed:
            with open(path, "w") as file:
                file.writelines(lines)
        else:
            print(f"Nothing changed in {path}")
    except UnicodeDecodeError:
        print(f"Invalid unicode characters in {path}")


def process_path(root: Path) -> int:
    processed_files = 0
    path_queue = [root]
    while path_queue:
        path = path_queue.pop()
        if not path.name.startswith("."):
            if path.is_file():
                if path.suffix in SUFFIXES:
                    print(f"Processing {path}")
                    change_imports(path)
                    processed_files += 1
            elif path.is_dir():
                path_queue.extend(path.iterdir())
    return processed_files


if __name__ == "__main__":
    obse_path = Path(getcwd(), "xobse").resolve()
    print(f"OBSE at {obse_path}")
    processed_files = process_path(obse_path)
    print(f"Processed {processed_files} files")
