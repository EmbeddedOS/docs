# 1. Initialize a WLAN app task

- 1. Device initialize: `rsi_device_init(LOAD_NWP_FW);`

- 2. Wireless initalize: `rsi_wireless_init(RSI_WLAN_CLIENT_MODE, RSI_OPERMODE_WLAN_BLE);`

- 3. Create a driver task for rsi_wireless:

```C
  rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                  (uint8_t *)"driver_task",
                  RSI_DRIVER_TASK_STACK_SIZE,
                  NULL,
                  RSI_DRIVER_TASK_PRIORITY,
                  &driver_task_handle);
```

- 4. Send feature frame: `status = rsi_send_feature_frame();`
  - This function select internal or external RF type and clock frequency.

- 5. Register callback for Remote socket terminate event:

    ```C
    rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB,
                                rsi_remote_socket_terminate_handler); // Initialize remote terminate call back
    ```

  - `rsi_wlan_register_callbacks()` This API register the WLAN callback functions. With parameters:
    - `callback_id`: ID of the callback function. Table below displays supported ids:
      - RSI_JOIN_FAIL_CB: 0
      - RSI_IP_FAIL_CB: 1
      - RSI_REMOTE_SOCKET_TERMINATE_CB: 2
      - RSI_IP_CHANGE_NOTIFY_CB: 3
      - etc.
    - `callback_handler_ptr`: `void (*callback_handler_ptr)(uint16_t status, uint8_t *buffer, const uint32_t length)` - Callback handler for corresponding event:
      - status                            - Status of the asynchronous response
      - buffer                            - Payload of the asynchronous response
      - length                            - Length of the payload

- 6. Load SSL CA certificate if any: `rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, cacert, (sizeof(cacert) - 1));`
- 7. Config RSI WLAN buffer config: `rsi_wlan_buffer_config();` this API configure the TX, RX global buffers ratio.

- 8. Scan for wifi access points: `rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);`
  - This API scan the surrounding Wi-Fi networks.
  - `ssid`: SSID
  - `chno`: Channel number to perform scan. If 0, then the module will scan all the channels.
    - Channels supported in 2.4 GHz Band: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
    - Channels supported in 5 GHz Band: 36, 40, 44, 48, 149, 153, 157, 161, 165
    - DFS Channels supported in 5 GHz Band: 52(DFS), 56(DFS), 60(DFS), 64(DFS), 100(DFS), 104(DFS), 108(DFS), 112(DFS), 116(DFS), 120(DFS), 124(DFS), 128(DFS), 132(DFS), 136(DFS), 140(DFS), 144(DFS)
    - Channels supported in 4.9 GHz Band: 184, 188, 192, 196, 8, 12, 16
  - `result`: Scanned Wi-Fi networks information. This is an output parameter. This output contains a maximum of 11 scan results.
  - `h`: Size that should be allocated to buffer that will store scan results.

- 9. Connect to an Access point: `rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);`
  - Connect to the specified Wi-Fi network. With param:
    - `ssid` - SSID of an access point to connect. SSID should be less than or equal to 32 bytes.
    - `sec_type` - Security type of the access point to connect.
      - 0: RSI_OPEN,
      - 1: RSI_WPA,
      - 2: RSI_WPA2,
      - 3: RSI_WEP,
      - 4: RSI_WPA_EAP,
      - etc.
    - `secret_key` - pointer to a buffer that contains security information based on `sec_type`.

- 10. Configure IP: Using static IP or DHCP if any.
  - `rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, ip_buff, sizeof(ip_buff), 0);`
    - This API configure the IP address to the module. Send IPconfig command to configure IP address with input parameters (like ip-version, static mode/dhcp, vap_id) and return error in case of invalid parameters.
    - `rsi_wlan_connect()` API needs to be called before this API.
    - Parameters:
      - `version` - IP version to be used. `RSI_IP_VERSION_4` (4) - to select IPv4, `RSI_IP_VERSION_6` (6) - to select IPv6.
      - `mode` - 1 - DHCP mode; 0 - static mode.
      - `ip_addr` - Pointer to IP address.
      - `mask` - Pointer to network mask.
      - `gw` - Pointer to gateway address.
      - `ipconfig_rsp` - Hold the IP configuration received using DHCP.
      - `length` - Length of `ipconfig_rsp` buffer.
      - `vap_id` - VAP id to differentiate between AP and station in concurrent mode.
        - 0 - for station.
        - 1 - for Access point.

- After get the IP address, we will do something with the IP :V. Some throughput types:
  - Transfer UDP packet.
  - Receive UDP packet.
  - Transfer TCP packet.
  - Receive TCP packet.

- 11. UDP transfer some packets (UDP client):
  - Create a UDP rsi socket: `client_socket = rsi_socket(AF_INET, SOCK_DGRAM, 0);`
  - Determine the server address:

    ```C
    #define SERVER_PORT 5001
    #define SERVER_IP_ADDRESS "192.168.5.234"
    ```

    ```C
    struct rsi_sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);
    ```

  - Send data to server:

    ```C
    status = rsi_sendto(client_socket, send_buf, BUF_SIZE, 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));

    ```

  - This feature using some API:
    - `rsi_socket()`: Create and return a socket instance, also referred to as an end-point of communication. `rsi_config_ipaddress()` API needs to be called before this API. With params:
      - `protocolFamily`: - Protocol family to select IPv4 or IPv6.
        - `AF_INET`: select IPv4
        - `AF_INET6`: select IPv6
      - `type`: Select socket type UDP or TCP:
        - `SOCK_STREAM`: Select TCP
        - `TCP_SOCK_DGRM`: Select UDP
      - `protocol`:
        - 0: NON SSL sockets.
        - 1: SSL sockets.
    - `rsi_sendto()`: This API sends data to the specified remote IP address on the given socket. `This is non-blocking API.`
      - For tcp_client, rsi_connect().
      - For tcp_server, rsi_listen() and rsi_accept()/ API needs to be called before this API.
      - For udp_server, rsi_bind() API needs to be called before this API.
      - For udp_client, rsi_client() or rsi_bind() API needs to be called before this API.
      - With params:
        - `sockID` - Socket descriptor ID.
        - `msg` - Pointer to data buffer containing data to send to remote peer.
        - `msgLength` - Length of the buffer.
        - `flags` - Reserved.
        - `destAddr` - Address of the remote peer to send data.
        - `destAddrLen` - Length of the address in bytes.

- 12. UDP receive some packets (UDP server):
  - We need to make a udp server.
  - Create a socket for receiving: `server_socket = rsi_socket(AF_INET, SOCK_DGRAM, 0);`
  - Next make the server address to bind:

    ```C
    struct rsi_sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    ```

  - Bind the server address to the socket:

    ```C
    status = rsi_bind(server_socket, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
    ```

  - Receive some packets that come from the socket:

    ```C
    status = rsi_recvfrom(server_socket, recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);

    ```

  - This feature using APIs:
    - `rsi_bind()`: Assign an address to a socket. This is a non-blocking API for TCP and a blocking API for UDP.
    - With params:
      - `sockID`: Socket descriptor ID
      - `localAddress`: Address assigned to the socket. The format is compatible with BSD socket.
      - `addressLength`: Length of the address measured in bytes.
    - `rsi_recvfrom()`: Retrieve the received data from the remote peer on a given socket descriptor.
      - With params:
        - `sockID`: Socket descriptor.
        - `buffer`: Pointer to buffer to hold receive data.
        - `bufferSize`: Size of the buffer supplied.
        - `flags`: Reserved.
        - `fromAddr`: Address of remote peer, from where current packet was received.
        - `fromAddrLen`: Pointer that contains remote peer address length.

- 13. TCP transfer some packets (TCP client)
  - First, we will make a TCP socket: `client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);`
  - Make client structure:

    ```C
    #define PORT_NUM 5001
    ```

    ```C
    struct rsi_sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT_NUM);
    ```

  - Bind the client address to the socket:

    ```C
    status = rsi_bind(client_socket, (struct rsi_sockaddr *)&client_addr, sizeof(client_addr));
    ```

  - Make server structure:

    ```C
    // Server port number
    #define SERVER_PORT 5001

    // Server IP address.
    #define SERVER_IP_ADDRESS "192.168.5.234"
    ```

    ```C
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);
    ```

  - Make connection with the Server via server address:

    ```C
    status = rsi_connect(client_socket, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
    ```

  - After we connect to the server, we will send some messages via client_socket:

    ```C
    status = rsi_send(client_socket, send_buf, BUF_SIZE, 0);
    ```

  - This feature using some APIs:
    - `rsi_connect()`: Connect the socket to the specified remote address.
      - `sockID`: Socket Descriptor ID.
      - `remoteAddress`: Remote peer address.
      - `addressLength`: Length of the address in bytes.
    - `rsi_send()`: This API sends the data to the remote peer on the given socket.
      - `sockID`: Socket Descriptor ID.
      - `msg`: Pointer to the buffer containing data to send to the remote peer.
      - `msgLength`: Length of the buffer.
      - `flags`: Reserved.

- 14. TCP receive some packets (TCP server)

  - Create a rsi socket for TCP server: `server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);`

  - Config socket options:

    ```C
    #define SOL_SOCKET   1  /* Define the socket option category. */
    #define SO_HIGH_PERFORMANCE_SOCKET  38             /* To Configure the high performance socket */

    uint8_t high_performance_socket = RSI_HIGH_PERFORMANCE_SOCKET;
    status = rsi_setsockopt(server_socket,
                        SOL_SOCKET,
                        SO_HIGH_PERFORMANCE_SOCKET,
                        &high_performance_socket,
                        sizeof(high_performance_socket));
    ```

  - Config server address:

    ```C
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    ```

  - Bin the server address to the socket:

    ```C
    status = rsi_bind(server_socket, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
    ```

  - Listen on the address:

    ```C
    status = rsi_listen(server_socket, 1);
    ```

  - When we receive some connection requests, we will accept them:

    ```C
    new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&client_addr, &addr_size);

    ```

    - After accept the connection, the API will return the socket ID, we are going to use it to receive message from the client.

    ```C
    status = rsi_recvfrom(new_socket, recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);
    ```

  - This features using some new APIs:
    - `rsi_listen()`: Enable socket to listen for a remote connection request in passive mode. This is a blocking API.
      - `socID`: Socket Descriptor ID
      - `backlog`: - Maximum length to which the queue of pending connections can be held.
    - `rsi_accept()`: Accept the connection request from the remote peer. This APi extracts the connection request from the queue of pending connections on listening socket and accepts it.
      - `sockID`: Socket descriptor.
      - `ClientAddress`: Remote peer address.
      - `addressLength`: Length of the address measured in bytes.

- 15. SSL Client:
