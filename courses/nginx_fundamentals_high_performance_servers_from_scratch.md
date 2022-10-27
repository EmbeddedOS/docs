# NGINX Fundamentals: High Performance Servers from Scratch

## I. Overview

## II. Installation

### 4. Server Overview

### 6. Building Nginx from Source & Adding Modules

Check offical documents in the page: [Building NGINX from sources](http://nginx.org/en/docs/configure.html)

Building step:

* Step 1: Download nginx github repo: [NGINX](https://github.com/nginx/nginx.git)
* Step 2: Run configure: ```./auto/configure```
  * Check all option of the configure scripts: ```./auto/configure --help```
  * Specify the path of nginx executable file: ```--sbin-path=path```
  * Sets the name of an `nginx.conf` configuration file: ```--conf-path=path```
  * for instance: ```./auto/configure --sbin-path=/home/larva/Documents/Larva_ngx/bin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --with-pcre --pid-path=/var/run/nginx.pid --with-http_ssl_module```
* Step 3: Build nginx with `make` command.
* Step 4: Install nginx with `make install` command.

Note: If you are using nginx package, you should remove it before you install new nginx: 

```bash
sudo apt-get remove nginx
#Removes all but config files.
sudo apt-get remove nginx nginx-common
#Removes everything.
sudo apt-get purge nginx nginx-common
sudo apt-get autoremove

```

### 7. Adding an NGINX Service

#### 7.1. systemd System and Service Manager

**What is this?**

`systemd` is a suite of basic building blocks for a Linux system. It provides a system and service manager that runs as PID 1 and starts the rest of the system. `systemd` provides aggressive parallelization capabilities, uses socket and D-bus activation for starting services, offers on-demand starting of deamons, keeps track of processes using Linux control groups, maintains mount and automount points, and implements an elaborate transactional dependency-based service control logic.
Other parts include a logging daemon, utilities to control basic system configuration like the hostname, date, locale, maintain a list of logged-in users and running containers and virtual machines, system accounts, runtime directories and settings, and daemons to manage simple network configuration, network time synchronization, log forwarding, and name resolution.

#### 7.2. Nginx option

* `-h`: show help info.
* `-V`: show version and configure options then exit.
* `-t`: test configuration and exit.
* `-T`: test configuration, dump it and exit.
* `-s`: send signal to master process: stop, quit, reopen, reload.
  * For instance: ```./bin/nginx -s nginx```

### 7.3. Writing nginx service file

Save this file as `/lib/systemd/system/nginx.service`

```text
[Unit]
Description=The NGINX HTTP and reverse proxy server
After=syslog.target network-online.target remote-fs.target nss-lookup.target
Wants=network-online.target

[Service]
Type=forking
PIDFile=/var/run/nginx.pid
ExecStartPre=/home/larva/Documents/Larva_ngx/bin/nginx -t
ExecStart=/home/larva/Documents/Larva_ngx/bin/nginx
ExecReload=/home/larva/Documents/Larva_ngx/bin/nginx -s reload
ExecStop=/bin/kill -s QUIT $MAINPID
PrivateTmp=true

[Install]
WantedBy=multi-user.target
```

Detailed information: [click here](https://www.nginx.com/resources/wiki/start/topics/examples/systemd/)

And then start nginx service: ```systemctl start nginx```

Check nginx is active: ```ps aux | grep nginx```

Set-up nginx service auto start on booting: ```systemctl enable nginx```

## III. Configuration

### 9. Understanding Configuration Terms

Two main configuration term are **Context** and **Directive**.

**Directive** is a specification options that get, set  in the configuration file and consist of a name and a value. For example: ```server_name mydomain.com;```

**Context** is sections within the configuration where directives can be set for that given context. Context as a scope, and like scope, context are also nested and inherit from their parents with the **topmost context** simply being the configuration file itself.

**The configuration file context** is called the main context and is where we configure global directives that apply to **master process**.

For example of a configuration file:

```text
user www www;
worker_processes auto;
error_log logs/error.log;

eventd {
  worker_connections 4096;
}
http {
  index index.html index.php;
  server {
    listen 80;
  }

  location /some_path {
    add_header header_name header_value;
  }
}
```
