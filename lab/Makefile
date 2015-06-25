all: fork_pipe fiber_test

fork_pipe: pipe/fork_pipe.c
	clang pipe/fork_pipe.c -o fork_pipe

fiber_test: fiber_proto/fiber_test.c
	gcc fiber_proto/fiber_test.c -o fiber_test

clean:
	rm fork_pipe
