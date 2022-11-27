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

       * containerd.io_<version>_<arch>.deb
       * docker-ce_<version>_<arch>.deb
       * docker-ce-cli_<version>_<arch>.deb
       * docker-compose-plugin_<version>_<arch>.deb

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

INSTRUCTION ARGUMENT

For example:

```dockerfile
FROM Ubuntu
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

### docker command vs entry point
