src			  :=	test.c
name		  :=	test
num_processes :=	16

$(name): $(src)
	gcc $< libRockey4ndLib.a -lobjc -framework IOKit -framework CoreFoundation -o $(name)

all: $(name)

run: $(name)
	seq $(num_processes) | xargs -L1 -P32 ./$<

clean:
	$(RM) $(name)
