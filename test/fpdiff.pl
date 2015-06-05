#!/usr/bin/perl -w
use strict;
use Getopt::Long;

sub main();
sub usage();
sub parseargs();
sub fddiff();
sub difflines($$);
sub decompline($);
sub comparefps($$);
sub externaldiff();
sub abort();

my $file1 = "";
my $file2 = "";
my $EPSILON = 0.0001;
my $epsilon = $EPSILON;
my $CLOSETOZERO = 0.00000000001;
my $EXTDIFF = "diff -w -b";

main();
exit 0;

sub main()
{
  parseargs();

  fddiff();
}

sub usage()
{
  print 
"Usage: fpdiff.pl [OPTION]... FILE1 FILE2
Print differences between FILE1 and FILE2 while allowing for floating
point error.

  -e NUM    percent difference allowed for floating point error
            ($EPSILON is the default)
  -x        run an external diff ($EXTDIFF FILE1 FILE2) and exit
  --help    display this help and exit

  exit values
  0         no error
  1         floating point differences exceed tolernece
  2         internal error
  note that if executed, the exit value of \"$EXTDIFF FILE1 FIL2\"
  is returned
";
}

sub parseargs()
{
  my $xdiff = 0;
  my $help = 0;
  GetOptions("e=f"=>\$epsilon, "x"=>\$xdiff, "help"=>\$help);
  if ($help != 0)
  {
    usage();
    exit 0;
  }
  if ($#ARGV < 1)
  {
    usage();
    exit 2;
  }
  $file1 = $ARGV[0];
  $file2 = $ARGV[1];
  if ($xdiff)
  {
    my $exitval = externaldiff();
    exit $exitval;
  }
}

sub fddiff()
{
  open FILE1, "<$file1" or die "Error opening file \"$file1\": $!\n";
  open FILE2, "<$file2" or die "Error opening file \"$file2\": $!\n";

  my $exitval = 0;
  my $line1 = <FILE1>;
  my $line2 = <FILE2>;

  # one file is empty
  if (($line1 && !$line2) || (!$line1 && $line2))
  {
    close FILE1;
    close FILE2;
    abort();
  }
  my $linenum = 1;
  while ($line1 && $line2)
  {
    chomp($line1);
    chomp($line2);
    my $d = difflines($line1, $line2);

    if ($d == 2)
    {
      print "line $linenum\n";
      print "< $line1\n";
      print "> $line2\n";
      $exitval = 1;
    }
    elsif ($d == 3)
    {
      close FILE1;
      close FILE2;
      abort();
    }

    $line1 = <FILE1>;
    $line2 = <FILE2>;
    $linenum++;
  }

  close FILE1;
  close FILE2;

  exit $exitval;
}

# return
#   0:  lines are the same
#   1:  lines are within the tolerable difference
#   2:  lines are similar but have different floating point values
#   3:  lines are not similar
sub difflines($$)
{
  my($l1, $l2) = @_;

  # if $l1 and $l2 are the same then there is nothing left to do
  if ($l1 eq $l2)
  {
    return 0;
  }

  my($nonfp1, @fps1) = decompline($l1);
  my($nonfp2, @fps2) = decompline($l2);

  # if $nonfp1 and $nonfp2 are not the same then the diff fails
  if ($nonfp1 ne $nonfp2)
  {
    return 3;
  }

  # if $#fps1 and $#fps2 are not equal then the diff fails
  if ($#fps1 != $#fps2)
  {
    return 3;
  }

  # all of the 3 cases have been checked so 2 is the worst that can happen after this point

  # compare each pair of $fps1[$i] and $fps2[$i]
  for (my $i = 0; $i <= $#fps1; $i++)
  {
    my $c = comparefps($fps1[$i], $fps2[$i]);
    if ($c == 1)
    {
      return 2;
    }
  }

  return 1;
}

sub decompline($)
{
  my $nonfp = shift;
  my @fps;

  my $i = 0;
  while ($nonfp =~ /(-?\d*\.\d+)/)
  {
    $fps[$i] = $1;
    $nonfp =~ s/$1//;
    $i++;
  }

  $nonfp =~ s/\s+/ /g;
  $nonfp =~ s/\s+$//g;

  return ($nonfp, @fps);
}

# return
#   0:  floating point numbers are within the tolerable differnce
#   1:  floating point numbers are not within the tolerable differnce
sub comparefps($$)
{
  my($fp1, $fp2) = @_;

  my $e;

  if ($fp1 > $CLOSETOZERO)
  {
    $e = $fp2 / $fp1;
  }
  elsif ($fp2 > $CLOSETOZERO)
  {
    $e = $fp1 / $fp2;
  }
  else
  {
    # both are really close to zero, so they must be the same, right?
    return 0;
  }

  $e = 1.0 - $e;
  if ($e < 0.0)
  {
    $e *= -1.0;
  }

  if ($e <= $epsilon)
  {
    return 0;
  }

  return 1;
}

sub externaldiff()
{
  my $diffout = `$EXTDIFF $file1 $file2`;
  my $ret = ($? >> 8);
  print $diffout;
  return $ret;
}

sub abort()
{
  print "Files differ by more than floating point error tolerances.\n";
  print "Output of \"$EXTDIFF $file1 $file2\" follows...\n";
  my $exitval = externaldiff();
  exit $exitval;
}
