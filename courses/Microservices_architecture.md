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