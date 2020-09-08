#!/usr/bin/perl -w
use warnings;

use lib($ENV{"DATR_OS"} . "/tools/perl");
use Cpp::File();

for (my $a=0; $a <= $#ARGV; ++$a)
{
    &process($ARGV[$a]);
}

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
sub process
{
    my ($path) = @_;

    my $token;
    my $type;

    my $file = Cpp::File::new();

    $file->open($path);

    while ( (($token, $type) = $file->token_get), defined($token) and $token ne "}")
    {
        print "token: " . $token . ";  type = " . $type . "\n";
    }
}


