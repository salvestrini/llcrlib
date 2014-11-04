SUBDIRS = examples

all:
	for i in $(SUBDIRS) ; do	\
		(cd $$i && $(MAKE) all)	\
	done

clean:
	for i in $(SUBDIRS) ; do		\
		(cd $ii && $(MAKE) clean)	\
	done
