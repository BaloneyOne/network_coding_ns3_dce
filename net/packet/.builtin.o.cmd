cmd_/home/nick/network-sim/mptcp/net/packet/builtin.o := mkdir -p /home/nick/network-sim/mptcp/net/packet/; if test -n "/home/nick/network-sim/mptcp/net/packet/af_packet.o"; then for f in /home/nick/network-sim/mptcp/net/packet/af_packet.o; do ar Tcru /home/nick/network-sim/mptcp/net/packet/builtin.o $$f; done; else ar Tcru /home/nick/network-sim/mptcp/net/packet/builtin.o; fi