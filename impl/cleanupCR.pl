#!/usr/bin/perl -w
# $Id: cleanupCR.pl 3014 2008-11-29 10:19:16Z helmut $
# Replace \r\n by \n in directory tree for all files newer than sentinel file.

use Cwd;
use Getopt::Long;
use Logging::Debug;
use File::Find;

my @options = (
  "count",
  "c",
  "list",
  "help",
  "verbose:i",
  "logfile:s",
);
my $gDoList;
my $gDoCount;

sub usage {
  my $msg = shift;
  if ($msg) { print "$msg\n"; }
  my $debugDefault = GetDebugDefault();
  print <<EOUSAGE;
cleanupCR.pl [options] [path...]
Replace \\r\\n by \\n in directory tree for all files newer than sentinel file.

  list                only lists all files containing \\r\\n
  count               reports number of \\r\\n per file
  help                displays this text
  verbose (0|1|2)     sets messages level (errors only | info | debug) (default: $debugDefault)
  logfile <filename>  sets output filename for logging (default: $LosOpt::LOGFILE_DEFAULT)
EOUSAGE
  exit;
}

sub Load($)
{
  my $path = shift or die;
  if (!open(FILE, $path)) {
    MsgError("Load: cannot read file $path");
    return 0;
  }
  MsgDebug("Loading '$path'");
  my $old = $/;
  undef $/;
  my $contents = <FILE>;
  close(FILE);
  $/ = $old;
  return $contents;
}

sub Save($;$)
{
  my $path = shift or die;
  my $contents = shift;
  if (!open(FILE, ">$path")) {
    MsgError("Save: unable to open $path for writing");
    return 0;
  }
  MsgDebug("Saving '$path'");
  print FILE $contents;
  close(FILE);
  return 1;
}

sub ProcessFile
{
  my $path = $File::Find::name;
  return if $path !~ m/\.(cpp|h|rc)\Z/i;

  my $contents = Load($path) or return 0;
  my $count = $contents =~ s/\r\n/\n/g;

  if ($count) {
    MsgInfo( ($gDoCount ? sprintf("%4d ", $count) : "") . $path);
    if (!$gDoList) {
      Save($path, $contents) or return 0;
    }
  }
  undef $contents;
}


# -------------
# main
{
  %opt = ();
  &GetOptions(\%opt, @options) or usage();
  $opt{in} = \@ARGV;

  usage() if $opt{"help"};
  SetDebug($opt{"verbose"}) if defined($opt{"verbose"});
  if (!defined($opt{"logfile"})) {
    $opt{"logfile"} = $LosOpt::LOGFILE_DEFAULT;
  }
  SetLogfile($opt{"logfile"});
  $gDoList = $opt{"list"};
  $gDoCount = $opt{"count"} || $opt{"c"};
  MsgInfo("$0");
  SetStdErrListener();

  find(\&ProcessFile, cwd());
}
