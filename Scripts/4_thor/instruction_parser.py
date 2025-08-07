import re

# Read your instructions from file
with open("instructions.txt", "r", encoding="utf-8") as f:
    lines = f.readlines()

# Start building the output Python code
output_lines = [
    "import turtle",
    "",
    "t = turtle.Turtle()",
    "t.shape('turtle')",
    "t.speed(0)",
    "",
]

# Translate each instruction
for line in lines:
    line = line.strip().lower()

    match = re.match(r"tourne droite de (\d+)", line)
    if match:
        deg = int(match.group(1))
        output_lines.append(f"t.right({deg})")
        continue

    match = re.match(r"tourne gauche de (\d+)", line)
    if match:
        deg = int(match.group(1))
        output_lines.append(f"t.left({deg})")
        continue

    match = re.match(r"avance (\d+)", line)
    if match:
        dist = int(match.group(1))
        output_lines.append(f"t.forward({dist})")
        continue

    match = re.match(r"recule (\d+)", line)
    if match:
        dist = int(match.group(1))
        output_lines.append(f"t.backward({dist})")
        continue

# Finalize
output_lines.append("")
output_lines.append("turtle.done()")

# Write to output file (optional)
with open("generated_turtle_script.py", "w", encoding="utf-8") as f:
    f.write("\n".join(output_lines))

# Also print the result so you can copy-paste
print("\n".join(output_lines))

