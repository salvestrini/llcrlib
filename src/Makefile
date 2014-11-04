SWIG = swig
SFLAGS = -I.

all: wrap-java wrap-python

wrap-java:
	$(SWIG) $(SFLAGS) -java rib/rib.i

wrap-python:
	$(SWIG) $(SFLAGS) -python rib/rib.i

clean-java:

clean-python:

clean: clean-java clean-python
