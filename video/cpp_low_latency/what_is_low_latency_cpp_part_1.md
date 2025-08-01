# What is Low latency C++ part 1

Link [What is low latency c++](https://www.youtube.com/watch?v=EzmNeAhWqVs&list=PLJFFAyzgew0_EcU9Fcytq3jGjjsqY37c6&index=1)

## 1. Low latency programming domains

- video games.
- audio processing.
- high-frequency trading (HFT).
- Embedded: Automotive, Aeronautics, Robotics, Medicals, etc.

## 2. This talk is not about

- Embedded systems.
- Mainframes.
- Exotic architectures.
- GPUs.
- Heterogeneous computing.
- Safety & Security.
- Language design.
- C++ successor languages.

## Performance?

- Performance is actually like 2 orthogonal components:

```text
Throughput
    ^
    |
    |
    |
    |
    |______________________> Latency
```

If you visualize your programs as a pipe, and there's like information flowing through, then the `throughput` is how much data you can process per unit of time. And `latency` is how much time gonna pass between time when you ask the question and the program giving you an answer.

You might want to optimize for one, throughout, latency or both.

## Efficiency

To program efficiently is **to reduce the amount of work** that the program has to perform to complete a certain task.

## 3. Why use C++ for low latency programming?

- Manual memory management.
  - You can program in such a way as to avoid allocations/deallocations and blocking in the hot path.
- C has memory management too. But C++ as scalable zero-cost abstractions: algorithms, templates.
- Huge body of existing libraries and frameworks. e.g: JUCE(audio), Unreal Engine (gaming).

## 4. C++ techniques for low-latency programming

- Two categories:
  - Efficient programming.
  - Programming for deterministic execution time.
- Most crucial thing: **measuring**.

## 5. Measuring

- You don't know whether something is efficient, fast, etc. Unless you measure it.
- Results often surprising/counter intuitive:
  - Fedor Pikus: The Art of Writing Efficient Programs.
  - Dave Rowland: Optimizing a real-time audio processing library (CppOnSea 2023).
  - C++ Weekly, Episode 274: Why is My pair 310x Faster than `std::pair`?

34:42
 