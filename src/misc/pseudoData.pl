#!/usr/bin/perl -w

if (1)
{
    my $symbol = "sinwave";
    my $start = "20040101T000000";
    my $end = "20050519T000000";
    writeSinData($symbol, $start, $end);
    writeMeta($symbol, $start, $end);
}

if (0)
{
    my $symbol = "slinear";
    my $start = "20040101T000000";
    my $end = "20050419T000000";
    writeLinearData($symbol, $start, $end);
    writeMeta($symbol, $start, $end);
}


sub getSymbolRoot
{
    my $symbol = shift;

    return "$symbol";
}


sub writeSinData
{
    my ($symbol, $start, $end) = @_;
    my $fname = getSymbolRoot($symbol) . ".dat";

    my $truncStart = $start;
    $truncStart =~ s/T.*//;

    my $truncEnd = $end;
    $truncEnd =~ s/T.*//;

    open F, ">$fname"
        or die "Failed to open for writing: $fname: $!\n";

    my $idx = 0;

    #20000623T000000,14.19,14.44,13.5,15.5,366400,14.44

    while ($truncStart < $truncEnd)
    {
        my $close = 5.0 * sin(3.1416 * 0.07 * $idx) + 10.0;
        my $open = $close - 0.5;
        my $min = $close - 0.6;
        my $max = $close + 0.1;
        my $vol = 100000 + $idx;
        my $adj = $close;

        print F "${truncStart}T000000,$open,$close,$min,$max,$vol,$adj\n";

        # 20051101
        my $year = int($truncStart / 10000);
        my $month = int($truncStart / 100) % 100;
        my $day = $truncStart % 100;

        if (++$day > 28)
        {
            $day = 1;
            if (++$month > 12)
            {
                $month = 1;
                ++$year;
            }
        }

        $truncStart = int($year * 10000) + int($month * 100) + $day;

        $idx++;
    }

    close F;
}

sub writeLinearData
{
    my ($symbol, $start, $end) = @_;
    my $fname = getSymbolRoot($symbol) . ".dat";

    my $truncStart = $start;
    $truncStart =~ s/T.*//;

    my $truncEnd = $end;
    $truncEnd =~ s/T.*//;

    open F, ">$fname"
        or die "Failed to open for writing: $fname: $!\n";

    my $idx = 0;

    #20000623T000000,14.19,14.44,13.5,15.5,366400,14.44

    while ($truncStart < $truncEnd)
    {
        my $close = 5.0 + (0.20 * $idx);
        my $open = $close - 0.5;
        my $min = $close - 0.6;
        my $max = $close + 0.1;
        my $vol = 100000 + $idx;
        my $adj = $close;

        print F "${truncStart}T000000,$open,$close,$min,$max,$vol,$adj\n";

        # 20051101
        my $year = int($truncStart / 10000);
        my $month = int($truncStart / 100) % 100;
        my $day = $truncStart % 100;

        if (++$day > 28)
        {
            $day = 1;
            if (++$month > 12)
            {
                $month = 1;
                ++$year;
            }
        }

        $truncStart = int($year * 10000) + int($month * 100) + $day;

        $idx++;
    }

    close F;
}


sub writeMeta
{
    my ($symbol, $start, $end) = @_;
    my $fname = getSymbolRoot($symbol) . ".meta";

    open F, ">$fname"
        or die "Failed to open for writing: $fname: $!\n";

    print F "Symbol $symbol\n";
    print F "Start 14000101T000000\n";
    print F "End $end\n";


    close F;
}
