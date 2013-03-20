#!/usr/bin/ruby -w

=begin
Scheduling Algorithms Trace Event Preprocessor

Description: Takes a CSV file with two columns
  (event time and event type), formatted with the
  QNX Momentics Trace Event profiler.

Usage: 
  ./preprocess test_in

=end

require 'csv'

CSV.open(ARGV[0] + "-out", 'w') do |csv_out|
  CSV.foreach(ARGV[0], 'r') do |row|
    row[0].gsub!(/[mus\s]/, '')
    row[1].gsub!(/[\D]/, '')
    csv_out << row
  end
end

