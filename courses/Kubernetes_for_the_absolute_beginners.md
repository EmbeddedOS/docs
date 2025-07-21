# Kubernetes for the Absolute Beginners - Hands-on

## 1. Intro

### 1. Introduction

- Top 5 surging IT operations skills, 2017-2021 (percentage consumption growth compared from 2017 to 2021)
  - Certified Kubernetes Administrator (CKA) - 842%
  - Server administration - 398%
  - Cisxo Nexus - 379%
  - Computer network - 202%
  - PowerShell - 183%

- Kubernetes is the go to platform for hosting production grade applications.

### 2. Course series

- Kubernetes for the absolute beginners
  - LAb environment.
  - Pre-requisites - YAML.
  - PODs, Deployments.
  - Networking Basics.
  - Services.
  - Micro-services architecture.
  - Demos.
  - Coding exercise.
- Kubernetes for administrators.
  - HA deployment.
  - Logging/monitoring.
  - Maintenance.
  - Troubleshooting.
  - Demos.
  - Kubernetes scheduler.
  - Application LifeCycle.
  - Security.
  - Core concept.
  - coding exercise.
- Kubernetes for developers.
  - Core concept: design, build, configure cloud native application.
  - Multi-Container PODs.
  - Logging & Monitoring.
  - Jobs.
  - Demos.
  - ConfigMaps.
  - Readiness.
  - POD design.
  - Services.

## 2. Kubernetes Overview

### 8. Containers Overview

- To understand Kubernetes, you must understand two things: `Container` + `Orchestration`.

- Containers:
  - Without container, deploying multi processes on hosts are called as `The matrix form hell`.
  - Why do you need containers?
    - Compatibility/Dependency.
    - Long setup time.
    - Different Dev/Test/Prod environments.
  - What can it do?
    |Container 1|   |Container 2|   |Container 3|   |Container 4|
    |Lib    Deps|   |Lib    Deps|   |Lib    Deps|   |Lib    Deps|

    |                           DOCKER                          |
    |                             OS                            |
    |                 Hardware Infrastructure                   |

    - With docker containers, you can run each component in a separate container, with its own libraries and its own dependencies all on the same VM and the OS but within separate environments or containers.
    - Containerize Applications.
    - Run each service with its own dependencies in separate containers.

- Sharing the kernel:
  - Docker can run any flavor of OS on top of it as long as they are all based on the same kernel.
  - For example, with Linux, If the underlying OS is Ubuntu, docker can run a container based on another distribution like Debian, Fedora, CentOS, etc.
  - You won't be able to run a Windows based on container on a docker host with Linux OS on it. not being able to run another kernel on the OS.

- Containers vs Virtual Machines
  - VM:
        |Container 1|   |Container 2|   |Container 3|   |Container 4|
        |Lib    Deps|   |Lib    Deps|   |Lib    Deps|   |Lib    Deps|
        |    OS     |   |    OS     |   |    OS     |   |    OS     |
        |                         Hypervisor                        |
        |                             OS                            |
        |                 Hardware Infrastructure                   |

    - The virtual machines consume higher utilization, disk space and is usually in GB ins size, whereas Docker containers are lightweight and are usually in MB.
    - This allows containers to boot up faster usually in a matter of seconds whereas VM.
    - It is also important to note that docker has less isolation as more resources are shared in containers, like the kernel, whereas Vms have complete isolation from each other. Since mediums don't rely on the underlying OS or Kernel, you can have different types of OS such as Linux based, Windows based on the same hypervisor whereas it is not possible on a single docker host.

### 9. Container Orchestration

- How do you run containers in production?
- What if your application relies on other containers such as DB or messaging services or other backend services?
- What if the number of users increase and you need to scale your application?
- How do you scale down when the load decreases?

- To enable these functionalities, you need underlying platform with a set of resources and capabilities. The platform needs to orchestrate the connectivity between the containers and automatically scale up or down based on the load.
- This whole process of automatically deploying and managing containers is known as `Container Orchestration`.

- Orchestration Technologies:
  - Kubernetes is just a container orchestration technology.
  - There are multiple such technologies available today.
  - Docker has its own tool called Docker Swarm (Easy to setup and get started.)
  - Docker swarm lacks some of the advanced features required for complex applications.
  - Kubernetes is a bit difficult to set up and get start by provides a lot of options to customize deployments and supports deployment of complex architectures.
  - Kubernetes is now supported on all public cloud service providers like GCP, Azure and AWS, etc.
- Kubernetes Advantage:
  - Your application is now highly available as hardware failures to not bring your application down because we have multiple instances of your application running on different nodes.
  - The user traffic is load balanced across the various containers.
  - When demand increases, deploy more instances of the applications seamlessly and within a matter of seconds.
  - And we have the ability to do that at a service level when we run out of hardware resources, scale the number of underlying nodes up or down without having to take down the application and do all of these easily with a set of declarative object configuration files.
  - And that is Kubernetes, that orchestrate the deployment and management of hundreds and thousands of containers on a clustered environment.

### 10. Kubernetes Architecture

- Nodes:
  - A node is a machine, physical, or virtual on which Kubernetes is installed. A node is a worker machine and that is where containers will be launched by Kubernetes. It was also known as `minions` in the past.
  - If the node which your application is running fails? So you need to have more than one nodes. A `cluster` is a set of nodes grouped together. This way even if one node fails you your application still accessible from other nodes. Moreover having multiple nodes helps in sharing load as well.

- Master:
  - Now we have a cluster. But who is responsible for managing the cluster?
  - Where is the information about members of the cluster stored?
  - How are the nodes monitored?
  - When a node fails how do you move the workload of the field node to another worker node?
  - Thats where the Master comes in.

  - The master is another node with Kubernetes installed in it and is configured as a Master. The master watches over the nodes in the cluster and is responsible for the actual orchestration of containers on the worker nodes.

- Components:
  - When you install Kubernetes on a system, you are actually installing the following components:
    - An API server: Acts as the front end for Kubernetes: users, management devices, command line interfaces, all talk to the API server to interact with Kubernetes cluster.
    - `etcd` service: Key-value store. `etcd` is a distributed reliable key-value store used by Kubernetes to store all data used to manage the cluster. `etcd` is responsible for implementing locks within the cluster to ensure that there are no conflicts between the Masters.
    - `kubelet` service: is the agent that runs on each node in the cluster. Is responsible for making sure the containers are running on the nodes as expected.
    - Container Runtime: software is used to run containers, In our case it happens to be Docker.
    - Controllers: The controllers are the brain behind orchestration. They are responsible for noticing and responding when nodes, containers or end points goes down.
    - Schedulers: is responsible for distributing work or containers across multiple nodes. It looks for newly created containers and assigns them to node.

- Masters vs Worker Nodes:
  - How are these components distributed across different types of servers?
  - In other words, how does one server become a master and the other the slave?
  - The worker node or minion as it is also know is where the containers are hosted.
  - There are many container runtime engine: `rkt`, `CRI-O`, `docker`. In most case, we are going to use Docker as our container runtime engine.
  - The master server has the Kube API server and that is what makes it a master.
  - Similar, the worker nodes have `Kubelet` agent that is responsible for interacting with a master. To provide health information of the worker node and carry out actions requested by the Master on the worker nodes.
  - All the information gathered are stored in a key value store on the master.
  - The key value store is based on the popular `etcd` framework as we just discussed.
  - The master also has the control manager and the Scheduler.

    |</>Kube-APIserver|<------------------------>|</>Kubelet        |
    |       etcd      |                          |Container runtime |
    |    controller   |                          |                  |
    |    scheduler    |                          |                  |
    |-----------------|                          |------------------|
    |      Master     |                          |    Worker node   |

- `kubectl`
  - command line utilities know as the kube command line tool.
  - Is used to deploy and manage applications on a Kubernetes cluster.
  - Get cluster info, status of other nodes, manage many other things.

## 3. Setup Kubernetes

### 12. Kubernetes Setup - introduction and MiniKube

### 13. Setup

link install `kubectl`: [link](https://kubernetes.io/docs/tasks/tools/install-kubectl-linux/)

Install `minikube`: [link](https://minikube.sigs.k8s.io/docs/start/)

Kubernetes tools: [link](https://kubernetes.io/docs/tasks/tools/)

## 4. Kubernetes Concepts

### 16. PODs

- Assumptions:
  - The application is already developed and built into Docker Images and it is available on a Docker repository like Kubernetes can pull it down.
  - We also assume that the Kubernetes cluster has already been setup and is working.

- POD:
  - The containers are encapsulated into a Kubernetes object known as pods.
  - A pod is a single instance of an application.
  - A pod is a smallest object that you can create in Kubernetes.

  - To scale up, you create new pods and to scale down you delete existing pod. You do not add additional containers to an existing pod to scale your application.

- Multi-Container PODs:
  - Pods usually have a one to one relationship with the containers.
  - But are we restricted to having a single container in a single pod? NO. A single pod can have multiple containers expect for the fact that they are usually not multiple containers of the same kind. If our intention was to scale our application, then we would need to create additional pods.
  - But sometimes you might have a scenario where you helper container that might be doing some kind of supporting task for our web application, such as processing a user, enter data, processing a file uploaded by the user, etc. and you want these helper containers to live along side your application container. In that case, you can have both of these containers part of the same pod, so that when a new application container is created, the helper also created, and when it dies, the helper also die since they are part of the same pod.
  - The two containers can also communicate with each other directly by referring to each other as local hosts since they share the same network space. Plus they can easily share the same storage space as well.

- `kubectl`:
  - `kubectl run nginx` this command deploy a container by creating a pod. It first creates a pod automatically and deploys an instance of the nginx docker image.
    - But where does it get the application image from ?
      - For that, you need to specify the image name using the image parameter. In this case, the nginx image is downloaded from the Docker Hub Repository. You could configure Kubernetes to pull the image from the public Docker hub or a private repository within the organization. `kubectl run nginx --image nginx`
  - `kubectl get pods` help us see the list of pods in our cluster.
