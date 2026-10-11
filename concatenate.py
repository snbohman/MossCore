import sys
from pathlib import Path


def dump_files(root: str, output: str = "all_files.txt") -> None:
    root = Path(root).resolve()
    output_path = Path(output).resolve()

    if not root.is_dir():
        sys.exit(f"Error: '{root}' is not a directory.")

    exclude = {
        ".git",
        ".venv",
        "venv",
        "__pycache__",
        "node_modules",
        "dist",
        "build",
    }

    with output_path.open("w", encoding="utf-8") as out:
        for path in sorted(root.rglob("*")):
            if not path.is_file() or path == output_path:
                continue

            relative = path.relative_to(root)

            if any(part in exclude for part in relative.parts):
                continue

            try:
                content = path.read_text(encoding="utf-8")
            except (UnicodeDecodeError, PermissionError, OSError):
                continue

            out.write(f"============ [[{relative}]]\n")
            out.write(content)
            out.write("\n\n")

    print(f"Written to {output_path}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(f"Usage: {sys.argv[0]} <directory>")

    dump_files(sys.argv[1])
