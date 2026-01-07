# `man` Hunt

This set of exercises is intended to familiarize you with `man`, which is the
gateway to the official documentation of programs, system calls, library calls,
and operating system features for your system.

While there are man pages on the Web, those pages might or might not correspond
to the program/system/library _version_ installed on your system.  `man`
provides the most accurate documentation for the system on which you are
running.


## Setup

Using the guidance from the
[remote access information page](../REMOTE_ACCESS.md), log in to a CS lab
machine.

NOTE: Throughout this exercise, you _must_ run the `man` command on a BYU CS
lab machine, or you will get unexpected results.  This is because the man pages
installed on a system correspond to what is installed on that system  -- and
not on another system.


## Example 1

Run the following from a terminal:

```bash
man kill
```

First note the following in the upper left-hand corner: "kill(1)".  That means
that you are looking at the documentation for the program `kill`.  We know that
it is the documentation for a _program_ -- as opposed to a function or system
call -- because "(1)" refers to Section 1, which is all about programs.

Here are some of the typical contents of a man page in Section 1:

 - *NAME* - the program name and a brief description.
 - *SYNOPSIS* - a brief overview of how the program is run on the command line
   (i.e., its "usage").
 - *DESCRIPTION* - a more complete description of the functionality of the
   program.
 - *OPTIONS* - a listing and description of the options that can be used when
   running the program from the command line.
 - *EXAMPLES* - some useful usage examples.

`man` opens a "pager" (i.e., a file viewer) that displays the contents of the
manual page for the program `kill`.

 - Navigation: Use the arrow keys to move you up or down.
 - Search:
   - *Forward search:* `/`.  For example, typing "/foo" then `Enter` would find
     the _next_ instance of "foo" in the document, i.e., looking _forward_ from
     the current position.
   - *Backward search:* `?`. For example, typing "?foo" then `Enter` would find
     the _previous_ instance of "foo" in the document, i.e., looking _backward_
     from the current position.
   - *Find next or previous instance of most-recently-searched term:* `n`. If
     `/` was last used, then `n` will find the _next_ instance of the term,
     looking _forward_ from the current position; otherwise (`?` was used), `n`
     will find the _previous_ instance, looking _backward_ from the current
     position.
 - Quit: `q`

Try moving around a bit, go through the following examples while
still in the man page for `kill`:

 - Find the first instance of "-s" by typing "/-s" then `Enter`.  This will
   probably take you to the first instance of "-SIGKILL" in the text.
 - Type "n" to go to the next instance of "-s", looking forward  This one
   should correspond to the "-s" option in the "OPTIONS" section.
 - Find the next (forward) instance of "EXAMP" by typing "/EXAMP" then `Enter`.
   This will take you to the beginning of the "EXAMPLES" section.
 - Find the previous (backward) instance of "DESCRIP" by typing "?DESCRIP" then
   `Enter`.  This will take you to the beginning of the "DESCRIPTION" section.
   Read the first few sentences of the description.
 - Type "q" to exit the pager.


## Example 2

Run the following from a terminal:

```bash
man 2 kill
```

Specifying "2" on the command line means that you are looking for `kill` in
Section 2 of the man pages.  You can verify that by observing the following in
the upper left-hand corner: "kill(2)".  Section 2 contains documentation for
_system calls_, so this page contains the information for the _system call_
`kill()` -- as opposed to the _program_ `kill`.  (A system call is like a
function, but when invoked, it runs kernel code instead of user code.)
man pages corresponding to functions from installed libraries (user code) are
found in Section 3.

Here are some of the typical contents of a man page in Section 2 or Section 3.

 - *NAME* - the name and a brief description of the function or system call.
 - *SYNOPSIS* - 1) the `#include` statements needed for the function or system
   call and 2) the function definitions (i.e., arguments).
 - *DESCRIPTION* - a more complete description of the functionality the
   function or system call, including an explanation of the arguments.
 - *RETURN VALUE* - the meaning of the return value of the function or system
   call.


## Questions

Using only the `man` command, answer the following questions.  To answer each
question, you will need open specific man pages and look for specific content.

 1. What are the numbers associated with the manual sections for executable
    programs, system calls, and library calls, respectively?  Hint: See the
    DESCRIPTION section in the Section 1 (program) man page for `man`.
 2. Which file(s) should be included in a C program with `#include` to use the
    `open()` system call?
 3. What is the return value of a successful call to the `open()` system call?
 4. What is the return value of the `open()` system call when an error is
    encountered?
 5. Which file(s) should be included in a C program with `#include` to use the
    `close()` system call?
 6. What does the integer argument passed to `close()` system call represent?
    Hint: the argument name is abbreviation for these two words.
 7. Based on the "DESCRIPTION" section of the `close()` system call, is it
    possible for multiple file descriptors to refer to the same file
    description?  Hint: see the second paragraph.
 8. According to the "DESCRIPTION" section of the man page for `string`, the
    functions referred in that man page are used to perform operations on
    strings that are ________________. (fill in the blank)
 9. According to the "DESCRIPTION" section of the man page for the `strcpy()`
    function, the programmer is responsible for making sure `dst` refers to a
    buffer having an allocated size of at least _______________. (fill in the
    blank).
 10. What is the return value of `strcmp()` if the value of its first argument
     (i.e., `s1`) is _greater than_ the value of its second argument (i.e.,
     `s2`)?
