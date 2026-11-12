## Semgrep Results

### Path Traversal

**Severity:** Medium  
**Category:** Security  
**Language:** C++

The application builds a file path from potentially untrusted data, which can lead to a path traversal vulnerability. An attacker can manipulate the path the application uses to access files.

If the application does not validate user input and sanitize file paths, sensitive files such as configuration or user data can be accessed, potentially creating or overwriting files.

**Recommendation:**  
Validate and sanitize any input used to create file paths. Enforce strict file access controls so public-facing applications can only access required files.

**Affected files:**

- `AI_ready/code5.c:88`
- `AI_ready/code5.c:20`

---

### OS Command Injection

**Severity:** Medium  
**Category:** Security  
**Language:** C++

Untrusted input might be injected into a command executed by the application, which can lead to a command injection vulnerability. An attacker can execute arbitrary commands, potentially gaining complete control of the system.

**Recommendation:**  
Avoid executing OS commands with user input. If this is unavoidable, validate and sanitize the input, and use safe methods for executing commands.

**Affected files:**

- `AI_ready/code3.c:54`
- `AI_ready/code5.c:40`

---

### insecure-use-gets-fn

**Severity:** Medium  
**Category:** Security  
**Language:** C

Avoid `gets()`. This function does not consider buffer boundaries and can lead to buffer overflows.

**Recommendation:**  
Use `fgets()` or `gets_s()` instead.

**Affected files:**

- `AI_ready/code3.c:50`

---

### OS Command Injection

**Severity:** High  
**Category:** Security  
**Language:** C++

Do not call `system`. It is a high-level wrapper that allows stacking multiple commands.

**Recommendation:**  
Prefer a more restrictive API, such as `execve` from the `exec` family.

**Affected files:**

- `AI_ready/code5.c:73`
- `AI_ready/code3.c:54`
- `AI_ready/code5.c:40`
- `AI_ready/code1.c:38`
- `AI_ready/code2.c:18`
