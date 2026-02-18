Arguments

edit 21 -t "Buy milk"


To integrate the file I/O with the CRUD functions, you typically follow a Load-Process-Save pattern. In professional CLI applications, you load the file into memory at the start, perform your operations in the array, and then overwrite the file with the updated array before exiting.


To make this a "File-First" system, each handler will follow the Read-Modify-Write cycle. We will use the #define for the hardcoded filename to keep the function signatures clean as you requested.