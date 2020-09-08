package Cpp::FileInternal;
use strict;
use warnings;

#-------------------------------------------------------------------------------
#                                                                               
#@file  FileInternal.pm                                                         
#                                                                               
#@brief Parse a single C++ code file; don't rifle through include files.        
#                                                                               
#@version   2019-01-30  DHF     File creation.                                  
#                                                                               
#-------------------------------------------------------------------------------

our @EXPORT_OK = qw (open debug token_get token_fast_forward token_if_next);
our @EXPORT    = qw (open debug token_get token_fast_forward token_if_next);

#-------------------------------------------------------------------------------
#@brief Create a new C++ file parsing object.                                   
#-------------------------------------------------------------------------------
sub new
{
    my ($class) = @_;

    my $this = {};
    my $result = bless $this, "Cpp::FileInternal";

    $this->{"line-number"} = 0;

    return $result;
}

#-------------------------------------------------------------------------------
#@brief Open a C++ file for parsing.                                            
#-------------------------------------------------------------------------------
sub open()
{
    my ($this, $path) = @_;

    $this->{"path"} = $path;

    open(FILE_HANDLE, $path) or die "Unable to open ${path}\n$!\n";
    @{$this->{"lines"}} = <FILE_HANDLE>;
    close(FILE_HANDLE);

    $this->{"line-number"} = 0;
    $this->{"size"} = $#{$this->{"lines"}} + 1;
}



#-------------------------------------------------------------------------------
#@brief Skip to the matching #endif                                             
#                                                                               
#@par Expected Usage:                                                           
#       @code                                                                   
#           my $file = Cpp::File::new();                                        
#                                                                               
#           while (defined($token = $file->token_get))                          
#           {                                                                   
#               if ($token eq "#ifdef")                                         
#               {                                                               
#                   $next = $file->token_get;                                   
#                   if (!defined($definitions{$next}))                          
#                   {                                                           
#                       $file->skip_to_endif;                                   
#                   }                                                           
#               }                                                               
#           }                                                                   
#       @endcode                                                                
#                                                                               
#@pre   It is expected that the #if|#ifdef|#ifndef has already been pulled      
#       from the stream and we're now skipping to the matching #endif           
#-------------------------------------------------------------------------------
sub skip_to_endif
{
    my ($this) = @_;
    my $level = 1;
    my $token;

    while ($level > 0 and $token = $this->token_get)
    {
        if ($token eq "#endif")
        {
            --$level;
        } elsif ($token eq "#if" or $token eq "#ifndef" or $token eq "#ifdef")
        {
            ++$level;
        }
    }
}

#-------------------------------------------------------------------------------
#@brief Return a token if it should not have been taken from the input.         
#-------------------------------------------------------------------------------
sub token_put
{
    my ($this, $token) = @_;

    push @{$this->{"tokens"}}, $token;
}


#-------------------------------------------------------------------------------
#@brief Layered on top of token_get_internal, consider /* comments */ a single  
#       token.                                                                  
#@param $this           Pointer to this objects (standard Perl OO).             
#@param $allow_white    By default, white space is supressed, pass in a true    
#                       value and white space will also be returned.            
#-------------------------------------------------------------------------------
sub token_get
{
    my ($this, $allow_white) = @_;

    if (not defined($allow_white))
    {
        $allow_white = 0;
    }

    my $result;
    
    #---------------------------------------------------------------------------
    #   skip over white space                                                   
    #---------------------------------------------------------------------------
    if ($allow_white)
    {
        $result = $this->token_get_internal;
    } else {

        $result = $this->token_get_internal;
        while (defined($result) and ($result =~ /^\s*$/ or $result eq "\n"))
        {
            $result = $this->token_get_internal;
        }

    }

    if (defined($result) and $result eq "/*")
    {
        my $token = "";
        my $line = $this->{"line-number"};

        while ($token ne "*/")
        {
            $token = $this->token_get_internal;
            while ($line < $this->{"line-number"})
            {
                $result .= "\n";
                ++$line;
            }
            $result .= $token;
        }
    }

    return $result;
}


#-------------------------------------------------------------------------------
#@brief Get a token                                                             
#-------------------------------------------------------------------------------
sub token_get_internal
{
    my ($this) = @_;

    my $result;

    if ($#{$this->{"tokens"}} == -1)
    {
        my $line = ${$this->{"lines"}}[$this->{"line-number"}];
        ++$this->{"line-number"};

        if (not defined($line)) 
        {
            return;
        }
        chomp($line);

        while (length($line) > 0)
        {
            if ($line =~ /^(\s+)(.*)/)
            {
                #---------------------------------------------------------------
                #   white space                                                 
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1);

            } elsif ($line =~ /^(\w+)(.*)/)
            {
                #---------------------------------------------------------------
                #   Word class characters (a-zA-Z0-9_)                          
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1);

            } elsif ($line =~ /^\/\//) 
            {
                #---------------------------------------------------------------
                #   Rest of the line is a comment ...                           
                #---------------------------------------------------------------
                $this->token_put($line);
                $line = "";

            } elsif ($line =~ /^("(?:[^"\\]|\\.)*")(.*)/)
            {
                #---------------------------------------------------------------
                #   Handle literal "string" and "this is \"quoted\"."           
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1);

            } elsif ($line =~ /^([\|\.\+\-\=\!\%\^\&\*\>\<\/:]+)(.*)/)
            {
                #---------------------------------------------------------------
                #   Potentialy multiples: & or &&                               
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1);

            } elsif ($line =~ /^([\[\]\(\)\{\}\,\~\;\?\\])(.*)/) 
            {
                #---------------------------------------------------------------
                #   Singles                                                     
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1);

            } elsif ($line =~ /(#\w+)(.*)/)
            {
                $line = $2;
                $this->token_put($1);
                
            } elsif ($line =~ /^(.)(.*)/) 
            {
                print "unhandled symbol: '" . $1 . "'\n";
                $line = $2;
            }

        } # while (length($line) > 0)
        $this->token_put("\n");
    }

    if ($#{$this->{"tokens"}} > -1)
    {
        $result = shift @{$this->{"tokens"}};
    }

    return $result;

} # sub token_get_internal


#-------------------------------------------------------------------------------
#@brief Fast forward the input tokens until we find the given stream of tokens. 
#-------------------------------------------------------------------------------
sub token_fast_forward
{
    my ($this, @looking4) = @_;
    my @have;

    my $match = 0;

    while ($match == 0)
    {
        while ($#have < $#looking4) 
        {
            my $token = $this->token_get;
            if (!defined($token))
            {
                #---------------------------------------------------------------
                #   End of file                                                 
                #---------------------------------------------------------------
                return 0;
            }
            push @have, $token;
        }

        $match = 1;
        for (my $a = 0; $a <= $#looking4 and $match == 1; ++$a)
        {
            if ($have[$a] ne $looking4[$a])
            {
                $match = 0;
            }
        }
        if ($match == 0) { shift @have; }

    }

    return 1;

} # sub token_fast_forward


#-------------------------------------------------------------------------------
#@brief If the next tokens are the ones specified, remove them from the stream  
#       and return 1, otherwise leave the stream alone and return 0.            
#-------------------------------------------------------------------------------
sub token_if_next
{
    my ($this, @looking4) = @_;

    my @tokens;

    my $count = 0;

    while (
            $count <= $#looking4 
     and    defined($tokens[$count] = $this->token_get)
     and    $tokens[$count] eq $looking4[$count]
    )
    {
        ++$count;
    }

    my $result = 0;

    if ($count <= $#looking4)
    {
        for (my $i = 0; $i < $count; ++$i)
        {
            $this->token_put($tokens[$i]);
        }
    } else {
        $result = 1;
    }

    return $result;

} # sub token_if_next


#-------------------------------------------------------------------------------
#@brief Developer debug routine.                                                
#-------------------------------------------------------------------------------
sub debug
{
    my ($this) = @_;

    print "path: " . $this->{"path"} . "\n";
    print "size: " . ($#{$this->{"lines"}} + 1) . "\n";

    for(my $a = 0; $a <= $#{$this->{"lines"}}; ++$a)
    {
        printf "line %03d:  %s", $a + 1, ${$this->{"lines"}}[$a];
    }


}



1;
