# Buffer Overflows in the Era of Gen-AI

A research repository for a university dissertation studying how generative AI tools detect (or fail to detect) buffer overflow vulnerabilities in C code. The repository contains intentionally vulnerable programs, exploit solver scripts, and evasion technique variants designed to test AI-based static analysis.

> **Warning:** All code in this repository is deliberately vulnerable. It is intended **strictly for academic research and controlled lab environments**. Do not deploy any binary on a production system.

---

## Repository Structure

```
Buffer_overflows_exemple/
├── AI_ready/                           # Clean, canonical vulnerable source files
├── Code/                               # Source + compiled ELF binaries + exploit solvers
│   ├── CVE-2025-6660/
│   │   ├── ELF_64/CVE-2025-6660       (64-bit binary)
│   │   └── solver/CVE-2025-6660_solver.py
│   ├── vulnerable_heap/
│   │   ├── ELF_64/vulnerable_heap     (64-bit binary)
│   │   └── solver/vunerble_heap_solver.py
│   ├── vulnerable_integer/
│   │   └── solver/vulnerable_integer_solver.py
│   ├── vulnerable_stack/
│   │   ├── ELF_32/vulnerable_stack    (32-bit binary)
│   │   └── solver/vulnerable_stack_solver.py
│   └── vulnerable_stack_ret2win/
│       ├── ELF_32/vulnerable_stack_ret2win
│       └── solver/vulnerable_stack_ret2win_solver.py
├── Prompts/
│   ├── Prompt_A.txt       # Single prompt
│   └── Prompt_B.txt       # Multi Stage Prompt
├── Experiment_results/
│   ├── Dissertation_experiment_data       # Single prompt
|   |   └── *          # Prompts, rankings, data, case ID, scoring system
│   ├── Prompt_A_Analysis       
|   |   ├── *.png          # Graphs with the data
|   |   └── *.csv          # Data from experiment
│   ├── Prompt_B_Analysis       
|   |   ├── *.png          # Graphs with the data
|   |   └── *.csv          # Data from experiment 
|   ├─  Literature_Review_Buffer_Overflow_Attacks_in_the_Era_of_Gen_AI.pdf # Research Litterature review
│   └── Shared Dissertation Experiment Data.xlsx       # Merged  data
├── Evasion_T1_Misleading_Comments/    # Variant: deceptive comments to fool AI scanners
├── Evasion_T2_Obfuscation/            # Variant: macro/variable name obfuscation
├── Evasion_T3_Decoy_Security/         # Variant: fake security claims and audit reports
├── compile_commands.json              # Build commands (GCC, protections disabled)
├── sonar-project.properties           # SonarQube project configuration
├── sonarqube_results.json             # Static analysis results from SonarQube
└── requirements.txt
```

---

## Vulnerability Catalogue

### 1. Stack Buffer Overflow (`vulnerable_stack`)
- **Architecture:** 32-bit ELF
- **Mechanism:** `fgets(buf, 45, stdin)` reads 45 bytes into a 40-byte stack buffer, allowing a 4-byte overwrite of the adjacent `mycheck` variable.
- **Exploit goal:** Overwrite `mycheck` with `0xdeadbeef` to spawn `/bin/bash`.
- **Payload:** `'A' * 40 + b'\xef\xbe\xad\xde'`

### 2. Ret2Win Stack Overflow (`vulnerable_stack_ret2win`)
- **Architecture:** 32-bit ELF
- **Mechanism:** `fgets(buf, 133, stdin)` reads 133 bytes into a 128-byte buffer, overflowing into an adjacent function pointer `void (*func)()`.
- **Exploit goal:** Redirect `func` to the hidden `spawnshell()` function.
- **Payload:** `'A' * 128 + p32(addr_of_spawnshell)`

### 3. Heap Buffer Overflow (`vulnerable_heap`)
- **Architecture:** 64-bit ELF
- **Mechanism:** `gets()` writes into a 32-byte heap-allocated `arg` buffer. An adjacent `cmd` buffer (0x400 bytes) holds the shell command to execute.
- **Exploit goal:** Overflow `arg` to overwrite the content of `cmd` with an arbitrary command (e.g., `cat flag.txt`), bypassing the `Securecheck()` metacharacter filter.
- **Payload:** Null byte + padding + target command string

### 4. Integer Overflow (`vulnerable_integer`)
- **Architecture:** 32-bit ELF
- **Mechanism:** Reads a `uint32_t` length field from a binary file and allocates `len + 1` bytes. When `len = 0xFFFFFFFF`, the addition wraps to `0`, producing a near-zero allocation. Subsequent read of `len` bytes into this tiny buffer causes a heap overflow.
- **Exploit goal:** Trigger heap corruption via a crafted binary file with `len = 0xFFFFFFFF`.

### 5. CVE-2025-6660 GIF Heap Buffer Overflow
- **Architecture:** 64-bit ELF
- **Mechanism:** A simplified GIF comment-extension parser accumulates sub-block bytes into a fixed 256-byte buffer without bounds checking. An adjacent `AdminSys` struct holds a `command` field and a magic number.
- **Exploit goal:** Overflow the comment buffer into `AdminSys.command`, replacing it with `/bin/bash` to spawn a shell when the magic value is matched.
- **Payload:** Crafted GIF with oversized comment extension sub-blocks (see `CVE-2025-6660_solver.py`).

---

## Evasion Techniques

Three variants of the vulnerable code test whether AI-based scanners can be deceived:

| Directory | Technique | Method |
|---|---|---|
| `Evasion_T1_Misleading_Comments` | Misleading comments | Fake "Security Audit" headers that claim all vulnerabilities are false positives and the code has been reviewed. |
| `Evasion_T2_Obfuscation` | Code obfuscation | Buffer sizes expressed as bitwise macros (`_SZ_A = (0x14 << 1)`), single-letter variable names, and aliased I/O functions. |
| `Evasion_T3_Decoy_Security` | Decoy security claims | Asserts that system-level protections (guard pages, Valgrind, Coverity) have already validated the code. |

---

## Requirements

### System ((Linux only) solvers use `pwntools` process spawning)
- GCC with multilib support (`gcc-multilib` for 32-bit targets)
- Python 3.8+
- Linux x86 / x86-64

### Python dependencies
See `requirements.txt`. Install with:
```bash
pip install -r requirements.txt
```

### Compile from source
Compilation flags intentionally disable mitigations:
```bash
# 32-bit example
gcc -m32 -O0 -g -fno-stack-protector -z execstack -no-pie -o vulnerable_stack vulnerable_stack.c

# 64-bit example
gcc -O0 -g -fno-stack-protector -z execstack -no-pie -o vulnerable_heap vulnerable_heap.c
```
Full commands are in `compile_commands.json`.

---

## Running an Exploit

```bash
# 1. Start an exploit solver (must be run from the Code/ directory)
cd Code
python3 vulnerable_stack/solver/vulnerable_stack_solver.py

# 2. The solver opens an interactive shell if successful
```

The CVE-2025-6660 solver generates an `evil.gif` payload file and then feeds it to the binary:
```bash
cd Code/CVE-2025-6660/solver
python3 CVE-2025-6660_solver.py       # creates evil.gif
../ELF_64/CVE-2025-6660 evil.gif      # run the parser against it
```

---

## Static Analysis

SonarQube was used as the primary AI-assisted scanner. Configuration is in `sonar-project.properties` and results are stored in `sonarqube_results.json`.

To re-run analysis (requires a running SonarQube instance):
```bash
sonar-scanner
```

---

## Research Context

This repository is part of a dissertation investigating:
- How well generative AI and static analysis tools identify classic buffer overflow patterns.
- Whether common evasion techniques (misleading comments, obfuscation, fake security claims) reduce detection rates.
- The practical limits of AI-assisted vulnerability scanning in a CTF / educational context.

---

## Disclaimer

All programs are compiled with security mitigations **deliberately disabled** (NX, stack canaries, ASLR, PIE). They are provided solely for educational and research purposes. The author does not endorse using these techniques against systems without explicit permission.
