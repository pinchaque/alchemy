
package StockData::NetData;

use strict;

my $DEBUG = 0;

my @DAYS_BEFORE;

my $predictDays;
my $historyDays;

sub new 
{
    shift;
    my $self = {};
    bless $self;
    $self->init(@_);
    return $self;
}


sub init
{
    my $self = shift;
    $predictDays = shift;
    $historyDays = shift;

    @DAYS_BEFORE = ();
    push @DAYS_BEFORE, 0; # Jan
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 31); # Feb
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 28); # Mar
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 31); # Apr
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 30); # May
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 31); # Jun
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 30); # Jul
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 31); # Aug
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 31); # Sep
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 30); # Oct
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 31); # Nov
    push @DAYS_BEFORE, ($DAYS_BEFORE[$#DAYS_BEFORE] + 30); # Dec
}


sub processRawData
{
    my $self = shift;
    my @rawData = @_;

    my @allData = ();

    my $line;
    foreach $line (@rawData)
    {
        chomp $line;
        
        my @fields = split /,/, $line;
        
        next unless (@fields == 6);
        
        # add the number of day within this year
        push @fields, dayInYear($fields[0]);
        
        my %namedFields = ();
        
        $namedFields{"date"} = $fields[0];
        $namedFields{"open"} = $fields[1];
        $namedFields{"high"} = $fields[2];
        $namedFields{"low"} = $fields[3];
        $namedFields{"close"} = $fields[4];
        $namedFields{"volume"} = $fields[5];
        $namedFields{"dayinyear"} = $fields[6];
        
        push @allData, \%namedFields;
    }

    return sort {
        my %a = %{$a};
        my %b = %{$b};
        return ($a{'date'} cmp $b{'date'});
    } @allData;
}


sub dayInYear
{
    my ($date) = shift;
    if ($date !~ m/(\d+)-(\d+)-(\d+)/)
    {
        return 0;
    }

    my $year = $1;
    my $mon = $2;
    my $day = $3;

    my $numDays = $day + $DAYS_BEFORE[$mon - 1];

    # leap year
    if (($mon > 1)
        and !($year % 4)
        and (($year % 100) or !($year % 400)))
    {
        $numDays++;
    }

    return $numDays;
}


sub average
{
    my $total = 0.00;
    my $n = 0;

    my $i;
    foreach $i (@_)
    {
        $total += $i;
        ++$n;
    }
    if ($n)
    {
        return ($total / $n);
    }
    else
    {
        return 0.00;
    }
}


sub generateNetData
{
    my ($self, $allDataRef) = @_;

    my @allData = @{$allDataRef};

    # total number of days we need from allData for each data point we
    # generate
    my $daySpan = $predictDays + $historyDays;

    # total number of days for which we have data
    my $totDays = @allData;

    my $i;
    my @ret = ();
    for ($i = 0; ($i + $daySpan) <= $totDays; ++$i)
    {
        my $end = $i + $historyDays - 1;
        my @historyPoints = @allData[$i..$end];
        my $futurePoint = $allData[$end + $predictDays];

        push @ret, calculateDataPoint(\@historyPoints, $futurePoint);
    }
    return @ret;
}


sub getField
{
    my @historyPoints = @{$_[0]};
    my $field = $_[1];

    my @ret = ();
    foreach (@historyPoints)
    {
        my %dataPoint = %{$_};
        push @ret, $dataPoint{$field};
    }
    return @ret;
}


sub calculateDataPoint
{
    my @historyPoints = @{$_[0]};
    my $futurePoint = $_[1];

    my %endDataPoint = %{$historyPoints[$#historyPoints]};
    my %outputDataPoint = %{$futurePoint};

    my @closeData = getField(\@historyPoints, "close");
    my @volumeData = getField(\@historyPoints, "volume");

    my ($minClose, $maxClose) = minMax(@closeData,
                                       $outputDataPoint{'close'});
    my $avgClose = average(@closeData, $outputDataPoint{'close'});
    my ($minVolume, $maxVolume) = minMax(@volumeData,
                                         $outputDataPoint{'volume'});
    my $avgVolume = average(@volumeData, $outputDataPoint{'volume'});

    my @ret = ();
    my $point;
    foreach $point (@historyPoints)
    {
        my %dataPoint = %{$point};

#        push @ret, (($dataPoint{'close'} / $avgClose) - 1.0);
#        push @ret, (($dataPoint{'volume'} / $avgVolume) - 1.0);

        push @ret, normalize($minClose, $maxClose, $dataPoint{'close'});
        push @ret, normalize($minVolume, $maxVolume, $dataPoint{'volume'});
    }

    push @ret, normalize(0, 365.2425, $endDataPoint{'dayinyear'});

#    push @ret, (($outputDataPoint{'close'} / $avgClose) - 1.0);
    push @ret, normalize($minClose, $maxClose, $outputDataPoint{'close'});

    return \@ret;
}

sub normalize
{
    my ($min, $max, $value) = @_;
    
    my $range = ($max - $min);

    my $inputRange = 2.00;
    my $inputMin = -1.00;

    return (($inputRange * (($value - $min) / $range)) + $inputMin);
}

sub unnormalize
{
    my ($min, $max, $value) = @_;
    
    my $range = ($max - $min);

    my $inputRange = 2.00;
    my $inputMin = -1.00;

    return (($range * (($value - $inputMin) / $inputRange)) + $min);
}

sub minMax
{
    my $min = 99999999999;
    my $max = 0;

    foreach (@_)
    {
        if ($min > $_)
        {
            $min = $_;
        }
        
        if ($max < $_)
        {
            $max = $_;
        }
    }

    return ($min, $max);
}

sub priceConvert
{
    my ($self, $allDataRef, $outRef) = @_;

    my @allData = @{$allDataRef};

    my @output = @{$outRef};


    # total number of days we need from allData for each data point we
    # generate
    my $daySpan = $predictDays + $historyDays;

    # total number of days for which we have data
    my $totDays = @allData;

    my $i;
    my @ret = ();
    for ($i = 0; ($i + $daySpan) <= $totDays; ++$i)
    {
        my $end = $i + $historyDays - 1;
        my @historyPoints = @allData[$i..$end];
        my $futurePoint = $allData[$end + $predictDays];

        push @ret, priceConvertDataPoint(\@historyPoints,
                                         $futurePoint,
                                         $output[$i]);
    }
    return @ret;
}


sub priceConvertDataPoint
{
    my @historyPoints = @{$_[0]};
    my $futurePoint = $_[1];
    my $output = $_[2];

    my %outputDataPoint = %{$futurePoint};

    my @closeData = getField(\@historyPoints, "close");

    my ($minClose, $maxClose) = minMax(@closeData,
                                       $outputDataPoint{'close'});

#    my $avgClose = average(@closeData, $outputDataPoint{'close'});
#    return (($output + 1.0) * $avgClose);

    return unnormalize($minClose, $maxClose, $output);
}

1;

