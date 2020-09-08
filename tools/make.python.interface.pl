#!/usr/bin/perl -w
use warnings;

#-------------------------------------------------------------------------------
#                                                                               
#   @file   make.python.interface.pl                                            
#                                                                               
#   @par Classification:  UNCLASSIFIED, OPEN SOURCE                             
#                                                                               
#   @brief  Create an python interface for the source code.                     
#                                                                               
#   @warning    This script is more in the planning than the development stages.
#                                                                               
#   @version    2020-09-08  DHF     Added header comment block.                 
#                                                                               
#-------------------------------------------------------------------------------

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


