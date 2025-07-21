# Docker for the Absolute Beginner

## I. Introduction

### 1. Introduction

Objective:

* What are Container?
* What is Docker?
* Why do you need it?
* What can it do?

* Run Docker Containers
* Create a Docker image
* Networks in Docker

* Docker Concepts in Depth

* Docker for Windows/MAC

* Docker Swarm
* Docker vs Kubernetes

### 2. Docker overview

**Why do you need docker?**

We need to build a system including a web server - Node Js, Database - MongoDB, Messaging -Redis , etc. Each component needs libraries, dependencies, OS, or Hardware Infrastructure, etc.. We have issues where one service requires one version of a dependent library whereas another service requires another version. The architecture of our application changed over time. We have had to upgrade to newer versions of these components or change the database etc. And every time something changed we had to go through the same process of checking compatibility between these various components and the underlying infrastructure.

This compatibility metrics issue is usually referred to as the `Matrix from HELL`.

Next, every time we had a new developer on board, we found it really difficult to set up a new environment.

The new developers had to follow a large set of instructions and run hundreds of commands to finally setup their environment. We had to make sure they were using the right OS the right versions of each of these components.

We had different development test and production environments.

etc.

So we needed something that could help us with the compatibility issue and something that will allow us to modify or change these components without affecting the other components and even modify the OS as required. And that search landed us on Docker.

**What can I do?**

* COntainerize Applications.
* Run each service with its own dependencies on separate containers.

With Docker, I was able to run each component in a separate container with its own dependencies and its own library. All on the same VM and the OS but within separate environments or containers. We just had to build the docker configuration once and all our developers could now get started with a simple Docker run command irrespective of what the underlying OS they run.

All they needed to do was to make sure they had Docker installed on their systems.

**What are containers?**

Containers are completely **isolated environments**. As in they can have their own processes or services their own network interfaces their own mounts just like `virtual machines` except they all share the same OS kernel.

Each docker container only has the additional software that makes these OSes different.

The main purpose of Docker is to package and containerized applications and to ship them and to run them anywhere any times as you want, so that brings up to the differences between VM and containers.

How is it done?

```text
docker run ansible
docker run mongodb
docker run redis
docker run nodejs
docker run nodejs
docker run nodejs

```

Container vs image

Image is a package or a template just like a VM template, It is used to create one or more containers. Containers are running instances of images that are isolated and have their own environments and set of processes.

You could create your own image and push it to Docker Hub repository, making it available for public.

Traditions developers developed applications. Then they handed over to ops team to deploy and manage it in production environments. They do that by providing a set of instructions such as information about how the host must be set up, what prerequisites are to be installed on the host and how the dependencies are to be configured, etc. Since the ops tram did not really develop the application on their own they struggle with setting it up when they hit an issue, they work with the developers to resolve it.

With Docker, developers and operations teams work hand-in-hand to transform the guide into a docker file with both of their requirements. This Docker file is then used to create an image for their applications. This image can now run on any host with docker installed on it and is guaranteed to run the same way everywhere. So the ops team can now simply use the image to deploy the application since the image was already working when the developer built it and operations are have not modified it. It continues to work the same wat when deployed in production and that is one example of how to like Docker contributes to the devops culture.

### 3. Getting Started with Docker

Dockers editions: Community Edition and Enterprise Edition.

### 5. Setup and install docker

Remove old docker version:

```bash
apt-get remove docker docker-engine docker.io containerd runc
```

Ways to install docker:

* Install Docker Desktop on Linux, goto Ubuntu softwares and download it.
* using the repository:

    ```bash

    sudo apt-get update
    sudo apt-get install \
        ca-certificates \
        curl \
        gnupg \
        lsb-release

    # Add Docker’s official GPG key:
    sudo mkdir -p /etc/apt/keyrings
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
    
    # Use the following command to set up the repository:
    echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

    # Install Docker Engine
    sudo apt-get install docker-ce docker-ce-cli containerd.io docker-compose-plugin
    
    #Verify that the Docker Engine installation is successful by running the hello-world image:
    sudo docker run hello-world
    ```

* Install from a package:
    1. Go to [link](https://download.docker.com/linux/ubuntu/dists/.) Select your Ubuntu version in the list.

    2. Go to pool/stable/ and select the applicable architecture (amd64, armhf, arm64, or s390x).

    3. Download the following deb files for the Docker Engine, CLI, containerd, and Docker Compose packages:

       * `containerd.io_<version>_<arch>.deb`
       * `docker-ce_<version>_<arch>.deb`
       * `docker-ce-cli_<version>_<arch>.deb`
       * `docker-compose-plugin_<version>_<arch>.deb`

    4. Install the .deb packages. Update the paths in the following example to where you* downloaded the Docker packages.

        ```bash
        sudo dpkg -i ./containerd.io_<version>_<arch>.deb \
        ./docker-ce_<version>_<arch>.deb \
        ./docker-ce-cli_<version>_<arch>.deb \
        ./docker-compose-plugin_<version>_<arch>.deb
        ```

    5. Verify the docker.

    ```bash
   sudo docker run hello-world
    ```

## II. Docker Commands

### 9. Basic Docker Commands

All docker command: [link](https://docs.docker.com/engine/reference/commandline/)

run - start a container.
ps - list containers.
stop - stop a container.
rm - remove a container.
images - list images.
rmi - remove images.
pull -download a image.
exec - execute a command.

Run attach and detach. Run docker with `-d` option will run container in background mode. Or run `docker attach id_of_container`

[Docker Hub](https://hub.docker.com/) is the world's largest library and community for container images.

## III. Docker run

### 16. Docker Run

## IV. Docker images

### 20. Docker image

* What am I containerizing?
* How to create my own image?
  * OS-ubuntu
  * Update apt repo
  * Install dependencies using apt
  * Install Python dependencies using pip
  * Copy source code to /opt folder
  * Run the web server using "flask" command

* build image with docker build command and register it with docker push.

Dockerfile:

All dockerfile instruction: [link](https://docs.docker.com/engine/reference/builder/)
INSTRUCTION ARGUMENT

For example:

```dockerfile
FROM ubuntu:18.04
RUN apt-get update && apt-get -y install python
RUN pip install flask flask-my-sql
COPY . /opt/source-code
ENTRYPOINT FLASK_APP=/opt/source-code/app.py flask run
```

therein `FROM` specify starting from a base OS or another image, Every Docker image need to base something. `RUN` instructions specify installing all dependencies on the image. the `COPY` instruction copy files on the local system to the image. Finally, `ENTRYPOINT` instruction.

Layered architecture: five layers are defined:

docker build . -f Dockerfile -t larva_image/tag:2.0

* Layer 1. Base on Ubuntu layer - 120MB
* Layer 2. Changes in apt packages - 306MB
* Layer 3. Changes in pip packages - 6.3MB
* Layer 4. Source code - 229B
* Layer 5. Update Entrypoint with "flask" command - 0B

### 21. Create a Docker image

### 23. Environment Variable

To send environment variable to container when running using `-e` flag:

```bash
docker run -e IMAGE_NAME=larva larva_image
```

Inspect the environment variables with docker inspect:

```bash
docker inspect larva_image
```

### 25. Command vs entry point

Docker `CMD` vs `ENTRYPOINT`

When we run a docker image, for example: `docker run ubuntu` the docker run an instance of `ubuntu` image. The command is `/bin/bash`. When the task (the command `/bin/bash`) is complete, the container exits.

For example, in the `nginx` dockerfile the command to run image is `CMD ["nginx"]`.

the `CMD` stands for command that defines the program that will run within the container.

However, if you add an argument while starting a container, it overrides the `CMD` instructions given in Dockerfile. `ENTRYPOINT` is the another instruction used to specify the behavior of container once started. Just like with CMD, we need to specify a command and parameters. However, in the case of `ENTRYPOINT` we cannot override the `ENTRYPOINT` instructions by adding command-line parameters to the `docker run` command. By opting for this instruction method, you imply that the container is specifically built for certain use-cases where command should not be overridden.

The first param in JSON list of the `CMD` or `ENTRYPOINT` should be executable:

```dockerfile
ENTRYPOINT ["sleep", "5"]
# Do NOT
ENTRYPOINT ["sleep 5"]
```

Because, we can specific params `:1` from the `docker run` command. For example:

```bash
docker run ubuntu 10
# instead of
docker run ubuntu sleep 10
```

## V. Docker compose

### 27. Docker compose

Compose is a tool for defining and running multi-container Docker applications. With Compose, you use a `YAML` file to configure your application's services. Then with a single command, you create and start all the services from your configuration.

Compose works in all environments: production, staging, development, testing as well as CI workflows. It also has commands for managing the whole lifecycle of your application:

* Start, stop and rebuild services.
* View the status of running services.
* Stream log output of running services.
* Run a one-off command on a service.

Docker compose - build:

```docker-compose.yml
redis:
  build: ./redis.dockerfile
```

This time when you run the docker-compose, it will first build the images, and then use those images to run containers.

Docker compose file versions and upgrade:

* version 1
  * This is specified by omitting a version key at the root of the YAML.
  * Compose does not take advantage of networking when you use version 1: every container is placed on the default `bridge` network and is reachable from every other container at its IP address.
  * You need to use `links` to enable discovery between containers.

    ```yml
    web:
      build: .
      ports:
      - "8000:5000"
      volumes:
      - .:/code
      links:
      - redis
    redis:
      image: redis
    ```

* version 2

  * Compose files using the version 2 syntax must indicate the version number at the root of the document. All `services` must be declared under the `services` key.
  * Named `volumes` can be declared under the `volumes` key, and networks can be declared under the `networks` key.
  * By default, every container joins an application-wide default network, and is discoverable at a hostname that’s the same as the service name. This means `links` are largely unnecessary.
  * Version 1 to 2.x In the majority of cases, moving from version 1 to 2 is a very simple process:
    * Indent the whole file by one level and put a `services`: key at the top.
    * Add a `version: '2'` line at the top of the file.
    * `dockerfile`: This now lives under the `build` key:

        ```yml
        build:
          context: .
          dockerfile: Dockerfile-alternate
        ```

    * `log_driver`, `log_opt`: These now live under the `logging` key:

        ```yml
        logging:
          driver: syslog
          options:
            syslog-address: "tcp://192.168.0.42:123"
        ```

    * `net`: This is now replaced by `network_mode`:

        ```yml
        net: host    ->  network_mode: host
        net: bridge  ->  network_mode: bridge
        net: none    ->  network_mode: none
        ```

  * For example:

    ```yml
    version: "2.4"
    services:
      web:
        build: .
        ports:
        - "8000:5000"
        volumes:
        - .:/code
        networks:
        - front-tier
        - back-tier
      redis:
        image: redis
        volumes:
        - redis-data:/var/lib/redis
        networks:
        - back-tier
    volumes:
      redis-data:
        driver: local
    networks:
      front-tier:
        driver: bridge
      back-tier:
        driver: bridge
    ```

* version 3
  * Designed to be cross-compatible between Compose and the Docker Engine’s swarm mode, version 3 removes several options and adds several more.
  * Removed: `volume_driver`, `volumes_from`, `cpu_shares`, `cpu_quota`, `cpuset`, `mem_limit`, `memswap_limit`, `extends`, `group_add`.
  * Added: `deploy`

    ```docker-compose.yml
    ```

## VI. Docker Registry

### 35. Docker Registry

* For example, when we run: `docker run nginx` actually the image get:
  * image: docker.io/nginx/nginx
  * `image: <registry>/<user/account>/<image/repository>`
  * For example: `gcr.io/kubernetes-e2e-test-images/dnsutils`

* Private registry:
  * Docker hub, Google Registry or internal private registry.
  * You first log into your private registry using `docker login your_registry` command and put your credentials.

* Deploy Private Registry
  * `docker run -d -p 5000:5000 --name registry registry:2`
  * Tag your image: `docker image tag my-image localhost:5000/my-image`
  * push your image: `docker push localhost:5000/my-image`
  * Pull your image from local: `docker pull localhost:5000/my-image`
  * Pull your image from another host: `docker pull 192.168.56.100:5000/my-image`

## VII. Docker Engine, Storage and Networking

### 37. Docker engine

When you install Docker in your host, you are actually installing three different competence:

* Docker Daemon: is a background process that manages Docker objects: images, containers, volumes, networks, etc.
* Docker Rest API: is the API interface that programs can use to talk to the daemon and provide instructions.
* Docker CLI: command line. CLI uses REST API to communicate with docker daemon.
  * Remote docker engine address: `docker -H=remote-docker-engine:2375`
    * example: `docker -H=10.123.2.1:2375 run nginx`

* containerization:
  * docker uses `namespace` to isolate workspace process:
    * process ID.
    * Network.
    * InterProcess.
    * Mount.
    * Unix timesharing.

* Namespace - PID:
  * Linux system:
    PID: 1 ------- PID: 2
              |--- PID: 3
              |--- PID: 4

  * Child System (Container)
    PID: 1 ------- PID: 2
              |--- PID: 3
    * It is an independent system on its own and it has its own set of processes originating from a root process.
    * But we know that there is no hard isolation between the containers and the underlying host. So the process running inside the container are in fact processes running on the underlying host.
    * Each process can have multiple process ideas associated with it. For example, when the processes start in the container it is actually just another set of processes on the base Linux system and it gets the available process I.D.
    PID: 1 ------- PID: 2
              |--- PID: 3
              |--- PID: 4
              |--- PID: 5 <---- Child system (container) PID: 1
              |--- PID: 6 <---- Child system (container) PID: 2
              |--- PID: 7 <---- Child system (container) PID: 3

* `cgroups`:
  * Containers shared the same system resources such as CPU and memory.
  * By default, there is no restriction as to how much of a resource a container can use and hence a container may end utilizing all of the resources on the underlying host.
  * But there is a way to restrict the amount of CPU or memory a container can use Docker using `cgroups` or control groups to restrict the amount of hardware resources allocated to each container.
  * For example: `docker run --cpus=.5 ubuntu` this will ensure that the container does not take up more than 50 percent of the host CPU at any given time.
  * With memory, for example: `docker run --memory=100m ubuntu`.

### 39. Docker storage

* How a docker stores data on the local File system when you install Docker on a system?
  * It create this folder structure at `/var/lib/docker/` you have multiple folders under it called.
    * `/var/lib/docker/`
      * `aufs`
      * `containers`
      * `image`
      * `volumes`

* Docker Layered Architecture.
  * Example
    Dockerfile1:

    ```Dockerfile
    FROM Ubuntu
    RUN apt-get update && apt-get -y install python
    RUN pip install flask flask-mysql
    COPY . /opt/source-code
    ENTRYPOINT FLASK_APP=/opt/source-code/app.py flask run
    ```

    `docker build Dockerfile1 -t larva/custom-app`

    * Image Layers (read only):
      * Layer 1: base Ubuntu Layer - 120M
      * Layer 2: Changes in apt packages - 306M
      * Layer 3: changes in pip packages - 6.3M
      * Layer 4: Source code - 229B
      * Layer 5: Update Entrypoint - OB
    * Container layer (read-write):
      `docker run larva/custom-app`
      * Layer 6: Container layer.
  * Docker can reuse image layers. For example:
    Dockerfile2

    ```Dockerfile
    FROM Ubuntu
    RUN apt-get update && apt-get -y install python
    RUN pip install flask flask-mysql
    COPY app2.py /opt/source-code
    ENTRYPOINT FLASK_APP=/opt/source-code/app2.py flask run
    ```

    `docker build Dockerfile2 -t larva/custom-app-2`

    * Image Layers (read only):
      * Layer 1: base Ubuntu Layer - 0M (reuse)
      * Layer 2: Changes in apt packages - 0M (reuse)
      * Layer 3: changes in pip packages - 0M (reuse)
      * Layer 4: Source code - 229B
      * Layer 5: Update Entrypoint - OB

  * Who is responsible for doing all of these operations: Maintaining the layered architecture, creating a writable layer moving files across layers to enable copy and write etc. It's the storage drivers.
  * Docker uses storage drivers to enable layered architecture.
  * Common storage drivers:
    * AUFS
    * ZFS
    * BTRFS
    * Overlay
    * Overlay2
    * Device Mapper
  * Docker will choose the best storage driver available automatically based on the OS.

### 42. Docker Networking

* Default networks:
  * bridge.
  * none.
  * host.

* When you install docker, it creates three networks automatically `bridge`, `none`, `host`.
* `bridge` is the default network a container gets attached to.

* The `bridge` network is a private internal network created by docker on the host.
  * All containers attached to this network by default and they get internal IP address usually in the 172.17.x.x series.
  * the containers can access each other using this internal Ip if required.
  * docker0: `172.17.0.1`
    * web container 1: `172.17.0.2`
    * web container 2: `172.17.0.3`
    * web container 3: `172.17.0.4`
    * etc.
  * To access any of these containers from outside world map the ports of these containers to ports on the docker host.

* Another way to access the containers externally is to associate the container to the `host` network.
  * This takes out any network isolation between the docker host and the docker container.
  * Meaning if you were run a web server on Port 5000 in a web container, it is automatically as accessible on the same port externally without requiring any port mapping as the web container uses the host network.

* With the `none` network, the containers are not a test to any network and doesn't have any access to the external network or other containers they run in an isolated network.

* User-defined networks:
  * By default, Docker only creates one internal bridge network. We could create our own internal network using the command:
  `docker network create --drive bridge --subnet 182.18.0.0/16 custom-isolated-network`
  * Run `docker network ls` command to list all networks.

* Inspect networks:
  * How do we see the network settings and the Ip address assigned to an existing container run the `docker inspect` command with ID or name of container.

* Embedded DNS:
  * containers can reach each other using their names.
  * docker0: `172.17.0.1`
    * web container 1: `172.17.0.2`
    * Mysql 2: `172.17.0.3`

    * How can I get my web server to access the database on the database container?
      * One thing I could do is to uses the internal Ip address assigned to the `MySQL`.
        * which in this case is 172.17.0.3.
      * But that is not very ideal because it is not guaranteed that the container will get the same IP address when the system reboots the right way to do it is to use the `container name`.
      * All containers in a docker host can resolve each other with the name of the container.
      * Docker has a built in a DNS server that helps the containers to resolve each other using the container name.
      * NOTE that the built in DNS server ALWAYS runs at address `127.0.0.11`.

* Docker uses network namespace, that creates a separate namespace for each container.
* It then uses virtual Ethernet pairs to connect containers together.

## IX. Container Orchestration - Docker Swarm & Kubernetes

### 48. Container Orchestration

* Why Orchestration?
  * To run a node JS based application, you are on the `docker run nodejs` command.
  * But that is just one instance of your application on one docker host.
  * What happens when the number of uses increase and that instance is no longer able to handle the load you deploy additional instance of your application by running the docker run command multiple times.
  * You have to keep a close watch on the load and performance of your application and deploy additional instance yourself.
  * And if a container was to fail, you should be able to detect that and run the docker run command again to deploy another instance of that application.
  * What about the health of the docker host itself?
  * What if the host crashes and is inaccessible, the containers hosted on that host become inaccessible too?
  * So what do you do in order to solve these issues?

* Container Orchestration is just a solution for that.
  * It is a solution that consists of a set of tools and scripts that can help host containers in a production environment.
  * Container Orchestration solution consist of multiple dockers hosts that can host containers. That way, if a container application run fail, the application is still accessible though the others a container orchestration.
  * Help you to deploy hundreds or thousands of instances of your application with a single command.

  ```bash
  docker service create --replicas=100 nodejs
  ```

  * some solutions can help to scale up the number of instances when users increase and scale down the number of instance when the demand decreases.

  * some of them also provide support for advanced networking between these containers across different hosts as well.
  * provide support for sharing storage between the host as well as support for configuration, security.

* common tools:
  * Docker Swarm.
  * Kubernetes.
  * MESOS.

### 49. Docker Swarm

* Docker Swarm could combine multiple Docker machines together into a single cluster Docker.
* We will take care of distributing your services or your application instances into separate hosts for high availability and for load balancing across different systems and hardware to set up a Docker swarm.

* in master host:
  `docker swarm init`

* in worker hosts:
  `docker swarm join --token <token>`

* docker service with replica to increase instances of containers on the docker cluster:

`docker service create --replicas=3 my-app`

### 50. Kubernetes

* With kubernetes using CLI know as Kube control, you can run a thousand instance of the same application with a single command.
`kubectl run --replicas=1000 my-app`

* can be scale up to two thousand instances with another command:
`kubectl scale --replicas=2000 my-app`

* POD Auto-scalers.
* Cluster Auto-scalers.
* Roll-back these with a single command:
`kubectl rolling-update my-app --image=web-server:2`
`kubectl rolling-update my-app --rollback`

* Can help you test new features of your application by only upgrading a percentage of these.
* Network.
* Storage.
* Security.

* consists set of nodes (machine physical or virtual).
