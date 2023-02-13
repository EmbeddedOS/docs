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
