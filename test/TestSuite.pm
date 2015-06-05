# TestSuite Perl Module 
package TestSuite;

use strict;
use FileHandle;
use Cwd;

use vars qw( $PID $DEBUG $CMD_FILTER $RESULT_FILTER $PATH $LIBPATH
             $LIBPATHENVNAME $CMDDIR $DATADIRNAME $DATADIR $OUTDIR $GOLDENDIR
             $TMPDIR $NUM_RUN $NUM_DIFF $NUM_PASS $MODE $LOGFILE
             %TEST_GROUPS $DIFF_NAME $DIFF $STARTTIME );

use POSIX "sys_wait_h";

########################################################################

# Process group id; used as a unique number in temporary file names.
# Also used to kill the process group if we get interrupted.
$PID = getpgrp;

# Debug level, 0 = off, 1 = on.
$DEBUG = 0;

$CMD_FILTER = sub { return $_[0]; };

$RESULT_FILTER = sub { return $_[0]; };

$PATH = ".";

$LIBPATH = ".";

$CMDDIR = "cmd";

$DATADIRNAME = "data";

$DATADIR = "$DATADIRNAME";

$MODE = "debug";

$OUTDIR = "output";

$GOLDENDIR = "golden";

$TMPDIR = "tmp";

$LOGFILE = "$OUTDIR/testsuite.log";

%TEST_GROUPS = ();

$DIFF_NAME = "diff.out";

#$DIFF = "diff -wb";
$DIFF = "fpdiff.pl";

$STARTTIME = time;

# This is the local test suite overrides for the above variables
eval { require "TestSuite_local.pm" }
    if -e "TestSuite_local.pm";

########################################################################

sub init()
{
    system("mkdir -p $OUTDIR $TMPDIR > /dev/null 2>&1");

    printMsg("*** Initializing test suite");

    $ENV{"LIBPATH"} = "." unless defined $ENV{"LIBPATH"};
    $ENV{"LD_LIBRARY_PATH"} = "." unless defined $ENV{"LD_LIBRARY_PATH"};
    $ENV{"PATH"} = "." unless defined $ENV{"PATH"};

    $ENV{"LIBPATH"} .= ":$LIBPATH";
    $ENV{"LD_LIBRARY_PATH"} .= ":$LIBPATH";
    $ENV{"PATH"} .= ":$PATH";

    $NUM_RUN = 0;
    $NUM_DIFF = 0;
    $NUM_PASS = 0;

    # for forking: kill children if interrupted
    $SIG{CHLD} = sub { 1; };
    $SIG{INT} = $SIG{HUP} = $SIG{TERM} = sub {
        # kill child if it hasn't already finished
        kill 'INT', $PID;
        exit(0);
    };

    $STARTTIME = time;
}

sub getElapsedStr($$)
{
    my $end = shift;
    my $start = shift;
    my $elapsed = $end - $start;

    my $h = $elapsed / 3600;
    $elapsed = $elapsed % 3600;
    my $m = $elapsed / 60;
    $elapsed = $elapsed % 60;
    my $s = $elapsed;

    return sprintf("%02d:%02d:%02d", $h, $m, $s);
}

sub finish()
{
    my $elapsed = getElapsedStr(scalar time, $STARTTIME);
    printMsg("*** Finishing test suite");
    printMsg("*** Total elapsed time: $elapsed");
}

sub save()
{
    my ($sec, $min, $hour, $mday, $mon, $year) = localtime(time);

    my $timestamp = sprintf("TEST-%04d:%02d:%02d:%02d:%02d:%02d",
                            $year + 1900, $mon + 1, $mday + 1, 
                            $hour, $min, $sec);

    if (-d $OUTDIR)
    {
        printMsg("--- Saving $OUTDIR to $timestamp");
        runCmd("mv $OUTDIR $timestamp");
    }
    else
    {
        printMsg("!!! ERROR: Directory $OUTDIR does not exist");
    }
}


sub run(@)
{
    my $test;
    foreach $test (@_)
    {
        printMsg("--- Running test $test");

        my $testoutdir = "$OUTDIR/$test";

        executeTest($test);

        my $testgoldendir = "$GOLDENDIR/$test";

        if (-e $testgoldendir)
        {
            processDiffs($test);
        }
        else
        {
            printMsg("--- Missing golden directory: $testgoldendir");
        }
    }

    return $NUM_DIFF;
}

sub diff(@)
{
    my $test;
    foreach $test (@_)
    {
        my $testoutdir = "$OUTDIR/$test";
        my $testgoldendir = "$GOLDENDIR/$test";

        # if old test dir exists, we remove the diff file from it
        if (-e $testoutdir)
        {
            runCmd("rm -f $testoutdir/$DIFF_NAME");

            if (-e $testgoldendir)
            {
                printMsg("--- Diffing test $test");
                $NUM_RUN++;
                processDiffs($test);
            }
            else
            {
                printMsg("--- Missing golden directory: $testgoldendir");
            }
        }
        else
        {
            printMsg("--- Missing output directory: $testoutdir");
        }
    }

    return $NUM_DIFF;
}


sub describe(@)
{
    my $test;
    foreach $test (@_)
    {
        printMsg("--- Description of $test:");

        describeTest($test);
    }

    return 0;
}


sub model(@)
{
    my $test;
    foreach $test (@_)
    {
        printMsg("--- Modeling test $test");

        my $testoutdir = "$OUTDIR/$test";
        my $testgoldendir = "$GOLDENDIR/$test";
        
        if (-e $testgoldendir)
        {
            printMsg("--- Golden directory already exists: $testgoldendir. Skipping.");
        }
        elsif (not -e $testoutdir)
        {
            printMsg("--- Output directory doesn't exist: $testoutdir.");
            printMsg("--- Create it with the 'run' action.");
        }
        else
        {
            printMsg("--- Creating golden directory: $testgoldendir");
            system("mkdir -p $testgoldendir 2>&1 > /dev/null");

            my @files = getFileList($testoutdir, $test);

            copyOutput(\@files, $test);
        }
    }

    return 0;
}

sub update(@)
{
    my $test;
    foreach $test (@_)
    {
        printMsg("--- Updating test $test");

        my $testoutdir = "$OUTDIR/$test";
        my $testgoldendir = "$GOLDENDIR/$test";
        
        if (not -e $testgoldendir)
        {
            printMsg("!!! WARNING: Missing golden directory $testgoldendir");
            printMsg("!!! WARNING: Ignoring update for $test");
        }
        elsif (not -e $testoutdir)
        {
            printMsg("!!! WARNING: Missing output directory $testoutdir");
            printMsg("!!! WARNING: Ignoring update for $test");
        }
        else
        {
            printMsg("--- Updating files in $testoutdir");

            my @files = getFileList($testgoldendir, $test);

            copyOutput(\@files, $test);
        }
    }

    return 0;
}

sub executeTest($)
{
    my $test = shift;
    my $testoutdir = "$OUTDIR/$test";
    my $testtmpdir = "$TMPDIR/$test";

    printMsg("[executeTest($test)]") if $DEBUG;

    # if old test dir exists, we remove its contents
    if (-e $testoutdir)
    {
        runCmd("rm -rf $testoutdir/*");
    }
    else
    {
        runCmd("mkdir -p $testoutdir");
    }
    if (-e $testtmpdir)
    {
        runCmd("rm -rf $testtmpdir/*");
    }
    else
    {
        runCmd("mkdir -p $testtmpdir");
    }

    $NUM_RUN++;

    runCmd("rm -f $testoutdir/$DATADIRNAME");

    runCmd("ln -s `cd $DATADIR; pwd` $testoutdir/$DATADIRNAME");

    my $origcmdfile = "$CMDDIR/$test";
    open ORIGF, "$origcmdfile"
        or die "ERROR: Failed to open $CMDDIR/$test for read: $!\n";
    my @orig_cmds = <ORIGF>;
    close ORIGF;

    my @cmds = map { &{$CMD_FILTER}($_, $test)} @orig_cmds;

    my $cmdfile = "$testoutdir/$test";
    open CMDF, ">$cmdfile"
        or die "ERROR: Failed to open $cmdfile for write: $!\n";

    print CMDF @cmds;
    close CMDF;

    runCmd("chmod +x $cmdfile");

    my $ret = system("cd $testoutdir; $test > stdout 2>stderr");
    if ($ret)
    {
        printMsg("!!! WARNING: '$test' returned non-zero ($ret)");
    }
    return $ret;
}

sub describeTest($)
{
    my $test = shift;

    printMsg("[describeTest($test)]") if $DEBUG;

    # spacing around description
    printMsg("", "");

    open CMDF, "$CMDDIR/$test"
        or die "ERROR: Failed to open $CMDDIR/$test for read: $!\n";

    my $line;
    while ($line = <CMDF>)
    {
        chomp $line;

        # ignore bang lines
        next if ($line =~ m|^\#!|);

        # remove starting whitespace
        $line =~ s/^\s*//;
        
        # ignore empty lines
        next if ($line =~ m|^\s*$|);

        # If this is a comment line, then we print it as part of the desc
        if ($line =~ m|^\#|)
        {
            $line =~ s/^\# ?//;
           
            printMsg($line, "> ");
        }
        # else this wasn't a comment line and wasn't blank, so it must
        # be part of the script. So we are done reading the description!
        else
        {
            last;
        }

    }
    close CMDF;

    # spacing around description
    printMsg("", "");

    return 0;
}

sub filterFile($$$)
{
    my $test = $_[2];
    printMsg("[filterFile($_[0],$_[1])]") if $DEBUG;

    open F1, $_[0] or die "ERROR: Failed to open $_[0] for read: $!\n";
    open F2, ">$_[1]" or die "ERROR: Failed to open $_[1] for write: $!\n";

    my $line;
    while ($line = <F1>)
    {
        print F2 &{$RESULT_FILTER}($line, $test);
    }

    close F1;
    close F2;
}

sub runDiff($$$)
{
    my $test = shift;
    my $f1 = shift;
    my $f2 = shift;

    printMsg("[runDiff($test, $f1, $f2)]") if $DEBUG;

    my $testoutdir = "$OUTDIR/$test";

    my $filt1 = $f1;
    $filt1 =~ s|.*/||;
    $filt1 = "$TMPDIR/$test/$filt1.$PID.golden";

    my $filt2 = $f2;
    $filt2 =~ s|.*/||;
    $filt2 = "$TMPDIR/$test/$filt2.$PID.test";

    filterFile($f1, $filt1, $test);
    filterFile($f2, $filt2, $test);
    
    my $tmpdiff = "$testoutdir/tmp.$DIFF_NAME";

    # get differences, write to temp diff file
    my $ret = system("$DIFF '$filt1' '$filt2' >> $tmpdiff 2>> $LOGFILE");

    # if there were differences, then we add to the diff file
    if ($ret)
    {
        my $diff = "$testoutdir/$DIFF_NAME";

        open DF, ">>$diff"
            or die "ERROR: Failed to open $diff for write: $!\n";
        print DF "<<<<< GOLDEN: $f1 ($filt1)\n";
        print DF ">>>>> OUTPUT: $f2 ($filt2)\n";

        # copy differences from tmpdiff to diff
        open TDF, $tmpdiff
            or die "ERROR: Failed to open $tmpdiff for read: $!\n";
        while (<TDF>)
        {
            print DF;
        }
        close TDF;

        print DF "-"x72, "\n";
        close DF;
    }
    else
    {
        system("rm -f '$filt1' '$filt2'") unless $DEBUG;
    }
    system("rm -f $tmpdiff");

    return $ret;
}

sub processDiffs($)
{
    my $test = shift;
    my $testoutdir = "$OUTDIR/$test";
    my $testgoldendir = "$GOLDENDIR/$test";

    printMsg("[processDiffs($test)]") if $DEBUG;

    my @files = getFileList($testgoldendir, $test);

    my $num_files = 0;
    my $num_missing = 0;
    my $num_diff = 0;

    my $file;
    foreach $file (@files)
    {
        $num_files++;

        my $goldenfile = "$testgoldendir/$file";
        my $outfile = "$testoutdir/$file";

        if (not -e $outfile)
        {
            printMsg("!!! File is missing: $outfile");
            $num_missing++;
        }
        elsif (runDiff($test, $goldenfile, $outfile))
        {
            printMsg("!!! Diffs found: $outfile");
            $num_diff++;
        }
    }

    if ($num_diff or $num_missing)
    {
        $NUM_DIFF++;
        printMsg("!!! Examine: $testoutdir/$DIFF_NAME");
    }
    else
    {
        $NUM_PASS++;
    }
}


sub getFileList($$)
{
    my $dir = shift;
    my $test = shift;

    printMsg("[getFileList($dir)]") if $DEBUG;

    my @files = ();

    my $file;
    # get list of all files in the output directory
    foreach $file (`cd $dir; find . -type f`)
    {
        chomp $file;
        $file =~ s|./||;
        next if ($file eq "$DIFF_NAME");
        next if ($file =~ /CVS/);
        next if ($file eq "$test");

        printMsg("--> $file") if $DEBUG;

        push @files, $file;
    }

    return @files;
}

sub copyOutput(\@$)
{
    my @files = @{$_[0]}; shift;
    my $test = shift;

    if ($DEBUG)
    {
        printMsg("[copyOutput($test)]");
        my $f;
        foreach $f (@files)
        {
            printMsg("--> $f");
        }
    }
    
    # copy the files using tar
    my $file;
    foreach $file (@files)
    {
        if (-e "$OUTDIR/$test/$file")
        {
            my $cmd = "(cd $OUTDIR/$test; tar cf - '$file') | ";
            $cmd .= "(cd $GOLDENDIR/$test; tar xf - )";
            runCmd($cmd);
        }
        else
        {
            printMsg("!!! WARNING: File does not exist: $OUTDIR/$test/$file");
        }
    }
}

sub printMsg($)
{
    my $msg = shift;

    my ($sec, $min, $hour, $mday, $mon) = localtime(time);

    my $timestamp = shift;

    if (not defined $timestamp)
    {
        $timestamp = sprintf("[%02d/%02d %02d:%02d:%02d] ",
                             $mon + 1, $mday, $hour, $min, $sec);
    }

    if (open LOGF, ">>$LOGFILE")
    { 
        print LOGF "$timestamp$msg\n";
        close LOGF;
    }
    else
    {
        warn "WARNING: Unable to open log file $LOGFILE for append: $!\n"
            if $DEBUG;
    }

    print STDERR "$timestamp$msg\n";
}

sub runCmd($)
{
    my $cmd = shift;

    printMsg("[runCmd($cmd)]") if $DEBUG;

    if (system($cmd))
    {
        printMsg("ERROR: Command failed: $cmd: $!");
        die "Abort.\n";
    }
}


sub printSummary()
{
    my $str;

    $str = sprintf("%30s %5d", "Number of tests run:", $NUM_RUN);
    printMsg("--- $str");

    $str = sprintf("%30s %5d (%.2f%%)",
                   "Number of tests passed:",
                   $NUM_PASS,
                   ($NUM_RUN ? ($NUM_PASS / $NUM_RUN) : 0) * 100.0);
    printMsg("--- $str");

    $str = sprintf("%30s %5d (%.2f%%)",
                   "Number of tests with diffs:",
                   $NUM_DIFF,
                   ($NUM_RUN ? ($NUM_DIFF / $NUM_RUN) : 0) * 100.0);
    printMsg("--- $str");
}


sub validTestName($)
{
    my $name = shift;

    # ignore . and ..
    return 0 if ($name =~ m/^\.\.?$/);
    
    # ignore emacs files
    return 0 if ($name =~ m/~$/);
    
    # only use regular files
    return 0 unless (-f "$CMDDIR/$name");

    return 1;
}

# sorts based on test name then numerically
# Helps avoid ugliness when 'test10' is lexicographically before 'test2'
sub testSorter
{
    if ($a =~ m/^(\w+)(\d+)$/)
    {
        my $a_name = $1;
        my $a_number = $2;

        if ($b =~ m/^(\w+)(\d+)$/)
        {
            my $b_name = $1;
            my $b_number = $2;

            return (($a_name cmp $b_name) or ($a_number <=> $b_number));
        }
        else
        {
            return ($a cmp $b);
        }
    }
    else
    {
        return ($a cmp $b);
    }
}

sub getTestList()
{
    opendir CMDDIR, $CMDDIR;
    my @contents;
    my $entry;
    while ($entry = readdir CMDDIR)
    {
        push @contents, $entry if validTestName($entry);
    }
    closedir CMDDIR;
    return sort testSorter @contents;
}

sub expandSubList($$);
sub expandSubList($$)
{
    my $name = shift;
    my $depth = shift;
    my @ret = ();

    printMsg("[expandSubList($name,$depth)]") if $DEBUG;

    # no infinite recursion!
    if ($depth > 64)
    {
        printMsg("!!! ERROR: Breaking infinite recursion in test $name");
        return ();
    }

    # strip off CMDDIR prefix, if any
    $name =~ s|^$CMDDIR/||;

    # Is this name a regular expression?
    if ($name =~ m|^/(.*)/$|)
    {
        my $regexp = $1;

        # get all tests matching this regular expression out of cmd directory
        my $test;
        foreach $test (getTestList())
        {
            if ($test =~ m|$regexp|)
            {
                push @ret, $test;
            }
        }

        printMsg("Expanded regular expression '$regexp'") if $DEBUG;
    }
    # otherwise we treat the name as a glob
    else
    {
        my $pwd = getcwd();

        # match the glob (we must do this all-at-once so that we aren't in
        # a different directory for validTestName)
        chdir $CMDDIR;
        my @globresults = glob $name;
        chdir $pwd;

        my $test;
        foreach $test (@globresults)
        {
            push @ret, $test if (validTestName($test));
        }

        printMsg("Expanded file glob '$name'") if $DEBUG;
    }

    # if we still don't have anything in @ret, then perhaps the name
    # wasn't a glob after all.. maybe it was a test group!
    if (not @ret and defined $TEST_GROUPS{$name})
    {
        my $subelem;
        foreach $subelem (@{$TEST_GROUPS{$name}})
        {
            push @ret, expandSubList($subelem, $depth + 1);
        }
    }

    return @ret;
}

sub expandTestList(@)
{
    my @testlist = ();
    my $name;

    # expand all list names
    foreach $name (@_)
    {
        my @sublist = expandSubList($name, 0);

        printMsg("Expanded $name to [ @sublist ]") if $DEBUG;

        push @testlist, @sublist;
    }

    # Remove duplicates without reordering and validate list

    my $i;
    my $position = 0;
    my $numelem = @testlist;
    my %found = ();
    for ($i = 0; $i < $numelem; $i++)
    {
        $name = $testlist[$i];
        my $cmdpath = "$CMDDIR/$name";
        if (defined $found{$name})
        {
            printMsg("!!! WARNING: Ignoring duplicate test: $name") if $DEBUG;
        }
        elsif (-d $cmdpath)
        {
            if ($cmdpath !~ /CVS/)
            {
                printMsg("!!! WARNING: Ignoring directory: $cmdpath");
            }
        }
        elsif (not -e $cmdpath)
        {
            printMsg("!!! WARNING: Command file $cmdpath does not exist");
            printMsg("!!! WARNING: Ignoring test: $name");
        }
        elsif (not -f $cmdpath)
        {
            printMsg("!!! WARNING: Command file $cmdpath is not a "
                     . "regular file.");
            printMsg("!!! WARNING: Ignoring test: $name");
        }
        elsif ($cmdpath =~ m/~$/) # ignore emacs files
        {
            printMsg("!!! WARNING: Skipping emacs file: $cmdpath")
                if $DEBUG;
        }
        else
        {
            $found{$name} = $position++;
        }
    }

    # Populate @ret in original order. There should be no undef elements in
    # the array after we're done.
    my @ret = ();
    foreach $name (keys %found)
    {
        $ret[$found{$name}] = $name;
    }

    return @ret;
}

1;
