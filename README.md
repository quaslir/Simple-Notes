# Simple-Notes
### Overview
Simple-Notes is a lightweight reminder tool to save events or meetings. Each note has three categories:
- Message – Description of the event.
- Severity – Priority or urgency level (e.g., low, medium, high).
- Time – When the event is scheduled (auto-parsed if you input relative terms like tomorrow, today, or yesterday).
### Notes are stored in a file, which you specify when running the program. If the file doesn't exist, it will be created automatically. You can also delete or edit existing notes.
## Features:
- Auto-time parsing for common phrases like "today," "tomorrow," and "yesterday."
- Save notes to a custom file.
- Edit or delete events when needed.
## Getting Started
Clone the repository:
```bash
git clone https://github.com/quaslir/Simple-Notes/
cd Simple-Notes
```
### Compile the program:
```bash
gcc notes.c read_string.c -o program
```
### Run the program with your desired file name:
```bash
./program "name_of_your_file"
```
### Example Usage
To add an event (e.g., a meeting with colleagues at a restaurant):
```bash
./program "notes.txt"
```
In the program, input:
Meeting with my colleagues at restaurant, 3, tomorrow
This will:
```Parse the message: "Meeting with my colleagues at restaurant."```
Set the severity: 3 (could be a scale from 1 to 5 or custom).
Automatically set the time to tomorrow.
Editing or Deleting Notes
You can edit or delete events from the saved file by using the program’s interactive interface.
