
# course instruction

Github repo for supporting files: [consul](https://github.com/btkrausen/hashicorp)

## II Objective 1: Explain Consul Architecture

### 7. Objective 1 overview

Objective 1a: Identify the components of Consul data-center, including agents and communication protocols.

objective 1b: prepare consul for high availability and performance.

objective 1c: Identify Consul's core functionally.

objective 1d: Differentiate agent roles.

### 8. Introduction to hashicorp consul

Hashicorp suite of tools: terraform, vault, consul, etc.
-> Cloud networking automation for dynamic infrastructure.

Core features of Consul:

* Dynamic Service registration.
* Service discovery.
* Distributed health checks.
* centralized K/V storage.
* Access control lists.
* Segmentation of service.
* cross cloud/Data center availability.
* HTTP API, UI, and CLI interfaces.

### 9. Consul discovery

* Centralized service registry.
  * Single point of contact for services to communicate to other services.
  * Importance for dynamic workloads (such as containers).
  * Especially important for a microservice architecture.
* Reduction or elimination of load balancers to front-end services.
  * Frequently referred to as east/west traffic.
* Real-time health monitoring
  * Distributed responsibility throughout the cluster.
  * Local agent performs query on services.
    * Node level health checks.
    * Application level health checks.

For example, we have three applications: inventory, search, order run on different infrastructures. On each infrastructure, we will install a consul agent, each consul agent will register to become service of the consul agent server.

|search1|------------DNS query------>|              |
|search2|------------DNS query------>|              |
|search3|------------DNS query------>|              |
|search4|------------DNS query------>|              |
|search5|------------DNS query------>|              |
|search*|------------DNS query------>|              |
                                     |              |
|inventory1|---------Health status-->| Consul server|-----External service------>|DB|
|inventory2|---------Health status-->|              |
|inventory*|---------Health status-->|              |
                                     |              |
|order|--------------API request---->|              |

--> Scale to thousands and thousands of nodes.

* Automate networking and security using identity-based authorization.
  * Not more IP-based or firewall-bashed security.

### 10. service mesh

* enables secure communication between services.
  * integrated mTLS secures communication.
  * Uses `sidecar` architecture that is placed alongside the registered service.
  * Sidecar (Envoy, etc.) transparently handles inbound/outbound connections.
* Defined access control for services
  * Defines which service can establish connections to other service.

For example, OK:

|Consul server          |
|Certificate Authority  |
    /|
    ||      ||=============OK============||
    |/      /\                           \/
|   mTLS |consult client|   |   mTLS |consult client|   |   mTLS |consult client|
|   Web                 |   |   Search              |   |   Payment             |
        container-1                 container-2                 container-3

NOT OK:

          ||============NOT-OK=========||
          /\                           \/
|                       |   |                       |   |                       |
|   Web                 |   |   Search              |   |   Payment             |
        container-1                 container-2                 container-3

### 11. Network automation

* Dynamic load balancing among services.
  * Consul will only send traffic to healthy nodes & services.
  * Use traffic-shaping to influence how traffic is sent.
* Extensible though networking partners.
  * F5, HAproxy, envoy.
* Reduce downtime by using multi-cloud and failover for services.
* L7 traffic management based on your workloads and environment
  * service failover, path-based routing
* Increased L7 visibility between services.
  * View metrics such as connections, timeout, open circuits, etc.
    Consul- envoy ------ StatsD, DogStatsD, Prometheus---> Grafana.

### 12. Service Configuration

* Consul provides a distributed K/V store.
* All data is replicated across all Consul server.
  * Can be sued to store configuration and parameters.
  * It is NOT a full featured datastore (like DynamoDB).
* Can be accessed by any agent (client or server).
  * Accessed using the CLi, API or Consul UI.
  * Make sure to enable ACLs to restrict access.
* No restriction on the type of object stored.
* Primary restriction is the object size -capped at 512KB.
* Doesn't use a directory structure, although you can use / to organize your data within the KV store.
  * / is treated like any other character.
  * This is different than Vault where / signifies a path.

For example, nginx application variables:
    host: larva.com     |
    port: 443           |--Write data to consul KV--->|Consul KV store|
    test-cookie: off    |

|Application 1|
|Application 2|<-----KV data------|jenkins|-----Retrieve data----->|Consul KV store|
|Application 3|

### 13. Basic consul architecture

Consul basics: Consul can be run as an **agent** on top of the underlying OS. And this agent is a long running daemon that manages the console service itself. Now, one of the benefits of a console agent is that it is platform agnostic, meaning that the agent can run on top of many different platforms. -> extremely flexible.

The console agent can be configured to run in what's called either server mode or client mode. This configuration is determined by either the **consul configuration file** or it can be configured by the **command line arguments** that are passed to it.

Consul agent mode:

* Server.
* Client.
* Dev.

Server vs. client mode:

* Server:

  * Consul server is generally a member of a consul cluster, and it is responsible for understanding **the state of the entire consul cluster**.
  * managing membership.
  * Responds to queries.
  * Registers services.
  * Maintain quorum.
  * Acts as Gateway to other DCs (data-center).

* Client:

  * Register local services
  * performs health-checks
  * forwards RPC calls to servers.
  * takes Part in LAN Gossip Pool.
  * Relatively stateless.

* Dev:

  * used only for testing/demo.
  * runs as a consul server.
  * not secure or scalable.
  * runs locally.
  * stores everything in memory.
  * does not write to disk.

Single Data-center:

* What is a Data center?
  * Single-cluster: First, a consul data center is made up of a single consul cluster. This consul cluster has one or more servers and it communicates with clients that are members of this consul cluster.
  * Private: Data center is also private to your organization, and it is not a publicly available consul. Data centers are intended to handle internal communications for your applications and not for external communications.
  * Low latency.
  * High bandwidth.
  * contained in a single location.
  * multi-AZ is acceptable.
  * uses the LAN gossip pool.

* What a Data-center is not !
  * multi-cloud or location.
  * multiple consul clusters.
  * uses the WAN gossip pool.
  * communicates via WAN or Internet.

* What is Multi-Data-center?
  * multi-cloud, multi-region, location, or cluster.
  * multiple consul cluster federation.
  * uses the WAN gossip pool.
  * communicates via WAN or Internet.
  * WAN federation through mesh gateways.

Key protocols:

* communication between servers: using Consensus Protocol (RAFt).
* communication between servers and clients: gossip protocol (serf)

### 14. Consensus Protocol

Based on RAft

* Used on only server nodes (cluster) - not clients.
* Strongly consistent.

Responsible:

* Leadership elections.
* Maintaining committed log entries server nodes.
* establishing a quorum.

Consensus glossary

* log:
  * Primary unit of work - an ordered sequence of entries.
  * ENtries can be a cluster change, key/value changes, etc.
  * Since we want to be sure that all server nodes have the same data for consistency, All members must agree on the entries and their order to be considered a consistent log.

* Peer set:
  * All members participating in log replication. If you have a consul cluster with five server nodes, your peer set would be those five server nodes.
  * In Consul's, all servers nodes in the local data-center.

* Quorum:
  * Majority of members of the peer set (servers).
  * No quorum = no consul.
  * A quorum requires at least (n+1)/2 members.
    * Five-node cluster = (5 + 1)/2 = 3
    * Three-node cluster = (3 + 1)/2 = 2

* Consensus Protocol:
  * Raft nodes are always in one of three states:
    * leader.
    * follower.
    * candidate.

|Follower|---VOTE-> OR -----> Leader promotion.
                    \-------> continue as follower.

When a consul server first comes online, it is a follower. If there are no leaders or an existing leader fails, a consul node can vote for itself ot another node as the leader.

Here, each cluster will have one and only one leader again, and that leader will remain the leader until the consul node or that consul service fails.

Leader is responsible for:

* Ingesting new log entries.
* Processing all queries and transactions.
* Replicating to followers.
* Determining when an entry is considered committed.

Follower is responsible for:

* Forwarding RPC request to the leader.
* Accepting logs from the leader.
* Casting votes for leader election.

Consensus protocol - leader election:

* Leadership is based on randomized election timeouts
  * Leader sends out frequent heartbeats to follower nodes.
  * Each server has a randomly assigned timeout (e.g., 150ms - 300ms).
  * If a heartbeat isn't received from the leader, an election takes place.
  * The node changes its state to candidate, votes for itself, and issues a requests for votes to establish majority.

### 15. Gossip Protocol

* Based on Self:
  * Used cluster wide - including multi-cluster.
  * Used by clients and servers.
* Responsible for:
  * Manage membership of the cluster (clients and servers).
  * Broadcast message to the cluster such as connectivity failures.
  * Allows reliable and fast broadcasts across data-centers.
  * Make use of two different gossip pools:
    * LAN
    * WAN

Gossip Protocol:

* LAN Gossip Pool:
  * Each data-center has its own LAN gossip pool.
  * Contains all members of the data-center (clients & servers).
  * Purpose:
    * Membership information allows clients to discover server.
    * Failure detection duties are shared by members of the entire cluster.
    * Reliable and fast event broadcasts.
* WAN Gossip Pool:
  * Separate, globally unique pool.
  * All servers participate in the WAN pool regardless of data-center.
  * Purpose:
    * Allows servers to perform cross data-center requests.
    * Assists with handling single server or entire data-center failures.

### 16. Network Traffic and port

* All communication happens over http and https.
* Network communication protected by TLS and gossip key.
* Ports (assume default):
  * HTTP API and UI -tcp/8500
  * LAN Gossip -tcp & udp/8301
  * WAN Gossip -tcp & udp/8302
  * RPC - tcp/8300
  * DNS - tcp/8600 & udp/8600
  * Sidecar Proxy -21000 -21255

Also, keep in mind that all these ports are completely customizable, if you wish.

Accessing consul:

* Consul API can be accessed by any machine (assuming network/firewall).
* Consul CLI can be accessed and configured from any server node.
* UI can be enabled in the configuration file and accessed from anywhere.

### 17. COnsul high availability

* High availability us achieved using clustering
  * Hashicorp recommends 3-5 servers in a consul cluster.
  * uses the consensus protocol to establish a cluster leader.
  * If a leader becomes unavailable, a new leader is elected.
* General recommendation is to not exceed 7 server nodes
  * consul generates a lot of traffic for replication.
  * More than 7 servers may be negative impacted by the network or negatively impact the network.

## III. deploy a single data-center

* objective 2a: start and manage the consul process.
* objective 2b: interpret a consul agent configuration.
* objective 2c: configuration consul network addresses and ports.
* objective 2d: describe and configure agent join and leave behaviors.

### 24. Starting the consul process

Starting the consul process

General workflow to start consul:

1. Download consul binary: [binary](https://releases.hashicorp.com/consul/)
2. develop configuration file.
3. start the consul process.

* consul is started by running the consul agent command:

```bash
consul agent<flag>
```

* The provide flags wii dicte how consul is configured.

```bash
consul agent -config-file=/opt/consul/config.hlc
```

* consul agent is commonly started using a service manager
  * systemctl.
  * windows service manager.
* service manager will start the consul agent using a command line:
  * command line can include flags or ...
  * configurations can be part of the configuration file (most common).

```bash
consul agent -datacenter="aws" -bind="10.0.10.42" -data-dir=/opt/consul -config-file=/opt/consul/config.hlc
```

* consul server - dev mode
  * using `consul agent -dev` will start consul in dev server mode.
  * useful for starting a consul agent:
    * All persistence options are turned off.
    * ENables in-memory server.
    * COnnect is enabled.
    * gPRC port defaults to 8502.

### 28. Manage the consul process

* Restarting the consul service: `systemctl restart consul`
* Permanently removing the node:
  * Gracefully remove node from consul: `consul leave`
  * stop service: `systemctl stop consul`

Make consul service: `vim /lib/systemd/system/consul.service`

```text
[Unit]
Description=Consul Service Discovery Agent
After=network-online.target
Wants=network-online.target
[Service]
Type=simple
User=root
Group=root
ExecStart=/usr/bin/consul agent -server\
            -data-dir=/opt/consul \
            -config-dir=/etc/consul.d/ \
            -config-file=/etc/consul.d/consul.json
ExecReload=/bin/kill -HUP $MAINPID
KillSignal=SIGINT
TimeoutStopSec=5
Restart=on-failure
SyslogIdentifier=consul
[Install]
WantedBy=multi-user.target
```

### 29. consul agent configuration

Reference link: [link](https://developer.hashicorp.com/consul/docs/agent/config/config-files)

For example configuration file: `/etc/consul.d/consul.json`

```json
{
        "bootstrap": true,
        "server": true,
        "ui": true,
        "log_level": "INFO",
        "enable_syslog": true,
        "enable_local_script_checks": true,
        "datacenter": "nginx_caching",
        "bind_addr": "192.168.20.22",
        "node_name": "caching_config_server",
        "acl_datacenter": "nginx_caching",
        "acl_default_policy": "allow",
        "encrypt": "RjSGtKF+/6iA0w+SPImHAYarQfiP/keyIxlrsslXq6U="
}
```

Interpret a Consul Agent configuration

* Environment variables cannot be used to configure the Consul client.
* Key options in a **server** configuration file:
  * server(boolean) - is this a server agent or not?
  * datacenter(string) - what datacenter to join.
  * node(string) - unique name of agent (usually server name).
  * join/retry_join/auto-join(string) - what other servers/cluster to join.
  * client_addr/bind_addr/advertise_addr(string) - what IP/interface to use for Consul communications.
  * log_level(string) - level of logging (trace, debug, info, etc.)
  * encrypt(string) - secret to use for encrypt of Consul traffic (gossip)
  * data-dir(string) - provide a persistent directory for the agent to store state.

* Key options in a **service** configuration file:
  * name(string)<req> - logical name of the service (web, app1, etc.)
  * id(string) - unique ID for this service - unique per agent(web_server_1,etc.)
  * port(integer) - what local port is the service running on? (80, 8080, 443).
  * check(arguments) - define arguments for health-check.

### 31. Configure network and ports

|Application/clients|----ethernet_interface----OR-------> Consul API - 8500
                                                \-------> LAN Gossip - 8301
                                                \-------> DNS - 8600

* DNS:
  * Port 8600 might work fine in your environment.
  * But others might lack the ability to send DNS traffic to a non standard port (UDP 53).
  * Port below 1024 require to be run with root privileges.
    * We do NOT run Consul as a root-user.
  * We may need to set up forwarding using BIND or dnsmasq to forward requests received on 53 forward to 8600.

* Consul API:
  * `-bind` - interface that the consul agent itself uses.
  * `-advertise` - the interface that consul tells other agents and clients to use when connecting to the local agent.
  * useful for when Consul server agent nodes have multiple interfaces or if consul is behind a NAT device.

For example, consul configuration:

```text
bind = 10.0.4.56
advertise = 10.0.9.32
```

|Consul|(10.0.4.56)<----------|NAT device|(10.0.9.32)<------------|terminal|

### 32. Adding/removing consul agents to the cluster

Adding Servers:

* Consul servers can `join` the cluster using multiple methods.
  * Using command line: `consul join <host>` with <host> can be any member of ther cluster,client, server. -> NOT recommended for production deployment.
  * Using configuration file:
    * `-join`:
      * specify one or more agents to join(IPv4, IPv6 or hostname).
      * IF consul is unable to join specified agents, agent startup will fail.
    * `-retry_join`:
      * specify one or more agents to join(IPv4, IPv6 or hostname).
      * will continue retrying until successful.
      * ideal for automated deployments or when agents may start random order.
      * For example: "retry_join":["10.0.10.34","10.0.11.72"]
  * Using Cloud Auto-join:
* A Consul agent can join any node in the cluster.
  * gossip will propagate the updated membership state across the cluster.
* An agent that is already a member can join a different cluster.
  * The two clusters will be merged into a single cluster.

Removing servers:

* command line: `consul leave`
  * consul leave triggers a graceful leave and shutdown.
  * it ensures that other nodes see the agent as `left` rather than `failed`.
  * For servers, a consul leave affects the rafts peer-set, as consul will reconfigure the cluster to have fewer servers.

Determining the members of the cluster:

* Displays both servers and clients. run `consul members`:

```text
Node                   Address         Status  Type    Build  Protocol  DC             Segment
caching_config_server  127.0.0.3:8301  alive   server  1.8.7  2         nginx_caching  <all>
```

## IV. Objective 3: Register services and use service discovery

### 36. Section overview

* Objective 3a: Interpret a service registration.
* Objective 3b: Differentiate ways to register a single service.
* Objective 3c: Interpret a service configuration with health check.
* Objective 3d: Check the service catalog status from the output of the DNS/API interface or via consul UI.
* Objective 3e: Interpret a prepared query.
* Objective 3f: Use a prepared query.

### 37. Registering a consul service

What is a Service ? - a service is really any workload or an application that you are running in your organization or customers externally. For example: Web application, JAVA app, database, etc.

* How do I register a service in consul?
  * Register with the local agent using:
    * service definition file.
    * HTTP API.
  * Service registration typically happens when a new service is provisioned:
    * container is scheduled by kubernetes.
    * Instance is deployed via Terraform.
    * jenkins provisions new VMs on a VMware cluster.
  * Register with consul API:
    * Method: `POST`
    * Endpoint: `/v1/agent/service/register`
    * terminal: `curl --request PUT --data @payload.json https://consul.example.com:8500/v1/agent/service/register`
    * content of `payload.json` file:

        ```json
        {
            "service": {
                "name": "retail-web",
                "port":8080
            }
        }
        ```

  * Register with a service definition:
    * Define a service using a service definition file:
      * `.json`
      * `.hcl`
    * Multiple options to register the service using a service definition:
      1. Create a single file and set using the -config-file parameter.
      2. Place file inside of the -config-dir directory.
      3. run the `consul services register <service_file_name>` command using file.
      4. Execute a `consul reload` command after adding.

### 38. Creating a Service Definition

* File that defines a service to be registered in consul.
* Once registered, the service is added to the consul service registry as an available * service.
* PArameters included in the service definition may include:
  * Service name.
  * ID of the agent.
  * tags.
  * Ip address and port of the service.
  * Health checks.

For example:

```json
{
    "service":
    {
        "id": "nginx_caching_1",
        "name": "nginx",
        "address": "129.44.4.5",
        "port": 6000,
        "checks": [
            {
                "args": [
                    "curl",
                    "localhost"
                ],
                "interval": "30s",
                "timeout": "20s"
            }
        ],
        "meta": {
            "testcookie": "off",
            "modsecurity": "on"
        }
    }
}
```

Therein, `id` is name of the node (server/container), `name` is name of the service to be registered. `address` is Ip address/ interface for the service. `port` the service is running on. `checks` is health check for the service.

* Default:
  * `id` will be set to the Name if not set.
  * `address` will be set to the default address of consul agent.
* Default namespace for a registered service:
  * `<name>.service.consul`
  * `nginx.service.consul`
* Each ID should be unique per agent:
  * web-server-1
  * web-server-2
  * web-server-3
* Multiple nodes in the catalog providing the same service
  * provides high-availability.
  * only registered services passing health checks will be returned.

### 40. Configuring service health checks

* Health checks determine when the node or service is healthy
* Health checks can be created/ updated via API or a service config
* Health check configuration may include:
  * Name
  * Arguments based on the type of health check.

* types of health checks:
  * Application-level (service) health check.
  * System-level (node) health check.

* A service may have multiple health checks defined.
  * If any heath check(s) are failing, the node is omitted from service queries.
* By default, newly registered heath checks are set to 'critical'.
  * Ensures that services aren't added to service pool before they are confirmed to be heathy.

|Critical|-----> <health check>------------>|passing|--->|added to service pool|
    /\                  |
    ||------Failed------/

Types of heath checks:

* Script health check - execute a specified script.
* HTTP health check - Perform GET looking for a 2xx return code.
* TCP health check - make TCP connection with IP/PORT.
* TTL health check - Relies on app to report health to endpoint.
* Docker health check - Invoke app in a Docker container.
* gRPC health check - Probe a gPRC health check endpoint.
* Alias health check - Determine health/state of another registered service.

For example, Script health check:

```json
{
    "args": ["/opt/check_mem.py", "-limit", "256MB"]
}
```

HTTP health check:

```json
{
    "http":"https://localhost:5000/health",
    "method":"POST",
    "tls_skip_verify":false,
    "header":{"Content-type":["application/json"]},
    "body": "{\"method\":\"health"}"
}
```

### 43. Check service status from the catalog

* Multiple ways to determine the status of services.
  * DNS query - most commonly used.
  * API request - requires app integration: `curl --request GET http://127.0.0.1:8500/v1/catalog/service/nginx`
    * return:

    ```json
    [{"ID":"115cd8d5-7cb5-fb4d-6ca6-12610af2ae21","Node":"caching_config_server","Address":"127.0.0.3","Datacenter":"nginx_caching","TaggedAddresses":{"lan":"127.0.0.3","lan_ipv4":"127.0.0.3","wan":"127.0.0.3","wan_ipv4":"127.0.0.3"},"NodeMeta":{"consul-network-segment":""},"ServiceKind":"","ServiceID":"nginx_caching_1","ServiceName":"nginx","ServiceTags":[],"ServiceAddress":"127.0.0.1","ServiceTaggedAddresses":{"lan_ipv4":{"Address":"127.0.0.1","Port":80},"wan_ipv4":{"Address":"127.0.0.1","Port":80}},"ServiceWeights":{"Passing":1,"Warning":1},"ServiceMeta":{"modsecurity":"on","testcookie":"off"},"ServicePort":80,"ServiceEnableTagOverride":false,"ServiceProxy":{"MeshGateway":{},"Expose":{}},"ServiceConnect":{},"CreateIndex":903,"ModifyIndex":903}]
    ```

  * Consul UI -least commonly used.

## V. Access the consul key/value (KV)

* Objective 4a: Understand the capabilities and limitations of the KV store.
* Objective 4b: Interact with the KV store using both the Consul CLI and UI.
* Objective 4c: Monitoring KV changes using watch.
* Objective 4d: Monitoring Kv changes using envconsul and consul-template.

### 51. Introduction

What is the key/value store?

* Centralized key/value store that allows users to store objects.
  * distributed architecture - data is replicated across all server nodes.
  * installed with consul and always enabled.
  * Can story any type of data - no restriction on type of object.

* Consul K/V use cases:
  * commonly used to store configuration parameters and related metadata.
    * Accessed by the application at runtime.
    * Accessible by both server and client agents.

What the key/value store is NOT?

* Not a fill featured database like DynamoDB.
* Not an encrypted K/V store - data is not encrypted.
  * Store only non-sensitive data (use Vault instead).
* Does not have a directory structure.
  * Does support using / to organize data, though.
  * / is treated like any other character.
* Consul K/V data is stored in a single datacenter - no built-in replication between data-centers.

Additional Information:

* Object size limitation: 512kb per object.
* backup and recovery:
  * Use `consul snapshot save`
  * Use `consul snapshot agent` (enterprise)

* Design your K/V structure before you start using it:
  * sit down with your peers and outline what the K/V structure will look like.
  * Align the design to your application and infrastructure teams and operational use cases.

* Every K/V structure will be different, and it should be designed for the `current` and `future` use cases.

For example:
                    |kv|
       ______________|______________
      |       |         |           |
    |dev|   |qa|    |staging|   |production|
     _|_____
    |       |
  |app1|  |app2|
    |
    |----|parameter 1|
    |----|parameter 2|
    |----|parameter 3|

### 52. Interacting with consul K/V

HTTP API:

* Uses the /kv endpoint.
* Path will be created if it does not exist.

Adding data to consul K/V:

* Method: PUT
* response value is either `true/false` on whether the create/update was successful.

```bash
curl --request PUT -data 'enabled' https://127.0.0.1:8500/v1/kv/data/app4
```

Retrieving data from consul K/V

* method: GET
* response is base64 encoded.

```bash
curl  https://127.0.0.1:8500/v1/kv/data/app4 |jq
```

Output:

```json
[{"LockIndex":0,"Key":"data/app4","Flags":0,"Value":"YXRh","CreateIndex":1201,"ModifyIndex":1201}]
```

using base 64 to read value:

```bash
echo "YXRh" | base64 --decode
```

Command-line interface - Uses the `consul kv` command:

* put
* get
* delete
* export
* import

```bash
consul kv put data/app4 YXRh
consul kv get data/app4
consul kv delete data/app4
```

Limit access to the K/V:

* To limit access to the K/V, use consul ACLs.
* WIll protect access though all three interface.

### 55. Using consul watch to monitor changes

Reference link: [link](https://developer.hashicorp.com/consul/docs/dynamic-app-config/watches)

Watch provides a way to monitor for specific changes in Consul.

* Built-in to Consul - no additional binary/ configuration needed.
* Once the "view" of data is updated, a specific handler is invoked...
* ...Or just log it to STDOUT.

Handlers

* Can invoke a command (using a shell) when change is detected.
* Can also hit an HTTP endpoint.

Watch types:

* key - watch a specific KV pair.
* Key prefix - watch a prefix in the KV store.
* services - watch the list of available services.
* nodes - watch the list of node.
* service - watch the instance of a service.
* checks - watch the value of health checks.
* events - watch for a custom user event.

Configuring a Watch:

* Watches are implemented using blocking queries in consul API.
* Watches can be added to an `agent configuration`, causing it to be run once the agent is started...
* ...or it can be started outside of the agent using consul watch command.
* Even if only one entry changed, consul will return all the matching entries for the query.

### 56. using envconsul

repo: [link](https://github.com/hashicorp/envconsul)

* `envconsul` launches a a subprocess to set environment variables from data retrieved from consul.
* Separate binary that runs on the application server (Consul client).

* `envconsul` populates the ENV(s) and the applications reads the ENV(s)
  * Apps no longer need to read config files with sensitive data in clear text.
  * Retrieve data from the KV or about Consul services.
* Allows simplified application integration without the application knowing it's using consul to retrieve values.

For example:

```bash
consul kv put db01/DB_ADDRESS 10.2.23.98
consul kv put db01/DB_PORT 3306
consul kv put db01/DB_MAX_CONNS 50

envconsul -prefix db01 env
```

New environment variables:

```text
DB_ADDRESS 10.2.23.98
DB_PORT 3306
DB_MAX_CONNS 50
```

### 58. using consul template

* what is consul-template populates values from consul into the **file system** running the consul-template daemon.
* Separate binary that runs on the application server (Consul client).

* Using a pre-configured, templated file as an input.
* Outputs a file with data populated from consul.

For example: [repo](https://github.com/btkrausen/hashicorp/blob/master/consul/config.json.tmpl)
Download template file: `curl –silent -Lo /tmp/consul-template.zip
https://releases.hashicorp.com/consul-template/0.25.1/consultemplate_0.25.1_linux_amd64.zip`

Run command:

```bash
consul-template -template "config.json.tmpl:config.json" -once
```
