# Explore Docker concepts in-depth with lectures, demos and hands-on coding exercises. Learn Swarm and Services in DevOps

## 1. Intro

### 1. Introduction

- Docker Advanced.
- Objectives:
  - Docker overview.
  - Running docker container.
  - Creating a docker image.
  - Docker compose.
  - Docker swarm.
  - Networking in Docker.
  - Docker Architecture.
  - Docker For Windows.
  - Docker Service.
  - OVerlay networks.
  - Load Balancing.
  - CI/CD Integration.

## 2. Recap - docker compose

### 3. Introduction

## 3. Docker Swarm

- We running containers on the single host, maybe good for dev and test environment, but in a production setup, this may not be a good idea.
- If the underlying host fails, we lose all the containers and our application goes down. This is where Docker Swarm comes into play.
- With docker swarm, you could now combine multiple Docker machines together into a single cluster. Docker swarm will take care of distributing your services into separate hosts for high availability.
- Docker swarm also helps us in balancing load across different systems and hardware. Now, we can run web services across different systems.

- How to setup Swarm?
  - First you must have hosts with Docker installed on them.
  - Then, you must designate one of the hosts to be the master or the Swarm manager and others as slaves or workers.
    - |host 1: Master| <-------> |host 2: Slave 1| <-----> |host 3: Slave 2|
  - Run: `docker swarm init` command on the Swarm manager node.
    - The output of that command will also provide the command to be run on the workers.
      - `docker swarm join --token <token>`
    - Copy the command and run it on the worker nodes to join the manager.
    - And you are now ready to create services and deploy them on the Swarm cluster.
    - If you forgot to save the command, run: `docker swarm join-token worker` to get join command.

- Docker Manager node
  - Is the master node where the swarm cluster is initiated.
  - The manager node is responsible for maintaining the cluster state, managing the workers, adding and removing workers, and creating, distributing, and ensuring the state of containers and services across all workers.
  - Having a single manager node is not recommended, because if it fails, there will no manager node to manage the cluster. You can have multiple manager nodes in a single cluster.
  - However, when you have multiple manager node, there arises a conflict of interest. To prevent that, only a single manager node is allowed to make management decisions. That node is called as `leader`.
  - Having said that, it is important node that the leader cannot always make a decision on its own. All decisions have to be mutually agreed upon by all the managers or the majority of the managers in the cluster. Docker make it by implementing the raft consensus algorithm.

- Distributed consensus - RAFT
- Docker Recommends - 7 managers
- No limit on manager.
- Should use odd managers.

- Cluster failure:
  - What happens when you don't have enough manages up and the cluster fails?
  - For example: 3-managers and 5-workers.
    - What if 2 managers failed at the same time?
    - the swarm is no longer to perform any more management tasks.
    - However, the existing worker nodes, their configuration and the services running on those worker nodes will continue to operate as they were supposed to. But you cannot perform any modifications on them like adding a new worker or updating the service configuration or creating, destroying or moving an existing service.
    - How do you resolve this situation?
      - the best way to recover from losing the quorum is to bring the failed nodes back online.
      - If we could not do it, the only way to recover is to force create a new cluster.
      - When we run the `docker swarm init --force-new-cluster` command. a new cluster is created with the current node as the only manager and we get a healthy cluster with a single manager node. Since this manager already has information about the services and tasks, the worker nodes are still part of the swarm and services continue to run.
      - You may later add more manager nodes or promote existing workers to become manager nodes.
      - To promote an existing worker to become a manager node run `docker node promote` command on the manager node.

- Can manager work?
  - By default, all manager nodes also worker nodes. Meaning, when you create a service the instances are also spread across manager node.
  - You can disable that and dedicate a node for management purpose alone. This can be done by running the command `docker node update --availability drain <node>`.
  - Docker recommends dedicating manager nodes for management tasks only in the production environment. However, if you simply need to set up a test or development environment and play around with Docker Swarm feel free to create a single node swarm cluster with the same node acting as both a manager and a worker.

## 4. Docker Service

### 14. Docker service in depth

- The key component of a swarm orchestration is Docker service.
- Docker services are one or more instance of a single application or service that runs across the Swarm cluster.
- For example, I could create a Docker service running multiple instances of my web server application across worker nodes in my Swarm cluster.
- We run: `docker service create --replicas=3 my-web-server` on the manager nodes. Use the option `replicas` to specify the number of instances of `my-web-server` I would like to run across the cluster.
  - `docker service create --replicas=3 -p 8080:80 my-web-server` with port mapping.
  - `docker service create --replicas=3 --network frontend my-web-server` to specify to a particular network.

- Tasks
  - When we run `docker service create` to create three instances of my-web-server. The orchestrator on the manager node decides how many tasks to create, and then the scheduler schedules that many tasks on each worker node.

  Manager node---> (|Task- web server| - node 1)
              ---> (|Task- web server| - node 2)
              ---> (|Task- web server| - node 3)

  - The task on the worker node is a process that kicks off the actually container instances. The task has a one-to-one relationship with each container. The task is responsible for updating the status of the container to the manager node so the manager node can keep track of the workers and instances running on them.
  - In case the container was to fail, the tasks fails as well, and if the task fail, the manager node becomes aware of it and it automatically reach a deals a new task to run a new container instance to replace the failed container.

- Replicas
  - Let's say you have only two worker node and you would like to create three replicas of an instance running the Docker service. `create` command will create three containers spread across two worker nodes.
  - The ultimate goal is to ensure that the requested number of instances running at all times.
  - If one of the worker node was to fail it will redeploy a new instance of that container on the other worker to ensure number of replicas of the service are available.

- Replicas vs global
  - There are two types of services replicated and global.
  - If you simply want one instance of a service placed on all nodes in the cluster.
  - A good example would be a monitoring agent or a lot collection agent or an antivirus scanner that you might want to run on every node in the cluster. Just one instance but on every node.
  - This is called **Global Services**.
  - For example, to place a monitoring agent on all my nodes, I will create a new service and using the command:
    - `docker service create --mode global my-monitoring-agent` to indicate that this service is a global service. And so one instance of my monitor agent gets placed on all the nodes in the cluster.

- Service name:
  - `docker service create --replicas=2 --name web-server my-web-server`
  - If you don't specify the service name, docker will automatically generate a random name.
  - If you specify service name, docker will generate `web-server.1`, `web-server.2`.

- Service update:
  - let's say we start with three services, so we run the command `docker service create` to create a service with three replicas. At a later point in time, we decide that we must have four instances running and so we must update the service to add a new instance.
  - To do this, run the `docker service update --replicas=4 web-server`.

### 15. Demo

- Get all service with: `docker service ls`
- If we do not specify the `replicas` option, default `replicas` is 1.
- Publish service ports externally to the swarm (-p, --publish)
  - You can publish service ports to make them available externally to the swarm using the --publish flag. The --publish flag can take two different styles of arguments. The short version is positional, and allows you to specify the published port and target port separated by a colon (:).
  - `docker service create --name my_web --replicas 3 --publish 8080:80 nginx`
- Add or remove published service ports (--publish-add, --publish-rm):
  - `docker service update --publish-add published=8080,target=80 my-service`

- Can manager work?
  - There is a way if you want to not host any containers on the master:
    - `docker node update --availability drain <node>`

## 5. Advanced Networking

### 17. Advanced Networking

- We discussed about three types of networks available in Docker:
  - Bridge (Default network mode): Is a private internal network. All containers get an internal IP address (usually 172.17.x.x series). To access to the containers, we need to map ports of these containers to ports on the Docker host.
  - None.
  - Host.

- With `docker swarm` you could create a new network of type overlay which  will create an internal network private network that spans across all the nodes.
  - `docker network create --driver overlay --subnet 10.0.9.0/24 my-overlay-networking`
- We could then attach the containers or services to this network using the network options.
  - `docker service create --replicas 2 --network my-overlay-networking`
  - And so we can get them to communicate with each other though the overlay network.

- ingress network:
  - In a single host, we can use publish - port mapping to access the container from host.
    - `docker run -p 80:5000 my-web-server`
  - But what happens when we are working with a swarm cluster?. For example, think of the host as single node swarm cluster. Say we were to create a web-server service with two replicas and a port mapping of port 80 to 5000.
    - `docker service create --replicas=2 -p 80:500 my-web-server`.
    - Since this is a single node cluster both the instances are deployed on the same node, this will result in two web-service containers both trying to map their 5000 ports to the common port 80 on the Docker host.
    - But we can have two mappings to the same port.
    - This is where increased networking comes into picture.
  - When you create a Docker Swarm it automatically creates an *ingress network*. The ingress network has a built-in load balancer that redirects traffic from the published port which in this case is port 80 to all the mapped ports which are the ports 5000 in on each container.
    external access-->(host-80)--> built-in load-balancer |---> my-web-server.1 - 5000
                                                          |---> my-web-server.2 - 5000
  - Since the ingress network is created automatically, there is no configuration that you have to do. You simply have to create the service you need by running the service:
    - `docker service create` and publish the ports you would like to publish.
    - But it is important for us to know how it really works when there are multiple nodes in the docker swarm cluster.
  - How this might work without the ingress networking?
      external access-->(192.168.1.5:80)|---> my-web-server.1 - 5000
                        (192.168.1.6:80)|---> my-web-server.2 - 5000
                        (192.168.1.7:80)|
    - First of all, how do we expect the user to access our services in a swarm cluster of multiple nodes? Since this is a cluster, we expect the users to be able to access services from any node in our cluster, meaning any user should be able to access the web server using the IP address of any of these containers since they are all part of the same cluster.
    - Without ingress networking, A user could access the web-server on nodes one and two but not on three because there is no web service instance running on node three.
  - With ingress networking, is in fact a type of overlay network meaning it's a single network that spans across all the nodes in the cluster.
    external-->LB-(192.168.1.5:80)|                                   ->|my-web-server.1 - 5000
               LB-(192.168.1.6:80)|--->(ingress network)(routing mesh)->|my-web-server.2 - 5000
               LB-(192.168.1.7:80)|
  - The way the load balancer works is it receives request from any node in the cluster and forwards that request to the respective instances on any other node, essentially creating a routing mesh. The routing mesh helps in routing the user traffic that is received on a node that isn't even  running an instance of the web service to other nodes where the instances are actually running.
  - *All of this is the default behavior of Docker Swarm* And you don't need to do any additional configurations. Simply create your service, specify the number of replicas and publish the port.
  - Docker swarm will ensure that the instances are distributed equally across the cluster the ports are published on all the nodes and the users can access the services using the IP of any nodes and when they do the traffic is routed to the right services internally.

- Embedded DNS
  - For example, I have a web service and a MySQL db service running on the same node or worker. How can I get my web service to access the database on the database container?
    - One thing I can do is to use the internal IP address that assigned to the MySQL container (For-example, 172.17.0.3), But that is not very ideal. Because it is not guaranteed that the container will get the same IP when the system reboots.
    - The right way to do it is to use the container name. All containers in the Docker host can resolve each other with the name of the container.
  - Docker has a built-in DNS server that helps the containers to resolve each other using the container name.
  - Note that the built-in DNS server always runs at address 127.0.0.11.

### 18. Docker stacks

- We have seen running individual containers with `docker run`.
- Instead of running multiple docker run commands, we were able to put together the application stack in a *Docker compose file* and bring our stack up with a single `docker-compose up` command.

- So how does it work when it comes to a swarm?
  - With docker swarm, we do the same thing with little difficult.
  - As we could run multiple instance of each service:
    - `docker service create mongodb`
    - `docker service create redis`
    - `docker service create ansible`
    - And orchestrate their deployment with services and stack.
  - We could convert the `docker service` command into a docker compose file.

    ```docker-compose.yml
    version: 3
    services:
      database:
        image: "mongodb"
      messaging:
        image: "redis:alpine"
    ```

  - Once we have that ready, we run `docker stack deploy` command to deploy the entire application stack all at once.

- STACK
  - A container, as we know is a packaged form of an application that has its own dependencies and run in its own environment.
  - A service is one or more instances of the same container that runs on a single node or across multiple nodes in a swarm cluster.
  - The next level on the hierarchy is a stack.
  - A stack is a group of interrelated services, that together forms an entire application.
         ^
        / \
       /   \
      /Stack\
     /Service\
    /Container\

    Stack ( Service web-server (container),Service db (container.redis.1, container.redis.2)).
  - This configuration makes my application distributed, easy to manage and easily scalable by component.

- Sample application swarm:
  - We have 3 nodes - 2 worker and 1 manager.
  - We would like to have one instance of Redis Running. So docker swarm places it anywhere in the cluster, say a worker node.
  - We would like to have one instance of Postgres db running. However, we would prefer it to be placed on the manager node. No particular reason, just to show some use case scenarios.
  - We would like two instances of voting application. as we expect high user traffic on that application.
  - One instance for result application.
  - And another instance for the worker.
  - Docker Swarm will place containers on an appropriate node unless we provide a preference.
  - And finally, we also would like to limit usage of underlying hardware resources by these services.
  - For example, we would like the Redis service to not consume more than 5% of the CPU resources or more than 50MB memory on the underlying host.
  - So to summarize, we have high expectations from running an application stack on Swarm.
  - We would like to run:
    - multiple instances of services.
    - Placement Preferences.
    - Resource Constrains.
  - Let's see how we do this in a stack configuration file, is defined in a YAML format.

- Stack definition - replica
  - New property for docker swarm: `deploy`

  ```docker-compose.yml
  version: 3
  services:
    redis:
      image: redis
      deploy:
        replicas: 1
        resources:
          limits:
            cpus: 0.01
            memory: 50M
    database:
      image: "postgres:9.4"
      deploy:
        replicas: 1
        placement:
          constrains:
            - node.hostname == node1
            - node.role == manager
    vote:
      image: voting-app
      deploy:
        replicas: 2
    result:
      image: result
      deploy:
        replicas: 1
    worker:
      image: worker
      deploy:
        replicas: 1
  ```

  - For Placement Preferences, If we don't specify, docker swarm will place the containers on any worker nodes.
  - To place the database service on a specific node, we could use the placement property under `deploy`, under `placement`, use a `constraint`. `constraints` are like conditions. for example, you could say things like: `node.hostname == node1`. This will ensure that the service gets placed on a node with its hostname node 1. And `node.role == manager` this will place the service on a node that has a role of a manager, which is what our requirement is in this case. You could specify multiple `constraints` for placing a service and it will attempt to match all of them to place a service on a node.
  - There are many more constraints available on the docker documentation page: [link](https://docs.docker.com/engine/swarm/services/#placement-constraints).

### 21. Docker visualizer

- run command: `docker run -it -d -p 8080:8080 -v /var/run/docker.sock:/var/run/docker.sock dockersamples/visualizer`
- Open web browser and access to port 8080.

## 7. CI/CD integration

### 22. CI/CD introduction

- How docker integrates with CI/CD pipeline?

- CI - Continuous integration

  Developer 1: Feature #1 \
  Developer 2: Feature #2  |--(Pushing their code into the same code repo)--> (Version control)
  Developer 3: Fix bug #1 /

  - When a developer pushes a code to the repo, it is linked to a build management system (like Jenkins for instance), that takes the code and builds it.
  - If your application is based on Java, then you can think of this as a `jar` file.
    (Version control - github)--trigger build-->(Build system - jenkins)
  - When the developers push your code into the same application, we would like to make sure that it builds correctly and does not introduce any new issues into the application. For this, we could configure the build system to run:
    - Test on the build package:
      - Unit test.
      - Web UI test.
      - Integration test.
      - etc.
  - Once the tests pass successfully, we can confirm that the build cycle is complete.
  - This whole process of enabling multiple developers to work seamlessly on the same application without stepping on each other's toes and at the same time ensuring, that these new changes integrate into the application without introducing any new issues. Is know as `Continuous integration`.

- CD - Continuous delivery/deployment
  - So far, we have coded our application, built and tested it all though automated pipeline using the code repositories and built and test systems.
  - What do we do once the testing is successful?
  - The next step is to release our new version of the application.
  - Now, that could be packaging the build application, an executable or an RPM package, ios, etc. And so make it online so users can download and deploy the application in their environments.
  - Release the software this way automatically though a pipeline is known as `Continuous Delivery`. (SERENA tool, for example)
  - So the next step would be to take the packaged application and automatically deploy it in a target environment. (Pivotal CF, Google Cloud Platform, AWS)
    - This process of automatically updating our production environment with the changes in the application is referred to as `Continuous Deployment`.

- All integrated and automated without requiring any manual intervention. This is referred to as CI/CD.

### 23. CI/CD - Docker integration

- How Docker integrates with CI/CD pipeline?
  - Build systems -docker support:
    - Each project has a Docker file checked into its code repository along with the rest of code, for the application.
    - The dockerfile as we have learned before, has instructions on building the Docker image. Once checked in to GitHub, Jenkins pulls the code, uses the Dockerfile path of the code to build the docker image.
    - You may use a supported Docker plugin for this purpose. On building the new docker image, jenkins will tag the image with a new build number.
    - On successfully, this image can then be used to run tests. Once the tests are successful, it can then be pushed to the image repositories, known as Docker registries either to a repository, internal to the company or external Docker Hub.
    - The image repository can then be integrated into a container hosting platform like Amazon ECS, to host our application.
    - This entire cycle of automated actions, from making a change in the application, to building, testing, releasing, and finally, deploying in production, completes CI/CD pipeline.

- Public cloud - docker support
  - The next step is to deploy this image in production.
  - Major cloud service providers like Amazon, Google, Azure, all supported containers.
  - Google Container Engine supports running containers, in production, on Kubernetes Clusters. Kubernetes is a container orchestration technology which is an alternate solution to Docker Swarm that we learned earlier.
  - AWS has ECS, which stands for EC2 Container Service. It provides another mechanism to run containers in production.
  - Pivotal Cloud Foundry, has PKS which stands for Pivotal Container Service which again uses Kubernetes underneath.
  - Finally, Docker's own container hosting platform, Docker Cloud, uses Docker Swarm underneath to orchestrate containers.

### 24. Docker Registry

- When we run the `docker build . -t larva/my-app`, docker creates an image of your application using the instructions specified in the Docker file.
- Once built, we could push the image to a repository called Docker Hub by running the docker push command: `docker push larva/my-app`.
- Docker hub is known as is the publicly-hosted repo for all docker images. The repository is known as Docker Registry.
- By default, `docker push` command push the images to Docker Hub. But you might not want to do that all the time. For example, enterprises may want to store their images in their local data center. To do that, you can setup your own Docker Registry internally.
- To setup an internal Docker registry, you can either install docker registry software on a server or an easier way to do it, run: `docker run -d -p 5000:5000 registry:2` and it is available on Docker Hub. You can build your image, and push it to private registry: `docker push localhost:5000/larva/my-app`.

## 8. Docker on CLoud

### 26. Docker Cloud
