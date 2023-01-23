# Microservices Architecture -- The complete Guide

## 1. Welcome

### 1. Course Introduction

- Why Microservices?
  - The Most Popular Architecture Paradigm.
  - Not Tied to specific technology.
  - Solves Real Problems.
  - In high demand in the job market.
  - Generates a Lot of Buzz...
- Companies using microservices: netflix, facebook, aws, uber, etc.
- But "If microservices are implemented incorrectly or used as a band-aid without addressing some of the root flaws in your system, you will be unable to do new product development because you are drowning in the complexity."

- By the end of this course:
  - You will know what microservices architecture is.
  - You will know what problems it solves.
  - You will know how to design a robust, scalable, easy to maintain, microservices architecture.
  - You will know when NOT to use microservices architecture.
  - **You will become a much better Software Architect!**

- At the end of this course:
  - Download the microservices checklist! - the ultimate practice, step-by-step guide to help you design great microservices architecture!

### 2. Who is this course for?

- Architects.
- Development background.
  - developer, team leader, dev manager...
- Preferably with some experience.
- Preferably with Backend Development Experience.
- Anyone who wants to learn about microservices architecture.

- Agenda:
    1. welcome
    2. History of MA.
    3. Problems with Monolith & SOA.
    4. Microservices Architecture.
    5. Problems solved by Microservices.
    6. Designing MA.
    7. Deploying Microservices.
    8. Testing Microservices.
    9. Service Mesh.
    10. Logging & Monitoring.
    11. When Not to Use Microservices.
    12. Microservices and the Organization.
    13. Anti-patterns and common mistakes.
    14. Breaking Monolith to Microservices.
    15. Case Study.
    16. Conclusion.

## 2. History of Microservices

### 6. Introduction

- Microservices are result of problems with two architecture paradigms:
  - Monolith.
  - SOA.

### 7. Monolith

- The original architecture.
- All software components are executed in a single process.
- No distribution of any kind.
- Strong coupling between all classes.
- Usually implemented as Silo.

- Example:
  - HR App ----> Process 1 {} ---> Database 1.
  - purchasing App ----> Process 2 {} ---> Database 2.

- Monolith Pros:
  - Easier to design.
  - Performance.

### 8. Service Oriented Architecture

- First coined in 1998.
- Apps are services exposing functionality to the outside world.
- Services expose metadata to declare their functionality.
- Usually implemented using SOAP & WSDL.
- Usually implemented with ESB.

- Example:
  - HR App ------------>| Enterprise Service Bus | ---->(SOAP) Process 1 {} ---> Database 1.
  - purchasing App ---->|                        | ---->(SOAP) Process 2 {} ---> Database 2.

- SOA pros:
  - Sharing Data & Functionality.
  - Polyglots between services.

## 3. Problems with Monolith & SOA

### 9. Introduction

- A lot of problems were found in both paradigms.
- Problems relevant to technology, deployment, cost and more.

### 10. Single Technology Platform

- With Monolith, all the components must be developed using the same development platform.
- Not always the best for task.
- Can't use specific platform for specific features.
- Future upgrade is a problem - need to upgrade the whole app.

### 11. Inflexible deployment

- With monolith, new deployment is always for the whole app.
- No way to deploy only part of the app.
- Even when updating only one component - the whole codebase is deployed.
- Forces rigorous testing for every deployment.
- Forces long development cycles.

### 12. Ineffective Compute Resources

- With Monolith, compute resources (CPU and RAM) and divided across all components.
- If a specific component needs more resources - no way to do that.

### 13. Large and complex

- With monolith, the codebase is large and complex
- Every little change can affect other components.
- Testing no always detects all the bugs.
- Very difficult to maintain.
- Might make the system obsolete.

### 14. Complicated and expensive ESB

- With SOA, the ESB is one of the main components.
- Can quickly become bloated and expensive.
- Tries to do everything.
- Very difficult to maintain.

### 15. Lack of tooling

- For SOA to be effective, short development cycles were needed.
- Allow for quick testing and deploying.
- No tooling existed to support this.
- No time saving was achieved.

## 4. Microservices Architecture

### 16. Introduction

- History:
  - Problems with monolith and SOA led to a new paradigm.
  - Has to be modular, with simple API.
  - The term "microservices" first appeared in  2011.
  - In 2014 Martin Fowler and James Lewis published their "Microservices". [Link](https://martinfowler.com/articles/microservices.html).
  - Became the de-facto standard for Microservices definition.

- Characteristics of Microservices:
  - Componentization via Services.
  - Organized Around Business Capabilities.
  - Products not Projects.
  - Smart Endpoints and Dumb Pipes.
  - Decentralized Governance.
  - Decentralized Data Management.
  - Infrastructure Automation.
  - Design for Failure.
  - Evolutionary Design.

### 17. Componentization via Services

- Modular design is always a good idea.
- Components are the parts that together compose the software.
- Modularity can be achieved using:
  - Libraries - called directly within the process (import, include, require, using, etc.).
  - Services - called by out-of-process mechanism (Web API, RPC).

- In Microservices we prefer using Services for the componentization.
- Libraries can be used inside the service.

- Motivation:
  - Independent deployment.
  - Well defined interface.

### 18. Organized Around Business Capabilities

- Tradition projects have teams with horizontal responsibilities - UI, API, Logic, DB, etc.
- For example:

|------UI Team A-----|
|-----API Team B-----|
|-----LOGIC TEAM C---|
|---DATABASE TEAM D--|

- The problem begins when these team need to talk with each other. Now, since the teams work on the same system they will have a lot of opportunities to talk to each other. Inter-group communication is by definition, slow and cumbersome. The teams don't use same terminology, they don't have the same schedule, and in the wort case they don't even have the same GOALS.

- With microservices, every service is handled by a single team, response for all aspects.

|------UI Team A-----|
|-----API Team A-----|
|-----LOGIC TEAM A---|
|---DATABASE TEAM A--|

- The team has one and only one GOAL.
- With Microservices, every service handles a well-defined business capability.

- Motivation:
  - Quick development.
  - Well-defined boundaries.

### 19. Products not Projects

- With traditional projects, the goal is to deliver a working code.
- No lasting relationship with the customer.
- Often no acquaintance with the customer.
- After delivering - the team moves on to the next project.

- With Microservices - the goal is to deliver a working product.
- A product needs ongoing support and requires close relationship with the customer.
- The team is responsible for the Microservice after the delivery too.
- 'You build it, you run it.' Werner Vogels, AWS CTO.

- Motivation:
  - Increase customer's satisfaction.
  - Change developers' mindset.

### 20. Smart endpoints and dumb pipes

- Traditional SOA projects used two complicated mechanisms:
  - ESB (Enterprise Service Bus).
  - WS-* protocol.

- Made inter-service communication complicated and difficult to maintain.
- Microservice systems use "dumb pipes" - simple protocols. We want to use the simplest form of communication possible and leave the complexity to the services logic itself.
- We strive to use what the web already offers.
- Usually - REST API, the simplest API in existence.
- Important notes:
  - Direct connections between services is not a good idea.
  - Better use discovery service or a gateway.
  - In recent years more protocols were introduced (GraphQL, gRPC), some of them quite complex.

- Motivation:
  - Accelerate development.
  - make the app easier to maintain.

### 21. Decentralized Governance

- In traditional projects there is a standard for almost anything:
  - Which dev platform to use.
  - Which database to use.
  - How logs are created.
  - And more.

- With microservices each team makes its own decisions:
  - Which dev platform to use.
  - Which database to use.
  - How logs are created.
  - And more.

- Each team is fully responsible for its service.
- "You build it, you run it."
- ... and so will make the optimal decisions.
- Enabled by the loosely coupled nature of Microservices.
- Multi dev platform is called Polyglot.

- Motivation:
  - Enables making the optimal technological decisions for the specific service.

### 22. Decentralized Data Management

- Traditional systems have a single database.
  - Store all the system's data from all the components.
- With Microservices each service has its own database.
- Important notes:
  - This is the most controversial attribute of Microservices.
  - Not always possible.
  - Raises problems such as distributed transactions, data duplication and more.
  - Don't insist on it.

- Motivation:
  - Right tool for the right task - having the right database is importance.
  - Encourages isolation.

### 23. Infrastructure Automation

- Infrastructure automation techniques have evolved enormously over the last few years - the evolution of the cloud and AWS in particular has reduced the operational complexity of building, deploying and operating microservices.

- Many of the products or systems being build with microservices are being built by team with extensive experience of `Continuous Delivery` and it's precursor, `Continuous Integration`. Team building software this way make extensive use of infrastructure automation techniques. This is illustrated in the build pipeline shown below:

  Compile, unit     acceptance     Integration      user acceptance     performance
    and               test            test              test              test
  functional test
-----|-------------------|--------------|-----------------|-----------------|-------------|Deploy to product.
  Run on build      deployed on     deployed to     deployed to UAT     deployed to
  machine           build machine   Integration     environment         performance
                                    Environment                         environment

- The SOA paradigm suffered from lack of tooling.
- Tooling greatly helps in deployment using:
  - Automated Testing.
  - Automated Deployment.

- For Microservices automation is essential.
- Short deployment cycles are a must.
- Can't be done manually.
- There are a lot of automation tools: Gitlab, Azure DevOps, Jenkins, etc.

- Motivation:
  - Short deployment cycles.

### 24. Design for failure

- With Microservices there are a lot of processes and a lot of network traffic.
- A lot can go wrong.
- The code must assume failure can happen and handle it gracefully.
- Extensive logging and monitoring should be in place.

- Solutions:
  - Catch the exception.
  - Retry.
  - Logging the exception.
  - Monitoring services: Kubernetes, Azure Monitor, Application Insight.

- Motivation:
  - Increase system's reliability.

### 25. Evolutionary Design

- The move to microservices should be gradual.
- No need to break everything apart.
- Start small and upgrade each part separately.

### 26. Summary

- These are guidelines, not mandatory instructions.
- Adopt what works for you.
- The Microservices world is rapidly changing.
  - Follow new APIs, monitoring, cloud services etc.
- The most important attributes:
  - Componentization.
  - Organized around business capabilities.
  - Decentralized governance.
  - Decentralized data management (when possible).
  - Infrastructure automation.

## 5. Problems Solved by microservices

### 27. Introduction

### 28. Single Technology Platform

- With Monolith, all the components must be developed using the same development platform.
- Not always the best for the task.
- Can't use specific platform for specific features.
- Future upgrade is a problem - need to upgrade the whole app.

- With Microservices, the Decentralized Governance attribute solves it.

### 29. Inflexible deployment

- WIth Monolith, new deployment is always for the whole app.
- No way to deploy only part of the app.
- Even when updating only one component - the whole codebase is deployed.
- Forces rigorous testing for every deployment.
- Forces long development cycles.

- With Microservices, the Componentization via Services attribute solves it.
- Also - Decentralized Data Management.

### 30. Inefficient Compute Resources

- With monolith, compute resource (CPU and RAM) are divided across all components.
- If a specific component needs more resources - no way to do that.
- Very inefficient.

- With Microservices, te Componentization via Services attribute solves it.

### 31. Large and Complex

- WIth monolith, the codebase is large and complex.
- Every little change can affect other components.
- Testing not always detects all the bugs.
- Very difficult to maintain.
- Might make the system obsolete.

- With Microservices, te Componentization via Services attribute solves it.
- Also - Decentralized Data Management.
- And - Organized around business capabilities.

### 32. Complicated and Expensive ESB

- With SOA, the ESN is one of the main components.
- Can quickly become bloated and expensive.
- Tries to do everything.
- Very difficult to maintain.

- WIth microservices, the Smart Endpoint and Dumb Pipes attribute solves it.
- Remember:
  - Application Gateway & Discovery.
  - Other APIs: GraphQL, gRPC.

### 33. Lack of tools

- For SOA to be effective, short development cycles were needed.
- Allow for quick testing and deployment.
- No tooling existed to support this.
- No time saving was achieved.

- With Microservices, the Infrastructure Automation attribute solves it.
- Automates testing and deployment.
- Provides short deployments cycles.
- Make the architecture efficient and effective.

## 6. Designing Microservice Architecture

### 34. Introduction

- Designing Microservice Architecture should be methodical.
- Do not rush into development.
- "Plan more, code less".
- Critical to the success of the system.

- The Architecture Process:
  1. Understand the System's Requirements.
  2. Understand the Non-functional Requirements.
  3. Map the components.
     1. Mapping.
     2. Communication.
     3. Patterns.
  4. select the technology stack.
  5. design the Architecture.
  6. Write Architecture Document.
  7. Support the Team.

### 35. Mapping the component

- The single most important step in the whole process.
- Determine how the system will look like in the long run.
- Once set - not easy to change.

- What is it?
  - Defining the various components of the system.
  - Remembers: Components = Services.

- Mapping should be based on:
  - Business requirements.
  - Functional autonomy.
  - Data entities.
  - Data autonomy.

- Business requirements.
  - The collection of requirements around a specific business capability.
  - For example: Orders management:
    - Add, remove, update, calculate amount.

- Functional autonomy:
  - The maximum functionality that does not involve other business requirements.
  - For example:
    - Retrieve the orders made in the last week. -> YES
    - Get all the orders made by users aged 34-45. -> NO
  
- Data Entities:
  - Service is designed around well-specified data entities.
  - For example: orders, items.
  - Data can be related to other entities but just by ID.
    - Example: Order stores the customer ID.

- Data autonomy:
  - Underlying data is an atomic unit.
  - Service does not depend on data from other services to function properly.
  - For example: Employees services that relies on Addresses service to return employee's data.

- Mapping the components - Example:
  - eCommerce System: Inventory, Orders, Customers, Payments.

|                     |Inventory        |Orders                     |Customers                |Payments         |
|---------------------|-----------------|---------------------------|-------------------------|-----------------|
|Business Requirements|Manage inventory |Manage orders              |Manage                   |Perform payments |
|                     |items            |                           |Customers                |                 |
|Functional           |Add, remove,     |Add, cancel,               |Add, update, remove,     |Perform payments |
|                     |update, quantity |calculate sum              |get account details      |                 |
|Data entities        |Items            |Orders, shipping           |Customer, address,       |Payment history  |
|                     |                 |address                    |contact detailed         |                 |
|Data Autonomy        |None             |Related to Items by ID     |Related to Orders by ID  |None             |
|                     |                 |Related to Customer by ID  |                         |                 |

- Edge case #1:
  - Retrieve all customers from NYC with total number of orders for each customer.
    - David-Smith - 16 orders.
    - Diane Rice - 23 orders.
    - George Murray - 22 orders.
  - The problem is this data is spread across two services.
  - Three approaches:
    - Data duplication: Customer -> DB (Customer + ORders).
      - Useful when:
        - very little data.
        - Readonly.
    - Service Query: Customers -> Orders.
    - Aggregation Service: Query -> customer, Query -> Orders.

- Edge case #2:
  - Retrieve list of all the orders in the system.
  - The problem lies in the volume of the data retrieved. The number of orders in the system will be huge, and retrieving it all in a single request will bring the service and the network to their knees.
  - Service are not designed for this scenario.
  - What should we do?
    - Find out what is the purpose of this query. For example, reporting.
    - Report engine is preferred mechanism for this. All report engines out there can work directly with the database by passing the service API and performing efficient querying against the database.

- Cross-Cutting Services
  - Services that provide system-wide utilities.
  - Common examples:
    - Logging.
    - Caching.
    - User management.
  - MUST be part of the mapping.

### 36. Defining Communication patterns

- Efficient communication between services is crucial.
- It's important to choose the correct communication pattern. Using the wrong pattern can lead to slow performance, unmaintainable system, and poor role handling.
- Main patterns:
  - 1-to-1 Sync.
  - 1-to-1 Async.
  - Pub-sub / Event Driven.

- 1-to-1 Sync
  - A service calls another service and waits for the response. The calling services process is blocked until a response is returned from the other service. This is called a synchronous request.
  - Used mainly when the first service needs the response to continue processing.
  - For example:

    Orders|----Is item in stock?------------>| Inventory
          |<-------------yes-----------------|

  - Pros:
    - Immediate response
    - Error handling.
    - Easy to implement.
  - cons:
    - performance.
  - Implementation:
    - Orders ---- API (REST, GraphQL, gRPC) ----> Payments.
    - Direct connection -> Not recommend.
  - Direct connection is not recommended for many services. -> Complex and depend to each other.
  - Solution:
    - Using Service Discovery, Services query to the Service Discovery to get URL that will be used to query to other service.
      - Services only need to know the Directory's URL: Consul
    - Gateway
      - Services only need to know the Gateway's URL.
      - For example: Service A ---> Gateway ---> Service E.

- 1-to-1 Async:
  - A service calls another service and continues working.
  - Doesn't for the response - Fire and Forget.
  - Used mainly when the first service wants to pass a message to the other services.
  - Pros:
    - Performance.
  - Cons:
    - Need more setup.
    - Difficult error handling.
  - Implementation:
    - Orders -----> Queue (RabbitMQ)-----> Payments.

- Pub-Sub / Event Driven.
  - A service wants to notify other services about something.
  - The service has no idea how many services listen.
  - Doesn't wait for the response - Fire and Forget.
  - Used mainly when the first service wants to notify about an important event in the system.
  - Orders ------(Order cancelled)----> Inventory.
                                  |---> Customers.
                                  |---> Payments.
  - Pros:
    - Performance.
    - Notify multiple services at once.
  - Cons:
    - Needs more setup.
    - Might cause load.
    - Difficult error handling.
  - Orders ------Pub/sub(Rabbit MQ)----> Inventory.
                                   |---> Customers.
                                   |---> Payments.
  - Other engines such as Azure Event Grid, Rabbit MQ.

- Communication Patterns Summary
  - Choosing the correct communication pattern is crucial.
  - Affects:
    - Performance.
    - Error handling.
    - Flow.
    - Almost impossible to reverse.

### 37. Selecting Technology Stack

- The decentralized governance allows selecting different technology stack for each service.
- We will focus on Backend platforms and Storage platforms.
- There is no objective "RIght" or "Wrong".
- Make it concrete decision based on hard evidence.

- Development Platform:
  - Python - App types: All, Type System: Dynamic, Cross platform: Yes, Community: huge, performance: ok, learning curve: short.
  - PHP - App types: Web apps, web API , Type System: Dynamic, Cross platform: Yes, Community: large, performance: ok, learning curve: medium.

- Data store:
  - 4 types of data store:
    - Relational Database.
      - Stores data in tables.
      - Tables have concrete set of columns.
      - Popular database:
        - SQLserver.
        - MySQL.
        - PostgreSQL.
    - NoSQL Database.
      - Emphasis on scale and performance.
      - Schema-less.
      - Data usually stored in JSON format.
      - Popular databases:
        - MongoDB.
        - Amazon DynamoDB.
        - Couchbase.
        - Azure Cosmos DB.
    - Cache.
      - Stores in-memory data for fast access.
      - Distributes data across nodes.
      - Uses proprietary protocol.
      - Stores serializable objects.
      - Popular databases:
        - Redis.
    - Object Store.
      - Stores un-structured, large data:
        - Documents.
        - Photos.
        - Images.
      - Popular stores:
        - Microsoft Azure.
        - Amazon S3.
        - MINIO.

### 38. Design the architecture

- Service's architecture is no different from regular software.
- Based on the layers paradigm.
- Layers:
  - Represent horizontal functionality.
    Expose User interface / API     ----> UI/SI (Expose API, JSON Handling, Auth)
        Execute Logic               ----> Business Logic (BL)(Validation, Enrichment, Computations)
      Save / Retrieve Data          ----> Data access layer (DAL)(Connection Handling, query/saving data, transaction handling).
  - Purpose of Layers:
    - Forces well formed and focused code.
    - Modular.
  - Concepts of Layers:
    - COde Flow: UI/SI ---> Business Logic (BL) ---> Data Access Layer (DAL).

## 7. Deploying Microservices

### 39. Introduction

- Deployment of microservices is extremely important.
- Remember: "Infrastructure Automation".
- Slow and complicated deployment will render the whole system ineffective and useless.
- Architect should be aware of deployment, not responsible.

### 40. CI/CD

- Stands for:
  - Continuous Integration / Continuous Delivery.
  - The full automation of the integration and delivery stages.

- Integration & Delivery
  - Typical schedule of system development:
    Build-----Unit tests-----Integration Tests------Staging---------Production-->
    |----------------Integration-------------||-----Delivery, Deployment-----|

- Why Use CI/CD?
  - Faster release cycle.
  - Reliability.
  - Reporting.

- CI/CD Pipelines:
  - The heart of the CI/CD process.
  - Define the set of actions to perform as part of the CI/CD.
  - Usually defined using YAML, with UI representation.

- As the Architect:
  - MAke sure there is a CI/CD engine in place.
  - Help shape the steps in the pipeline.

### 41. Containers

- Traditional deployment:
  - Code was copied and built on the production server.
  - Problems were found on the servers that were not found in the dev machines.
- Containers:
  - Thin package model.
  - Packages software, its dependencies, and configuration files.
  - Can be copied between machines.
  - Uses the underlying operating system.
- Container vs VM:
  - VM: infrastructure -> hypervisor -> virtual machine (Guest OS -> APP.)
  - Container: infrastructure -> Host OS -> container runtime -> App 1, App 2, etc.

- Why containers?
  - Predictability: The same package is deployed from the dev machine to the test to production.
  - Performance: Container goes up in seconds vs minutes in VM.
  - Density: One server can thousands of containers vs dozens of VMs.

- Why Not Containers?
  - Isolation: Containers share the same OS, so isolation is lighter than VM.

### 42. Introduction to docker

- Docker:
  - The most popular container environment.
  - De-facto standard for containers.
  - Release on 2013.
- dockerfile:
  - Contains instructions for building custom images.
- Support for docker:
  - all major systems (window, Linux, OSX)
  - Supported by major cloud providers: amazon ECR, Azure ACR.

### 43. Containers management

For example, we have six container: two front-end, two BE, one Batch Processes, one DB.

- Deployment.
- Scalability.
- MOnitoring.
- ROuting.
- High-availability.

### 44. Introduction to Kubernetes

- The most popular container management platform.
- De-facto standard for container management.
- Release by google 2014.
- Provides all aspects of management:
  - Deployment.
  - Scalability.
  - MOnitoring.
  - ROuting.
  - High-availability.
  - Automated Deployment.
  - COnfiguration Management.
  - And more...

- service is a mechanism used by Kubernetes to expose functionality to the outside world.

- Deployment - Summary
  - Automated deployment is must for effective microservices architect.
  - Docker and Kubernetes are the de-facto industry standard.
  - As an Architect, you are not responsible for the deployment but should be aware.

## 8. Testing microservices

### 45. Introduction

- Testing Microservices:
  - Testing is important in all systems and all architecture types.
  - With Microservice it's even more important.
  - Testing Microservices poses additional challenges.
- Tests Types:
  - Unit Test.
  - Integration test.
  - End-to-End Test.

### 46. Challenges with Microservices testing

- Microservices systems have a lot of moving parts.
- Testing and tracing all the services is not trivial.
  Begin test -> service A ---> service B -> service C -> End test.

- Main challenges:
  - Testing state across services.
  - Non-functional dependent services.

### 47. Unit tests

- Unit tests:
  - Tests individual code units.
    - Method, interface, etc.
  - In-process only.
  - Usually automated.
  - Developed by the developers.

- For example:
  - Orders Service:
    - AddOrder()
    - CancelOrder()
    - GetOrderDetails()
    - GetOrderForUser()
- Unit test in microservice:
  - No different.
  - Test only in-process code.
  - Use the same frameworks and methodologies.

### 48. Integration Tests

- Test the service's functionality.
- Cover (almost) all code paths in the service.
- Some paths might include accessing external objects:
  - Database, other services.
- What happens if the database (or other external service) is not ready?
  - Test double:
    - Pretends to be the real object / service to allow testing.
    - Three types:
      - Fake:
        - Implements a shortcut to the external service.
        - For example - stores data in-memory.
        - Many times implemented in-process.
        - Requires code change in the code.
      - Stub:
        - Holds hard-coded data.
        - Usually replaces data stored in a DB.
        - Allows simulating data services quickly.
        - No code change required.
      - Mock:
        - Verifies access was made.
        - Holds no data.
        - NO code change required.
- Integration tests:
  - Use the service's API.
  - Developed and conducted by the QA team.
  - Should be automated.
  - MOst unit testing frameworks support integration test.

### 49. End to End Tests

- TEst the whole flow(s) of the system.
- TOuch all services.
- TEst for end state.
- Extremely fragile.
- Require code.

### 50. Summary

- Extremely important.
- FOcus on the integration tests.
- As an Architect:
  - Make sure there is a test automation framework in place.
  - Be involved in the test results analysis, may requires architecture changes.

## 9. Service Mesh

### 51. Introduction

- Service Mesh:
  - Manages all service-to-service communication.
  - Provides additional services.
  - Platform agnostic (usually).

### 52. Problems solved by service mesh

- Microservices communicate between them a lot.
- The communication might cause a lot of problems and challenges:
  - Timeouts.
  - Security.
  - Retries.
  - Monitoring.

- Service Mesh:
  - Software Components that sit near the service and manage all service-to-service communication.
  - Provides all communication services.
  - The service interacts with the service mesh only.

- Service Mesh Services:
  - Protocol conversion.
  - COmmunication security.
  - Authentication.
  - Reliability (timeouts, retries, health checks, circuit, breaking).
  - Monitoring.
  - Service Discovery.
  - Testing(A/B testing, traffic splitting).
  - Load Balancing.

- Circuit Breaker:
  - Prevents cascading failures when a service fails:

- In short:
- Service's developers need not handle communication aspects when using Service Mesh.

### 53. Service Mesh Architecture

- Service A <-> (Data plane) Mesh <---  Control plane -----> Mesh (Data plane) <-> Service B

### 54. Type of service mesh

- Two main types:
  - In-Process
  - Sidecar

- In process:
  - (Service A | Mesh) <-----------> (Mesh | Service B)

- Sidecar:
  - Service A <-> Mesh <-----------> Mesh <-> Service B

- In-process -- Sidecar
  Performance -- platform agnostic
              -- code agnostic
              -- MORE POPULAR

### 55. Products and implementations

- There are quite a few Service Mesh Implementations.
- Some in-process, most sidecar.
- MOst free, some aren't.
- DO NOT develop your own.

- Sidecar:
  - Istio.
  - Linkerd.
  - maesh.

- In-Process:
  - DDS.

### 46. Should you use Service Mesh?

- Only if:
  - You have a lot of services.
  - Which communicate a lot with each other.
  - Or you have a complex communication requirement with various protocols or brittle network.

## 10. Logging and Monitoring

### 57. Introduction

- Extremely important Microservices.
- Flow goes through multiple processes.
- Hard to get wholistic view.
- Hard to know what is going on with the services.

### 58. Logging and Monitoring

- Logging ----------------------------------------- Monitoring
  Recording the system's active |Based on system's metrics.
  Audit                         |Alerting when needed.
  Documenting errors            |

### 59. Implementing Logging

- Logging should provide wholistic view on the system.
- Should allow tracing end-to-end flow.
- Should contain as much information as possible.
- Can be filtered using severity, module, time, etc.

- Logging:
  - Traditional:
    - Service A -> Log
    - Service B -> Log
    - What are problems?
      - Separate.
      - Different formats.
      - Not aggregated.
      - Can't be analyzed.
  - Microservices:
    - Service A -> Logging Service
    - Service B /
    - Solved:
      - Unified.
      - Aggregated.
      - Can be analyzed.
    - Logging library?
    - Transport?
    - Service?

- Logging Library
  - Better use one library for all the services.
    - Winston (NodeJS), Serilog (.NET core).
  - If using various platforms - one library for each platform.
  - Use severity wisely.
  - Logging as much info as possible, at least:
    - timestamp.
    - user.
    - severity.
    - service.
    - message.
    - stack trace (if error).
    - correlation ID.

- Correlation ID:
  - A flow identifier.
  - Correlates events between services.
  - Enable stitching separate events to single flow.
  - For example:
    - Same flow: Correlation ID:56, FLow start
                 Correlation ID: 56, FLow stop

- Transport
  - Preferably Queue.
  - Balances the load.
  - No performance hit on the client side.
  - Usually RabbitMQ / Kafka.

- Logging service:
  - Preferably based on indexing/ digesting/ search product.
  - Can index any log format.
  - Provides great visualization.
  - No development required.
  - Stack, Splunk

### 60. IMplement Monitoring

- Monitoring looks at metrics and detects anomalies.
- Provides simplified view of the system status.
- ALerts when there is a problem.
- Types of Monitoring:
  - Infrastructure
    - Monitors the server
    - CPU/RAM/DISK/Network
    - Alerts when infrastructure problems is detected.
    - Data source: agent on the machine.
  - Application:
    - Monitors the application.
    - Looks at metrics published by the app.
      - Requests/min, orders/day, etc.
    - Alerts when application problem is detected.
    - Data source: Application's logs, event log.
  - Most monitoring products provide both.

- Monitoring product:
  - Nagios, Stack, New Relic, Application Insight, etc.

## 11. WHen not to use microservices?

### 61. INtroduction

- Microservices are not one-size-fits-all.
- There are cases where they shouldn't be used.
- Might even cause damage.
- Need to evaluate on a case-by-case basis.

### 62. Small systems

- Small systems with low complexity should usually be a monolith.
- Microservices add complexity.
- If the service mapping result in 2-3 services - microservices are probably not the right option.

### 63. Intermingled functionality or data

- One of the most important microservice's attributes is its autonomy.
- When there is no way to separate logic or data - microservices will not help.
- If almost all requests for data span more than one service - there's a problem.

### 64. Performance Sensitive systems

- Microservices systems have performance overhead.
- Result of the network hops.
- If the system is VERY performance sensitive - think twice.
  - SLA is in low-milliseconds or even microseconds.

### 65. Quick and dirty systems

- Microservices design and implementation takes time.
- If you need a small, quick system, and need it NOW, don't go with microservices.
- Usually have short lifespan.

### 66. No planned Updates

- Some systems have almost no planned future updates.
- For example: embedded systems.
- Microservice's main strength is in the short update cycle.
- No updates == No microservices.

## 12. Microservices and the Organization

### 67. Introduction

- Microservices require different mindset.
- Traditional organizations will have hard time succeeding with Microservice.
- Without adapting - there is no point in going with Microservices.

### 68. Conway's Law

- Introduced in 1967 by Melvin Conway.
- Still relevant ...
- Describes the relationship between the organization and the software structure / architecture.
- "Any organization that design a system (defined broadly) will produce a design whose structure is a copy of the organization's communication structure."

- Traditional Project's Organization Chart.
  - Project Manager:
    - IT services.
    - Backend Developers.
    - Frontend Developers.
    - DBA.
  - Frontend --> Backend --> DB --> IT.

### 69. The Problem with Traditional Team

- When there are multiple teams - no one takes responsibility.
- Teams are horizontal - Backend, Frontend, IT etc.
- No wholistic view on the product.

### 70. The ideal team

- The ideal team is responsible for all aspects of the service:
  - Backend.
  - Frontend.
  - DB.
  - Deployment.

- Jeff Bezos: "Every internal team should be small enough that it can be fed with two pizzas."
- The ideal Team Size:
  - Pizza not mandatory :).
  - Exact number varies.
  - Should be small (Usually 3-7).

### 71. Changing Mindset

- Traditional organizations have hard time transition to Microservices.
- Need help in the process.
- You can and should help.
- How to help?
  - Training - Lecture on Microservices, success stories, basic principles.
  - POC - Go small, quick win.
  - Work closely with the team during design and development.

## 13. Anti-Patterns and Common Mistakes

### 72. Introduction

- Microservices require thorough design.
- They are not "Fire and Forget".
- It's easy to make mistakes that will cause the project to fail.

### 73. No Well-defined Services

- Mapping the Components
  - The single most important step in the whole process.
  - Determines how the system will look like in the long run.
  - Once set - not easy to change.

- Negligent mapping results in bloated services.
- Dependent functionality gets added continuously...
- ...and creates a mini-monolith.

### 74. No Well-Defined API

- API is the door to the service.
- Should be well thought of and easy to learn.
- MUST be consistent.
- MUST be versioned.
- MUST be platform agnostic.
- MUST be part of the design.

- For example, API design:

|Functionality|Path|Return codes|
|-----------------------------|-----------------------------------------|-----------------|
|Get next list to be processed| GET /api/v1/lists/next?location=...     | 200 OK          |
|                             |                                         | 400 Bad Request |
|Mask items as collected /    | PUT /api/v1/list/{listID}/item/{itemID} | 200 OK          |
|Unavailable                  |                                         | 404 Not found   |
|Export list's payment data   | POST /api/v1/list/{listID}/export       | 200 OK          |
|                             |                                         | 404 Not found   |

### 75. Implement cross-cutting last

- Every system has cross-cutting (system-wide) services.
  - Logging.
  - Caching.
  - User Management.
  - Authz & Authn.
  - And more.

- Should be implemented first.
- Other services are going to use them.
- No one likes to go back and modify existing code.

### 76. Expanding Service Boundaries

- Every service has well-defined boundaries.
- Expanding these boundaries makes the service inefficient and bloated.
- It's tempting - don't to that!
- Many times new service should be used instead of expanding existing service's boundaries.

## 14. Breaking Monolith to Microservices

### 77. Introduction

- Quite common scenario.
- The organization wants to improve the current system.
- Needs to be thoroughly planned.
- High rate of failures.

### 78. Motivation for Breaking Monolith

- Shorten update cycle.
- Modularize the system.
- Save costs.
- Modernize the system.
- Being attractive.

### 79. Strategies for Breaking Monolith

- Breaking Monolith is a delicate process.
- Must be planned ahead.
- There are three main strategies for that.
- Strategies:
  - New Modules as Services.
  - Separate Existing Modules to Services.
  - Complete Rewrite.

### 80. New module as services

- Pros:
  - Easy to implement.
  - Minimum code changes.
- Cons:
  - Takes time.
  - End result is not pure Microservices architecture.

### 81. Separate Existing Modules to Services

- Pros
  - End result is pure Microservices architecture.
- Cons
  - Takes time.
  - A lot of code changes.
  - Regression testing required.

### 82. Complete rewrite

- Pros:
  - End result is pure microservices architecture.
  - Opportunity for modernization.
- Cons:
  - Takes time.
  - Rigorous testing required.

## 15. Case study

### 83. Introduction

- Use what we learned in a real-world system.
- Go through main steps of the architecture process:
  - Functional Requirements.
  - Non-Functional Requirements.
  - Mapping the Components.
  - Defining Communication.

- We won't discuss technology stack.
  - Not specifically relevant to Microservices.
- By the end this section.
