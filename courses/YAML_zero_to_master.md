# YAML Zero to Master

## 1. Introduction to YAML

### 1. YAML Course Overview

### 3. What is an YAML

- YAML is light-weight, human-readable data-serialization language. It is primarily designed to make the format easy to read while including advanced features.
- YAML stands for "YAML Ain't Markup Language".
- It is similar to XML and JSON files but uses a more minimalist syntax even while maintaining similar capabilities.
- YAM files are created with extensions ".yaml" or ".yml". You can use any IDE or text editor to open/create YAML files.
- YAML is similar inline style to JSON (is a superset of JSON).
- Is very easy and simple for represent complex mapping. Due to which it is heavily used in configuration settings.

### 4. XML vs JSON vs YAML

- XML -> JSON -> YAML.
- YAML is a super set of JSON which means all the features of JSON can be found in YAML. For example if you compare with JavaScript and TypeScript will be YAML and JavaScript will be JSON.

- json:

```json
{
    "applications": [
        {
            "name": "Accounts",
            "technology": "Python"
        },
        {
            "name": "Cards",
            "technology": "Java"
        },
        {
            "name": "Loans",
            "technology": "Ruby"
        }
    ]
}
```

- yaml:

```yaml
applications:
- name: Accounts
  technology: Python
- name: Cards
  technology: Java
- name: Loans
  technology: Ruby
```

|              |XML|JSON|YAML|
|--------------|---|----|----|
|Human-readable|Harder to read|Harder to read|Easier to read and understand|
|Syntax        | More verbose| Explicit, strict syntax requirements|Minimalist syntax|
|Comments      | Allows comments| Not Allows | Allows|
|Hierarchy     | Hierarchy is denoted by using open & close tags. | using braces and brackets. | using double space characters.|
|storage       |will take lot of storage and network bandwidth|lighter compared to XML.|Lighter compared to XML & JSON|
|use-cases|Best for complex project that require fine control over schema|Web, transmitting data over HTTP|Best for configuration files, along with JSON features.|

### 5. Use-cases

- Since YAML is a human friendly readable format, it is widely used in writing configurations files in different DevOps tools, cloud platforms and applications.

- Few of the most famous tools: docker, kubernetes, prometheus, grafana, jenkins, ansible, github, aws, google cloud platform, spring, etc.

### 6. Writing a simple YAML file and validating with Atom

- By default, for any strings you don't have to explicit mention, double codes `"` or single code `'` inside YAML, like json until you have special characters.

- Json to yaml: [link](https://json2yaml.com)

- Thumb rules in writing yaml:
  - Indentation of whitespace is used to denote structure. This is very similar to Python uses indentation to highlight the blocks of code.
  - The basic structure of a YAM file is a map. You might call this a dictionary, hash or object, depending on your favorite programming language.
  - Tabs are not including as indentation for YAML files. So please careful with space and tab inside yaml files.
  - yaml is case sensitive in nature. [name: accounts != Name: Accounts]

## 2. YAML Basic concepts

### 7. Scalars inside YAML

- Key-value pairs
  - most things in a YAML file are a form of key-value pair. Key-value pairs are the basis for all other YAML constructions.
- Scalars
  - scalars represent a single stored value. Scalars are assigned to key names as values. You define a key with a name, colon, and space, then a value for it to hold.
    - `<key>:<value>` : Space is mandatory between a Key and Value after colon. With out space it will be treated as a String value.
    - example:
      - `name: cong`
      - with `cong` is scalar.

- YAML supports common types like: int, floating-point numeric values, boolean, string.

### 8. Strings in YAML

- strings in yaml doesn't need explicit double or single quotes.
- Use single or double quotes in YAML if your value includes special characters. For example, these special characters may require quotes: { } [ ] , & : * # ? | - < > = ! % @ \
- "Yes" and "No" should be enclosed in quotes (single or double) or else they will be interpreted as True and False boolean values.

- we can use `>` removes newlines within string.
- we can use `|` to turn every newline within the string into a literal newline.

- For example:

```json
{
    "about": "EazyBank \nis an \nBank based \napplication.\n",
    "history": "EazyBank established in 1989.\n"
}
```

is similar to yaml:

```yaml
about: |
  EazyBank
  is an
  Bank based
  application.

history: >
  Eazybank
  established
  in 1989.
```

### 9. comments in yaml

- YAML allows you to add comments to files using the hash symbol (#) similar to Python comments.
- YAML supports only single line comments.

### 10. implicit and explicit typing in yaml

- YAML offers versatility in typing by auto-detecting data types while also support explicit typing options.
- To tag data as a certain type, simply include `!![typeName]` before value.
- For example:
  - `positive-float: !!float 3.14`

### 11. timestamp

- `!!timestamp`
  - A timestamp value represents a single pont in time. it is helpful data type that lets you store times as a unit rather than as a collection of different numbers.
  - For example:
    - `iso8601: !!timestamp 2001-12-14 21:59:43.10 -5`

### 12. sequence/collections in yaml

- Sequences/collections
  - Sequences are values listed in specific order. A sequence starts with a dash `-` and a space. You can think of a sequence as a Python list ot an array in Bash or Perl. They can be defined with either blocks style ot inline flow style.
  - Block style uses spaces to structure the list or array. It is easier to read but is less compact compared fo flow style.
  - In Flow styles we can write sequences inline using square brackets, similar to an array declaration in a programming language like Python, Java or JavaScript. Flow style is more compact but harder to read a a glance.
  - We can always, embed a sequence into another sequence.

- For example:

```yaml
# List sequence in block style
applications:
- Cards
- Loans
- Accounts
departments:
  - Marketing
  - Insurance
  - Security

# List sequence in flow Style
locations: [Hyderabad, NewYork, Berlin, "Paris"]

# Embedded/Nested Sequences
products:
  - Accounts:
    - Savings Account
    - Current Account
  - Loans:
    - Home Loan
    - Car Loan
    - Personal Loan
  - Cards:
    - Credit Card
    - Debit Card
```

To json:

```json
{
  "applications": [
    "Cards",
    "Loans",
    "Accounts"
  ],
  "departments": [
    "Marketing",
    "Insurance",
    "Security"
  ],
  "locations": [
    "Hyderabad",
    "NewYork",
    "Berlin",
    "Paris"
  ],
  "products": [
    {
      "Accounts": [
        "Savings Account",
        "Current Account"
      ]
    },
    {
      "Loans": [
        "Home Loan",
        "Car Loan",
        "Personal Loan"
      ]
    },
    {
      "Cards": [
        "Credit Card",
        "Debit Card"
      ]
    }
  ]
}
```

### 13. Dictionaries inside yaml

- Dictionaries/mapping
  - Dictionaries are collections of key-value pairs all nested under the same subgroup. They are helpful to divide data into logical categories for later use.
  - Dictionaries are defined with a name, a colon, and a space followed by 1 or more indented key-value pairs.
  - Using dictionaries, we can build information related objects like Person, Application, Vehicle, etc.
  - The key-value is YAML's basic building block. Every item in YAML document is a member of at least one dictionary. They key is always a string. The value is a scalar so that it can be any datatype. So, as we have already seen, the value can be a string, a number, or another dictionary.

### 14. Common structure of yaml

- Below is the most common structure of YAML file.

```yaml
applications:
-cards:
  name: EazyCards
  technology: Python
  team-size: 12
- accounts:
  name: EazyAccounts
  technology: Java
  team-size: 8
- loans:
  name: EazyLoans
  technology: Ruby
  team-size: 8
```

## 3. YAML advance concepts

### 15. Complex keys inside YAML

- Complex keys in yaml
- Keys can also be complex inside YAML, like multi-line string. We use `?` followed by a space to indicate the start of a complex key.
- For example:

```yaml
? This is a key 
  that has multiple lines
: and this is its value

? - Development
  - UAT
  - PROD
: - http://dev.eazybank.com
  - http://uat.eazybank.com
  - http://pro.eazybank.com

```

```json
{
  "This is a key that has multiple lines": "and this is its value",
  "[\"Development\", \"UAT\", \"PROD\"]": [
    "http://dev.eazybank.com",
    "http://uat.eazybank.com",
    "http://pro.eazybank.com"
  ]
}
```

### 16. anchors and alias inside YAML

- Anchors and aliases let you identify an item with an anchor in a YAMl document, and then refer to that item with an alias later in the same document. Anchors are identified by an `&` and aliases by an `*` character.
  - The anchor `&` which defines a chunk of values/configuration.
  - The alias `*` used to refer to that chunk elsewhere.

- Anchors & Aliases can be considered if we have repeated sections inside our yaml files. They can reduce effort and make updating in bulk, easier.

For example:

```yaml
- cards: &card
  - name: EazyCards
  - technology: Python
  - team-size: 12
- accounts:
  - name: EazyAccounts
  - technology: Java
  - team-size: 8
- loans: *card
```

```json
[
  {
    "cards": [
      {
        "name": "EazyCards"
      },
      {
        "technology": "Python"
      },
      {
        "team-size": 12
      }
    ]
  },
  {
    "accounts": [
      {
        "name": "EazyAccounts"
      },
      {
        "technology": "Java"
      },
      {
        "team-size": 8
      }
    ]
  },
  {
    "loans": [
      {
        "name": "EazyCards"
      },
      {
        "technology": "Python"
      },
      {
        "team-size": 12
      }
    ]
  }
]
```

### 17. Overriding inside YAML

- You can use overrides with the characters `<<:` before the Alias to add more values, or override existing ones.
- When using overrides mappings are overridden if the new mapping has the same name or is added afterward if different.

### 18. Multi document support inside YAML

- using characters `---` to start a document and stop it with characters `...`
- For example:

```yaml
---
- cards: &card
  - name: EazyCards
  - technology: Python
  - team-size: 12
- accounts:
  - name: EazyAccounts
  - technology: Java
  - team-size: 8
- loans: *card
...

---
- cards: &card
  - name: EazyCards
  - technology: Python
  - team-size: 12
- accounts:
  - name: EazyAccounts
  - technology: Java
  - team-size: 8
- loans: *card
...
```

### 19. validating yaml with yamllint command

```bash
yamllint .
```
