#!/usr/bin/perl

my $USAGE = "

$0 startDate endDate symbol

Retrieves quote data for specified symbol over given date range. Dates
must be in format YYYY-MM-DD.

";


die $USAGE unless (@ARGV == 3);

my ($startDate, $endDate, $tickerSymbol) = @ARGV;

use StockData::Quote;

my $quoteData = new StockData::Quote;

my @data = $quoteData->getData($startDate, $endDate, $tickerSymbol);

foreach (@data)
{
    print STDOUT "$_\n";
}

sleep rand(3);
