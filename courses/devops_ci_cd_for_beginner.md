# DevOps, CI/CD (Continuous Integration/Continuous Delivery) for beginner

## 2. The Old school Software development cycle

Software development team -> Git -> Build & Integration team -> compile -> package.

Package + Instructions -> Operation team -> deploy to test environment -> tester testing the package. -> public.

If tester find some issues, they give feedback to software development team.

Painful:

- Long feedback cycle for functional defects.
- Long iterations.
- Fixing issues at the end of iterations.
- Integration is painful and effort consuming.

## 3. Continuous integration

CI is a development practice that requires developers to integrate code into a shared repository several times a day.

```text
                 _______________________Build Server____________________
                |                                                       |
Users --commit--|->git-> compile -> package -> automate -> automate ----|->package.
                |                             unit tests   UI tests     |
                |_______________________________________________________|
```

Cardinal Principles of CI:

- A single central repository where the code lives.
- Developers check-in/commit their code frequently.
- Build should be triggered every time a developer checks in code.
- Build should be automated.
- Build should compile the code as well as run automated.
- Fixing a failed build should be top priority for the developers.
- Building results should always be communicated to all developers.

## 4. Pipeline explained

Pipeline is all steps that one change will need.

```text
 _______________________Build Server____________________
|                                                       |
|                    BUILD PIPELINE                     |
|->git-> compile -> package -> automate -> automate ----|
|                             unit tests   UI tests     |
|_______________________________________________________|
```

Each commit now run on its own pipeline with the same step, but no block each other.

## 5. Old school Continuous Delivery painful

- Pain point #1: correctness of instructions.
- Pain point #2: Difference in instructions across the env.
- Pain point #3: Error prone nature of manual tasks.
- Pain point #4: Deployments are sophisticated. high-impact with downtime.

## 6. Bring in the CD

CD is a sw development practice where sw can be released to production at anytime.

```text
                            Release pipeline
package -|-> automated scripts -> test env -> testing -> production env|
```
