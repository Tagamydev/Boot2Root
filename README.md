# Boot2Root Walkthrough

## 1. Network Setup and Discovery

The analyst configured the virtual machine with a **host-only adapter** to perform internal reconnaissance. An `nmap` scan revealed the presence of a target machine running **HTTP (port 80)** and **HTTPS (port 443)** services.

* **HTTPS**: Returned an empty or default response.
* **HTTP**: Served a default placeholder page.

## 2. Directory Enumeration

Using a common wordlist, a directory fuzzing scan was performed against the HTTP service. The scan revealed the following accessible endpoints:

* `/forum`
* `/phpmyadmin`
* `/webmail`

Both `phpMyAdmin` and `webmail` were login-protected. The analyst therefore focused on exploring the forum.

## 3. Forum Analysis and Credential Discovery

Within the forum, a post by a user contained log snippets from 2015:

```
Oct 5 08:45:29 BornToSecHackMe sshd[7547]: Failed password for invalid user !q\]Ej?*5K5cy*AJ from 161.202.39.38 port 57764 ssh2
Oct 5 08:46:01 BornToSecHackMe CRON[7549]: session opened for user lmezard
```

The log suggested a failed brute-force attempt using the password `!q\]Ej?*5K5cy*AJ`, followed by a successful CRON job initiated under the `lmezard` user.

The analyst attempted to log in to the forum using the `lmezard` username and the extracted password. Authentication succeeded.

Upon logging in, user settings revealed the email address: `laurie@borntosec.net`.

## 4. Webmail Access and Credential Escalation

An attempt was made to log in to the **webmail** interface using the discovered email and the same password. This login was successful.

An email was found containing database credentials:

**Subject:** DB Access
**From:** [qudevide@mail.borntosec.net](mailto:qudevide@mail.borntosec.net)
**To:** [laurie@borntosec.net](mailto:laurie@borntosec.net)

```
Hey Laurie,

You can’t connect to the databases now. Use:
root/Fg-'kKXBj87E:aJ$

Best regards.
```

## 5. phpMyAdmin Exploitation

Using the credentials `root / Fg-'kKXBj87E:aJ$`, the analyst accessed **phpMyAdmin**.

Within the interface, multiple databases were available. Importantly, the analyst had the ability to **write files to the web root**, enabling remote code execution.

## 6. Web Shell Deployment

A PHP reverse shell was obtained from [revshells.com](https://www.revshells.com/) and converted into hexadecimal format. The payload was then written to the web server’s accessible template directory using SQL:

```sql
SET @bin = 0x[hex of php shell];
SELECT @bin INTO DUMPFILE '/var/www/forum/templates_c/cmd.php';
```

This granted remote code execution capabilities via the web interface. From here, the analyst could either issue individual commands or deploy a reverse shell for further interaction.

## 7. Privilege Escalation and Enumeration

While enumerating the system, the analyst discovered the directory:

```
/home/LOOKATME
```

Inside was a credential file containing:

```
lmezard:G!@M6f4Eatau{sF"
```

These credentials permitted local login as user **lmezard**, although SSH access was not available. Only local shell access was viable from the compromised web shell.

## 8. Binary and File Analysis

Within `lmezard`’s home directory, a file named `fun` was found. This file was transferred to a writable location, downloaded, and analyzed. It was a **tar archive** containing a series of `.pcap` files.

Each `.pcap` contained fragments of **C source code**, embedded within the payloads. A custom Python script was written to reconstruct the complete C file:

```python
import os

def get_key(string):
    tmp = string[len("//file"):]
    digits = ''
    for c in tmp:
        if not c.isdigit():
            break
        digits += c
    return int(digits) if digits else -1

def main():
    path = os.getcwd()
    files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]
    map_of_files = {}

    for file in files:
        with open(file, 'r') as f:
            lines = f.readlines()
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
```

The reconstructed source code was saved as `first.c`, compiled, and executed:

```bash
python3 ./flag02.py > first.c
gcc first.c -o test
./test
```

The output contained the following string:

```
MY PASSWORD IS: Iheartpwnage
Now SHA-256 it and submit%
```

The password was then hashed as required:

```bash
echo -n Iheartpwnage | sha256sum
# 330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4
```

## 9. Laurie Access

The hashed password, and its plaintext version, were tested against users found in `/home`. It successfully authenticated as **Laurie** via SSH.





## Thor 

### 1. Initial Enumeration

During the post-exploitation phase, the analyst navigated to a directory named `thor`. Within this folder, two files were identified:

* `README`
* `turtle`

The `README` file indicated that completing the challenge would grant access to the user **zaz**.

### 2. Turtle File Analysis

The `turtle` file appeared to contain a set of instructions resembling a command or scripting language. Upon further investigation, the instructions were recognized as commands intended for **Turtle Graphics**—a Python-based drawing library used for educational purposes.

### 3. Visualization and Message Extraction

To interpret the instructions, the analyst wrote a custom script to parse the turtle commands and generate the corresponding graphic. The output of the rendered turtle script was a drawing composed of several capital letters:

* Two **S** characters
* One **A**
* One **H**
* One **L**

Based on the drawing order and instructions, the intended sequence appeared to be:

```
SLASH
```

This string matched the context provided in the `turtle` file, which included the hint:

> *"Can you digest the message? :)"*

The hint implied that the extracted string should be **hashed**, similar to previous steps in the Boot2Root challenge. The SHA-256 hash was attepted, but it did not produce the correct result.

After testing multiple hashing algorithms, it was determined that **MD5** was the intended method. Hashing the string `SLASH` with MD5 produced the following digest:

```
646da671ca01bb5d84dbb5fb2238dc8e
```

This hash served as the **password for user zaz**, allowing access to the next stage of the machine.

### Additional Resources

* [Turtle Graphics Online Interpreter](https://trinket.io/turtle)


