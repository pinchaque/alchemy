
# get test lists set up

opendir DIR, "cmd";
my @allTests = readdir DIR;
closedir DIR;

# filter out anything that doesn't look like a test
@allTests = sort testSorter grep {
    /^[^\.].*[^~]$/ 
    and ($_ ne "CVS")
    } @allTests;


my @quickTests = grep {
    /^\D+1$/
} @allTests;

%TEST_GROUPS = 
    (
     'all' => \@allTests,
     'quick' => \@quickTests,
     );


# Removes all filterList tests from testList, returning testList.
sub removeTests
{
    my @testList = @{$_[0]};
    my @filterList = @{$_[1]};

    my %filterTests;
    foreach (@filterList)
    {
        $filterTests{$_} = 1;
    }

    my @newTests;
    foreach (@testList)
    {
        push @newTests, $_
            unless (defined $filterTests{$_} and $filterTests{$_});
    }

    return @newTests;
}

sub resultFilter
{
    my $line = shift;
    my $test = shift;

    #YYYY-MM-DD
    $line =~ s|\d{4}-\d{2}-\d{2}|YYYY-MM-DD|g;

    # [05/08 15:10:05]
    $line =~ s|\[\d\d/\d\d \d\d:\d\d:\d\d\]|[MM/DD HH:MM:SS]|g;

    # Info: Total elapsed time: 00:00:01
    $line =~ s|(Info: Total elapsed time:) \d\d:\d\d:\d\d|$1 HH:MM:SS|g;

    return $line;
}


sub cmdFilter
{
    my $line = shift;

    return $line;
}

$CMD_FILTER = \&cmdFilter;

$RESULT_FILTER = \&resultFilter;

$DEBUG = 0;
