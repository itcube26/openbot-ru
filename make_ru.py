#!/usr/bin/env python3
"""Helper: create .ru_RU.md translations and add Russian to language switchers."""
import os
import re
import sys

ROOT = "/workspace"

# base file -> translated file path (relative to repo root)
BASES = []


def find_bases():
    bases = []
    for dirpath, dirnames, filenames in os.walk(ROOT):
        if ".git" in dirpath.split(os.sep):
            continue
        for fn in filenames:
            if fn.endswith(".md") and not re.search(
                r"\.(de-DE|es-ES|fr-FR|ko-KR|zh-CN|ru-RU)\.md$", fn
            ):
                bases.append(os.path.join(dirpath, fn))
    return sorted(bases)


LANG_RE = re.compile(r"<a href=\"([A-Za-z0-9_\-\.]+)\.(zh-CN|de-DE|fr-FR|es-ES|ko-KR)\.md\">")


def add_ru_link(text, base_name):
    """Insert a Russian link into the language switcher block after the last lang link."""
    stem = os.path.splitext(base_name)[0]  # e.g. README or CONTRIBUTING
    m_last = None
    for m in LANG_RE.finditer(text):
        m_last = m
    if m_last is None:
        return text, False
    ru_href = f'{stem}.ru_RU.md'
    if ru_href in text:
        return text, False
    end = m_last.end()  # end of matched <a href="..."> part; we insert a new entry after full line item
    # find the end of that <a ...>text</a> element
    close = text.find("</a>", end)
    if close == -1:
        return text, False
    close += len("</a>")
    insert = f'\n  <a href="{ru_href}">Русский</a>'
    return text[:close] + insert + text[close:], True


def main():
    mode = sys.argv[1] if len(sys.argv) > 1 else "list"
    bases = find_bases()
    if mode == "list":
        for b in bases:
            print(os.path.relpath(b, ROOT))
        return
    if mode == "links":
        n = 0
        for b in bases:
            with open(b, encoding="utf-8") as f:
                t = f.read()
            t2, changed = add_ru_link(t, os.path.basename(b))
            if changed:
                with open(b, "w", encoding="utf-8") as f:
                    f.write(t2)
                n += 1
                print("updated links:", os.path.relpath(b, ROOT))
        print("total updated:", n)
        # also update existing translation files' switchers? Not required.
        return


if __name__ == "__main__":
    main()
