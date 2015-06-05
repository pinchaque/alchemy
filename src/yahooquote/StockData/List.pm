
package StockData::List;

use strict;

use HTTP::Lite;

my $DEBUG = 0;

my $BASEURL = "http://biz.yahoo.com";
my $PATHURL = "/i/";

sub new 
{
    my $self = {};
    bless $self;
    return $self;
}

sub getData($)
{
    my ($self) = @_;

    print STDERR "StockData::List::getData()\n" if $DEBUG;

    my @topLinks = $self->parseLinks($self->getPage("$BASEURL$PATHURL"));

    my %allLinks = ();
    foreach my $link (@topLinks)
    {
        print STDERR "Reading links from $link...\n" if $DEBUG;
        foreach my $subLink ($self->parseLinks($self->getPage($link)))
        {
            $allLinks{$subLink} = 1;
        }
    }


    my %allSymbols = ();
    foreach my $link (sort keys %allLinks)
    {
        print STDERR "Processing symbols from $link...\n" if $DEBUG;
        foreach my $symbol ($self->parseSymbols($self->getPage($link)))
        {
            # duplicate symbols are fairly common, so we don't bother looking
            # for them
            $allSymbols{$symbol} = 1;
        }
    }

    return sort keys %allSymbols;
}


sub getPage($$)
{
    my ($self, $url) = @_;

    print STDERR "StockData::List::getPage($url)\n" if $DEBUG;

    my $http = new HTTP::Lite;
    my $req = $http->request($url);

    if ($req)
    {
        my $st = $http->status();
        my $msg = $http->status_message(); 
        $msg =~ s/\r\n//g;
        
        if ($st == 200)
        {
            my $data = $http->body();
            return $data;
        }
        else
        {
            die "Failed to retrieve url $url: $st: $msg\n";
        }
    }
    else
    {
        die "Request failed for $url: $!\n";
    }
}


sub parseLinks($$)
{
    my ($self, $page) = @_;

    my @links = ();

    print STDERR "StockData::List::parseLinks(...)\n" if $DEBUG;

    foreach my $line (split /<\/a>/, $page)
    {
        if ($line =~ m|href=$PATHURL([\w\-]+\.html)|)
        {
            push @links, $1;
        }
    }

    return map { "$BASEURL$PATHURL$_" } @links;
}


sub parseSymbols($$)
{
    my ($self, $page) = @_;

    my @symbols = ();

    print STDERR "StockData::List::parseSymbols(...)\n" if $DEBUG;

    foreach my $line (split /<\/tr>/, $page)
    {
        if ($line =~ m|<tt>\s*(\S+)\s*</tt>|)
        {
            push @symbols, $1;
        }
        elsif ($line =~ m|<tt>|)
        {
            warn "Ignoring row: $line\n";
        }
    }

    return @symbols;
}


1;
