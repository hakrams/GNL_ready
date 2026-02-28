*This project has been created as part of the 42 curriculum by assempas.*

## Description

The **get_next_line** project consists of implementing a function that reads a file descriptor and returns one line at a time.  
Each call to the function returns the next line from the given file descriptor, including the terminating newline character (`\n`) if it exists.  
The function returns `NULL` when there is nothing more to read or if an error occurs.

## Instructions

The project is compiled by including all mandatory source files together with a user-defined `BUFFER_SIZE` macro.

Example compilation with a custom buffer size:

`cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c`

Example compilation with a minimal buffer size:

`cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c`

If `BUFFER_SIZE` is not defined at compilation time, the default value defined in the header file is used.


The `get_next_line` function can then be called repeatedly to read successive lines from a file descriptor until it returns `NULL`.

## Algorithm and Justification

The main challenge of the `get_next_line` function is that the `read()` system call does not return complete lines but fixed-size chunks of data. A line may therefore span multiple reads or be shorter than the defined buffer size. To handle this, the implementation uses a static memory area (stash) to store unread data between function calls.

First, the function reads data from the file descriptor into a temporary buffer of size `BUFFER_SIZE + 1`. The data read is appended to the stash, and the reading process continues until either a newline character (`\n`) is found or there is no more data to read. This ensures that the function reads only as much as necessary to construct a single line.

Once the reading phase is complete, the function extracts one line from the stash. Characters are copied from the beginning of the stash up to and including the newline character if it exists. If no newline is present, the entire remaining content of the stash is returned as the last line.

After extracting the line, the stash is updated. If there is remaining data after the extracted line, a new stash is created containing only this leftover data. If no data remains, the stash is freed and set to `NULL`. This mechanism guarantees that no data is lost between successive calls.

The implementation also validates the file descriptor and handles read errors early. In case of an invalid file descriptor, a read error, or when the end of the file is reached with no remaining data, the function returns `NULL`.

This algorithm supports lines of arbitrary length, minimizes unnecessary read operations, preserves leftover data across function calls, and follows the project requirements by returning exactly one line per call or `NULL` when appropriate.

## Edge Cases

The implementation correctly handles the following cases:

- Variable buffer sizes, including very small values such as `BUFFER_SIZE = 1` and larger values.
- Empty files, where the function immediately returns `NULL`.
- Files without a trailing newline, returning the remaining content as the last line.
- Lines shorter or longer than `BUFFER_SIZE`, by accumulating data across multiple reads when necessary.
- Early newline characters, where reading stops as soon as a newline is found.
- Invalid file descriptors or read errors, in which case the function returns `NULL`.
- Standard input, allowing lines to be read from `stdin`.

## Resources

- The Linux manual pages for `read`, `malloc`, and `free`.
- The official 42 get_next_line subject PDF.
- Online documentation and references related to file descriptors and static variables in C.

## AI Usage Disclosure

AI tools were used as a learning aid to ask conceptual questions, explore different approaches, and gain deeper explanations of the problem. All design decisions, implementation, and final code were written and understood by the author.

## Bonus

The bonus version of `get_next_line` supports reading from multiple file descriptors
simultaneously without mixing lines.

This is achieved by maintaining a separate buffer (stash) for each file descriptor using a static aray indexed by `fd`,
allowing alternating calls such as:

- `get_next_line(fd1)`
- `get_next_line(fd2)`
- `get_next_line(fd1)`
- `get_next_line(fd2)`

The bonus implementation is provided in the following files:

- `get_next_line_bonus.c`
- `get_next_line_utils_bonus.c`
- `get_next_line_bonus.h`

Compilation example for the bonus version:


`cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c`
