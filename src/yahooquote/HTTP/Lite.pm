
#
# HTTP::Lite.pm
#
# $Id: Lite.pm,v 1.1 2005-02-21 06:32:45 chuck Exp $
#
# $Log: Lite.pm,v $
# Revision 1.1  2005-02-21 06:32:45  chuck
# Add yahoo quote fetch script
#
# Revision 1.1  2003/08/19 07:15:57  chuck
# Used for HTTP access
#
# Revision 1.7  2000/12/21 18:05:09  rhooper
# FIxed post form MIME-Type -- was application/x-www-urlencoded should
# have been x-www-form-urlencoded.
#
# Revision 1.6  2000/11/02 01:47:58  rhooper
# Fixed a greedy regular expression in the URL decoder.  URLs with :// embedded now work.
#
# Revision 1.5  2000/10/31 01:27:03  rhooper
# added proxy port support.
#
# Revision 1.4  2000/09/29 03:47:53  rhooper
# Requests without a terminating CR or LF are now properly handled.
# HTTP/1.1 chunked mode transfers are now supported
# Host: headers are properly added to all requests
# Proxy support has been added
# Significant test code updates
#
# Revision 1.3  2000/09/09 18:06:55  rhooper
# Revision 1.2  2000/08/28 02:46:05  rhooper
# Revision 1.1  2000/08/28 02:43:57  rhooper
# Initial revision
#

package HTTP::Lite;

use vars qw($VERSION);
use strict qw(vars);

$VERSION = "0.2.5";
my $CRLF = "\r\n";

# Required modules for Network I/O
use Socket 1.3;
use Fcntl;
use Errno qw(EAGAIN);

# Forward declarations
sub prepare_post;
sub http_writeline;
sub http_readline;

sub new 
{
  my $self = {};
  bless $self;
  $self->initialize();
  return $self;
}

sub initialize
{
  my $self = shift;
  foreach my $var ("body", "request", "content", "status", "proxy",
    "proxyport", "resp-protocol", "error-message", "response", 
    "resp-headers")
  {
    $self->{$var} = undef;
  }
  $self->{method} = "GET";
  $self->{timeout} = 120;
  $self->{headers} = { 'User-Agent' => "HTTP::Lite/$VERSION" };
  $self->{HTTPReadBuffer} = "";
}

sub reset
{
  my $self = shift;
  $self->initialize;
}


# URL-encode data
sub escape {
  my $toencode = shift;
  $toencode=~s/([^a-zA-Z0-9_.-])/uc sprintf("%%%02x",ord($1))/eg;
  return $toencode;
}

sub request
{
  my ($self, $url) = @_;
  
  my $method = $self->{method};

  # Parse URL 
  my ($protocol,$host,$junk,$port,$object) = 
    $url =~ m{^([^:/]+)://([^/:]*)(:(\d+))?(/.*)$};

  # Only HTTP is supported here
  if ($protocol ne "http")
  {
    warn "Only http is supported by HTTP::Lite";
    return undef;
  }
  
  # Setup the connection
  my $proto = getprotobyname('tcp');
  my $fhname = $url . localtime;
  my $fh = *$fhname;
  socket($fh, PF_INET, SOCK_STREAM, $proto);
  $port = 80 if !$port;

  my $connecthost = $self->{'proxy'} || $host;
  $connecthost = $connecthost ? $connecthost : $host;
  my $connectport = $self->{'proxyport'} || $port;
  $connectport = $connectport ? $connectport : $port;
  my $addr = inet_aton($connecthost) || return undef;
  if ($connecthost ne $host)
  {
    # if proxy active, use full URL as object to request
    $object = "$url";
  }

  my $sin = sockaddr_in($connectport,$addr);
  connect($fh, $sin) || return undef;
  # Set nonblocking IO on the handle to allow timeouts
  fcntl($fh, F_SETFL, O_NONBLOCK);

  # Start the request (HTTP/1.1 mode)
  http_writeline($fh, "$method $object HTTP/1.1$CRLF");

  # Add some required headers
  # we only support a single transaction per request in this version.
  $self->add_req_header("Connection", "close");    
  $self->add_req_header("Host", $host);
  $self->add_req_header("Accept", "*/*");
    
  # Output headers
  my $headerref = $self->{headers};
  foreach my $header (keys %$headerref)
  {
    http_writeline($fh, $header.": ".$$headerref{$header}."$CRLF");
  }
  
  # Handle Content-type and Content-Length seperately
  if (defined($self->{content}))
  {
    http_writeline($fh, "Content-Length: ".length($self->{content})."$CRLF");
  }
  http_writeline($fh, "$CRLF");
  
  # Output content, if any
  if (defined($self->{content}))
  {
    http_writeline($fh, $self->{content});
  }
  
  # Read response from server
  my $headmode=1;
  my $chunkmode=undef;
  my $chunksize=0;
  my $chunklength=0;
  my $chunk;
  my $line = 0;
  while ($_ = $self->http_readline($fh))
  {
    #print "reading: $chunkmode, $chunksize, $chunklength, $headmode, ".
    #	length($self->{body}).": //$_//\n";
    $line++;
    if ($line == 1)
    {
      my ($proto,$status,$message) = split(' ', $_, 3);
      $self->{status}=$status;
      $self->{'resp-protocol'}=$proto;
      $self->{'error-message'}=$message;
      next;
    } 
    $self->{response} .= $_;
    if ($_ =~ /^[\r\n]*$/ && ($headmode || $chunkmode eq "entity-header"))
    {
      if ($chunkmode)
      {
        undef $chunkmode;
      }
      $headmode = 0;
      
      # Check for Transfer-Encoding
      my $header = join(' ',@{$self->get_header("Transfer-Encoding")});
      if ($header =~ /chunked/i)
      {
        $chunkmode = "chunksize";
      }
      next;
    }
    if ($headmode || $chunkmode eq "entity-header")
    {
      my ($var,$data) = $_ =~ /^([^:]*):\s*(.*)$/;
      if (defined($var))
      {
        $data =~s/[\r\n]$//g;
        $var = lc($var);
        $var =~ s/^(.)|(-.)/&upper($1,$2)/ge;
        my $hr = ${$self->{'resp-headers'}}{$var};
        if (!ref($hr))
        {
          $hr = [ $data ];
        }
        else 
        {
          push @{ $hr }, $data;
        }
        ${$self->{'resp-headers'}}{$var} = $hr;
      }
    } elsif ($chunkmode)
    {
      if ($chunkmode eq "chunksize")
      {
        $chunksize = $_;
        $chunksize =~ s/^\s*|;.*$//g;
        $chunksize =~ s/\s*$//g;
        $chunksize = hex($chunksize);
        if ($chunksize == 0)
        {
          $chunkmode = "entity-header";
        } else {
          $chunkmode = "chunk";
          $chunklength = 0;
        }
      } elsif ($chunkmode eq "chunk")
      {
        $chunk .= $_;
        $chunklength += length($_);
        if ($chunklength >= $chunksize)
        {
          $chunkmode = "chunksize";
          if ($chunklength > $chunksize)
          {
            $chunk = substr($chunk,0,$chunksize);
          } 
          elsif ($chunklength == $chunksize && $chunk !~ /$CRLF$/) 
          {
            # chunk data is exactly chunksize -- need CRLF still
            $chunkmode = "ignorecrlf";
          }
          $self->{'body'} .= $chunk;
          $chunk="";
          $chunklength = 0;
          $chunksize = "";
        } 
      } elsif ($chunkmode eq "ignorecrlf")
      {
        $chunkmode = "chunksize";
      }
    } else {
      $self->{body}.=$_;
    }
  }
  return $self->{status};
}

sub add_req_header
{
  my $self = shift;
  my ($header, $value) = @_;
  
  ${$self->{headers}}{$header} = $value;
}

sub get_req_header
{
  my $self = shift;
  my ($header) = @_;
  
  return $self->{headers}{$header};
}

sub delete_req_header
{
  my $self = shift;
  my ($header) = @_;
  
  my $exists;
  if ($exists=defined(${$self->{headers}}{$header}))
  {
    delete ${$self->{headers}}{$header};
  }
  return $exists;
}

sub body
{
  my $self = shift;
  return $self->{body};
}

sub response
{
  my $self = shift;
  return $self->{response};
}

sub status
{
  my $self = shift;
  return $self->{status};
}

sub protocol
{
  my $self = shift;
  return $self->{'resp-protocol'};
}

sub status_message
{
  my $self = shift;
  return $self->{'error-message'};
}

sub proxy
{
  my $self = shift;
  my ($value) = @_;
  
  # Parse URL 
  my ($protocol,$host,$junk,$port,$object) = 
    $value =~ m{^(\S+)://([^/:]*)(:(\d+))?(/.*)$};
  if (!$host)
  {
    ($host,$port) = $value =~ /^([^:]+):(.*)$/;
  }

  $self->{'proxy'} = $host || $value;
  $self->{'proxyport'} = $port || 80;
}

sub headers_array
{
  my $self = shift;
  
  my @array = ();
  
  foreach my $header (keys %{$self->{'resp-headers'}})
  {
    my $aref = ${$self->{'resp-headers'}}{$header};
    foreach my $value (@$aref)
    {
      push @array, "$header: $value";
    }
  }
  return @array;
}

sub headers_string
{
  my $self = shift;
  
  my $string = "";
  
  foreach my $header (keys %{$self->{'resp-headers'}})
  {
    my $aref = ${$self->{'resp-headers'}}{$header};
    foreach my $value (@$aref)
    {
      $string .= "$header: $value\n";
    }
  }
  return $string;
}

sub get_header
{
  my $self = shift;
  my $header = shift;

  return $self->{'resp-headers'}{$header};
}


sub prepare_post
{
  my $self = shift;
  my $varref = shift;
  
  my $body = "";
  while (my ($var,$value) = map { escape($_) } each %$varref)
  {
    if ($body)
    {
      $body .= "&$var=$value";
    } else {
      $body = "$var=$value";
    }
  }
  $self->{content} = $body;
  $self->{headers}{'Content-Type'} = "application/x-www-form-urlencoded";
  $self->{method} = "POST";
}

sub http_writeline
{
  my ($fh,$line) = @_;
  syswrite($fh, $line, length($line));
}


sub http_readline
{
  my $self = shift;
  my ($fh, $timeout) = @_;
  my $EOL = "\n";
  
  # is there a line in the buffer yet?
  while ($self->{HTTPReadBuffer} !~ /$EOL/)
  {
    # nope -- wait for incoming data
    my ($inbuf,$bits,$chars) = ("","",0);
    vec($bits,fileno($fh),1)=1;
    my $nfound = select($bits, undef, $bits, $timeout);
    if ($nfound == 0)
    {
      # Timed out
      return undef;
    } else {
      # Get the data
      $chars = sysread($fh, $inbuf, 256);
    }
    # End of stream?
    if ($chars <= 0 && !$!{EAGAIN})
    {
      last;
    }
    # tag data onto end of buffer
    $self->{HTTPReadBuffer}.=$inbuf;
  }
  # get a single line from the buffer
  my $nlat = index($self->{HTTPReadBuffer}, $EOL);
  my $newline;
  my $oldline;
  if ($nlat > -1)
  {
    $newline = substr($self->{HTTPReadBuffer},0,$nlat+1);
    $oldline = substr($self->{HTTPReadBuffer},$nlat+1);
  } else {
    $newline = substr($self->{HTTPReadBuffer},0);
    $oldline = "";
  }
  # and update the buffer
  $self->{HTTPReadBuffer}=$oldline;
  # Put the linefeed back on the line and return it
  return $newline;
}

sub upper
{
  return uc(join("",@_));
}

1;

__END__

=pod

=head1 NAME

HTTP::Lite - Lightweight HTTP implementation

=head1 SYNOPSIS

    use HTTP::Lite;
    $http = new HTTP::Lite;
    $req = $http->request("http://www.cpan.org/") 
        or die "Unable to get document: $!";
    print $http->body();

=head1 DESCRIPTION

    HTTP::Lite is a stand-alone lightweight HTTP/1.1
    implementation for perl.  It is not intended to replace LWP,
    but rather is intended for use in situations where it is
    desirable to install the minimal number of modules to
    achieve HTTP support, or where LWP is not a good candidate
    due to CPU overhead, such as slower processors.

    HTTP::Lite is ideal for CGI (or mod_perl) programs or for
    bundling for redistribution with larger packages where only
    HTTP GET and POST functionality are necessary.

    HTTP::Lite supports basic POST and GET operations only.  As
    of 0.2.1, HTTP::Lite supports HTTP/1.1 and is compliant with
    the Host header, necessary for name based virtual hosting. 
    Additionally, HTTP::Live now supports Proxies.

    If you require more functionality, such as FTP or HTTPS,
    please see libwwwperl (LWP).  LWP is a significantly better
    and more comprehensive package than HTTP::Lite, and should
    be used instead of HTTP::Lite whenever possible.

=head1 CONSTRUCTOR

=over 4

=item new

This is the constructor for HTTP::Lite.  It presently takes no
arguments.  A future version of HTTP::Lite might accept
parameters.

=back

=head1 METHODS

=over 4

=item request ( URL )

Initiates a request to the specified URL.

Returns undef if an I/O error is encountered, otherwise the HTTP
status code will be returned.  200 series status codes represent
success, 300 represent temporary errors, 400 represent permanent
errors, and 500 represent server errors.

See F<http://www.w3.org/Protocols/HTTP/HTRESP.html> for detailled 
information about HTTP status codes.

=item prepare_post

=item add_req_header ( $header, $value )
=item get_req_header ( $header )
=item delete_req_header ( $header )

Add, Delete, or a HTTP header(s) for the request.  These
functions allow you to override any header.  Presently, Host,
User-Agent, Content-Type, Accept, and Connection are pre-defined
by the HTTP::Lite module.  You may not override Host,
Connection, or Accept.

To provide (proxy) authentication or authorization, you would use:

    use HTTP::Lite;
    use MIME::Base64;
    $http = new HTTP::Lite;
    $encoded = encode_base64('username:password');
    $http->add_req_header("Authorization", $encoded);

B<NOTE>: The present implementation limits you to one instance
of each header.

=item body

Returns the body of the document retured by the remote server.

=item headers_array

Returns an array of the HTTP headers returned by the remote
server.

=item headers_string

Returns a string representation of the HTTP headers returned by
the remote server.

=item get_header ( $header )

Returns an array of values for the requested header.  

B<NOTE>: HTTP requests are not limited to a single instance of
each header.  As a result, there may be more than one entry for
every header.

=item protocol

Returns the HTTP protocol identifier, as reported by the remote
server.  This will generally be either HTTP/1.0 or HTTP/1.1.

=item proxy ( $proxy_server )

The URL or hostname of the proxy to use for the next request.

=item status

Returns the HTTP status code returned by the server.  This is
also reported as the return value of I<request()>.

=item status_message

Returns the textual description of the status code as returned
by the server.  The status string is not required to adhere to
any particular format, although most HTTP servers use a standard
set of descriptions.

=item response

Returns the entire unparsed HTTP response as returned by the
server.

=item reset

You must call this prior to re-using an HTTP::Lite handle,
otherwise the results are undefined.

=head1 EXAMPLES

    # Get and print out the headers and body of the CPAN homepage
    use HTTP::Lite;
    $http = new HTTP::Lite;
    $req = $http->request("http://www.cpan.org/")
        or die "Unable to get document: $!";
    die "Request failed ($req): ".$http->status_message()
      if $req ne "200";
    @headers = $http->headers_array();
    $body = $http->body();
    foreach $header (@headers)
    {
      print "$header$CRLF";
    }
    print "$CRLF";
    print "$body$CRLF";

    # POST a query to the dejanews USENET search engine
    use HTTP::Lite;
    $http = new HTTP::Lite;
    %vars = (
             "QRY" => "perl",
             "ST" => "MS",
             "svcclass" => "dncurrent",
             "DBS" => "2"
            );
    $http->prepare_post(\%vars);
    $req = $http->request("http://www.deja.com/dnquery.xp")
      or die "Unable to get document: $!";
    print "req: $req\n";
    print $http->body();

=head1 UNIMPLEMENTED

    - FTP 
    - HTTPS (SSL)
    - Authenitcation/Authorizaton/Proxy-Authorization
      are not directly supported, and require MIME::Base64.
    - Redirects (Location) are not automatically followed
    - multipart/form-data POSTs are not supported (necessary for
      File uploads).
    
=head1 BUGS

    Some bugs likely still exist.  This is a beta version.
    
    Large requests are stored in ram, potentially more than once
    due to HTTP/1.1 chunked transfer mode support.  A future
    version of this module may support writing requests to a
    filehandle to avoid excessive disk use.

=head1 ACKNOWLEDGEMENTS

	Marcus I. Ryan	shad@cce-7.cce.iastate.edu
	michael.kloss@de.adp.com

=head1 AUTHOR

Roy Hooper <rhooper@thetoybox.org>

=head1 SEE ALSO

L<LWP>
RFC 2068 - HTTP/1.1 -http://www.w3.org/

=head1 COPYRIGHT

Copyright (c) 2000 Roy Hooper.  All rights reserved.

This program is free software; you can redistribute it and/or modify it
under the same terms as Perl itself.

=cut
