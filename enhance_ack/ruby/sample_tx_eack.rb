#! /usr/bin/ruby
# -*- coding: utf-8; mode: ruby -*-
# Function:
#   Lazurite Sub-GHz/Lazurite Pi Gateway Sample program
#   SerialMonitor.rb
#require 'LazGem'
require 'LazGem'

laz = LazGem::Device.new

# Halt process when CTRL+C is pushed.
finish_flag=0
Signal.trap(:INT){
	finish_flag=1
}
if ARGV.size == 0
	printf("please input argument of ch at least\n")
	printf("command format is shown below...\n")
	printf("./sample_tx.rb ch panid dst_short_addr baud pwr\n")
	exit 0
end

# open device deriver
laz.init()

dst_short_addr = 0x408c
ch = 36
panid = 0xabcd
baud = 100
pwr = 20

if ARGV.size > 0
	ch=Integer(ARGV[0])
end
if ARGV.size > 1
	panid = Integer(ARGV[1])
end
if ARGV.size > 2
	dst_short_addr = Integer(ARGV[2])
end
if ARGV.size > 3
	baud = Integer(ARGV[3])
end
if ARGV.size > 4
	pwr = Integer(ARGV[4])
end

i = 0
# main routine
while finish_flag == 0 do
	begin
	laz.begin(ch,panid,baud,pwr)
	rescue Exception => e
		p "file io error!! reset driver"
		laz.remove()
		laz.init()
	end
	begin
		laz.send(panid,dst_short_addr,"LAPIS Lazurite RF system")
		p laz.getEnhanceAck();
	rescue Exception => e
		p e
		sleep 1
	end
	laz.close()
	sleep 1.000
end

# finishing process
laz.remove()


