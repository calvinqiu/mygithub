#!/usr/bin/perl
BEGIN{push @INC,'./MakeCommon/perl_script/perl_pm'};
use Cwd;
$myPath = getcwd;
if($myPath =~ /^\/cygdrive\//)
{
	$myPath =~ s/^\/cygdrive\///g;
	$myPath =~ s/\//\:\//;
}
print $myPath;