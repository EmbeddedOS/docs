# Complete Cyber Security Course: the hackers exposed

## 1. Introduction

### 1.1. Goals & Learning objectives

- Master the fundamental building blocks of security & privacy.
- Understand the online thread and vulnerability landscape.
- Perform thread modeling and risk assessments.
- Determine personal threats and adversaries.
- Build test environment in Virtualbox and VMware.
- Master encryption.
- Understand Windows, MacOS, Linux security & privacy features.
- Be able to mitigate social engineering attacks.
- Use isolation and compartmentalization effectively.

## 4. Encryption Crash Course

### 4.1. Goals

### 4.2. Symmetric Encryption

What exactly encryption? transform a readable data into unreadable. So the data can be storage or transmission unreadable.

```text
Plain text message --Encryption--> cipher text message --decryption --> back to plain text.
```

Symmetric Encryption include 2 parts:

- Algorithm: Public. like a padlock.
- Key: Secret. the key is the key for that padlock.

The key and algorithm determine how the plain text will be encrypted.

AES = Symmetric Encryption Algorithm (Use | key).

256-bit AES, 128-bit AES, etc indicate the bit length and the key space. bigger space -> stronger algorithm but slower to encrypt and decrypt.

Some Symmetric Encryption Algorithms:

- Data Encryption Standard (DES).
- Triple-DES (3DES).
- Blowfish.
- RC4.
- RC5.
- RC6.
- Advanced Encryption Standard(AES).

### 4.3. Asymmetric Encryption

Asymmetric / Public and Private Key.

The point is Symmetric Encryption is very strong, but assume that you send a encrypted file to a person, how can you share the key to them? via email, chat, etc. to risky and not scalable.

Asymmetric = 2 keys (public + private).
Symmetric = only 1 key (private).

Some Asymmetric Algorithms:

- Rivest-Shamir-Adleman (RSA).
- Elliptic curve cryptosystem (ECC).
- Diffie-Hellman(DH).
- El Gammal.

If you encrypt with the private key you need the public key to decrypt.
If you encrypt with the public key you need the private key to decrypt.

```text
person 1 -plain_text--> encryption --encrypted_text--> decryption --plain_text--> person 2.
                         /\                              /\
                         ||                              ||
                    Using person 2 public           Using person 2 private key
                    key to encrypt                  to decrypt

```

The sender encrypts with receivers public key.

But still have one more problem, we don't know who actually send that, because every one know person 2 public key can encrypt text and send to person 2.

The second way is sender encrypts with senders private key, and receiver use sender's public key to encrypt.

```text
person 1 -plain_text--> encryption --encrypted_text--> decryption --plain_text--> person 2.
                         /\                              /\
                         ||                              ||
                    Using person 1 private           Using person 1 public key
                    key to encrypt                  to decrypt

```

So now the receiver know exactly the sender sending.

That also cause other problem, now anyone who know sender public key can decrypt that message.
We will solve this problem in the few next section.

Some comparison bw asymmetric and symmetric:

- 1024-bit RSA keys are equivalent in strength to 80-bit symmetric keys.
- 2048-bit RSA keys are equivalent in strength to 112-bit symmetric keys.
- 3072-bit RSA keys are equivalent in strength to 128-bit symmetric keys.
- 1536-bit RSA keys are equivalent in strength to 256-bit symmetric keys.
