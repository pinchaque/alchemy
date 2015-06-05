
package StockData::Info;

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

sub new 
{
  my $self = {};
  bless $self;
  return $self;
}

sub getData2
{
  my ($self, $tickerSymbol) = @_;
  open F, '/tmp/pr.html' or die "failed";
  my @data = <F>;
  close F;
  my $data = join "\n", @data;
  return processData($tickerSymbol, $data);
}

sub getData
{
  my ($self, $tickerSymbol) = @_;


  my $doc = "http://finance.yahoo.com/q/pr?s=$tickerSymbol";

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

sub convertHtml($)
{
  my ($data) = @_;

  my %patterns = (
      '&amp;' => '&'
      );

  foreach my $k (keys %patterns)
  {
    my $v = $patterns{$k};
    $data =~ s/$k/$v/g; 
  }

  return $data;
}

sub trim($)
{
  my $x = shift @_;
  $x =~ s/^\s+//;
  $x =~ s/\s+$//;
  return $x;
}

# gets data from two adjacent cells where first cell matches $label. Returns
# data from subsequent cell
sub getAdjacentCell($$)
{
  my ($label, $data) = @_;

  if ($data =~ m|<td[^>]*>$label:?\s*</td><td[^>]*>([^<>]+)</td>|is)
  {
    my $ret = trim $1;

    # trim percentages
    if ($ret =~ m/^([\d\.]+)\%$/)
    {
      $ret = $1;     
    }
    # trim dollars
    elsif ($ret =~ m/^\$\d/)
    {
      $ret =~ s/[\$,]//g;
    }
    # trim amounts
    elsif ($ret =~ m/^([\d\.]+)([BM])/)
    {
      $ret = $1;
      if ($2 == 'M')
      {
        $ret *= 1000000;
      }
      elsif ($2 == 'B')
      {
        $ret *= 1000000000;
      }
    }
    elsif ($ret =~ m/^(\d{2})-(\w{3})-(\d{2,4})$/)
    {
      my $day = $1;
      my $mon = $2;
      my $year = $3;
      if (defined ($MONTH_TO_NUM{$mon}))
      {
        $mon = $MONTH_TO_NUM{$mon};

        if ($year < 20)
        {
          $year += 2000;
        }
        elsif ($year < 100)
        {
          $year += 1900;
        }

        $ret = sprintf "%04d%02d%02dT000000", $year, $mon, $day;
      }
    }

    return $ret;
  }
  else
  {
    return '';
  }
}


sub processData
{
  my ($tickerSymbol, $data) = @_;

  my %data = ();

  my $name = '';
  if ($data =~ m|<td class="ygtb" height="30"><b>([^<>]+)</b></td>|is)
  { $name = $1; }
  elsif ($data =~ m|<td height="30" class="ygtb"><b>([^<>]+)</b></td>|is)
  { $name = $1; }

  $name =~ s| *\($tickerSymbol\)||i;
  $data{'Name'} = $name;

  if ($data =~ m|<td[^>]*>Morningstar Rating:</td><td[^>]*><img [^<>]* alt="(\d+) stars\s*"[^>]*></td>|is)
  {
    $data{'Rating'} = $1;
  }

  if ($data =~ m|<td[^>]*>Fund Family:</td><td[^>]*><a[^<>]*>([^<>]+)</a></td>|is)
  {
    $data{'Family'} = $1;
  }
  else
  {
    $data{'Family'} = getAdjacentCell('Fund Family', $data);
  }

  $data{'Category'} = getAdjacentCell('Category', $data);
  $data{'Assets'} = getAdjacentCell('Net Assets', $data);
  $data{'Inception'} = getAdjacentCell('Fund Inception Date', $data);
  $data{'Yield'} = getAdjacentCell('Yield', $data);
  $data{'Turnover'} = getAdjacentCell('Annual Holdings Turnover', $data);
  $data{'InitInvestment'} = getAdjacentCell('Min Initial Investment', $data);
  $data{'SubsInvestment'} = getAdjacentCell('Min Subsequent Investment', $data);
  $data{'ExpenseRatio'} = getAdjacentCell('Total Expense Ratio', $data);
  $data{'Max12b1Fee'} = getAdjacentCell('Max 12b1 Fee', $data);
  $data{'FrontLoad'} = getAdjacentCell('Max Front End Sales Load', $data);
  $data{'DeferredLoad'} = getAdjacentCell('Max Deferred Sales Load', $data);

  # clean up html
  foreach my $k (keys %data)
  {
    $data{$k} = convertHtml($data{$k});
  }

  return %data;
}


1;
