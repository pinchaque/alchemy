#!/bin/sh

if [ "x$1" = "x" ] ; then
    echo ""
    echo "$0 ClassName"
    echo ""
    echo "Creates application and main in current working directory"
    echo "for the specified application class name."
    echo ""
    exit 1
fi

className=$1

header=${className}.h
source=${className}.cpp
main=Main.cpp
makefile=Makefile

if [ -e $header -o -e $source -o -e $main -o -e $makefile ] ; then
    echo "Some files for $className already exist!"
    exit 1
fi

templateDir=../template

if [ ! -d $templateDir ] ; then
    echo "Couldn't find template directory $templateDir!"
    exit 1
fi

dirName=`pwd | sed 's|.*/||'`

echo "Using dirName = $dirName"


echo "Creating $header..."
sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/Application.h > $header

echo "Creating $source..."
sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/Application.cpp > $source

echo "Creating $main..."
sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/Main.cpp > $main

echo "Creating $makefile..."
sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
${templateDir}/Makefile.app > $makefile
