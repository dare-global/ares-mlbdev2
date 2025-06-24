#!/usr/bin/perl
# #############################################################################
# #############################################################################
# MLB Utility Program
# #############################################################################
#
# File Name       : CheckFileLineSums.pl
#
# File Description: Check per-line checksums for lines in a file.
#
# Revision History: 2022-11-03 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2025.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

use strict;
use warnings;

# #############################################################################
	if (grep(/^\-\-?H$|\-\-?HELP$/i, @ARGV)) {
		EmitHelp();
	}

	if (($#ARGV < 0) || ($#ARGV > 1)) {
		ReportFatalError("Invalid command line -- use '-h' for help.");
	}

	my $return_code      = 0;

	my $CFLS_FileName    = "";
	my $CFLS_LineNumbers = -1;

	$CFLS_FileName = $ARGV[0];

	if ($#ARGV == 1) {
		if ($ARGV[1] !~ /^\d+$/) {
			ReportFatalError("Invalid line number parameter ('" .
				$ARGV[1] . "').");
		}
		$CFLS_LineNumbers = ($ARGV[1] + 0);
		if ($CFLS_LineNumbers < 1) {
			ReportFatalError("Invalid line number parameter ('" .
				$ARGV[1] . "'): minimum value is '1'.");
		}
	}

	if (!open(IN_FILE, "<$CFLS_FileName")) {
		ReportFatalError("Unable to open file '" . $CFLS_FileName .
			"' for reading.");
	}

	my @line_list = <IN_FILE>;
	my $line_number;
	chop(@line_list);
	close(IN_FILE);

	if ($CFLS_LineNumbers == -1) {
		for ($line_number = 0; $line_number <= $#line_list; ++$line_number) {
			my @results_list = `echo '$line_list[$line_number]' | sum`;
			ExitIfSystemError("Unable to calculate a checksum for line number " .
				($line_number + 1) . ".");
			chop @results_list;
			printf("%10d: ", $line_number + 1);
			print $results_list[0], "\n";
		}
	}
	elsif (($CFLS_LineNumbers - 1) > $#line_list) {
		ReportFatalError("The specified line number parameter ('" .
			$CFLS_LineNumbers . "') exceeds the number of lines in the file ('" .
			($#line_list + 1) + "').");
	}
	elsif (!length($line_list[$CFLS_LineNumbers - 1])) {
		printf("%10d: %6d / %6d: 00000     1\n", $CFLS_LineNumbers, 0, 0);
	}
	else {
		$line_number    = $CFLS_LineNumbers - 1;
		my $src_line    = $line_list[$line_number];
		my $src_length  = length($src_line);
		my $start_idx;
		for ($start_idx = 0; $start_idx < $src_length; $start_idx += 10) {
			my $tmp_part;
			if (($src_length - $start_idx) >= 10) {
				$tmp_part = substr($src_line, $start_idx, 10);
			}
			else {
				$tmp_part = substr($src_line, $start_idx);
			}
			my @results_list = `echo '$tmp_part' | sum`;
			ExitIfSystemError("Unable to calculate a checksum for line number " .
				($line_number + 1) . " beginning at offset " . $start_idx . ".");
			chop @results_list;
			printf("%10d: %6d / %6d: ",
				$line_number + 1, $start_idx, length($tmp_part));
			print $results_list[0], ": [", $tmp_part, "]\n";
		}
	}

	exit($return_code);
# #############################################################################

# #############################################################################
sub EmitHelp {
	print "Help request with \'-HELP\' noted . . .\n\n";

	my $tmp_string =  $0;
	$tmp_string    =~ s/^(.+)\/(.+)$/$2/;

	print "   ", $tmp_string, " <file-name> [ <line-number> ]\n\n";

	exit(0);
}
# #############################################################################

# #############################################################################
sub ExitIfSystemError {
	my($in_error_text) = @_;
	my($tmp_error_text);

	if ($? == 0) {
		return;
	}

	$tmp_error_text = $in_error_text .
		": A system() invocation file with error code " . $? . ": " . $!;

	print STDERR $tmp_error_text, "\n";

	exit(-1);
}
# #############################################################################

# #############################################################################
sub ReportFatalError {
	my($error_text) = @_;

	print "ERROR: ", $error_text, "\n";

	exit(-1);
}
# #############################################################################

