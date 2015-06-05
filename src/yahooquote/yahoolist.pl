#!/usr/bin/perl

my $USAGE = "

$0 

Lists all stock symbols from Yahoo.

";


die $USAGE unless (@ARGV == 0);

use StockData::List;

my $listData = new StockData::List;

foreach ($listData->getData())
{
    print STDOUT "$_\n";
}
