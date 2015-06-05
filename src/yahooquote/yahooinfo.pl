#!/usr/bin/perl

my $USAGE = "

$0 symbol

Retrieves additional info about a stock.

";


die $USAGE unless (@ARGV == 1);

my ($tickerSymbol) = @ARGV;

use StockData::Info;

my $info = new StockData::Info;

%data = $info->getData($tickerSymbol);
foreach my $k (keys %data)
{
  $v = $data{$k};
  print STDOUT "$k $v\n";
}

sleep rand(3);
