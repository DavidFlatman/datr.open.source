#!/usr/bin/perl
use strict;
use warnings;
use threads;

# $ scalar
# @ list
# % hash

my $message = "Hello World";
print "$message\n";

my $scalar = "This is a scalar variable";
print "$scalar\n";

my @arrayOfWords = ("This", "is", "an", "array");
print "@arrayOfWords\n";

my %hashTable = ("1", "While", "2", "this", "3", "is", "4", "a hash");
print "$hashTable{'1'} $hashTable{'2'} $hashTable{'3'} $hashTable{'4'}\n";

=begin
Hello world.
All of this is a comment text.
Multiline ends at cut.
=cut

for my $a(0377)
{
    print "octal: ", $a, "\n";
    print $a . $a . " 255 \n";
}

my @list = ("A", "Random", "List");
foreach my $a (@list)
{
    print "a: " . $a . "\n";
}

print "list: @list\n";
print "list size: " . scalar @list . "\n";
print "list max index: " . $#list . "\n";

my %hash;
$hash{"test"} = 1;
print $hash{"test"} . "\n";

sub routine {
    my @list = @_;
    my $size = scalar(@_);

    print "parameter size: $size\n";
    foreach my $item(@_)
    {
        print $item . "\n";
    }
    print "List: @list\n";
}

routine(10,20,30);

my $testingSpecialCharacter = `pwd`;
chomp($testingSpecialCharacter);
print $testingSpecialCharacter . "\n";