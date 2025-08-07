import os

def get_key(string):
    # Extract number after "//file"
    tmp = string[len("//file"):]
    digits = ''
    for c in tmp:
        if not c.isdigit():
            break
        digits += c
    return int(digits) if digits else -1  # Return -1 or handle as needed if no digits found

def main():
    path = os.getcwd()
    files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]
    map_of_files = {}

    for file in files:
        with open(file, 'r') as file_opened:
            lines = file_opened.readlines()
            if not lines:
                continue
            last_line = lines[-1].strip()
            key = get_key(last_line)
            map_of_files[key] = ''.join(lines) + '\n'

    buffer = ''
    for i in sorted(map_of_files):
        buffer += map_of_files[i]

    print(buffer)

if __name__ == "__main__":
    main()

