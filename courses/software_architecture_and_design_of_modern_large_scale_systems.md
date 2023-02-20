# Software architecture & design of modern large scale systems

## 1. Introduction

### 1. Introduction to Software Architecture

- What we learn in this lecture?
  - SA - Motivation.
  - SA -Definition.
  - SA Definition - Explanation.
  - Software Development Cycle.

- Analogies from Outside of the Software World.
  - Everything we build has a structure.
  - The more we invest in building a product, the harder it is to change its structure.
  - The structure describes:
    - The intent of our product.
    - The qualities of the product.

- Applications to Software:
  - Infinite ways to organize our code.
  - Different organizations will give us different properties.
  - The software architecture impacts:
    - Performance and scale of the product.
    - Ease of adding new features.
    - Response to failure or security attack.
  - Cost of redesign will be significant in terms of time and money.

- SA - Definition:
  - many ways to define SA.
  - the definition we are going to use is as follows:
    - "The SA of a system is a high-level description of the system's structure, its different components, and how those components communicate with each other to fulfill the systems' requirements and constraints."

- SA Definition - Explanation:
  - "The SA of a system is a high-level description of the system's structure":
    - An abstraction that shows us the important components while hiding the implementation details.
    - Technologies or programming languages are NOT a part of the SA but a part of the implementation.
    - Decisions about implementation should be delayed to the very end of the design.

  - "its different components, and how those components communicate with each other":
    - The components here are "black box" elements defined by their behavior and APIs.
    - The components may themselves be complex systems with their own SA diagrams.

  - "to fulfill the systems' requirements and constraints.":
    - Components come together to do what the `system must do`, which are our requirements.

- Levels of Abstraction:
  - SA can have many different levels of abstraction:
    - Classes/structures
    - Modules/Packages/Libraries
    - Services (Processes/ Group of processes).

- Advantages:
  - This more distributed, multiple service approach allows us to architect systems that can:
    - Handle large amounts of requests.
    - Process and store very large large amounts of data.
    - Serve many users every day.

- Large Scale Systems -examples:
  - Ride-Sharing
  - Video-on-demand
  - Social media
  - Online video games
  - Investment Services
  - Banks
  - Etc.

- Importance of Design and Architecture - Benefits:
  - If we get the architecture right we can:
    - Go from a small startup to a multi-billion dollar company.
    - Make a positive impact on millions of people.

- Importance of Design and Architecture - Risks:
  - Not doing a good job at the design phase can:
    - Waste months of engineering time.
    - Build a system that doesn't meet our requirements.
  - Restructuring a system that was not architected correctly is very hard and expensive.
  - So the stakes here are high.

- Software Development Cycle:
  Design ----> Implementation -> Testing -> Deployment.
          /\
          ||
          SA

- Challenges of SA:
  - We cannot prove SA to be either:
    - Correct.
    - Optimal.
  - What we can do to guarantee success is follow:
    - Methodical Design Process.
    - Architecture Patterns.

- Summary:
  - Go the intuition and motivation for SA.
  - Every software system has an architecture, which is critical for its success.
  - SA is thr `output` of the `design` phase and the `input` of the `implementation`.

## 2. System Requirements & Architectural Drivers

### 3. Introduction to System Requirements & Architectural Drivers

- What are system requirements:
  - Requirements - Format description of what we need to build.
  - Large scale system requirements are different than the usual requirements we typically get for implementing:
    - A method.
    - An algorithm.
    - a Class.
    - etc.

- Big scope and high level of abstraction:
  - Large system
    - Application
      - Library
        - Module
          - Class
            - Method/Function

- Example of scope and abstraction:
  - File Storage system.
  - Video streaming solution.
  - Ride Sharing service.

- High level of ambiguity:
  - SYstem Design has high level of ambiguity.
  - Two reasons:
    - The person providing the requirements is often not an engineer and may even be not very technical.
    - getting the requirements is part of the solution.
      - The client doesn't always know what they need.
      - The client generally knows only what problem they need solved.
      - For example - hitchhiking service:
        - High level requirement - "Allow people to join drivers on a route, who are willing to tak passengers for a fee."
        - Clarifying Questions:
          - Real time vs Advance reservation.
          - User Experience  - Mobile? Desktop? both?
          - Payment though us or direct payment?

- Importance of Gathering Requirements:
  - What happens if we don't get the requirements right?
  - We can simply build something and then fix it.
  - Seemingly there's no cost of materials on software so changes should be cheap??
    - Large scale systems are big projects that cannot be easily changed.
  - Many engineers are involved.
  - Many months of engineering work.
  - Hardware and Software costs.
  - Contracts include financial obligations.
  - Reputation and brand.

- Types of Requirements:
  - Features of the System:
    - Functional requirements.
  - Quality Attributes:
    - Non-functional requirements.
  - System Constraints:
    - Limitations and boundaries.

- Features / Functional Requirements
  - Describe the system behavior - what `the system must do`
  - Easily tied to the objective of our system.
    User action--->|        |
                   | System |---->Result/Outcome--->
    Events-------->|        |

  - Functional requirements do not determine its architecture.
  - Generally, any architecture can achieve any feature.
  - Examples:
    - "When `a rider logs into the service mobile app`, the `system must` `display a map with nearby drivers within 5 miles radius.`"
    - "`When a rider is completed`, `the system will` `charge the rider's credit card and credit the driver, minus service fees.`"

- Quality Attributes / Non-Functional Requirements
  - System properties that `the system much have`.
