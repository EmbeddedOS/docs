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

### 37. Selecting Technology Stack

### 38. Design the architecture
