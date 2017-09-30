#!/usr/bin/perl
#use warnings;
# Copyright (C) 2016 YOUQING  . All Rights Reserved.

#if u want debug this file you can set debug value default 1,the debugPrint can print log info
my $debug = 1;

# param 0 :is youqing project info.  eg: DiDi in resource/DiDi
# perl ./MakeCommon/perl_script/yq_prebuild.pl $cust_project 

my $my_cust_project = $ARGV[0];

$_=$my_cust_project;
@para = split(/\./);

my $plat_name = $para[1];
$makeValue{YQCONFIG_PLATFORM_NAME} = $plat_name;
my @yq_auto_add_global_define_by_name;
my @yq_auto_add_global_define_by_number;
my @yq_auto_add_global_define_by_string;

my @defined_marco;
my @defined_marco_value;
my @comment_marco_value;
my $defined_marco_flag = 0;

my $vendorfolder = "./vendoryq";

$defined_marco[$defined_marco_flag] = "YQCONFIG_PLATFORM_NAME";
$defined_marco_value[$defined_marco_flag] = $makeValue{YQCONFIG_PLATFORM_NAME};
$defined_marco_flag++;

my $PLAT_NAME = uc($plat_name);
$defined_marco[$defined_marco_flag] = "YQCONFIG_PLATFORM_NAME_$PLAT_NAME";
$defined_marco_value[$defined_marco_flag] = "yes";
$defined_marco_flag++;

$defined_marco[$defined_marco_flag] = "YQCONFIG_CUSTOM_PRODUCT";
$defined_marco_value[$defined_marco_flag] =  $my_cust_project;
$defined_marco_flag++;

parser_file("./resource/common_makefile/features.mak");
parser_file("./resource/$makeValue{YQCONFIG_PLATFORM_NAME}/$my_cust_project.mak");
parser_file_option("./resource/common_makefile/default_options.mak");
parser_file("./resource/$makeValue{YQCONFIG_PLATFORM_NAME}/$my_cust_project.mak");

&parserMarcoValue();

&CreateMakHeadFile();

#######################################################################################################################################################################

sub debugPrint
{
  if($debug == 1)
  {
    print "DEBUG_PRINT:[1]$_[0]    [2]$_[1]    [3]$_[2]    [4]$_[3]\n";
  }
}

sub CreateMakHeadFile
  {
    my $mak_cfg  = "resource/YqFeatures.mk";
    open  MAK_FILE, ">$mak_cfg" or die "$mak_cfg: $!";
    print MAK_FILE "#this file is auto created by yq_prebuild.pl YOUQING,pls do not modified this file;\n";
    print MAK_FILE "#\n";
    print MAK_FILE "#\n\n";

    for ($count = 0; $count < $defined_marco_flag; $count++)
    {
      if(($defined_marco[$count] !~/YOUQING_APPS/)&&!($defined_marco_value[$count] eq ""))
      {
        print MAK_FILE "#[$defined_marco[$count]]=start=======================================================================================\n";
        if("$defined_marco_value[$count]" eq "yes")
        {
          if(("$defined_marco_value[$count]" eq "yes")&&($defined_marco[$count] =~/YQCONFIG_/))
          {
            print MAK_FILE "$defined_marco[$count] := true\n";
          }
          else
          {
            print MAK_FILE "$defined_marco[$count] := $defined_marco_value[$count]\n";
          }
        }
        elsif("$defined_marco_value[$count]" eq "no")
        {
          if(("$defined_marco_value[$count]" eq "no")&&($defined_marco[$count] =~/YQCONFIG_/))
          {
            print MAK_FILE "$defined_marco[$count] := false\n";
          }
          else
          {
            print MAK_FILE "$defined_marco[$count] := $defined_marco_value[$count]\n";
          }
        }
        else
        {
          print MAK_FILE "$defined_marco[$count] := $defined_marco_value[$count]\n";
        }

        if (("$defined_marco_value[$count]" eq "yes") || ("$defined_marco_value[$count]" eq "no"))
        {
        	push (@yq_auto_add_global_define_by_name,$defined_marco[$count]);
        }

        if ((IsNumeric($defined_marco_value[$count]))&&($defined_marco[$count] =~/YQCONFIG_/))
        {
            push (@yq_auto_add_global_define_by_number,$defined_marco[$count]);
        }

        if (($defined_marco_value[$count] =~/\"(\S+)\"/)&&($defined_marco[$count] =~/YQCONFIG_/))
        {
            push (@yq_auto_add_global_define_by_string,$defined_marco[$count]);   
        }
        
        $_ = $comment_marco_value[$count];
        @para = split(/In /);
        print MAK_FILE "#".$para[0]."\n";
        print MAK_FILE "#".$para[1]."\n";
        print MAK_FILE "#".$para[2]."\n";
        print MAK_FILE "#".$para[3]."\n";
        print MAK_FILE  "# end ========================================================================================\n";
        print MAK_FILE  "\n\n";
        
      }
    }

      print MAK_FILE "\n\n";
      print MAK_FILE "YQ_AUTO_ADD_GLOBAL_DEFINE_BY_NAME := @yq_auto_add_global_define_by_name";
      print MAK_FILE "\n\n";
      print MAK_FILE "YQ_AUTO_ADD_GLOBAL_DEFINE_BY_NUMBER := @yq_auto_add_global_define_by_number";
      print MAK_FILE "\n\n";
      print MAK_FILE "YQ_AUTO_ADD_GLOBAL_DEFINE_BY_STRING := @yq_auto_add_global_define_by_string";
	  print MAK_FILE "\n\n";
	  
    close MAK_FILE;

  }
  
sub  parserMarcoValue
{
 for ($parseDeep = 0; $parseDeep < 30; $parseDeep++)
 {
      for ($count = 0; $count < $defined_marco_flag; $count++)
       {
            $_ = $defined_marco_value[$count] ;
            if (/^\[(\S+)\](\S+)/)
          {
               $defined_marco_value[$count]= $makeValue{$1}.$2;
            }
            if (/^\[(\S+)\]$/)
          {
               $defined_marco_value[$count]= $makeValue{$1};
            }
           if (/(\S+)\[(\S+)\](\S+)/)
          {
               $defined_marco_value[$count]= $1.$makeValue{$2}.$3;
            }
            elsif (/(\S+)\[(\S+)\]/)
          {
               $defined_marco_value[$count]= $1.$makeValue{$2};
            }
       }

  for ($count = 0; $count < $defined_marco_flag; $count++)
       {
            $_ = $defined_marco_value[$count] ;
            my $count1 = () = /\{/g;
            if ($count1>1)
              {
                if ($count1 == 9)
                  {
                    if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                     {
                       $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5.uc($makeValue{$6}).$7.uc($makeValue{$8}).$9.uc($makeValue{$10}).$11.uc($makeValue{$12}).$13.uc($makeValue{$14}).$15.uc($makeValue{$16}).$17.uc($makeValue{$18}).$19;
                      }
                   }
                 if ($count1 == 8)
                 {
                   if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                    {
                      $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5.uc($makeValue{$6}).$7.uc($makeValue{$8}).$9.uc($makeValue{$10}).$11.uc($makeValue{$12}).$13.uc($makeValue{$14}).$15.uc($makeValue{$16}).$17;
                    }
                  }
                 if ($count1 == 7)
                  {
                    if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                     {
                        $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5.uc($makeValue{$6}).$7.uc($makeValue{$8}).$9.uc($makeValue{$10}).$11.uc($makeValue{$12}).$13.uc($makeValue{$14}).$15;
                      }
                    }

                   if ($count1 == 6)
                   {
                     if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                       {
                         $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5.uc($makeValue{$6}).$7.uc($makeValue{$8}).$9.uc($makeValue{$10}).$11.uc($makeValue{$12}).$13;
                       }
                    }
                    if ($count1 == 5)
                     {
                       if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                        {
                           $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5.uc($makeValue{$6}).$7.uc($makeValue{$8}).$9.uc($makeValue{$10}).$11;
                        }
                     }
                    if ($count1 == 4)
                     {
                       if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                         {
                           $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5.uc($makeValue{$6}).$7.uc($makeValue{$8}).$9;
                         }
                     }

                     if ($count1 == 3)
                     {
                       if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                         {
                            $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5.uc($makeValue{$6}).$7;
                          }
                      }

                      if ($count1 == 2)
                      {
                        if (/(.*)\{(\S+)\}(\S+)\{(\S+)\}(.*)/)
                         {
                           $defined_marco_value[$count] =$1.uc($makeValue{$2}).$3.uc($makeValue{$4}).$5;
                         }
                      }
              }
              else
             {
                $_ = $defined_marco_value[$count] ;
                if (/^\{(\S+)\}(\S+)/)
                {
                   $defined_marco_value[$count]= uc($makeValue{$1}).$2;
                 }
                 if (/^\{(\S+)\}$/)
                 {
                    $defined_marco_value[$count]= uc($makeValue{$1});
                 }
                 if (/(\S+)\{(\S+)\}(\S+)/)
                 {
                     $defined_marco_value[$count]= $1.uc($makeValue{$2}).$3;
                 }
                 elsif (/(\S+)\{(\S+)\}/)
                 {
                    $defined_marco_value[$count]= $1.uc($makeValue{$2});
                 }
          }

    $_ = $defined_marco_value[$count] ;
    my $count1 = () = /\[/g;
    if ($count1>1)
    {
       if ($count1 == 9)
       {
          if (/(.*)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](.*)/)
          {
            $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5.lc($makeValue{$6}).$7.lc($makeValue{$8}).$9.lc($makeValue{$10}).$11.lc($makeValue{$12}).$13.lc($makeValue{$14}).$15.lc($makeValue{$16}).$17.lc($makeValue{$18}).$19;
          }
       }
      if ($count1 == 8)
         {
           if (/(.*)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](.*)/)
            {
             $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5.lc($makeValue{$6}).$7.lc($makeValue{$8}).$9.lc($makeValue{$10}).$11.lc($makeValue{$12}).$13.lc($makeValue{$14}).$15.lc($makeValue{$16}).$17;
            }
          }
      if ($count1 == 7)
        {
           if (/(.*)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](.*)/)
            {
             $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5.lc($makeValue{$6}).$7.lc($makeValue{$8}).$9.lc($makeValue{$10}).$11.lc($makeValue{$12}).$13.lc($makeValue{$14}).$15;
            }
        }

       if ($count1 == 6)
         {
            if (/(.*)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](.*)/)
             {
              $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5.lc($makeValue{$6}).$7.lc($makeValue{$8}).$9.lc($makeValue{$10}).$11.lc($makeValue{$12}).$13;
             }
         }
        if ($count1 == 5)
         {
            if (/(.*)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](.*)/)
             {
              $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5.lc($makeValue{$6}).$7.lc($makeValue{$8}).$9.lc($makeValue{$10}).$11;
             }
         }
        if ($count1 == 4)
         {

            if (/(.*)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](.*)/)
             {
              $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5.lc($makeValue{$6}).$7.lc($makeValue{$8}).$9;
             }
         }

        if ($count1 == 3)
         {
            if (/(.*)\[(\S+)\](\S+)\[(\S+)\](\S+)\[(\S+)\](.*)/)
             {
              $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5.lc($makeValue{$6}).$7;
             }
         }

       if ($count1 == 2)
        {
           if (/(.*)\[(\S+)\](\S+)\[(\S+)\](.*)/)
           {
             $defined_marco_value[$count] =$1.lc($makeValue{$2}).$3.lc($makeValue{$4}).$5;
            }
        }
    }
    else
    {
           $_ = $defined_marco_value[$count] ;
            if (/^\[(\S+)\](\S+)/)
          {
               $defined_marco_value[$count]= uc($makeValue{$1}).$2;
            }
            if (/^\[(\S+)\]$/)
          {
               $defined_marco_value[$count]= uc($makeValue{$1});
            }
           if (/(\S+)\[(\S+)\](\S+)/)
          {
               $defined_marco_value[$count]= $1.lc($makeValue{$2}).$3;
            }
            elsif (/(\S+)\[(\S+)\]/)
          {
               $defined_marco_value[$count]= $1.lc($makeValue{$2});
            }

    }

  }

 for ($count = 0; $count < $defined_marco_flag; $count++)
       {
            $_ = $defined_marco[$count] ;
            if (/^\{(\S+)\}(\S+)/)
          {
               $defined_marco[$count]= uc($makeValue{$1}).$2;
            }
            if (/^\{(\S+)\}$/)
          {
               $defined_marco[$count]= uc($makeValue{$1});
            }
           if (/(\S+)\{(\S+)\}(\S+)/)
          {
               $defined_marco[$count]= $1.uc($makeValue{$2}).$3;
            }
            elsif (/(\S+)\{(\S+)\}/)
          {
               $defined_marco[$count]= $1.uc($makeValue{$2});
            }
       }
  for ($count = 0; $count < $defined_marco_flag; $count++)
       {
            $_ = $defined_marco[$count] ;
            if (/^\[(\S+)\](\S+)/)
          {
               $defined_marco[$count]= uc($makeValue{$1}).$2;
            }
            if (/^\[(\S+)\]$/)
          {
               $defined_marco[$count]= uc($makeValue{$1});
            }
           if (/(\S+)\[(\S+)\](\S+)/)
          {
               $defined_marco[$count]= $1.uc($makeValue{$2}).$3;
            }
            elsif (/(\S+)\[(\S+)\]/)
          {
               $defined_marco[$count]= $1.uc($makeValue{$2});
            }
       }

 }#parserDeep

  }


sub parser_file
{
  open  SRC_FILE,  "<$_[0]" or die "$_[0]: $!";
  while(<SRC_FILE>)
  {
     $line = $_;
     $_ = $line;
     s/^\s+|\s+$//g;
     $line=$_;

     if ((/^(\S+)\s*=\s*(\"\S+\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*")/) || (/^(\S+)\s*=\s*(\S+\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*\s*\S*)/))
   {
        $isHave = 65534;
        for ($count=0; $count < $defined_marco_flag; $count++)
        {
           if($defined_marco[$count] eq "$1")
           {
              if($defined_marco_value[$count] eq "$2")
              {
                $isHave = $count;
              }              
              else
              {
                $isHave = $count;
              }
           }
        }
        if ($isHave < $defined_marco_flag)
        {
           if (/^YQCONFIG_COMB_(\S+)\s*=\s*(\S+)/)
             {
                #debugPrint($isHave, $1, $2, $defined_marco[$isHave]);
                $defined_marco[$isHave] = "YQCONFIG_COMB_$1";
                $defined_marco_value[$isHave] = $2;
                $makeValue{$defined_marco[$isHave]} = $2;
                $comment_marco_value[$isHave] = "In $_[0] redefined YQCONFIG_COMB_$1=".$2.$comment_marco_value[$isHave] ;

                $defined_marco[$isHave+1] = "YQCONFIG_COMB_$1_$2";
                $defined_marco_value[$isHave+1] = yes;
                $makeValue{$defined_marco[$isHave+1]} = yes;
                $comment_marco_value[$isHave+1] = "In $_[0] redefined comb marco="."YQCONFIG_COMB_$1_$2". "=1".$comment_marco_value[$isHave+1] ;
             }
           else
            {
              {
                 $defined_marco_value[$isHave] = $2; $makeValue{$defined_marco[$isHave]} = $2;
                 $comment_marco_value[$isHave] = "In $_[0] redefined value=".$2.$comment_marco_value[$isHave] ;

              }
            }
        }
        else
        {


           if (/^YQCONFIG_COMB_(\S+)\s*=\s*(\S+)/)
             {

                $defined_marco[$defined_marco_flag] = "YQCONFIG_COMB_$1";
                $defined_marco_value[$defined_marco_flag] = $2;  $makeValue{$defined_marco[$defined_marco_flag]} = $defined_marco_value[$defined_marco_flag];
                $comment_marco_value[$defined_marco_flag] = "In $_[0] defined YQCONFIG_COMB_$1=".$2.$comment_marco_value[$defined_marco_flag] ;

                $defined_marco[$defined_marco_flag+1] = "YQCONFIG_COMB_$1_$2";
                $defined_marco_value[$defined_marco_flag+1] = yes;    $makeValue{"YQCONFIG_COMB_$1_$2"} = yes;
                $comment_marco_value[$defined_marco_flag+1] = "In $_[0] defined comb marco="."YQCONFIG_COMB_$1_$2". "=1".$comment_marco_value[$defined_marco_flag+1] ;
                $defined_marco_flag++;
                $defined_marco_flag++;
             }
           else
             {
               $defined_marco[$defined_marco_flag] = "$1";
               {
                  $defined_marco_value[$defined_marco_flag] = $2;$makeValue{$defined_marco[$defined_marco_flag]} = $2;
                    $comment_marco_value[$defined_marco_flag] ="In $_[0] defined value=".$2.$comment_marco_value[$defined_marco_flag] ;
                    $_ = $defined_marco[$defined_marco_flag];
                    if (/^YQCONFIG_COMB_(\S+)\s*=\s*(\S+)/)
                  {
                        $defined_marco_flag++;
                        $defined_marco[$defined_marco_flag] = "YQCONFIG_COMB_$1_$2";
                        $defined_marco_value[$defined_marco_flag] = yes;  $makeValue{$defined_marco[$defined_marco_flag]} = yes;
                        $comment_marco_value[$defined_marco_flag] ="In $_[0] defined value="."YQCONFIG_COMB_$1_$2".$comment_marco_value[$defined_marco_flag] ;

                  }
               }
             $defined_marco_flag++;
            }
        }
   }

    if (/^YOUQING_APPS(\S+)\s*=\s*(\S+)/)
    {
      $isHave = 65534;

      #Check for duplicate definitions
      for ($count=0; $count < $defined_marco_flag; $count++)
      {
        if($defined_marco[$count] eq "YOUQING_APPS$1")
        {
          if($defined_marco_value[$count] eq "$2")
          {
            $isHave = $count;
          }
        }
      }
      if ($isHave < $defined_marco_flag)
      {

      }
      else
      {
        $defined_marco[$defined_marco_flag] = "YOUQING_APPS$1";
        {
          $defined_marco_value[$defined_marco_flag] = $2;
          $comment_marco_value[$defined_marco_flag] ="In $_[0] defined value=".$2.$comment_marco_value[$defined_marco_flag] ;
        }
          $defined_marco_flag++;
      }
    }
  }


  close SRC_FILE;
}

sub parser_file_option
{
   my $filename = $_[0];
   open(FILE, $filename ) || die "*** error:  Can't open $filename";
   $lineNo = 1;
   while(<FILE>)
     {
        $line = $_;
        $_ = $line;
        s/^\s+|\s+$//g;$line=$_;
     RESTART1:
        if (($line =~ /^else/i))
          {
             if($nestValue[$nestValueCount] == 0)
               {
                  $nestValueCount--;
                  break;
               }
             else
              {
                 while (<FILE>)
                   {
                      $line = $_;$_ = $line;
                      s/^\s+|\s+$//g;$line=$_;

                      if (($line =~ /^endif/i))
                        {
                           goto RESTART1;
                        }
                   }
               }
          }
        if ((/^(if)\s*(\S+)\s*=\s*(.*)/i))
          {
             if("$makeValue{$2}" eq "$3")
               {
                  $nestValueCount++;
                  $nestValue[$nestValueCount] = 1;
                  break;
               }
             else
               {
                  $nestValueCount++;
                  $nestValue[$nestValueCount] = 0;
                  $countif = 0;
                  while (<FILE>)
                    {
                       $line = $_;$_ = $line;
                       s/^\s+|\s+$//g  ;$line=$_;
                       if (($line =~ /^if/i))
                         {
                            $countif ++;
                         }
                       if (($line =~ /^endif/i))
                         {
                            $countif --;
                         }
                       if (($line =~ /^else/i))
                         {
                           if ($countif == 0)
                             {
                                goto RESTART1;
                             }
                          }
                    }

               }
          }
        if (/^YOUQING_APPS(\S+)\s*=\s*(\S+)/)
        {
            $isHave = 65534;
            for ($count=0; $count < $defined_marco_flag; $count++)
            {
                if($defined_marco[$count] eq "YOUQING_APPS$1")
                {
                    if($defined_marco_value[$count] eq "$2")
                    {
                        $isHave = $count;
                    }
                }
            }
            if ($isHave < $defined_marco_flag)
            {
                $defined_marco_value[$isHave] = $2;
                $comment_marco_value[$isHave] ="In $filename redefined value=".$2.$comment_marco_value[$isHave] ;
            }
            else
            {
                $defined_marco[$defined_marco_flag] = "YOUQING_APPS$1";
                $defined_marco_value[$defined_marco_flag] = $2;
                $comment_marco_value[$defined_marco_flag] ="In $_[0] defined value=".$2.$comment_marco_value[$defined_marco_flag] ;
                $defined_marco_flag++;
            }
        }
        elsif ((/^(\S+)\s*=\s*(\"\S+\s*\S*\s*\S*\s*\S*\")/) || (/^(\S+)\s*=\s*(\S+\s*\S*\s*\S*\s*\S*)/))
        {
             $isHave = 65534;
            for ($count=0; $count < $defined_marco_flag; $count++)
            {
               if($defined_marco[$count] eq "$1")
               {
                  if($defined_marco_value[$count] eq "$2")
                  {
                    $isHave = $count;
                  }
                  elsif((/^CONFIG_COMB_LCD_(\S+)\s*=\s*(\S+)/))
                  {

                  }
                  else
                  {
                    $isHave = $count;
                  }
               }
            }
             if ($isHave < $defined_marco_flag)
               {
                if (/^YQCONFIG_COMB_(\S+)\s*=\s*(\S+)/)
                 {
                    $defined_marco[$isHave] = "YQCONFIG_COMB_$1";
                    $defined_marco_value[$isHave] = $2;  $makeValue{$defined_marco[$isHave]} = $2;
                    $comment_marco_value[$isHave] = "In $_[0] redefined YQCONFIG_COMB_$1=".$2.$comment_marco_value[$isHave] ;

                    $defined_marco[$isHave+1] = "YQCONFIG_COMB_$1_$2";
                    $defined_marco_value[$isHave+1] = yes; $makeValue{$defined_marco[$isHave+1]} = yes;
                    $comment_marco_value[$isHave+1] = "In $_[0] redefined comb marco="."YQCONFIG_COMB_$1_$2". "=1".$comment_marco_value[$isHave+1] ;

                  }
              else
                {
                   $defined_marco_value[$isHave] = $2;  $makeValue{$defined_marco[$isHave]} = $2;
                       $comment_marco_value[$isHave] ="In $filename redefined value=".$2.$comment_marco_value[$isHave] ;
                }
                }
             else
               {
                 if (/^YQCONFIG_COMB_(\S+)\s*=\s*(\S+)/)
                 {
                    $defined_marco[$defined_marco_flag] = "YQCONFIG_COMB_$1";
                    $defined_marco_value[$defined_marco_flag] = $2;   $makeValue{$defined_marco[$defined_marco_flag]} = $2;
                    $comment_marco_value[$defined_marco_flag] = "In $_[0] redefined YQCONFIG_COMB_$1=".$2.$comment_marco_value[$defined_marco_flag] ;

                    $defined_marco[$defined_marco_flag+1] = "YQCONFIG_COMB_$1_$2";
                    $defined_marco_value[$defined_marco_flag+1] = yes;    $makeValue{$defined_marco[$defined_marco_flag+1]} = yes;
                    $comment_marco_value[$defined_marco_flag+1] = "In $_[0] redefined comb marco="."YQCONFIG_COMB_$1_$2". "=1".$comment_marco_value[$defined_marco_flag+1] ;
                    $defined_marco_flag++;
                    $defined_marco_flag++;
                  }
                 else
                 {
                   $defined_marco[$defined_marco_flag] = "$1";
                {
                    $defined_marco_value[$defined_marco_flag] = $2;  $makeValue{$defined_marco[$defined_marco_flag]} = $2;
                        $comment_marco_value[$defined_marco_flag] ="In $_[0] defined value=".$2.$comment_marco_value[$defined_marco_flag] ;
                 }
                  $defined_marco_flag++;
                 }
               }
         }


      }
    close(FILE);
  }

sub IsNumeric()
  {
    my ($t) = @_;
    if ( ($t =~ m/^(\d+\.?\d*|\.\d+)$/))
      {
        return 1;
      }
    else
      {
        return 0;
      }
  }
