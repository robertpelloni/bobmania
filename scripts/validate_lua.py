#!/usr/bin/env python3
"""
Validates basic syntax and structure of Lua files in Themes/default/.
"""
import sys

def validate_lua_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # Basic parenthesis / brace / bracket balance check
    stack = []
    pairs = {')': '(', '}': '{', ']': '['}
    in_string = False
    quote_char = None
    escaped = False

    for line_no, line in enumerate(content.splitlines(), start=1):
        i = 0
        while i < len(line):
            ch = line[i]
            if escaped:
                escaped = False
                i += 1
                continue
            if ch == '\\':
                escaped = True
                i += 1
                continue
            if in_string:
                if ch == quote_char:
                    in_string = False
                i += 1
                continue
            if ch in ('"', "'"):
                in_string = True
                quote_char = ch
                i += 1
                continue
            if ch == '-' and i + 1 < len(line) and line[i+1] == '-':
                # Comment line to end
                break
            if ch in '({[':
                stack.append((ch, line_no))
            elif ch in ')}]':
                if not stack:
                    print(f"Error in {filepath}:{line_no}: unmatched '{ch}'")
                    return False
                top, top_line = stack.pop()
                if top != pairs[ch]:
                    print(f"Error in {filepath}:{line_no}: mismatched '{ch}', expected closing for '{top}' from line {top_line}")
                    return False
            i += 1

    if stack:
        top, top_line = stack[-1]
        print(f"Error in {filepath}: unclosed '{top}' from line {top_line}")
        return False

    return True

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: validate_lua.py <file1.lua> ...")
        sys.exit(1)
    for arg in sys.argv[1:]:
        if not validate_lua_file(arg):
            sys.exit(1)
    print("Lua validation successful.")
