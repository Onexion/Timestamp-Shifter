# ⏳ Timestamp-Shifter — Simple File Timestamp Editor

**Timestamp-Shifter** is a lightweight Windows command-line tool that lets you change the date and time of any file (`Created`, `Last Accessed`, and `Last Modified`) in seconds.

---

## 🤔 What can you do with it?

* **Fix Broken Dates:** Reset file dates that got messed up during copy-pasting or moving between drives.
* **Simulate Time for Testing:** Easily test how your own code or backup scripts react to older or newer files.
* **Privacy:** Clear or modify file history timestamps before sharing documents with others.

---

## 🚀 How to use it

Using Chronos is extremely straightforward:

### Step 1: Run the Tool & Select a File
1. Start `Main.exe`.
2. **Drag & Drop** any file from your Windows Explorer directly into the console window (or type the path manually) and press `Enter`.

### Step 2: Choose what to change
You will see the current file dates and a simple menu:
* `[1]` Change Creation Date
* `[2]` Change Last Access Date
* `[3]` Change Last Modify Date
* `[4]` Change ALL of them at once
* `[0]` Exit the program

### Step 3: Enter the new time
Type your choice (e.g., `1`) and press `Enter`. Now you have two options:

* **Option A (Set a custom date):** Type the new date in this exact format: `DD.MM.YYYY HH:MM:SS` (Example: `24.12.2026 18:00:00`) and press `Enter`.
* **Option B (Set to NOW):** Just press **`ENTER`** without typing anything to instantly apply the current system time.

*The tool will instantly update the file, refresh the screen, and show you the new timestamps!*
