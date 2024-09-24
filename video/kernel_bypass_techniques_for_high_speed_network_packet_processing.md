# Kernel-bypass techniques for high-speed network packet processing

- Youtube: [Link](https://www.youtube.com/watch?v=MpjlWt7fvrw&t=1120s)

- Outline:
  - The journey of packet through the Linux network stack.
  - Need for kernel bypass techniques for packet processing.
  - Kernel-bypass techniques:
    - User-space packet processing.
      - DPDK.
      - Netmap.
    - User-space network stack:
      - mTCP.
  - What's trending?

## Typical packet flow

```text
    |       TX      |                   |       RX      |
    |Application    |                   |Application    |
    |Transport(L4)  |                   |Transport(L4)  |
    |Network(L3)    |                   |Network(L3)    |
    |Data link(L2)  |                   |Data link(L2)  |
    |NIC Driver     |                   |NIC Driver     |
    |NIC hardware   |                   |NIC hardware   |
                \\                       //
                 \\>>>>>>>>>>>>>>>>>>>>>//
```

## What does a packet contain?

```text
|Ethernet header|IP header| TCP header |        payload         |FCS|
```

- When:
  - `FCS` stands for Frame Check Sequence.
  - Ethernet contain: dst MAC + src MAC + type.
  - IP header contain: ... + length + IP type + header cs + src IP + dst IP
  - TCP header contain: src port + dst port + ... + checksum + ...

## Rx path: packet arrives at the destination NIC

- When a packet arrives the NIC hardware, it validate the packet (MAC address + verify checksum), write the packet to Rx rings circular buffer and then raise an interrupt.
- The NIC driver handle the interrupt, copy the packet from Rx ring to the kernel memory and forward to the upper layer.

- The Rx rings circular buffer is shared between NIC device and NIC driver (DMA).

## Top-half interrupt processing

## Bottom-half interrupt processing

- NIC driver processing:

1. Driver dynamically allocates an `sk-buff`.
2. Update `sk-buff` with packet metadata.
3. Remove the Ethernet header.
4. Pass `sk-buff` to the network stack. -> Call L3 protocol handler.

- L3 processing:
    1. match destination IP/socket.
    2. verify checksum.
    3. route lookup.
    4. combine fragmented packets.
    5. remove header.
    6. Call L4 protocol handler.

- L4 processing:
  - Handle TCP state machine.
  - Enqueue to socket read queue.
  - Signal the socket.

- User space read processing:
  - dequeue packet from socket receive queue (kernel space).
  - Copy packet to application buffer.
  - Release `sk-buff`.
  - Return back to application.
