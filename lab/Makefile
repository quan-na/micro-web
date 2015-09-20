all: fork_pipe fiber_test mem_share

fork_pipe: pipe/fork_pipe.c
	gcc pipe/fork_pipe.c -o fork_pipe

fiber_test: fiber_proto/fiber_test.c
	gcc fiber_proto/fiber_test.c -o fiber_test

mem_share: memshare/proc_src.c memshare/proc_dest.c
	gcc -lrt memshare/proc_src.c -o proc_src
	gcc -lrt memshare/proc_dest.c -o proc_dest

clean:
	rm fork_pipe
	rm fiber_test
	rm proc_src proc_dest
