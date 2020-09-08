package Cpp::File;
use strict;
use warnings;

#-------------------------------------------------------------------------------
#                                                                               
#@file  File.pm                                                                 
#                                                                               
#@brief Parse a C++ code file.                                                  
#                                                                               
#@version   2019-01-30  DHF     File creation.                                  
#                                                                               
#-------------------------------------------------------------------------------

our @EXPORT_OK = qw (open debug token_get token_fast_forward token_if_next);
our @EXPORT    = qw (open debug token_get token_fast_forward token_if_next);

my %access = (
    "public", 1
  , "protected", 1
  , "private", 1
);

my %statements = (
    "new", 1 
  , "delete", 1 
  , "this", 1 
  , "friend", 1 
  , "using", 1
  , "throw", 1 
  , "try", 1 
  , "catch", 1
  , "inline", 1 
  , "virtual", 1 
  , "explicit", 1 
  , "export", 1
  , "mutable", 1
  , "class", 1 
  , "typename", 1 
  , "template", 1 
  , "namespace", 1
  , "override", 1 
  , "final", 1
  , "noexcept", 1
  , "goto", 1 
  , "break", 1 
  , "return", 1 
  , "continue", 1 
  , "asm", 1
  , "case", 1 
  , "default", 1
  , "if", 1 
  , "else", 1 
  , "switch", 1
  , "while", 1 
  , "for", 1 
  , "do", 1
  , "struct", 1 
  , "union", 1 
  , "enum", 1 
  , "typedef", 1
  , "__asm__", 1
);

my %types = (
    "wchar_t", 1
  , "constexpr", 1 
  , "decltype", 1 
  , "thread_local", 1
  , "int", 1 
  , "long", 1 
  , "short", 1 
  , "char", 1 
  , "void", 1
  , "signed", 1 
  , "unsigned", 1 
  , "float", 1 
  , "double", 1
  , "size_t", 1 
  , "ssize_t", 1 
  , "off_t", 1 
  , "wchar_t", 1 
  , "ptrdiff_t", 1 
  , "sig_atomic_t", 1 
  , "fpos_t", 1
  , "clock_t", 1 
  , "time_t", 1 
  , "va_list", 1 
  , "jmp_buf", 1 
  , "FILE", 1 
  , "DIR", 1 
  , "div_t", 1 
  , "ldiv_t", 1
  , "mbstate_t", 1 
  , "wctrans_t", 1 
  , "wint_t", 1 
  , "wctype_t", 1
  , "_Bool", 1 
  , "bool", 1 
  , "_Complex", 1 
  , "complex", 1 
  , "_Imaginary", 1 
  , "imaginary", 1
  , "int8_t", 1 
  , "int16_t", 1 
  , "int32_t", 1 
  , "int64_t", 1
  , "uint8_t", 1 
  , "uint16_t", 1 
  , "uint32_t", 1 
  , "uint64_t", 1
  , "int_least8_t", 1 
  , "int_least16_t", 1 
  , "int_least32_t", 1 
  , "int_least64_t", 1
  , "uint_least8_t", 1 
  , "uint_least16_t", 1 
  , "uint_least32_t", 1 
  , "uint_least64_t", 1
  , "int_fast8_t", 1 
  , "int_fast16_t", 1 
  , "int_fast32_t", 1 
  , "int_fast64_t", 1
  , "uint_fast8_t", 1 
  , "uint_fast16_t", 1 
  , "uint_fast32_t", 1 
  , "uint_fast64_t", 1
  , "intptr_t", 1 
  , "uintptr_t", 1
  , "intmax_t", 1 
  , "uintmax_t", 1
  , "__label__", 1 
  , "__complex__", 1 
  , "__volatile__", 1
  , "char16_t", 1 
  , "char32_t", 1
  , "nullptr_t", 1 
  , "auto", 1
  , "static", 1 
  , "register", 1 
  , "auto", 1 
  , "volatile", 1 
  , "extern", 1 
  , "const", 1
  , "inline", 1 
  , "__attribute__", 1
  , "inline", 1 
  , "restrict", 1
  , "_Alignas", 1 
  , "alignas", 1
  , "_Atomic", 1
  , "_Noreturn", 1 
  , "noreturn", 1
  , "_Thread_local", 1 
  , "thread_local", 1
);


my %operators = (
    "operator", 1 
  , "typeid", 1
  , "and", 1 
  , "bitor", 1 
  , "or", 1 
  , "xor", 1 
  , "compl", 1 
  , "bitand", 1 
  , "and_eq", 1 
  , "or_eq", 1 
  , "xor_eq", 1 
  , "not", 1 
  , "not_eq", 1
  , "sizeof", 1
  , "_Alignof", 1 
  , "alignof", 1
  , "_Generic", 1
  , "_Static_assert", 1 
  , "static_assert", 1
  , "typeof", 1 
  , "__real__", 1 
  , "__imag__", 1
);

my %constants = (
    "__cplusplus", 1
  , "nullptr", 1
  , "ATOMIC_FLAG_INIT", 1 
  , "ATOMIC_VAR_INIT", 1
  , "ATOMIC_BOOL_LOCK_FREE", 1 
  , "ATOMIC_CHAR_LOCK_FREE", 1
  , "ATOMIC_CHAR16_T_LOCK_FREE", 1 
  , "ATOMIC_CHAR32_T_LOCK_FREE", 1
  , "ATOMIC_WCHAR_T_LOCK_FREE", 1 
  , "ATOMIC_SHORT_LOCK_FREE", 1
  , "ATOMIC_INT_LOCK_FREE", 1 
  , "ATOMIC_LONG_LOCK_FREE", 1
  , "ATOMIC_LLONG_LOCK_FREE", 1 
  , "ATOMIC_POINTER_LOCK_FREE", 1
  , "__GNUC__", 1 
  , "__FUNCTION__", 1 
  , "__PRETTY_FUNCTION__", 1 
  , "__func__", 1
  , "__LINE__", 1 
  , "__FILE__", 1 
  , "__DATE__", 1 
  , "__TIME__", 1 
  , "__STDC__", 1
  , "__STDC_VERSION__", 1
  , "CHAR_BIT", 1 
  , "MB_LEN_MAX", 1 
  , "MB_CUR_MAX", 1
  , "UCHAR_MAX", 1 
  , "UINT_MAX", 1 
  , "ULONG_MAX", 1 
  , "USHRT_MAX", 1
  , "CHAR_MIN", 1 
  , "INT_MIN", 1 
  , "LONG_MIN", 1 
  , "SHRT_MIN", 1
  , "CHAR_MAX", 1 
  , "INT_MAX", 1 
  , "LONG_MAX", 1 
  , "SHRT_MAX", 1
  , "SCHAR_MIN", 1 
  , "SINT_MIN", 1 
  , "SLONG_MIN", 1 
  , "SSHRT_MIN", 1
  , "SCHAR_MAX", 1 
  , "SINT_MAX", 1 
  , "SLONG_MAX", 1 
  , "SSHRT_MAX", 1
  , "__func__", 1
  , "LLONG_MIN", 1 
  , "LLONG_MAX", 1 
  , "ULLONG_MAX", 1
  , "INT8_MIN", 1 
  , "INT16_MIN", 1 
  , "INT32_MIN", 1 
  , "INT64_MIN", 1
  , "INT8_MAX", 1 
  , "INT16_MAX", 1 
  , "INT32_MAX", 1 
  , "INT64_MAX", 1
  , "UINT8_MAX", 1 
  , "UINT16_MAX", 1 
  , "UINT32_MAX", 1 
  , "UINT64_MAX", 1
  , "INT_LEAST8_MIN", 1 
  , "INT_LEAST16_MIN", 1 
  , "INT_LEAST32_MIN", 1 
  , "INT_LEAST64_MIN", 1
  , "INT_LEAST8_MAX", 1 
  , "INT_LEAST16_MAX", 1 
  , "INT_LEAST32_MAX", 1 
  , "INT_LEAST64_MAX", 1
  , "UINT_LEAST8_MAX", 1 
  , "UINT_LEAST16_MAX", 1 
  , "UINT_LEAST32_MAX", 1 
  , "UINT_LEAST64_MAX", 1
  , "INT_FAST8_MIN", 1 
  , "INT_FAST16_MIN", 1 
  , "INT_FAST32_MIN", 1 
  , "INT_FAST64_MIN", 1
  , "INT_FAST8_MAX", 1 
  , "INT_FAST16_MAX", 1 
  , "INT_FAST32_MAX", 1 
  , "INT_FAST64_MAX", 1
  , "UINT_FAST8_MAX", 1 
  , "UINT_FAST16_MAX", 1 
  , "UINT_FAST32_MAX", 1 
  , "UINT_FAST64_MAX", 1
  , "INTPTR_MIN", 1 
  , "INTPTR_MAX", 1 
  , "UINTPTR_MAX", 1
  , "INTMAX_MIN", 1 
  , "INTMAX_MAX", 1 
  , "UINTMAX_MAX", 1
  , "PTRDIFF_MIN", 1 
  , "PTRDIFF_MAX", 1 
  , "SIG_ATOMIC_MIN", 1 
  , "SIG_ATOMIC_MAX", 1
  , "SIZE_MAX", 1 
  , "WCHAR_MIN", 1 
  , "WCHAR_MAX", 1 
  , "WINT_MIN", 1 
  , "WINT_MAX", 1
  , "FLT_RADIX", 1 
  , "FLT_ROUNDS", 1
  , "FLT_DIG", 1 
  , "FLT_MANT_DIG", 1 
  , "FLT_EPSILON", 1
  , "DBL_DIG", 1 
  , "DBL_MANT_DIG", 1 
  , "DBL_EPSILON", 1
  , "LDBL_DIG", 1 
  , "LDBL_MANT_DIG", 1 
  , "LDBL_EPSILON", 1
  , "FLT_MIN", 1 
  , "FLT_MAX", 1 
  , "FLT_MIN_EXP", 1 
  , "FLT_MAX_EXP", 1
  , "FLT_MIN_10_EXP", 1 
  , "FLT_MAX_10_EXP", 1
  , "DBL_MIN", 1 
  , "DBL_MAX", 1 
  , "DBL_MIN_EXP", 1 
  , "DBL_MAX_EXP", 1
  , "DBL_MIN_10_EXP", 1 
  , "DBL_MAX_10_EXP", 1
  , "LDBL_MIN", 1 
  , "LDBL_MAX", 1 
  , "LDBL_MIN_EXP", 1 
  , "LDBL_MAX_EXP", 1
  , "LDBL_MIN_10_EXP", 1 
  , "LDBL_MAX_10_EXP", 1
  , "HUGE_VAL", 1 
  , "CLOCKS_PER_SEC", 1 
  , "NULL", 1
  , "LC_ALL", 1 
  , "LC_COLLATE", 1 
  , "LC_CTYPE", 1 
  , "LC_MONETARY", 1
  , "LC_NUMERIC", 1 
  , "LC_TIME", 1
  , "SIG_DFL", 1 
  , "SIG_ERR", 1 
  , "SIG_IGN", 1
  , "SIGABRT", 1 
  , "SIGFPE", 1 
  , "SIGILL", 1 
  , "SIGHUP", 1 
  , "SIGINT", 1 
  , "SIGSEGV", 1 
  , "SIGTERM", 1
  , "SIGABRT", 1 
  , "SIGALRM", 1 
  , "SIGCHLD", 1 
  , "SIGCONT", 1 
  , "SIGFPE", 1 
  , "SIGHUP", 1
  , "SIGILL", 1 
  , "SIGINT", 1 
  , "SIGKILL", 1 
  , "SIGPIPE", 1 
  , "SIGQUIT", 1 
  , "SIGSEGV", 1
  , "SIGSTOP", 1 
  , "SIGTERM", 1 
  , "SIGTRAP", 1 
  , "SIGTSTP", 1 
  , "SIGTTIN", 1 
  , "SIGTTOU", 1
  , "SIGUSR1", 1 
  , "SIGUSR2", 1
  , "_IOFBF", 1 
  , "_IOLBF", 1 
  , "_IONBF", 1 
  , "BUFSIZ", 1 
  , "EOF", 1 
  , "WEOF", 1
  , "FOPEN_MAX", 1 
  , "FILENAME_MAX", 1 
  , "L_tmpnam", 1
  , "SEEK_CUR", 1 
  , "SEEK_END", 1 
  , "SEEK_SET", 1
  , "TMP_MAX", 1 
  , "stderr", 1 
  , "stdin", 1 
  , "stdout", 1
  , "EXIT_FAILURE", 1 
  , "EXIT_SUCCESS", 1 
  , "RAND_MAX", 1
  , "E2BIG", 1 
  , "EACCES", 1 
  , "EAGAIN", 1 
  , "EBADF", 1 
  , "EBADMSG", 1 
  , "EBUSY", 1
  , "ECANCELED", 1 
  , "ECHILD", 1 
  , "EDEADLK", 1 
  , "EDOM", 1 
  , "EEXIST", 1 
  , "EFAULT", 1
  , "EFBIG", 1 
  , "EILSEQ", 1 
  , "EINPROGRESS", 1 
  , "EINTR", 1 
  , "EINVAL", 1 
  , "EIO", 1 
  , "EISDIR", 1
  , "EMFILE", 1 
  , "EMLINK", 1 
  , "EMSGSIZE", 1 
  , "ENAMETOOLONG", 1 
  , "ENFILE", 1 
  , "ENODEV", 1
  , "ENOENT", 1 
  , "ENOEXEC", 1 
  , "ENOLCK", 1 
  , "ENOMEM", 1 
  , "ENOSPC", 1 
  , "ENOSYS", 1
  , "ENOTDIR", 1 
  , "ENOTEMPTY", 1 
  , "ENOTSUP", 1 
  , "ENOTTY", 1 
  , "ENXIO", 1 
  , "EPERM", 1
  , "EPIPE", 1 
  , "ERANGE", 1 
  , "EROFS", 1 
  , "ESPIPE", 1 
  , "ESRCH", 1 
  , "ETIMEDOUT", 1 
  , "EXDEV", 1
  , "M_E", 1 
  , "M_LOG2E", 1 
  , "M_LOG10E", 1 
  , "M_LN2", 1 
  , "M_LN10", 1 
  , "M_PI", 1 
  , "M_PI_2", 1 
  , "M_PI_4", 1
  , "M_1_PI", 1 
  , "M_2_PI", 1 
  , "M_2_SQRTPI", 1 
  , "M_SQRT2", 1 
  , "M_SQRT1_2", 1
  , "true", 1 
  , "false", 1
);

#-------------------------------------------------------------------------------
#@brief Create a new C++ file parsing object.                                   
#-------------------------------------------------------------------------------
sub new
{
    my ($class) = @_;

    my $this = {};
    my $result = bless $this, "Cpp::File";

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
    my $type;

    while ($level > 0 and ($token, $type) = $this->token_get)
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
    my ($this, $token, $type) = @_;

    push @{$this->{"tokens"}}, $token;
    push @{$this->{"token-types"}}, $type;
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
    my $type = "undefined";
    
    #---------------------------------------------------------------------------
    #   skip over white space                                                   
    #---------------------------------------------------------------------------
    if ($allow_white)
    {
        ($result, $type) = $this->token_get_internal;

    } else {
        ($result, $type) = $this->token_get_internal;
        while (defined($result) and ($result =~ /^\s*$/ or $result eq "\n"))
        {
            ($result, $type) = $this->token_get_internal;
        }

    }

    if (defined($result) and $result eq "/*")
    {
        my $token = "";
        my $line = $this->{"line-number"};

        while ($token ne "*/")
        {
            ($token, $type) = $this->token_get_internal;
            while ($line < $this->{"line-number"})
            {
                $result .= "\n";
                ++$line;
            }
            $result .= $token;
        }
        $type = "comment";

    } 
    
    #---------------------------------------------------------------------------
    #   If the last result was a #include and this one is an < then we have a   
    #   string terminated by >.                                                 
    #---------------------------------------------------------------------------
    elsif ( defined($result) 
        and $result eq "<" 
        and $this->{"last-result"} eq "#include"
    )
    {
        my $temp = "";
        while (($temp, $type) = $this->token_get_internal, $temp ne ">")
        {
            $result .= $temp;
        }
        $result .= ">";
        $type = "string";
    }

    #---------------------------------------------------------------------------
    #   Save off the result (last-result) in case or context dependendcies.     
    #---------------------------------------------------------------------------
    if (defined($type) and $type ne "white-space")
    {
        $this->{"last-result"} = $result;
        $this->{"last-type"}   = $type;
    }

    return ($result, $type);

} # sub token_get


#-------------------------------------------------------------------------------
#@brief Get a token                                                             
#-------------------------------------------------------------------------------
sub token_get_internal
{
    my ($this) = @_;

    my $result;
    my $type;

    my $prev_type  = "";
    my $prev_token = "";

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
                $this->token_put($1, "white-space");

            } elsif ($line =~ /^(\w+)(.*)/)
            {
                #---------------------------------------------------------------
                #   Word class characters (a-zA-Z0-9_)                          
                #---------------------------------------------------------------
                $line = $2;
                my $temp1 = $1;
                my $temp2 = "identifier";

                if    (defined($access{$temp1}))       { $temp2 = "access"; }
                elsif (defined($statements{$temp1}))   { $temp2 = "statement"; }
                elsif (defined($types{$temp1}))        { $temp2 = "type"; }
                elsif (defined($operators{$temp1}))    { $temp2 = "operator"; }
                elsif (defined($constants{$temp1}))    { $temp2 = "constant"; }
#print "DEBUG: token = " . ${token} . ";  type = " . ${type} . "\n";
#                $this->token_put($token, $type);
                $this->token_put($temp1, $temp2);

            } elsif ($line =~ /^\/\//) 
            {
                #---------------------------------------------------------------
                #   Rest of the line is a comment ...                           
                #---------------------------------------------------------------
                $this->token_put($line, "comment");
                $line = "";

            } elsif ($line =~ /^("(?:[^"\\]|\\.)*")(.*)/)
            {
                #---------------------------------------------------------------
                #   Handle literal "string" and "this is \"quoted\"."           
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1, "string");

            } elsif ($line =~ /^('(?:[^'\\]|\\.)*')(.*)/)
            {
                #---------------------------------------------------------------
                #   Handle literal single character string: 'a' '\'' '\0'       
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1, "string");

            } elsif ($line =~ /^([\|\.\+\-\=\!\%\^\&\*\>\<\/:]+)(.*)/)
            {
                #---------------------------------------------------------------
                #   Potentialy multiples: & or &&                               
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1, "operator");

            } elsif ($line =~ /^([\[\]\(\)\{\}\,\~\;\?\\])(.*)/) 
            {
                #---------------------------------------------------------------
                #   Singles                                                     
                #---------------------------------------------------------------
                $line = $2;
                $this->token_put($1, "operator");

            } elsif ($line =~ /(#\w+)(.*)/)
            {
                $line = $2;
                $this->token_put($1, "preprocessor");
                
            } elsif ($line =~ /^(.)(.*)/) 
            {
                print "unhandled symbol: '" . $1 . "' at line " . $this->{"line-number"} . "\n";
                $line = $2;

            }

        } # while (length($line) > 0)
        $this->token_put("\n", "new-line");
    }

    if ($#{$this->{"tokens"}} > -1)
    {
        $result = shift @{$this->{"tokens"}};
        $type   = shift @{$this->{"token-types"}};

    }

    return ($result, $type);

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
            my ($token, $token_type) = $this->token_get;
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
    my @types;

    my $count = 0;

    while (
            $count <= $#looking4 
     and    (($tokens[$count],$types[$count]) = $this->token_get, defined($tokens[$count]))
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
            $this->token_put($tokens[$i], $types[$i]);
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
