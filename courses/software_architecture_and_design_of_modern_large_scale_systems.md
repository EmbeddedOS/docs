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
  - Examples:
    - Scalability
    - Availability
    - Reliability
    - Security
    - Performance

- Quality attributes and SA
  - The `Quality attributes` dictate the SA of our system.

- System constraints - examples:
  - Time constraints - strict deadlines
  - Financial constraints - Limited budget
  - Staffing Constraints - Small number of available engineers.

- Summary:
  - Importance of system requirements
  - Challenges:
    - High Scope and Abstraction.
    - Ambiguity.
  - Risks of not getting the requirements correctly up front.
  - Requirements classification/Architectural drivers:
    - Features - Functional requirements.
    - Quality Attributes - Non functional requirements.
    - Constraints - Limitations and boundaries.

### 4. Feature Requirements - Step by step Process

- Requirements Gathering - Native way
  - Ask the client to describe everything they need.
  - For complex systems - Not a good app approach.

- Methods of Gathering Requirements
  - More powerful method of gathering requirements:
    - Use cases:
      - Situation / Scenario in which our system is used.
    - User flows:
      - A step by step / Graphical representation of each use case.
- Requirement Gathering Steps
  1. identify all the actors/users in our system.
  2. Capture and describe all the possible use-cases/scenarios.
  3. User flow - expand each use case though flow of events.
     - Each event contains:
       - Action
       - Data

- For example:
  - Hitchhiking Service - actors:
    - "Allow people to join drivers on a route, who are willing to take passengers for a fee."
  - Hitchhiking Service - Rider Use Cases:
    - Rider first time registration.
    - Driver registration.
    - Rider login.
    - Driver login.
    - Successful match and ride.
    - Unsuccessful ride.

- Example with a Sequence Diagram:
  - Unified Modeling Language - Sequence Diagram.
    - Sequence Diagram - Diagram that represents interactions between actors and objects.
    - Part of the UML - Standard for visualizing system design.
    - IN Practice:
      - UML diagram are used mostly for software design.
      - No real standard diagrams representing SA in the industry.
      - UML is not strictly followed in the industry.
      - Sequence Diagrams are frequently used to represent interactions between entities.

### 5. System Quality Attributes Requirements

- Quality Attributes:
  - Systems are frequently redesigned NOT because of functional requirements
  - But because the system as it stands:
    - Isn't `fast` enough
    - Doesn't `scale`
    - Slow to `develop`
    - Hard to `maintain`
    - Not `secure` enough
  - Quality attributes are non functional requirements
  - They describe:
    - The `qualities` of the `functional` requirements.
    - The overall `properties` of the `system`.
  - Provide a `quality measure` on how well our system performs on a `particular dimension`.
  - They have direct correlation with the architecture of our system.
  - Example:
    - "`When a user clicks on a search button after they typed in a particular search keyword, the user will be provided with a list of products that closely match the search keyword` `within at most a 100 milliseconds.`"
      - with `When a user clicks on a search button after they typed in a particular search keyword, the user will be provided with a list of products that closely match the search keyword` is functional requirements.
      - `within at most a 100 milliseconds.` is **Performance Quality Attribute**.

    - `The online store must be available to user a requests at least 99.9% of time.` is Availability attribute.
    - `Development team can deploy a new version of the online store at least twice a week` is a Deployability quality attribute.

- Quality Attributes Considerations:
  - Important 1: `Testability` and `Measurability`
    - Quality attributes need to be:
      - Measurable.
      - Testable.
    - If we can prove that our system satisfied the required the quality attribute we don't know if our system performs `well` or `poorly`.
    - Unmeasurable Quality Attribute - Example:
      - `When I user clicks on the buy button, the purchase confirmation must be displayed quickly to the user.` -- quickly ???

  - Important 2: Tradeoffs
    - No single software architecture can provide all the quality attributes.
    - Certain quality attributes contradict one another.
    - Some combinations of quality attributes are very hard / impossible to achieve.
    - We (Software Architects) need to make the right `tradeoff`.

    - Trade Off - Login Page Example, we have two conflicting requirements:
      1. Performance: Login time < 1s
      2. Security: Username, Password, SSL. -> Slower.

  - Important 3: Feasibility
    - We need to make sure that the system is capable of delivering with the client asking for.
    - The client may ask for something that is either:
      - Technically impossible.
      - Prohibitively expensive to implement.
      - for example:
        - Latency by distance. VN -> US: 200ms, we can not make requests < 100ms.
        - Our system can never fail:
          - We never have a chance to take our system down for:
            - upgrade.
            - maintain.
            - etc.
        - Full protection against hackers.
        - High resolution video streaming in limited bandwidth areas.
        - very high storage growth.

### 6. System constraints in Software architecture

- Once we define what our system must do, we have freedom on how to structure our system.
- While defining the final architecture, we have to make a lot of decisions.
- For `quality attributes`, we are expected to make trade-offs.

- System Constraints - Definition:
  - "A system constraint is essentially a decision that was already either fully or partially made for us, restricting our degrees of freedom."
  - But this is not always a bad thing.
  - Instead of looking at a constraints as a choice that was taken away, we look at it as a decision that was already made.
  - System Constraints are referred as pillars for software architecture because:
    - They provide us with a SOLID starting point.
    - The rest of the system need to be designed around them.

- Types of system constraints:
  - Technical constraints.
  - Business constraints.
  - Regulatory/legal constraints.

- Technical constraints:
  - Example of Technical constraints:
    - Being locked to a particular `HW/Cloud vendor`.
    - Having to use a particular `programming language`.
    - Having to use a particular `database or technology`.
    - Having to use a particular `platform, browsers, OS`.

  - Technical constraints may seem like they belong to implementation and not to SA.
  - In practice, they affect the decisions we make in the design phase and put restrictions on our architecture.
  - Examples:
    - If our company makes a decision to run on-premise data centers then:
      - All the cloud architectures and paradigms will become unavailable to us.
      - We would have to implement a lot of non-trivial infrastructure.
    - If we have to support some older browsers or low-end mobile devices then:
      - We have to adapt our architecture to support those platforms and their APIs.
      - keep providing a different, more high-end experience for newer browsers or high-er devices.

- Business constraints
  - As engineers, we make the right decisions and architectural choices from a technical perspective.
  - This forces us to make sacrifices in:
    - Architecture.
    - Implementation.
  - Limited budget or a strict deadline will make us have very different choices than if we had an unlimited budget and unlimited time.
  - Different software architectural patterns are based on suitability between small startups or bigger organizations.
  - Examples:
    - Usage of third-party services with their own APIs and architectural paradigms as part of our architecture.
      - Using third-party shipping/billing providers for an online store.
      - Integration of different banks/brokers/security/fraud detection services for an investing platform.

- Regulatory/legal constraints:
  - Regulatory constraints may be:
  - Global
  - Specific to a region.
  - Examples:
    - In the US, `HIPAA` (Health insurance portability and accountability Act) places constraints on accessing patients' data.
    - In the European Union, GDPR (General Data Protection Regulation) sets limitations on collecting, storing and sharing users' data.

- System Constraints Considerations
  - We shouldn't take any given constraint lightly.
    - Accepting constraints lightly.
      - There should be a distinction between:
      - Real constraints.
      - Self-imposed constraints.
    - Examples:
      - External rules regulations may not have room to negotiate.
      - Internal constraints can be negotiated.
      - If locked to using a particular:
        - Hardware
        - Cloud vendor
        - Technologies
      - May be an opportunity to explore other options.
  - Use Loose Coupled Architecture
    - Examples:
      - If limited to a database/third-party service, we need to make sure our system is not tightly coupled to that technology or APIs.
      - Usage of different technology/service in future should need minimal changes.
      - Different parts of the system can be decoupled to be easily replaced or updated independently.

## 3. Most Important Quality Attributes in Large Scale Systems

### 7. Performance

- List of Quality Attributes:
  - Performance.
  - Scalability.
  - Availability.
  - Testability.
  - Deployability.
  - Maintainability.
  - Portability.
  - Security.
  - Observability.
  - Consistency.
  - Efficiency.
  - Usability.
  - Etc.

- Performance:
  - Performance - Response Time.
  - Performance - Throughput.

- Performance Definitions - Response Time:
  - Response Time: `Time between a client sending a request and receiving a response.`
  - Processing Time: `TIme spent in our system actively process in the request and building/sending the response.`
  - Waiting Time: `Duration of time request/response spends inactively in our system.`
  - Response Time = Processing Time + Waiting Time
  - Response Time Importance:
    - Response time is an important metric when the request is in the critical path of a user interaction.
- Performance Definitions - Throughput:
  - Amount of work performed by our system time.
    - Measured in tasks/second.
  - Amount of data processed by our system per unit of time.
    - Measured in bits/second, Bytes/second, Megabytes/second.

- Importance:
  - Measuring Response Time Correctly:
    - Response Time = Processing Time + Waiting Time
      - NOT ignore Waiting Time
  - Response Time Distribution:
    - What is the metric that we should:
      - Set our goal around.
      - Measure.
    - Average?
    - Median?
    - Maximum?

- Percentile - Definition
  - The `xth percentile` is the `value` below which `x%` of the values can be found.

- Tail Latency - Definition
  - The small percentage of response times from a system, that take the longest in comparison to the rest of values.
  - Consideration Response Time goals:
    - Definition response time goals using percentiles.
    - Measure and compare to our goals using percentile distribution.
    - Examples:
      - Example 1: 30ms at 95th percentile of response time.
        - Set our goal to be 30ms at 95th percentile, mean we only allow our system to exceed that response time only for 5% of our requests.

- Performance Degradation - High Resource Utilization
  - Potential overly-utilized resources:
    - High CPU utilization.
    - High memory consumption.
    - Too many connections/IO
    - Message queue is at capacity.
    - etc.

### 8. Scalability

- Traffic Patterns:
  - The load/traffic on our system never stays the same.
  - It can follow different patterns.

- Scalability Formal Definition
  - `The measure of a system ability to handle a growing amount of work, in an easy and cost effective way, by adding resources to the system.`

- Types of Scalability:
  - Scale up/Vertical scalability.
  - Scale out/Horizontal scalability.
  - Team/Organization Scalability.

- Scale up/Vertical scalability:
  - `Adding resources or upgrading the existing resources on a single computer, to allow our system to handle higher traffic or load.`
  - Pros:
    - Any application can benefit from it.
    - No code changes are required.
    - The migration between different machines is very easy.
  - Cons:
    - The scope of upgrade is limited.
    - We are locked to centralized system which CANNOT provide:
      - High Availability.
      - Fault Tolerance.

- Scale out/Horizontal scalability:
  - `Adding more resources in a form of new instances running on different machines, to allow our system to handle higher traffic or load.`
  - Pros:
    - No limit on scalability.
    - It is easy to add/remove machines.
    - If designed correctly we get:
      - High Availability.
      - Fault Tolerance.
  - Cons:
    - Initial code changes may be required.
    - Increased complexity, coordination overhead.

- Team/Organizational Scalability.
  - `The measure of a systems ability to handle a growing amount of work, in an easy and cost effective way, by adding resources to the system.`

- Reasons for Productivity Degradation:
  - Many crowded meetings.
  - Code merge conflict.
  - Business COmplexity -Longer ramp up time.
  - Testing is harder and slower.
  - Releases become very risks.
  - etc.

- Team scalability:
  - Software Architecture impacts engineering velocity (team productivity).

### 9. Availability

- Availability is an important attribute when designing a system.
- It has the greatest impact on:
  - Our users
  - Our business

- Importance of Availability - Users
  - Users try to purchase from online store but:
    - The page doesn't load.
    - They get an error at checkout.
  - Users lose access to their email for considerable time.

- Importance of Availability - Business (B2B)
  - If we provide services to other companies, the impact of outage is compounded.
  - AWS simple storage service had an outage in February 2017.
  - SInce many companies and websites used the service, it almost took down the entire internet.

- Importance of Availability - Mission Critical Services
  - System downtime is not always just an inconvenience.
  - Our software can be used for mission-critical service like:
    - Air traffic control in airports.
    - Healthcare in hospitals.
  - If it crashes/becomes unavailable for prolonged periods, people's lives are on line.

- Impact of downtown on the business.
  - For a business, the consequences of our system going down are:
    - IF users cannot use system, our profits becomes zero.
    - if users lose access constantly, they go to our competitors.
  - Thus, downtime of our system for a business results in a loss of:
    - Money.
    - Customer.

- Availability Definition:
  - `The faction of time/probability that our service is operationally functional and accessible to the user.`

- Formula of Availability
  - Uptime - Time that our system is operationally functional and accessible to the user.
  - Downtime - Time that our system is unavailability to the user.
  - Availability (in %) = Uptime / (Entire time our system is running)
  - `Availability = Uptime/ (Uptime + Downtime)`

- Alternative Way to Calculate/Estimate Availability
  - MTBF - Mean Time Between Failures
    - Represents the average time our system is operational.
    - Useful when:
      - Dealing with multiple pieces of hardware.
      - Each component has its own operational shelf life.
      - We are not using cloud/third-party API with a given uptime and availability.
  - MTTR - Mean Time To Recovery
    - Time average it takes us to detect and recover from a failure.
    - Average downtime of our system.
  - `Availability = MTBF / (MTBF + MTTR)`

  - In practice, MTTR cannot be zero.
  - Shows that detectability and fast recovery can help us achieve high availability.

- How much is HA?
  - Users would want 100% availability.
  - On the engineering side it is:
    - Extremely hard to achieve.
    - Leaves no time for maintenance/upgrades.

- Industry Standard of Availability
  - No strict definition of what constitutes HA.
  - INdustry standard set by cloud vendors is typically 99.9% or higher.

- The `nines`:
  - Percentages are referred to by the number of `nines` in their digits.
  - For example:
    - `99.9%` - 3 nines.
    - `99.99%` - 4 nines.

### 10. Fault Tolerance & HA

- Three sources of Failure:
  1. Human Error:
     - Pushing a faulty config to production.
     - Running the  wrong command or script.
     - Deploying an incompletely tested new version of software.
  2. Software Error:
     - Long garbage collections.
     - Out-of-memory exceptions.
     - NULL pointer exceptions.
     - Segmentation faults.
  3. Hardware failures:
     - Servers/routers/storage devices breaking down due to limited shelf-lef.
     - Power outages due to natural disasters.
     - Network failures because of:
       - Infrastructure issues.
       - General congestion.

- Fault Tolerance:
  - Failures will happen despite:
    - Improvements to our code.
    - Review, testing, and release processes.
    - Performing ongoing maintenance to our hardware.
  - `Fault Tolerance` is the best way to achieve `HA` in our system.
  - `Fault Tolerance enables our system to remain operational and available to the users despite failures within one or multiple of its components.`
  - When failures happens a *fault-tolerant* system will:
    - continue operation at the same/reduced level of performance.
    - Prevent the system from becoming unavailable.
  - Tactics for achieving Fault Tolerance:
    - Fault Tolerance revolves around 3 major tactics:
      - Failure Prevention:
        - To prevent our entire system from going down, eliminate any `single point of failure` in our system.
        - Examples of `single point of failure` can be:
          - `One server` where we are running our application.
          - Storing all our data on the one instance of our database that runs on a `single computer`.
        - Best way to eliminate a `single point of failure` is through `Replication and Redundancy`.

- Types of Redundancy:
  - Spatial Redundancy - RUnning replicas of our application on different computers.
  - Time Redundancy - Repeating the same operation/request multiple times until we succeed/give up.

- Strategies for Redundancy and Replication:
  - Two strategies which are extensively used in the industry in different systems:
    - Active-Active architecture.
    - Active-Passive architecture.

- Advantages of Active-Active architecture:
  - Load is spread among all the replicas.
  - Identical to horizontal scalability.
  - Allows more traffic.
- Disadvantages of Active-Active architecture:
  - All the replicas are taking requests.
  - Additional coordination required to keep active replicas in sync.
- Disadvantages of Active-Passive architecture:
  - Ability to scale out system is lost.
  - All the requests still go to only one machine.
- Advantages of Active-Passive architecture:
  - Implementation is easier.
  - There is a clear leader with up-to-date data.
  - Rest of the replicas are followers.

- False Positive:
  - But the issue might be `the network` or `long garbage collection`:
  - Then the monitoring service is going to have a `false positive`.
  - It assumed that a healthy host is faulty.
- False Negative:
  - The monitoring service shouldn't have `false negative`.
  - `false negative` mean that:
    - The servers may have crashed.
    - The monitoring system did not detect that.

- Monitoring system FUnctions:
  - Exchange of messages in the form of pings adn heartbeats.
  - Collect data about the number of errors each host gets per minute.
    - If the error rate in one of the hosts is high, it can interpret that as `failure of the host`.
  - Collect information about time taken for each host to respond.
    - If the time to respond to requests becomes long, it can decide that `host is slow`.

- Recovery from failure:
  - `Availability = MTBF / (MTBF + MTTR)`
    - Fast recovery -> MTTR ~ 0 -> HA ~ 100%
  - Action after detecting faulty instance/server:
    - Stop sending traffic workload to that host.
    - Restart the host to make the problem go away.
    - Rollback - going back to a version that was stable and correct.

- Rollbacks:
  - Common in Databases.
  - If we get to a state violating some condition/data, we can roll back to the last correct state in the past.
  - If we detect errors while rolling out new versions of software, we can roll back to the previous version.

### 11. SLA, SLI, SLO

- SLA - Service Level Agreement:
  - SLA is a agreement between Service Provider and our Clients/USers.
  - It is a legal contract that represents our quality service such as:
    - Availability.
    - Performance.
    - Data durability.
    - Time to respond to system failures.
  - It states the penalties and financial consequences, if we breach the contract.
  - The penalties include:
    - Full/Partial refunds.
    - Subscription/License extension.
    - Service credits.

- SLA and Users
  - SLA exists for:
    - External paying users (always)
    - Free external users (sometimes)
    - Internal users within our company (occasionally)
  - Internal SLAs don't include any penalties.
  - SLA for free external users makes sense if our system has major issues during a free trial of our service.
  - We compensate those users with a `trial extension` or `credits of future`.
  - Companies providing entirely free services don't publish SLA.

- SLO - Service Level Objectives:
  - Individual goals set for our system.
  - Each SLO represents a target value/range that our service needs to meet.

  - For example, we can have:
    - Availability Service Level Objective of 3 nines.
    - Response Time SLO of `less than 100 milliseconds at the 90th percentile`.
    - Issue Resolution Time Objective of between 24 and 48 hours.
  - SLOs include:
    - Quality attributes from the beginning of the design process.
    - Other objectives of our system.

- SLO and SLA:
  - Systems that don't have SLA still must have SLOs.
  - If we don't set SLOs, our users won't know what to expect from our system.

- SLIs - Service Level Indicators:
  - Quantitative measure of our compliance with a service-level objective.
  - It is the actual numbers:
    - Measured using a monitoring service.
    - Calculated from our logs.
  - It can be later compared to our SLOs.
  - Examples:
    - Percentage of user requests receiving a successful response can be used as a SLI for availability.
    - Later it can be compared to the availability SLO of three nines that we set.
    - The average/percentile distribution of the response time experienced by users can be calculated.

- SLI, SLO and SLA
  - SLO represent the `target values` for the important quality attributes.
  - Quality attributes need to be testable and measurable.
  - If they weren't measurable, we wouldn't find any SLIs to validate that we meet our SLOs.
  - If we can't prove that we meet the SLOs, we can't say that we meet our SLA.
  - SLAs are crafted by `the business and the legal team`.
  - SLOs and SLIs and defined and set by `the software engineers and architects`.

- Important Considerations:
  - We shouldn't take every SLI that we can measure in our system and define an objective associated with it.
    - **Metrics that users care about**:
      - Think about the metrics that users care about the most.
      - Define the SLOs around those metrics.
      - From that, right SLIs to track those SLOs can be found.
  - Promising fewer SLOs is better:
    - Number of SLOs:
      - With many SLOs it is hard to prioritize all of them equally.
      - With few SLOs, it is easier to focus the entire software architecture around them.
  - Set realistic goals with a budger for error:
    - We should not commit to five-nines of availability even if we can provide that.
    - We should commit to a much lower availability than we can provide.
    - This `saves costs` and `incorporates unexpected issues`.
    - This is true when our SLOs are represented in an external SLA.
    - Companies define separate external SLOs which are looser than internal SLOs.
    - We can strive for better quality internally while committing less to clients.
  - Create a recovery plan for when the SLIs show that we are not meeting our SLOs.
    - We need to decide ahead of time what to do if:
      - The system goes down for long time.
      - The performance degrades.
      - Reports about issues/bugs in the system.
      - The plan should include:
        - Automatic alerts to engineers/DevOps
        - Automatic failovers/restarts/rollbacks/auto-scaling policies.
        - Predefined handbooks on what to do in certain situations.

- Cloud Vendor SLA Examples:
  - [AWS](https://aws.amazon.com/legal/service-level-agreements/?aws-sla-cards.sort-by=item.additionalFields.serviceNameLower&aws-sla-cards.sort-order=asc&awsf.tech-category-filter=*all)
  - [Google Cloud](https://cloud.google.com/terms/sla)

## 4. API design

### 13. API design in Software Architect

- What is an API?
  - After capturing all functional requirements, we can think of our system as a `black box`.
  - The `black box` has:
    - Behavior.
    - Well-Defined.
  - That `interface` is a contract between:
    - Engineers who implement the system.
    - Client applications who use the system.
  - Since this `interface` is called by other applications. it is referred to as Application Programming Interface or API.
  - In a large-scale system, API is called by other applications remotely through the network.

  - The applications calling our API may be:
    - `Frontend clients` like mobile applications/web browsers.
    - `Backend systems` that belong to other companies.
    - `Internal system` within our organization.
  - Each component of our system will have its own API.

- Categories of API:
  - APis are classified into three groups:
    - Public APIs.
    - Private/Internal APIs.
    - Partner APIs.

- Public APIs:
  - Exposed to the general public.
  - Any developer can use/call them from their application.
  - Good practice:
    - Requiring the users to `register` with us before allowing to send requests and use the system.
      - This allows:
        - Control over who uses the system externally.
        - Control how they use the system.
        - Better security.
        - To blacklist users breaking rules.

- Private APIs:
  - Exposed only internal within the company.
  - They allow other teams/parts of the organization to:
    - Provide bigger value for the company.
    - Take advantage of the system.
    - Not expose the system directly outside the organization.

- Partner APIs:
  - Similar to Public APIs.
  - Exposed only to companies/users having business relationship with us.
  - The business relationship is the form of:
    - Customer Agreement after buying our product.
    - Subscribing to our service.

- Benefits of a well designed API:
  - Client who uses it can immediately and easily enhance their business by using our system.
  - They do not need to know anything about our system's internal design/implementation.
  - Once we define and expose our API, clients can integrate with us without waiting for full implementation of our system.
  - API makes it easier to design and architect the internal structure of our system.

- API best practice and pattern:
  - Complete Encapsulation:
    - `Complete Encapsulation` of the internal design and implementation.
    - Abstracting it away from a developer wanting to use our system.
    - If client wanting to use our API:
      - Requires any information about how it is implemented internally.
      - needs to know our business logic to use it.
    - API should be `completely decoupled` from our internal design and implementation.
    - We can `change the design` later without breaking the contract with our clients.

  - Easy to Use:
    - Easy to use.
    - Easy to understanding.
    - Impossible to misuse.
    - The ways to make an API simple can be:
      - Only way to `get certain data` or `perform a task`.
      - `Descriptive names` for actions and resources.
      - Exposing `only the information` and actions that users need.
      - keeping things consistent all across our API.

  - Keeping the Operations Idempotent:
    - `An operation doesn't have any additional effect on the result if it performed more than once.`
    - For example:
      - Updating the user's address to a new address is an Idempotent Operation.
      - The result is the same regardless of performing it any number of times.
    - Non-Idempotent Operations - Example:
      - Incrementing a user's balance by a hundred dollars is not an idempotent Operation.
      - The result will be different depending on the number of times we perform it.
      - `Operations Idempotent` are preferred for our API as they are going to be through the network.
      - If the client application sends us a message:
        - The message can be lost.
        - The response to that message may be lost.
        - The message wasn't received as a critical component in our system went down.
        - Because of `networking decoupling`, the client application has no idea which scenario actually happened.
        - If our operation is idempotent, they can simply `resend the same message` again without any consequences.

  - API Pagination:
    - Important when a response from our system to the client request contains a `very large payload or dataset`.
    - Without `API Pagination` most client applications will:
      - Not able to handle big responses.
      - Result in a poor user experience.
    - Imagine what would happen if:
      - After opening your email account, you see all the emails you ever received instead of the latest emails.
      - After searching for an item on online store/search engine, you see too many items that matched your query.
    - The client application/web browser is unlikely to handle so much data.
    - It would take an unreasonable time to show all those results.
    - API Pagination allows the client:
      - To request only a small segment of the response.
      - Specify the maximum size of each response from our system.
      - Specify an offset within the overall dataset.
    - To receive the next segment we increment the offset.

  - Asynchronous Operations:
    - Some operations need one big result at the end.
    - Nothing meaningful can be provided before the entire operation finishes.
    - Examples:
      - Running a big report requiring our system to talk to many databases.
      - Big data analysis that scans a lot of records/log files.
      - Compression of large video files.
    - If the operation takes a long time, the client application has to wait for the result.
    - The pattern used for these situations is an Asynchronous API.
    - A client application receives a response immediately without having to wait for the final result.
    - That response includes some kind of identifier that allows:
      - To track the progress and status of the operation.
      - Receive the final result.

  - Versioning our API
    - Best API design allows us to make `changes to the internal design and implementation` without changing the API.
    - In practice, we may need to make `non-backward compatible API changes`.
    - If we explicitly version the APIs we can:
      - Maintain two versions of the API at the same time.
      - Deprecate the older one gradually.

- Defining Our API:
  - We can define our API in any way as long as we adhere to these best practices.
  - A few types of APIs have became more standard in the industry.
