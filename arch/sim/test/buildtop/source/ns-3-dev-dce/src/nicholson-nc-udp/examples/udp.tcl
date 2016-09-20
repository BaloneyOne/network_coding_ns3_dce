set k [lindex $argv 0]
set ns [new Simulator]

set tracefile [open out.tr w]
$ns trace-all $tracefile
set nf [open out.nam w]
$ns namtrace-all $nf

set source [$ns node]
set udpEncoder [$ns node]
set udpDecoder [$ns node]
set sink [$ns node]


$ns duplex-link $source $udpEncoder 2Mb 2.5ms DropTail
$ns duplex-link $udpEncoder $udpDecoder 2Mb 25ms DropTail
$ns duplex-link $udpDecoder $sink 2Mb 25ms DropTail


$ns duplex-link-op $source $udpEncoder orient right
$ns duplex-link-op $udpEncoder $udpDecoder orient right
$ns duplex-link-op $udpDecoder $sink orient right

set p1 [new Agent/UDPEncoder]
$p1 set blksize $k
$ns attach-agent $udpEncoder $p1

set p2 [new Agent/UDPDecoder]
$p2 set blksize $k
$ns attach-agent $udpDecoder $p2


set udp0 [new Agent/UDP]     
$udp0 set fid_ 1
set null0 [new Agent/LossMonitor]
$ns attach-agent $source $udp0   
$ns attach-agent $sink $null0  


set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 1000             
$cbr0 set rate_ 500Kb
$cbr0 set type_ CBR
$cbr0 attach-agent $udp0	

$ns connect $udp0 $p1
$ns connect $p1 $p2
$ns connect $p2 $null0


#set em2 [new ErrorModel/Uniform 0.2 pkt]
#set em4 [new ErrorModel/Expo 4 2]
#$em4 set rate_ 0.5
#$ns lossmodel $em2 $udpEncoder $udpDecoder

set f_udp [open udp.tr w]

proc Record {} {
global f_udp null0 ns
set intval 1
set now [$ns now]

set app2 [$null0 set bytes_]
$null0 set bytes_ 0
puts $f_udp "$now [expr $app2*8/$intval]"

$ns at [expr $now + $intval] "Record"
}


proc finish {} {
    global ns tracefile nf f_udp
    $ns flush-trace
    close $tracefile
    close $nf
    close $f_udp
    #exec nam out.nam &
    exit 0
}

$ns at 0.0 "Record"
$ns at 0.0 "$cbr0 start"
$ns at 30.0 "$cbr0 stop"
$ns at 30.0 "finish"
$ns run
