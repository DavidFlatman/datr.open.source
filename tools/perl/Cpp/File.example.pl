#!/usr/bin/perl -w
use strict;
use warnings;

#-------------------------------------------------------------------------------
#   @file   File.example.pl                                                     
#                                                                               
#   @par    Classification:  UNCLASSIFIED, OPEN SOURCE                          
#                                                                               
#   @brief  Demonstrate how to use the Cpp::File code.                          
#                                                                               
#   @version    2020-09-08  DHF     Added header comment block.                 
#-------------------------------------------------------------------------------

use lib($ENV{"HOME"} . "/bin/perl");
use Cpp::File();

for (my $a = 0; $a <= $#ARGV; ++$a)
{
    &process($ARGV[$a]);
}

sub process
{
    my ($path) = @_;

    my $file = Cpp::File::new();

    $file->open($path);
    $file->token_fast_forward("struct", "Main", "::", "Data");

    print "line-number at: " . $file->{"line-number"} . "\n";
    print "file-size: " . $file->{"size"} . "\n";

    print "\n\n";

    my $token;
    my $type;

#$file->debug;
#    ($token, $type) = $file->token_get;
#
#    if (defined($token))
#    {
#        print "token: ${token}\n";
#    } else {
#        print "token is undefined\n";
#    }
#
#    if (defined($type))
#    {
#        print "type: ${type}\n";
#    } else {
#        print "type is undefined\n";
#    }


    while ( (($token, $type) = $file->token_get), defined($token) and $token ne "}")
    {
        print "token: " . $token 
            . ";  type : " . $type
            . "\n";
    }

    #$file->debug;
}

