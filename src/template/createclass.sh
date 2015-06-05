#!/bin/sh

if [ "x$1" = "x" ] ; then
    echo ""
    echo "$0 ClassName"
    echo ""
    echo "Creates class and unit test files in current working directory"
    echo "for the specified class name."
    echo ""
    exit 1
fi

className=$1

header=${className}.h
source=${className}.cpp
testHeader=Test${className}.h
testSource=Test${className}.cpp
testMain=TestMain.cpp
makefile=Makefile

if [ -e $header -o -e $source -o -e $testHeader -o -e $testSource ] ; then
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
    ${templateDir}/Template.h > $header

echo "Creating $source..."
sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/Template.cpp > $source

echo "Creating $testHeader..."
sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/TestTemplate.h > $testHeader

echo "Creating $testSource..."
sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/TestTemplate.cpp > $testSource

if [ ! -e $testMain ] ; then
    echo "Creating $testMain..."
    sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/$testMain > $testMain
else
    echo "Remember to add '#include \"$testHeader\"' to $testMain!"
fi

if [ ! -e $makefile ] ; then
    echo "Creating $makefile..."
    sed -e "s/DIRNAME/${dirName}/g" -e "s/CLASSNAME/${className}/g" \
    ${templateDir}/$makefile > $makefile
else
    echo "Remember to add $source and $testSource to the Makefile!"
fi
