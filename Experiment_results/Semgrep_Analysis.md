# Semgrep Static Analysis Results

## Summary

Semgrep identified **4 distinct rule violations** across **5 source files**, producing a total of **9 findings**. However, only **one** of these findings directly corresponds to the actual heap buffer overflow vulnerability present in `vulnerable_heap.c`.

| Severity | Rule | Findings |
|----------|------|----------|
| High | OS Command Injection (`system()`) | 5 |
| Medium | OS Command Injection (untrusted input) | 2 |
| Medium | Path Traversal | 2 |
| Medium | Insecure use of `gets()` | 1 |
| **Total** | | **10** |

---

## Findings Detail

### 1. OS Command Injection — `system()` usage (High)

Semgrep flags every call to `system()` as high-severity, recommending `execve` instead.

| File | Line |
|------|------|
| `CVE-2025-6660/CVE-2025-6660.c` | 73 |
| `vulnerable_heap/vulnerable_heap.c` | 54 |
| `vulnerable_integer/vulnerable_integer.c` | 40 |
| `vulnerable_stack/vulnerable_stack.c` | 38 |
| `vulnerable_stack_ret2win/vulnerable_stack_ret2win.c` | 18 |

This is a blanket rule — it catches every `system()` call regardless of whether user input actually reaches it. In this codebase all five programs do use `system()` to spawn shells, so the finding is technically correct but lacks the nuance to distinguish between a controllable injection point (e.g. `vulnerable_heap`) and a hardcoded string (e.g. `vulnerable_stack` calling `system("/bin/bash")`).

### 2. OS Command Injection — untrusted input (Medium)

| File | Line |
|------|------|
| `vulnerable_heap/vulnerable_heap.c` | 54 |
| `vulnerable_integer/vulnerable_integer.c` | 40 |

Semgrep identifies that user-controlled data flows into `system()`. For `vulnerable_heap` this is accurate: user input from `gets()` is concatenated into `cmd` and passed to `system()`. For `vulnerable_integer` the flow is indirect (file-sourced data), but the flag is still reasonable.

### 3. Path Traversal (Medium)

| File | Line |
|------|------|
| `CVE-2025-6660/CVE-2025-6660.c` | 88 |
| `vulnerable_integer/vulnerable_integer.c` | 20 |

Both programs open files using `argv`-supplied paths without validation. This is a legitimate finding — an attacker-controlled path could reference arbitrary files — but it is a secondary concern compared to the buffer overflow vulnerabilities these programs are designed to demonstrate.

### 4. Insecure use of `gets()` (Medium)

| File | Line |
|------|------|
| `vulnerable_heap/vulnerable_heap.c` | 50 |

Semgrep correctly flags `gets()` as unsafe because it performs no bounds checking. This is the **only finding that directly relates to the heap buffer overflow vulnerability**: `gets(arg)` writes into a 32-byte heap allocation (`malloc(0x20)`) with no length limit, allowing an attacker to overflow into the adjacent `cmd` buffer and overwrite the command string passed to `system()`.

---

## Heap Buffer Overflow — The Only True Positive

Of all findings, only the `insecure-use-gets-fn` rule on `vulnerable_heap.c:50` identifies the mechanism behind the actual heap buffer overflow exploit.

**Vulnerable code path:**

```c
char *arg = malloc(0x20);       // 32-byte heap buffer
char *cmd = malloc(0x400);      // adjacent 1024-byte buffer holding "/bin/ls -l "

gets(arg);                      // no bounds check — writes past arg into cmd
Securecheck(arg);               // metacharacter filter (bypassable via overflow)
strcat(cmd, arg);               // cmd already overwritten at this point
system(cmd);                    // executes attacker-controlled command
```

**What Semgrep caught:** The use of `gets()` — a function banned since C11 — which accepts unbounded input into a fixed-size heap buffer.

**What Semgrep missed:**

- It did not flag this as a **buffer overflow**. The finding is categorised as "insecure function usage," not as a memory safety violation.
- It did not detect the **heap-adjacency exploit**: the attacker overflows `arg` to corrupt `cmd`, bypassing the `Securecheck()` filter entirely because the filter only inspects `arg` while `cmd` has already been overwritten.
- It did not recognise that `Securecheck()` provides a false sense of security — the metacharacter filter is irrelevant when the overflow writes directly into the command buffer before the check runs on the original pointer.
- It did not flag the missing bounds check on the `malloc(0x20)` allocation relative to the unbounded `gets()` call.

**Verdict:** Semgrep identifies a symptom (`gets()` is dangerous) but does not diagnose the disease (a heap buffer overflow that chains into command injection via adjacent heap chunk corruption).
