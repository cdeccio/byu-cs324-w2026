# BYU Bandit

The purpose of this assignment is to familiarize you with working in a shell
environment, including redirection, pipelining, backgrounding, and more.  Read
the entire assignment before beginning!


## Maintain Your Repository

 Before beginning:
 - [Mirror the class repository](../01a-hw-private-repo-mirror), if you haven't
   already.
 - [Merge upstream changes](../01a-hw-private-repo-mirror#update-your-mirrored-repository-from-the-upstream)
   into your private repository.


## Preparation

Using the guidance from the
[remote access information page](../REMOTE_ACCESS.md), log in to a CS lab
machine.

> [!IMPORTANT]
> Throughout this assignment you _must_ contact the remote bandit machine
> (`imaal.byu.edu`) from a CS lab machine.  That is, you must use the `ssh`
> command from a CS lab machine.  For security reasons, `imaal.byu.edu` is
> configured to only allow logins from CS lab machines, so if you try to
> connect from any other location, you will get unexpected results.


## Instructions

Follow these steps:

 1. Use the SSH program to log in remotely to the computer `imaal.byu.edu` with
    username `bandit0` and password `bandit0`:

    ```bash
    ssh bandit0@imaal.byu.edu
    ```

 2. Look at the contents of the `readme` file in the current directory, which
    is `bandit0`'s home directory.  Use `cat readme` to display the contents on
    the terminal.

 3. Use the hints to find the password for Level 1 (`bandit1`) using
    a series of one or more command-line tools, possibly with output
    redirection.

 4. If more than one command-line tool is used, arrange the multiple commands
    used into a single pipeline, such that the password for Level 1 is the only
    output.  Save both the password and the output.  See below for helps and
    examples.

    Note that for the command used to obtain the password for Level 9 (i.e., as
    the `bandit8` user), a pipeline does not make sense.  Just include the
    first command used.

 5. Close out your session to log out of `imaal.byu.edu` (`ctrl`+`d` or
    `exit`).

 6. Use SSH to again log in to `imaal.byu.edu`, this time with username
    `bandit1` and the password you obtained for Level 1.

 7. Repeat steps 2 through 6, for levels 1 through 10, such that you can log in
    to `imaal.byu.edu` successfully as the `bandit10` user.

 8. Create a file called `bandit.txt` with the passwords and pipelines that you
    have created, using the following format:

    ```bash
    Level 0:
    PASSWORD1
    PIPELINE1
    Level 1:
    PASSWORD2
    PIPELINE2
    ...
    ```

    Where `PASSWORD1` is the password for the `bandit1` user, and `PIPELINE1`
    is the pipeline used to obtain that password in Level 0 (i.e., as the
    `bandit0` user), etc.

    We remind you that the pipeline used to obtain the password for Level 9
    does not require any more than a single command. You do not need to include
    what you did to suspend and resume, but you must include the initial
    command.

    Note that following the format above is important, as it will allow your
    assignment to be graded automatically. Do not add extra new lines, spaces,
    or formatting.


## Automated Testing

For your convenience, a script is also provided for automated testing.  This is
not a replacement for manual testing but can be used as a sanity check.  You
can use it by simply running the following:

```bash
./SshTester.py bandit.txt
```

Please note this is the same script that the TAs use to grade.


## Helps

### Pipelines

In the simplest case, a pipeline is only a single command.  An example is the
command used to obtain the password for Level 1.  However, in more complex
examples, two or more commands are linked together using pipes, such as the
following:

```bash
grep bar somefile.txt | awk '{ print $8 }' | base64 -d
```

Note that three commands were used in the example pipeline above: `grep`,
`awk`, and `base64`.  The standard output of `grep` was connected to the
standard input of the `awk` command (via a pipe), and the standard output of
`awk` was connected to the standard input of the `base64` command (via a pipe).
There was no further command in the pipeline, so `base64`'s standard output
simply goes to the console.

You might feel overwhelmed with the "pipeline" aspect of this assignment.  To
help you out, build the pipeline gradually.  For example, in the above example,
run just the following to see what the output is:

```bash
grep bar somefile.txt
```

Then run:

```bash
grep bar somefile.txt | awk '{ print $8 }'
```

Finally, when that is working, run the whole thing:

```bash
grep bar somefile.txt | awk '{ print $8 }' | base64 -d
```


### Useful Commands

Here are some commands that you might use to help you:

 - `awk`
 - `base64`
 - `cat`
 - `curl`
 - `cut`
 - `echo`
 - `grep`
 - `gzip`
 - `head`
 - `md5sum`
 - `sha1sum`
 - `sort`
 - `tar`
 - `uniq`


### Other Helps

 - Use the man pages to learn about a command, as they are the primary
   documentation!  You can find helpful examples on the Web, but note that the
   man pages on the web might not be the same _version_ as the man pages on the
   lab machines.
 - To suspend the pipeline currently running in the foreground, use `ctrl`+`z`.
   Use `fg` to resume.  For more information, See the sections on
   `REDIRECTION`, `Pipelines` (under `SHELL GRAMMAR`), and `JOB CONTROL` in the
   `bash(1)` man page.
 - Where a pipelined command begins with a command that can receive input from
   standard input, and the initial input is a file, one way of doing it is to
   use `<` to open the file and send it to the standard input of the first
   command.  For example:
   ```bash
   cat < file.txt
   cat < file.txt | grep f
   ```
 - You can redirect standard output or standard error to `/dev/null` (or any
   file) by adding `> /dev/null` or `2> /dev/null`, respectively, to the end of
   a command.  What this says is that before the command is run, `/dev/null` is
   opened for writing and then file descriptor 1 (standard output, implied) or
   file descriptor 2 (standard error), respectively, should point to whatever
   file descriptor resulting from the newly-opened `/dev/null` file points to.
   (Also, `/dev/null` isn't actually a file but is really just a file-like
   device to "write" things that won't be kept.)  For example:
   ```bash
   echo foo > /dev/null
   echo foo 2> /dev/null
   echo foo 2> /dev/null | grep f
   ```
 - You can the duplicate standard error of a command onto its standard output
   by using `2>&1`.  What this is saying is that file descriptor 2 (standard
   error) should point to whatever file descriptor 1 (standard output) points
   to.  Something like this is useful for sending both standard output _and_
   standard error to the next command in a pipeline, rather than only the
   standard output.  With a pipelined command, redirecting standard output of
   the command to the pipe always happens before any file descriptor
   duplication.  For example:
   ```bash
   echo foo 2>&1
   echo foo 2>&1 | grep f
   ```
 - You combine redirection and duplication to send output from both standard
   output and standard error to the same file.  For example:
   ```bash
   echo foo > /dev/null 2>&1
   ```
   Note that the order here is that the standard output of `echo` is redirected
   to `/dev/null`, and then the standard error of `echo` is duplicated onto
   standard output.  If the order were reversed, you would get different results.
   Think about why that might be.
 - The `awk` command is pretty extensive and indeed includes a whole language.
   However, one of the common uses is to print one or more fields from every a
   space-delimited line of input.  For example, a simple `awk` script to print
   out just the second field of text from every line, the following command
   would work:
   ```bash
   awk '{ print $2 }'
   ```
 - `curl` is a command used to issue a request to a HyperText Transfer Protocol
   (HTTP) server, otherwise known as a Web server.  This is what your browser
   does to retrieve content. The difference is that `curl` simply prints the
   contents of what it retrieves to standard output.


## Submission

Upload `bandit.txt` to the assignment page on LearningSuite.
