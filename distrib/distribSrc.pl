#!/usr/bin/perl -w
# $Id: distribSrc.pl 3938 2009-09-30 20:35:16Z helmut $

sub usage {
  my $msg = shift;
  if ($msg) { print "$msg\n"; }
  print <<EOUSAGE;
distribSrc.pl
Create .zip file for publishing source code
EOUSAGE
  exit;
}


# -------------
# main
{
  my $s = `svn up`;
  my ($rev) = $s =~ m/(\d+)/s;
  my $TGT = "Logoskop_Src_r$rev";
  my $DIR = "/tmp/$TGT";
  my $ZIP = "/tmp/$TGT.zip";

  unlink $ZIP;
  print `rm -rf $DIR && svn export impl $DIR && cd $DIR/.. && zip -r9 $ZIP $TGT/ && ls -l $ZIP`;
}
