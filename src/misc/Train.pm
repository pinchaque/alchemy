#!/usr/bin/perl -w

package Train;

$CSVFILE = "train.csv";
$LOGFILE = "train.log";
$VERBOSE = 0;


sub runCmd($)
{
    my ($cmd) = @_;

    print "$cmd" if $VERBOSE;

    if (0)
    {
        my $val = rand;
        open L, ">>$LOGFILE" or die "$LOGFILE: $!\n";
        print L "Info: Minimum testing error was at step 135: $val\n";
        close L;
    }
    else
    {
        system($cmd);
    }
}

sub getTimeStamp
{
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) =
        localtime(time);

    return sprintf("[%02d/%02d %02d:%02d:%02d]",
                   $mon, $mday, $hour, $min, $sec);
}

sub printMsg($)
{
    my $str = shift;

    my $msg = getTimeStamp() . " $str\n";

    if (open L, ">>$LOGFILE")
    {
        print L "$msg";
        close L;
    }

    print $msg;
}

sub getTrainFileName($$)
{
    my ($days, $symbol) = @_;

    return "train.$days.$symbol.dat";
}

sub getTestFileName($$)
{
    my ($days, $symbol) = @_;

    return "test.$days.$symbol.dat";
}


sub prepareData($$$)
{
    my ($days, $symbol, $sample) = @_;

    my $trainFile = getTrainFileName($days, $symbol);
    my $testFile = getTestFileName($days, $symbol);

    runCmd("rm $trainFile $testFile");

    # generate the data sets
    runCmd("alchemygendata -s $symbol --train $trainFile --test $testFile --days $days --randomize --sample $sample");
}
    

sub printHeader
{
    return if -e $CSVFILE;

    open CSV, ">$CSVFILE" or die "Unable to open $CSVFILE for writing: $!\n";
    print CSV "days,symbol,units,layers,numTrials,error\n";
    close CSV;
}


sub runTrials($$$$$)
{
    my ($days, $symbol, $units, $layers, $numTrials) = @_;

    my $trainFile = getTrainFileName($days, $symbol);
    my $testFile = getTestFileName($days, $symbol);

    printMsg("=== runTrials ===");
    printMsg("Days = $days");
    printMsg("Symbol = $symbol");
    printMsg("Units = $units");
    printMsg("Layers = $layers");
    printMsg("NumTrials = $numTrials");

    my @errorList = ();

    for ($trial = 1; $trial <= $numTrials; ++$trial)
    {
        printMsg("Running trial $trial...");

        my $profileFile = "profile_${days}_${units}x${layers}";
        runCmd("rm $profileFile*");

        # create the initial profile
        runCmd("alchemycreateprofile --name \"${units}x${layers} $days-day\" --profile $profileFile --layers $layers --units $units --days $days --train $trainFile >> $LOGFILE 2>&1");


        my $eta;
        my $steps = 1000;
        my $autoStop = 25;
        foreach $eta (0.0005, 0.0002, 0.0001, 0.00005, 0.00002)
        {
            runCmd("alchemytrain --train $trainFile --test $testFile --profile $profileFile --steps $steps --autostop $autoStop --eta $eta >> $LOGFILE 2>&1");
        }

        {
            my $error = parseLastError($LOGFILE);
            if ($error > 0.00)
            {
                printMsg("Error for trial $trial = $error") if $VERBOSE;
                push @errorList, $error;
            }
        }
    }

    my $error = average(@errorList);

    printMsg("Average error for trials: $error");
    printMsg("-"x40);

    open CSV, ">>$CSVFILE" or die "Unable to open $CSVFILE for writing: $!\n";
    print CSV "$days,$symbol,$units,$layers,$numTrials,$error\n";
    close CSV;
}

sub parseLastError
{
    my $filename = shift;

    #[04/18 20:19:33] Info: Minimum testing error was at step 135: 0.02209862

    my @a = `grep 'Minimum testing error' $filename | tail -n 1`;
    printMsg($a[0]) if $VERBOSE;

    if ($a[0] =~ m/Minimum testing error was at step \d+: ([e\-\d\.]+)/)
    {
        return $1;
    }
    else
    {
        warn "Failed to get last error\n";
        return 0.0;
    }
}

sub average
{
    my $tot = 0.0;
    my $n = 0;
    my $val;

    foreach $val (@_)
    {
        ++$n;
        $tot += $val;
    }

    return $n ? ($tot / $n) : 0.00;
}


1;
