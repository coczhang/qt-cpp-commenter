#!/usr/bin/env python3
"""Simple local checker for the qt-cpp-commenter skill package."""

from __future__ import annotations

from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
SKILL = ROOT / "SKILL.md"


def fail(message: str) -> None:
    print(f"[FAIL] {message}")
    sys.exit(1)


def main() -> None:
    if not SKILL.exists():
        fail("SKILL.md not found")

    text = SKILL.read_text(encoding="utf-8")

    if not text.startswith("---\n"):
        fail("SKILL.md must start with YAML front matter")

    match = re.match(r"---\n(.*?)\n---\n", text, re.S)
    if not match:
        fail("YAML front matter block is incomplete")

    front_matter = match.group(1)
    required = ["name:", "description:"]
    for item in required:
        if item not in front_matter:
            fail(f"missing required metadata: {item}")

    if "name: qt-cpp-commenter" not in front_matter:
        fail("skill name should be qt-cpp-commenter")

    if len(text) < 1500:
        fail("SKILL.md seems too short to be useful")

    print("[OK] qt-cpp-commenter skill package looks valid")


if __name__ == "__main__":
    main()
