# NGINX Fundamentals: High Performance Servers from Scratch

## I. Overview

## II. Installation

### 4. Server Overview

### 6. Building Nginx from Source & Adding Modules

Check official documents in the page: [Building NGINX from sources](http://nginx.org/en/docs/configure.html)

Building step:

* Step 1: Download nginx github repo: [NGINX](https://github.com/nginx/nginx.git)
* Step 2: Run configure: ```./auto/configure```
  * Check all option of the configure scripts: ```./auto/configure --help```
  * Specify the path of nginx executable file: ```--sbin-path=path```
  * Sets the name of an `nginx.conf` configuration file: ```--conf-path=path```
  * for instance: ```./auto/configure --sbin-path=/home/larva/Documents/Larva_ngx/bin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --with-pcre --pid-path=/var/run/nginx.pid --with-http_ssl_module --prefix=/home/larva/Documents/Larva_ngx/config/sites --user=larva --group=larva```
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

`systemd` is a suite of basic building blocks for a Linux system. It provides a system and service manager that runs as PID 1 and starts the rest of the system. `systemd` provides aggressive parallelization capabilities, uses socket and D-bus activation for starting services, offers on-demand starting of daemons, keeps track of processes using Linux control groups, maintains mount and auto-mount points, and implements an elaborate transactional dependency-based service control logic.
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

Two main configuration terms are **Context** and **Directive**.

**Directive** is a specification options that get, set  in the configuration file and consist of a **name** and a **value**. For example: ```server_name mydomain.com;```

**Context** is sections within the configuration where directives can be set for that given context. Context as a scope, and like scope, context are also nested and inherit from their parents with the **topmost context** simply being the configuration file itself.

**The configuration file context** is called the main context and is where we configure global directives that apply to **master process**.

For example of a configuration file:

```text
user www www;
worker_processes auto;
error_log logs/error.log;

event {
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

### 10. Creating a virtual host

Directive information: [link](http://nginx.org/en/docs/dirindex.html)
Book: nginx module reference

* **listen**:
  * syntax:
    * **listen** address[:port] [default_server] [ssl] [http2 | spdy] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
    * **listen** port [default_server] [ssl] [http2 | spdy] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
    * **listen** unix:path [default_server] [ssl] [http2 | spdy] [proxy_protocol] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
  * Default: **listen** *:80 | *:8000;
  * context: **server**
  * Description: Sets the `address` and `port` for IP, or the `path` for a UNIX-domain socket on which the server will accept requests. Both `address` and `port`, or only `address` or only `port` can be specified. An address may also be a `hostname`, for example:

    ```text
    listen 127.0.0.1:8000;
    listen 127.0.0.1;
    listen 8000;
    listen *:8000;
    listen localhost:8000;
    ```

    IPv6 addresses (0.7.36) are specified in square brackets:

    ```text
    listen [::]:8000;
    listen [::1];
    ```

    If only address is given, the port 80 is used.

    If the directive is not present then either `*:80` is used if nginx runs with the superuser privileges, or `*:8000` otherwise.

  * Parameters:
    * `default_server`: if present, will cause the server to become the default server for the specified `address:port` pair. If none of the directives have the default_server parameter then the first server with the `address:port` pair will be the default server for this pair.

    * `ssl`: allows specifying that all connections accepted on this port should work in SSL mode.

    * `http2`: The http2 parameter configures the port to accept HTTP/2 connections.

    * etc.

### 11. location context

Format:

```text
sever {
  location URI {
    # ... handle response
  }
}
```

Matching types:

|type|format|
|----|------|
|Exact Match| = uri|
|Preferential Prefix Match| ^~ uri|
|REGEX Match|~* uri|
|Prefix Match|uri|

* Syntax:
  * **location** [ = | ~ | ~* | ^~ ] uri { ... }
  * **location** @name { ... }

* Context: server, location
* Description: Sets configuration depending on a request URI.
  * The matching is performed against a normalized URI, after decoding the text encoded in the `%xx` form, resolving references to relative path components `.` and `..`, and possible compression of two or more adjacent slashes into a single slash.
  * A location can either be defined by a prefix string, or by a regular expression. Regular expressions are specified with the preceding `~*` modifier (for **case-insensitive matching**), or the `~` modifier (for **case-sensitive matching**).
  * To find location matching a given request, nginx first checks locations defined using the prefix strings (prefix locations). Among them, the location with the longest matching prefix is selected and remembered. Then regular expressions are checked, in the order of their appearance in the configuration file. The search of regular expressions terminates on the first match, and the corresponding configuration is used. If no match with a regular expression is found then the configuration of the prefix location remembered earlier is used.
  * Also, using the `=` modifier it is possible to define an exact match of URI and location. If an exact match is found, the search terminates. For example, if a `/` request happens frequently, defining `location = /` will **speed up the processing of these requests**, as search terminates right after the first comparison. Such a location cannot obviously contain nested locations.
  * **location** blocks can be nested.

Let’s illustrate the above by an example:

```text
location = / {
    [ configuration A ]
}

location / {
    [ configuration B ]
}

location /documents/ {
    [ configuration C ]
}

location ^~ /images/ {
    [ configuration D ]
}

location ~* \.(gif|jpg|jpeg)$ {
    [ configuration E ]
}
```

The `/` request will match configuration A, the `/index.html` request will match configuration B, the `/documents/document.html` request will match configuration C, the `/images/1.gif` request will match configuration D, and the `/documents/1.jpg` request will match configuration E.

The `@` prefix defines a named location. Such a location is not used for a regular request processing, but instead used for **request redirection**. They cannot be nested, and cannot contain nested locations.

### 12. Variables

Configuration Variables:

```text
set $var 'something';
```

Alphabetical index of variable: [link](http://nginx.org/en/docs/varindex.html), Nginx Module Variable:

```text
$http, $uri, $args
```

for example of using variables: `$uri` will return uri of processing request. `$args` will return all argument and value in uri of processing request. `$arg_name` will only return all argument name in uri of processing request.

### 13. Rewrites and redirects

format

```text
rewrite pattern URI
return status URI
```

Return normal response, we are using some data response:

```text
return 200 "some data response";
```

However, when we use `return` directive to redirect (status is 301, etc.), we use URI instead. For example:

```text
return 301 /some/path;
```

Instead of sending 301 status to clients to request them redirect, we can rewrite uri to process internal server. For instance:

```text
# Rewrite all uri that match `^/rewrite/\w+` pattern to `/(w+)`.
rewrite ^/rewrite/(\w+) /$1;

location = /lava {
        return 200 "Hello Lava";
}
```

### 14. try files and Named locations

`try_files`:

* syntax:
  * **try_files** file ... uri;
  * **try_files** file ... =code;
* context: server, location
* description:
  * checks the existence of files in the specified order and uses the first found file for request processing; the processing is performed in the current context. The path to a file is constructed from the file parameter according to the `root` and `alias` directives.
  * If none of the files were found, an **internal redirect** to the uri specified in the last parameter is made. For example:

    ```text
    location /images/ {
        # internal redirect to location /images/default.gif context.
        try_files $uri /images/default.gif;
    }

    location = /images/default.gif {
        expires 30s;
    }
    ```

  * The last parameter can also point to a named location, as shown in examples below. the last parameter can also be a `code`:

    ```text
    location / {
    # return 404 if does not found files.
    try_files $uri $uri/index.html $uri.html =404;
    }

    location / {
      # internal redirect to @mongrel named location.
      try_files /system/maintenance.html
                $uri $uri/index.html $uri.html
                @mongrel;
    }

    location @mongrel {
        proxy_pass http://mongrel;
    }
    ```

### 15. logging

### 16. Inheritance & Directive types

Directive types:

* `Array Directive`:
  * inside the main context (file scope).
  * Can be specified multiple times without overriding a previous setting. Gets inherited by all child context. Child context can override inheritance by re-declaring directive.
  * for example:
  
    ```text
    # array directives, off log by default.
    access_log off;
    error_log off;
    
    http {

    }
    ```

* `Standard Directive`:
* `Action directive`:
  * Invokes an action such as a rewrite or redirect.
  * Inheritance does not apply as the request is either stooped (redirect/response) or re-evaluated (rewrite).
  * For example:

    ```text
    return 403 "File not found";
    ```

### 17. PHP proccessing

### 18. Worker processes

worker_processes * worker_connections = max connections.

set ```worker_processes auto;``` nginx will automatically make number of processes equal to number of CPUs host.

### 19. Buffers & Timeouts

What a buffer is?

Buffering is when a process (nginx worker), in this case, reads data into memory or RAM before writing it to its next destination. For example, nginx receives a request which it reads from a TCP port, writes that request data to memory, which is buffering.

The opposite of this, then, is that nginx respond to a request with, for example, a static file, which it reads from disk into memory, so buffering the file and send that data to the client from memory.

This happens, as the implies, to create a buffer ot layer of protection between reading and writing of data, a process which can get extremely complicated, but for the purposes of configuring nginx, just have a understanding of what it is in order to better understand configuring these directives.

Timeouts

Time outs are pretty self-explanatory.They simply suggest a cutoff time for a given event. For example, of receiving a request from a client, stop a certain number of seconds, thus preventing a client from sending an endless stream of data and eventually breaking the server.

For example, using timeout & buffering:

```text
        # Buffer size for Headers.
        client_header_buffer_size 1k;

        # Max time to receive client header.
        client_header_timeout 12; # Milliseconds.


        # Buffer size for POST submissions.
        client_body_buffer_size 10k;
        client_max_body_size 2m;

        # Max time to receive client body.
        client_body_timeout 12; # Milliseconds.


        # Max time to keep a connection open for.
        keepalive_timeout 15; # Milliseconds.

        # Skip buffering for static files, don't use the buffer.
        # Read data from the disk and write it directly to the response with TCP no push enabling.
        sendfile on;

        # Optimize sendfile packets, options are enabled only when sendfile is used.
        tcp_nopush on;
```

### 20. Adding dynamic modules

Adding mod-security module.

* build lib mod-security.
* Rebuild executable nginx file with option `-add-dynamic-module=../modules/waf/connector`. For example:

  ```text
  ./auto/configure --sbin-path=/home/larva/Documents/Larva_ngx/bin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --with-pcre --pid-path=/var/run/nginx.pid --with-http_ssl_module --prefix=/home/larva/Documents/Larva_ngx/config/sites --user=larva --group=larva --add-dynamic-module=../modules/waf/connector
  make
  ```

* Or  build modules with: `make modules`

  ```text
  ./auto/configure --sbin-path=/home/larva/Documents/Larva_ngx/bin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --with-pcre --pid-path=/var/run/nginx.pid --with-http_ssl_module --prefix=/home/larva/Documents/Larva_ngx/config/sites --user=larva --group=larva --add-dynamic-module=../modules/waf/connector
  make modules
  ```

* Copy `.so` file to modules/ directory.
* add `load_module` directive to main configure:

  ```text
  load_module modules/ngx_http_modsecurity_module.so;
  ```

* using directives of module in corresponding context:

  ```text
  server {
    modsecurity on;
    modsecurity_rules_file /home/larva/Documents/Larva_ngx/modules/waf/config/main.conf;
  }
  ```

## IV. Performance

### 24. HTTP2

About HTTP2:

* Binary Protocol.
* Compressed Headers.
* Persistent Connections.
* Multiplex Streaming.
* Server push.

How to use it in the nginx?

* Rebuild binary with option `--with-http_v2_module`.
* In `listen` directive, adding `ssl` and `http2`. For example:
  
  ```text
  listen       443 ssl http2;
  ```
