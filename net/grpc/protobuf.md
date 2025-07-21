# Protocol buffer

## 1. Overview

- Are a language-neutral, platform-neutral extensible mechanism for serializing structured data.

- Protobuf vs JSON, Smaller and faster because we just transfer bytes, meanwhile json transfer human-readable data. That saves network bandwidth. And also the CPU to handle JSON messages.

- Define an interface for all applications. Each applications generate that interface to corresponding code and use it. That means each application will have their own adapter to grpc to communicate with others.

```text
|App 1|<->|gRPC adapter| <------------------->|gRPC adapter|<->|App 2|
```

### 1.1. Who use Protobuf

- gRPC.
- Google Cloud.
- Envoy.

### 1.2. How do Protobuf work?

1. Create `.proto` file to define data structure.
2. Generate code using the `protoc` compiler.
3. Compile PB code with your project code.
4. Use PB classes to serialize, share, and deserialize data.

### 1.3. Revision

- proto2.
- proto3.
- edition 2023.

### 1.4. Keep in mind

- It can sometimes be difficult, or even impossible, to change field names after they've been used in production.
- Field names contain dashes.
- Use pluralized names for repeated fields.

### 1.5. Field number

- Each field in a message has a unique numbered tag. These tags are used to identify your fields  in the message binary format, and should not be changed once in use.

## 2. Syntax

## 3. Generating client and server code

This command will generate 2 files .grpc.pb.h and .grpc.pb.c for service classes.

```bash
protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin`
```

###


