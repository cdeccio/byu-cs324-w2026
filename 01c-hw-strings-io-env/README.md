# Strings, I/O, and Environment

The purpose of this assignment is to help you better understand strings, I/O,
command-line arguments, exit status, and environment variables in C with a
series of hands-on exercises.  You will flesh out sections of an existing C
program and answer questions about its output.


## Preparation

Read the following in preparation for this assignment:

 - 10.1 - 10.4 and 10.9 in the book

Additionally, man pages for the following are also referenced throughout the
assignment:

 - `write(2)`
 - `charsets(7)`
 - `ascii(7)`
 - `printf(3)`
 - `fprintf(3)`
 - `stdin(3)`
 - `stdout(3)`
 - `stderr(3)`
 - `fileno(3)`
 - `open(2)`
 - `read(2)`
 - `close(2)`
 - `getenv(3)`


## Overview

This file contains instructions and questions divided into four parts:
character encoding, command-line arguments and exit status, I/O, and
environment variables.  The file `exercises.c` contains a function
corresponding to each part, in which you will do the specified work for each
set of questions.

Follow the instructions for and answer each question.  For most sets of
questions, you will re-compile and re-run the program using the following
commands:

```bash
gcc -Wall -Wno-unused-variable -o exercises exercises.c
./exercises test.txt
```

`gcc` is the GNU compiler collection, the C compiler that will be used.  The
`-o` option designates the name of the binary file resulting from compilation.
The combined options `-Wall -Wno-unused-variable` mean to show all compilation
warnings, _except_ warnings associated with unused variables.

Note that several exercises will have you modify the command line that you use
to get different results.

All of the exercises and questions are supposed to be taken at face value.
They might seem a little too straight-forward, but the point is to teach you
the concepts in a simple, hands-on way.


## Part 1: Characters, Encoding, and Presentation

This is very brief lesson on characters, encoding, and presentation.


### ASCII

All `char`/byte values in C are simply 8-bit integers, i.e., with values
between 0 and 255 (0 through 2<sup>8</sup> - 1).  It is the _interpretation_
and _presentation_ of those integer values according to some character set that
makes them "readable" characters.  Byte values between 0 and 127 (i.e., without
the most significant bit set) can be interpreted as part of the American
Standard Code for Information Interchange (ASCII) character set, which includes
the upper- and lower-case letters (A-Z and a-z), numbers (0-9), punctuation,
and more.  If these byte values are sent to the terminal, or any other
application that understands them to be ASCII, then it will print out the ASCII
equivalents.

For example, consider the following array of bytes:

```c
char s1a[] = { 104, 101, 108, 108, 111, 10 };
```

This is equivalent to the following, in which integer literals are specified
using hexadecimal notation instead of decimal notation:

```c
char s1b[] = { 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x0a };
```

Together, they represent the ASCII values: `'h'`, `'e'`, `'l'`, `'l'`, `'o'`,
`'\n'` (newline).

If you send either of these sets of bytes to the terminal using the `write()`
function, you will see them displayed as their ASCII equivalents.

```c
write(STDOUT_FILENO, s1, 6);
```

Compile and run `exercises.c` to see this.

More will be explained about the `write()` system call later on.  For now, just
know that `write(STDOUT_FILENO, s1, 6)` sends the six bytes at `s1` to standard
output (shown in the code as `STDOUT_FILENO`, which is defined as the integer
`1`), which refers to the terminal.


### Beyond ASCII: Unicode (Bonus)

Here is a bit of bonus information related the limitations of ASCII and how
they are addressed.

In the examples we have given, a single `char`/byte is used to represent each
ASCII character.  However, a byte can only represent up to 256 (2<sup>8</sup>)
different characters. While that is plenty to represent the Latin character
set, it is far from sufficient for covering international character sets.
Thus, character sets other than ASCII have been developed, the most widely used
of which is Unicode.  Unicode has the ambitious goal of having a mapping for
"every character in every human language" (man page for `charsets(7)`).  ASCII
is a subset of Unicode: if a byte value is 127 or less (most significant bit is
0), then it is ASCII, but if it is 128 or more (most significant bit is 1),
then it is encoded for Unicode, and further bytes are used to determine
character.  Finally, unicode is typically encoding using a byte encoding called
UTF-8.  For example, the following array of _seven_ bytes comprise the UTF-8
encoding for the _two_ simplified Chinese characters that mean "Hello",
followed by a newline (`0x0a` or '\n'):

```c
char s2[] = { 0xe4, 0xbd, 0xa0, 0xe5, 0xa5, 0xbd, 0x0a };
```

Again you can send these bytes to the terminal using the `write()` function,
and the terminal will decode them from UTF-8 and display them as their
corresponding unicode characters.

Compile and run `exercises.c`.

```c
write(STDOUT_FILENO, s2, 7);
```

Finally, take a look at the presentation of the following sequence of
UTF-8-encoded Unicode characters:

```c
char s3[] = { 0xf0, 0x9f, 0x98, 0x82, 0x0a };
write(STDOUT_FILENO, s3, 5);
```

Yup, that is a "joy" emoji.  Not exactly "human language", but you get the
idea.  If it doesn't display in the terminal, try copying it and pasting it in
another application that has the proper support to display it.

For more information, see the man pages for `charsets(7)` and `ascii(7)`.


### `printf()` and Friends

What do `printf()` and `fprintf()` do?  They are similar to `write()`, but with
some important differences:

 - First, `printf()` and `fprintf()` operate on file streams (`FILE *`), which
   include user-level buffering.  That simply means that they "save up"
   `write()` calls and send the pending bytes only when it's most efficient to
   do so.  This is like going to the grocery store only when you need a whole
   week's worth of groceries instead of going there when you just need a single
   food item.

   The difference between `printf()` and `fprintf()` is that `printf()` always
   uses `stdout` (standard output) as the file stream, but with `fprintf()` the
   file stream must be specified.  It  might be `stdout`, `stderr` (which you
   will learn about later), or a `FILE *` that refers to an open file.
 - Second, instead of explicitly setting the number of bytes to send,
   `printf()` and `fprintf()` know when to stop sending bytes when they detect
   a null byte value (integer value 0), which you will see in this assignment.
 - Third, and most importantly for now, they perform "replacements" before
   calling `write()` on the resulting string.  For example, the `s` (e.g.,
   `"%s"`) conversion specifier indicates that it should be replaced by the
   null-terminated string specified.  The `x` and `d` conversion specifiers
   indicate that they should be replaced with the hexadecimal and decimal
   representations (in ASCII characters) of an integer, respectively.

   For example, consider the following statement:

   ```c
   printf("hello %d\n", 42);
   ```

   Before the string is written to the terminal, the substring "%d" (i.e., the
   two characters `'%'` and `'d'`) is replaced with the two byte values that
   comprise the string "42": 52 and 50 (decimal), 0x34 and 0x32 (hexadecimal),
   or `'4'` and `'2'` (ASCII).  The resulting string that is printed is:
   "hello 42\n".

   You can learn more about the available conversion specifiers on the man page
   for `printf(3)`.

The following snippets all yield equivalent results:

```c
// Use an integer literal
printf("hello %d\n", 42);
```

```c
// Use an int variable
int val = 42;
printf("hello %d\n", val);
```

```c
// Specify the stdout file stream explicitly
fprintf(stdout, "hello %d\n", 42);
```

```c
// Use the write() system call directly instead of calling printf().
// write() does not have buffering like printf() does.
write(STDOUT_FILENO, "hello 42\n", 9);
```

Specifically, what is sent to the console in each case is the following
sequence of bytes/characters:

| Representation | | | | | | | | | |
| ---------------|-|-|-|-|-|-|-|-|-|
| Hexadecimal | 0x68 | 0x65 | 0x6c | 0x6c | 0x6f | 0x20 | 0x34 | 0x32 | 0x0a |
| Decimal | 104 | 101 | 108 | 108 | 111 | 32 | 52 | 50 | 10 |
| ASCII | `'h'` | `'e'` | `'l'` | `'l'` | `'o'` | `' '` | `'4'` | `'2'` | `'\n'` |

Again, see the man pages for `charsets(7)` and `ascii(7)`.


### Exercises

The function `memprint()` was written to help better visualize character
encoding. It is defined right in `exercises.c`.  It simply prints the contents
of an array of `char`, byte-by-byte, to standard output (i.e., the terminal)
using the specified format: `"%02x"` for a hexadecimal representation of the
byte;  `"%d"` for a decimal representation of the byte; and `"%c"` for the
ASCII representation of the byte.

The `sizeof()` operator is used in `exercises.c` to get the allocated size for
given type or variable.  Note that this is a compile-time option, so it can
only calculate allocations that are made at compile time, not run time.

Call `memprint()` on `s1a` three times, passing `s1a_len` as `len` each time.
The first time, show each byte/character value as hexadecimal (i.e., format
`"%02x"`).  The second time, show each byte/character value as decimal (i.e.,
format `"%d"`).  Finally, show each byte/character value as its corresponding
ASCII character (i.e., format `"%c"`).

Compile and run `exercises.c`.  Use the output to answer the following
questions.

 1. What is the integer value of the ASCII letter `'o'` (lower-case letter
    "o"), in _decimal_ notation?

 3. What is the integer value of the ASCII letter `'o'` (lower-case letter
    "o"), in _hexadecimal_ notation?

 5. What is the ASCII character associated with the (hexadecimal) value 0x6c?

 6. What is the decimal representation of the (hexadecimal) value 0x6c?

Add some C code to answer the following questions.  Then re-compile and run
`./exercises`.  See also the man page for `ascii(7)`.

 5. What is the hexadecimal value of the ASCII character `'B'`?

 6. What is the decimal value of the ASCII character `'$'`?

 7. How does the terminal render the ASCII character with the decimal value
    `7`?  Hint: make sure your volume is up!


### Summary and Main Points

 - Text is really just a bunch of integer values that an application (e.g., a
   terminal) knows how to interpret and present a certain way--i.e., as text.
 - `printf()` and friends can be used to format text for it to be presented.


## Part 2 - Command-Line Arguments and Exit Status

In this section you will learn about command-line arguments and exit status.

In C, command-line arguments are available to the `main()` function, which is
invoked when the program starts:

 - `int argc` indicates the number of command-line arguments passed; and
 - `char *argv[]` is the actual array of command-line arguments passed, each of
   type string (`char *`).

Unlike the other sections, for this section you will mostly make changes to the
`main()` function because this is where the command-line arguments are
received.

We will experiment more with strings later.  For now, let's just print out the
contents of the array.  In the `main()` function, under the "Part 2" comment,
write code for the following:

 - a `printf()` statement that prints out `argc`, the number of command-line
   arguments.
 - a `for` loop that iterates from `0` through `argc - 1`, and prints out each
   argument, one per line.

Compile and run `exercises.c`.  Make sure you are running the command as
specified above.

 8. What is the value of `argc`?

 9. What is the value of `argv[0]`?

It might surprise you to know that first (i.e., index 0) command-line argument
is always the name of the program being executed.

Now add an `if` statement that tests the following.  If there are not exactly
two command-line arguments, then do the following:

 - Use `fprintf()` to print the following statement to the terminal, specifying
   `stderr` (standard error) as the file stream: "Exactly one command-line
   option is required: filename".  Standard error will be discussed more later.
 - Call one of the following:

   - `exit(6);` or
   - `return 6;`

   Note that `return` causes a program to return from a _function call_, while
   `exit()` causes the entire _program_ to terminate, no matter which function
   it is called from.  Because `return` is called from `main()`, the two calls
   above result in the same behavior.

   The argument to `exit()` and `return` is the _exit status_ for the program.

Compile `exercises.c`.  Then run the program according to the following:

 - Run the program normally.
 - Run the following from the command line to print out the exit status from
   the command line:

   ```bash
   echo $?
   ```

 - Run the program _without_ specifying the filename argument.
 - Run the following from the command line to print out the exit status from
   the command line:

   ```bash
   echo $?
   ```

 10. What is the output of the _first_ `echo` command?

 11. What is the output of the _second_ `echo` command?

Convention is for a program that exited _without errors_ to have exit status 0
and for a program that encountered errors (e.g., wrong number of arguments) to
exit with non-zero status.  If not `exit()` is not called and `return` is not
used in the `main()` function, then the return value is 0.


### Summary and Main Points

 - The `argc` and `argv` arguments are used to capture the command-line
   arguments passed to the program at runtime.
 - Returning from `main()` and calling `exit()` are two ways to terminate a
   running program and specify an exit status.


## Part 3 - Input/Output

In this section you will perform some hands-on exercises to learn about
file descriptors and reading and writing to files and devices.  File
descriptors are the system's way of allowing a program to interact with an open
file or device.  You will start by examining standard input, standard output,
and standard error, which are the three open "files" provided to every running
program, by default.  Additionally, you will learn about user-level buffering
with higher-level constructs known as file streams (`FILE *`).

Read the entire man pages for `stdin(3)` (which also shows the information for
`stdout` and `stderr`) and `fileno(3)`.  Then use the `fileno()` and `printf()`
functions to find and print out the file descriptor values for the `stdin`,
`stdout`, and `stderr` file streams, each on its own line.

Compile and run `exercises.c`.

 12. What are the reported file descriptor values for `stdin`, `stdout`, and
    `stderr`?

Next we will experiment with strings and I/O.  Specifically, we will see how
`write()` handles output vs. how `printf()` handles output.  To prepare for
this, initialize every byte value in `buf` to `'z'` (or, equivalently, `0x7a`)
using either `memset()` or a `for` loop.  Then assign the byte at index 24 to
`'\0'` (or, equivalently, `0`).  Finally, call `memprint()` on `buf` to show
the integer value of each character, using hexadecimal representation.

At this point, you should have an array of type `char` with values that are all
`'z'`, except the null value at index 24.  Now we will see the difference
between `write()`, which sends as many characters as you specify, and
`printf()` which goes until the null character (value `0` or `'\0'`), which
designates the end of a string.

Compile and run `exercises.c`.

 13. Confirm with the `memprint()` output that the array consists of only `'z'`
    characters and a single null value.

With the array having the values that you have set, let us now see how
`printf()` treats the array differently than `write()` does.  We expect
`printf()` to treat the array as a string, recognizing the null byte.  However,
we expect `write()` to treat it as an array of bytes.  To experiment with this,
print out the contents of `buf` to standard output in two ways:

 - call `printf()` using the `"%s"` format string;
 - call `write()`, specifying the integer file descriptor value for standard
   output; in this case, use `BUFSIZE` as the byte count.

After each call, make an additional call to print a newline character (`"\n"`),
so each printout shows up on its own line.

Compile and run `exercises.c`.

 14. Is there a difference between what was printed by `printf()` and what was
     printed by `write()`?  Why or why not?  (Hint: See the `s` Conversion
     Specifier in the man page for `printf(3)`.)

Now print out the contents of `buf` to standard _error_ (not standard
_output_!) in two (new) ways:

 - call `fprintf()` (using `stderr` as the `stream`);
 - call `write()`, specifying the integer file descriptor value for standard
   error; in this case, use `BUFSIZE` as the byte count.

After each, print a newline character (`"\n"`), so each printout is on its own
line.

Now we will use the shell (the program into which we are entering the commands)
to do some output redirection.  First we will run the program without any
redirection.  Then we will redirect standard output to `/dev/null`, then
standard error to `/dev/null`, then both to `/dev/null`.  This means that
anything written to standard output--which normally would have gone to the
terminal--will now go to `/dev/null`, which is a "pseudo-file" for which
anything written to it is discarded.

Compile `exercises.c`.  Then run the program according to the following:

 - Run the program normally.
 - Run the program with "> /dev/null" (no quotes) appended to the end of the
   command line.
 - Run the program with "2> /dev/null" (no quotes) appended to the end of the
   command line.
 - Run the program with "2> /dev/null 2>&1" (no quotes) appended to the
   end of the command line.

 15. What output do you see when the program is run normally?

 16. What happens to the output when you the program is run with "> /dev/null"
     appended?

 17. What happens to the output when you the program is run with "2> /dev/null"
     appended?

 18. What happens to the output when you the program is run with
     "2> /dev/null 2>&1" appended?

Now let's be hands-on with files beyond standard input, standard output, and
standard error.  Let's start by opening a file with the `open()` system call.
Using the `open()` system call (not the higher level `fopen()`), open the file
specified by the filename variable passing `O_RDONLY` as the only flag (i.e.,
open the file for reading only).  Save the return value as an integer variable,
`fd1`.  Now copy that value to another integer variable, `fd2`.  Print out the
values of `fd1` and `fd2`, each on its own line.

Compile and run `exercises.c`.

 19. What is the significance of the value of `fd1`, i.e., the return value of
     `open()`? (Hint: See the first two paragraphs of the DESCRIPTION for
     `open(2)`.)

The `read()` system call reads _up to_ a specified number of bytes from an open
file.  The return value is used to indicate to the programmer things like
whether there was an error, fewer bytes were available to read than were
requested, or end of the file has been reached.

To experiment with this, use the `read()` system call to read up to 4 bytes
from `fd1`, placing the bytes read into `buf`. (Yes, you will be overwriting
some of the values that are currently there.)  Save the return value as
`numread`.  Add the value of `numread` to `totread`.  Then print the values of
`numread` and `totread`, each on its own line (they should currently be the
same).  Finally, call `memprint()` on `buf` using `BUFSIZE` as length, and
showing each byte/character value as hexadecimal (i.e., format `"%02x"`).

Compile and run `exercises.c`.

 20. What was the return value of `read()`?

 21. Did the return value from `read()` match the `count` value passed in?  Why
     or why not? (Hint: See the RETURN VALUE section in the man page for
     `read(2)`.)

Let's continue reading from `test.txt` using the open file descriptor.  Use the
`read()` system call to read up to 4 bytes from `fd2` (not `fd1`!).  Instead of
using `buf` as the starting point at which the read data should be copied, use
the offset into `buf` corresponding to the total bytes read.  The two ways to
do that are:

 - `buf + totread`

   (This is pointer arithmetic; adding an integer to a pointer returns a new
   pointer that is offset from the original pointer by the value of the
   integer -- in multiples of the size of the type pointed to by the integer.
   So `buf + totread` yields a pointer that refers to the `char` `totread`
   beyond the first `char` at `buf`.)

 - `&buf[totread]`

   (This notation does two things: `buf[totread]` yields the `char` at index
   `totread` of `buf`; and prefixing it with `&` yields the _address_ of that
   byte, effectively a pointer to `buf[totread]`.)

Save the return value as `numread`. Add the value of `numread` to `totread`.
Then print the values of `numread` and `totread`, each on its own line.
Finally, call `memprint()` on `buf` using `BUFSIZE` as length, and showing each
byte/character value as hexadecimal (i.e., format `"%02x"`).

Note that you now have two variables of type `int`: `fd1` and `fd2`.  `fd1` and
`fd2` exist at two different memory locations but have the same integer value.
You have used both variables, in different calls, to read from the same open
file.

Compile and run `exercises.c`.

 22. Did this new call to `read()` from `fd2` start reading from beginning of
     the file or continue where it left off after the last call?  Why? (Hint:
     See the RETURN VALUE section in the man page for `read(2)`.

 23. Based on your answer to the previous question, when reading from an open
     file using a file descriptor, does the _address_ of the variable
     referencing a file descriptor matter, or only its _value_?

 24. What was the return value of `read()`?

 25. How many total bytes have been read?

Repeat the instructions preceding question 22, but this time read up to
`BUFSIZE - totread` bytes (instead of 4).

 26. What was the return value of `read()`?

 27. Did the return value from `read()` match the `count` value passed in?  Why
     or why not? (Hint: See the RETURN VALUE section in the man page for
     `read(2)`.)

 28. How many total bytes have been read?

 29. How many total bytes are in the file? (Hint: Use the `stat` command-line
     program to see the size of the file, in bytes.)

 30. Was a null character (`'\0'` or 0) read from the file?

 31. What would happen if `BUFSIZE` had been specified, instead of
     `BUFSIZE - totread` and there were still `BUFSIZE` bytes available to
     read?

Repeat the instructions preceding question 26.

 32. What is the return value of `read()`?
     (Hint: See the RETURN VALUE section in the man page for `read(2)`.)

 33. What is the significance of the return value of `read()`?
     (Hint: See the RETURN VALUE section in the man page for `read(2)`.)

Use `printf()` to print the contents of `buf` to standard output using the
`"%s\n"` format string.

Compile and run `exercises.c`.

 34. How does the output compare to the actual contents of the file?  Briefly
     explain your response. (Hint: See questions 13, 27, and 30 for context.)

Assign the value of `buf` at index `totread` to the null character (`'\0'` or
`0`).  Then repeat the instructions preceding question 34.

 35. How does the output compare to the actual contents of the file?  Briefly
     explain your response. (Hint: See questions 13, 27, and 30 for context.)

Now that we have had opened and read from the file, let's close the file to
better understand the semantics of the `close()` system call.  Call `close()`
on `fd1`, and use `printf()` to print the return value on a line by itself.

Compile and run `exercises.c`.

 36. What is the return value of `close()`?  What does this mean? (Hint: See
     the RETURN VALUE section in the man page for `close(2)`.)

Call `close()` on `fd2` (not `fd1`!) , and use `printf()` to print the return
value on a line by itself.

Compile and run `exercises.c`.

 37. What is the return value of this second instance of `close()`?  What does
     this mean, and what is the likely cause? (Hint: See the RETURN VALUE
     section in the man page for `close(2)`. See also question 23.)

We referred to buffering earlier in this assignment.  Let's briefly return to
that by comparing buffered output with unbuffered output.  First, as mentioned
earlier, output associated with `write()` is not buffered.  File streams
(`FILE *`) are buffered, except for `stderr`.  Read the NOTES section in the
`stdin(3)` man page for more information.

Using that knowledge, add the following code to experiment with buffering.  Use
`fprintf()` to print the following, in order:

 a. `"abc"` (no newline) to standard output (`stdout`)
 b. `"def"` (no newline) to standard error (`stderr`)
 c. `"ghi\n"` to standard output (`stdout`)

Based on the explanation above, what do you expect the output to be?

Compile and run `exercises.c`.

 38. What is the order of the letters? (Exclude the newline.)

Now use `write()` to print the same three strings to the same locations, again.

Compile and run `exercises.c`.

 39. What is the order of the letters? (Exclude the newline.)

 40. What differences do you observe in the output of the strings using
     `fprintf()` vs. using `write()` and why?*  (Hint: See
     [intro](#printf-and-friends) and the "NOTES" section of the man page for
     `stdout(3)`.)

The `fflush()` function will immediately flush any buffered output of the
specified file stream.  Repeat the instructions preceding question 38.
However, this time, use the `fflush()` function to flush standard output
immediately after printing `"abc"`.

Compile and run `exercises.c`.

 41. What differences do you observe in the output of the strings using
     `fprintf()` vs. using `write()` and why?


### Summary and Main Points

 - A file descriptor is allocated for every file-like object opened during a
   program execution.  This descriptor is simply an integer that is passed to
   functions like `write()`, `read()`, and `close()`.
 - Reading fewer bytes than requested means that all available bytes have been
   read.
 - When `read()` returns 0, it is a indicator that the file associated with the
   file descriptor has been closed (i.e., end-of-file).
 - File streams offer a higher-level interface for I/O, including buffering.


## Part 4 - Getting and Setting Environment Variables

In this section, you will write code that looks for the presence of an
environment variable and then practice getting and setting it.

Read the RETURN VALUE section for the `getenv(3)` man page.  Use the return
value to write the following conditional.  Use `getenv()` to assign the pointer
`s1` to the string corresponding to the environment variable `CS324_VAR`.  If
such a value exists, then print: `"CS324_VAR is _____\n"` (replace `_____` with
the actual value); otherwise, print `"CS324_VAR not found\n"`.

Now we will run the program in three ways.  First we will run it with the
default environment (i.e., the one used by the running shell).  Then we will
add the environment variable `CS324_VAR` to the environment used to execute
`./exercises` (i.e., a one-time use).  Finally, we will add `CS324_VAR` to the
environment of the running shell, and then run `./exercises` with the modified
environment of the running shell.

Compile `exercises.c`.  Then run the program according to the following:

```bash
# Use default environment
./exercises test.txt

# Set environment var for One-time use
CS324_VAR=foo ./exercises test.txt

# Add variable to environment of running shell
export CS324_VAR=foo

# Use modified environment
./exercises test.txt
```

 42. What is the difference between running the three commands?  Briefly
     explain.

 43. (Upload your edited `exercises.c` file to LearningSuite.)


### Summary and Main Points

 - Environment variables can be set on the command line (or with `setenv()`).
 - Environment variables can be retrieved by a C program using `getenv()`.
