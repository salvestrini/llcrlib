SWIG = swig
SFLAGS = -I.

all: wrap-java wrap-python

wrap-java:
	$(SWIG) $(SFLAGS) -java rib/rib.i

wrap-python:
	$(SWIG) $(SFLAGS) -python rib/rib.i

clean-java:
	rm -f rib/rib.java
	rm -f rib/ribJNI.java
	rm -f rib/rib_wrap.c

clean-python:
	rm -f rib/rib.py

clean: clean-java clean-python
