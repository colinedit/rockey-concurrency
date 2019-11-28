Thu Nov 28 12:56:51 CET 2019

`git clone https://github.com/colinedit/rockey-concurrency.git && cd rockey-concurrency && make run`

To launch the test case, provide a dongle and its passwords via
#define R4ND_P1 and #define R4ND_P2 in test.c, then simply `make run`.

By default, the Makefile will run 16 concurrent copies of the program.
Please note, one set of executions does not always exhibit wrong behavior from
the API. You may need to try several times, although definitely fewer than ten.

You can adjust how many parallel copies will be executed using the argument
called num_processes. For instance, `make run num_processes=5` will start five
copies of the program.
