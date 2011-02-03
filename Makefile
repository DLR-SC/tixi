DOXYGEN = doxygen
.PHONY: all doc clean test lib

all: doc lib test bindist_win

doc:
	$(DOXYGEN); cd Doc/latex; $(MAKE) all
html:
	cd Doc; $(DOXYGEN)
pdf:
	cd Doc; $(DOXYGEN); cd latex; $(MAKE) all
test:
	cd CUnitTests; $(MAKE) 
lib:
	cd Src; $(MAKE)
doc_clean:
	cd Doc; \rm -rf html latex

build_win:
	cd Src; $(MAKE) -f Makefile.windows all

build_lin: 
	cd Src; $(MAKE) -f Makefile.linux all

bindist_win:
	\rm -f tixi.zip
	cd Src; zip ../tixi.zip TIXI.lib tixi.h tixiData.h

demodist:
	\rm -f ibuckdemo.zip
	cd Src; zip ../ibuckdemo.zip tixiDemo.c TixiDemo.vcproj
	cd Demo/IBUCK; zip ../../ibuckdemo.zip ibuckdemo.zip myInput.xml myOutput.xml

clean:
	cd Doc; \rm -rf latex html
	cd Src; $(MAKE) -f Makefile.windows clean; $(MAKE) -f Makefile.linux clean; 
	cd CunitTests; $(MAKE) clean
