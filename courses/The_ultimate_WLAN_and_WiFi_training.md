# The ultimate WLAN and WiFi

## Introduction

- Wireless networking is based on radio frequency communications.
- Wireless networking is a foundational part of modern local area networking.
- Common characteristics:
  - Incredibly convenient.
  - Widely used.
  - Easy to install.
  - Widely misunderstood.
  - Rarely properly implemented.
- More about WLAN & WiFi.
- Low-speed wireless data networking products began to appear in the 1990s.
- The Institute of Electrical and Electronics Engineers (IEEE) ratified the original 802.11 standard in 1997.
- Warehousing and manufacturing environments were the early adopters.

- Home networks were the first to widely implement wireless networking.
  - This aspect of wireless networking seriously affected development and future implementations.
  - Security suffered; easy and simplicity ruled.

## Getting started

### 2. What are WLAN and WiFi? Why they are so special?

- WLAN stands for Wireless Local Area Network, and WiFi is a type of WLAN that uses the 802.11 standard for wireless communication.

- **WiFi is special because it allows for wireless connectivity to the internet in a local area network (LAN) without the need for cables or physical connectivity.**
  - This allows for increased mobility and flexibility in internet access, as well as making it easier to set up a network and connect multiple devices.
  - Additionally, WiFi is widely used and supported, making it a convenient and popular choice for both personal and commercial use.

- Additionally, WiFi technology has improved over time with faster speeds, greater coverage, and enhanced security features, making it a reliable and secure option for connecting devices.
- This has enabled the widespread use of WiFi-enabled devices such as laptops, smartphones, tablets, and smart home devices, leading to the growth of the IOT.

### WiFi Alliance

- The IEEE creates and maintains standards for wireless communication.
  - The standards do not address every possible detail.
  - Manufacturers may not conform to all standards.

- Early wireless devices experienced interoperability problems.
- In 1999 a group of companies formed the Wireless Ethernet Compatibility Alliance (WECA).
- In 2002, WECA renamed itself the WiFi Alliance.

- **Regulatory bodies**:
  - Frequencies within the RF (Radio Frequency) spectrum are available because they exist worldwide.
    - Using frequencies at will would eventually disable all communication.
  - Regulatory bodies have been formed to keep the RP spectrum organized and open for fair use.

  - There are three main regulatory bodies that govern the RF spectrum.
    - `ITU-R`: International Telecommunication Union Radiocommunication Sector. Maintains spectrum and frequency assignments in three distinct regions.
      - Region 1: Europe, Africa, and Northern Asia.
      - Region 2: North and South America.
      - Region 3: Southern Asia and Australasia.
      - Also attempts to make sure that the RF signals from one country do not interfere with the signals of another country.
    - `FCC`: Federal Communications Commission.
    - `ETSI`.

### 4. 802.11 Standards

- IEEE:
  - Institute of Electrical and Electronics Engineers
  - The professional society that creates and maintains standards that we use for network communications.
  - The 802.3 Ethernet standard is for wired networking.
  - The 802.11 standard is for WLAN communication.
    - **LAN communications using radio frequencies (RF)**.

- 802.11 STANDARDS:
  - The 802.11 working group is big.
    - Approximately 400 active members.
    - From more than 200 wireless companies.
  - Every member can make suggestions on operations and implementation.
    - Final standard is a collection of best integrated suggestion solutions.

- 802.11
  - Ratified in 1997.
  - Included only transmission types FHSS and DSSS.
    - Only ones available at that time.
  - 2.4-GHz band.
  - Provided data speeds of 1-2Mbps.

- 802.11a
  - Approved in 1999 (same as 11b).
  - 5.0 GHz.
  - 54 M-bps (but much less range than 802.11b).
  - Higher cost and shorter distance made 11A less popular.

- 802.11b
  - Approved in 1999 (same as 11A).
  - 2.4GHz.
  - Provided data rates up to 11MBps.
  - Received much better than 11A by the marketplace.

- 802.11g
  - Approved in 2003
  - 2.4GHz.
  - Data rates up to 54 Mbps.
  - Compatible with 802.11b.
  - Data transfer rates suffer.

- 802.11n
  - Approved in 2009.
  - 2.4 and 5GHz.
  - Introduced MIMO.
    - Multiple Input Multiple Output.
    - Channel aggregation.
    - Multiple antennas.
  - Data speeds up to 600 Mbps.

- 802.11ac
  - Approved in 2014
  - Data rates of 6.933 Mbps... just under 7Gbps (theoretical)
  - 5.0 GHz.
  - ENhanced MIMO functionalities.
    - Extends channel bandwidth from 11n's 40 MHz to 80 Mhz.
    - Increased MIMO functionalities and capacities.
  - Utilizes `beamforming` as a core part of the spec.
  - Signals are not just sent out in all directions.
  - Detects where devices are and intensifies the signal in their direction.
    - Compensates for the shorter range limitation of the 5GHz band.

### 5. Basic of WLAN

- RF MATH:
  - Early we learned two RF related facts:
    - Power is measured in actual units.
      - Watts, milli-watts, etc.
    - Comparisons are made using relative units:
      - Decibels.
      - Uses logarithms to compare one absolute measurement to another.
    - RF math can get complex.
  - There are four simplified facts you need to remember to make power-level /dB comparisons.
    - A value of 0 dB means the two absolute powers being compared are equal.
    - A value of 3 dB means the power value of interest is double the reference value.
      - -3 dB means it is half of the reference value.
    - A value of 10 dB means the power value of interest is ten times the reference value.
      - -10 dB means it is one-tenth the reference value.
  - Decibel gains and losses are cumulative.
    - 3 dB + 3 dB = 6 dB
    - 5 dB + 4 dB - 2 dB - 3dB = 4 dB
  - How can we use RF math to understand power?
    - Example:
      - Transmitter provides 30mW of power.
      - Antenna provides 3dB of passive gain.
      - What is the resulting power level of the signal at the antenna?
        - 30 mW + 3 dB = power level
          - We know that 3 dB means the signal doubles in strength.
            - 30mW * 2 = 60 mW.
            - 60 mW is the effective power level.

- WIRELESS SWOTS
  - **Strengths**:
    - Convenience
      - Network access without a dedicated line, cable, port, etc.
      - Mobility
        - Devices can move around an area.
        - Network access availability.
      - Productivity
        - Network access from various locations.
      - Deployment
        - No cabling, required number of ports, etc.
      - Expandability
        - Easily scaled up or down.
      - Expense
        - Lower total cost of ownership and maintenance over time.
  - **Weaknesses**:
    - Security
      - All data is transmitted as RF and can be **captured by any wireless receiver**.
      - All data must be encrypted.
    - Range
      - Range is limited by physical characteristics and environmental conditions.
    - Speed
      - Usually no faster than 54 Mbps.
      - This will change dramatically in the near future.
    - Complexity
      - Wavelengths, channels, bands, spectrum, etc.
  - **Opportunities**:
    - Increased Speed:
      - 802.11ac promises drastically improved speed.
    - Better security:
      - Security continues to improve.
      - Use of VLANs, vendor-specific features, physical security additions, etc.
  - **Threats**:
    - Usual security threats
      - Denial of service.
      - Rogue WAPs.
      - Poorly configured WAPs.
      - Sniffing.
      - Wireless Driver Attacks.

### 6. Specific Characteristics in Radio Frequency Signals

- Wireless signal characteristics
  - Radio Frequency (RF) signals have specific characteristics:
    - Defined by the laws of physics.

- There are four characteristics you should understand
  - Wavelength
    - One complete oscillation from **positive** (crest) to **negative** (valley) and back is called **a cycle**.
    - The distance between two successive **crests** or **valleys** is the **wavelength** of the signal.
  - Frequency
    - The number of times a signal cycle occurs in a certain time period.
    - Measured in Hertz (Hz).
      - Named after German physicist Heinrich Rudolf Hertz.
    - A cycle that occurs one time in one second is equal to 1 Hz.
    - A cycle that occurs 500 times in one second is 500 Hz.
  - Amplitude
  - Phase
    - The relationship between two or more signals.
      - If the peaks are in exact alignment, the signals are in phase.
      - If the peaks are not in exact  alignment, the signals are out of phase.

- Wireless components
  - What are the components of a wireless network?
    - Easy to answer
      - Wireless accept point.
      - Device receiving the signal.
  - These are the hardware devices that are used to create a wireless network.
    - There are a lot more components.
    - These components have major implications.
  - A wireless network has other components in addition to the two main pieces of hardware.
    - Frequency.
    - Environment.
    - Radio frequency interference.
    - Electrical interference.
    - Obstacles.
    - Signal delivery.
  - We'll start with the two obvious components
    - Wireless Access Point.
      - Various types.
      - Based on various 802.11 standards.
      - Utilities different frequencies.
    - Receiver:
      - Different wireless NICs (Network Interface Card) can have drastically different performance characteristics.

## Wireless LANs (WLAN)

### 25. Introduction

- Wireless networking continues to grow
  - Wireless LANs are now the norm.
  - Enterprise wireless networks are becoming commonplace.
    - Span large geographical areas.
    - Centrally managed.

- 802.11 WLANs use half-duplex communication methods.
  - QUick review of communications:
    - Simplex communication.
      - One device can only transmit, the other device can only receive.
      - AM/FM radio.
    - Half-duplex.
      - Both devices have the ability to transmit and receive.
      - Only one device can transmit at a time.
        - Walkie-talkie, two-way radio, etc.
      - Currently **802.11 wireless network use half-duplex**.
    - Full-duplex.
      - Both devices can transmit and receive at the same time.
      - Telephone.
      - Wireless networks can achieve full-duplex communications using two separate one-direction channels.
        - Not currently being implemented.

- **Simplicity** is biggest strength point, also, is biggest weakness.
  - Easy to setup. -> Lazy, poorly planned implementation.
  - Security issues.

- Access point basics
  - The wireless access point (WAP) is the key part of a wireless network.
  - two main functions of AP:
    - 1. Sends and receives wireless signals to/from wireless clients.
    - 2. Connects the wireless network to the wired network.
  - Supports the connection of multiple wireless devices to a single wired connection.
    - Support as many as 30 clients within a radius of 200-300 ft.

  - WAPs can be used individually or as an inter-connected group.
    - Managed by a WLAN Controller via the wired network.
  - WAPs operate based on a specific 802.11 standard
    - **802.11n** is currently the most popular.

### 26. Understanding SSID, BSS & BSSID

- SSID:
  - Service Set Identifier
    - Logical name used to identify an 802.11 wireless network.
    - Comparable to a Windows workgroup name.
    - Allows the WAP and the clients to identify each other.
    - Up to 32 characters.
    - Case sensitive.
  - Most WAPs automatically broadcast the SSID name.
    - Enable possible clients to locate and identify the wireless network.
  - SSID cloaking:
    - Attempting to hide the SSID by not broadcasting it.
      - An optional setting on most WAPs.
    - Weak attempt at security.

- BSS and BSSID

  - The basic service set (BSS) is the foundational topology of an 802.11 network.
  - A BSS consists of one AP and one or more client stations.
    - The AP manages the wireless network.
    - Clients can communicate, but through the AP.
  - Clients join the AP wireless domain using the SSID.
    - Service Set Identifier.
    - These BSS clients also have a layer 2 connection.
      - Layer 2 of the OSI model; the Data Link Layer.
      - When clients are joined to the BSS network they are said to be associated.
      - Associated clients are also referred to in 802.11 speak as a station (STA).

  - **The MAC address of the access point is known as the basic service set identifier (BSSID).**
  - This allows clients to identify the AP they are communicating with.
    - There can be two BSSs near each other.
    - They can both advertise the same SSID to allow clients to roam between the BSSs. (Using different MAC address).

- NOTE:
  - The SSID identifies the wireless network.
  - The BSSID identifies the AP in the wireless network.
  - The physical coverage area provided by an Access Point in a BSS is known as the Basic Service Area (BSA).

### 27. Understanding Distribution System & ESS

- Reminder:
  - A basic service set has an AP and clients.
  - It is a standalone network.
  - No explicit connection into a wired Ethernet network.
  - The AP's job is to manage the BSS.

- **Distribution System**:
  - A standalone wireless network is ok, but ...
  - Connecting into a wired Ethernet network would be great.
  - The AP can accomplish this via an uplink into Ethernet network.

  - The 802.11 standard refers to the upstream wired Ethernet network as the distribution system for the wireless BSS.

  - **The AP is acting as bridge**.
    - Translating frames from two different networks:
      - Wired and wireless.

- **Extended Service Set**:
  - A BSS is limited to the reach of a single AP.
  - What if you need to provide coverage throughout a much larger area?
  - You can use an Extended Service Set (ESS).
  - An extended service set (ESS) is too or more BSSs connected by a distribution system medium.

  - Different service types:
    - Seamless roaming.
      - Clients can move from one coverage area to another without losing their connection.
    - Nomadic roaming.
      - Moving from one coverage to another will result in the loss of connectivity.
    - Colocation.
      - Provide increased client capacity.

### 29. Understanding AP modes, CSMA/CD & CSMA/CA

- Access point modes:
  - By default APs behave the way we normally want them to behave.
    - They allow client stations to connect.
    - They transfer data back and forth between the distribution system (wired network) and the wire network.
    - This is called root mode, AP mode, or access mode by most vendors.
  - There are several other modes that an AP can be configured to perform:
    - Bridge mode.
    - Workgroup bridge mode.
    - Repeater mode.
    - Mesh mode.
    - Scanner mode.

- **Bridge Mode**:
  - The AP operate as a wireless bridge to connect two wired networks together.
  - It utilizes additional MAC-layer functionalities to learn, build and maintain **tables of MAC addresses** on the wired side of the network.

- **Workgroup Bridge Mode**:
  - The AP associates to another AP as a client and provides a network connection for the equipment connected to its Ethernet port.

- **Repeater Mode**:
  - The AP acts as a repeater to extend the coverage of a another AP on the same channel.

- **Mesh mode**:
  - The AP functions as a wireless backhaul radio for a mesh environment.
  - It may allow for client access.

- **Scanner mode**:
  - Operates as a sensor radio for integration into a wireless intrusion detection system (WIDS).

- What about client station modes?
  - Client stations can operate in one of two modes:
    - 1. Infrastructure mode.
      - Connect to an AP.
    - 2. Ad Hoc or Peer to Peer mode.
      - Connect to other client stations with using an AP.

- **CSMA/CD and CSMA/CA**:
  - Network communication involves many devices trying to communicate over the same medium (wired network).
    - It's not very useful if everyone transmits at the same time.
  - A set of communication rules is required to provide a way of controlling media access.
  - Two main forms of managing media access in networking today is:
    - 1. CSMA/CD.
      - Carrier Sense Multiple Access/ Collision Detection.
      - Used on 802.3 (wired) networks.
      - Wired network is full-duplex networks.
        - Transmit and receive at the same time.
      - Handles collisions the way we do on a phone call.
        - **Detect collision; stop transmitting wait a random amount of time; begin transmit again.**
    - 2. CSMA/CA.
      - Carrier Sense Multiple Access/ Collision Avoidance.
      - Used on 802.3 (wireless) networks.
      - Half-duplex:
        - Cannot transmit and receive at the same time.
        - Cannot detect collisions as they are occurring.
      - The device first listens to see if the network in use.
        - This is called performing a carrier sense.
        - It listens for a random period of time.
        - If the network is still available at the end of the random time, it will check once more and transmit if clear.
      - How does CSMA/CA know if a collision occurred?
        - it doesn't but.. it can assume it.
        - **Every frame sent on an 802.11 network is acknowledged by the receiver with an ACK frame.**
        - If an ACk frame is not received, a collision is assumed.
        - The process of listening starts over...
