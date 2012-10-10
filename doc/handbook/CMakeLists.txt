## Process this file with cmake
#====================================================================
#
#====================================================================
cmake_minimum_required(VERSION 2.8)
find_program(SPHINXBUILD sphinx-build)

# You can set these variables from the command line.
set(SPHINXOPTS)
set(PAPER a4)
set(BUILDDIR "${CMAKE_BINARY_DIR}/doc/handbook/handbook")

# Internal variables.
set(PAPEROPT_a4 -D latex_paper_size=a4)
set(PAPEROPT_letter -D latex_paper_size=letter)
set(ALLSPHINXOPTS  -d ${BUILDDIR}/doctrees ${PAPEROPT_${PAPER}} ${SPHINXOPTS} ${CMAKE_SOURCE_DIR}/doc/handbook/src)

#       @echo "Please use \`make <target>' where <target> is one of"
#       @echo "  html       to make standalone HTML files"
#       @echo "  dirhtml    to make HTML files named index.html in directories"
#       @echo "  singlehtml to make a single large HTML file"
#       @echo "  pickle     to make pickle files"
#       @echo "  json       to make JSON files"
#       @echo "  htmlhelp   to make HTML files and a HTML help project"
#       @echo "  qthelp     to make HTML files and a qthelp project"
#       @echo "  devhelp    to make HTML files and a Devhelp project"
#       @echo "  epub       to make an epub"
#       @echo "  latex      to make LaTeX files, you can set PAPER=a4 or PAPER=letter"
#       @echo "  latexpdf   to make LaTeX files and run them through pdflatex"
#       @echo "  text       to make text files"
#       @echo "  man        to make manual pages"
#       @echo "  changes    to make an overview of all changed/added/deprecated items"
#       @echo "  linkcheck  to check all external links for integrity"
#       @echo "  doctest    to run all doctests embedded in the documentation (if enabled)"

set(linkcheck_comment "Link check complete; look for any errors in the above output or in ${BUILDDIR}/linkcheck/output.txt.")
set(changes_comment "The overview file is in ${BUILDDIR}/changes.")
#set(doctest_comment "Testing of doctests in the sources finished, look at the results in ${BUILDDIRR}/doctest/output.txt.")
set(htmlhelp_comment "Build finished; now you can run HTML Help Workshop with the .hhp project file in $(BUILDDIR)/htmlhelp.")
set(qthelp_comment "
Build finished; now you can run \"qcollectiongenerator\" with the .qhcp project file in ${BUILDDIR}/qthelp, like this:
# qcollectiongenerator ${BUILDDIR}/qthelp/TIXIManual.qhcp
To view the help file:
# assistant -collectionFile ${BUILDDIR}/qthelp/TIXIManual.qhc
")
set(devhelp_comment "
Build finished.
To view the help file:
# mkdir -p $$HOME/.local/share/devhelp/TIXI
# ln -s ${BUILDDIR}/devhelp $$HOME/.local/share/devhelp/TIXI
# devhelp
")
set(latex_comment "Build finished; the TIXI files are in $(BUILDDIR)/latex. Run 'make' in that directory to run these through (pdf)latex (use 'make latexpdf' here to do that automatically).")

foreach(VAL html dirhtml singlehtml pickle json htmlhelp qthelp epub latex text changes man) #linkcheck doctest)
    add_custom_command(
        OUTPUT ${BUILDDIR}/${VAL}
        COMMAND ${SPHINXBUILD}
        ARGS -b ${VAL} ${ALLSPHINXOPTS} ${BUILDDIR}/${VAL}
        DEPENDS ${CMAKE_SOURCE_DIR}/src
        COMMENT   "${${VAL}_comment}"
    ) 
    add_custom_target(${VAL} ALL DEPENDS "${BUILDDIR}/${VAL}")
endforeach(VAL)


add_custom_target(complete COMMAND cp -f ${BUILDDIR}/latex/*.pdf ${BUILDDIR}/html DEPENDS html latexpdf COMMENT "Build finished. The HTML and PDF pages are in ${BUILDDIR}/html.")

add_custom_target(latexpdf COMMAND make -C ${BUILDDIR}/latex all-pdf DEPENDS latex COMMENT "Running latex through pdflatex")