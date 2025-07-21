# Working with Microservices in GO (Golang)

## 1. Introduction

### 1. Intro

- Monolithic applications.
  - Everything is handled by one application:
    - User authentication.
    - Sending email.
    - Logging.
    - All business logic.
- Distributed applications.
- Microservices.
  - Breaking monolith up from functions/packages to completely separate programs.
  - communicate via JSON/REST, RPC, gRPC, and over a messaging queue.
  - Easier to scale.
  - Easier to maintain.
  - Harder to write.
- Do one thing, and do it well -- the Unix philosophy.

- What we will build:
  - A front end web application that connects to 5 Microservices:
    - Broker - optional single point of entry to microservices.
    - Authentication - Postgres.
    - Logger - MongoDB.
    - Mail - send email with a specific template.
    - Listener -consumes messages in RabbitMQ and initiates a process.
  - we will communicate from between Microservices using:
    - REST API with JSON as transport.
    - Sending & Receiving using RPC.
    - Sending and Receiving using gRPC.
    - Initiating and responding to events using Advanced Message Queue Protocol (AMQP)

### 3. Installing GO

```bash

```

## 2. Building a simple front end and one Microservices

### 9. WHat we will cover in this sector

- Broker Service:

User---------------------------->broker
(via browser)                   (via docker)

### 12. Our first service: the Broker

```sh
# make broker module file.
go mod init broker
```
