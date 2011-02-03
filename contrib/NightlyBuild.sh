#!/bin/sh
#----------------------------------------------------------------------------
# NightlyBuild.sh
#
# Script for performing the TIXI nightly builds.
# Based on the TENT NightlyBuild scipt by Micheal Wins
#
# Created: 06/07/2005 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
# 
# Version: $Id: NightlyBuild.sh 526 2005-10-11 12:44:00Z tiva $
#
# Copyright (C) 2005 DLR/SC, Germany
#
# All rights reserved
#
#----------------------------------------------------------------------------


if [ $# -lt 2 ] ; then
    echo
    echo "Usage:  <BaseDirectory> <BranchTag> <LIBXML2_HOME> <CUNIT_HOME>"
    echo
    echo "Example: "
    echo "NightlyBuild.sh /work HEAD /sistec /sistec"
    echo
    exit 1
fi



echo 'TIXI NightlyBuild.sh $Revision: 526 $'
echo
echo -------------------------------------------------------------------------------
echo Starting at `date`
echo -------------------------------------------------------------------------------
echo Operating system `uname -s`
echo Release          `uname -r`
echo Hostname         `uname -n`

CVSROOT=:ext:tiva@cvs.sistec.dlr.de:/cvs/tiva
export CVSROOT
CVS_RSH=ssh
export CVS_RSH

# Mail notification: Set correct email address
ADDRESS='tiva-builds@listserv.dlr.de'

BUILD_RESULT=0

CONFIGFILE=$HOME/.tixi-buildrc.`hostname`



# Put local changes in ~/.tent-buildrc.<hostname> rather than change this one
if [ ! -f $CONFIGFILE ]; then
cat <<EOTEXT > $CONFIGFILE
    ##################################################
    # Set the appropriate path
    PATH=$PATH:/usr/local/bin:/usr/bin    
    # Mail notification: Set correct mail command
    MAILCMD=`which Mail`
    # Set script which should be executed prior the make command
    #PREMAKESCRIPT=~/bin/removedepends.sh
    PREMAKESCRIPT=
    ##################################################

EOTEXT
fi
    
# Rewrite defaults with ~/.tent-buildrc.<hostname>
if [ -f $CONFIGFILE ]; then
    echo 
    echo -------------------------------------------------------------------------------
    echo Reading local host-specific config file $CONFIGFILE...
    . $CONFIGFILE
    echo -------------------------------------------------------------------------------
fi

export PATH


BASENAME=$1
BRANCH=$2
LIBXML2_HOME=$3
export LIBXML2_HOME
CUNIT_HOME=$4
export CUNIT_HOME
SCONS=$5
shift; shift; shift; shift; shift
BUILDOPTIONS=$*

LD_LIBRARY_PATH=$LIBXML2_HOME/lib
export LD_LIBRARY_PATH

echo
echo -------------------------------------------------------------------------------
echo Basic Settings for compilation:
echo -------------------------------------------------------------------------------
echo BRANCH=$BRANCH
echo LIBXML2_HOME=$LIBXML2_HOME
echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH

BUILDDIR="$BASENAME"/NightlyBuild.$BRANCH.`uname -s`.`uname -r`.`uname -n`
TAGFILE=$HOME/.Nightlybuild.tags.`uname -n`.`uname`
LOGFILE=$BUILDDIR/tixi-build.$$.log

NRTAGS=0

# Subject line for notification Mail
subject="Nightly Build: `uname -s` `uname -n` `date +'%x %X'` ($BRANCH)"
tmpmailfile=mail.$$.tmp


echo
echo -------------------------------------------------------------------------------
echo Cleaning build directory \($BUILDDIR\) ...
echo -------------------------------------------------------------------------------
rm -rf $BUILDDIR
mkdir $BUILDDIR
cd $BUILDDIR


echo
echo -------------------------------------------------------------------------------
echo Checking out stuff from SISTEC cvs server ...
echo -------------------------------------------------------------------------------
cvs -Q co -r $BRANCH TIXI

buildstart=`date`


echo
echo -------------------------------------------------------------------------------
echo Build ...
echo -------------------------------------------------------------------------------
cd TIXI
if $SCONS UnitTests=yes $BUILDOPTIONS > $LOGFILE 2>&1
then
    BUILD_RESULT=1
    echo
    echo
    echo -------------------------------------------------------------------------------
    echo 'BUILD SUCCESSFUL'
    echo -------------------------------------------------------------------------------
    echo        
    buildresult="SUCCESSFUL"
else
    echo
    echo
    echo -------------------------------------------------------------------------------
    echo BUILD FAILED
    echo -------------------------------------------------------------------------------
    echo
fi


# create CVS tag
if [ ${BUILD_RESULT} -ne 0 ]
then
    tag=`date "+BUILD_%y_%m_%d"`_`uname`
    echo 
    echo -------------------------------------------------------------------------------
    echo "Creating tag $tag..."
    echo -------------------------------------------------------------------------------
    cvs -Q tag $tag

    echo
    echo Copying $TAGFILE to $TAGFILE.BCK for desaster recovery ...
    cp $TAGFILE $TAGFILE.BCK 

    # Store tag in $TAGFILE
    echo
    echo $tag >> $TAGFILE

    # count the tags in $TAGFILE
    for tag in `cat $TAGFILE`; do
            NRTAGS=`expr $NRTAGS + 1`
    done
    echo
    echo Number of tags found: $NRTAGS

    # remove the first tags, copy the last 10 tags to a new tags file
    tagnow=0;   
    echo
    echo Removing old tags...
    for tag in `cat $TAGFILE`; do
        tagnow=`expr $tagnow + 1`
        if [ $tagnow -lt `expr $NRTAGS - 19` ] ; then
            cvs -Q rtag -d $tag TIXI
            echo "  deleting tag $tag"
        else 
            echo "  keeping tag $tag"
            echo $tag >> $TAGFILE.tmp
        fi
    done        
    mv $TAGFILE.tmp $TAGFILE
fi



# Mail a message
echo " " > $tmpmailfile
if [ ${BUILD_RESULT} -ne 0 ]
then
    echo "BUILD SUCCESSFUL" >> $tmpmailfile
else
    echo "BUILD FAILED" >> $tmpmailfile
fi
echo " " >> $tmpmailfile

if [ ${BUILD_RESULT} -ne 0 ]
then
    echo "Starting time: ${buildstart}" >> $tmpmailfile
    echo "Finished at: `date`" >> $tmpmailfile
    cat $tmpmailfile | $MAILCMD -s "Successful $subject." $ADDRESS
else
    whoami=`whoami` 
    hostname=`hostname`
    echo "Check logfile ${hostname}:$LOGFILE " >> $tmpmailfile
    echo " " >> $tmpmailfile
    echo " " >> $tmpmailfile
    cat $tmpmailfile | $MAILCMD -s "UNSUCCESSFUL $subject!" $ADDRESS 
fi

rm $tmpmailfile


echo
echo -------------------------------------------------------------------------------
echo 'Nightlybuild exits at '`date` 
echo -------------------------------------------------------------------------------
