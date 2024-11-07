sum_serial:
	gcc sum_serial.c -o sum_serial
	./sum_serial 1000000

sum_multi_thread:
	gcc sum_multi_thread.c -o sum_multi_thread
	./sum_multi_thread 10 1000000

clean:
	rm -rf sum_serial
	rm -rf sum_multi_thread
