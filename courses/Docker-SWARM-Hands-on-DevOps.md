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
      - To promote an existing worker to become a manager node run `docker node promote` command.

- Can manager work?
  - By default, all manager nodes also worker nodes. Meaning, when you create a service the instances are also spread across manager node.
  - You can disable that and dedicate a node for management purpose alone. This can be done by running the command `docker node update --availability drain <node>`.
  - Docker recommends dedicating manager nodes for management tasks only in the production environment. However, if you simply need to set up a test or development environment and play around with Docker Swarm feel free to create a single node swarm cluster with the same node acting as both a manager and a worker.
