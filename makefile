build: 
	cc ptopk.c -o test.o -lm -l pthread
	cc single_threaded_read.c -o sr.o -l pthread
	cc multi_threaded_read.c -o mr.o -l pthread
	cc single_threaded_read_and_parse.c -o srp.o -l pthread
	cc multi_threaded_read_and_parse.c -o mrp.o -l pthread
	cc multi_threaded_segment_read_and_parse.c -o msrp.o -l pthread
test:
	echo "here are your program's output"
	
	./test.o case1/ 1645491600 5
	./test.o case2/ 1645491600 5
	./test.o case3/ 1645491600 5
	./test.o case4/ 1645491600 5
	./test.o case5/ 1645491600 5

	echo "here are the reference output"

	./test.example case1/ 1645491600 5
	./test.example case2/ 1645491600 5
	./test.example case3/ 1645491600 5
	./test.example case4/ 1645491600 5
	./test.example case5/ 1645491600 5

run:
	./test.o case1/ 1645491600 5
	./test.o case2/ 1645491600 5
	./test.o case3/ 1645491600 5
	./test.o case4/ 1645491600 5
	./test.o case5/ 1645491600 5

clean :
	-rm -f *.out *.o
	-rm myoutput referenceoutput

getdata:
	unzip -j /public/cs3103/project/project.zip "project_log_analyze/case1/*" -d "case1/"
	unzip -j /public/cs3103/project/project.zip "project_log_analyze/case2/*" -d "case2/"
	unzip -j /public/cs3103/project/project.zip "project_log_analyze/case3/*" -d "case3/"
	unzip -j /public/cs3103/project/project.zip "project_log_analyze/case4/*" -d "case4/"
	unzip -j /public/cs3103/project/project.zip "project_log_analyze/case5/*" -d "case5/"

testdiff:
	@./test.o case1/ 1645491600 5 >> myoutput
	@./test.o case2/ 1645491600 5 >> myoutput
	@./test.o case3/ 1645491600 5 >> myoutput
	@./test.o case4/ 1645491600 5 >> myoutput
	@./test.o case5/ 1645491600 5 >> myoutput

	@./test.example case1/ 1645491600 5 >> referenceoutput
	@./test.example case2/ 1645491600 5 >> referenceoutput
	@./test.example case3/ 1645491600 5 >> referenceoutput
	@./test.example case4/ 1645491600 5 >> referenceoutput
	@./test.example case5/ 1645491600 5 >> referenceoutput

	diff myoutput referenceoutput
