
package StockData::Quote;

use strict;

use HTTP::Lite;

my %MONTH_TO_NUM = (
                    "Jan" => 1,
                    "Feb" => 2,
                    "Mar" => 3,
                    "Apr" => 4,
                    "May" => 5,
                    "Jun" => 6,
                    "Jul" => 7,
                    "Aug" => 8,
                    "Sep" => 9,
                    "Oct" => 10,
                    "Nov" => 11,
                    "Dec" => 12 );
my $DEBUG = 0;

my $USEISO = 1;

sub new 
{
    my $self = {};
    bless $self;
    return $self;
}

sub getData
{
    my ($self, $startDate, $endDate, $tickerSymbol) = @_;

    my ($startYear, $startMonth, $startDay) = splitDate($startDate);
    my ($endYear, $endMonth, $endDay) = splitDate($endDate);

    --$startMonth;
    --$endMonth;

    my $doc = "http://ichart.finance.yahoo.com/table.csv"
        . "?a=$startMonth"
        . "&b=$startDay"
        . "&c=$startYear"
        . "&d=$endMonth"
        . "&e=$endDay"
        . "&f=$endYear"
        . "&s=$tickerSymbol"
        . "&ignore=.csv";

    #print "$doc\n";
    #$WARN = 0;
    my $http = new HTTP::Lite;
    my $req = $http->request($doc);
    #$WARNING = 1;

    if ($req)
    {
        my $st = $http->status();
        my $msg = $http->status_message(); 
        $msg =~ s/\r\n//g;
        
        if ($st == 200)
        {
            my $data = $http->body();
            return processData($tickerSymbol, $data);
        }
        else
        {
            die "Failed to retrieve data for $tickerSymbol: $st\n$msg\n";
        }
    }
    else
    {
        die "Request failed for $doc: $!\n";
    }
}

sub splitDate
{
    my $date = shift;

    if ($date =~ m/^(\d+)-(\d+)-(\d+)$/)
    {
        return ($1, $2, $3);
    }

    die "Invalid date specified: '$date'\n";
}

sub processData
{
    my ($tickerSymbol, $data) = @_;

    my @lines = split /\n/, $data;
    
    my @ret = ();
    foreach (@lines)
    {
        my $val = processLine($_);
        push @ret, $val if defined $val and $val ne "";
    }
    return reverse @ret;
}

sub processLine
{
    my $data = shift;

    # change the date format
    my @fields = split /,/, $data;

    if ((@fields != 7) or ($fields[0] !~ m/(\d+)-(\d+)-(\d+)/))
    {
        print STDERR "Ignoring unrecognized line '$data'\n" if $DEBUG;
        return;
    }

    my $year = $1;
    my $mon = $2;
    my $day = $3;

    if ($year < 20)
    {
        $year += 2000;
    }
    elsif ($year < 100)
    {
        $year += 1900;
    }

    if ($USEISO)
    {
        $fields[0] = sprintf "%04d%02d%02dT000000", $year, $mon, $day;
    }
    else
    {
        $fields[0] = sprintf "%04d-%02d-%02d", $year, $mon, $day;
    }

    return join ",", @fields;
}

1;
