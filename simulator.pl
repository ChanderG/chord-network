#! /usr/bin/perl

$command = "gnome-terminal -x bash -c \"./main";

$CONFIG = "conf/config.chord";
open(CONFIG) or die("Could not open config file.");

foreach $line (<CONFIG>) {
  if($line =~ /^\d+\.\d+\.\d+\.\d+ (\d+)$/){
    system($command." $1; read -n1\"");
  }
}

